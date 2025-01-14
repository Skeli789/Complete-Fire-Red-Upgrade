#pragma once

#include "../global.h"

/**
 * \file battle_strings_2.h
 * \brief A file to be included on by "src/battle_strings.c". Contains several
 *		  macros for getting battling Pokemon nicknames.
 */

#define ILLUSION_HIDE_NAME(bank)																	\
{																									\
	if (gStatuses3[bank] & STATUS3_ILLUSION) {														\
		memcpy(&(IllusionNickNameBackup[bank]), &(GetBankPartyData(bank)->nickname), 10);			\
		memcpy(&(GetBankPartyData(bank)->nickname), &(GetIllusionPartyData(bank)->nickname), 10);	\
	}																								\
}

#define ILLUSION_RESTORE_NAME(bank)																	\
{																									\
	if (gStatuses3[bank] & STATUS3_ILLUSION) {														\
		memcpy(&(GetBankPartyData(bank)->nickname), &(IllusionNickNameBackup[bank]), 10);			\
		memset(&(IllusionNickNameBackup[bank]), 0, 10);												\
	}																								\
}

#define HANDLE_NICKNAME_STRING_CASE(battlerId, monIndex) 						\
{																				\
    if (SIDE(battlerId) != B_SIDE_PLAYER)                     					\
    {                                                                   		\
        if (useTheOpposing)                     								\
            toCpy = TheOpposingString;                                			\
		else if (gBattleTypeFlags & BATTLE_TYPE_SHADOW_WARRIOR)					\
			toCpy = TheShadowString;  											\
        else                                                            		\
            toCpy = gText_TheWild;                               				\
																				\
        while (*toCpy != EOS)                                           		\
        {                                                              	 		\
            dst[dstID] = *toCpy;                                        		\
            dstID++;                                                    		\
            toCpy++;                                                    		\
        }                                                               		\
        GetMonData(GetIllusionPartyData(battlerId), MON_DATA_NICKNAME, text);   \
    }                                                                   		\
    else                                                                		\
    {                                                                   		\
        GetMonData(GetIllusionPartyData(battlerId), MON_DATA_NICKNAME, text);   \
    }                                                                   		\
    StringGetEnd10(text);                                               		\
    toCpy = text;																\
}
