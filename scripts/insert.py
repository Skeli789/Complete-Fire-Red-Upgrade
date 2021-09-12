#!/usr/bin/env python3

import os
import subprocess
import shutil
import sys
from datetime import datetime
import _io

OFFSET_TO_PUT = 0x900000
SOURCE_ROM = "BPRE0.gba"
ROM_NAME = "test.gba"

if sys.platform.startswith('win'):
    PathVar = os.environ.get('Path')
    Paths = PathVar.split(';')
    PATH = ''
    for candidatePath in Paths:
        if 'devkitARM' in candidatePath:
            PATH = candidatePath
            break
    if PATH == '':
        PATH = 'C://devkitPro//devkitARM//bin'
        if os.path.isdir(PATH) is False:
            print('Devkit not found.')
            sys.exit(1)

    PREFIX = 'arm-none-eabi-'
    OBJDUMP = os.path.join(PATH, PREFIX + 'objdump')
    NM = os.path.join(PATH, PREFIX + 'nm')
    AS = os.path.join(PATH, PREFIX + 'as')

else:  # Linux, OSX, etc.
    PREFIX = 'arm-none-eabi-'
    OBJDUMP = (PREFIX + 'objdump')
    NM = (PREFIX + 'nm')
    AS = (PREFIX + 'as')

OUTPUT = 'build/output.bin'
BYTE_REPLACEMENT = 'bytereplacement'
HOOKS = 'hooks'
REPOINTS = 'repoints'
GENERATED_REPOINTS = 'generatedrepoints'
REPOINT_ALL = 'repointall'
ROUTINE_POINTERS = 'routinepointers'
FUNCTION_REWRITES = 'functionrewrites'
EVENT_SCRIPTS = "eventscripts"
SONGS = "songs"
SPECIAL_INSERTS = 'special_inserts.asm'
SPECIAL_INSERTS_OUT = 'build/special_inserts.bin'


def ExtractPointer(byteList: [bytes]):
    pointer = 0
    for a in range(len(byteList)):
        pointer += (int(byteList[a])) << (8 * a)

    return pointer


def GetTextSection() -> int:
    try:
        # Dump sections
        out = subprocess.check_output([OBJDUMP, '-t', 'build/linked.o'])
        lines = out.decode().split('\n')

        # Find text section
        text = filter(lambda x: x.strip().endswith('.text'), lines)
        section = (list(text))[0]

        # Get the offset
        offset = int(section.split(' ')[0], 16)
        return offset

    except:
        print("Error: The insertion process could not be completed.\n"
              + "The linker symbol file was not found. Most likely the compilation process was not completed.")
        sys.exit(1)


def GetSymbols(subtract=0) -> {str: int}:
    out = subprocess.check_output([NM, 'build/linked.o'])
    lines = out.decode().split('\n')

    ret = {}
    for line in lines:
        parts = line.strip().split()

        if len(parts) < 3:
            continue

        if parts[1].lower() not in {'t', 'd'}:
            continue

        offset = int(parts[0], 16)
        ret[parts[2]] = offset - subtract

    return ret


def Hook(rom: _io.BufferedReader, space: int, hookAt: int, register=0):
    # Align 2
    if hookAt & 1:
        hookAt -= 1

    rom.seek(hookAt)

    register &= 7

    if hookAt % 4:
        data = bytes([0x01, 0x48 | register, 0x00 | (register << 3), 0x47, 0x0, 0x0])
    else:
        data = bytes([0x00, 0x48 | register, 0x00 | (register << 3), 0x47])

    space += 0x08000001
    data += (space.to_bytes(4, 'little'))
    rom.write(bytes(data))


