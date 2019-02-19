#include "defines.h"
#include "helper_functions.h"
#include "battle_strings.h"
#include "frontier.h"

//The link opponent needs the data from BattleStringLoader sent over
//Change to "You have no more Pokemon that can fight!"
//Change to "You blacked out!"

#define EOS 0xFF

//New Strings
extern u8 BattleText_TwoTrainersWantToBattle[];
extern u8 BattleText_ShadowWarriorAttacked[];
extern u8 BattleText_InGamePartnerSentOutZGoN[];
extern u8 BattleText_TwoTrainersSentPkmn[];
extern u8 BattleText_Trainer2SentOutPkmn[];
extern u8 BattleText_PartnerWithdrewPkmn[];
extern u8 BattleText_PartnerSaysGo[];
extern u8 BattleText_AttackerUsedZStatusMove[];
extern u8 BattleText_TwoInGameTrainersDefeated[];
extern u8 BattleText_PlayerLostToTwoInGameTrainers[];

//Rewritten Strings
extern u8 BattleText_TwoLinkTrainersWantToBattle[];
extern u8 BattleText_Trainer1WantsToBattle[];
extern u8 BattleText_LinkTrainerWantsToBattle[];
extern u8 BattleText_WildPkmnAppeared6[];
extern u8 BattleText_TwoWildPkmnAppeared[];
extern u8 BattleText_WildPkmnAppearedPause[];
extern u8 BattleText_WildPkmnAppeared[];
extern u8 sText_FoePkmnPrefix2[];
extern u8 sText_FoePkmnPrefix3[];
extern u8 sText_FoePkmnPrefix4[];

extern u8* ZMoveNames[];
extern u8 gMoveNames[][MOVE_NAME_LENGTH + 1];

extern u8 GetFrontierTrainerClassId(u16 trainerId, u8 battlerNum);
extern void GetFrontierTrainerName(u8* dst, u16 trainerId, u8 battlerNum);
extern void CopyFrontierTrainerText(u8 whichText, u16 trainerId, u8 battlerNum);
extern u8* GetTrainerBLoseText(void);

void PrepareStringBattle(u16 stringId, u8 bank) {
    gActiveBattler = bank;
    EmitPrintString(0, stringId);
    MarkBufferBankForExecution(gActiveBattler);
}

struct StringInfoBattle** gStringInfo = ((struct StringInfoBattle**) 0x2039A34);

