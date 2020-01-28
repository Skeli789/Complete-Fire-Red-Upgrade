#!/usr/bin/env python3
# -*- coding: cp437 -*-

import sys
from insert import TryProcessFileInclusion, TryProcessConditionalCompilation

CharMap = "charmap.tbl"

SpecialBuffers = {
    ".": ["B0"],
    "BUFFER": ["FD"],
    "ATTACKER": ["FD", "0F"],
    "TARGET": ["FD", "10"],
    "EFFECT_BANK": ["FD", "11"],
    "SCRIPTING_BANK": ["FD", "13"],
    "CURRENT_MOVE": ["FD", "14"],
    "LAST_ITEM": ["FD", "16"],
    "LAST_ABILITY": ["FD", "17"],
    "ATTACKER_ABILITY": ["FD", "18"],
    "TARGET_ABILITY": ["FD", "19"],
    "SCRIPTING_BANK_ABILITY": ["FD", "1A"],
    "PLAYER_NAME": ["FD", "23"],

    "PLAYER": ["FD", "01"],
    "BUFFER1": ["FD", "02"],
    "BUFFER2": ["FD", "03"],
    "BUFFER3": ["FD", "04"],
    "RIVAL": ["FD", "06"],
    "WHITE": ["FC", "01", "01"],
    "BLACK": ["FC", "01", "02"],
    "GRAY": ["FC", "01", "03"],
    "RED": ["FC", "01", "04"],
    "ORANGE": ["FC", "01", "05"],
    "GREEN": ["FC", "01", "06"],
    "LIGHT_GREEN": ["FC", "01", "07"],
    "BLUE": ["FC", "01", "08"],
    "LIGHT_BLUE": ["FC", "01", "09"],

    "ARROW_UP": ["79"],
    "ARROW_DOWN": ["7A"],
    "ARROW_LEFT": ["7B"],
    "ARROW_RIGHT": ["7C"],

    "SHRINK": ["FC", "06", "00"],
    "PAUSE": ["FC", "08"],
    "PAUSE_UNTIL_PRESS": ["FC", "09"],
    "PLAY_BGM": ["FC", "0B"],
    "ESCAPE": ["FC", "0C"],
    "SHIFT_TEXT": ["FC", "0D"],
    "PLAY_SE": ["FC", "10"],
    "CLEAR": ["FC", "11"],
    "SKIP": ["FC", "12"],
    "ALIGN": ["FC", "13"],
    "MIN_LETTER_SPACING": ["FC", "14"],
    "PAUSE_MUSIC": ["FC", "17"],
    "RESUME_MUSIC": ["FC", "18"],

    "A_BUTTON": ["F8", "00"],
    "B_BUTTON": ["F8", "01"],
    "L_BUTTON": ["F8", "02"],
    "R_BUTTON": ["F8", "03"],
    "START_BUTTON": ["F8", "04"],
    "SELECT_BUTTON": ["F8", "05"],
    "DPAD_UP": ["F8", "06"],
    "DPAD_DOWN": ["F8", "07"],
    "DPAD_LEFT": ["F8", "08"],
    "DPAD_RIGHT": ["F8", "09"],
    "DPAD_UP_DOWN": ["F8", "0A"],
    "DPAD_LEFT_RIGHT": ["F8", "0B"],
    "DPAD": ["F8", "0C"],
}


