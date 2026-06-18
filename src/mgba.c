/*
 mgba.h
 Copyright (c) 2016 Jeffrey Pfau
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:
  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.
 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdarg.h>
#include "../include/gba/types.h"
#include "../include/gba/defines.h"
#include "../include/mgba.h"
#include "../include/mini_printf.h"
#include "../include/text.h"
#include "../include/string_util.h"
#include "../include/malloc.h"

#define REG_DEBUG_ENABLE ((vu16*) (0x4FFF780))
#define REG_DEBUG_FLAGS  ((vu16*) (0x4FFF700))
#define REG_DEBUG_STRING ((char*) (0x4FFF600))


#define PRINTF_BUFFER_SIZE (4096)

static u32 MgbaConvertPStringToAsciiN(const char *src, char *dst, u32 n);
static void MgbaPrintfBounded(s32 level, const char* ptr, ...);

/*
 * Converts a "PokeString" to ASCII. This function does not have any sanity checks for buffer lengths.
 * Be careful not to overflow!
 * Failed ASSERTIONS for buffers != NULL will halt the program!
 */
static u32 MgbaConvertPStringToAsciiN(const char *src, char *dst, u32 n)
{
    u32 i;

    MGBA_ASSERT(dst != NULL);
    MGBA_ASSERT(src != NULL);

    for (i = 0; (*src != EOS) && i < n; i++, src++)
    {
        char modifiedCode = '?';
        if (*src >= CHAR_a && *src <= CHAR_z)
            modifiedCode = *src-(CHAR_a-'a'); // lower-case characters
        else if (*src >= CHAR_A && *src <= CHAR_Z)
            modifiedCode = *src-(CHAR_A-'A'); // upper-case characters
        else if (*src >= CHAR_0 && *src <= CHAR_9)
            modifiedCode = *src-(CHAR_0-'0'); // numbers
        else if (*src == CHAR_SPACE)
            modifiedCode = ' '; // space
        else if (*src == CHAR_EXCL_MARK)
            modifiedCode = '!'; // exclamation point
        else if (*src == CHAR_QUESTION_MARK)
            modifiedCode = '?'; // question mark
        else if (*src == CHAR_PERIOD)
            modifiedCode = '.'; // period
        else if (*src == CHAR_DBL_QUOT_LEFT || *src == CHAR_DBL_QUOT_RIGHT)
            modifiedCode = '"'; // double quote
        else if (*src == CHAR_SGL_QUOT_LEFT || *src == CHAR_SGL_QUOT_RIGHT)
            modifiedCode = '\''; // single quote
        else if (*src == CHAR_CURRENCY)
            modifiedCode = '$'; // currency mark (pokemonies in game, dollar sign in logs)
        else if (*src == CHAR_COMMA)
            modifiedCode = ','; // comma
        else if (*src == CHAR_MULT_SIGN)
            modifiedCode = '#'; // pound, hashtag, octothorpe, whatever
        else if (*src == CHAR_SLASH)
            modifiedCode = '/'; // slash
        else if (*src == CHAR_LESS_THAN)
            modifiedCode = '<'; // less than sign
        else if (*src == CHAR_GREATER_THAN)
            modifiedCode = '>'; // greater than sign
        else if (*src == CHAR_PERCENT)
            modifiedCode = '%'; // percentage
        else if (*src == CHAR_LEFT_PAREN)
            modifiedCode = '('; // opening parentheses
        else if (*src == CHAR_RIGHT_PAREN)
            modifiedCode = ')'; // closing parentheses
        dst[i] = modifiedCode;
    }
    dst[i] = 0;
    return i;
}

static void MgbaPrintfBounded(s32 level, const char* ptr, ...)
{
    va_list args;

    level &= 0x7;
    va_start(args, ptr);
    mini_vsnprintf(REG_DEBUG_STRING, MGBA_REG_DEBUG_MAX, ptr, args);
    va_end(args);
    *REG_DEBUG_FLAGS = level | 0x100;
}

void MgbaPrintf(s32 level, const char* ptr, ...)
{
    va_list args;
    u32 offset = 0;
    u32 n = 0;
    u32 i;
    char *buffer = Alloc(PRINTF_BUFFER_SIZE);

    MGBA_ASSERT(buffer != NULL);

    level &= 0x7;

    va_start(args, ptr);
    n = mini_vsnprintf(buffer, PRINTF_BUFFER_SIZE, ptr, args);
    va_end(args);

    MGBA_ASSERT(n < PRINTF_BUFFER_SIZE);

    do 
    {
        for (i = 0; i < MGBA_REG_DEBUG_MAX; ++i)
        {
            REG_DEBUG_STRING[i] = buffer[offset + i];
            if (buffer[offset + i] == 0)
                break;
        }
        offset += i;
        *REG_DEBUG_FLAGS = level | 0x100;
    } while ((i == MGBA_REG_DEBUG_MAX) && (buffer[offset] != '\0'));

    Free(buffer);
}

bool8 MgbaOpen(void)
{
    *REG_DEBUG_ENABLE = 0xC0DE;
    return *REG_DEBUG_ENABLE == 0x1DEA;
}

void MgbaClose(void)
{
    *REG_DEBUG_ENABLE = 0;
}

void MgbaAssert(const char *pFile, s32 nLine, const char *pExpression, bool32 nStopProgram)
{
    if (nStopProgram)
    {
        MgbaPrintfBounded(MGBA_LOG_ERROR, "ASSERTION FAILED  FILE=[%s] LINE=[%d]  EXP=[%s]", pFile, nLine, pExpression);
        asm(".hword 0xEFFF");
    }
    else
    {
        MgbaPrintfBounded(MGBA_LOG_WARN, "WARING FILE=[%s] LINE=[%d]  EXP=[%s]", pFile, nLine, pExpression);
    }
}

/*
 * Prints a "PokeString" to the MGBA LOG specified with `level`.
 */
void MgbaPrintEncoded(s32 level, const char *encodedString)
{    
    u32 offset = 0;
    u32 size = 0;

    level &= 0x7;
    do
    {
        size = MgbaConvertPStringToAsciiN(&encodedString[offset], REG_DEBUG_STRING, MGBA_REG_DEBUG_MAX);
        offset += size;
        *REG_DEBUG_FLAGS = level | 0x100;
    } while ((size == MGBA_REG_DEBUG_MAX) && (encodedString[offset] != EOS));
}