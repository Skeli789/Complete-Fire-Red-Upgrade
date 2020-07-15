#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"
#include "../include/field_weather.h"
#include "../include/random.h"
#include "../include/script.h"
#include "../include/string_util.h"

#include "../include/new/build_pokemon.h"
#include "../include/new/util.h"
#include "../include/new/frontier.h"
#include "../include/new/mega.h"
#include "../include/new/pokemon_storage_system.h"
/*
frontier.c
	all supporting and master functions for developing a battle frontier

tables to edit:
	gBattleTowerTiers
	gBattleMineFormat1Tiers
	gBattleMineFormat2Tiers
	gBattleMineFormat3Tiers
	gBattleMineTiers
	gBattleCircusTiers
	gBattleFrontierTierNames
	gBattleFrontierFormats
	sBattleCircusEffectDescriptions

*/

extern u8* gMaleFrontierNamesTable[];
extern u8* gFemaleFrontierNamesTable[];

extern const u8 gText_SingleBattle[];
extern const u8 gText_DoubleBattle[];
extern const u8 gText_MultiBattle[];
extern const u8 gText_LinkMultiBattle[];
extern const u8 gText_RandomSingleBattle[];
extern const u8 gText_RandomDoubleBattle[];
extern const u8 gText_RandomMultiBattle[];

extern const u8 gText_BattleTowerStandard[];
extern const u8 gText_NoRestrictions[];
extern const u8 gText_SmogonGen7OU[];
extern const u8 gText_SmogonGen7Uber[];
extern const u8 gText_SmogonLittleCup[];
extern const u8 gText_MiddleCup[];
extern const u8 gText_SmogonMonotype[];
extern const u8 gText_GSCup[];
extern const u8 gText_GSCupCamomons[];
extern const u8 gText_SmogonCamomons[];
extern const u8 gText_UberCamomons[];
extern const u8 gText_LittleCupCamomons[];
extern const u8 gText_MiddleCupCamomons[];
extern const u8 gText_SmogonScalemons[];
extern const u8 gText_Smogon350Cup[];
extern const u8 gText_SmogonAveragemons[];
extern const u8 gText_SmogonBenjaminButterfree[];
extern const u8 gText_MegaBrawl[];
extern const u8 gText_DynamaxStandard[];
extern const u8 gText_NationalDexOU[];
extern const u8 gText_BattleMineFormat1[];
extern const u8 gText_BattleMineFormat2[];
extern const u8 gText_BattleMineFormat3[];

extern const u8 gText_On[];
extern const u8 gText_Off[];
extern const u8 gText_BeAble[];
extern const u8 gText_NotBeAble[];
extern const u8 gText_Previous[];
extern const u8 gText_Max[];
extern const u8 gText_None[];

extern const u8 gText_BattleCircusDescriptionElectricTerrain[];
extern const u8 gText_BattleCircusDescriptionGrassyTerrain[];
extern const u8 gText_BattleCircusDescriptionMistyTerrain[];
extern const u8 gText_BattleCircusDescriptionPsychicTerrain[];
extern const u8 gText_BattleCircusDescriptionRain[];
extern const u8 gText_BattleCircusDescriptionSun[];
extern const u8 gText_BattleCircusDescriptionSandstorm[];
extern const u8 gText_BattleCircusDescriptionHail[];
extern const u8 gText_BattleCircusDescriptionFog[];
extern const u8 gText_BattleCircusDescriptionTrickRoom[];
extern const u8 gText_BattleCircusDescriptionMagicRoom[];
extern const u8 gText_BattleCircusDescriptionWonderRoom[];
extern const u8 gText_BattleCircusDescriptionGravity[];
extern const u8 gText_BattleCircusDescriptionIonDeluge[];
extern const u8 gText_BattleCircusDescriptionFairyLock[];
extern const u8 gText_BattleCircusDescriptionMudSport[];
extern const u8 gText_BattleCircusDescriptionWaterSport[];
extern const u8 gText_BattleCircusDescriptionInverseBattle[];
extern const u8 gText_BattleCircusDescriptionDynamax[];
extern const u8 gText_BattleCircusDescriptionTradeMon[];
extern const u8 gText_BattleCircusDescriptionSafeguard[];
extern const u8 gText_BattleCircusDescriptionMist[];
extern const u8 gText_BattleCircusDescriptionSeaOfFire[];
extern const u8 gText_BattleCircusDescriptionRainbow[];
extern const u8 gText_BattleCircusDescriptionConfused[];
extern const u8 gText_BattleCircusDescriptionTaunt[];
extern const u8 gText_BattleCircusDescriptionTorment[];
extern const u8 gText_BattleCircusDescriptionHealBlock[];
extern const u8 gText_BattleCircusDescriptionThroatChop[];
extern const u8 gText_BattleCircusDescriptionAlwaysCrit[];
extern const u8 gText_BattleCircusDescriptionNoCrits[];
extern const u8 gText_BattleCircusDescriptionAbilitySuppression[];

const u8 gBattleTowerTiers[] =
{
	BATTLE_FACILITY_STANDARD,
	BATTLE_FACILITY_NO_RESTRICTIONS,
	BATTLE_FACILITY_OU,
	BATTLE_FACILITY_UBER,
	BATTLE_FACILITY_LITTLE_CUP,
	BATTLE_FACILITY_MIDDLE_CUP,
	BATTLE_FACILITY_MONOTYPE,
	BATTLE_FACILITY_DYNAMAX_STANDARD,
};

const u8 gNumBattleTowerTiers = ARRAY_COUNT(gBattleTowerTiers);

const u8 gBattleMineFormat1Tiers[] =
{
	BATTLE_FACILITY_OU,
	BATTLE_FACILITY_CAMOMONS,
	BATTLE_FACILITY_BENJAMIN_BUTTERFREE,
};

const u8 gBattleMineFormat2Tiers[] =
{
	BATTLE_FACILITY_SCALEMONS,
	BATTLE_FACILITY_350_CUP,
	BATTLE_FACILITY_AVERAGE_MONS,
};

const u8 gBattleMineFormat3Tiers[] =
{
	BATTLE_FACILITY_LITTLE_CUP,
	BATTLE_FACILITY_LC_CAMOMONS,
};

const u8 gBattleMineTiers[] =
{
	BATTLE_MINE_FORMAT_1,
	BATTLE_MINE_FORMAT_2,
	BATTLE_MINE_FORMAT_3,
};

const u8 gNumBattleMineTiers = ARRAY_COUNT(gBattleMineTiers);

const u8 gBattleCircusTiers[] =
{
	BATTLE_FACILITY_STANDARD,
	BATTLE_FACILITY_NO_RESTRICTIONS,
	BATTLE_FACILITY_MONOTYPE,
	BATTLE_FACILITY_CAMOMONS,
	BATTLE_FACILITY_UBER_CAMOMONS,
	BATTLE_FACILITY_LC_CAMOMONS,
	BATTLE_FACILITY_MC_CAMOMONS,
	BATTLE_FACILITY_SCALEMONS,
	BATTLE_FACILITY_350_CUP,
	BATTLE_FACILITY_AVERAGE_MONS,
	BATTLE_FACILITY_BENJAMIN_BUTTERFREE,
	BATTLE_FACILITY_DYNAMAX_STANDARD,
	BATTLE_FACILITY_NATIONAL_DEX_OU,
};

const u8 gNumBattleCircusTiers = ARRAY_COUNT(gBattleCircusTiers);

