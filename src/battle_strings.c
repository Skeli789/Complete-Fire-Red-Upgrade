#include "defines.h"
#include "defines_battle.h"

#include "../include/battle_string_ids.h"
#include "../include/link.h"
#include "../include/string_util.h"
#include "../include/text.h"
#include "../include/constants/flags.h"
#include "../include/constants/region_map_sections.h"
#include "../include/constants/trainers.h"
#include "../include/constants/trainer_classes.h"

#include "../include/new/battle_strings.h"
#include "../include/new/battle_strings_2.h"
#include "../include/new/battle_util.h"
#include "../include/new/dynamax.h"
#include "../include/new/frontier.h"
#include "../include/new/general_battle_strings.h"
#include "../include/new/mega.h"
#include "../include/new/multi.h"
#include "../include/new/set_z_effect.h"
#include "../include/new/text.h"

/*
battle_strings.c
	modifies the strings displayed in battle.
*/

extern u8 gMoveNames[][MOVE_NAME_LENGTH + 1];

extern const u8 gStatusConditionString_Frostbite[];
extern const u8 gStatusConditionString_DisableProblem[];
extern const u8 gStatusConditionString_EncoreProblem[];
extern const u8 gStatusConditionString_MentalState[];
extern const u8 gStatusConditionString_TauntProblem[];

const u8 * const gStatusConditionStringsTable[11][2] =
{
	{gStatusConditionString_Poison, (const u8*) 0x83FE830},
	{gStatusConditionString_Sleep, (const u8*) 0x83FE82A},
	{gStatusConditionString_Paralysis, (const u8*) 0x83FE83C},
	{gStatusConditionString_Burn, (const u8*) 0x83FE837},
	#ifdef FROSTBITE
	{gStatusConditionString_Frostbite, gStatusConditionString_Frostbite},
	#else
	{gStatusConditionString_Ice, (const u8*) 0x83FE846},
	#endif
	{gStatusConditionString_Confusion, (const u8*) 0x83FE84A},
	{gStatusConditionString_Love, (const u8*) 0x83FE854},
	{gStatusConditionString_MentalState, gStatusConditionString_MentalState},
	{gStatusConditionString_TauntProblem, gStatusConditionString_TauntProblem},
	{gStatusConditionString_DisableProblem, gStatusConditionString_DisableProblem},
	{gStatusConditionString_EncoreProblem, gStatusConditionString_EncoreProblem},
};

//This file's functions:
static void FixTheCapitalizationInDisplayedString(void);
#ifdef OPEN_WORLD_TRAINERS
static u8* GetOpenWorldTrainerName(bool8 female);
#endif
static const u8* GetTrainerClassName(u8 class, u8* text);

void PrepareStringBattle(u16 stringId, u8 bank)
{
	gActiveBattler = bank;
	EmitPrintString(0, stringId);
	MarkBufferBankForExecution(gActiveBattler);
}

const struct BattleMsgData** gStringInfo = ((const struct BattleMsgData**) 0x2039A34);

