#include "defines.h"
#include "../include/coins.h"
#include "../include/money.h"
#include "../include/event_data.h"
#include "../include/item.h"
#include "../include/constants/items.h"

#include "../include/new/build_pokemon.h"
#include "../include/new/item.h"
#include "../include/new/util.h"

void DebugMenu_ProcessSetFlag(void)
{
	u32 i;

	switch (gSpecialVar_LastResult) {
		case 0: //Badges
			for (i = FLAG_BADGE01_GET; i <= FLAG_BADGE08_GET; ++i)
				FlagSet(i);
			break;
		case 1: //Game Clear
			FlagSet(FLAG_SYS_GAME_CLEAR);
			break;
		case 2: //Pokedexes
			FlagSet(FLAG_SYS_POKEDEX_GET);
			FlagSet(FLAG_SYS_DEXNAV);
			break;
		case 3: //Fly Spots
			for (i = 0x890; i <= 0x8CA; ++i)
				FlagSet(i);
			FlagSet(FLAG_SYS_SEVII_MAP_123);
			FlagSet(FLAG_SYS_SEVII_MAP_4567);
			break;
		case 4: //Custom Flag and Var - Modify this number
			//FlagSet(0x152D);
			VarSet(0x500C, 0x28);
			//FlagClear(0x15AC);
			break;
	}
}