const u8* const gBattleFrontierTierNames[NUM_TIERS] =
{
	[BATTLE_FACILITY_STANDARD] = gText_BattleTowerStandard,
	[BATTLE_FACILITY_NO_RESTRICTIONS] = gText_NoRestrictions,
	[BATTLE_FACILITY_OU] = gText_SmogonGen7OU,
	[BATTLE_FACILITY_UBER] gText_SmogonGen7Uber,
	[BATTLE_FACILITY_LITTLE_CUP] = gText_SmogonLittleCup,
	[BATTLE_FACILITY_MIDDLE_CUP] = gText_MiddleCup,
	[BATTLE_FACILITY_MONOTYPE] = gText_SmogonMonotype,
	[BATTLE_FACILITY_CAMOMONS] = gText_SmogonCamomons,
	[BATTLE_FACILITY_UBER_CAMOMONS] = gText_UberCamomons,
	[BATTLE_FACILITY_LC_CAMOMONS] = gText_LittleCupCamomons,
	[BATTLE_FACILITY_MC_CAMOMONS] = gText_MiddleCupCamomons,
	[BATTLE_FACILITY_SCALEMONS] = gText_SmogonScalemons,
	[BATTLE_FACILITY_350_CUP] = gText_Smogon350Cup,
	[BATTLE_FACILITY_AVERAGE_MONS] = gText_SmogonAveragemons,
	[BATTLE_FACILITY_BENJAMIN_BUTTERFREE] = gText_SmogonBenjaminButterfree,
	[BATTLE_FACILITY_MEGA_BRAWL] = gText_MegaBrawl,
	[BATTLE_FACILITY_DYNAMAX_STANDARD] = gText_DynamaxStandard,
	[BATTLE_FACILITY_NATIONAL_DEX_OU] = gText_NationalDexOU,
	[BATTLE_MINE_FORMAT_1] = gText_BattleMineFormat1,
	[BATTLE_MINE_FORMAT_2] = gText_BattleMineFormat2,
	[BATTLE_MINE_FORMAT_3] = gText_BattleMineFormat3,
};

const u8* const gBattleFrontierFormats[NUM_TOWER_BATTLE_TYPES] =
{
	[BATTLE_FACILITY_SINGLE] = gText_SingleBattle,
	[BATTLE_FACILITY_DOUBLE] = gText_DoubleBattle,
	[BATTLE_FACILITY_MULTI] = gText_MultiBattle,
	[BATTLE_FACILITY_LINK_MULTI] = gText_LinkMultiBattle,
	[BATTLE_FACILITY_SINGLE_RANDOM] = gText_RandomSingleBattle,
	[BATTLE_FACILITY_DOUBLE_RANDOM] = gText_RandomDoubleBattle,
	[BATTLE_FACILITY_MULTI_RANDOM] = gText_RandomMultiBattle,
};

const u8* const sBattleCircusEffectDescriptions[] =
{
	gText_BattleCircusDescriptionElectricTerrain,
	gText_BattleCircusDescriptionGrassyTerrain,
	gText_BattleCircusDescriptionMistyTerrain,
	gText_BattleCircusDescriptionPsychicTerrain,
	gText_BattleCircusDescriptionRain,
	gText_BattleCircusDescriptionSun,
	gText_BattleCircusDescriptionSandstorm,
	gText_BattleCircusDescriptionHail,
	gText_BattleCircusDescriptionFog,
	gText_BattleCircusDescriptionTrickRoom,
	gText_BattleCircusDescriptionMagicRoom,
	gText_BattleCircusDescriptionWonderRoom,
	gText_BattleCircusDescriptionGravity,
	gText_BattleCircusDescriptionIonDeluge,
	gText_BattleCircusDescriptionFairyLock,
	gText_BattleCircusDescriptionMudSport,
	gText_BattleCircusDescriptionWaterSport,
	gText_BattleCircusDescriptionInverseBattle,
	gText_BattleCircusDescriptionDynamax,
	gText_BattleCircusDescriptionTradeMon,
	//Side Effects
	gText_BattleCircusDescriptionSafeguard,
	gText_BattleCircusDescriptionMist,
	gText_BattleCircusDescriptionSeaOfFire,
	gText_BattleCircusDescriptionRainbow,
	//Personal Effects
	gText_BattleCircusDescriptionConfused,
	gText_BattleCircusDescriptionTaunt,
	gText_BattleCircusDescriptionTorment,
	gText_BattleCircusDescriptionHealBlock,
	gText_BattleCircusDescriptionThroatChop,
	gText_BattleCircusDescriptionAlwaysCrit,
	gText_BattleCircusDescriptionNoCrits,
	gText_BattleCircusDescriptionAbilitySuppression,
};

//This file's functions:
static u8 AdjustLevelForTier(u8 level, u8 tier);
static void LoadProperStreakData(u8* facilityNum, u8* currentOrMax, u8* battleStyle, u8* tier, u8* partySize, u8* level);

u8 GetFrontierTrainerClassId(u16 trainerId, u8 battlerNum)
{
	switch (trainerId) {
		case BATTLE_TOWER_TID:
			return gTowerTrainers[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].trainerClass;
		case BATTLE_TOWER_SPECIAL_TID:
			return gSpecialTowerTrainers[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].trainerClass;
		case FRONTIER_BRAIN_TID:
			return gFrontierBrains[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].trainerClass;
		case BATTLE_FACILITY_MULTI_TRAINER_TID:
			return gFrontierMultiBattleTrainers[VarGet(VAR_FACILITY_TRAINER_ID_PARTNER)].trainerClass;
		case RAID_BATTLE_MULTI_TRAINER_TID:
			return gRaidPartners[VarGet(VAR_FACILITY_TRAINER_ID_PARTNER)].trainerClass;
		default:
			return gTrainers[trainerId].trainerClass;
	}
}

bool8 IsFrontierTrainerId(u16 trainerId)
{
	switch (trainerId) {
		case BATTLE_TOWER_TID:
		case BATTLE_TOWER_SPECIAL_TID:
		case FRONTIER_BRAIN_TID:
		case BATTLE_FACILITY_MULTI_TRAINER_TID:
		case RAID_BATTLE_MULTI_TRAINER_TID:
			return TRUE;
	}
	
	return FALSE;
}

void CopyFrontierTrainerName(u8* dst, u16 trainerId, u8 battlerNum)
{
	int i;
	const u8* name = GetFrontierTrainerName(trainerId, battlerNum);

	for (i = 0; name[i] != EOS; ++i)
		dst[i] = name[i];

	dst[i] = EOS;
}

const u8* GetFrontierTrainerName(u16 trainerId, u8 battlerNum)
{
	const u8* name;
	u16 partnerId;

	switch (trainerId) {
		case BATTLE_TOWER_TID: ;
			u16 nameId = (battlerNum == 0) ? VarGet(VAR_BATTLE_FACILITY_TRAINER1_NAME) : VarGet(VAR_BATTLE_FACILITY_TRAINER2_NAME);

			if (gTowerTrainers[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].gender == BATTLE_FACILITY_MALE)
			{
				if (nameId == 0xFFFF)
					nameId = Random() % NUM_MALE_NAMES;

				name = gMaleFrontierNamesTable[nameId];
			}
			else
			{
				if (nameId == 0xFFFF)
					nameId = Random() % NUM_FEMALE_NAMES;

				name = gFemaleFrontierNamesTable[nameId];
			}

			if (battlerNum == 0)
				VarSet(VAR_BATTLE_FACILITY_TRAINER1_NAME, nameId);
			else
				VarSet(VAR_BATTLE_FACILITY_TRAINER2_NAME, nameId);
			break;
		case BATTLE_TOWER_SPECIAL_TID:
			name = gSpecialTowerTrainers[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].name;
			break;
		case FRONTIER_BRAIN_TID:
			name = gFrontierBrains[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].name;
			break;
		case BATTLE_FACILITY_MULTI_TRAINER_TID:
			partnerId = VarGet(VAR_FACILITY_TRAINER_ID_PARTNER);
			name = TryGetRivalNameByTrainerClass(gFrontierMultiBattleTrainers[partnerId].trainerClass);

			if (name == NULL) //Rival name isn't tied to a trainer class
				name = gFrontierMultiBattleTrainers[partnerId].name;
			break;
		case RAID_BATTLE_MULTI_TRAINER_TID:
			partnerId = VarGet(VAR_FACILITY_TRAINER_ID_PARTNER);
			name = TryGetRivalNameByTrainerClass(gRaidPartners[partnerId].trainerClass);

			if (name == NULL) //Rival name isn't tied to a trainer class
				name = gRaidPartners[partnerId].name;
			break;
		default:
			name = TryGetRivalNameByTrainerClass(gTrainers[trainerId].trainerClass);
			if (name == NULL) //Rival name isn't tied to a trainer class
				name = gTrainers[trainerId].trainerName;
			break;
	}

	return ReturnEmptyStringIfNull(name);
}