void BufferStringBattle(u16 stringID) {
    int i;
    const u8 *stringPtr = NULL;
	
    *gStringInfo = (struct StringInfoBattle*) (&(gBattleBufferA[gActiveBattler][4]));
    gLastUsedItem = (*gStringInfo)->lastItem;
    gLastUsedAbility = (*gStringInfo)->lastAbility;
    gBattleScripting->bank = (*gStringInfo)->scrActive;
    gBattleStruct->field_52 = (*gStringInfo)->unk1605E;
    gBattleStruct->hpScale = (*gStringInfo)->hpScale; //Check this line
    gStringBank = (*gStringInfo)->StringBank;
    gBattleStruct->stringMoveType = (*gStringInfo)->moveType;

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        gAbilitiesPerBank[i] = (*gStringInfo)->abilities[i];
    }
    for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; i++)
    {
        gBattleTextBuff1[i] = (*gStringInfo)->textBuffs[0][i];
        gBattleTextBuff2[i] = (*gStringInfo)->textBuffs[1][i];
        gBattleTextBuff3[i] = (*gStringInfo)->textBuffs[2][i];
    }

    switch (stringID)
    {
    case STRINGID_INTROMSG: // first battle msg
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER) 
		{
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                    stringPtr = BattleText_TwoTrainersWantToBattle; //NEED DATA
					
                else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = BattleText_TwoLinkTrainersWantToBattle; //0x83FD397

                else
                {
                    if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                        stringPtr = BattleText_Trainer1WantsToBattle; //0x83FD366
                    else
                        stringPtr = BattleText_LinkTrainerWantsToBattle; //0x83FD383
                }
            }
			
            else //Not Link Battle
            {
				if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                    stringPtr = BattleText_TwoTrainersWantToBattle; //NEED DATA
				
                else
                    stringPtr = BattleText_Trainer1WantsToBattle; //0x83FD366
            }
        }
		
        else //Wild Battle
		{
			#ifdef UNBOUND
			if (gBattleTypeFlags & BATTLE_TYPE_SHADOW_WARRIOR)
				stringPtr = BattleText_ShadowWarriorAttacked; //NEED DATA
			#elif defined NO_GHOST_BATTLES
			if (gBattleTypeFlags & BATTLE_TYPE_SCRIPTED_WILD_1)
				stringPtr = BattleText_WildPkmnAppeared6; //0x83FD297
			#else
            if (gBattleTypeFlags & BATTLE_TYPE_GHOST) {
				if (gBattleTypeFlags & BATTLE_TYPE_SCRIPTED_WILD_1)
					stringPtr = BattleText_GhostAppeared; //0x83FD30D
				else
					stringPtr = BattleText_GhostAppearedNoID; //0x83FD2D9
			}
			else if (gBattleTypeFlags & BATTLE_TYPE_SCRIPTED_WILD_1)
				stringPtr = BattleText_WildPkmnAppeared6; //0x83FD297
			#endif
			
            else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                stringPtr = BattleText_TwoWildPkmnAppeared; //0x83FD2BF
            else if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN)
                stringPtr = BattleText_WildPkmnAppearedPause; //0x83FD2AA
            else
                stringPtr = BattleText_WildPkmnAppeared; //0x83FD284
        }
        break;

    case STRINGID_INTROSENDOUT: // poke first send-out
        if (SIDE(gActiveBattler) == B_SIDE_PLAYER)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER)
                    stringPtr = BattleText_InGamePartnerSentOutZGoN; //NEED DATA
                else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                    stringPtr = BattleText_GoTwoPkmn; //0x83FD466
                else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = BattleText_LinkPartnerSentOutPkmnGoPkmn; //0x83FD4B5
                else
                    stringPtr = BattleText_GoTwoPkmn; //0x83FD466
            }
            else //Single Battle
                stringPtr = BattleText_GoPkmn; //0x83FD45E
        }
		
        else //Opponent Send Out
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                    stringPtr = BattleText_TwoTrainersSentPkmn; //NEED DATA
                else if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                    stringPtr = BattleText_TwoTrainersSentPkmn; //NEED DATA
                else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = BattleText_TwoLinkTrainersSentOutPkmn; //0x83FD41E
                else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
                    stringPtr = BattleText_LinkTrainerSentOutTwoPkmn; //0x83FD407
                else
                    stringPtr = BattleText_Trainer1SentOutTwoPkmn; //0x83FD3C7
            }
			
            else //Opponent Send Out Single
            {
                if (gBattleTypeFlags & BATTLE_TYPE_LINK)
                    stringPtr = BattleText_LinkTrainerSentOutPkmn; //0x83FD3F7
                else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                    stringPtr = BattleText_Trainer1SentOutPkmn; //0x83FD3B1
                else
					stringPtr = BattleText_Trainer1SentOutPkmn; //0x83FD3B1
            }
        }
        break;
	
    case STRINGID_RETURNMON: // sending poke to ball msg
        if (SIDE(gActiveBattler) == B_SIDE_PLAYER)
        {
			if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(gActiveBattler) == B_POSITION_PLAYER_RIGHT)
				stringPtr = BattleText_PartnerWithdrewPkmn;
			else {
				if (gBattleStruct->hpScale == 0)
					stringPtr = BattleText_PkmnThatsEnough; //0x83FD4CD
				else if (gBattleStruct->hpScale == 1 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
					stringPtr = BattleText_PkmnComeBack; //0x83FD4EB
				else if (gBattleStruct->hpScale == 2)
					stringPtr = BattleText_PkmnOkComeBack; //0x83FD4FA
				else
					stringPtr = BattleText_PkmnGoodComeBack; //0x83FD50D
			}
        }
		
        else //Opponent Withdraw Pokemon
        {
            if (gTrainerBattleOpponent_A == TRAINER_LINK_OPPONENT)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = BattleText_LinkTrainer2WithdrewPkmn; //0x83FD545
                else
                    stringPtr = BattleText_LinkTrainer1WithdrewPkmn; //0x83FD535
            }
            else
                stringPtr = BattleText_Trainer1WithdrewPkmn; //0x83FD522
        }
        break;
	
    case STRINGID_SWITCHINMON: // switch-in msg
        if (SIDE(gBattleScripting->bank) == B_SIDE_PLAYER)
        {
			if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(gBattleScripting->bank) == B_POSITION_PLAYER_RIGHT)
				stringPtr = BattleText_PartnerSaysGo;
            else if (gBattleStruct->hpScale == 0 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                stringPtr = BattleText_GoPkmn2;  //0x83FD475
            else if (gBattleStruct->hpScale == 1)
                stringPtr = BattleText_DoItPkmn; //0x83FD47D
            else if (gBattleStruct->hpScale == 2)
                stringPtr = BattleText_GoForItPkmn; //0x83FD488
            else
                stringPtr = BattleText_YourFoesWeakGetEmPkmn; //0x83FD497
        }
		
        else //Opponent Sent Out Poke
        {
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                {
                    if (gActiveBattler == 1)
                        stringPtr = BattleText_Trainer1SentOutPkmn2; //0x83FD3E4
                    else
                        stringPtr = BattleText_Trainer2SentOutPkmn; //NEED DATA
                }
                else
                {
                    if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                        stringPtr = BattleText_LinkTrainerMultiSentOutPkmn; //0x83FD44E
                    else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                        stringPtr = BattleText_Trainer1SentOutPkmn2; //0x83FD3E4
                    else
                        stringPtr = BattleText_LinkTrainerSentOutPkmn2; //0x83FD43E
                }
            }
            else
            {
                if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
                {
                    if (gBattleScripting->bank == 1)
                        stringPtr = BattleText_Trainer1SentOutPkmn2; //0x83FD3E4
                    else
                        stringPtr = BattleText_Trainer2SentOutPkmn; //NEED DATA
                }
                else
                    stringPtr = BattleText_Trainer1SentOutPkmn2; //0x83FD3E4
            }
        }
        break;
		
    case STRINGID_USEDMOVE: 	;// pokemon used a move msg
		u16 move = (*gStringInfo)->currentMove;
        ChooseMoveUsedParticle(gBattleTextBuff1); // buff1 doesn't appear in the string, leftover from japanese move names
		
        if (move >= MOVES_COUNT)
            StringCopy(gBattleTextBuff2, sATypeMove_Table[gBattleStruct->stringMoveType]);
			
        else 
		{
			if (gNewBS->ZMoveData->active && SPLIT(move) != SPLIT_STATUS) 
			{
				//Load elongated move names for Z-Moves
				if (move < MOVE_BREAKNECK_BLITZ_P) //Prevent accidental bugs
					StringCopy(gBattleTextBuff2, ZMoveNames[0]);
				else
					StringCopy(gBattleTextBuff2, ZMoveNames[move - MOVE_BREAKNECK_BLITZ_P]);
			}
			else
				StringCopy(gBattleTextBuff2, gMoveNames[move]);
		}
		
        ChooseTypeOfMoveUsedString(gBattleTextBuff2);
		
		if (gNewBS->ZMoveData->active && SPLIT(move) == SPLIT_STATUS)
			stringPtr = BattleText_AttackerUsedZStatusMove; //NEED DATA
		else 
			stringPtr = BattleText_AttackerUsedX; //0x83FD57B
		
        break;
	
    case STRINGID_BATTLEEND: // battle end
        if (gBattleTextBuff1[0] & B_OUTCOME_LINK_BATTLE_RAN)
        {
            gBattleTextBuff1[0] &= ~(B_OUTCOME_LINK_BATTLE_RAN);
            if (SIDE(gActiveBattler) == B_SIDE_OPPONENT && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTextBuff1[0] == B_OUTCOME_LOST || gBattleTextBuff1[0] == B_OUTCOME_DREW)
                stringPtr = BattleText_GotAwaySafely; //0x83FCC8A
            else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                stringPtr = BattleText_TwoWildFled; //0x83FCD9F
			else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
				stringPtr = BattleText_C00Fled; //0x83FE9D4
            else
                stringPtr = BattleText_WildFled; //0x83FCD92
        }
		
        else
        {
            if (SIDE(gActiveBattler) == B_SIDE_OPPONENT && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI && gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    if (gBattleTypeFlags & BATTLE_TYPE_TOWER_LINK_MULTI)
                        stringPtr = BattleText_TwoInGameTrainersDefeated; //NEED DATA
                    else
                        stringPtr = BattleText_TwoLinkTrainersDefeated; //0x83FCCF8
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = BattleText_PlayerLostToTwo; //0x83FCD27
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = BattleText_PlayerBattledToDrawVsTwo; //0x83FCD66
                    break;
                }
            }
            else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = BattleText_PlayerDefeatedLinkTrainer1; //0x83FD1C7
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = BattleText_PlayerLostAgainstTrainer1; //0x83FE9E4
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = BattleText_PlayerBattledToDrawTrainer1; //0x83FE9FF
                    break;
                }
            }
			else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
			{
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = BattleText_TwoInGameTrainersDefeated; //NEED DATA
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = BattleText_PlayerLostToTwoInGameTrainers; //NEED DATA
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = BattleText_PlayerBattledToDrawLinkTrainer; //0x83FCD41
                    break;
                }
			}
            else
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = BattleText_PlayerDefeatedLinkTrainer; //0x83FCCE4
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = BattleText_PlayerLostAgainstLinkTrainer; //0x83FCD0F
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = BattleText_PlayerBattledToDrawLinkTrainer; //0x83FCD41
                    break;
                }
            }
        }
        break;
	case 0x184: //The Ultimate Battle String Loader
		stringPtr = BattleStringLoader;
		if (stringPtr == (u8*) 0)
			goto DEFAULT_STRING_LOAD;
		break;

    default: // load a string from the table
	DEFAULT_STRING_LOAD:
        if (stringID >= BATTLESTRINGS_COUNT + BATTLESTRINGS_ID_ADDER) {
            gDisplayedStringBattle[0] = EOS;
            return;
        }
        else
            stringPtr = gBattleStringsTable[stringID - BATTLESTRINGS_ID_ADDER];
        break;
    }

    StrCpyDecodeToDisplayedStringBattle(stringPtr);
}