void BufferStringBattle(u16 stringID)
{
	int i;
	const u8 *stringPtr = NULL;

	*gStringInfo = (struct BattleMsgData*) (&(gBattleBufferA[gActiveBattler][4]));
	gLastUsedItem = (*gStringInfo)->lastItem;
	gLastUsedAbility = (*gStringInfo)->lastAbility;
	gBattleScripting.bank = (*gStringInfo)->scrActive;
	gBattleStruct->field_52 = (*gStringInfo)->unk1605E;
	gBattleStruct->hpScale = (*gStringInfo)->hpScale; //Check this line
	gStringBank = (*gStringInfo)->stringBank;
	gBattleStruct->stringMoveType = (*gStringInfo)->moveType;
	gBattleStringLoader = (*gStringInfo)->battleStringLoader;
	bool8 zMoveActive = (*gStringInfo)->zMoveActive;

	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
		gAbilitiesPerBank[i] = (*gStringInfo)->abilities[i];

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
				if (IS_TOWER_LINK_MULTI_BATTLE)
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

				else if (IsAIControlledBattle())
					stringPtr = BattleText_Trainer1WantsToBattle_BattleSands;

				else
					stringPtr = BattleText_Trainer1WantsToBattle; //0x83FD366
			}
		}

		else //Wild Battle
		{
			#ifdef UNBOUND
			if (gBattleTypeFlags & BATTLE_TYPE_SHADOW_WARRIOR)
			{
				if (IS_DOUBLE_BATTLE)
					stringPtr = BattleText_ShadowWarriorsAttacked;
				else
					stringPtr = BattleText_ShadowWarriorAttacked;
			}
			#elif defined NO_GHOST_BATTLES
			if (gBattleTypeFlags & BATTLE_TYPE_SCRIPTED_WILD_1)
				stringPtr = BattleText_WildPkmnAppeared6; //0x83FD297
			#else
			if (gBattleTypeFlags & BATTLE_TYPE_GHOST)
			{
				if (gBattleTypeFlags & BATTLE_TYPE_SCRIPTED_WILD_1)
					stringPtr = BattleText_GhostAppeared; //0x83FD30D
				else
					stringPtr = BattleText_GhostAppearedNoID; //0x83FD2D9
			}
			else if (gBattleTypeFlags & BATTLE_TYPE_SCRIPTED_WILD_1)
				stringPtr = BattleText_WildPkmnAppeared6; //0x83FD297
			#endif

			#ifdef FLAG_WORST_NIGHTMARE_STRING
			else if (FlagGet(FLAG_WORST_NIGHTMARE_STRING))
				stringPtr = BattleText_WorstNightmare;
			#endif

			else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && !IsRaidBattle())
			{
				#ifdef FLAG_TWO_WILD_POKEMON_ATTACKED_STRING
				if (FlagGet(FLAG_TWO_WILD_POKEMON_ATTACKED_STRING))
					stringPtr = BattleText_TwoWildAttacked;
				else
				#endif
					stringPtr = BattleText_TwoWildPkmnAppeared; //0x83FD2BF
			}
			else if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN)
				stringPtr = BattleText_WildPkmnAppearedPause; //0x83FD2AA
			else
			{
				#ifdef MAPSEC_DISTORTION_WORLD
				if (GetCurrentRegionMapSectionId() == MAPSEC_DISTORTION_WORLD)
					stringPtr = BattleText_DistortionWorldMonAttacked;
				else
				#endif
				#ifdef FLAG_LEGENDARY_APPEARED_STRING
				if (FlagGet(FLAG_LEGENDARY_APPEARED_STRING))
					stringPtr = BattleText_LegendaryAppeared;
				else
				#endif

				stringPtr = BattleText_WildPkmnAppeared; //0x83FD284
			}
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
				else if (IsAIControlledBattle())
					stringPtr = BattleText_GoTwoPkmn_BattleSands;
				else
					stringPtr = BattleText_GoTwoPkmn; //0x83FD466
			}
			//Single Battle
			else if (IsAIControlledBattle())
				stringPtr = BattleText_GoPkmn_BattleSands;
			else //Single Battle
				stringPtr = BattleText_GoPkmn; //0x83FD45E
		}

		else //Opponent Send Out
		{
			if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
			{
				if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS)
					stringPtr = BattleText_TwoTrainersSentPkmn; //NEED DATA
				else if (IS_TOWER_LINK_MULTI_BATTLE)
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
			else if (IsAIControlledBattle())
				stringPtr = BattleText_PartnerWithdrewPkmn;
			else if (gBattleStruct->hpScale == 0)
				stringPtr = BattleText_PkmnThatsEnough; //0x83FD4CD
			else if (gBattleStruct->hpScale == 1 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
				stringPtr = BattleText_PkmnComeBack; //0x83FD4EB
			else if (gBattleStruct->hpScale == 2)
				stringPtr = BattleText_PkmnOkComeBack; //0x83FD4FA
			else
				stringPtr = BattleText_PkmnGoodComeBack; //0x83FD50D
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
			else if (gBattleTypeFlags & BATTLE_TYPE_TWO_OPPONENTS
			&& GetBattlerPosition(gActiveBattler) == B_POSITION_OPPONENT_RIGHT)
				stringPtr = BattleText_Trainer2WithdrewPkmn;
			else
				stringPtr = BattleText_Trainer1WithdrewPkmn; //0x83FD522
		}
		break;

	case STRINGID_SWITCHINMON: // switch-in msg
		if (SIDE(gBattleScripting.bank) == B_SIDE_PLAYER)
		{
			if (gBattleTypeFlags & BATTLE_TYPE_INGAME_PARTNER && GetBattlerPosition(gBattleScripting.bank) == B_POSITION_PLAYER_RIGHT)
				stringPtr = BattleText_PartnerSaysGo;
			else if (IsAIControlledBattle())
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
				if (IS_TOWER_LINK_MULTI_BATTLE)
				{
					if (GetBattlerPosition(gBattleScripting.bank) == B_POSITION_OPPONENT_LEFT)
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
					if (GetBattlerPosition(gBattleScripting.bank) == B_POSITION_OPPONENT_LEFT)
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
			BufferMoveNameBattle(move, gBattleTextBuff2);

		ChooseTypeOfMoveUsedString(gBattleTextBuff2);

		if (zMoveActive && SPLIT(move) == SPLIT_STATUS && move != MOVE_EXTREME_EVOBOOST)
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
					if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
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
		stringPtr = gBattleStringLoader;
		if (stringPtr == (u8*) 0)
			goto DEFAULT_STRING_LOAD;
		break;

	default: // load a string from the table
	DEFAULT_STRING_LOAD:
		if (stringID >= BATTLESTRINGS_COUNT + BATTLESTRINGS_ID_ADDER)
		{
			gDisplayedStringBattle[0] = EOS;
			return;
		}
		else
			stringPtr = gBattleStringsTable[stringID - BATTLESTRINGS_ID_ADDER];
		break;
	}

	BattleStringExpandPlaceholdersToDisplayedString(stringPtr);
	FixTheCapitalizationInDisplayedString();
}

u32 BattleStringExpandPlaceholders(const u8* src, u8* dst)
{
	int i;
	u8 multiplayerId;
	u8 text[30];

	u32 dstID = 0; // if they used dstID, why not use srcID as well?
	const u8* toCpy = NULL;
	bool8 useTheOpposing = (gBattleTypeFlags & BATTLE_TYPE_TRAINER) != 0
		#ifdef FLAG_DISPLAY_OPPOSING_FOR_WILD
		|| FlagGet(FLAG_DISPLAY_OPPOSING_FOR_WILD)
		#endif
		;

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
				HANDLE_NICKNAME_STRING_CASE(gBankAttacker, GetBattlerAtPosition(SIDE(gBankAttacker)));
				break;
			case B_TXT_ATK_PARTNER_NAME: // attacker partner name
				GetMonData(GetIllusionPartyData(gBankAttacker ^ BIT_FLANK), MON_DATA_NICKNAME, text);
				StringGetEnd10(text);
				toCpy = text;
				break;
			case B_TXT_ATK_NAME_WITH_PREFIX: // attacker name with prefix
				HANDLE_NICKNAME_STRING_CASE(gBankAttacker, gBankAttacker);
				break;
			case B_TXT_DEF_NAME_WITH_PREFIX: // target name with prefix
				HANDLE_NICKNAME_STRING_CASE(gBankTarget, gBankTarget);
				break;
			case B_TXT_EFF_NAME_WITH_PREFIX: // effect battlerId name with prefix
				HANDLE_NICKNAME_STRING_CASE(gEffectBank, gEffectBank);
				break;
			case B_TXT_ACTIVE_NAME_WITH_PREFIX: // active battlerId name with prefix
				HANDLE_NICKNAME_STRING_CASE(gActiveBattler, gActiveBattler);
				break;
			case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX: // scripting active battlerId name with prefix
				HANDLE_NICKNAME_STRING_CASE(gBattleScripting.bank, gBattleScripting.bank);
				break;
			case B_TXT_CURRENT_MOVE: // current move name
				if ((*gStringInfo)->currentMove >= MOVES_COUNT)
					toCpy = sATypeMove_Table[gBattleStruct->stringMoveType];
				else
				{
					BufferMoveNameBattle((*gStringInfo)->currentMove, text);
					toCpy = text;
				}
				break;
			case B_TXT_LAST_MOVE: // originally used move name
				if ((*gStringInfo)->originallyUsedMove >= MOVES_COUNT)
					toCpy = sATypeMove_Table[gBattleStruct->stringMoveType];
				else
				{
					BufferMoveNameBattle((*gStringInfo)->originallyUsedMove, text);
					toCpy = text;
				}
				break;
			case B_TXT_LAST_ITEM: // last used item
				CopyItemName(gLastUsedItem, text);
				toCpy = text;
				break;
			case B_TXT_LAST_ABILITY: // last used ability
				toCpy = GetAbilityName(gLastUsedAbility, SPECIES(gActiveBattler));
				break;
			case B_TXT_ATK_ABILITY: // attacker ability
				toCpy = GetAbilityName(gAbilitiesPerBank[gBankAttacker], (*gStringInfo)->species[gBankAttacker]);
				break;
			case B_TXT_DEF_ABILITY: // target ability
				toCpy = GetAbilityName(gAbilitiesPerBank[gBankTarget], (*gStringInfo)->species[gBankTarget]);
				break;
			case B_TXT_SCR_ACTIVE_ABILITY: // scripting active ability
				toCpy = GetAbilityName(gAbilitiesPerBank[gBattleScripting.bank], (*gStringInfo)->species[gBattleScripting.bank]);
				break;
			case B_TXT_EFF_ABILITY: // effect battlerId ability
				toCpy = GetAbilityName(gAbilitiesPerBank[gEffectBank], (*gStringInfo)->species[gEffectBank]);
				break;
			case B_TXT_TRAINER1_CLASS: // trainer class name
				if (gTrainerBattleOpponent_A == TRAINER_SECRET_BASE) //LOL Secret Bases
					toCpy = GetTrainerClassName(GetSecretBaseTrainerNameIndex(), text);
				else if (gTrainerBattleOpponent_A == TRAINER_OPPONENT_C00)
					toCpy = GetTrainerClassName(GetUnionRoomTrainerClass(), text);
				else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
					toCpy = GetTrainerClassName(GetTrainerTowerTrainerClassId(), text);
				else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
					toCpy = GetTrainerClassName(GetEreaderTrainerClassId(), text);
				else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER || IsFrontierTrainerId(gTrainerBattleOpponent_A))
					toCpy = GetTrainerClassName(GetFrontierTrainerClassId(gTrainerBattleOpponent_A, 0), text);
				else
					toCpy = GetTrainerClassName(gTrainers[gTrainerBattleOpponent_A].trainerClass, text);

				if (toCpy[3] == 0x8 || toCpy[3] == 0x9) //Expanded Trainer Class Names
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
				else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER || IsFrontierTrainerId(gTrainerBattleOpponent_A))
				{
					CopyFrontierTrainerName(text, gTrainerBattleOpponent_A, 0);
					toCpy = text;
				}
				else
				{
					u8 class = gTrainers[gTrainerBattleOpponent_A].trainerClass;
					class += 0; //So no unusued variable is displayed
					#ifdef OPEN_WORLD_TRAINERS
						if (gTrainerBattleOpponent_A < DYNAMIC_TRAINER_LIMIT && class != CLASS_TEAM_ROCKET)
						{
							toCpy = GetOpenWorldTrainerName(gTrainers[gTrainerBattleOpponent_A].gender);
							break;
						}
					#endif

					#ifdef UNBOUND
						if (class == 0x51 || class == 0x59)
							toCpy = GetExpandedPlaceholder(ExpandPlaceholder_RivalName);
						else
					#elif defined OVERWRITE_RIVAL
						if (class == 0x51 || class == 0x59 || class == 0x5A)
							toCpy = GetExpandedPlaceholder(ExpandPlaceholder_RivalName);
						else
					#endif

					toCpy = gTrainers[gTrainerBattleOpponent_A].trainerName;
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
				toCpy = gLinkPlayers[GetBattlerMultiplayerId(gBattleScripting.bank)].name;
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
				else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER
				|| (IsFrontierTrainerId(gTrainerBattleOpponent_A)
				 && (GetTrainerALoseText() == NULL || GetTrainerALoseText()[0] == EOS)))
				{
					CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, gTrainerBattleOpponent_A, 0);
					BattleStringExpandPlaceholders(gStringVar4, gStringVar3);
					toCpy = gStringVar3;
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
				else if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER
				|| (IsFrontierTrainerId(gTrainerBattleOpponent_A) && GetTrainerAWinText() == NULL))
				{
					CopyFrontierTrainerText(FRONTIER_PLAYER_LOST_TEXT, gTrainerBattleOpponent_A, 0);
					BattleStringExpandPlaceholders(gStringVar4, gStringVar3);
					toCpy = gStringVar3;
				}
				else
				{
					toCpy = GetTrainerAWinText();
				}
				break;
			case B_TXT_26: // ?
				HANDLE_NICKNAME_STRING_CASE(gBattleScripting.bank, gBattleStruct->field_52);
				break;
			case B_TXT_PC_CREATOR_NAME: // lanette pc
				if (FlagGet(FLAG_SYS_NOT_SOMEONES_PC))
					toCpy = sText_Bills;
				else
					toCpy = sText_Someones;
				break;

			case B_TXT_ATK_PREFIX1:
				if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
					toCpy = sText_AllyPkmnPrefix; //0x83FD564
				else
					toCpy = sText_FoePkmnPrefix2; //0x83FD560
				break;
			case B_TXT_DEF_PREFIX1:
				if (SIDE(gBankTarget) == B_SIDE_PLAYER)
					toCpy = sText_AllyPkmnPrefix; //0x83FD564
				else
					toCpy = sText_FoePkmnPrefix2; //0x83FD560
				break;

			case B_TXT_ATK_PREFIX2:
				if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
					toCpy = sText_AllyPkmnPrefix2; //0x83FD56D
				else
					toCpy = sText_FoePkmnPrefix3; //0x83FD569
				break;
			case B_TXT_DEF_PREFIX2:
				if (SIDE(gBankTarget) == B_SIDE_PLAYER)
					toCpy = sText_AllyPkmnPrefix2; //0x83FD56D
				else
					toCpy = sText_FoePkmnPrefix3; //0x83FD569
				break;

			case B_TXT_ATK_PREFIX3:
				if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
					toCpy = sText_AllyPkmnPrefix3; //0x83FD576
				else
					toCpy = sText_FoePkmnPrefix4; //0x83FD572
				break;
			case B_TXT_DEF_PREFIX3:
				if (SIDE(gBankTarget) == B_SIDE_PLAYER)
					toCpy = sText_AllyPkmnPrefix3; //0x83FD576
				else
					toCpy = sText_FoePkmnPrefix4; //0x83FD572
				break;

			case B_TXT_TRAINER2_CLASS: //In FR, Trainer Tower Opponent Defeated Text
				if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER || IsFrontierTrainerId(gTrainerBattleOpponent_B))
				{
					toCpy = GetTrainerClassName(GetFrontierTrainerClassId(VarGet(VAR_SECOND_OPPONENT), 1), text);
				}
				else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
				{
					toCpy = gStringVar4;
					CopyTrainerTowerPlayerWonText(gStringVar4, 0);
				}
				else
					toCpy = GetTrainerClassName(gTrainers[VarGet(VAR_SECOND_OPPONENT)].trainerClass, text);


				if (toCpy[3] == 0x8 || toCpy[3] == 0x9) //Expanded Trainer Class Names
					toCpy = T1_READ_PTR(toCpy);

				break;

			case B_TXT_TRAINER2_NAME: //In FR, Lost to Trainer Tower Opponent Text / Trainer Tower Opponent Win Text
				if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER || IsFrontierTrainerId(gTrainerBattleOpponent_B))
				{
					CopyFrontierTrainerName(text, VarGet(VAR_SECOND_OPPONENT), 1);
					toCpy = text;
				}
				else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
				{
					toCpy = gStringVar4;
					CopyTrainerTowerPlayerLostText(gStringVar4, 0);
				}
				else
				{
					u8 class = gTrainers[VarGet(VAR_SECOND_OPPONENT)].trainerClass;
					class += 0;
					#ifdef OPEN_WORLD_TRAINERS
						if (VarGet(VAR_SECOND_OPPONENT) < DYNAMIC_TRAINER_LIMIT && class != CLASS_TEAM_ROCKET)
						{
							toCpy = GetOpenWorldTrainerName(gTrainers[VarGet(VAR_SECOND_OPPONENT)].gender);
							break;
						}
					#endif

					#ifdef UNBOUND
						if (class == 0x51 || class == 0x59)
							toCpy = GetExpandedPlaceholder(ExpandPlaceholder_RivalName);
						else
					#elif defined OVERWRITE_RIVAL
						if (class == 0x51 || class == 0x59 || class == 0x5A)
							toCpy = GetExpandedPlaceholder(ExpandPlaceholder_RivalName);
						else
					#endif

					toCpy = gTrainers[VarGet(VAR_SECOND_OPPONENT)].trainerName;
				}
				break;
			case B_TXT_TRAINER2_LOSE_TEXT:
				if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER
				|| (IsFrontierTrainerId(gTrainerBattleOpponent_B)
				 && (GetTrainerBLoseText() == NULL || GetTrainerBLoseText()[0] == EOS)))
				{
					CopyFrontierTrainerText(FRONTIER_PLAYER_WON_TEXT, VarGet(VAR_SECOND_OPPONENT), 1);
					BattleStringExpandPlaceholders(gStringVar4, gStringVar3);
					toCpy = gStringVar3;
				}
				else
				{
					toCpy = GetTrainerBLoseText();
				}
				break;
			case B_TXT_TRAINER2_WIN_TEXT:
				if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER || IsFrontierTrainerId(gTrainerBattleOpponent_B))
				{
					CopyFrontierTrainerText(FRONTIER_PLAYER_LOST_TEXT, VarGet(VAR_SECOND_OPPONENT), 1);
					BattleStringExpandPlaceholders(gStringVar4, gStringVar3);
					toCpy = gStringVar3;
				}
				break;
			case B_TXT_PARTNER_CLASS:
				toCpy = GetTrainerClassName(GetFrontierTrainerClassId(VarGet(VAR_PARTNER), 2), text);
				break;
			case B_TXT_PARTNER_NAME:
				CopyFrontierTrainerName(text, VarGet(VAR_PARTNER), 2);
				toCpy = text;
				break;
			case B_TXT_AFFECTS_TARGET_SIDE:
				if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
					toCpy = sText_TheOpposingNoCaps;
				else
					toCpy = sText_Your;
				break;
			case B_TXT_AFFECTS_ATTACKER_SIDE:
				if (SIDE(gBankAttacker) == B_SIDE_OPPONENT)
					toCpy = sText_TheOpposingNoCaps;
				else
					toCpy = sText_Your;
				break;
			case B_TXT_AFFECTS_TARGET_SIDE_CAPS:
				if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
					toCpy = sText_TheOpposingCapsNoSpace;
				else
					toCpy = sText_YourCaps;
				break;
			case B_TXT_AFFECTS_ATTACKER_SIDE_CAPS:
				if (SIDE(gBankAttacker) == B_SIDE_OPPONENT)
					toCpy = sText_TheOpposingCapsNoSpace;
				else
					toCpy = sText_YourCaps;
				break;
			case B_TXT_SCRIPTING_TRAINER:
				 toCpy = (u8*) GetTrainerName(gBattleScripting.bank);
				 break;
			case B_TXT_CAPTURED_MON_NAME:
				GetMonData(GetBankPartyData(gBankTarget), MON_DATA_NICKNAME, text);
				StringGetEnd10(text);
				toCpy = text;
				break;
			case B_TXT_DEF_NAME:
				GetMonData(GetIllusionPartyData(gBankTarget), MON_DATA_NICKNAME, text);
				StringGetEnd10(text);
				toCpy = text;
				break;
			case B_TXT_DEF_PARTNER_NAME:
				GetMonData(GetIllusionPartyData(gBankTarget ^ BIT_FLANK), MON_DATA_NICKNAME, text);
				StringGetEnd10(text);
				toCpy = text;
				break;
			case B_TXT_DEF_PREFIX5:
				if (SIDE(gBankTarget) == B_SIDE_PLAYER)
					toCpy = NULL;
				else
				{
					if (!useTheOpposing)
						toCpy = gText_TheWild; //the wild
					else
						toCpy = sText_FoePkmnPrefix5; //the opposing
				}
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