void CopyFrontierTrainerText(u8 whichText, u16 trainerId, u8 battlerNum)
{
	switch (trainerId) {
		case BATTLE_TOWER_TID:
			switch (whichText) {
				case FRONTIER_BEFORE_TEXT:
					StringCopy(gStringVar4, (gTowerTrainers[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].preBattleText));
					break;

				case FRONTIER_PLAYER_LOST_TEXT:
					StringCopy(gStringVar4, (gTowerTrainers[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].playerLoseText));
					break;

				case FRONTIER_PLAYER_WON_TEXT:
					StringCopy(gStringVar4, (gTowerTrainers[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].playerWinText));
			}
			break;
		case BATTLE_TOWER_SPECIAL_TID:
			switch (whichText) {
				case FRONTIER_BEFORE_TEXT:
					StringCopy(gStringVar4, (gSpecialTowerTrainers[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].preBattleText));
					break;

				case FRONTIER_PLAYER_LOST_TEXT:
					StringCopy(gStringVar4, (gSpecialTowerTrainers[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].playerLoseText));
					break;

				case FRONTIER_PLAYER_WON_TEXT:
					StringCopy(gStringVar4, (gSpecialTowerTrainers[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].playerWinText));
			}
			break;
		case FRONTIER_BRAIN_TID:
		default:
			switch (whichText) {
				case FRONTIER_BEFORE_TEXT:
					if (gFrontierBrains[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].preBattleText != NULL)
						StringCopy(gStringVar4, gFrontierBrains[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].preBattleText);
					break;

				case FRONTIER_PLAYER_LOST_TEXT:
					if (gFrontierBrains[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].playerLoseText != NULL)
						StringCopy(gStringVar4, (gFrontierBrains[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].playerLoseText));
					else //Frontier Brain text can be loaded from the OW
						StringCopy(gStringVar4, GetTrainerAWinText());
					break;

				case FRONTIER_PLAYER_WON_TEXT:
					if (gFrontierBrains[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].playerWinText != NULL)
						StringCopy(gStringVar4, (gFrontierBrains[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].playerWinText));
					else //Frontier Brain text can be loaded from the OW
						StringCopy(gStringVar4, GetTrainerALoseText());
			}
			break;
	}
}

u8 GetFrontierTrainerFrontSpriteId(u16 trainerId, u8 battlerNum)
{
	switch (trainerId) {
		case BATTLE_TOWER_TID:
			return gTowerTrainers[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].trainerSprite;
		case BATTLE_TOWER_SPECIAL_TID:
			return gSpecialTowerTrainers[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].trainerSprite;
		case FRONTIER_BRAIN_TID:
			return gFrontierBrains[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].trainerSprite;
		default:
			return gTrainers[trainerId].trainerPic;
	}
}

u16 TryGetSpecialFrontierTrainerMusic(u16 trainerId, u8 battlerNum)
{
	switch (trainerId) {
		case BATTLE_TOWER_SPECIAL_TID:
			return gSpecialTowerTrainers[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].songId;
		case FRONTIER_BRAIN_TID:
			return gFrontierBrains[VarGet(VAR_FACILITY_TRAINER_ID + battlerNum)].songId;
		default:
			return 0;
	}
}

u32 GetAIFlagsInBattleFrontier(unusedArg u8 bank)
{
	return AI_SCRIPT_CHECK_BAD_MOVE | AI_SCRIPT_CHECK_GOOD_MOVE;
}

u8 GetNumMonsOnTeamInFrontier(void)
{
	if (FlagGet(FLAG_BATTLE_FACILITY))
		return MathMin(MathMax(1, VarGet(VAR_BATTLE_FACILITY_POKE_NUM)), PARTY_SIZE);
	else
		return 3; //Regular multi battle
}

bool8 IsFrontierSingles(u8 battleType)
{
	return battleType == BATTLE_FACILITY_SINGLE
		|| battleType == BATTLE_FACILITY_SINGLE_RANDOM;
}

bool8 IsFrontierDoubles(u8 battleType)
{
	return battleType == BATTLE_FACILITY_DOUBLE
		|| battleType == BATTLE_FACILITY_DOUBLE_RANDOM;
}

bool8 IsFrontierMulti(u8 battleType)
{
	return battleType == BATTLE_FACILITY_MULTI
		|| battleType == BATTLE_FACILITY_MULTI_RANDOM
		|| battleType == BATTLE_FACILITY_LINK_MULTI;
}

bool8 IsRandomBattleTowerBattle()
{
	u8 battleType = VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE);

	return battleType == BATTLE_FACILITY_SINGLE_RANDOM
		|| battleType == BATTLE_FACILITY_DOUBLE_RANDOM
		|| battleType == BATTLE_FACILITY_MULTI_RANDOM;
}

bool8 IsGSCupBattle()
{
	u8 battleType = VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE);

	return (IsFrontierDoubles(battleType) || IsFrontierMulti(battleType))
		&&  VarGet(VAR_BATTLE_FACILITY_TIER) == BATTLE_FACILITY_GS_CUP;
}

bool8 DuplicateItemsAreBannedInTier(u8 tier, u8 battleType)
{
	if (tier == BATTLE_FACILITY_STANDARD
	||  IsMiddleCupTier(tier)
	||  tier == BATTLE_FACILITY_MEGA_BRAWL
	||  tier == BATTLE_FACILITY_DYNAMAX_STANDARD)
		return TRUE;

	return !IsFrontierSingles(battleType) && tier == BATTLE_FACILITY_GS_CUP;
}

bool8 RayquazaCanMegaEvolveInFrontierBattle(void)
{
	return IsGSCupBattle()
		|| VarGet(VAR_BATTLE_FACILITY_TIER) == BATTLE_FACILITY_NO_RESTRICTIONS
		|| IsScaleMonsBattle();
}

bool8 DynamaxAllowedInTier(u8 tier)
{
	switch (tier) {
		case BATTLE_FACILITY_NO_RESTRICTIONS:
		case BATTLE_FACILITY_DYNAMAX_STANDARD:
		case BATTLE_FACILITY_NATIONAL_DEX_OU:
			return TRUE;
	}

	return FALSE;
}

u8 GetBattleFacilityLevel(u8 tier)
{
	return AdjustLevelForTier(VarGet(VAR_BATTLE_FACILITY_POKE_LEVEL), tier);
}

void UpdateTypesForCamomons(u8 bank)
{
	gBattleMons[bank].type1 = gBattleMoves[gBattleMons[bank].moves[0]].type;

	if (gBattleMons[bank].moves[1] != MOVE_NONE)
		gBattleMons[bank].type2 = gBattleMoves[gBattleMons[bank].moves[1]].type;
	else
		gBattleMons[bank].type2 = gBattleMons[bank].type1;
}

u8 GetCamomonsTypeByMon(struct Pokemon* mon, u8 whichType)
{
	if (whichType == 0)
	{
		return gBattleMoves[GetMonData(mon, MON_DATA_MOVE1, NULL)].type;
	}
	else
	{
		u16 move2 = GetMonData(mon, MON_DATA_MOVE2, NULL);
		if (move2 != MOVE_NONE)
			return gBattleMoves[move2].type;

		return gBattleMoves[GetMonData(mon, MON_DATA_MOVE1, NULL)].type;
	}
}

u8 GetCamomonsTypeBySpread(const struct BattleTowerSpread* spread, u8 whichType)
{
	if (whichType == 0)
	{
		return gBattleMoves[spread->moves[0]].type;
	}
	else
	{
		if (spread->moves[1] != MOVE_NONE)
			return gBattleMoves[spread->moves[1]].type;

		return gBattleMoves[spread->moves[0]].type;
	}
}