def FunctionWrap(rom: _io.BufferedReader, space: int, hookAt: int, numParams: int, isReturning: int):
    # Align 2
    if hookAt & 1:
        hookAt -= 1

    rom.seek(hookAt)
    numParams = numParams - 1

    if numParams < 4:
        data = bytes([0x10, 0xB5, 0x3, 0x4C, 0x0, 0xF0, 0x3, 0xF8, 0x10, 0xBC,
                      (isReturning + 1), 0xBC, (isReturning << 3), 0x47, 0x20, 0x47])
    else:
        k = numParams - 3
        data = bytes([0x10, 0xB5, 0x82, 0xB0])
        for i in range(k + 2):
            data += bytes([i + 2, 0x9C, i, 0x94])
        data += bytes([0x0, 0x9C, numParams - 1, 0x94, 0x1, 0x9C, numParams, 0x94, 0x2, 0xB0, k + 8, 0x4C,
                       0x0, 0xF0, (k << 1) + 13, 0xF8, 0x82, 0xB0, numParams, 0x9C, 0x1, 0x94, numParams - 1,
                       0x9C, 0x0, 0x94])

        for i in reversed(range(k + 2)):
            data += bytes([i, 0x9C, i+2, 0x94])
        data += bytes([0x2, 0xB0, 0x10, 0xBC, isReturning + 1, 0xBC, isReturning << 3, 0x47, 0x20, 0x47])

    space += 0x08000001
    data += (space.to_bytes(4, 'little'))
    rom.write(bytes(data))


def Repoint(rom: _io.BufferedReader, space: int, repointAt: int, slideFactor=0):
    rom.seek(repointAt)

    space += (0x08000000 + slideFactor)
    data = (space.to_bytes(4, 'little'))
    rom.write(bytes(data))


# These offsets contain the word 0x8900000 - the attack data from
# Mr. DS's rombase. In order to maintain as much compatibility as
# possible, the data at these offsets is never modified.
IGNORED_OFFSETS = {0x3986C0: True, 0x3986EC: True, 0xDABDF0: True}


def RealRepoint(sourceRom: _io.BufferedReader, targetRom: _io.BufferedReader, offsetTuples: [(int, int, str)], endInsertOffset):
    pointerList = []
    pointerDict = {}
    for tup in offsetTuples:  # Format is (Double Pointer, New Pointer, Symbol)
        offset = tup[0]
        sourceRom.seek(offset)
        pointer = ExtractPointer(sourceRom.read(4))
        pointerList.append(pointer)
        pointerDict[pointer] = (tup[1] + 0x08000000, tup[2])

    offset = 0
    offsetList = []

    while offset < 0xFFFFFD:
        if offset in IGNORED_OFFSETS:
            offset += 4
            continue
        elif OFFSET_TO_PUT <= offset < endInsertOffset:  # Skip insert area
            offset = endInsertOffset
            while offset % 4 != 0:  # End insert offset is not divisible by 4
                offset += 1  # Make divisible by 4
            continue

        sourceRom.seek(offset)
        word = ExtractPointer(sourceRom.read(4))
        targetRom.seek(offset)

        for pointer in pointerList:
            if word == pointer:
                offsetList.append((offset, pointerDict[pointer][1]))
                targetRom.write(bytes(pointerDict[pointer][0].to_bytes(4, 'little')))
                break

        offset += 4

    return offsetList


def ReplaceBytes(rom: _io.BufferedReader, offset: int, data: str):
    ar = offset
    words = data.split()
    for i in range(0, len(words)):
        rom.seek(ar)
        intByte = int(words[i], 16)
        rom.write(bytes(intByte.to_bytes(1, 'big')))
        ar += 1


def TryProcessFileInclusion(line: str, definesDict: dict) -> bool:
    if line.startswith('#include "'):
        try:
            path = line.split('"')[1].strip()
            with open(path, 'r') as file:
                for line in file:
                    if line.startswith('#define '):
                        try:
                            lineList = line.strip().split()
                            title = lineList[1]

                            if len(lineList) == 2 or lineList[2].startswith('//') or lineList[2].startswith('/*'):
                                define = True
                            else:
                                define = lineList[2]

                            definesDict[title] = define
                        except IndexError:
                            print('Error reading define on line"' + line.strip() + '" in file "' + path + '".')

        except Exception as e:
            print('Error including file on line "' + line.strip() + '".')
            print(e)

        return True  # Inclusion line; don't read otherwise

    return False