static u8* StringCopyBattleStringLoader(u8 *dest, const u8 *src)
{
	s32 i;
	s32 limit = MAX_BATTLE_STRING_LOADER_LENGTH;

	for (i = 0; i < limit; i++)
	{
		dest[i] = src[i];

		if (dest[i] == EOS)
			return &dest[i];
	}

	dest[i] = EOS;
	return &dest[i];
}

void BufferMoveNameBattle(u16 move, u8* dst)
{
	if (IsZMove(move))
	{
		//Load elongated move names for Z-Moves
		StringCopy(dst, GetZMoveName(move));
	}
	else if (IsGMaxMove(move))
	{
		dst[0] = PC_G,
		dst[1] = PC_DASH,
		dst[2] = PC_M,
		dst[3] = PC_a,
		dst[4] = PC_x,
		dst[5] = PC_SPACE, //Space
		StringCopy(&dst[6], gMoveNames[move]);
	}
	else if (IsAnyMaxMove(move))
	{
		dst[0] = PC_M,
		dst[1] = PC_a,
		dst[2] = PC_x,
		dst[3] = PC_SPACE, //Space
		StringCopy(&dst[4], gMoveNames[move]);
	}
	else
		StringCopy(dst, gMoveNames[move]);
}

void EmitPrintString(u8 bufferId, u16 stringID)
{
	int i;
	struct BattleMsgData* stringInfo;

	gBattleBuffersTransferData[0] = CONTROLLER_PRINTSTRING;
	gBattleBuffersTransferData[1] = gBattleOutcome;
	gBattleBuffersTransferData[2] = stringID;
	gBattleBuffersTransferData[3] = (stringID & 0xFF00) >> 8;

	stringInfo = (struct BattleMsgData*)(&gBattleBuffersTransferData[4]);
	stringInfo->currentMove = gCurrentMove;
	stringInfo->originallyUsedMove = gChosenMove;
	stringInfo->lastItem = gLastUsedItem;
	stringInfo->lastAbility = gLastUsedAbility;
	stringInfo->scrActive = gBattleScripting.bank;
	stringInfo->unk1605E = gBattleStruct->field_52;
	stringInfo->hpScale = gBattleStruct->hpScale;
	stringInfo->stringBank = gStringBank;
	stringInfo->moveType = gBattleMoves[gCurrentMove].type;
	stringInfo->zMoveActive = gNewBS->zMoveData.active;
	stringInfo->dynamaxActive = gNewBS->dynamaxData.active;

	if (gBattleStringLoader !=  NULL)
		StringCopyBattleStringLoader(stringInfo->battleStringLoader, gBattleStringLoader);

	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
	{
		stringInfo->abilities[i] = *GetAbilityLocation(i);	
		stringInfo->species[i] = GetProperAbilityPopUpSpecies(i);
	}

	for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; i++)
	{
		stringInfo->textBuffs[0][i] = gBattleTextBuff1[i];
		stringInfo->textBuffs[1][i] = gBattleTextBuff2[i];
		stringInfo->textBuffs[2][i] = gBattleTextBuff3[i];
	}
	PrepareBufferDataTransfer(bufferId, gBattleBuffersTransferData, sizeof(struct BattleMsgData) + 4);
}