u8 ShouldReplaceTypesWithCamomons(void)
{
	return (gMain.inBattle && gBattleTypeFlags & BATTLE_TYPE_CAMOMONS) || IsCamomonsBattle();
}

bool8 TryUpdateOutcomeForFrontierBattle(void)
{
	u32 i;
	u32 playerHPCount, enemyHPCount;

	if (gBattleTypeFlags & BATTLE_TYPE_FRONTIER)
	{

		for (playerHPCount = 0, i = 0; i < PARTY_SIZE; ++i)
		{
			if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE
			&& !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, NULL))
				playerHPCount += GetMonData(&gPlayerParty[i], MON_DATA_HP, NULL);
		}

		for (enemyHPCount = 0, i = 0; i < PARTY_SIZE; ++i)
		{
			if (GetMonData(&gEnemyParty[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE
			&& !GetMonData(&gEnemyParty[i], MON_DATA_IS_EGG, NULL))
				enemyHPCount += GetMonData(&gEnemyParty[i], MON_DATA_HP, NULL);
		}

		if (playerHPCount == 0 && enemyHPCount > 0)
			gBattleOutcome |= B_OUTCOME_LOST;
		else if (playerHPCount > 0 && enemyHPCount == 0)
			gBattleOutcome |= B_OUTCOME_WON;
		else if (playerHPCount == 0 && enemyHPCount == 0)
		{
			if (SIDE(gNewBS->lastFainted) == B_SIDE_PLAYER)
				gBattleOutcome |= B_OUTCOME_WON;
			else
				gBattleOutcome |= B_OUTCOME_LOST;
		}

		gBattlescriptCurrInstr += 5;
		return TRUE;
	}

	return FALSE;
}

bool8 ShouldDisablePartyMenuItemsBattleTower(void)
{
	return FlagGet(FLAG_BATTLE_FACILITY);
}

const u8* GetFrontierTierName(u8 tier, u8 format)
{
	const u8* string = gBattleFrontierTierNames[tier];

	if (!IsFrontierSingles(format))
	{
		if (tier == BATTLE_FACILITY_MIDDLE_CUP)
			string = gText_GSCup;
		else if (tier == BATTLE_FACILITY_MC_CAMOMONS)
		string = gText_GSCupCamomons;
	}

	return string;
}

bool8 InBattleSands(void)
{
	return (gBattleTypeFlags & BATTLE_TYPE_BATTLE_SANDS) != 0;
}

bool8 IsCamomonsTier(u8 tier)
{
	return tier == BATTLE_FACILITY_CAMOMONS
		|| tier == BATTLE_FACILITY_UBER_CAMOMONS
		|| tier == BATTLE_FACILITY_LC_CAMOMONS
		|| tier == BATTLE_FACILITY_MC_CAMOMONS;
}

bool8 IsLittleCupTier(u8 tier)
{
	return tier == BATTLE_FACILITY_LITTLE_CUP || tier == BATTLE_FACILITY_LC_CAMOMONS;
}

bool8 IsMiddleCupTier(u8 tier)
{
	return tier == BATTLE_FACILITY_MIDDLE_CUP || tier == BATTLE_FACILITY_MC_CAMOMONS;
}

bool8 IsAverageMonsBattle(void)
{
	return FlagGet(FLAG_BATTLE_FACILITY) && VarGet(VAR_BATTLE_FACILITY_TIER) == BATTLE_FACILITY_AVERAGE_MONS;
}

bool8 Is350CupBattle(void)
{
	return FlagGet(FLAG_BATTLE_FACILITY) && VarGet(VAR_BATTLE_FACILITY_TIER) == BATTLE_FACILITY_350_CUP;
}

bool8 IsScaleMonsBattle(void)
{
	return FlagGet(FLAG_BATTLE_FACILITY) && VarGet(VAR_BATTLE_FACILITY_TIER) == BATTLE_FACILITY_SCALEMONS;
}

bool8 IsCamomonsBattle(void)
{
	return FlagGet(FLAG_BATTLE_FACILITY) && IsCamomonsTier(VarGet(VAR_BATTLE_FACILITY_TIER));
}

bool8 IsBenjaminButterfreeBattle(void)
{
	return (gBattleTypeFlags & BATTLE_TYPE_BENJAMIN_BUTTERFREE) != 0;
}

bool8 AreMegasZMovesBannedInTier(u8 tier)
{
	return tier == BATTLE_FACILITY_DYNAMAX_STANDARD;
}

bool8 IsMegaZMoveBannedBattle(void)
{
	return gBattleTypeFlags & BATTLE_TYPE_TRAINER //Excludes Raid Battles
	&& FlagGet(FLAG_BATTLE_FACILITY)
	&& (AreMegasZMovesBannedInTier(VarGet(VAR_BATTLE_FACILITY_TIER))
	 || (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_DYNAMAX));
}

//@Details: Generates a tower trainer id and name for the requested trainer.
//			Also buffers the trainer name to gStringVar1 (BUFFER1).
//@Inputs:
//		Var8000: 0 = Trainer Opponent A
//				 1 = Trainer Opponent B
//				 2 = Partner Trainer
//		Var8001: 0 = Regular Trainers
//				 1 = Special Trainers
//				 2 = Frontier Brain
//		Var8002: If Var8001 == Frontier Brain: Frontier Brain Id
//@Returns: To given var OW sprite num of generated trainer.
u16 sp052_GenerateFacilityTrainer(void)
{
	u8 battler = Var8000;
	u16 id = Random();

	if (Var8001 == 0)
	{
		id %= NUM_TOWER_TRAINERS;
		VarSet(VAR_FACILITY_TRAINER_ID + battler, id);

		if (gTowerTrainers[id].gender == BATTLE_FACILITY_MALE)
		{
			if (battler == 0)
				VarSet(VAR_BATTLE_FACILITY_TRAINER1_NAME, Random() % NUM_MALE_NAMES);
			else
				VarSet(VAR_BATTLE_FACILITY_TRAINER2_NAME, Random() % NUM_MALE_NAMES);
		}
		else
		{
			if (battler == 0)
				VarSet(VAR_BATTLE_FACILITY_TRAINER1_NAME, Random() % NUM_FEMALE_NAMES);
			else
				VarSet(VAR_BATTLE_FACILITY_TRAINER2_NAME, Random() % NUM_FEMALE_NAMES);
		}

		StringCopy(gStringVar1, GetFrontierTrainerName(BATTLE_TOWER_TID, battler));
		return gTowerTrainers[id].owNum;
	}
	else if (Var8001 == 1)
	{
		u8 tier = VarGet(VAR_BATTLE_FACILITY_TIER);

		do
		{
			id = Random();
			id %= NUM_SPECIAL_TOWER_TRAINERS;
		}	while (tier == BATTLE_FACILITY_MONOTYPE && !gSpecialTowerTrainers[id].isMonotype);

		VarSet(VAR_FACILITY_TRAINER_ID + battler, id);
		StringCopy(gStringVar1, GetFrontierTrainerName(BATTLE_TOWER_SPECIAL_TID, battler));
		return gSpecialTowerTrainers[id].owNum;
	}
	else
	{
		id = Var8002;
		VarSet(VAR_FACILITY_TRAINER_ID + battler, id);
		StringCopy(gStringVar1, GetFrontierTrainerName(FRONTIER_BRAIN_TID, battler));
		return gFrontierBrains[id].owNum;
	}
}