def TryProcessConditionalCompilation(line: str, definesDict: dict, conditionals: [(str, bool)]) -> bool:
    line = line.strip()
    upperLine = line.upper()
    numWordsOnLine = len(line.split())

    if upperLine.startswith('#IFDEF ') and numWordsOnLine > 1:
        condition = line.strip().split()[1]
        conditionals.insert(0, (condition, True))  # Insert at front
        return True
    elif upperLine.startswith('#IFNDEF ') and numWordsOnLine > 1:
        condition = line.strip().split()[1]
        conditionals.insert(0, (condition, False))  # Insert at front
        return True
    elif upperLine == '#ELSE':
        if len(conditionals) >= 1:  # At least one statement was pushed before
            condition = conditionals.pop(0)
            if condition[1] is True:
                conditionals.insert(0, (condition[0], False))  # Invert old statement
            else:
                conditionals.insert(0, (condition[0], True))  # Invert old statement
            return True
    elif upperLine == '#ENDIF':
        conditionals.pop(0)  # Remove first element (last pushed)
        return True
    else:
        for condition in conditionals:
            definedType = condition[1]
            condition = condition[0]

            if definedType is True:  # From #ifdef
                if condition not in definesDict:
                    return True  # If something isn't defined then skip the line
            else:  # From #ifndef
                if condition in definesDict:
                    return True  # If something is defined then skip the line

    return False