void DebugMenu_ProcessGiveItem(void)
{
	u32 i;

	switch (gSpecialVar_LastResult) {
		case 0: //Useful Key Items
			AddBagItem(ITEM_BICYCLE, 1);
			AddBagItem(ITEM_TOWN_MAP, 1);
			AddBagItem(ITEM_OLD_ROD, 1);
			AddBagItem(ITEM_GOOD_ROD, 1);
			AddBagItem(ITEM_SUPER_ROD, 1);
			AddBagItem(ITEM_VS_SEEKER, 1);
			AddBagItem(ITEM_ITEMFINDER, 1);
			AddBagItem(ITEM_MEGA_RING, 1);
			break;
		case 1: //General Useful Items
			AddBagItem(ITEM_MAX_REPEL, 100);
			AddBagItem(ITEM_ESCAPE_ROPE, 100);
			AddBagItem(ITEM_HEART_SCALE, 100);
			#if (defined ITEM_HM01_CUT && defined ITEM_HM08_ROCK_CLIMB)
			for (i = ITEM_HM01_CUT; i <= ITEM_HM08_ROCK_CLIMB; ++i)
				AddBagItem(i, 1);
			#endif

			AddBagItem(ITEM_RED_SHARD, 100);
			AddBagItem(ITEM_BLUE_SHARD, 100);
			AddBagItem(ITEM_YELLOW_SHARD, 100);
			AddBagItem(ITEM_GREEN_SHARD, 100);
			break;
		case 2: //Poke Balls
			for (i = ITEM_MASTER_BALL; i <= ITEM_PREMIER_BALL; ++i)
				AddBagItem(i, 100);

			AddBagItem(ITEM_PARK_BALL, 100);
			AddBagItem(ITEM_CHERISH_BALL, 100);
			AddBagItem(ITEM_DUSK_BALL, 100);
			AddBagItem(ITEM_HEAL_BALL, 100);
			AddBagItem(ITEM_QUICK_BALL, 100);
			AddBagItem(ITEM_FAST_BALL, 100);
			AddBagItem(ITEM_LEVEL_BALL, 100);
			AddBagItem(ITEM_LURE_BALL, 100);
			AddBagItem(ITEM_HEAVY_BALL, 100);
			AddBagItem(ITEM_LOVE_BALL, 100);
			AddBagItem(ITEM_FRIEND_BALL, 100);
			AddBagItem(ITEM_MOON_BALL, 100);
			AddBagItem(ITEM_SPORT_BALL, 100);
			AddBagItem(ITEM_BEAST_BALL, 100);
			AddBagItem(ITEM_DREAM_BALL, 100);
			break;
		case 3: //Berries
			for (i = ITEM_CHERI_BERRY; i <= ITEM_STARF_BERRY; ++i)
				AddBagItem(i, 100);
			
			AddBagItem(ITEM_ENIGMA_BERRY, 100);
			AddBagItem(ITEM_OCCA_BERRY, 100);
			AddBagItem(ITEM_PASSHO_BERRY, 100);
			AddBagItem(ITEM_WACAN_BERRY, 100);
			AddBagItem(ITEM_RINDO_BERRY, 100);
			AddBagItem(ITEM_YACHE_BERRY, 100);
			AddBagItem(ITEM_CHOPLE_BERRY, 100);
			AddBagItem(ITEM_KEBIA_BERRY, 100);
			AddBagItem(ITEM_SHUCA_BERRY, 100);
			AddBagItem(ITEM_COBA_BERRY, 100);
			AddBagItem(ITEM_PAYAPA_BERRY, 100);
			AddBagItem(ITEM_TANGA_BERRY, 100);
			AddBagItem(ITEM_CHARTI_BERRY, 100);
			AddBagItem(ITEM_KASIB_BERRY, 100);
			AddBagItem(ITEM_HABAN_BERRY, 100);
			AddBagItem(ITEM_COLBUR_BERRY, 100);
			AddBagItem(ITEM_BABIRI_BERRY, 100);
			AddBagItem(ITEM_CHILAN_BERRY, 100);
			AddBagItem(ITEM_MICLE_BERRY, 100);
			AddBagItem(ITEM_CUSTAP_BERRY, 100);
			AddBagItem(ITEM_JABOCA_BERRY, 100);
			AddBagItem(ITEM_ROWAP_BERRY, 100);
			AddBagItem(ITEM_ROSELI_BERRY, 100);
			AddBagItem(ITEM_KEE_BERRY, 100);
			AddBagItem(ITEM_MARANGA_BERRY, 100);
			break;
		case 4: //TMs & HMs
			#ifdef UNBOUND //Remove if you want this, enums can't be #ifdefed
			for (i = ITEM_TM01_FOCUS_PUNCH; i <= ITEM_TM50_OVERHEAT; ++i)
				AddBagItem(i, 1);

			for (i = ITEM_TM51_ROOST; i <= ITEM_TM58_ENDURE; ++i)
				AddBagItem(i, 1);

			for (i = ITEM_TM59_DRAGON_PULSE; i <= ITEM_TM120_NATURE_POWER; ++i)
				AddBagItem(i, 1);

			for (i = ITEM_HM01_CUT; i <= ITEM_HM08_ROCK_CLIMB; ++i)
				AddBagItem(i, 1);
			#endif
			break;
		case 5: //All items
			for (i = 0; i < ITEMS_COUNT; ++i)
			{
				const u8* name = ItemId_GetName(i);
				if (name[0] != 0xAC && name[0] != 0xFF) //'?', ' '
					AddBagItem(i, 1);
			}
			break;
	}
}

void DebugMenu_SetTeamToLevel100(void)
{
	for (u32 i = 0; i < PARTY_SIZE; ++i)
	{
		u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL);
		if (species != SPECIES_NONE && species != SPECIES_EGG)
		{
			u32 exp = gExperienceTables[gBaseStats[species].growthRate][MAX_LEVEL];
			SetMonData(&gPlayerParty[i], MON_DATA_EXP, &exp);
			CalculateMonStats(&gPlayerParty[i]);
			HealMon(&gPlayerParty[i]);
		}
	}
}

void DebugMenu_MaxMoneyAndCoins(void)
{
	AddMoney(&gSaveBlock1->money, 0xFFFFFFFF);
	SetCoins(999999999);
}

void DebugMenu_ShinyTeam(void)
{
	for (u32 i = 0; i < PARTY_SIZE; ++i)
	{
		u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL);
		if (species != SPECIES_NONE && species != SPECIES_EGG)
			ForceMonShiny(&gPlayerParty[i]);
	}
}