//@Details: Loads the battle intro message of the requested trainer.
//@Inputs:
//		Var8000: 0 = Trainer Opponent A
//				 1 = Trainer Opponent B
//		Var8001: 0 = Regular Trainers
//				 1 = Special Trainers
//				 2 = Frontier Brain
void sp053_LoadFrontierIntroBattleMessage(void)
{
	u8 gender;
	u16 id = VarGet(VAR_FACILITY_TRAINER_ID + Var8000);

	const u8* text;
	if (Var8001 == 0)
	{
		text = gTowerTrainers[id].preBattleText;
		gender = gTowerTrainers[id].gender;
	}
	else if (Var8001 == 1)
	{
		text = gSpecialTowerTrainers[id].preBattleText;
		gender = gSpecialTowerTrainers[id].gender;
	}
	else
	{
		text = gFrontierBrains[id].preBattleText;
		gender = gFrontierBrains[id].gender;
	}

	gLoadPointer = text;

	//Change text colour
	if (gender == BATTLE_FACILITY_MALE)
	{
		gTextColourBackup = gTextColourCurrent;
		gTextColourCurrent = 0; //Blue
	}
	else
	{
		gTextColourBackup = gTextColourCurrent;
		gTextColourCurrent = 1; //Red
	}
}

//u16 Streaks[BATTLE_STYLE (4)][TIER (6)][PARTY_SIZE (2)][LEVEL (4)][CURRENT_OR_MAX (2)]

//@Details: Gets the streak for the requested Battle Tower format.
//@Input:
//		Var8000: 0 = Current Streak
//				 1 = Max Streak
//		Var8001: 0xFFFF = Load Style From Var
//				 0+ = Given Style
//		Var8002: 0xFFFF = Load Tier From Var
//				 0+ = Given Tier
//		Var8003: 1 - 6 = Party Size (Options are split into 6 v 6 and NOT 6 v 6)
//				 0xFFFF = Load Party Size From Var
//		Var8004: 0 = Load level from var
//				 1+ = Given Level
u16 sp054_GetBattleFacilityStreak(void)
{
	return GetBattleTowerStreak(Var8000, Var8001, Var8002, Var8003, Var8004);
}

u16 GetCurrentBattleTowerStreak(void)
{
	return GetBattleTowerStreak(CURR_STREAK, 0xFFFF, 0xFFFF, 0xFFFF, 0);
}

u16 GetBattleMineStreak(u8 type, u8 tier)
{
	return GetBattleTowerStreak(type, 0xFFFF, tier, 0xFFFF, 0);
}

u16 GetMaxBattleTowerStreakForTier(u8 tier)
{
	u8 battleType, level, partySize;
	u16 streak = 0;
	u16 max = 0;

	for (battleType = 0; battleType < NUM_TOWER_BATTLE_TYPES; ++battleType)
	{
		for (level = 50; level <= 100; level += 50)
		{
			for (partySize = 3; partySize <= 6; partySize += 3) //3 represents one record, 6 represents another
			{
				streak = GetBattleTowerStreak(MAX_STREAK, battleType, tier, partySize, level);

				if (streak > max)
					max = streak;
			}
		}
	}

	return max;
}

static u8 AdjustLevelForTier(u8 level, u8 tier)
{
	if (IsLittleCupTier(tier))
		return 5;

	if (IS_SINGLE_100_RECORD_TIER(tier))
		return 100;

	return level;
}

u16 GetBattleTowerStreak(u8 currentOrMax, u16 inputBattleStyle, u16 inputTier, u16 partySize, u8 level)
{
	u8 facilityNum = BATTLE_FACILITY_NUM;
	u8 battleStyle = (inputBattleStyle == 0xFFFF) ? VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE) : inputBattleStyle;
	u8 tier = (inputTier == 0xFFFF) ? VarGet(VAR_BATTLE_FACILITY_TIER) : inputTier;
	u8 size = (partySize == 0xFFFF) ? VarGet(VAR_BATTLE_FACILITY_POKE_NUM) : partySize;
	level = (level == 0) ? VarGet(VAR_BATTLE_FACILITY_POKE_LEVEL) : level;
	level = AdjustLevelForTier(level, tier);

	LoadProperStreakData(&facilityNum, &currentOrMax, &battleStyle, &tier, &size, &level);

	switch (facilityNum) {
		case IN_BATTLE_TOWER:
		default:
			return gBattleTowerStreaks[battleStyle][tier][size][level][currentOrMax];
		case IN_BATTLE_SANDS:
			return gBattleSandsStreaks[currentOrMax].streakLength;
		case IN_BATTLE_MINE:
			return gBattleMineStreaks[tier][currentOrMax];
		case IN_BATTLE_CIRCUS:
			return gBattleCircusStreaks[tier][battleStyle][size][level][currentOrMax];
	}
}

//@Details: Updates the streak for the current Battle Tower format.
//@Input:
//		Var8000: 0 = Increment by 1
//				 1 = Reset
void sp055_UpdateBattleFacilityStreak(void)
{
	u8 facilityNum = BATTLE_FACILITY_NUM;
	u8 dummy = 0;
	u8 battleStyle = VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE);
	u8 tier = VarGet(VAR_BATTLE_FACILITY_TIER);
	u8 partySize = VarGet(VAR_BATTLE_FACILITY_POKE_NUM);
	u8 level = AdjustLevelForTier(VarGet(VAR_BATTLE_FACILITY_POKE_LEVEL), tier);
	LoadProperStreakData(&facilityNum, &dummy, &battleStyle, &tier, &partySize, &level);

	u16* currentStreak, *maxStreak;
	bool8 inBattleSands = FALSE;

	switch (facilityNum) {
		case IN_BATTLE_TOWER:
		default:
			currentStreak = &gBattleTowerStreaks[battleStyle][tier][partySize][level][CURR_STREAK]; //Current Streak
			maxStreak = &gBattleTowerStreaks[battleStyle][tier][partySize][level][MAX_STREAK]; //Max Streak
			break;
		case IN_BATTLE_SANDS:
			currentStreak = &gBattleSandsStreaks[CURR_STREAK].streakLength;
			maxStreak = &gBattleSandsStreaks[MAX_STREAK].streakLength;
			inBattleSands = TRUE;
			break;
		case IN_BATTLE_MINE:
			currentStreak = &gBattleMineStreaks[tier][CURR_STREAK];
			maxStreak = &gBattleMineStreaks[tier][MAX_STREAK];
			break;
		case IN_BATTLE_CIRCUS:
			currentStreak = &gBattleCircusStreaks[tier][battleStyle][partySize][level][CURR_STREAK]; //Current Streak
			maxStreak = &gBattleCircusStreaks[tier][battleStyle][partySize][level][MAX_STREAK]; //Max Streak
			break;
	}

	switch (Var8000) {
		case 0:
			if (*currentStreak < 0xFFFF) //Prevent overflow
				*currentStreak += 1;

			if (*maxStreak < *currentStreak)
			{
				*maxStreak = *currentStreak;
				if (GetGameStat(GAME_STAT_BATTLE_FACILITY_BEST_STREAK) < *currentStreak)
					SetGameStat(GAME_STAT_BATTLE_FACILITY_BEST_STREAK, *currentStreak);

				if (inBattleSands)
				{
					gBattleSandsStreaks[MAX_STREAK].tier = VarGet(VAR_BATTLE_FACILITY_TIER); //Actual Tier
					gBattleSandsStreaks[MAX_STREAK].format = battleStyle;
					gBattleSandsStreaks[MAX_STREAK].level = level;
					#ifdef FLAG_INVERSE
					gBattleSandsStreaks[MAX_STREAK].inverse = FlagGet(FLAG_INVERSE);
					#endif
					gBattleSandsStreaks[MAX_STREAK].species1 = GetMonData(&gPlayerParty[0], MON_DATA_SPECIES, NULL);
					gBattleSandsStreaks[MAX_STREAK].species2 = GetMonData(&gPlayerParty[1], MON_DATA_SPECIES, NULL);
					gBattleSandsStreaks[MAX_STREAK].species3 = GetMonData(&gPlayerParty[2], MON_DATA_SPECIES, NULL);
				}
			}
			break;

		case 1:
			*currentStreak = 0; //Rest current streak

			if (inBattleSands)
			{
				gBattleSandsStreaks[CURR_STREAK].tier = VarGet(VAR_BATTLE_FACILITY_TIER); //Actual Tier
				gBattleSandsStreaks[CURR_STREAK].format = battleStyle;
				gBattleSandsStreaks[CURR_STREAK].level = level;
				#ifdef FLAG_INVERSE
				gBattleSandsStreaks[CURR_STREAK].inverse = FlagGet(FLAG_INVERSE);
				#endif
				gBattleSandsStreaks[CURR_STREAK].species1 = GetMonData(&gPlayerParty[0], MON_DATA_SPECIES, NULL);
				gBattleSandsStreaks[CURR_STREAK].species2 = GetMonData(&gPlayerParty[1], MON_DATA_SPECIES, NULL);
				gBattleSandsStreaks[CURR_STREAK].species3 = GetMonData(&gPlayerParty[2], MON_DATA_SPECIES, NULL);
			}
			break;
	}
}

