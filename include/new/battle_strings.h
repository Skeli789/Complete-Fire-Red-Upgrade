#pragma once

#include "../../src/defines.h"
#include "../../src/defines_battle.h"

extern u8 TheOpposingString[];
extern u8 TheShadowString[];
extern u8 TheWildString[];

#define ILLUSION_HIDE_NAME(bank)																						\
{																														\
	if (gStatuses3[bank] & STATUS3_ILLUSION) {																			\
		memcpy(&(IllusionNickNameBackup[bank]), &(GetBankPartyData(bank)->nickname), 10);								\
		memcpy(&(GetBankPartyData(bank)->nickname), &(GetIllusionPartyData(bank)->nickname), 10);						\
	}																													\
}																														

#define ILLUSION_RESTORE_NAME(bank)																						\
{																														\
	if (gStatuses3[bank] & STATUS3_ILLUSION) {																			\
		memcpy(&(GetBankPartyData(bank)->nickname), &(IllusionNickNameBackup[bank]), 10);								\
		memset(&(IllusionNickNameBackup[bank]), 0, 10);																	\
	}																													\
}

#define HANDLE_NICKNAME_STRING_CASE(battlerId, monIndex) 						\
{																				\
    if (SIDE(battlerId) != B_SIDE_PLAYER)                     					\
    {                                                                   		\
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     		\
            toCpy = TheOpposingString;                                			\
																				\
																				\
		else if (gBattleTypeFlags & BATTLE_TYPE_SHADOW_WARRIOR)					\
			toCpy = TheShadowString;  											\
																				\
																				\
        else                                                            		\
            toCpy = TheWildString;                               				\
        while (*toCpy != EOS)                                           		\
        {                                                              	 		\
            dst[dstID] = *toCpy;                                        		\
            dstID++;                                                    		\
            toCpy++;                                                    		\
        }                                                               		\
        GetMonData(GetIllusionPartyData(battlerId), MON_DATA_NICKNAME, text);    \
    }                                                                   		\
    else                                                                		\
    {                                                                   		\
        GetMonData(GetIllusionPartyData(battlerId), MON_DATA_NICKNAME, text);   	\
    }                                                                   		\
    StringGetEnd10(text);                                               		\
    toCpy = text;																\
}

#define BattleText_GhostAppeared (u8*) 0x83FD30D
#define BattleText_GhostAppearedNoID (u8*) 0x83FD2D9
#define BattleText_GoTwoPkmn (u8*) 0x83FD466
#define BattleText_LinkPartnerSentOutPkmnGoPkmn (u8*) 0x83FD4B5
#define BattleText_GoTwoPkmn (u8*) 0x83FD466
#define BattleText_GoPkmn (u8*) 0x83FD45E
#define BattleText_TwoLinkTrainersSentOutPkmn (u8*) 0x83FD41E
#define BattleText_LinkTrainerSentOutTwoPkmn (u8*) 0x83FD407
#define BattleText_Trainer1SentOutTwoPkmn (u8*) 0x83FD3C7
#define BattleText_LinkTrainerSentOutPkmn (u8*) 0x83FD3F7
#define BattleText_Trainer1SentOutPkmn (u8*) 0x83FD3B1
#define BattleText_PkmnThatsEnough (u8*) 0x83FD4CD
#define BattleText_PkmnComeBack (u8*) 0x83FD4EB
#define BattleText_PkmnOkComeBack (u8*) 0x83FD4FA
#define BattleText_PkmnGoodComeBack (u8*) 0x83FD50D
#define BattleText_LinkTrainer2WithdrewPkmn (u8*) 0x83FD545
#define BattleText_LinkTrainer1WithdrewPkmn (u8*) 0x83FD535
#define BattleText_Trainer1WithdrewPkmn (u8*) 0x83FD522
#define BattleText_GoPkmn2 (u8*) 0x83FD475
#define BattleText_DoItPkmn (u8*) 0x83FD47D
#define BattleText_GoForItPkmn (u8*) 0x83FD488
#define BattleText_YourFoesWeakGetEmPkmn (u8*) 0x83FD497
#define BattleText_Trainer1SentOutPkmn2 (u8*) 0x83FD3E4
#define BattleText_LinkTrainerMultiSentOutPkmn (u8*) 0x83FD44E
#define BattleText_Trainer1SentOutPkmn2 (u8*) 0x83FD3E4
#define BattleText_LinkTrainerSentOutPkmn2 (u8*) 0x83FD43E
#define BattleText_Trainer1SentOutPkmn2 (u8*) 0x83FD3E4
#define BattleText_AttackerUsedX (u8*) 0x83FD57B
#define BattleText_GotAwaySafely (u8*) 0x83FCC8A
#define BattleText_TwoWildFled (u8*) 0x83FCD9F
#define BattleText_C00Fled (u8*) 0x83FE9D4
#define BattleText_WildFled (u8*) 0x83FCD92
#define BattleText_TwoLinkTrainersDefeated (u8*) 0x83FCCF8
#define BattleText_PlayerLostToTwo (u8*) 0x83FCD27
#define BattleText_PlayerBattledToDrawVsTwo (u8*) 0x83FCD66
#define BattleText_PlayerDefeatedLinkTrainer1 (u8*) 0x83FD1C7
#define BattleText_PlayerLostAgainstTrainer1 (u8*) 0x83FE9E4
#define BattleText_PlayerBattledToDrawTrainer1 (u8*) 0x83FE9FF
#define BattleText_PlayerDefeatedLinkTrainer (u8*) 0x83FCCE4
#define BattleText_PlayerLostAgainstLinkTrainer (u8*) 0x83FCD0F
#define BattleText_PlayerBattledToDrawLinkTrainer (u8*) 0x83FCD41
#define sText_AllyPkmnPrefix (u8*) 0x83FD564
#define sText_AllyPkmnPrefix2 (u8*) 0x83FD56D
#define sText_AllyPkmnPrefix3 (u8*) 0x83FD576

#define sText_Someones (u8*) 0x83FD81A
#define sText_Bills	(u8*) 0x83FD824