void EmitPrintSelectionString(u8 bufferId, u16 stringID)
{
	int i;
	struct BattleMsgData* stringInfo;

	gBattleBuffersTransferData[0] = CONTROLLER_PRINTSTRINGPLAYERONLY;
	gBattleBuffersTransferData[1] = CONTROLLER_PRINTSTRINGPLAYERONLY;
	gBattleBuffersTransferData[2] = stringID;
	gBattleBuffersTransferData[3] = (stringID & 0xFF00) >> 8;

	stringInfo = (struct BattleMsgData*)(&gBattleBuffersTransferData[4]);
	stringInfo->currentMove = gCurrentMove;
	stringInfo->originallyUsedMove = gChosenMove;
	stringInfo->lastItem = gLastUsedItem;
	stringInfo->lastAbility = gLastUsedAbility;
	stringInfo->scrActive = gBattleScripting.bank;
	stringInfo->unk1605E = gBattleStruct->field_52;
	stringInfo->zMoveActive = gNewBS->zMoveData.active;
	stringInfo->dynamaxActive = gNewBS->dynamaxData.active;

	if (gBattleStringLoader !=  NULL)
		StringCopyBattleStringLoader(stringInfo->battleStringLoader, gBattleStringLoader);

	for (i = 0; i < MAX_BATTLERS_COUNT; i++)
	{
		stringInfo->abilities[i] = *GetAbilityLocation(i);
		stringInfo->species[i] = GetProperAbilityPopUpSpecies(i);
	}

	for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; i++)
	{
		stringInfo->textBuffs[0][i] = gBattleTextBuff1[i];
		stringInfo->textBuffs[1][i] = gBattleTextBuff2[i];
		stringInfo->textBuffs[2][i] = gBattleTextBuff3[i];
	}

	PrepareBufferDataTransfer(bufferId, gBattleBuffersTransferData, sizeof(struct BattleMsgData) + 4);
}

