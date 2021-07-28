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
    "TRAINER1_CLASS": ["FD", "1C"],
    "TRAINER1_NAME": ["FD", "1D"],
    "PLAYER_NAME": ["FD", "23"],
    "TARGET_NAME": ["FD", "3B"],
    "TARGET_PARTNER_NAME": ["FD", "3C"],
    "DEF_PREFIX_5": ["FD", "3D"],

    "PLAYER": ["FD", "01"],
    "BUFFER1": ["FD", "02"],
    "BUFFER2": ["FD", "03"],
    "BUFFER3": ["FD", "04"],
    "RIVAL": ["FD", "06"],

    "ARROW_UP": ["79"],
    "ARROW_DOWN": ["7A"],
    "ARROW_LEFT": ["7B"],
    "ARROW_RIGHT": ["7C"],

    "MAIN_COLOUR": ["FC", "01"],  # Takes 1 byte
    "WHITE": ["FC", "01", "01"],
    "BLACK": ["FC", "01", "02"],
    "GRAY": ["FC", "01", "03"],
    "RED": ["FC", "01", "04"],
    "ORANGE": ["FC", "01", "05"],
    "GREEN": ["FC", "01", "06"],
    "LIGHT_GREEN": ["FC", "01", "07"],
    "BLUE": ["FC", "01", "08"],
    "LIGHT_BLUE": ["FC", "01", "09"],
    "HIGHLIGHT": ["FC", "02"],  # Same as MAIN_COLOUR
    "SHADOW_COLOUR": ["FC", "03"],
    "COLOR_HIGHLIGHT_SHADOW": ["FC", "04"],  # Takes 3 bytes
    "PALETTE": ["FC", "05"],
    "SHRINK": ["FC", "06", "00"],
    "RESET_SIZE": ["FC", "07"],
    "PAUSE": ["FC", "08"],
    "PAUSE_UNTIL_PRESS": ["FC", "09"],
    "WAIT_SE": ["FC", "0A"],
    "PLAY_BGM": ["FC", "0B"],
    "ESCAPE": ["FC", "0C"],
    "SHIFT_RIGHT": ["FC", "0D"],
    "SHIFT_DOWN": ["FC", "0E"],
    "FILL_WINDOW": ["FC", "0F"],
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

    "UP_ARROW_2": ["F9", "00"],
    "DOWN_ARROW_2": ["F9", "01"],
    "LEFT_ARROW_2": ["F9", "02"],
    "RIGHT_ARROW_2": ["F9", "03"],
    "PLUS": ["F9", "04"],
    "LV_2": ["F9", "05"],
    "PP": ["F9", "06"],
    "ID": ["F9", "07"],
    "NO": ["F9", "08"],
    "UNDERSCORE": ["F9", "09"],
    "CIRCLE_1": ["F9", "0A"],
    "CIRCLE_2": ["F9", "0B"],
    "CIRCLE_3": ["F9", "0C"],
    "CIRCLE_4": ["F9", "0D"],
    "CIRCLE_5": ["F9", "0E"],
    "CIRCLE_6": ["F9", "0F"],
    "CIRCLE_7": ["F9", "10"],
    "CIRCLE_8": ["F9", "11"],
    "CIRCLE_9": ["F9", "12"],
    "LEFT_PAREN": ["F9", "13"],
    "RIGHT_PAREN": ["F9", "14"],
    "CIRCLE_DOT": ["F9", "15"],
    "TRIANGLE": ["F9", "16"],
    "BIG_MULT_X": ["F9", "17"],

    "EMOJI_UNDERSCORE": ["F9", "D0"],
    "EMOJI_PIPE": ["F9", "D1"],
    "EMOJI_HIGHBAR": ["F9", "D2"],
    "EMOJI_TILDE": ["F9", "D3"],
    "EMOJI_LEFT_PAREN": ["F9", "D4"],
    "EMOJI_RIGHT_PAREN": ["F9", "D5"],
    "EMOJI_UNION": ["F9", "D6"],
    "EMOJI_GREATER_THAN": ["F9", "D7"],
    "EMOJI_LEFT_EYE": ["F9", "D8"],
    "EMOJI_RIGHT_EYE": ["F9", "D9"],
    "EMOJI_AT": ["F9", "DA"],
    "EMOJI_SEMICOLON": ["F9", "DB"],
    "EMOJI_PLUS": ["F9", "DC"],
    "EMOJI_MINUS": ["F9", "DD"],
    "EMOJI_EQUALS": ["F9", "DE"],
    "EMOJI_SPIRAL": ["F9", "DF"],
    "EMOJI_TONGUE": ["F9", "E0"],
    "EMOJI_TRIANGLE_OUTLINE": ["F9", "E1"],
    "EMOJI_ACUTE": ["F9", "E2"],
    "EMOJI_GRAVE": ["F9", "E3"],
    "EMOJI_CIRCLE": ["F9", "E4"],
    "EMOJI_TRIANGLE": ["F9", "E5"],
    "EMOJI_SQUARE": ["F9", "E6"],
    "EMOJI_HEART": ["F9", "E7"],
    "EMOJI_MOON": ["F9", "E8"],
    "EMOJI_NOTE": ["F9", "E9"],
    "EMOJI_BALL": ["F9", "EA"],
    "EMOJI_BOLT": ["F9", "EB"],
    "EMOJI_LEAF": ["F9", "EC"],
    "EMOJI_FIRE": ["F9", "ED"],
    "EMOJI_WATER": ["F9", "EE"],
    "EMOJI_LEFT_FIST": ["F9", "EF"],
    "EMOJI_RIGHT_FIST": ["F9", "F0"],
    "EMOJI_BIGWHEEL": ["F9", "F1"],
    "EMOJI_SMALLWHEEL": ["F9", "F2"],
    "EMOJI_SPHERE": ["F9", "F3"],
    "EMOJI_IRRITATED": ["F9", "F4"],
    "EMOJI_MISCHIEVOUS": ["F9", "F5"],
    "EMOJI_HAPPY": ["F9", "F6"],
    "EMOJI_ANGRY": ["F9", "F7"],
    "EMOJI_SURPRISED": ["F9", "F8"],
    "EMOJI_BIGSMILE": ["F9", "F9"],
    "EMOJI_EVIL": ["F9", "FA"],
    "EMOJI_TIRED": ["F9", "FB"],
    "EMOJI_NEUTRAL": ["F9", "FC"],
    "EMOJI_SHOCKED": ["F9", "FD"],
    "EMOJI_BIGANGER": ["F9", "FE"],
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