//@Details: Determines the number of battle points to give for
//			the current Battle Tower format.
//@Returns: The number of battle points to give.
u16 sp056_DetermineBattlePointsToGive(void)
{
	u16 toGive;
	u16 streakLength = GetCurrentBattleTowerStreak();

	if (streakLength <= 10)
		toGive = 2;
	else if (streakLength <= 19)
		toGive = 3;
	else if (streakLength == 20)
	{
		toGive = 3; //Just a special trainer

		switch (BATTLE_FACILITY_NUM) {
			case IN_BATTLE_TOWER:
				if (VarGet(VAR_BATTLE_FACILITY_TIER) != BATTLE_FACILITY_MONOTYPE)
					toGive = 20; //Battle against frontier brain
				break;

			case IN_BATTLE_SANDS:
				if (VarGet(VAR_BATTLE_FACILITY_TIER) != BATTLE_FACILITY_MONOTYPE)
					toGive = 20; //Battle against frontier brain
				break;

			case IN_BATTLE_MINE:
				toGive = 20; //Always battle against frontier brain
				break;

			case IN_BATTLE_CIRCUS:
				if (VarGet(VAR_BATTLE_FACILITY_TIER) != BATTLE_FACILITY_MONOTYPE)
					toGive = 20; //Battle against frontier brain
				break;
		}
	}
	else if (streakLength <= 30)
		toGive = 4;
	else if (streakLength <= 40)
		toGive = 5;
	else if (streakLength <= 49)
		toGive = 6;
	else if (streakLength == 50)
	{
		toGive = 6; //Just a special trainer

		switch (BATTLE_FACILITY_NUM) {
			case IN_BATTLE_TOWER:
				if (VarGet(VAR_BATTLE_FACILITY_TIER) != BATTLE_FACILITY_MONOTYPE)
					toGive = 50; //Battle against frontier brain
				break;

			case IN_BATTLE_SANDS:
				if (VarGet(VAR_BATTLE_FACILITY_TIER) != BATTLE_FACILITY_MONOTYPE)
					toGive = 50; //Battle against frontier brain
				break;

			case IN_BATTLE_MINE:
				toGive = 50; //Always battle against frontier brain
				break;

			case IN_BATTLE_CIRCUS:
				if (VarGet(VAR_BATTLE_FACILITY_TIER) != BATTLE_FACILITY_MONOTYPE)
					toGive = 50; //Battle against frontier brain
				break;
		}
	}
	else if (streakLength <= 70)
		toGive = 7;
	else if (streakLength <= 80)
		toGive = 8;
	else if (streakLength <= 99)
		toGive = 9;
	else if (streakLength == 100)
		toGive = 100;
	else if (streakLength == 500)
		toGive = 500;
	//Why not lol, if you get here you deserve it
	else if ((streakLength % 10000) == 0 && streakLength != 0) //Every 10000 wins give 10000
		toGive = 10000;
	else if ((streakLength % 1000) == 0 && streakLength != 0) //Every 1000 wins give 1000
		toGive = 1000;
	else
		toGive = 10;

	return toGive;
}

static void LoadProperStreakData(u8* facilityNum, u8* currentOrMax, u8* battleStyle, u8* tier, u8* partySize, u8* level)
{
	u32 i;

	switch (*facilityNum) {
		case IN_BATTLE_TOWER:
			switch (*tier) {
				case BATTLE_FACILITY_MONOTYPE:
					*tier = BATTLE_FACILITY_LITTLE_CUP; //Hijack Little Cup level 100 slot
					*level = 100;
					break;
				case BATTLE_FACILITY_DYNAMAX_STANDARD:
					*facilityNum = IN_BATTLE_CIRCUS; //Hijack Battle Circus Little Cup level 100 slot
					*tier = BATTLE_FACILITY_LC_CAMOMONS;
					*level = 100;
					return LoadProperStreakData(facilityNum, currentOrMax, battleStyle, tier, partySize, level);
			}

			for (i = 0; i < gNumBattleTowerTiers; ++i)
			{
				if (gBattleTowerTiers[i] == *tier)
				{
					*tier = i;
					break;
				}
			}

			if (i == gNumBattleTowerTiers)
				*tier = 0;
			break;

		case IN_BATTLE_SANDS:
			*tier = MathMin(*tier, gNumBattleTowerTiers);
			break;

		case IN_BATTLE_MINE:
			*tier = MathMin(*tier - BATTLE_MINE_FORMAT_1, 2);
			break;

		case IN_BATTLE_CIRCUS:
			for (i = 0; i < gNumBattleCircusTiers; ++i)
			{
				if (gBattleCircusTiers[i] == *tier)
				{
					*tier = i;
					break;
				}
			}

			if (i == gNumBattleCircusTiers)
				*tier = 0;
			break;
	}

	if (IsFrontierMulti(*battleStyle))
		*partySize *= 2; //Each player gets half the team

	*currentOrMax = MathMin(*currentOrMax, 1);
	*battleStyle = MathMin(*battleStyle, NUM_TOWER_BATTLE_TYPES);
	*partySize = (*partySize < 6) ? 0 : 1;
	*level = (*level < 100) ? 0 : 1;
}

//@Details: To be used after sp06B. Merges the player's choice of partner Pokemon onto their team.
void sp06C_SpliceFrontierTeamWithPlayerTeam(void)
{
	struct Pokemon partnerPokes[3];
	Memset(partnerPokes, 0, sizeof(struct Pokemon) * 3);

	for (int i = 0; i < 3; ++i)
	{
		if (gSelectedOrderFromParty[i] != 0)
			partnerPokes[i] = gPlayerParty[i]; //Player's party has already been remodeled by the special so call indices directly
	}

	RestorePartyFromTempTeam(0, 3);
	Memcpy(&gPlayerParty[3], partnerPokes, sizeof(struct Pokemon) * 3); //Fill second half of team with multi mons

	//Recalculate party count the special way because there may be a gap in the party
	gPlayerPartyCount = 0;
	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE)
			++gPlayerPartyCount;
	}
}

//@Details: Loads any relevant multi trainer data by the given Id value.
//			Also buffers the multi trainer name to gStringVar2 ([BUFFER2] / bufferstring 0x1).
//@Inputs:
//		Var8000: 0xFF - Random Id.
//				 0-0xFE - Given Id.
//@Returns: The OW id of the trainer.
u16 sp06D_LoadFrontierMultiTrainerById(void)
{
	u16 id = Var8000;

	if (id == 0xFF)
		id = Random() % gNumFrontierMultiTrainers;
	else if (id > 0xFF) //Invalid value
		id = 0;

	VarSet(VAR_FACILITY_TRAINER_ID_PARTNER, id);
	VarSet(VAR_PARTNER, BATTLE_FACILITY_MULTI_TRAINER_TID);
	VarSet(VAR_PARTNER_BACKSPRITE, gFrontierMultiBattleTrainers[id].backSpriteId);

	StringCopy(gStringVar2, GetFrontierTrainerName(BATTLE_FACILITY_MULTI_TRAINER_TID, 0));
	return gFrontierMultiBattleTrainers[id].owNum;
}