def main():
    startTime = datetime.now()

    try:
        shutil.copyfile(SOURCE_ROM, ROM_NAME)
    except FileNotFoundError:
        print('Error: Insertion could not be completed.\n'
              + 'Could not find source rom: "' + SOURCE_ROM + '".\n'
              + 'Please make sure a rom with this name exists in the root.')
        sys.exit(1)
    except PermissionError:
        print('Error: Insertion could not be completed.\n'
              + '"' + ROM_NAME + '" is currently in use by another application.'
              + '\nPlease free it up before trying again.')
        sys.exit(1)

    with open(ROM_NAME, 'rb+') as rom:
        print("Inserting code.")
        table = GetSymbols(GetTextSection())
        rom.seek(OFFSET_TO_PUT)
        with open(OUTPUT, 'rb') as binary:
            endInsertOffset = OFFSET_TO_PUT + os.path.getsize(OUTPUT)
            rom.write(binary.read())
            binary.close()

        # Adjust symbol table
        for entry in table:
            table[entry] += OFFSET_TO_PUT

        # Deal with en masse repoints
        symbolsRepointed = set()
        if os.path.isfile(GENERATED_REPOINTS):
            with open(GENERATED_REPOINTS, 'r') as repointList:
                for line in repointList:
                    if line.strip().startswith('#') or line.strip() == '':
                        continue

                    symbol, address = line.split()
                    offset = int(address)
                    try:
                        code = table[symbol]
                    except KeyError:
                        print('Symbol missing:', symbol)
                        continue

                    symbolsRepointed.add(symbol)
                    Repoint(rom, code, offset)

        else:
            with open(GENERATED_REPOINTS, 'w') as repointList:
                repointList.write('##This is a generated file at runtime. Do not modify it!\n')

        if os.path.isfile(REPOINT_ALL):
            offsetsToRepointTogether = []
            with open(REPOINT_ALL, 'r') as repointList:
                definesDict = {}
                conditionals = []
                for line in repointList:
                    if TryProcessFileInclusion(line, definesDict):
                        continue
                    if TryProcessConditionalCompilation(line, definesDict, conditionals):
                        continue
                    if line.strip().startswith('#') or line.strip() == '':
                        continue

                    symbol, address = line.split()
                    offset = int(address, 16) - 0x08000000

                    if symbol in symbolsRepointed:
                        continue

                    try:
                        code = table[symbol]
                    except KeyError:
                        print('Symbol missing:', symbol)
                        continue
                    offsetsToRepointTogether.append((offset, code, symbol))

                if offsetsToRepointTogether != []:
                    with open(SOURCE_ROM, 'rb') as sourceRom: # Repoint from source rom so new data doesn't accidentally get repointed
                        offsets = RealRepoint(sourceRom, rom, offsetsToRepointTogether, endInsertOffset) # Format is [(offset, symbol), ...]

                        output = open(GENERATED_REPOINTS, 'a')
                        for tup in offsets:
                            output.write(tup[1] + ' ' + str(tup[0]) + '\n')
                        output.close()

        # Do Special Inserts - Before bytereplacement!
        if os.path.isfile(SPECIAL_INSERTS) and os.path.isfile(SPECIAL_INSERTS_OUT):
            with open(SPECIAL_INSERTS, 'r') as file:
                offsetList = []
                for line in file:
                    if line.strip().startswith('.org '):
                        offsetList.append(int(line.split('.org ')[1].split(',')[0], 16))

                offsetList.sort()

            with open(SPECIAL_INSERTS_OUT, 'rb') as binFile:
                for offset in offsetList:
                    originalOffset = offset
                    dataList = ""

                    if offsetList.index(offset) == len(offsetList) - 1:
                        while True:
                            try:
                                binFile.seek(offset)
                                dataList += hex(binFile.read(1)[0]) + ' '
                            except IndexError:
                                break

                            offset += 1
                    else:
                        binFile.seek(offset)
                        word = ExtractPointer(binFile.read(4))

                        while word != 0xFFFFFFFF:
                            binFile.seek(offset)
                            dataList += hex(binFile.read(1)[0]) + ' '
                            offset += 1

                            if offset in offsetList:  # Overlapping data
                                break

                            word = ExtractPointer(binFile.read(4))

                    ReplaceBytes(rom, originalOffset, dataList.strip())

        # Insert byte changes
        if os.path.isfile(BYTE_REPLACEMENT):
            with open(BYTE_REPLACEMENT, 'r') as replacelist:
                definesDict = {}
                conditionals = []
                for line in replacelist:
                    if TryProcessFileInclusion(line, definesDict):
                        continue
                    if TryProcessConditionalCompilation(line, definesDict, conditionals):
                        continue
                    if line.strip().startswith('#') or line.strip() == '':
                        continue
 
                    offset = int(line[:8], 16) - 0x08000000
                    try:
                        ReplaceBytes(rom, offset, line[9:].strip())
                    except ValueError: #Try loading from the defines dict if unrecognizable character
                        newNumber = definesDict[line[9:].strip()]
                        try:
                            newNumber = int(newNumber)
                        except ValueError:
                            newNumber = int(newNumber, 16)

                        newNumber = str(hex(newNumber)).split('0x')[1]
                        ReplaceBytes(rom, offset, newNumber) 

        # Read hooks from a file
        if os.path.isfile(HOOKS):
            with open(HOOKS, 'r') as hookList:
                definesDict = {}
                conditionals = []
                for line in hookList:
                    if TryProcessFileInclusion(line, definesDict):
                        continue
                    if TryProcessConditionalCompilation(line, definesDict, conditionals):
                        continue
                    if line.strip().startswith('#') or line.strip() == '':
                        continue

                    symbol, address, register = line.split()
                    offset = int(address, 16) - 0x08000000
                    try:
                        code = table[symbol]
                    except KeyError:
                        print('Symbol missing:', symbol)
                        continue

                    Hook(rom, code, offset, int(register))

        # Read repoints from a file
        if os.path.isfile(REPOINTS):
            with open(REPOINTS, 'r') as repointList:
                definesDict = {}
                conditionals = []
                for line in repointList:
                    if TryProcessFileInclusion(line, definesDict):
                        continue
                    if TryProcessConditionalCompilation(line, definesDict, conditionals):
                        continue
                    if line.strip().startswith('#') or line.strip() == '':
                        continue

                    if len(line.split()) == 2:
                        symbol, address = line.split()
                        offset = int(address, 16) - 0x08000000
                        try:
                            code = table[symbol]
                        except KeyError:
                            print('Symbol missing:', symbol)
                            continue

                        Repoint(rom, code, offset)

                    if len(line.split()) == 3:
                        symbol, address, slide = line.split()
                        offset = int(address, 16) - 0x08000000
                        try:
                            code = table[symbol]
                        except KeyError:
                            print('Symbol missing:', symbol)
                            continue

                        Repoint(rom, code, offset, int(slide))

        # Read routine repoints from a file
        if os.path.isfile(ROUTINE_POINTERS):
            with open(ROUTINE_POINTERS, 'r') as pointerlist:
                definesDict = {}
                conditionals = []
                for line in pointerlist:
                    if TryProcessFileInclusion(line, definesDict):
                        continue
                    if TryProcessConditionalCompilation(line, definesDict, conditionals):
                        continue
                    if line.strip().startswith('#') or line.strip() == '':
                        continue

                    symbol, address = line.split()
                    offset = int(address, 16) - 0x08000000
                    try:
                        code = table[symbol]
                    except KeyError:
                        print('Symbol missing:', symbol)
                        continue

                    Repoint(rom, code, offset, 1)

        # Read routine rewrite wrapper from a file
        if os.path.isfile(FUNCTION_REWRITES):
            with open(FUNCTION_REWRITES, 'r') as frwlist:
                definesDict = {}
                conditionals = []
                for line in frwlist:
                    if TryProcessFileInclusion(line, definesDict):
                        continue
                    if TryProcessConditionalCompilation(line, definesDict, conditionals):
                        continue
                    if line.strip().startswith('#') or line.strip() == '':
                        continue

                    symbol, address, numParams, isReturning = line.split()
                    offset = int(address, 16) - 0x08000000
                    try:
                        code = table[symbol]
                    except KeyError:
                        print('Symbol missing:', symbol)
                        continue

                    FunctionWrap(rom, code, offset, int(numParams), int(isReturning))

        # Insert Event Scripts
        if os.path.isfile(EVENT_SCRIPTS):
            definesDict = {}

            mapHeaders = {}  # For signpost events
            npcTables = {}  # For people events
            tileTables = {}  # For script tiles
            signTables = {}  # For signpost events

            npcCounts = {}  # For people events
            tileCounts = {}  # For script tiles
            signCounts = {}  # For signpost events

            conditionals = []
            rom.seek(0x5524C)
            mapBanksHeader = ExtractPointer(rom.read(4)) - 0x08000000

            with open(EVENT_SCRIPTS, 'r') as file:
                for i, line in enumerate(file):
                    if TryProcessFileInclusion(line, definesDict):
                        continue
                    if TryProcessConditionalCompilation(line, definesDict, conditionals):
                        continue
                    if line.strip().startswith('#') or line.strip() == '':
                        continue

                    try:
                        eventId = -1  # Reset just in case of error
                        if len(line.split()) == 4 or len(line.split()) == 5:
                            if len(line.split()) == 5:
                                eventType, mapBank, mapNum, eventId, symbol = line.split()
                                eventId = int(eventId)
                            else:  # 4
                                eventType, mapBank, mapNum, symbol = line.split()

                            eventType = eventType.lower()
                            mapBank = int(mapBank)
                            mapNum = int(mapNum)
                            dictId = (mapBank << 8) | mapNum

                            if dictId not in mapHeaders:
                                rom.seek(mapBanksHeader + mapBank * 4)
                                mapBankHeader = ExtractPointer(rom.read(4)) - 0x08000000
                                if mapBankHeader == (0xF7F7F7F7 - 0x08000000):
                                    continue  # Garbage map bank header
                                rom.seek(mapBankHeader + mapNum * 4)
                                mapHeader = ExtractPointer(rom.read(4)) - 0x08000000
                                if mapHeader == (0xF7F7F7F7 - 0x08000000):
                                    continue  # Garbage map header
                                mapHeaders[dictId] = mapHeader  # Store for later
                            else:
                                mapHeader = mapHeaders[dictId]

                            if eventType == "map":
                                offset = mapHeader + 0x8
                            elif eventType == "npc" or eventType == "trainer" or eventType == "item":
                                if eventId < 0:
                                    raise(OSError)
                                if dictId not in npcTables:
                                    rom.seek(mapHeader + 0x4)
                                    eventHeader = ExtractPointer(rom.read(4)) - 0x08000000
                                    rom.seek(eventHeader)
                                    npcCount = int(rom.read(1)[0])
                                    rom.seek(eventHeader + 0x4)
                                    npcTable = ExtractPointer(rom.read(4)) - 0x08000000
                                    npcTables[dictId] = npcTable  # Store for later
                                    npcCounts[dictId] = npcCount  # Store for later
                                else:
                                    npcTable = npcTables[dictId]
                                    npcCount = npcCounts[dictId]

                                # Check if valid npc
                                if eventId >= npcCount:
                                    print("Errror! NPC id {} exceeds the count of {} on line {}: {}".format(eventId, npcCount, i, line.strip()))
                                    continue

                                # Check shortcut npcs and modify symbol
                                if eventType == "trainer":
                                    symbol = "EventScript_" + symbol
                                elif eventType == "item":
                                    symbol = "ItemFindScript_" + symbol

                                length = 0x18  # Length of one entry
                                offset = npcTable + eventId * 0x18 + 0x10

                            elif eventType == "tile":
                                if eventId < 0:
                                    raise(OSError)
                                if dictId not in tileTables:
                                    rom.seek(mapHeader + 0x4)
                                    eventHeader = ExtractPointer(rom.read(4)) - 0x08000000
                                    rom.seek(eventHeader + 0xC)
                                    tileTable = ExtractPointer(rom.read(4)) - 0x08000000
                                    tileTables[dictId] = tileTable  # Store for later
                                else:
                                    tileTable = tileTables[dictId]
                                length = 0x10  # Length of one entry
                                offset = tileTable + eventId * length + 0xC

                            elif eventType == "sign":
                                if eventId < 0:
                                    raise(OSError)
                                if dictId not in signTables:
                                    rom.seek(mapHeader + 0x4)
                                    eventHeader = ExtractPointer(rom.read(4)) - 0x08000000
                                    rom.seek(eventHeader + 0x10)
                                    signTable = ExtractPointer(rom.read(4)) - 0x08000000
                                    signTables[dictId] = signTable  # Store for later
                                else:
                                    signTable = signTables[dictId]
                                length = 0xC  # Length of one entry
                                offset = signTable + eventId * length + 0x8

                            else:
                                print("Unknown event type \"{}\"!".format(eventType))
                                continue

                            if symbol in definesDict:
                                symbol = definesDict[symbol]

                            try:
                                code = table[symbol]
                            except KeyError:
                                try:
                                    code = int(symbol, 16)  # If script offset was written in hex
                                except ValueError:
                                    print('Symbol missing:', symbol)
                                    continue

                            Repoint(rom, code, offset)
                    except OSError:
                        print("There was an error inserting the event script on line {}: {}".format(i, line.strip()))

        # Insert Song Pointers
        if os.path.isfile(SONGS):
            rom.seek(0x1DD11C)  # m4aSongNumStart
            songTable = ExtractPointer(rom.read(4)) - 0x08000000

            with open(SONGS, "r") as file:
                for i, line in enumerate(file):
                    if TryProcessFileInclusion(line, definesDict):
                        continue
                    if TryProcessConditionalCompilation(line, definesDict, conditionals):
                        continue
                    if line.strip().startswith('#') or line.strip() == '':
                        continue

                    try:
                        lineList = line.split()
                        try:
                            songId = int(lineList[0])
                        except ValueError:
                            songId = int(lineList[0], 16)  # Hex
                        song = lineList[1]
                        offset = songTable + songId * 8

                        try:
                                code = table[song]
                        except KeyError:
                            try:
                                code = int(song, 16)  # If script offset was written in hex
                            except ValueError:
                                print('Symbol missing:', song)
                                continue

                        Repoint(rom, code, offset)
                    except:
                        print("There was an error inserting the song on line {}: {}".format(i, line.strip()))

        width = max(map(len, table.keys())) + 1
        if os.path.isfile('offsets.ini'):
            offsetIni = open('offsets.ini', 'r+')
        else:
            offsetIni = open('offsets.ini', 'w')

        offsetIni.truncate()
        for key in sorted(table.keys()):
            fstr = ('{:' + str(width) + '} {:08X}')
            offsetIni.write(fstr.format(key + ':', table[key] + 0x08000000) + '\n')
        offsetIni.close()

        print('Inserted in ' + str(datetime.now() - startTime) + '.')


if __name__ == '__main__':
    main()
