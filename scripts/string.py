#!/usr/bin/env python3
# -*- coding: cp437 -*-

CharMap = "charmap.tbl"

SpecialBuffers = {
    "." : ["B0"],
    "BUFFER" : ["FD"],
    "ATTACKER" : ["FD", "0F"],
    "TARGET" : ["FD", "10"],
    "EFFECT_BANK" : ["FD", "11"],
    "SCRIPTING_BANK" : ["FD", "13"],
    "CURRENT_MOVE" : ["FD", "14"],
    "LAST_ITEM" : ["FD", "16"],
    "LAST_ABILITY" : ["FD", "17"],
    "ATTACKER_ABILITY" : ["FD", "18"],
    "TARGET_ABILITY" : ["FD", "19"],
    "SCRIPTING_BANK_ABILITY" : ["FD", "1A"],
    "PLAYER_NAME" : ["FD", "23"],
}

def StringFileConverter(filename):
    stringToWrite = ".thumb\n.text\n.align 2\n\n"
    with open(filename, 'r') as file:
        maxLength = 0
        fillFF = False
        readingState = 0
        
        for line in file:
            line = line.rstrip("\n\r") #Remove only newline characters
            if line == "" or line[:2] == "//": #Ignore blank lines and comment lines
                continue
            
            if readingState == 0: #Only when the file starts
                line = line.strip()
                if line[:6].upper() == "#ORG @" and line[6:] != "":
                    title = line[6:]
                    stringToWrite += ".global " + title + "\n" + title + ":\n"
                    readingState = 1
                elif "MAX_LENGTH" in line and "=" in line:
                    try:
                        maxLength = int(line.split("=")[1])
                    except:
                        print('Error reading max length in line: "' + line + '" in file: "' + filename + '"')
                elif "FILL_FF" in line and "=" in line:
                    try:
                        fillFF = bool(line.split("=")[1])
                    except:
                        print('Error reading FF fill in line: "' + line + '" in file: "' + filename + '"') 
                else:
                    print('Warning! Error with line: "' + line + '" in file: "' + filename + '"')
                    
            elif readingState == 1:
                if line[:6].upper() == "#ORG @" and line[6:] != "":
                    line = line.strip()
                    title = line[6:]
                    stringToWrite += ".global " + title + "\n" + title + ":\n"
                else:
                    stringToWrite += ProcessString(line, maxLength, fillFF)
                    stringToWrite += "0xFF\n\n" #Only print line in everything went alright

    output = open(filename.split(".string")[0] + '.s', 'w') #Only open file once we know everything went okay.
    output.write(stringToWrite)
    output.close()
    
def ProcessString(string, maxLength = 0, fillWithFF = False):
    charMap = PokeByteTableMaker()
    stringToWrite = ".byte "
    buffer = False
    escapeChar = False
    bufferChars = ""
    strLen = 0

    for char in string:
        if maxLength > 0 and strLen >= maxLength:
            print('Warning: The string "' + string + '" has exceeded the maximum length of ' + str(maxLength) + ' and has been truncated!')
            break
        
        if buffer is True:
            if char == ']':
                buffer = False

                if bufferChars in SpecialBuffers:
                    for bufferChar in SpecialBuffers[bufferChars]:
                        if maxLength > 0 and strLen >= maxLength: #End buffer in middle
                            print('Warning: The string buffer "' + bufferChars + '" has exceeded the maximum length of ' + str(maxLength) + ' and has been truncated!')
                            break
                        
                        stringToWrite += ("0x" + bufferChar + ", ")
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
                print('Error parsing string: "' + string + '"')
                break

        else:
            try:
                stringToWrite += hex(charMap[char]) + ", "
                strLen += 1

            except KeyError:
                if (char == '['):
                    buffer = True
                elif (char == '\\'):
                    escapeChar = True
                elif char == '"':
                    stringToWrite += hex(charMap["\\" + char])
                    strLen += 1
                else:
                    print('Error parsing string: "' + string + '"' + ' at character "' + char + '".')
                    break
    
    if strLen < maxLength and fillWithFF:
        while strLen < maxLength:
            stringToWrite += "0xFF, "
            strLen += 1
    
    return stringToWrite

def PokeByteTableMaker():
    dicty = {}
    with open(CharMap) as file:
            for line in file:
                if line.strip() != "/FF" and line.strip() != "":
                    if (line[2] == '=' and line[3] != ""):
                        try:
                            if line[3] == '\\':
                                dicty[line[3] + line[4]] = int(line.split('=')[0], 16)
                            else:
                                dicty[line[3]] = int(line.split('=')[0], 16)
                        except:
                            pass
            dicty[' '] = 0
    
    dicty["“"] = 0xB0
    dicty["”"] = 0xB1
    return dicty