//@Details: Buffers text relating to battle sands records.
//@Inputs:
//		Var8000: 0 = Previous Streak
//				 1 = Max Streak
//@Returns: LastResult: TRUE if the requested record exists.
//			gStringVar1: Tier name.
//			gStringVar2: Battle format name.
//			gStringVar3: Level.
//			gStringVar7: Inverse on or off.
//			gStringVar8: Species 1.
//			gStringVar9: Species 2.
//			gStringVarA: Species 3.
//			gStringVarB: Streak length.
//			gStringVarC: "previous" or "max"
void sp06E_BufferBattleSandsRecords(void)
{
	static const u8* const requestStrings[] =
	{
		gText_Previous,
		gText_Max,
	};

	gSpecialVar_LastResult = FALSE;

	struct BattleSandsStreak* streak = (Var8000 == 0) ? &gBattleSandsStreaks[CURR_STREAK] : &gBattleSandsStreaks[MAX_STREAK];
	if (streak->species1 != SPECIES_NONE)
	{
		StringCopy(gStringVar1, GetFrontierTierName(streak->tier, streak->format));
		StringCopy(gStringVar2, gBattleFrontierFormats[streak->format]);
		ConvertIntToDecimalStringN(gStringVar3, (streak->level == 0) ? 50 : 100, 0, 3);
		StringCopy(gStringVar7, (streak->inverse) ? gText_On : gText_Off);
		GetSpeciesName(gStringVar8, streak->species1);
		GetSpeciesName(gStringVar9, streak->species2);
		if (IsFrontierSingles(streak->format))
			StringCopy(gStringVarA, gText_None);
		else
			GetSpeciesName(gStringVarA, streak->species3);
		ConvertIntToDecimalStringN(gStringVarB, streak->streakLength, 0, 5);
		StringCopy(gStringVarC, requestStrings[(Var8000 == 0) ? 0 : 1]);

		gSpecialVar_LastResult = TRUE;
	}
}

//@Details: Checks if the player's team can enter the Battle Mine.
//			Also sets the Battle Tower Tier var to the chosen tier.
//@Inputs:
//		Var8000: 0 = Check Battle Mine Format 1.
//				 1 = Check Battle Mine Format 2.
//				 2 = Check Battle Mine Format 3.
//@Returns: LastResult: TRUE if the team can participate.
void sp06F_CanTeamParticipateInBattleMine(void)
{
	int i, j, tier;
	u16 choice = Var8000;
	const u8* tiers = choice == 0 ? gBattleMineFormat1Tiers
					: choice == 1 ? gBattleMineFormat2Tiers
					: gBattleMineFormat3Tiers;
	u8 numTiers = choice == 0 ? ARRAY_COUNT(gBattleMineFormat1Tiers)
				: choice == 1 ? ARRAY_COUNT(gBattleMineFormat2Tiers)
				: ARRAY_COUNT(gBattleMineFormat3Tiers);

	gSpecialVar_LastResult = FALSE;

	//Check if party of 6 where every Pokemon can participate in every tier in the requested format
	for (i = 0; i < PARTY_SIZE; ++i)
	{
		struct Pokemon* mon = &gPlayerParty[i];

		if (GetMonData(mon, MON_DATA_SPECIES, NULL) == SPECIES_NONE
		||  GetMonData(mon, MON_DATA_IS_EGG, NULL))
			return;

		for (j = 0, tier = tiers[j]; j < numTiers; ++j, tier = tiers[j]) //Check every tier in requested format
		{
			u16 varBackup = VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE);

			//Check if mon is banned in singles
			VarSet(VAR_BATTLE_FACILITY_BATTLE_TYPE, BATTLE_FACILITY_SINGLE);
			if (IsMonBannedInTier(mon, tier))
			{
				VarSet(VAR_BATTLE_FACILITY_BATTLE_TYPE, varBackup);
				return;
			}

			//Check if mon is banned in doubles
			VarSet(VAR_BATTLE_FACILITY_BATTLE_TYPE, BATTLE_FACILITY_DOUBLE);
			if (IsMonBannedInTier(mon, tier))
			{
				VarSet(VAR_BATTLE_FACILITY_BATTLE_TYPE, varBackup);
				return;
			}

			VarSet(VAR_BATTLE_FACILITY_BATTLE_TYPE, varBackup);
		}
	}

	VarSet(VAR_BATTLE_FACILITY_TIER, BATTLE_MINE_FORMAT_1 + MathMin(choice, 2));
	gSpecialVar_LastResult = TRUE;
}

//@Details: Randomizes various battle options for a battle in the Battle Mine.
//@Returns: To given var the original tier to back up.
//			gStringVar7: Tier name.
//			gStringVar8: Battle format name.
//			gStringVar9: Level.
//			gStringVarA: Party size.
//			gStringVarB: Inverse on or off.
//			gStringVarC: Dynamax on or off.
u8 sp070_RandomizeBattleMineBattleOptions(void)
{
	u8 format, tier, level, partySize, inverse, dynamax;

	u8 originalTier = VarGet(VAR_BATTLE_FACILITY_TIER);
	const u8* tiers = originalTier == BATTLE_MINE_FORMAT_1 ? gBattleMineFormat1Tiers
					: originalTier == BATTLE_MINE_FORMAT_2 ? gBattleMineFormat2Tiers
					: gBattleMineFormat3Tiers;
	u8 numTiers = originalTier == BATTLE_MINE_FORMAT_1 ? ARRAY_COUNT(gBattleMineFormat1Tiers)
				: originalTier == BATTLE_MINE_FORMAT_2 ? ARRAY_COUNT(gBattleMineFormat2Tiers)
				: ARRAY_COUNT(gBattleMineFormat3Tiers);

	u16 streak = GetCurrentBattleTowerStreak();

	//Choose Battle Format
	switch (streak) {
		case 0 ... 29:
			format = BATTLE_FACILITY_SINGLE + (Random() & 1);
			break;
		default: ; //Random Battles become available after battle 30
			u8 randomOption = Random() & 3;
			switch (randomOption) {
				case 0:
				case 1:
					format = BATTLE_FACILITY_SINGLE + (randomOption & 1);
					break;
				case 2:
				default:
					format = BATTLE_FACILITY_SINGLE_RANDOM + (randomOption & 1);
			}
	}

	//Choose Tier
	tier = tiers[Random() % numTiers];

	//Choose Level
	if (IsLittleCupTier(tier))
		level = 5;
	else
	{
		switch (streak) {
			case 0 ... 9:
				level = 50;
				break;
			case 10 ... 18:
				if (Random() & 1)
					level = 50;
				else
					level = MAX_LEVEL;
				break;
			case 19: //Frontier Brain 1
				level = 50;
				break;
			case 49: //Frontier Brain 2
				level = MAX_LEVEL;
				break;
			default:
				level = (Random() % MAX_LEVEL) + 1; //Randomize level completely after battle 20
		}
	}

	//Choose Party Size
	switch (streak) {
		case 0 ... 9:
			if (IsFrontierSingles(format))
				partySize = 3; //3v3
			else
				partySize = 4; //4v4
			break;
		case 11 ... 39:
			partySize = Random() % (PARTY_SIZE - 1) + 2; //2v2 - 6v6

			if (partySize == 2 && !IsFrontierSingles(format))
				partySize = 3; //3v3
			break;
		default:
			partySize = Random() % PARTY_SIZE + 1; //1v1 - 6v6

			if (partySize == 1 && !IsFrontierSingles(format))
				partySize = 2; //2v2
	}

	//Choose Inverse & Dynamax
	switch (streak) {
		case 0 ... 19:
			inverse = FALSE;
			dynamax = FALSE;
			break;
		default:
			inverse = Random() & TRUE;
			dynamax = Random() & TRUE;
	}

	VarSet(VAR_BATTLE_FACILITY_POKE_LEVEL, level);
	VarSet(VAR_BATTLE_FACILITY_BATTLE_TYPE, format);
	VarSet(VAR_BATTLE_FACILITY_TIER, tier);
	VarSet(VAR_BATTLE_FACILITY_POKE_NUM, partySize);
	if (inverse)
	{
		#ifdef FLAG_INVERSE
		FlagSet(FLAG_INVERSE);
		#endif
	}
	if (dynamax)
	{
		#ifdef FLAG_DYNAMAX_BATTLE
		FlagSet(FLAG_DYNAMAX_BATTLE);
		#endif
	}

	StringCopy(gStringVar7, GetFrontierTierName(tier, format));
	StringCopy(gStringVar8, gBattleFrontierFormats[format]);
	ConvertIntToDecimalStringN(gStringVar9, level, 0, 3);
	ConvertIntToDecimalStringN(gStringVarA, partySize, 0, 1);
	StringCopy(gStringVarB, (inverse) ? gText_On : gText_Off);
	StringCopy(gStringVarC, (dynamax) ? gText_BeAble : gText_NotBeAble);

	return originalTier;
}