u32 BattleStringExpandPlaceholders(const u8* src, u8* dst)
{
    u32 dstID = 0; // if they used dstID, why not use srcID as well?
    const u8* toCpy = NULL;
    u8 text[30];
    u8 multiplayerId;
    int i;

    multiplayerId = GetMultiplayerId();

    while (*src != EOS)
    {
        if (*src == B_BUFF_PLACEHOLDER_BEGIN) //0xFD
        {
            src++;
            switch (*src)
            {
            case B_TXT_BUFF1:
                if (gBattleTextBuff1[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gStringVar1);
                    toCpy = gStringVar1;
                }
                else
                {
                    toCpy = TryGetStatusString(gBattleTextBuff1);
                    if (toCpy == NULL)
                        toCpy = gBattleTextBuff1;
                }
                break;
            case B_TXT_BUFF2:
                if (gBattleTextBuff2[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {					
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff2, gStringVar2);
                    toCpy = gStringVar2;
                }
                else
                    toCpy = gBattleTextBuff2;
                break;
            case B_TXT_BUFF3: //Used in Exp gain
                if (gBattleTextBuff3[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff3, gStringVar3);
                    toCpy = gStringVar3;
                }
                else
                    toCpy = gBattleTextBuff3;
                break;
            case B_TXT_COPY_VAR_1:
                toCpy = gStringVar1;
                break;
            case B_TXT_COPY_VAR_2:
                toCpy = gStringVar2;
                break;
            case B_TXT_COPY_VAR_3:
                toCpy = gStringVar3;
                break;
            case B_TXT_PLAYER_MON1_NAME: // first player poke name
                GetMonData(GetIllusionPartyData(GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)),
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON1_NAME: // first enemy poke name
                GetMonData(GetIllusionPartyData(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)),
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_PLAYER_MON2_NAME: // second player poke name
                GetMonData(GetIllusionPartyData(GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)),
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON2_NAME: // second enemy poke name
                GetMonData(GetIllusionPartyData(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)),
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON1_NAME: // link first player poke name
                GetMonData(GetIllusionPartyData(gLinkPlayers[multiplayerId].id),
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON1_NAME: // link first opponent poke name
                GetMonData(GetIllusionPartyData(gLinkPlayers[multiplayerId].id ^ 1),
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON2_NAME: // link second player poke name
                GetMonData(GetIllusionPartyData(gLinkPlayers[multiplayerId].id ^ 2),
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON2_NAME: // link second opponent poke name
                GetMonData(GetIllusionPartyData(gLinkPlayers[multiplayerId].id ^ 3),
                           MON_DATA_NICKNAME, text);
                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX_MON1: // attacker name with prefix, only battlerId 0/1
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker, GetBattlerAtPosition(SIDE(gBattlerAttacker)));
                break;
            case B_TXT_ATK_PARTNER_NAME: // attacker partner name
                if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
                    GetMonData(GetIllusionPartyData(gBattlerAttacker ^ BIT_FLANK), MON_DATA_NICKNAME, text);
                else
                    GetMonData(GetIllusionPartyData(gBattlerAttacker ^ BIT_FLANK), MON_DATA_NICKNAME, text);

                StringGetEnd10(text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX: // attacker name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker, gBattlerAttacker);
                break;
            case B_TXT_DEF_NAME_WITH_PREFIX: // target name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerTarget, gBattlerTarget);
                break;
            case B_TXT_EFF_NAME_WITH_PREFIX: // effect battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gEffectBank, gEffectBank);
                break;
            case B_TXT_ACTIVE_NAME_WITH_PREFIX: // active battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gActiveBattler, gActiveBattler);
                break;
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX: // scripting active battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting->bank, gBattleScripting->bank);
                break;
            case B_TXT_CURRENT_MOVE: // current move name
				if ((*gStringInfo)->currentMove >= MOVES_COUNT)
                    toCpy = sATypeMove_Table[gBattleStruct->stringMoveType];
				else 
				{
					if (gNewBS->ZMoveData->active && SPLIT((*gStringInfo)->currentMove) != SPLIT_STATUS) 
					{
						//Load elongated move names for Z-Moves
						if ((*gStringInfo)->currentMove < MOVE_BREAKNECK_BLITZ_P) //Prevent accidental bugs
							toCpy = ZMoveNames[0];
						else
							toCpy = ZMoveNames[(*gStringInfo)->currentMove - MOVE_BREAKNECK_BLITZ_P];
					}
					else
						toCpy = gMoveNames[(*gStringInfo)->currentMove];
				}
                break;
            case B_TXT_LAST_MOVE: // originally used move name
                if ((*gStringInfo)->originallyUsedMove >= MOVES_COUNT)
                    toCpy = sATypeMove_Table[gBattleStruct->stringMoveType];
                else
                    toCpy = gMoveNames[(*gStringInfo)->originallyUsedMove];
                break;
            case B_TXT_LAST_ITEM: // last used item
                CopyItemName(gLastUsedItem, text);
                toCpy = text;
                break;
            case B_TXT_LAST_ABILITY: // last used ability
                toCpy = gAbilityNames[gLastUsedAbility];
                break;
            case B_TXT_ATK_ABILITY: // attacker ability
                toCpy = gAbilityNames[gAbilitiesPerBank[gBattlerAttacker]];
                break;
            case B_TXT_DEF_ABILITY: // target ability
                toCpy = gAbilityNames[gAbilitiesPerBank[gBattlerTarget]];
                break;
            case B_TXT_SCR_ACTIVE_ABILITY: // scripting active ability
                toCpy = gAbilityNames[gAbilitiesPerBank[gBattleScripting->bank]];
                break;
            case B_TXT_EFF_ABILITY: // effect battlerId ability
                toCpy = gAbilityNames[gAbilitiesPerBank[gEffectBank]];
                break;
            case B_TXT_TRAINER1_CLASS: // trainer class name
                if (gTrainerBattleOpponent_A == 0x400) //Lol Secret Bases
                    toCpy = gTrainerClassNames[GetSecretBaseTrainerNameIndex()];
                else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                    toCpy = gTrainerClassNames[sub_80447F0()];
				else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                    toCpy = gTrainerClassNames[GetTrainerTowerTrainerClassId()];
                else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
                    toCpy = gTrainerClassNames[GetEreaderTrainerClassId()]; //sub_80E7440
				else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                    toCpy = gTrainerClassNames[GetFrontierTrainerClassId(gTrainerBattleOpponent_A, 0)];
                else
                    toCpy = gTrainerClassNames[gTrainers[gTrainerBattleOpponent_A].trainerClass];
				
				if (toCpy[3] == 0x8) //Expanded Trainer Class Names
					toCpy = T1_READ_PTR(toCpy);
                break;
            case B_TXT_TRAINER1_NAME: // trainer1 name
                if (gTrainerBattleOpponent_A == 0x400) //Was used for Secret Bases in Ruby
                {	//Despite there being no Secret Bases in Fire Red, this is still present in the original code. Why GF?
                    for (i = 0; i < (s32) ARRAY_COUNT(gBattleResources->secretBase->trainerName); ++i)
                        text[i] = gBattleResources->secretBase->trainerName[i];
                    text[i] = EOS;
                    toCpy = text;
                }
                else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
                {
                    toCpy = gLinkPlayers[multiplayerId ^ BIT_SIDE].name;
                }
				else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    GetTrainerTowerTrainerName(text);
                    toCpy = text;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
				{
					GetEreaderTrainerName(text);
					toCpy = text;
				}
                else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    GetFrontierTrainerName(text, gTrainerBattleOpponent_A, 0);
                    toCpy = text;
                }
                else
                {
					u8 class = gTrainers[gTrainerBattleOpponent_A].trainerClass;
					#ifdef UNBOUND
						if (class == 0x51 || class == 0x59)
							toCpy = GetExpandedPlaceholder(ExpandPlaceholder_RivalName);
						else
							toCpy = gTrainers[gTrainerBattleOpponent_A].trainerName;
					#elif defined OVERWRITE_RIVAL
						if (class == 0x51 || class == 0x59 || class == 0x5A)
							toCpy = GetExpandedPlaceholder(ExpandPlaceholder_RivalName);
						else
							toCpy = gTrainers[gTrainerBattleOpponent_A].trainerName;
					#else
						toCpy = gTrainers[gTrainerBattleOpponent_A].trainerName;
					#endif
                }
                break;
            case B_TXT_LINK_PLAYER_NAME: // link player name
                toCpy = gLinkPlayers[multiplayerId].name;
                break;
            case B_TXT_LINK_PARTNER_NAME: // link partner name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_PARTNER(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT1_NAME: // link opponent 1 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT2_NAME: // link opponent 2 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_PARTNER(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id)))].name;
                break;
            case B_TXT_LINK_SCR_TRAINER_NAME: // link scripting active name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(gBattleScripting->bank)].name;
                break;
            case B_TXT_PLAYER_NAME: // player name
                toCpy = gSaveBlock2->playerName;
                break;
            case B_TXT_TRAINER1_LOSE_TEXT: // trainerA lose text
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    CopyTrainerTowerPlayerWonText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && gTrainerBattleOpponent_A == VarGet(BATTLE_TOWER_TID))
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, gTrainerBattleOpponent_A, 0);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerALoseText();
                }
                break;
            case B_TXT_TRAINER1_WIN_TEXT: // trainerA win text
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    CopyTrainerTowerPlayerLostText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER && gTrainerBattleOpponent_A == VarGet(BATTLE_TOWER_TID))
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_LOST_TEXT, gTrainerBattleOpponent_A, 0);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerAWinText();
                }
                break;
            case B_TXT_26: // ?
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting->bank, gBattleStruct->field_52);
                break;
            case B_TXT_PC_CREATOR_NAME: // lanette pc
                if (FlagGet(FLAG_SYS_PC_BILL))
                    toCpy = sText_Bills;
                else
                    toCpy = sText_Someones;
                break;
			
            case B_TXT_ATK_PREFIX1:
                if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix; //0x83FD564
                else
                    toCpy = sText_FoePkmnPrefix2; //0x83FD560
                break;
            case B_TXT_DEF_PREFIX1:
                if (SIDE(gBattlerTarget) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix; //0x83FD564
                else
                    toCpy = sText_FoePkmnPrefix2; //0x83FD560
                break;
			
            case B_TXT_ATK_PREFIX2:
                if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix2; //0x83FD56D
                else
                    toCpy = sText_FoePkmnPrefix3; //0x83FD569
                break;
            case B_TXT_DEF_PREFIX2:
                if (SIDE(gBattlerTarget) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix2; //0x83FD56D
                else
                    toCpy = sText_FoePkmnPrefix3; //0x83FD569
                break;
			
            case B_TXT_ATK_PREFIX3:
                if (SIDE(gBattlerAttacker) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix3; //0x83FD576
                else
                    toCpy = sText_FoePkmnPrefix4; //0x83FD572
                break;
            case B_TXT_DEF_PREFIX3:
                if (SIDE(gBattlerTarget) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix3; //0x83FD576
                else
                    toCpy = sText_FoePkmnPrefix4; //0x83FD572
                break;
			
            case B_TXT_TRAINER2_CLASS: //In FR, Trainer Tower Opponent Defeated Text
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
				{
                    toCpy = gTrainerClassNames[GetFrontierTrainerClassId(VarGet(SECOND_OPPONENT_VAR), 1)];
				}
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
				{
                    toCpy = gStringVar4;
					CopyTrainerTowerPlayerWonText(gStringVar4, 0);
				}
                else
                    toCpy = gTrainerClassNames[gTrainers[VarGet(SECOND_OPPONENT_VAR)].trainerClass];
				
				
                if (toCpy[3] == 0x8) //Expanded Trainer Class Names
					toCpy = T1_READ_PTR(toCpy);
				
				break;
				
            case B_TXT_TRAINER2_NAME: //In FR, Lost to Trainer Tower Opponent Text / Trainer Tower Opponent Win Text
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    GetFrontierTrainerName(text, VarGet(SECOND_OPPONENT_VAR), 1);
                    toCpy = text;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
				{
                    toCpy = gStringVar4;
					CopyTrainerTowerPlayerLostText(gStringVar4, 0);
				}
                else
                {
                    toCpy = gTrainers[VarGet(SECOND_OPPONENT_VAR)].trainerName;
                }
                break;
            case B_TXT_TRAINER2_LOSE_TEXT:
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, VarGet(SECOND_OPPONENT_VAR), 1);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerBLoseText();
                }
                break;
            case B_TXT_TRAINER2_WIN_TEXT:
                if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
                {
                    CopyFrontierTrainerText(FRONTIER_PLAYER_LOST_TEXT, VarGet(SECOND_OPPONENT_VAR), 1);
                    toCpy = gStringVar4;
                }
                break;
            case B_TXT_PARTNER_CLASS:
                toCpy = gTrainerClassNames[GetFrontierTrainerClassId(VarGet(PARTNER_VAR), 2)];
                break;
            case B_TXT_PARTNER_NAME:
                GetFrontierTrainerName(text, VarGet(PARTNER_VAR), 2);
                toCpy = text;
                break;
            }

            if (toCpy != NULL) {
				while (*toCpy != EOS)
				{
					dst[dstID] = *toCpy;
					dstID++;
					toCpy++;
				}
			}
            if (*src == B_TXT_TRAINER1_LOSE_TEXT || *src == B_TXT_TRAINER2_LOSE_TEXT
                || *src == B_TXT_TRAINER1_WIN_TEXT || *src == B_TXT_TRAINER2_WIN_TEXT)
            {
                dst[dstID] = EXT_CTRL_CODE_BEGIN;
                dstID++;
                dst[dstID] = 9;
                dstID++;
            }
        }
        else
        {
            dst[dstID] = *src;
            dstID++;
        }
        ++src;
    }

    dst[dstID] = *src;
    dstID++;

    return dstID;
}

void EmitPrintStringUpdate(void)
{
    gBattleBuffersTransferData[0] = CONTROLLER_PRINTSTRING;
    gBattleBuffersTransferData[1] = (u32) BattleStringLoader;
    gBattleBuffersTransferData[2] = ((u32) BattleStringLoader) >> 0x8;
    gBattleBuffersTransferData[3] = ((u32) BattleStringLoader) >> 0x10;
	gBattleBuffersTransferData[4] = ((u32) BattleStringLoader) >> 0x18;
    PrepareBufferDataTransfer(1, gBattleBuffersTransferData, 5);
}

void PlayerHandlePrintStringUpdate(void)
{
	BattleStringLoader = T1_READ_PTR(&gBattleBufferB[gActiveBattler][1]);
    gBattle_BG0_X = 0;
    gBattle_BG0_Y = 0;
}