def StringFileConverter(fileName: str):
    stringToWrite = ".thumb\n.text\n.align 2\n\n"
    with open(fileName, 'r', encoding="utf-8") as file:
        maxLength = 0
        fillFF = False
        readingState = 0
        lineNum = 0
        definesDict = {}
        conditionals = []

        for line in file:
            lineNum += 1
            line = line.rstrip("\n\r")  # Remove only newline characters
            if TryProcessFileInclusion(line, definesDict):
                continue
            if TryProcessConditionalCompilation(line, definesDict, conditionals):
                continue
            if line.strip() == "" or line[:2] == "//":  # Ignore blank lines and comment lines
                continue
            
            if readingState == 0:  # Only when the file starts
                line = line.strip()
                if line[:6].upper() == "#ORG @" and line[6:] != "":
                    title = line[6:]
                    stringToWrite += ".global " + title + "\n" + title + ":\n"
                    readingState = 1
                elif "MAX_LENGTH" in line and "=" in line:
                    try:
                        maxLength = int(line.split("=")[1])
                    except:
                        print('Error reading max length on line ' + str(lineNum) + ' in file: "' + fileName + '"')
                        sys.exit(0)
                elif "FILL_FF" in line and "=" in line:
                    try:
                        fillFF = bool(line.split("=")[1])
                    except:
                        print('Error reading FF fill on line ' + str(lineNum) + ' in file: "' + fileName + '"')
                        sys.exit(0)
                else:
                    print('Warning! Error on line ' + str(lineNum) + ' in file: "' + fileName + '"')
                    
            elif readingState == 1:
                if line[:6].upper() == "#ORG @" and line[6:] != "":
                    line = line.strip()
                    title = line[6:]
                    stringToWrite += ".global " + title + "\n" + title + ":\n"
                else:
                    stringToWrite += ProcessString(line, lineNum, maxLength, fillFF)
                    stringToWrite += "0xFF\n\n"  # Only print line in everything went alright

    output = open(fileName.split(".string")[0] + '.s', 'w')  # Only open file once we know everything went okay.
    output.write(stringToWrite)
    output.close()


def ProcessString(string: str, lineNum: int, maxLength=0, fillWithFF=False) -> str:
    charMap = PokeByteTableMaker()
    stringToWrite = ".byte "
    buffer = False
    escapeChar = False
    bufferChars = ""
    strLen = 0

    for char in string:
        if 0 < maxLength <= strLen:
            print('Warning: The string "' + string + '" has exceeded the maximum length of '
                  + str(maxLength) + ' and has been truncated!')
            break

        if buffer is True:
            if char == ']':
                buffer = False

                if bufferChars in SpecialBuffers:
                    for bufferChar in SpecialBuffers[bufferChars]:
                        if 0 < maxLength <= strLen:  # End buffer in middle
                            print('Warning: The string buffer "' + bufferChars + '" has exceeded the maximum length of '
                                  + str(maxLength) + ' and has been truncated!')
                            break

                        stringToWrite += ("0x" + bufferChar + ", ")
                        strLen += 1

                elif len(bufferChars) > 2:  # Unrecognized buffer
                    print('Warning: The string buffer "' + bufferChars + '" is not recognized!')
                    stringToWrite += "0x0, "  # Place whitespace where the buffer should have gone
                    strLen += 1
                else:
                    stringToWrite += ("0x" + bufferChars + ", ")
                    strLen += 1

                bufferChars = ""
            else:
                bufferChars += char

        elif escapeChar is True:
            escapeChar = False
            try:
                stringToWrite += hex(charMap["\\" + char]) + ", "
                strLen += 1

            except KeyError:
                print('Error parsing string: "' + string + '" (Line ' + str(lineNum) + ')')
                sys.exit(0)

        else:
            try:
                stringToWrite += hex(charMap[char]) + ", "
                strLen += 1

            except KeyError:
                if char == '[':
                    buffer = True
                elif char == '\\':
                    escapeChar = True
                elif char == '"':
                    stringToWrite += hex(charMap["\\" + char])
                    strLen += 1
                else:
                    print('Error parsing string on line ' + str(lineNum) + ' at character "' + char + '".')
                    sys.exit(1)
    
    if strLen < maxLength and fillWithFF:
        while strLen < maxLength:
            stringToWrite += "0xFF, "
            strLen += 1

    return stringToWrite


def PokeByteTableMaker():
    dictionary = {}
    with open(CharMap, 'r', encoding="utf-8") as file:
        for line in file:
            if line.strip() != "/FF" and line.strip() != "":
                if line[2] == '=' and line[3] != "":
                    try:
                        if line[3] == '\\':
                            dictionary[line[3] + line[4]] = int(line.split('=')[0], 16)
                        else:
                            dictionary[line[3]] = int(line.split('=')[0], 16)
                    except:
                        pass
        dictionary[' '] = 0
    return dictionary