#ifdef OPEN_WORLD_TRAINERS
static u8* GetOpenWorldTrainerName(bool8 female)
{
	u8 nameId = gSpecialVar_LastTalked * MathMax(1, gSaveBlock1->location.mapGroup) * MathMax(1, gSaveBlock1->location.mapNum);

	if (female)
		return gFemaleFrontierNamesTable[nameId % NUM_FEMALE_NAMES];
	else
		return gMaleFrontierNamesTable[nameId % NUM_MALE_NAMES];
}
#endif

static const u8* GetTrainerClassName(u8 class, unusedArg u8* text)
{
	const u8* className;

	#ifdef UNBOUND
	if (class == CLASS_BLACK_EMBOAR)
	{
		if (VarGet(VAR_SQ_BLACK_EMBOAR) >= 4) //Black Emboar changed name
		{
			StringCopy(text, gText_BlackPlayerPrefix);
			StringAppend(text, gSaveBlock2->playerName);
			className = text;
		}
		else
			className = gTrainerClassNames[class];
	}
	else if (class == CLASS_SHADOW_ADMIN) //Marlon
	{
		extern const u8 gText_TrainerClass_ExShadowAdmin[];
		if (VarGet(VAR_MAIN_STORY) >= 0x50) //MAIN_STORY_SAVED_BORRIUS_CRYSTAL_PEAK
			className = gText_TrainerClass_ExShadowAdmin;
		else
			className = gTrainerClassNames[class];
	}
	else if (class == CLASS_LOR_ADMIN) //Ivory
	{
		if (VarGet(VAR_MAIN_STORY) <= 0x25) //MAIN_STORY_LEFT_CUBE
			className = gTrainerClassNames[CLASS_SHADOW_ADMIN]; //Ivory is still a Shadow Admin at this point
		else
			className = gTrainerClassNames[class];
	}
	else
	#endif
		className = gTrainerClassNames[class];

	return className;
}