//@Details: Sets the tier var to the correct tier the Battle Mine streaks are recorded in.
void sp071_LoadBattleMineRecordTier(void)
{
	u32 i, tier;
	u8 currTier = VarGet(VAR_BATTLE_FACILITY_TIER);

	if (currTier == BATTLE_MINE_FORMAT_1 || currTier == BATTLE_MINE_FORMAT_2 || currTier == BATTLE_MINE_FORMAT_3)
		return;

	for (i = 0; i < ARRAY_COUNT(gBattleMineFormat1Tiers); ++i)
	{
		tier = gBattleMineFormat1Tiers[i];
		if (currTier == tier)
		{
			VarSet(VAR_BATTLE_FACILITY_TIER, BATTLE_MINE_FORMAT_1);
			return;
		}
	}

	for (i = 0; i < ARRAY_COUNT(gBattleMineFormat2Tiers); ++i)
	{
		tier = gBattleMineFormat2Tiers[i];
		if (currTier == tier)
		{
			VarSet(VAR_BATTLE_FACILITY_TIER, BATTLE_MINE_FORMAT_2);
			return;
		}
	}

	for (i = 0; i < ARRAY_COUNT(gBattleMineFormat3Tiers); ++i)
	{
		tier = gBattleMineFormat3Tiers[i];
		if (currTier == tier)
		{
			VarSet(VAR_BATTLE_FACILITY_TIER, BATTLE_MINE_FORMAT_3);
			return;
		}
	}
}

//@Details: Loads random effects for Battle Circus battles.
//@Returns: Lastresult: TRUE if all effects loaded.
//			gStringVarC: A description of the effect just activated.
void sp072_LoadBattleCircusEffects(void)
{
	u32 i;
	u8 numActive = 0;
	gSpecialVar_LastResult = TRUE;

	for (i = 0; ; ++i)
	{
		if (gBattleCircusFlags & gBitTable[i])
			++numActive;

		if (gBitTable[i] == LAST_BATTLE_CIRCUS_FLAG)
			break;
	}

	u8 totalEffects = 1;
	bool8 sideEffectsAllowed = FALSE;
	bool8 personalEffectsAllowed = FALSE;
	u16 streak = GetCurrentBattleTowerStreak();

	switch (streak) {
		case 0 ... 9:
			break;
		case 10 ... 19:
			sideEffectsAllowed = TRUE;
			break;
		case 20 ... 29:
			sideEffectsAllowed = TRUE;
			totalEffects = 2;
			break;
		case 30 ... 39:
			sideEffectsAllowed = TRUE;
			personalEffectsAllowed = TRUE;
			totalEffects = 2;
			break;
		case 40 ... 49:
			sideEffectsAllowed = TRUE;
			personalEffectsAllowed = TRUE;
			totalEffects = 3;
			break;
		case 50 ... 70:
			sideEffectsAllowed = TRUE;
			personalEffectsAllowed = TRUE;
			totalEffects = 4;
			break;
		default:
			sideEffectsAllowed = TRUE;
			personalEffectsAllowed = TRUE;
			totalEffects = 5;
			break;
	}

	if (numActive < totalEffects)
	{
		bool8 weatherActive = (gBattleCircusFlags & BATTLE_CIRCUS_WEATHER) != 0;
		bool8 terrainActive = (gBattleCircusFlags & BATTLE_CIRCUS_TERRAIN) != 0;
		bool8 dynamaxActive = DynamaxAllowedInTier(VarGet(VAR_BATTLE_FACILITY_TIER));
		bool8 randomBattleActive = VarGet(VAR_BATTLE_FACILITY_BATTLE_TYPE) >= BATTLE_FACILITY_SINGLE_RANDOM;

		u8 effectNum;
		do
		{
			effectNum = Random() % (i + 1);
		} while (gBattleCircusFlags & gBitTable[effectNum] //Only add non active effects
			|| (weatherActive && gBitTable[effectNum] & BATTLE_CIRCUS_WEATHER) //One weather effect at a time
			|| (terrainActive && gBitTable[effectNum] & BATTLE_CIRCUS_TERRAIN) //One terrain effect at a time
			|| (dynamaxActive && gBitTable[effectNum] & BATTLE_CIRCUS_DYNAMAX) //No point in stacking Dynamax effect
			|| (randomBattleActive && gBitTable[effectNum] & BATTLE_CIRCUS_TRADE_MON) //No point in swapping mons in a random battle
			|| (!sideEffectsAllowed && gBitTable[effectNum] >= FIRST_BATTLE_CIRCUS_SIDE_EFFECT_FLAG && gBitTable[effectNum] <= LAST_BATTLE_CIRCUS_SIDE_EFFECT_FLAG)
			|| (!personalEffectsAllowed && gBitTable[effectNum] >= FIRST_BATTLE_CIRCUS_PERSONAL_EFFECT_FLAG));

		if (gBitTable[effectNum] & BATTLE_CIRCUS_WEATHER)
		{
			u8 weather = 0;
			if (gBitTable[effectNum] == BATTLE_CIRCUS_RAIN)
				weather = WEATHER_RAIN_LIGHT;
			else if (gBitTable[effectNum] == BATTLE_CIRCUS_SUN)
				weather = WEATHER_DROUGHT;
			else if (gBitTable[effectNum] == BATTLE_CIRCUS_SANDSTORM)
				weather = WEATHER_SANDSTORM;
			else if (gBitTable[effectNum] == BATTLE_CIRCUS_HAIL)
				weather = WEATHER_STEADY_SNOW;
			else if (gBitTable[effectNum] == BATTLE_CIRCUS_FOG)
				weather = WEATHER_FOG_2;

			SetSav1Weather(weather); //Followed up by a doweather in the script
		}

		gBattleCircusFlags |= gBitTable[effectNum];
		StringCopy(gStringVarC, sBattleCircusEffectDescriptions[effectNum]);
		gSpecialVar_LastResult = FALSE;
	}
}

void sp073_ModifyTeamForBattleTower(void)
{
	u8 tier = VarGet(VAR_BATTLE_FACILITY_TIER);
	u8 level = (tier == BATTLE_FACILITY_LITTLE_CUP || tier == BATTLE_FACILITY_LC_CAMOMONS) ? 5
			 : (IS_SINGLE_100_RECORD_TIER(tier)) ? 100 : VarGet(VAR_BATTLE_FACILITY_POKE_LEVEL);

	level = MathMax(1, MathMin(level, MAX_LEVEL));
	struct Pokemon* enteredMons = Calloc(sizeof(struct Pokemon) * PARTY_SIZE);

	for (int i = 0; i < PARTY_SIZE && gSelectedOrderFromParty[i] != 0; ++i) //Copy mons entered
	{
		Memcpy(&enteredMons[i], &gPlayerParty[gSelectedOrderFromParty[i] - 1], sizeof(struct Pokemon));

		u16 species = enteredMons[i].species;
		SetMonData(&enteredMons[i], MON_DATA_EXP, &gExperienceTables[gBaseStats[species].growthRate][level]);
		CalculateMonStats(&enteredMons[i]);
		HealMon(&enteredMons[i]);
	}

	Memcpy(gPlayerParty, enteredMons, sizeof(struct Pokemon) * PARTY_SIZE); //Overwrite old team
	Free(enteredMons);
}