static bool8 IsPunctuation(u8 character)
{
	return character == CHAR_PERIOD
		|| character == CHAR_EXCL_MARK
		|| character == CHAR_QUESTION_MARK
		|| character == CHAR_ELLIPSIS;
}

static void FixTheCapitalizationInDisplayedString(void)
{
	//Fixes the capitalization of the word "The" in the middle of a sentence
	u32 i, lastChar, secondLastChar;
	
	for (i = 0, lastChar = EOS, secondLastChar = EOS; gDisplayedStringBattle[i] != EOS; ++i)
	{
		if (lastChar == CHAR_SPACE //Middle of sentence
		&& !IsPunctuation(secondLastChar) //If it is, then a sentence just ended and this should be capital
		&& gDisplayedStringBattle[i + 0] == CHAR_T
		&& gDisplayedStringBattle[i + 1] == CHAR_h
		&& gDisplayedStringBattle[i + 2] == CHAR_e
		&& (gDisplayedStringBattle[i + 3] == CHAR_SPACE
		 || gDisplayedStringBattle[i + 3] == EOS
		 || IsPunctuation(gDisplayedStringBattle[i + 3]))) //Check the end of the word to not affect words starting with "The"
		{
			//Current word is "The" and it's not the beginning of a textbox
			gDisplayedStringBattle[i] = CHAR_t; //Decapitalize the "T"
		}

		secondLastChar = lastChar;
		lastChar = gDisplayedStringBattle[i];
		if (lastChar == CHAR_NEWLINE
		|| lastChar == CHAR_PROMPT_SCROLL
		|| lastChar == CHAR_PROMPT_CLEAR)
			lastChar = CHAR_SPACE; //Consider line breaks like spaces
	}
}
