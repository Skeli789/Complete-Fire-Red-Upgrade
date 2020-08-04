#include "defines.h"
#include "../include/bike.h"
#include "../include/field_player_avatar.h"
#include "../include/fieldmap.h"
#include "../include/field_message_box.h"
#include "../include/field_weather.h"
#include "../include/hall_of_fame.h"
#include "../include/item_icon.h"
#include "../include/item_menu.h"
#include "../include/list_menu.h"
#include "../include/map_name_popup.h"
#include "../include/menu.h"
#include "../include/m4a.h"
#include "../include/naming_screen.h"
#include "../include/overworld.h"
#include "../include/pokemon_storage_system.h"
#include "../include/region_map.h"
#include "../include/save.h"
#include "../include/script.h"
#include "../include/script_menu.h"
#include "../include/sound.h"
#include "../include/sprite.h"
#include "../include/string_util.h"
#include "../include/text.h"
#include "../include/wild_encounter.h"
#include "../include/window.h"
#include "../include/event_data.h"
#include "../include/constants/abilities.h"
#include "../include/constants/items.h"
#include "../include/constants/moves.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/songs.h"

#include "../include/new/battle_strings.h"
#include "../include/new/catching.h"
#include "../include/new/dns.h"
#include "../include/new/util.h"
#include "../include/new/item.h"
#include "../include/new_menu_helpers.h"
#include "../include/new/multi.h"
#include "../include/new/overworld.h"
#include "../include/new/pokemon_storage_system.h"
#include "../include/new/ram_locs_battle.h"
#include "../include/new/read_keys.h"
#include "../include/new/roamer.h"
#include "../include/new/text.h"
#include "../include/new/scrolling_multichoice.h"
#include "../include/new/Vanilla_functions_battle.h"
#include "../include/new/wild_encounter.h"
/*
scripting.c
	handles all scripting specials or other functions associated with scripts

NOTES:
	1. Many specials will not work unless you have chosen to expand the save block!
	2. PC selection hack allows a lot all of the attribute getter/setter specials to reference boxed pokemon via var8003
		-var8003 = 1: boxed pokemon (output from pc selection: box num in var8000, slot in var8001)
		-else: menu pokemon (and slot in var8004, etc)
		-Be sure to always set var8003 for data manipulation specials!

tables to edit:
	gFossilImageTable
	sMultichoiceSet1
	gScrollingSets

*/

#define POKERUS_CURED 0x10

extern u8 AddPalRef(u8 Type, u16 PalTag);
extern u8 BuildFrontierParty(pokemon_t* party, u16 trainerNum, bool8 firstTrainer, bool8 ForPlayer, u8 side);

extern const struct SwarmData gSwarmTable[];
extern const species_t gSkyBattleBannedSpeciesList[];

#ifdef AUTO_NAMING_SCREEN_SWAP
static u8 GetTextCaretPosition(void);
#endif

//Pokemon Specials//
///////////////////////////////////////////////////////////////////////////////////

#ifdef FOSSIL_IMAGE_HACK
	struct FossilTable
	{
		struct SpriteSheet* data;
		u16* palette;
	};

	#ifdef EXISTING_FOSSIL_IMAGE_TABLE_ADDRESS
		#define gFossilImageTable ((struct FossilTable*) EXISTING_FOSSIL_IMAGE_TABLE_ADDRESS)
	#else
		//Create a 255 image table
		struct FossilTable gFossilImageTable[] =
		{
			[0] = //Kabutops (originally index 0x8D, now 0x0)
				{
					.data = (struct SpriteSheet*) 	0x83E17C0,
					.palette = (u16*) 				0x83E17A0,
				},
			[1] =
				{ //Aerodactyl (originally index 0x8e, now 0x1)
					.data = (struct SpriteSheet*) 	0x83E17D0,
					.palette = (u16*) 				0x83E0F80,
				},
			[2] =
				{ //Start adding new data here
					.data = (struct SpriteSheet*) 	0x8AAAAAA,
					.palette = (u16*) 				0x8AAAAAA,
				},
		};
	#endif
#endif

enum EVStatChecker
{
	CheckEVs_HP,
	CheckEVs_Atk,
	CheckEVs_Def,
	CheckEVs_Spd,
	CheckEVs_SpAtk,
	CheckEVs_SpDef,
	CheckEVs_Coolness,
	CheckEVs_Beauty,
	CheckEVs_Cuteness,
	CheckEVs_Smartness,
	CheckEVs_Toughness,
	CheckEVs_Luster
};

enum IVStatChecker
{
	CheckIVs_HP,
	CheckIVs_Atk,
	CheckIVs_Def,
	CheckIVs_Spd,
	CheckIVs_SpAtk,
	CheckIVs_SpDef,
	SetAllIVs
};

u32 GetMonDataFromVar8003(u8 dataRequest) {
	u32 attr;
	#ifdef SELECT_FROM_PC
		if (Var8003 == 1)
			attr = GetBoxMonDataAt(Var8000, Var8001, dataRequest);
		else
			attr = GetMonData(&gPlayerParty[Var8004], dataRequest, NULL);
	#else
		attr = GetMonData(&gPlayerParty[Var8004], dataRequest, NULL);
	#endif
	return attr;
}

void SetMonDataFromVar8003(u8 dataRequest) {
	#ifdef SELECT_FROM_PC
		if (Var8003 == 1)
			SetBoxMonDataAt(Var8000, Var8001, dataRequest, &Var8005);
		else
			SetMonData(&gPlayerParty[Var8004], dataRequest, &Var8005);
	#else
		SetMonData(&gPlayerParty[Var8004], dataRequest, &Var8005);
	#endif
}


struct BoxPokemon* GetBoxedMonAddr(void)
{
	//return ((u32*) &(gSaveBlock3->boxes[Var8000][Var8001]));
	return GetBoxedMonPtr(Var8000, Var8001);
}


u8 sp007_PokemonEVContestStatsChecker(void) {
	u16 mon = Var8004;
	u16 stat = Var8005;

	if (mon >= PARTY_SIZE)
		return 0;

	switch(stat) {

	case CheckEVs_HP:
		return GetMonDataFromVar8003(MON_DATA_HP_EV);
	case CheckEVs_Atk:
		return GetMonDataFromVar8003(MON_DATA_ATK_EV);
	case CheckEVs_Def:
		return GetMonDataFromVar8003(MON_DATA_DEF_EV);
	case CheckEVs_Spd:
		return GetMonDataFromVar8003(MON_DATA_SPEED_EV);
	case CheckEVs_SpAtk:
		return GetMonDataFromVar8003(MON_DATA_SPATK_EV);
	case CheckEVs_SpDef:
		return GetMonDataFromVar8003(MON_DATA_SPDEF_EV);
	case CheckEVs_Coolness:
		return GetMonDataFromVar8003(MON_DATA_COOL);
	case CheckEVs_Beauty:
		return GetMonDataFromVar8003(MON_DATA_BEAUTY);
	case CheckEVs_Cuteness:
		return GetMonDataFromVar8003(MON_DATA_CUTE);
	case CheckEVs_Smartness:
		return GetMonDataFromVar8003(MON_DATA_SMART);
	case CheckEVs_Toughness:
		return GetMonDataFromVar8003(MON_DATA_TOUGH);
	case CheckEVs_Luster:
		return GetMonDataFromVar8003(MON_DATA_SHEEN);
	default:
		return 0;
	}
}

u8 sp008_PokemonIVChecker(void) {
	u16 mon = Var8004;
	u16 stat = Var8005;

	if (mon >= 6)
		return 0;

	switch(stat) {
		case CheckIVs_HP:
			return GetMonDataFromVar8003(MON_DATA_HP_IV);
		case CheckIVs_Atk:
			return GetMonDataFromVar8003(MON_DATA_ATK_IV);
		case CheckIVs_Def:
			return GetMonDataFromVar8003(MON_DATA_DEF_IV);
		case CheckIVs_Spd:
			return GetMonDataFromVar8003(MON_DATA_SPEED_IV);
		case CheckIVs_SpAtk:
			return GetMonDataFromVar8003(MON_DATA_SPATK_IV);
		case CheckIVs_SpDef:
			return GetMonDataFromVar8003(MON_DATA_SPDEF_IV);
		default:
			return 0;
	}
}


bool8 sp009_PokemonRibbonChecker(void) {
	u16 mon = Var8004;
	u16 ribbon = Var8005;

	if (mon >= PARTY_SIZE)
		return FALSE;
	else if (ribbon > 0x1F)
		return FALSE;

	if (GetMonData(&gPlayerParty[mon], MON_DATA_RIBBONS, NULL) & gBitTable[ribbon])
		return TRUE;

	return FALSE;
}


u8 sp00A_CheckPokerusTimer(void) {
	u16 mon = Var8004;
	if (mon >= 6)
		return 0;
	return GetMonDataFromVar8003(MON_DATA_POKERUS);
}


u16 sp00B_CheckPokeball(void) {
	u16 mon = Var8004;

	if (mon >= 6)
		return 0;

	return GetMonDataFromVar8003(MON_DATA_POKEBALL);
}


u8 sp00C_CheckCaptureLocation(void) {
	u16 mon = Var8004;

	if (mon >= 6)
		return 0;

	return GetMonDataFromVar8003(MON_DATA_MET_LOCATION);
}


u8 sp00D_CheckHappiness(void) {
	u16 mon = Var8004;
	if (mon >= 6)
		return 0;
	return GetMonDataFromVar8003(MON_DATA_FRIENDSHIP);
}


item_t sp00E_CheckHeldItem(void) {
	u16 mon = Var8004;

	if (mon >= 6)
		return 0;

	return GetMonDataFromVar8003(MON_DATA_HELD_ITEM);
}


void sp00F_EVAdderSubtracter(void) {
	u16 mon = Var8004;
	u16 stat = Var8005;
	s32 amount = Var8006;

	if (mon >= PARTY_SIZE)
		return;

	s16 curr = sp007_PokemonEVContestStatsChecker();	//Current val

	if (amount & 0x100)
	{
		amount ^= 0x100;
		amount *= -1;
		if (amount < -EV_CAP)
			amount = -255;
	}
	else if (amount > EV_CAP)
		amount = EV_CAP;

	curr += amount;
	if (curr < 0)
		curr = 0;
	else if (curr > EV_CAP)
		curr = EV_CAP;

	Var8005 = curr;

	switch(stat)
	{
		case CheckEVs_HP:
			return SetMonDataFromVar8003(MON_DATA_HP_EV);
		case CheckEVs_Atk:
			return SetMonDataFromVar8003(MON_DATA_ATK_EV);
		case CheckEVs_Def:
			return SetMonDataFromVar8003(MON_DATA_DEF_EV);
		case CheckEVs_Spd:
			return SetMonDataFromVar8003(MON_DATA_SPEED_EV);
		case CheckEVs_SpAtk:
			return SetMonDataFromVar8003(MON_DATA_SPATK_EV);
		case CheckEVs_SpDef:
			return SetMonDataFromVar8003(MON_DATA_SPDEF_EV);
	}
}


void sp010_IVSetter(void) {
	u16 mon = Var8004;
	u16 stat = Var8005;
	u16 amount = Var8006;
	if (mon >= 6)
		return;
	if (amount > 31)
		amount = 31;
	Var8005 = amount;

	switch(stat) {
		case CheckIVs_HP:
			SetMonDataFromVar8003(MON_DATA_HP_IV);
			break;
		case CheckIVs_Atk:
			SetMonDataFromVar8003(MON_DATA_ATK_IV);
			break;
		case CheckIVs_Def:
			SetMonDataFromVar8003(MON_DATA_DEF_IV);
			break;
		case CheckIVs_Spd:
			SetMonDataFromVar8003(MON_DATA_SPEED_IV);
			break;
		case CheckIVs_SpAtk:
			SetMonDataFromVar8003(MON_DATA_SPATK_IV);
			break;
		case CheckIVs_SpDef:
			SetMonDataFromVar8003(MON_DATA_SPDEF_IV);
			break;
		case SetAllIVs:
		{
			u8 i;
			for (i = 0; i <= CheckIVs_SpDef; ++i)
			{
				SetMonDataFromVar8003(MON_DATA_HP_IV + i);
			}
		}
	}
	return;
}

void sp011_RibbonSetterCleaner(void) {
	u16 mon = Var8004;
	u16 ribbon = Var8005;
	u32* ribbonArray = (u32*) &(((u8*) 0x20242D0)[mon * sizeof(struct Pokemon)]);

	if (mon >= PARTY_SIZE)
		return;

	if (ribbon & 0x100)
	{
		ribbon ^= 0x100;
		*ribbonArray &= ~gBitTable[ribbon];
	}
	else if (ribbon > 0x1F)
	{
		return;
	}
	else
	{
		*ribbonArray |= gBitTable[ribbon];
	}
}

void sp012_PokerusSetter(void) {
	u16 mon = Var8004;
	//u16 timer = Var8005;
	//u8 strain;

	if (mon >= 6)
		return;

	if (Var8005 > 16)
		Var8005 = 16;

	SetMonDataFromVar8003(MON_DATA_POKERUS);

}

void sp013_IncreaseDecreaseHappiness(void) {
	u16 mon = Var8004;
	u16 amount = Var8005;

	if (mon >= 6)
		return;
	u8 current = GetMonDataFromVar8003(MON_DATA_FRIENDSHIP);	// current happiness
	if (amount & 0x100)
	{
		//subtraction
		amount ^= 0x100;
		Var8005 = current - amount;
	}
	else
		Var8005 = current + amount;
	// bound friendship
	if (Var8005 < 0)
		Var8005 = 0;
	else if (Var8005 > 255)
		Var8005 = 255;
	Var8006 = 0;
	SetMonDataFromVar8003(MON_DATA_FRIENDSHIP);
}

void sp014_ChangeCapturedBall(void) {
	u16 mon = Var8004;
	u16 ball = Var8005;

	if (mon >= 6)
		return;
	else if (ball >= NUM_BALLS)
		return;
	SetMonDataFromVar8003(MON_DATA_POKEBALL);

	return;
}

void sp015_ModifyHeldItem(void) {
	u16 mon = Var8004;
	//u16 item = Var8005;
	Var800D = 1;

	if (mon >= 6)
		return;

	if (GetMonDataFromVar8003(MON_DATA_HELD_ITEM) != ITEM_NONE)
	{
		if (Var8005 != 0)
			return;	// cant change existing item

		Var8005 = ITEM_NONE;
		SetMonDataFromVar8003(MON_DATA_HELD_ITEM); //Remove items
	}
	else
	{
		SetMonDataFromVar8003(MON_DATA_HELD_ITEM);
		Var800D = 0;
	}
	return;
}

void sp016_ChangePokemonSpecies(void) {
	u16 mon = Var8004;
	//u16 species = Var8005;

	if (mon >= 6)
		return;
	SetMonDataFromVar8003(MON_DATA_SPECIES);
}


// Change Pokemon Attacks
// Inputs:
//		Var8004: pokemon slot
//		Var8005: move slot (0x0-0x3) for move 1-4, respectively
//		Var8006: move to replace (0 to delete)
//
// Unavailable for pc selection
void sp017_ChangePokemonAttacks(void) {
	u16 mon = Var8004;
	u16 moveSlot = Var8005;
	u16 move = Var8006;

	u8 i;
	if (mon >= 6)
		return;
	else if (move > LAST_MOVE_INDEX)
		return;
	else if (moveSlot > MAX_MON_MOVES)
		return;

	// check empty move slots, if not deleting
	if (move != 0)
	{
		for (i = 0; i < MAX_MON_MOVES-1; ++i)
		{
			if (gPlayerParty[mon].moves[i] == 0)
			{
				gPlayerParty[mon].moves[i] = move;
				gPlayerParty[mon].pp[i] = gBattleMoves[move].pp;
				return;
			}
		}
		// otherwise, replace given slot
		gPlayerParty[mon].moves[moveSlot] = move;
		gPlayerParty[mon].pp[moveSlot] = gBattleMoves[move].pp;
		gPlayerParty[mon].pp_bonuses &= ~(gBitTable[moveSlot * 2] | gBitTable[moveSlot * 2 + 1]);	//reset pp bonuses
	}
	else if (move == 0)
		Special_0DD_DeleteMove();
}


species_t sp018_CheckPokemonSpecies(void) {
	u16 mon = Var8004;
	if (mon >= 6)
		return 0;
	else
		return GetMonDataFromVar8003(MON_DATA_SPECIES);
}


//Add pp bonus calc?
u8 sp019_CheckAttackPP(void) {
	u16 mon = Var8004;
	u16 slot = Var8005;
	if (mon >= 6)
		return 0;
	else if (slot >= MAX_MON_MOVES)
		return 0;

	return GetMonDataFromVar8003(MON_DATA_PP1 + slot);
}


// Trading Specials //
////////////////////////////////////////////////////////////////
// Store/Return Party Pokemon Data to Free RAM
// Inputs:
//		Var8002:
//			0 to store from party to free ram
//			1 to store from free ram to party
//			2 to store from free ram to box
//			3 to store from box to free ram
// 		Var8005: slot num (for special 0xFE, activate trade)
// Outputs:
//		Var800D: success (0) or failure (1)
//
void sp01A_CopyPartyData(void)
{
#ifdef SELECT_FROM_PC
	u8 slot = Var8005;
	Var800D = 1;
	if (slot >= 6)
		return;

	switch (Var8002)
	{
		case 0:
			// save party slot data to last enemy slot
			Memcpy(&gEnemyParty[5], &gPlayerParty[slot], 100);
			break;

		case 1:
			// return last enemy slot data to given party slot
			Memcpy(&gPlayerParty[slot], &gEnemyParty[5], 100);
			break;

		case 2:
			// enemy ram to box
			SendMonToBoxPos(&gEnemyParty[5], Var8000, Var8001);
			break;

		case 3:
			// box to free ram
			Memcpy(&gEnemyParty[5], GetBoxedMonAddr(), sizeof(struct CompressedPokemon));
			CalculateMonStats((pokemon_t*) &gEnemyParty[5]);
			break;

		default:
			return;	//failure
	}
	Var800D = 0;
	return;
#else
	return;
#endif
}


// replace a pokemon party slot and boxed mon slot
//Inputs:
//	var8000: box num
//	var8001: box index
//	var8002: withdraw (0) or deposit (1)
//	var8005: party slot num
//Outputs:
//	var800d: success(0) or failure(1)
//
void sp01B_SwapPartyAndBoxData(void)
{
#ifdef SELECT_FROM_PC
	u16 slot = Var8005;
	Var800D = 1;
	if (slot >= 6)
		return;

	switch (Var8002)
	{
		case 0:
			// withdraw from box to party
			Memcpy(&gPlayerParty[slot], GetBoxedMonAddr(), sizeof(struct CompressedPokemon));
			CalculateMonStats((pokemon_t*) &gPlayerParty[slot]);
			ZeroBoxMonAt(Var8000, Var8001);	//clear box pokemon data
			Var800D = 0;
			break;

		case 1:
			Var800D = SendMonToBoxPos(&gPlayerParty[slot], Var8000, Var8001);
			break;

	}
	return;
#else
	return;
#endif
}

//Nicknaming Specials//
///////////////////////////////////////////////////////////////////////////////////
void sp07C_BufferNickname(void)
{
#ifdef SELECT_FROM_PC
	if (Var8003 == 1)
		GetAndCopyBoxMonDataAt(Var8000, Var8001, MON_DATA_NICKNAME, gStringVar1);
	else
		GetMonData(&gPlayerParty[Var8004], MON_DATA_NICKNAME, gStringVar1);
#else
	GetMonData(&gPlayerParty[Var8004], MON_DATA_NICKNAME, gStringVar1);
#endif
	StringGetEnd10(gStringVar1);
}


bool8 sp07D_CheckTradedPokemon(void)
{
	u16 monOTID;
	u16 TID = GetCombinedOTID();
#ifdef SELECT_FROM_PC
	if (Var8003 == 1)
		monOTID = GetBoxMonDataAt(Var8000, Var8001, MON_DATA_OT_ID);
	else
		monOTID = GetMonData(&gPlayerParty[Var8004], MON_DATA_OT_ID, 0);
#else
	monOTID = GetMonData(&gPlayerParty[Var8004], MON_DATA_OT_ID, 0);

#endif
	if (monOTID == TID)
		return TRUE;
	else
		return FALSE;
}


void NicknameFunc(void)
{
#ifdef SELECT_FROM_PC
	if (Var8003 == 1)
		SetBoxMonDataAt(Var8000, Var8001, MON_DATA_NICKNAME, gStringVar2);
	else
		SetMonData(&gPlayerParty[Var8004], MON_DATA_NICKNAME, gStringVar2);
#else
	SetMonData(&gPlayerParty[Var8004], MON_DATA_NICKNAME, gStringVar2);
#endif
	CB2_ReturnToFieldContinueScriptPlayMapMusic();
}


void sp09E_NicknamePokemon(void) {
	void* src;
#ifdef SELECT_FROM_PC
	if (Var8003 == 1)
		src = GetBoxedMonAddr();
	else
		src = &gPlayerParty[Var8004];
#else
	src = &gPlayerParty[Var8004];
#endif
	GetMonData(src, MON_DATA_NICKNAME, gStringVar3);
	GetMonData(src, MON_DATA_NICKNAME, gStringVar2);
	u16 species = GetMonData(src, MON_DATA_SPECIES, 0);
	u8 gender = GetMonGender(src);
	u16 PID = GetMonData(src, MON_DATA_PERSONALITY, 0);
	DoNamingScreen(3, gStringVar2, species, gender, PID, (void*) NicknameFunc);
	return;
}


//Party Specials//
///////////////////////////////////////////////////////////////////////////////////

//Erase pokemon from party, or entire party
void sp062_PokemonEraser(void)
{
	u8 slot = Var8004;
	if (slot == 0xF)
		ZeroPlayerPartyMons();
	else
	{
		ZeroMonData(&gPlayerParty[slot]);
		CompactPartySlots();
		CalculatePlayerPartyCount();
	}
}


//Check status of pokemon in slot var8004
u8 sp063_StatusChecker(void)
{
	return gPlayerParty[Var8004].condition;
}


static void TryAssignStatusToMon(pokemon_t* mon, u32 status)
{
	if (status & STATUS1_SLEEP && CanPartyMonBePutToSleep(mon))
		mon->condition = STATUS1_SLEEP;
	else if (status & STATUS1_POISON && CanPartyMonBePoisoned(mon))
		mon->condition = STATUS1_POISON;
	else if (status & STATUS1_BURN && CanPartyMonBeBurned(mon))
		mon->condition = STATUS1_BURN;
	else if (status & STATUS1_FREEZE && CanPartyMonBeFrozen(mon))
		mon->condition = STATUS1_FREEZE;
	else if (status & STATUS1_PARALYSIS && CanPartyMonBeParalyzed(mon))
		mon->condition = STATUS1_PARALYSIS;
	else if (status & STATUS1_TOXIC_POISON && CanPartyMonBePoisoned(mon))
		mon->condition = STATUS1_TOXIC_POISON;
}

// Inflict a status to affect a party member or entire party
void sp064_InflictStatus(void)
{
	u8 slot = Var8004;
	u8 status = Var8005;
	u8 i;

	CalculatePlayerPartyCount();

	if (!Var8006)
	{
		if (slot == 0xF)
		{
			for (i = 0; i < gPlayerPartyCount; ++i)
				gPlayerParty[i].condition = status;
		}
		else
			gPlayerParty[slot].condition = status;
	}
	else //Only assign status if the mon can actually get that status condition.
	{
		if (slot == 0xF)
		{
			for (i = 0; i < gPlayerPartyCount; ++i)
				TryAssignStatusToMon(&gPlayerParty[i], status);
		}
		else
			TryAssignStatusToMon(&gPlayerParty[slot], status);
	}
}


// check slot pokemon's HP
u16 sp065_CheckMonHP(void)
{
	return gPlayerParty[Var8004].hp;
}

static void InflictPartyDamageOrHeal(pokemon_t* mon, u16 damage, u8 type)
{
	s32 currHP = mon->hp;

	if (type == 1) //Heal
		currHP = currHP +  damage;
	else
		currHP = currHP -  damage;

	if (currHP < 0)
		currHP = 0;
	else if (currHP > mon->maxHP)
		currHP = mon->maxHP;

	mon->hp = currHP;
}

// inflict damage on a party pokemon, or entire party
void sp066_InflictPartyDamage(void) {
	u8 slot = Var8004;
	u16 dmg = Var8005;
	u8 switcher = Var8006;	//1 to heal, else damage

	CalculatePlayerPartyCount();

	if (slot == 0xf)
	{
		for (int i = 0; i < gPlayerPartyCount; ++i)
			InflictPartyDamageOrHeal(&gPlayerParty[i], dmg, switcher);
	}
	else
		InflictPartyDamageOrHeal(&gPlayerParty[slot], dmg, switcher);
}

//void sp067_GenerateRandomBattleTowerTeam(void) - In "src/build_pokemon.c"

//Key Specials//
///////////////////////////////////////////////////////////////////////////////////

//Special 0x2b checks for the A and B keys. Returns
//0x0 if none pressed
//0x1 if A pressed
//0x2 if B pressed
//0x3 if both are pressed
u16 sp02B_CheckABButtons(void)
{
	return (~(gKeyReg) & 3);
}

//Special 0x2c checks for the D-pad. Returns
//0x0 if no direction is pressed
//0x1 if up is pressed
//0x2 if left is pressed
//0x3 if down is pressed
//0x4 if right is pressed
//0x5 if up-left is pressed
//0x6 if up-right is pressed
//0x7 if down-left is pressed
//0x8 if down-right is pressed
u16 sp02C_CheckDPad(void)
{
	switch(~(gKeyReg) & 0xFF) {
		case DPAD_UP:
			return 1;
		case DPAD_LEFT:
			return 2;
		case DPAD_DOWN:
			return 3;
		case DPAD_RIGHT:
			return 4;
		case DPAD_UP | DPAD_LEFT:
			return 5;
		case DPAD_UP | DPAD_RIGHT:
			return 6;
		case DPAD_DOWN | DPAD_LEFT:
			return 7;
		case DPAD_DOWN | DPAD_RIGHT:
			return 8;
	}
	return 0;
}

//Special 0x2d checks for the start select buttons
//0x0 if none is pressed
//0x1 if select is pressed
//0x2 if start is pressed
//0x3 if both are pressed
u16 sp02D_CheckStartSelect(void)
{
	return ((~(gKeyReg) >> 2) & 3);
}

//Special 0x2e checks for the L-R buttons
//0x0 if none is pressed
//0x1 if R is pressed
//0x2 if L is pressed
//0x3 if both are pressed
u16 sp02E_CheckLRButtons(void)
{
	return (~(gKeyReg) >> 8) & 3;
}

void sp02F_KeyDump(void)
{
	Var800D = ~(gKeyReg) & 0x3FF;
}

// Inputs:
//		var8004: key(s) to force
//		var8005: num times to 'press'
void sp0C9_ForceOneKeyInput(void)
{
#ifdef SAVE_BLOCK_EXPANSION
	gKeypadSetter->keyMapToForce = Var8004;
	gKeypadSetter->keyForcingCounter = Var8005;
	gKeypadSetter->keyFlags |= 1;
#endif
}

void sp0CA_IgnoreKeys(void)
{
#ifdef SAVE_BLOCK_EXPANSION
	gKeypadSetter->keysToIgnore = Var8004;
	gKeypadSetter->keyFlags |= 2;
#endif
}

void sp0CB_PlaceKeyScript(void)
{
#ifdef SAVE_BLOCK_EXPANSION
	u16 key = Var8004;
	gKeypadSetter->keyToRunScript = key;
	if (key == 0)
	{
		gKeypadSetter->scriptToRun = 0;
		gKeypadSetter->keyFlags &= 0xFB;
	}
	else
	{
		gKeypadSetter->scriptToRun = gLoadPointer;
		gKeypadSetter->keyFlags |= 4;
	}
#endif
}

//Variable Math Specials//
///////////////////////////////////////////////////////////////////////////////////

u16 sp03E_AddVariables(void)
{
	u16 var1 = Var8004; //Var contained in Var8004
	u16 var2 = Var8005; //Var contained in Var8005
	bool8 overflow = FALSE;

	var1 = VarGet(var1);
	var2 = VarGet(var2);

	u32 sum = var1 + var2;

	if (sum > 0xFFFF)
	{
		overflow = TRUE;
		sum = 0xFFFF;
	}

	VarSet(Var8004, sum); //Set var in Var8004
	return overflow;
}

u16 sp03F_SubtractVariables(void)
{
	u16 var1 = Var8004; //Var contained in Var8004
	u16 var2 = Var8005; //Var contained in Var8005
	bool8 underflow = FALSE;
	u32 diff;

	var1 = VarGet(var1);
	var2 = VarGet(var2);

	if (var2 > var1)
	{
		underflow = TRUE;
		diff = 0;
	}
	else
		diff = var1 - var2;

	VarSet(Var8004, diff); //Set var in Var8004
	return underflow;
}

u16 sp040_MultiplyVariables(void)
{
	u16 var1 = Var8004; //Var contained in Var8004
	u16 var2 = Var8005; //Var contained in Var8005
	bool8 overflow = FALSE;
	u32 prod;

	var1 = VarGet(var1);
	var2 = VarGet(var2);

	prod = var1 * var2;

	if (prod > 0xFFFF)
	{
		prod = 0xFFFF;
		overflow = TRUE;
	}

	VarSet(Var8004, prod); //Set var in Var8004
	return overflow;
}


u16 sp041_DivideVariables(void)
{
	u16 numerator = Var8004;
	u16 denominator = Var8005;

	Var8004 = numerator / denominator;
	return numerator % denominator; //Return remainder
}


u16 sp042_ANDVariables(void)
{
	return Var8004 & Var8005;
}

u16 sp043_ORVariables(void)
{
	return Var8004 | Var8005;
}

u16 sp044_XORVariables(void)
{
	return Var8004 ^ Var8005;
}

//Other Specials//
///////////////////////////////////////////////////////////////////////////////////
// add a string to custom multichoice box by vars 0x8004 and 0x8005
// eg:
//		setvar 0x8004 0x0890
//		setvar 0x8005 0x5040
//		special 0x24
// would load a multichoice pointer to 0x8905040
// personally, special 0x25 is much better/easier to use
void sp024_AddTextByVariable(void)
{
#ifdef SAVE_BLOCK_EXPANSION
	u8 multiIndex = Var8006;
	u8* stringPointer = (u8*) ((Var8004 << 16) | Var8005);
	if (multiIndex <= 6)
		gMultiChoice[multiIndex].name = stringPointer;
#endif
}


// add a string to custom multichoice box by loadpointer
// eg.
//		setvar 0x8006 0x0
//		loadpointer 0x0 @string
//		special 0x25
void sp025_AddTextByPointer(void)
{
#ifdef SAVE_BLOCK_EXPANSION
	u8 multiIndex = Var8006;
	if (multiIndex <= 6)
		gMultiChoice[multiIndex].name = gLoadPointer;
#endif
}


// special to buffer a pokemon species and size
// Inputs: 	Var8005: variable to get measurements from/store to
// 			Var8006: species to measure
void sp075_MeasurePokemon1(void)
{
	u16 species = Var8006;
	SizeMinigame_BufferPokeNameSize(species, GetVarPointer(Var8005));
}


// measure pokemon special
// Inputs:
// 		Var8005: Variable the measurement was in (from special 0x75)
// 		Var8006: Species
// Output:
//		Returns 1 if the pokémon is not of the selected type
//		Returns 2 if the pokémon is smaller
//		Returns 3 if bigger, and also stores the biggest value in the variable
//		Returns 4 if equal in length
u16 sp076_MeasurePokemon2(void)
{
	u16 species = Var8006;
	return SizeMinigame_CalculateMonHeight(species, GetVarPointer(Var8005));
}


// old man battle with given species/level
// Inputs:
//		var8004: species
//		var8005: level
void sp09C_OldManBattleModifier(void)
{
	CreateMaleMon(&gEnemyParty[0], Var8004, Var8005);
	ScriptContext2_Enable();
	gMain.savedCallback = CB2_ReturnToFieldContinueScriptPlayMapMusic;
	gBattleTypeFlags = BATTLE_TYPE_OLD_MAN;
	CreateBattleStartTask(8, 0);
}

/*
// in Assembly/script.s
void sp18B_DisplayImagesFromTable(void) {
	return;
}
*/

//Battle Specials//
///////////////////////////////////////////////////////////////////////////////////

bool8 CanMonParticipateInASkyBattle(struct Pokemon* mon)
{
	u16 species = mon->species;

	if (GetMonData(mon, MON_DATA_IS_EGG, NULL)
	||  mon->hp == 0
	||  CheckTableForSpecies(species, gSkyBattleBannedSpeciesList))
		return FALSE;

	if (gBaseStats[species].type1 == TYPE_FLYING
	||  gBaseStats[species].type2 == TYPE_FLYING
	||  GetMonAbility(mon) == ABILITY_LEVITATE)
		return TRUE;

	return FALSE;
}

bool8 sp051_CanTeamParticipateInSkyBattle(void)
{
	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		if (CanMonParticipateInASkyBattle(&gPlayerParty[i]))
			return gSpecialVar_LastResult = TRUE;
	}

	return gSpecialVar_LastResult = FALSE;
}


//u16 sp052_GenerateFacilityTrainer(void); //Code in "src/frontier.c"
//void sp053_LoadFrontierIntroBattleMessage(void); //Code in "src/frontier.c"
//void sp054_GetBattleFacilityStreak(void) //Code in "src/frontier.c"
//void sp055_UpdateBattleFacilityStreak(void) //Code in "src/frontier.c"
//void sp056_DetermineBattlePointsToGive(void) //Code in "src/frontier.c"

//@Details: Buffers the map name where there is currently a swarm to buffer1,
//			and the species name where there is currently a swarm to buffer2.
//@Returns: Species roaming.
u16 sp058_BufferSwarmText(void)
{
	u8 index = VarGet(VAR_SWARM_INDEX);
	u8 mapName = gSwarmTable[index].mapName;
	u16 species = gSwarmTable[index].species;

	GetMapName(sScriptStringVars[0], mapName, 0);
	StringCopy(sScriptStringVars[1], gSpeciesNames[species]);

	return species;
}

//@Details: Buffers the map name where the given roamer can be found to buffer1,
//			and the species name of the roamer to buffer2.
//@Inputs:
//		Var8000: Species
//@Returns: 0 to given var if species is not roaming. 1 if it is and the name was buffered.
bool8 sp059_BufferSpeciesRoamingText(void)
{
	u8 mapGroup;
	u8 mapNum;
	u16 species = Var8000;

	if (!IsSpeciesRoaming(species))
		return FALSE;

	GetMapGroupAndMapNumOfRoamer(species, &mapGroup, &mapNum);
	GetMapName(sScriptStringVars[0], Overworld_GetMapHeaderByGroupAndId(mapGroup, mapNum)->regionMapSectionId, 0);
	StringCopy(sScriptStringVars[1], gSpeciesNames[species]);
	return TRUE;
}

void sp05A_WildDataSwitch(void)
{
	gWildDataSwitch = gLoadPointer;
}

void sp05B_WildDataSwitchCanceller(void)
{
	gWildDataSwitch = NULL;
}

void sp0AC_LoadTrainerBDefeatText(void)
{
	sTrainerDefeatSpeech_B = (u8*) gLoadPointer;
}


//Timer Specials//
///////////////////////////////////////////////////////////////////////////////////

struct GbaTimers
{
	/*0x0*/u16 init;	// 4000108			TM2CNT_L -> Timer 2
	/*0x2*/u16 timerFlags;	// 400010A			TM2CNT_H - Timer 2 Control (R/W)
	/*0x4*/u16 timerVal;	// 400010C		TM3CNT_L -> Timer 3
	/*0x6*/u16 timerOn;	// 400010E				TM3CNT_H - Timer 3 Control
};
#define gGbaTimer ((struct GbaTimers*) 0x4000108)

/* timer control
  Bit   Expl.
  0-1   Prescaler Selection (0=F/1, 1=F/64, 2=F/256, 3=F/1024)							0x1,0x2
  2	 Count-up Timing   (0=Normal, 1=See below)  ;Not used in TM0CNT_H				0x4
  3-5   Not used																		0x8,0x10,0x20
  6	 Timer IRQ Enable  (0=Disable, 1=IRQ on Timer overflow)							0x40
  7	 Timer Start/Stop  (0=Stop, 1=Operate)											0x80
  8-15  Not used
When Count-up Timing is enabled, the prescaler value is ignored, instead the time is incremented each time when the previous counter overflows. This function cannot be used for Timer 0 (as it is the first timer).
 */

//@Details: Starts the timer
void sp046_StartTimer(void)
{
	gGbaTimer->init = 0xC000;
	gGbaTimer->timerFlags = 0x83;
	gGbaTimer->timerVal = 0;
	gGbaTimer->timerOn = 0x84;
}

//@Details: Pauses the timer, saves val to gTimerValue
void sp047_HaltTimer(void)
{
	#ifdef SAVE_BLOCK_EXPANSION
		gTimerValue = gGbaTimer->timerVal;
	#endif
	gGbaTimer->timerOn ^= 0x80;
	gGbaTimer->timerFlags ^= 0x80;
}

//@Details: Unpauses the timer
//Without save block expansion, the timer will reset
void sp048_ResumeTimer(void)
{
	#ifdef SAVE_BLOCK_EXPANSION
		gGbaTimer->timerVal = gTimerValue;
	#endif
	gGbaTimer->timerOn |= 0x80;
	gGbaTimer->timerFlags |= 0x80;
}

//@Details:	Stops the timer.
//@Returns: The time on the timer.
u16 sp049_StopTimer(void)
{
	gGbaTimer->timerOn ^= 0x80;
	gGbaTimer->timerFlags ^= 0x80;
	return gGbaTimer->timerVal;

}

//@Returns: The time on the timer.
u16 sp04A_GetTimerValue(void)
{
	return gGbaTimer->timerVal;
}



void sp04C_UpdatePlaytime(void)
{
	u8 secs = gGbaTimer->timerVal + gSaveBlock2->playTimeSeconds;

	// subtract from hrs/mins until mins/secs < 60, respectively
	while (secs > 60)
	{
		secs -= 60;
		gSaveBlock2->playTimeMinutes++;
		if (gSaveBlock2->playTimeMinutes == 60)
		{
			gSaveBlock2->playTimeMinutes -= 60;
			gSaveBlock2->playTimeHours++;
		}
	}
}

void sp04B_StopAndUpdatePlaytime(void)
{
	gGbaTimer->init = 0;
	gGbaTimer->timerFlags = 0;
	gGbaTimer->timerOn = 0;
	sp04C_UpdatePlaytime();
}



//@Details: Checks if the timer has reached a value
//			sorted in var 0x8010.
//@Returns: True or False
bool8 sp04D_TimerValueReached(void)
{
	u16 timerVal = gGbaTimer->timerVal;
	if (timerVal < Var8010)
		return FALSE;
	return TRUE;
}


//@Details: Saves the value in the seconds timer to a
//			specific memory address.
void sp04E_SaveTimerValue(void)
{
#ifdef SAVE_BLOCK_EXPANSION
	gTimerValue = sp049_StopTimer();
#endif
}


//@Details: Starts the timer with the value stored by
//			Special 0x4E
//		or manually write start time at gTimerValue
// redundant after fixes.
void sp04F_StartTimerAtTime(void)
{
#ifdef SAVE_BLOCK_EXPANSION
	sp048_ResumeTimer();
#endif
}


//@Details: Stores the timer value stored by
//			Special 0x4E.
//@Returns: Var inside var 0x8006 - Timer time.
void sp050_StoreTimerToVariable(void)
{
#ifdef SAVE_BLOCK_EXPANSION
	VarSet(Var8006, gTimerValue);
#endif
}


//@Details: Loads the value at a given variable and stores
//			it to the saved timer.
//@Input:	Var 0x8006 - Variable that is holding timer.
void sp061_LoadTimerFromVariable(void)
{
#ifdef SAVE_BLOCK_EXPANSION
	gTimerValue = VarGet(Var8006);
#endif
}

//Safari Specials//
///////////////////////////////////////////////////////////////////////////////////


#ifdef EXPAND_SAFARI_BALLS
	#define gSafariBallNumber (*((u16*) 0x02039994))
#else
	#define gSafariBallNumber (*((u8*) 0x02039994))
#endif
#if MAX_SAFARI_BALLS < SAFARI_ZONE_BALL_START
	#define MAX_SAFARI_BALLS SAFARI_ZONE_BALL_START
#endif
//@Details: The check Safari ball quantity.
//@Returns: 1. Var 0x8004 - Normal Safari Ball number.
//		  2. Var 0x8005 - The extra ball slot number
//		  3. To a given variable the number as a full integer. Max value is 0x63FF
u16 sp086_GetSafariBalls(void)
{
	/*
	Var8004 = gSafariBallNumber;
	Var8005 = *(&gSafariBallNumber + 1);
	return Var8004 + Var8005;
	*/
	return gSafariBallNumber;
}


//@Details: An increase\decrease safari balls code.
//		  This code doesn't allow people to add over 100 balls as a safety measure.
//@Input: Var 0x8004 as the number to increase or decrease the balls by.
//		0x1aa decreases the balls by aa, and 0x0aa increases them by aa.
//@Returns: None
void sp087_ChangeSafariBalls(void)
{
	s32 input = Var8004;
	s32 calc;

	if (input & 0x100)
	{
		input ^= 0x100;
		input = MathMin(input, 100);	//limit to 100 balls at once
		input *= -1;
	}
	else
		MathMin(input, 100);	//limit to 100 balls at once

	calc = gSafariBallNumber + input;

	if (calc > MAX_SAFARI_BALLS)
		calc = MAX_SAFARI_BALLS;
	else if (calc < 0)
		calc = 0;
	gSafariBallNumber = calc;

	/*
	if (calc > 100)
		calc = 100;
	else if (calc < 0)
		calc = 0;
	gSafariBallNumber = calc;
	*/
}

//@Details: The get safari pedometer special.
//@Returns: To a given variable the number of remaining steps
u16 sp088_GetSafariCounter(void)
{
	return gSafariSteps;
}

//@Details: The Set Safari steps special.
//		  It allows you to set a specific ammount of steps
//		  until the safari handler is called.
//Input: Var 0x8004 is the amount of steps to place.
void sp089_SetSafariCounter(void)
{
	u16 input = Var8004;
	if (input > SAFARI_ZONE_MAX_STEPS)
		input = SAFARI_ZONE_MAX_STEPS;

	gSafariSteps = input;
}


void SetSafariZone(void)
{
	gSafariSteps = SAFARI_ZONE_MAX_STEPS;
	gSafariBallNumber = SAFARI_ZONE_BALL_START;
}

#if MAX_SAFARI_BALLS > 9999
	#define SAFARI_BALL_DIGITS 5	//max of 0xFFFF or 65535
#elif MAX_SAFARI_BALLS > 999
	#define SAFARI_BALL_DIGITS 4
#elif MAX_SAFARI_BALLS > 99
	#define SAFARI_BALL_DIGITS 3
#elif MAX_SAFARI_BALLS > 9
	#define SAFARI_BALL_DIGITS 2
#else
	#define SAFARI_BALL_DIGITS 1
#endif
#if SAFARI_ZONE_MAX_STEPS > 9999	//max of 0xFFFF
	#define SAFARI_STEP_DIGITS 5
#elif SAFARI_ZONE_MAX_STEPS > 999
	#define SAFARI_STEP_DIGITS 4
#elif SAFARI_ZONE_MAX_STEPS > 99
	#define SAFARI_STEP_DIGITS 3
#elif SAFARI_ZONE_MAX_STEPS > 9
	#define SAFARI_STEP_DIGITS 2
#else
	#define SAFARI_STEP_DIGITS 1
#endif
void DisplaySafariZoneCounters(void)
{
	ConvertIntToDecimalStringN(gStringVar1, gSafariSteps, 1, SAFARI_STEP_DIGITS);
	ConvertIntToDecimalStringN(gStringVar2, SAFARI_ZONE_MAX_STEPS, 1, SAFARI_STEP_DIGITS);
	ConvertIntToDecimalStringN(gStringVar3, gSafariBallNumber, 1, SAFARI_BALL_DIGITS);
}

u8* SafariZoneBallLabel(void)
{
	#if (SAFARI_BALL_DIGITS < 3)
		return (u8*) 0x83FE81C;
	#else
		return (u8*) 0x83FE825;
	#endif
}

void SafariZoneBattleBarCount(u8* strPtr)
{

	ConvertIntToDecimalStringN(strPtr, gSafariBallNumber, 0, SAFARI_BALL_DIGITS);
}

//Walking Specials//
///////////////////////////////////////////////////////////////////////////////////


//@Details: Tile Number getter, gets the tile's number at (0x8004, 0x8005) location.
//@Input: 1. Var 0x8004 - X-Coordinate
//		2. Var 0x8005 - Y-Coordinate
//@Returns: Returns to a given variable the desired tile number
u16 sp07E_GetTileNumber(void) {
	u16 x = Var8004;
	u16 y = Var8005;
	return MapGridGetMetatileIdAt(x + 7, y + 7);
}



//@Details: The Tile Attribute getter. Designed to be used with
//		  special 0x8f before it (to get the positions) and to
//		  be possible to use the required wild battle randomizer afterwards.'
//@Input: 1. Var 0x8004 - X-Coordinate
//		2. Var 0x8005 - Y-Coordinate
//@Returns: 1. Var 0x8004 - The tile's background byte
//		  2. Var 0x8005 - The tile's behaviour bytes.
//		  3. To a given variable the background byte.
u16 sp07F_GetTileBehaviour(void)
{
	u16 x = Var8004;
	u16 y = Var8005;
	u32 field = MapGridGetMetatileField(x + 7, y + 7, 0xFF);
	Var8004 = (field & 0xFFFF0000) >> 24;
	Var8005 = (field & 0xFFFF);
	return Var8004 & 3;
}

void sp097_StartGroundBattle(void)
{
	StartRandomWildEncounter(FALSE);
}

void sp098_StartWaterBattle(void)
{
	StartRandomWildEncounter(TRUE);
}

// WALKING SCRIPTS
void sp081_SetWalkingScript(void) {
	gWalkingScript = gLoadPointer;
	return;
}

// read a pedometer value from custom pedometers (see ram_locs.h)
// flag-based
// Inputs:
//	Var8004:
//		0: return Pedometers.alwaysActive (32bit)
//		1: Pedometers.large (32bit)
//		2: Pedometers.medium (16bit)
//		3: Pedometers.small_1 (8bit)
//		4: Pedometers.small_2 (8bit)
// Outputs: given pedometer value (be sure to free up 2 variables for the 32bit vals)
u32 sp08A_ReadPedometerValue(void) {
	#ifdef SAVE_BLOCK_EXPANSION
		u8 input = Var8004;
		switch (input)
		{
			case 0:
				return gPedometers->alwaysActive;
			case 1:
				return gPedometers->large;
			case 2:
				return gPedometers->medium;
			case 3:
				return gPedometers->smallOne;
			case 4:
				return gPedometers->smallTwo;
			default:
				return 0;
		}
	#else
		return 0;
	#endif
}

//Other New Specials//
///////////////////////////////////////////////////////////////////////////////////

bool8 CheckAndSetDailyEvent(u16 eventVar, bool8 setDailyEventVar);

struct DailyEventVar
{
	u32 minute : 6;	//60 min in hour
	u32 hour : 5;	//24 hrs in day
	u32 day : 5;	//31 days max in month
	u32 month : 4;	//12 months in year
	u32 year : 7; //99 years in century
	u32 century : 5; //31 centuries max - starts at 0
};

bool8 IsTimeInVarInFuture(u16 var)
{
	struct DailyEventVar* timeData = (struct DailyEventVar*) GetVarPointer(var);

	u8 hour = timeData->hour;
	u8 minute = timeData->minute;
	u8 day = timeData->day;
	u8 month = timeData->month;
	u32 year = timeData->year + timeData->century * 100;

	return year > gClock.year
	|| (year == gClock.year && month > gClock.month)
	|| (year == gClock.year && month == gClock.month && day > gClock.day)
	|| (year == gClock.year && month == gClock.month && day == gClock.day && hour > gClock.hour)
	|| (year == gClock.year && month == gClock.month && day == gClock.day && hour == gClock.hour && minute > gClock.minute);
}

//@Details: Runs a daily event.
//@Input: Var 0x8000: A var containing the daily event data.
//					  The var after this one is used as well.
//		  Var 0x8001: 0x0 - Don't set daily event var.
//@Returns: False if the event has already been done. True otherwise.
bool8 sp0A0_CheckAndSetDailyEvent(void)
{
	return CheckAndSetDailyEvent(Var8000, Var8001); //Var contained in Var8000
}

bool8 CheckAndSetDailyEvent(u16 eventVar, bool8 setDailyEventVar)
{
	bool8 toReturn = FALSE;
	struct DailyEventVar* dailyData = (struct DailyEventVar*) GetVarPointer(eventVar);

	u8 dailyDay = dailyData->day;
	u8 dailyMonth = dailyData->month;
	u32 dailyYear = dailyData->year + dailyData->century * 100;

	if (dailyYear > gClock.year
	|| (dailyYear == gClock.year && dailyMonth > gClock.month)
	|| (dailyYear == gClock.year && dailyMonth == gClock.month && dailyDay > gClock.day))
		return FALSE; //Player changed date on their computer.

	if (dailyDay != gClock.day
	||  dailyMonth != gClock.month
	||  dailyYear != gClock.year)
	{
		if (setDailyEventVar)
		{
			dailyData->minute = gClock.minute;
			dailyData->hour = gClock.hour;
			dailyData->day = gClock.day;
			dailyData->month = gClock.month;
			dailyData->year = gClock.year % 100;
			dailyData->century = gClock.year / 100;
		}
		toReturn = TRUE;
	}

	return toReturn;
}

u32 GetDaysSinceTimeInValue(u32 value)
{
	struct DailyEventVar* startTime = (struct DailyEventVar*) &value;

	u16 startYear = startTime->year + startTime->century * 100;
	if (startYear < 1900)
		startYear = 1900;

	return GetDayDifference(startYear, startTime->month, startTime->day, gClock.year, gClock.month, gClock.day);
}

void UpdateTimeInVars(u16 eventVar)
{
	struct DailyEventVar* data = (struct DailyEventVar*) GetVarPointer(eventVar);

	data->minute = gClock.minute;
	data->hour = gClock.hour;
	data->day = gClock.day;
	data->month = gClock.month;
	data->year = gClock.year % 100;
	data->century = gClock.year / 100;
}

//@Details: Updates the time stored in a pair of vars.
//@Input: Var 0x8000: A var containing the daily event data.
//					  The var after this one is used as well.
void sp0A1_UpdateTimeInVars(void)
{
	UpdateTimeInVars(Var8000); //Var contained in Var8000
}

//@Details: Gets the time difference between the data stored in a var and the current time.
//@Input: Var 0x8000: A var containing the time data.
//					  The var after this one is used as well.
//@Input: Var 0x8001: 0 - Get minute difference
//					  1 - Get hour difference.
//					  2 - Get day difference.
//					  3 - Get month difference.
//					  4 - Get year difference.
//@Returns: The time difference between the current time and the time stored in the given var.
u32 sp0A2_GetTimeDifference(void)
{
	u32 difference = 0;
	u16 eventVar = Var8000; //Var contained in Var8000
	struct DailyEventVar* startTime = (struct DailyEventVar*) GetVarPointer(eventVar);

	u8 startMinute = startTime->minute;
	u8 startHour = startTime->hour;
	u8 startDay = startTime->day;
	u8 startMonth = startTime->month;
	u32 startYear = startTime->year + startTime->century * 100;

	switch (Var8001) {
		case 0: //Get minute difference
			difference = GetMinuteDifference(startYear, startMonth, startDay, startHour, startMinute, gClock.year, gClock.month, gClock.day, gClock.hour, gClock.minute);
			break;
		case 1: //Get hour difference
			difference = GetMinuteDifference(startYear, startMonth, startDay, startHour, startMinute, gClock.year, gClock.month, gClock.day, gClock.hour, gClock.minute) / 60;
			break;
		case 2: //Get day difference.
			difference = GetMinuteDifference(startYear, startMonth, startDay, startHour, startMinute, gClock.year, gClock.month, gClock.day, gClock.hour, gClock.minute) / 60 / 24;
			break;
		case 3: //Get month difference.
			difference = GetMonthDifference(startYear, startMonth, gClock.year, gClock.month);
			break;
		case 4: //Get year difference.
			difference = GetYearDifference(startYear, gClock.year);
			break;
	}

	return difference;
}

enum
{
	MORNING,
	DAY,
	EVENING,
	NIGHT,
};

//@Details: Gets the time of day.
//@Input: Var 0x8000: 1 = Merge morning, day, and evening.
//@Returns: 0 = Morning, 1 = Day, 2 = Evening, 3 = Night
u8 sp0AD_GetTimeOfDay(void)
{
	switch (Var8000) {
		case 1:
			if (IsDayTime()) //Combined with morning and evening
				return DAY;
			else
				return NIGHT;

		default:
			if (IsNightTime())
				return NIGHT;
			else if (IsEvening())
				return EVENING;
			else if (IsOnlyDayTime())
				return DAY;
			else
				return MORNING;

	}
}

//@Details: Gets the current hour in the day.
//@Returns: The current hour of the day.
u8 sp0D9_GetHour(void)
{
	return gClock.hour;
}

//@Details: Gets the current day of week.
//@Returns: The current day of week.
u8 sp0DA_GetDayOfWeek(void)
{
	return gClock.dayOfWeek;
}

//@Details: Gets the time of day.
//@Input: Var 0x8000: Flag to clear.
void sp0AE_ClearFlag(void)
{
	FlagClear(Var8000); //Flag contained in Var 0x8000
}

//@Details: Dismounts the player from the bicycle if they're on it.
void sp0AF_DismountBicyle(void)
{
	if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_BIKE))
		StartTransitionToFlipBikeState(PLAYER_AVATAR_FLAG_ON_FOOT);
}

//@Details: Stops any sound effects from playing.
void sp09A_StopSounds(void)
{
	m4aMPlayStop(&gMPlayInfo_SE1);
	m4aMPlayStop(&gMPlayInfo_SE2);
}

//@Details: Gets the types of the requested party Pokemon.
//@Input: Var 0x8000: Party Pokemon number
//@Returns: Var 0x8000: Type 1
//			Var 0x8001: Type 2
void sp0B0_LoadPartyPokemonTypes(void)
{
	u8 mon = Var8000;
	if (mon < PARTY_SIZE)
	{
		Var8000 = gBaseStats[gPlayerParty[mon].species].type1;
		Var8001 = gBaseStats[gPlayerParty[mon].species].type2;
	}
}

//@Details: Opens the bag and lets the player select an item.
//@Input: 	Var 0x8000: 0 = Any Pocket, Open From Item's Pocket
//						1 = Any Pocket, Open From Key Item's Pocket
//						2 = Any Pocket, Open From Poke Ball Pocket
//						3 = Any Pocket, Open From Item's Pocket
//						4 = Berry Pouch
//						5 = TM Case
//@Returns: Var 0x800E: The item the player chose. 0 if they chose nothing.
void sp0B1_ChooseItemFromBag(void)
{
	PlayRainStoppingSoundEffect();

	switch (Var8000) {
		case 0:
		case 1:
		case 2:
		case 3:
			GoToBagMenu(BAG_OPEN_SELECT_ITEM, Var8000, CB2_ReturnToFieldContinueScript);
			break;
		case 4:
			GoToBerryMenu(BERRY_POUCH_CHOOSE_ITEM, CB2_ReturnToFieldContinueScript, 0);
			break;
		case 5:
			GoToTMMenu(TM_CASE_SELECT_ITEM, CB2_ReturnToFieldContinueScript, 0);
			break;
		default:
			GoToBagMenu(BAG_OPEN_SELECT_ITEM, 0, CB2_ReturnToFieldContinueScript);
	}
}

//@Details: Checks if any Pokemon in the player's party is of the given type.
//@Input: 	Var 0x8000: Pokemon type.
//@Returns: LastResult: Party index of mon with type. PARTY_SIZE if not found.
void sp0B2_PokemonTypeInParty(void)
{
	u8 type = Var8000;

	for (int i = 0; i < PARTY_SIZE; ++i)
	{
		u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL);

		if (species != SPECIES_NONE
		&& !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, NULL)) //Don't count Egg types
		{
			if (type == gBaseStats[species].type1
			||  type == gBaseStats[species].type2)
			{
				gSpecialVar_LastResult = i;
				return;
			}
		}
	}

	gSpecialVar_LastResult = PARTY_SIZE;
}

//@Details: Checks if any Pokemon in the player's party can learn Draco Meteor.
//			Does not include a friendship check. That is handled in the tutor part.
//@Returns: LastResult: 1 if any Pokemon can learn Draco Meteor. 0 otherwise.
void sp0CC_CanLearnDracoMeteorInParty(void)
{
	int i, j;

	for (i = 0; i < PARTY_SIZE; ++i)
	{
		u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL);

		if (species != SPECIES_NONE
		&& !GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, NULL)) //Don't count Egg types
		{
			if (gBaseStats[species].type1 == TYPE_DRAGON
			||  gBaseStats[species].type2 == TYPE_DRAGON)
			{
				for (j = 0; j < MAX_MON_MOVES; ++j)
				{
					if (GetMonData(&gPlayerParty[i], MON_DATA_MOVE1 + j, NULL) == MOVE_DRACOMETEOR)
						break;
				}

				if (j != MAX_MON_MOVES)
					continue; //If the Pokemon already knows Draco Meteor then skip it

				gSpecialVar_LastResult = TRUE;
				return;
			}
		}
	}

	gSpecialVar_LastResult = FALSE;
}

#define gTMCasePositionStruct ((u8*) 0x203B10C)
bool8 ShouldSelectItemFromTMCase(void)
{
	return gTMCasePositionStruct[4] == TM_CASE_SELECT_ITEM; //bagMenuType
}

bool8 ShouldSelectItemFromBag(void)
{
	return gBagMenuState.location == BAG_OPEN_SELECT_ITEM; //bagMenuType
}

#define FossilTemplate ((struct SpriteTemplate*) 0x83E0768)
bool8 sp18B_ShowFossilImage(void) {
	u8 result = BoxCreateTask(PicBoxSetter);
	if (result == 1)
		return TRUE;

	result = FindTaskIdByFunc(TaskDestroyPicBox);
	if (result != 0xFF)
		return FALSE;
	if (Var8004 > 255)
		return FALSE;

	u8 pal;
#ifdef FOSSIL_IMAGE_HACK
	LoadSpriteSheets(gFossilImageTable[Var8004].data);

	pal = AddPalRef(5, Var8004);	// dynamic OW pals
	if (pal == 0xFF)
		LoadPalette(gFossilImageTable[Var8004].palette, 0x1d0, 0x20);
	else
		DoLoadSpritePalette(gFossilImageTable[Var8004].palette, pal*16);

#else
	if (Var8004 == 0x8D)
	{
		LoadSpriteSheets((void*) 0x83E17C0);
		pal = AddPalRef(5, Var8004);	// dynamic OW pals
		if (pal == 0xFF)
			LoadPalette((void*) 0x83E17A0, 0x1d0, 0x20);
		else
			DoLoadSpritePalette((void*) 0x83E17A0, pal*16);
	}
	else if (Var8004 == 0x8E)
	{
		LoadSpriteSheets((void*) 0x83E17D0);
		pal = AddPalRef(5, Var8004);	// dynamic OW pals
		if (pal == 0xFF)
			LoadPalette((void*) 0x83E0F80, 0x1d0, 0x20);
		else
			DoLoadSpritePalette((void*) 0x83E0F80, pal*16);
	}
	else
		return FALSE;

#endif

	s16 x = ((Var8005 << 0x13) + 0x280000) >> 0x10;
	s16 y = ((Var8006 << 0x13) + 0x280000) >> 0x10;

	u8 objId = CreateSprite(FossilTemplate, x, y, 0);

	gSprites[objId].oam.paletteNum |= pal;

	u8 taskId = CreateTask(TaskDestroyPicBox, 0x50);
	u8 windowId = CreateWindowFromRect(Var8005, Var8006, 8, 8);
	gTasks[taskId].data[5] = windowId;
	gTasks[taskId].data[0] = 0;
	gTasks[taskId].data[2] = objId;

	SetStandardWindowBorderStyle(windowId, 1);
	ScheduleBgCopyTilemapToVram(0);

	return TRUE;
}

//@Details: Checks if any Pokemon in the player's party can learn the given
//			TM or HM.
//@Input:	Var 0x8000: TM/HM id (1 - 128).
//@Returns: Given Var: Party id of mon that can learn it. PARTY_SIZE if none can.
u8 sp0D0_PokemonInPartyThatCanLearnTMHM(void)
{
	u32 i;
	u16 tm = Var8000;
	for (i = 0; i < ITEMS_COUNT; ++i)
	{
		if (TMIdFromItemId(i) == tm)
			break;
	}

	if (i == ITEMS_COUNT)
		return PARTY_SIZE; //Doesn't have the TM so can't use it without knowing it

	for (i = 0; i < PARTY_SIZE; ++i)
	{
		if (CanMonLearnTMHM(&gPlayerParty[i], tm - 1))
			return i;
	}

	return PARTY_SIZE;
}

// Hall of Fame Fix for Expanded Pokemon
// credit to sagiri: https://github.com/Sagiri/fame-hall

void CB2_DoHallOfFameScreen(void);
void Task_Hof_InitTeamSaveData(u8 taskId);
void Task_Hof_DisplayMon(u8 taskId);
void Task_Hof_PrintMonInfoAfterAnimating(u8 taskId);
void Task_Hof_TryDisplayAnotherMon(u8 taskId);
void Task_HofPC_CopySaveData(u8 taskId);
void Task_HofPC_DrawSpritesPrintText(u8 taskId);
void Task_HofPC_PrintMonInfo(u8 taskId);
void Task_Hof_InitMonData(u8 taskId);
void HallOfFame_PrintMonInfo(struct HallofFameMon *currMon, unusedArg u8 a1, unusedArg u8 a2);


void CB2_DoHallOfFameScreen(void) {
	if (!InitHallOfFameScreen()) {
		u8 taskId = CreateTask(Task_Hof_InitMonData, 0);
		gTasks[taskId].tDontSaveData = FALSE;
		sHofMonPtr = Calloc(sizeof(*sHofMonPtr));
	}
}


void Task_Hof_InitTeamSaveData(u8 taskId) {
	u16 i;

	Var8005 = 50;	// 020370c2
	struct HallofFameTeam *lastSavedTeam = (struct HallofFameTeam *)(gDecompressionBuffer);

	sub_8112450();

	if (gHasHallOfFameRecords == 0)
		Memset(gDecompressionBuffer, 0, 0x2000);
	else
	{
		if (SaveLoadGameData(3) != TRUE)
			Memset(gDecompressionBuffer, 0, 0x2000);
	}

	for (i = 0; i < HALL_OF_FAME_MAX_TEAMS; ++i, ++lastSavedTeam)
	{
		if (lastSavedTeam->mon[0].species == SPECIES_NONE)
			break;
	}

	if (i >= HALL_OF_FAME_MAX_TEAMS) {
		struct HallofFameTeam *afterTeam = (struct HallofFameTeam *)(gDecompressionBuffer);
		struct HallofFameTeam *beforeTeam = (struct HallofFameTeam *)(gDecompressionBuffer);
		afterTeam++;
		for (i = 0; i < HALL_OF_FAME_MAX_TEAMS - 1; ++i, beforeTeam++, afterTeam++) {
			*beforeTeam = *afterTeam;
		}
		lastSavedTeam--;
	}
	*lastSavedTeam = *sHofMonPtr;

	NewMenuHelpers_DrawDialogueFrame(0, 0);
	AddTextPrinterParameterized2(0, 1, gText_SavingDontTurnOffPower, 0, NULL, 2, 1, 3);
	CopyWindowToVram(0, 3);
	gTasks[taskId].func = Task_Hof_TrySaveData;
}


void Task_Hof_DisplayMon(u8 taskId) {
	u8 spriteId;
	s16 xPos, yPos, field4, field6;

	u16 currMonId = gTasks[taskId].tDisplayedMonId;
	struct HallofFameMon *currMon = &sHofMonPtr->mon[currMonId];

	if (gTasks[taskId].tMonNumber > 3) {
		xPos = sHallOfFame_MonFullTeamPositions[currMonId][0];
		yPos = sHallOfFame_MonFullTeamPositions[currMonId][1];
		field4 = sHallOfFame_MonFullTeamPositions[currMonId][2];
		field6 = sHallOfFame_MonFullTeamPositions[currMonId][3];
	} else {
		xPos = sHallOfFame_MonHalfTeamPositions[currMonId][0];
		yPos = sHallOfFame_MonHalfTeamPositions[currMonId][1];
		field4 = sHallOfFame_MonHalfTeamPositions[currMonId][2];
		field6 = sHallOfFame_MonHalfTeamPositions[currMonId][3];
	}

	// if (currMon->species == SPECIES_EGG)
	//	 field6 += 10;

	spriteId = CreateMonPicSprite_HandleDeoxys(currMon->species, currMon->tid, currMon->personality, 1, xPos, yPos, currMonId, 0xFFFF);
	gSprites[spriteId].tDestinationX = field4;
	gSprites[spriteId].tDestinationY = field6;
	gSprites[spriteId].data[0] = 0;
	// gSprites[spriteId].tSpecies = currMon->species;
	gSprites[spriteId].callback = SpriteCB_GetOnScreenAndAnimate;
	gTasks[taskId].tMonSpriteId(currMonId) = spriteId;
	ClearDialogWindowAndFrame(0, TRUE);
	gTasks[taskId].func = Task_Hof_PrintMonInfoAfterAnimating;
}


void Task_Hof_PrintMonInfoAfterAnimating(u8 taskId) {
	u16 currMonId = gTasks[taskId].tDisplayedMonId;
	struct HallofFameMon *currMon = &sHofMonPtr->mon[currMonId];
	struct Sprite *monSprite = &gSprites[gTasks[taskId].tMonSpriteId(currMonId)];

	if (monSprite->data[0]) {
		if (currMon->species != SPECIES_EGG)
			PlayCry1(currMon->species, 0);

		HallOfFame_PrintMonInfo(currMon, 0, 14);
		gTasks[taskId].tFrameCount = 120;
		gTasks[taskId].func = Task_Hof_TryDisplayAnotherMon;
	}
}


void Task_Hof_TryDisplayAnotherMon(u8 taskId) {
	u16 currPokeID = gTasks[taskId].tDisplayedMonId;
	struct HallofFameMon *currMon = &sHofMonPtr->mon[currPokeID];

	if (gTasks[taskId].tFrameCount != 0) {
		gTasks[taskId].tFrameCount--;
	} else {
		sHofFadingRelated |= (0x10000 << gSprites[gTasks[taskId].tMonSpriteId(currPokeID)].oam.paletteNum);
		if (gTasks[taskId].tDisplayedMonId <= 4 && currMon[1].species != SPECIES_NONE)  // there is another pokemon to display
		{
			gTasks[taskId].tDisplayedMonId++;
			BeginNormalPaletteFade(sHofFadingRelated, 0, 12, 12, RGB(22, 24, 29));
			gSprites[gTasks[taskId].tMonSpriteId(currPokeID)].oam.priority = 1;
			gTasks[taskId].func = Task_Hof_DisplayMon;
		} else {
			gTasks[taskId].func = Task_Hof_PaletteFadeAndPrintWelcomeText;
		}
	}
}



void Task_HofPC_CopySaveData(u8 taskId) {
	HofPC_CreateWindow(0, 0x1E, 0, 0xC, 0x226);
	if (SaveLoadGameData(3) != 1) {
		gTasks[taskId].func = Task_HofPC_PrintDataIsCorrupted;
	}
	else
	{
		u16 i;
		struct HallofFameTeam *savedTeams;

		CpuCopy16(gDecompressionBuffer, sHofMonPtr, 0x2000);
		savedTeams = sHofMonPtr;
		for (i = 0; i < HALL_OF_FAME_MAX_TEAMS; ++i, ++savedTeams)
		{
			if (savedTeams->mon[0].species == SPECIES_NONE)
				break;
		}

		if (i < HALL_OF_FAME_MAX_TEAMS)
			gTasks[taskId].tCurrTeamNo = i - 1;
		else
			gTasks[taskId].tCurrTeamNo = HALL_OF_FAME_MAX_TEAMS - 1;

		gTasks[taskId].tCurrPageNo = GetGameStat(GAME_STAT_ENTERED_HOF);

		gTasks[taskId].func = Task_HofPC_DrawSpritesPrintText;
	}
}



void Task_HofPC_DrawSpritesPrintText(u8 taskId) {
	struct HallofFameTeam *savedTeams = sHofMonPtr;
	struct HallofFameMon *currMon;
	u16 i;

	for (i = 0; i < gTasks[taskId].tCurrTeamNo; ++i)
		savedTeams++;

	currMon = &savedTeams->mon[0];
	sHofFadingRelated = 0;
	gTasks[taskId].tCurrMonId = 0;
	gTasks[taskId].tMonNo = 0;

	for (i = 0; i < PARTY_SIZE; ++i, currMon++) {
		if (currMon->species != 0)
			gTasks[taskId].tMonNo++;
	}

	currMon = &savedTeams->mon[0];

	for (i = 0; i < PARTY_SIZE; ++i, currMon++) {
		if (currMon->species != 0) {
			u16 spriteId;
			s16 posX, posY;

			if (gTasks[taskId].tMonNo > 3) {
				posX = sHallOfFame_MonFullTeamPositions[i][2];
				posY = sHallOfFame_MonFullTeamPositions[i][3];
			} else {
				posX = sHallOfFame_MonHalfTeamPositions[i][2];
				posY = sHallOfFame_MonHalfTeamPositions[i][3];
			}

			if (currMon->species == SPECIES_EGG)
				posY += 10;

			spriteId = CreateMonPicSprite_HandleDeoxys(currMon->species, currMon->tid, currMon->personality, 1, posX, posY, i, 0xFFFF);
			gSprites[spriteId].oam.priority = 1;
			gTasks[taskId].tMonSpriteId(i) = spriteId;
		} else {
			gTasks[taskId].tMonSpriteId(i) = 0xFF;
		}
	}

	BlendPalettes(0xFFFF0000, 0xC, RGB(22, 24, 29));

	ConvertIntToDecimalStringN(gStringVar1, gTasks[taskId].tCurrPageNo, STR_CONV_MODE_LEFT_ALIGN, 3);
	StringExpandPlaceholders(gStringVar4, gText_HOFNumber);

	if (gTasks[taskId].tCurrTeamNo <= 0)
		HofPC_PutText(gStringVar4, gText_PickCancel, 0, 0, TRUE);
	else
		HofPC_PutText(gStringVar4, gText_PickNextCancel, 0, 0, TRUE);

	gTasks[taskId].func = Task_HofPC_PrintMonInfo;
}



void Task_HofPC_PrintMonInfo(u8 taskId) {
	struct HallofFameTeam *savedTeams = sHofMonPtr;
	struct HallofFameMon *currMon;
	u16 i;
	u16 currMonID;

	for (i = 0; i < gTasks[taskId].tCurrTeamNo; ++i)
		savedTeams++;

	for (i = 0; i < PARTY_SIZE; ++i) {
		u16 spriteId = gTasks[taskId].tMonSpriteId(i);
		if (spriteId != 0xFF)
			gSprites[spriteId].oam.priority = 1;
	}

	currMonID = gTasks[taskId].tMonSpriteId(gTasks[taskId].tCurrMonId);
	gSprites[currMonID].oam.priority = 0;
	sHofFadingRelated = (0x10000 << gSprites[currMonID].oam.paletteNum) ^ 0xFFFF0000;
	BlendPalettesUnfaded(sHofFadingRelated, 0xC, RGB(22, 24, 29));

	currMon = &savedTeams->mon[gTasks[taskId].tCurrMonId];
	if (currMon->species != SPECIES_EGG)
	{
		StopCryAndClearCrySongs();
		PlayCry1(currMon->species, 0);
	}
	HallOfFame_PrintMonInfo(currMon, 0, 14);

	gTasks[taskId].func = Task_HofPC_HandleInput;
}



void Task_Hof_InitMonData(u8 taskId) {
	u16 i, j;

	gTasks[taskId].tMonNumber = 0;  // valid pokes

	for (i = 0; i < PARTY_SIZE; ++i)
	{
		u8 nick[POKEMON_NAME_LENGTH + 2];
		if (GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) != 0)
		{
			sHofMonPtr->mon[i].species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES2, NULL);
			sHofMonPtr->mon[i].tid = GetMonData(&gPlayerParty[i], MON_DATA_OT_ID, NULL);
			sHofMonPtr->mon[i].personality = GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY, NULL);
			sHofMonPtr->mon[i].lvl = GetMonData(&gPlayerParty[i], MON_DATA_LEVEL, NULL);
			GetMonData(&gPlayerParty[i], MON_DATA_NICKNAME, nick);
			for (j = 0; j < POKEMON_NAME_LENGTH; ++j)
			{
				sHofMonPtr->mon[i].nick[j] = nick[j];
			}
			gTasks[taskId].tMonNumber++;
		}
		else
		{
			sHofMonPtr->mon[i].species = 0;
			sHofMonPtr->mon[i].tid = 0;
			sHofMonPtr->mon[i].personality = 0;
			sHofMonPtr->mon[i].lvl = 0;
			sHofMonPtr->mon[i].nick[0] = EOS;
		}
	}

	sHofFadingRelated = 0;
	gTasks[taskId].tDisplayedMonId = 0;
	gTasks[taskId].tPlayerSpriteID = 0xFF;

	for (i = 0; i < PARTY_SIZE; ++i)
	{
		gTasks[taskId].tMonSpriteId(i) = 0xFF;
	}

	if (gTasks[taskId].tDontSaveData != 0)
		gTasks[taskId].func = Task_Hof_SetMonDisplayTask;
	else
		gTasks[taskId].func = Task_Hof_InitTeamSaveData;
}



void HallOfFame_PrintMonInfo(struct HallofFameMon *currMon, unusedArg u8 a1, unusedArg u8 a2) {
	u8 text[30];
	u8 *stringPtr;
	s32 dexNumber;
	s32 width;

	FillWindowPixelBuffer(0, 0);
	PutWindowTilemap(0);

	// dex number
	if (currMon->species != SPECIES_EGG) {
		stringPtr = StringCopy(text, gText_Number);
		dexNumber = SpeciesToPokedexNum(currMon->species);
		if (dexNumber != 0xFFFF) {
			stringPtr[0] = (dexNumber / 100) + CHAR_0;
			stringPtr++;
			dexNumber = umodsi(dexNumber, 100);
			stringPtr[0] = (dexNumber / 10) + CHAR_0;
			stringPtr++;
			stringPtr[0] = umodsi(dexNumber, 10) + CHAR_0;
			stringPtr++;
		} else {
			*(stringPtr)++ = CHAR_QUESTION_MARK;
			*(stringPtr)++ = CHAR_QUESTION_MARK;
			*(stringPtr)++ = CHAR_QUESTION_MARK;
		}
		stringPtr[0] = EOS;
		WindowPrint(0, 2, 0x10, 1, sUnknown_0840C23C, 0, text);
	}

	// nick, species names, gender and level
	Memcpy(text, currMon->nick, POKEMON_NAME_LENGTH);
	text[POKEMON_NAME_LENGTH] = EOS;
	if (currMon->species == SPECIES_EGG) {
		width = 128 - GetStringWidth(2, text, GetFontAttribute(2, 2)) / 2;
		WindowPrint(0, 2, width, 1, sUnknown_0840C23C, 0, text);
		CopyWindowToVram(0, 3);
	} else {
		width = -128 - GetStringWidth(2, text, GetFontAttribute(2, 2));
		WindowPrint(0, 2, width, 1, sUnknown_0840C23C, 0, text);

		text[0] = CHAR_SLASH;
		stringPtr = StringCopy(text + 1, gSpeciesNames[currMon->species]);

		#if (defined NATIONAL_DEX_NIDORAN_M && defined NATIONAL_DEX_NIDORAN_F)
		if (currMon->species != NATIONAL_DEX_NIDORAN_M && currMon->species != NATIONAL_DEX_NIDORAN_F)
		{
		#endif
			switch (GetGenderFromSpeciesAndPersonality(currMon->species, currMon->personality)) {
				case MON_MALE:
					stringPtr[0] = CHAR_MALE;
					stringPtr++;
					break;
				case MON_FEMALE:
					stringPtr[0] = CHAR_FEMALE;
					stringPtr++;
					break;
			}
		#if (defined NATIONAL_DEX_NIDORAN_M && defined NATIONAL_DEX_NIDORAN_F)
		}
		#endif

		stringPtr[0] = EOS;
		WindowPrint(0, 2, 0x80, 1, sUnknown_0840C23C, 0, text);

		stringPtr = StringCopy(text, gText_Level);
		ConvertIntToDecimalStringN(stringPtr, currMon->lvl, STR_CONV_MODE_LEFT_ALIGN, 3);
		WindowPrint(0, 2, 0x20, 0x11, sUnknown_0840C23C, 0, text);

		stringPtr = StringCopy(text, gText_IDNumber);
		ConvertIntToDecimalStringN(stringPtr, (u16)(currMon->tid), STR_CONV_MODE_LEADING_ZEROS, 5);
		WindowPrint(0, 2, 0x60, 0x11, sUnknown_0840C23C, 0, text);

		CopyWindowToVram(0, 3);
	}
}

//Naming Screen Special////////////////////////////////////////////////////////////////////////////
//Pointer+1 at 083E23D0, orig func at 0809F11C
bool8 KeyboardKeyHandler_Character(u8 event)
{
	sub_809E518(3, 0, 0);
	if (event == KBEVENT_PRESSED_A)
	{
		if (gNamingScreenData->templateNum == NAMING_SCREEN_CHOOSE_NUMBER)
		{
			u8 character;
			s16 x, y;

			NamingScreen_GetCursorPos(&x, &y);
			character = NamingScreen_GetCharAtKeyboardPos(x, y);

			if (character < PC_0 || character > PC_9)
			{
				PlaySE(SE_ERROR);
				return FALSE; //Only can select numbers
			}
		}

		bool8 atMaxChars = NameChooserSelectLetter();

		#ifdef AUTO_NAMING_SCREEN_SWAP
		if (gNamingScreenData->currentPage == PAGE_UPPER && GetTextCaretPosition() == 1)
			gNamingScreenData->state = MAIN_STATE_START_PAGE_SWAP;
		#endif

		sub_809EAA8();
		if (atMaxChars)
		{
			SetInputState(INPUT_STATE_DISABLED); //In Emerald it's INPUT_STATE_2 for some reason
			gNamingScreenData->state = MAIN_STATE_MOVE_TO_OK_BUTTON;
		}
	}

	if (gNamingScreenData->templateNum == NAMING_SCREEN_CHOOSE_NUMBER && gNamingScreenData->currentPage != PAGE_OTHERS)
		gNamingScreenData->state = MAIN_STATE_START_PAGE_SWAP; //Jump immediately to numbers page

	return FALSE;
}

#ifdef AUTO_NAMING_SCREEN_SWAP
static u8 GetTextCaretPosition(void)
{
	u8 i;
	for (i = 0; i < gNamingScreenData->template->maxChars; i++)
	{
		if (gNamingScreenData->textBuffer[i] == EOS)
			return i;
	}
	return gNamingScreenData->template->maxChars - 1;
}
#endif

#define sPlayerNamingScreenTemplate (const struct NamingScreenTemplate*) 0x83E245C
#define sPCBoxNamingTemplate (const struct NamingScreenTemplate*) 0x83E2468
#define sMonNamingScreenTemplate (const struct NamingScreenTemplate*) 0x83E2474
#define sRivalNamingScreenTemplate (const struct NamingScreenTemplate*) 0x83E2480

static void ConvertNumberEntryToInteger(void)
{
	u8 numDigits = StringLength(gStringVar1);
	u32 val = 0;

	if (numDigits > 0)
	{
		for (int i = 0; i < numDigits; ++i)
		{
			if (gStringVar1[i] >= PC_0 && gStringVar1[i] <= PC_9)
				val = (val * 10) + (gStringVar1[i] - PC_0);
			else
			{
				val = 0xFFFF;
				break;
			}
		}
	}
	else
		val = 0xFFFF;

	gSpecialVar_LastResult = MathMin(val, 0xFFFF);
	CB2_ReturnToFieldContinueScriptPlayMapMusic();
}

void sp0B3_DoChooseNumberScreen(void)
{
	gStringVar1[0] = EOS; //Empty input
	DoNamingScreen(NAMING_SCREEN_CHOOSE_NUMBER, gStringVar1, 0, 0, 0, (void*) ConvertNumberEntryToInteger);
}

void sp12C_DoEnterPhraseScreen(void)
{
	u8 type;

	switch (Var8000) {
		case 0x1:
			type = NAMING_SCREEN_ENTER_PASSWORD;
			break;
		default:
			type = NAMING_SCREEN_ENTER_PHRASE;
	}

	gStringVar1[0] = EOS; //Empty input
	DoNamingScreen(type, gStringVar1, 0, 0, 0, (void*) CB2_ReturnToFieldContinueScriptPlayMapMusic);
}

void sp12D_CompareEnteredPhrase(void)
{
	gSpecialVar_LastResult = StringCompare(gLoadPointer, gStringVar1);
}

extern const u8 gText_EnterNumber[];
static const struct NamingScreenTemplate sChooseNumberNamingScreenTemplate =
{
	.copyExistingString = 0,
	.maxChars = 4, //Max 9999 - should be preloaded
	.iconFunction = 0,
	.addGenderIcon = 0,
	.initialPage = 1,
	.unused = 35,
	.title = gText_EnterNumber,
};

extern const u8 gText_EnterPhrase[];
static const struct NamingScreenTemplate sEnterPhraseNamingScreenTemplate =
{
	.copyExistingString = 0,
	.maxChars = 12,
	.iconFunction = 0,
	.addGenderIcon = 0,
	.initialPage = 1,
	.unused = 35,
	.title = gText_EnterPhrase,
};

extern const u8 gText_EnterPassword[];
static const struct NamingScreenTemplate sEnterPasswordNamingScreenTemplate =
{
	.copyExistingString = 0,
	.maxChars = 12,
	.iconFunction = 0,
	.addGenderIcon = 0,
	.initialPage = 1,
	.unused = 35,
	.title = gText_EnterPassword,
};

const struct NamingScreenTemplate* const sNamingScreenTemplates[] =
{
	sPlayerNamingScreenTemplate,
	sPCBoxNamingTemplate,
	sMonNamingScreenTemplate,
	sMonNamingScreenTemplate,
	sRivalNamingScreenTemplate,
	&sChooseNumberNamingScreenTemplate,
	&sEnterPhraseNamingScreenTemplate,
	&sEnterPasswordNamingScreenTemplate,
};

void (*const sNamingScreenTitlePrintingFuncs[])(void) =
{
	(void*) (0x809F49C | 1),
	(void*) (0x809F49C | 1),
	(void*) (0x809F4F0 | 1),
	(void*) (0x809F4F0 | 1),
	(void*) (0x809F49C | 1),
	(void*) (0x809F49C | 1),
	(void*) (0x809F49C | 1),
	(void*) (0x809F49C | 1),
};


//Item Find Show Picture Special (Really Callasm)
#define ITEM_ICON_X (10 + 16)
#define ITEM_ICON_Y (8 + 16)
#define sHeaderBoxWindowId (*((u8*) 0x2039A28)) //Steal coin box Id
#if (defined ITEM_PICTURE_ACQUIRE && defined ITEM_DESCRIPTION_ACQUIRE)
static void ShowObtainedItemDescription(unusedArg u16 itemId)
{
	struct WindowTemplate template;
	s16 textX, textY, maxWidth, windowHeight, numLines;
	u8 pocket = GetPocketByItemId(itemId);

	if (pocket == POCKET_KEY_ITEMS || pocket == POCKET_TM_CASE) //Displayed in the middle of the screen
	{
		textX = 1;
		maxWidth = 222;
	}
	else
	{
		textX = ITEM_ICON_X + 2;
		maxWidth = 195;
	}

	Memset(gStringVar4, 0xFF, 500); //Clear enough so GetStringWidth can work properly
	numLines = ReformatItemDescription(itemId, gStringVar4, maxWidth);

	if (numLines == 1)
	{
		textY = 4;
		windowHeight = 3;
	}
	else if (numLines >= 3)
	{
		textY = 0;
		windowHeight = 5;
	}
	else
	{
		textY = 0;
		windowHeight = 4;
	}

	template = SetWindowTemplateFields(0, 1, 1, 28, windowHeight, 14, 0x20);
	sHeaderBoxWindowId = AddWindow(&template);
	FillWindowPixelBuffer(sHeaderBoxWindowId, PIXEL_FILL(1));
	DrawStdFrameWithCustomTileAndPalette(sHeaderBoxWindowId, FALSE, 0x214, 14);
	PutWindowTilemap(sHeaderBoxWindowId);
	CopyWindowToVram(sHeaderBoxWindowId, 3);

	AddTextPrinterParameterized(sHeaderBoxWindowId, 0, gStringVar4, textX, textY, 0, NULL);
	GetSetItemObtained(itemId, FLAG_SET_OBTAINED);
}
#endif

#define ITEM_TAG 0xFDF3
static void ShowItemSpriteOnFind(unusedArg u16 itemId, unusedArg u8* spriteId)
{
	#ifdef ITEM_PICTURE_ACQUIRE
	static const union AffineAnimCmd sSpriteAffineAnim_KeyItemTM[] =
	{
		AFFINEANIMCMD_FRAME(0, 0, 128, 1), //Start rotated left
		AFFINEANIMCMD_FRAME(16, 16, -8, 16), //Double sprite size + rotate right
		AFFINEANIMCMD_FRAME(0, 0, -3, 8), //End at right 24
		AFFINEANIMCMD_FRAME(0, 0, 3, 16), //End at left 24
		AFFINEANIMCMD_FRAME(0, 0, -3, 16), //End at right 24
		AFFINEANIMCMD_FRAME(0, 0, 3, 16), //End at left 24
		AFFINEANIMCMD_FRAME(0, 0, -3, 8), //End at 0
		AFFINEANIMCMD_END,
	};
	
	static const union AffineAnimCmd* const sSpriteAffineAnimTable_KeyItemTM[] =
	{
		sSpriteAffineAnim_KeyItemTM,
	};

	s16 x, y;
	u8 iconSpriteId;

	if (itemId == ITEM_TM59_DRAGON_PULSE && ITEM_TM59_DRAGON_PULSE == 0x177) //Replaced the arrow
		iconSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, ITEM_TM02_DRAGON_CLAW); //Replace the close bag arrow with a Dragon TM sprite
	else
		iconSpriteId = AddItemIconSprite(ITEM_TAG, ITEM_TAG, itemId);

	if (iconSpriteId != MAX_SPRITES)
	{
		u8 pocket = GetPocketByItemId(itemId);
		if (pocket == POCKET_KEY_ITEMS || pocket == POCKET_TM_CASE)
		{ 	//Double the size of the item and place it in the centre of the screen
			x = 96 + 16;
			y = 48 + 16;
			gSprites[iconSpriteId].oam.affineMode = ST_OAM_AFFINE_DOUBLE;
			gSprites[iconSpriteId].oam.matrixNum = AllocOamMatrix();
			gSprites[iconSpriteId].affineAnims = sSpriteAffineAnimTable_KeyItemTM;
			StartSpriteAffineAnim(&gSprites[iconSpriteId], 0);

			#ifdef ITEM_DESCRIPTION_ACQUIRE
			if (!GetSetItemObtained(itemId, FLAG_GET_OBTAINED))
				ShowObtainedItemDescription(itemId);
			#endif
		}
		else
		{
			#ifdef ITEM_DESCRIPTION_ACQUIRE
			if (GetSetItemObtained(itemId, FLAG_GET_OBTAINED))
			{
				//Place the item in the bottom right hand corner of the textbox
				x = 197 + 16;
				y = 124 + 16;
				sHeaderBoxWindowId = 0xFF;
			}
			else //Show description
			{
				x = ITEM_ICON_X;
				y = ITEM_ICON_Y;
				ShowObtainedItemDescription(itemId);
			}
			#else
			//Place the item in the bottom right hand corner of the textbox
			x = 197 + 16;
			y = 124 + 16;
			sHeaderBoxWindowId = 0xFF;
			#endif
		}

		gSprites[iconSpriteId].pos2.x = x;
		gSprites[iconSpriteId].pos2.y = y;
		gSprites[iconSpriteId].oam.priority = 0; //Highest priority
	}

	*spriteId = iconSpriteId;
	#endif
}

static void ClearItemSpriteAfterFind(unusedArg u8 spriteId)
{
	#ifdef ITEM_PICTURE_ACQUIRE
	FreeSpriteTilesByTag(ITEM_TAG);
	FreeSpritePaletteByTag(ITEM_TAG);
	FreeSpriteOamMatrix(&gSprites[spriteId]);
	DestroySprite(&gSprites[spriteId]);
	
	if (sHeaderBoxWindowId != 0xFF) //Description was shown
	{
		ClearDialogWindowAndFrame(sHeaderBoxWindowId, TRUE);
		RemoveWindow(sHeaderBoxWindowId);
	}
	#endif
}

void sp0E4_ShowItemSpriteOnFindObtain(void)
{
	ShowItemSpriteOnFind(Var8004, (u8*) &Var8006);
}

void ShowItemSpriteOnFindHidden(void)
{
	ShowItemSpriteOnFind(Var8005, (u8*) &Var8007);
}

void sp0E5_ClearItemSpriteAfterFindObtain(void)
{
	ClearItemSpriteAfterFind(Var8006);
}

void ClearItemSpriteAfterFindHidden(void)
{
	ClearItemSpriteAfterFind(Var8007);
}

bool8 sp196_TryCopyTMNameToBuffer1(void)
{
	if (IsTMHM(Var8004))
	{
		CopyTMName(gStringVar1, Var8004);
		return TRUE;
	}

	return FALSE;
}

void TryAppendSOntoEndOfItemString(void)
{
	if (Var8005 > 1)
	{
		u8 length = StringLength(gStringVar2);

		switch (gStringVar2[length - 1]) {
			case PC_y:
				gStringVar2[length - 1] = PC_i;
				gStringVar2[length + 0] = PC_e;
				gStringVar2[length + 1] = PC_s;
				gStringVar2[length + 2] = EOS;
				break;
			case PC_Y:
				gStringVar2[length - 1] = PC_I;
				gStringVar2[length + 0] = PC_E;
				gStringVar2[length + 1] = PC_S;
				gStringVar2[length + 2] = EOS;
				break;
			case PC_x:
				gStringVar2[length + 0] = PC_e;
				gStringVar2[length + 1] = PC_s;
				gStringVar2[length + 2] = EOS;
				break;
			case PC_X:
				gStringVar2[length + 0] = PC_E;
				gStringVar2[length + 1] = PC_S;
				gStringVar2[length + 2] = EOS;
				break;
			case PC_s:
			case PC_S:
				break; //Already S on end of string
			default:
				gStringVar2[length + 0] = PC_s;
				gStringVar2[length + 1] = EOS;
		}
	}
}

//Map Name Pop-Up Fix
bool8 ScrCmd_message(struct ScriptContext* ctx)
{
	const u8* msg = (const u8*) ScriptReadWord(ctx);

	if (IsMapNamePopupTaskActive())
	{
		ChangeBgY(0, 0, 0);
		DismissMapNamePopup();
	}

	if (msg == NULL)
		msg = (const u8*) ctx->data[0];

	ShowFieldMessage(msg);
	return FALSE;
}

bool8 ScrCmd_hidemappopup(unusedArg struct ScriptContext * ctx)
{
	if (IsMapNamePopupTaskActive())
	{
		ChangeBgY(0, 0, 0);
		DismissMapNamePopup();
	}

	return FALSE;
}

///////////// EXPANDED TEXT BUFFERS //////////////////////////////////////////
u8* sScriptStringVars[12] =
{
	gStringVar1,	//FD 02
	gStringVar2,	//FD 03
	gStringVar3,	//FD 04
	0,				//FD 05
	0,				//FD 06 is used for Rival name
#ifdef EXPANDED_TEXT_BUFFERS
	gStringVar7,	//FD 07, [buffer5] in XSE
	gStringVar8,	//FD 08
	gStringVar9,	//FD 09
	gStringVarA,	//FD 0A
	gStringVarB,	//FD 0B
	gStringVarC,	//FD 0C
	gStringVarD,	//FD 0D
#else
	(u8*) 0x8415A2C, //FD 07 = RUBY
	(u8*) 0x8415A36, //FD 08 = MAGMA
	(u8*) 0x8415A31, //FD 09 = AQUA
	(u8*) 0x8415A43, //FD 0A = MAXIE
	(u8*) 0x8415A3C, //FD 0B = ARCHIE
	(u8*) 0x8415A50, //FD 0C = GROUDON
	(u8*) 0x8415A49, //FD 0D = KYOGRE
#endif
};

u8* LoadTextBuffer07(void)
{
	return sScriptStringVars[5];
}

u8* LoadTextBuffer08(void)
{
	return sScriptStringVars[6];
}

u8* LoadTextBuffer09(void)
{
	return sScriptStringVars[7];
}

u8* LoadTextBuffer0A(void)
{
	return sScriptStringVars[8];
}

u8* LoadTextBuffer0B(void)
{
	return sScriptStringVars[9];
}

u8* LoadTextBuffer0C(void)
{
	return sScriptStringVars[10];
}

u8* LoadTextBuffer0D(void)
{
	return sScriptStringVars[11];
}

//@Details: Buffers the given ability name to the chosen buffer.
//@Input: Var 0x8000 - Ability Num
//		  Var 0x8001 - Buffer #
void sp0CF_BufferAbilityName(void)
{
	const u8* name = GetAbilityName(Var8000);
	StringCopy(sScriptStringVars[Var8001], name);
}

//////////////////EXPANDED COINS///////////////////////
#ifdef SAVE_BLOCK_EXPANSION
	#if MAX_COINS_DIGITS == 5
		#define MAX_COINS 99999
	#elif MAX_COINS_DIGITS == 6
		#define MAX_COINS 999999
	#elif MAX_COINS_DIGITS == 7
		#define MAX_COINS 9999999
	#elif MAX_COINS_DIGITS == 8
		#define MAX_COINS 99999999
	#elif MAX_COINS_DIGITS == 9
		#define MAX_COINS 999999999
	#else
		#define MAX_COINS 9999
	#endif
#else
	#define MAX_COINS 9999
#endif

u32 GetCoins(void)
{
	#ifdef SAVE_BLOCK_EXPANSION
		return gPlayerCoins;
	#else
		return gSaveBlock1->coins;
	#endif
}


void SetCoins(u32 numCoins)
{
	#ifdef SAVE_BLOCK_EXPANSION
		gPlayerCoins = numCoins;
	#else
		gSaveBlock1->coins = MathMin(numCoins, 0xFFFF);
	#endif
}


bool8 GiveCoins(u32 toAdd)
{
	u32 newAmount;
	u32 current = GetCoins();
	if (current >= MAX_COINS)
		return FALSE;
	else if (current + toAdd >= MAX_COINS)
		newAmount = MAX_COINS;
	else
		newAmount = current + toAdd;

	SetCoins(newAmount);
	return TRUE;
}


bool8 TakeCoins(u32 toTake)
{
	u32 newAmount;
	u32 current = GetCoins();
	if (toTake > current)
		return FALSE;
	else
		newAmount = current - toTake;

	SetCoins(newAmount);
	return TRUE;
}


// Check if player has a certain number of coins
bool8 scrB3_CheckCoins(struct ScriptContext *ctx)
{
	#ifndef REPLACE_SOME_VANILLA_SPECIALS
	u16 *ptr = GetVarPointer(ScriptReadHalfword(ctx));
	*ptr = GetCoins();
	#else
	u32 amount, arg;

	#ifndef UNBOUND
		arg = ScriptReadHalfword(ctx);
		#ifdef SAVE_BLOCK_EXPANSION
		if (arg == 0xFFFF)
			amount = (Var8000 << 16) + Var8001;
		else
			amount = VarGet(arg);
		#else
		amount = VarGet(arg);
		#endif
	#else //Unbound
		arg = ScriptReadWord(ctx);
		amount = arg;
	#endif

	if (GetCoins() >= amount)
		gSpecialVar_LastResult = TRUE;
	else
		gSpecialVar_LastResult = FALSE;
	#endif

	return FALSE;
}


// Add a certain number of coins
bool8 scrB4_AddCoins(struct ScriptContext *ctx)
{
	u32 amount;
	u16 arg = ScriptReadHalfword(ctx);
	#if (defined SAVE_BLOCK_EXPANSION && defined REPLACE_SOME_VANILLA_SPECIALS)
		if (arg == 0xFFFF)
			amount = (Var8000 << 16) + Var8001;
		else
			amount = VarGet(arg);
	#else
		amount = VarGet(arg);
	#endif

	if (GiveCoins(amount))
		gSpecialVar_LastResult = FALSE;
	else
		gSpecialVar_LastResult = TRUE;
	return FALSE;
}


// Subtract a certain number of coins
bool8 scrB5_SubtractCoins(struct ScriptContext *ctx)
{
	u32 amount;
	u16 arg = ScriptReadHalfword(ctx);
	#if (defined SAVE_BLOCK_EXPANSION && defined REPLACE_SOME_VANILLA_SPECIALS)
		if (arg == 0xFFFF)
			amount = (Var8000 << 16) + Var8001;
		else
			amount = VarGet(arg);
	#else
		amount = VarGet(arg);
	#endif

	if (TakeCoins(amount) == TRUE)
		gSpecialVar_LastResult = FALSE;
	else
		gSpecialVar_LastResult = TRUE;
	return FALSE;
}


void ConvertCoinInt(u32 coinAmount)
{
	ConvertIntToDecimalStringN(gStringVar1, coinAmount, STR_CONV_MODE_RIGHT_ALIGN, MAX_COINS_DIGITS);
}

#ifdef SCROLLING_MULTICHOICE

//////////////////////////////////
///// Scrolling Multichoice //////
//////////////////////////////////
//Var8000 - set index
//Var8001 - num choices showed at once
//special 0x158
//waitstate

//Text Declarations
extern const u8 sExampleText_1[];
extern const u8 sExampleText_2[];
extern const u8 sExampleText_3[];
extern const u8 sExampleText_4[];
extern const u8 sExampleText_5[];
extern const u8 sExampleText_6[];
extern const u8 sExampleText_7[];
extern const u8 sExampleText_8[];
extern const u8 sExampleText_9[];
extern const u8 sExampleText_10[];

extern const u8 sText_1[];
extern const u8 sText_2[];
extern const u8 sText_3[];
extern const u8 sText_4[];
extern const u8 sText_5[];
extern const u8 sText_6[];
extern const u8 sText_7[];
extern const u8 sText_8[];
extern const u8 sText_9[];
extern const u8 sText_10[];
extern const u8 sText_11[];
extern const u8 sText_12[];
extern const u8 sText_13[];
extern const u8 sText_14[];
extern const u8 sText_15[];

//Scrolling Lists
static const u8* sMultichoiceSet1[] =
{
	sExampleText_1,
	sExampleText_2,
	sExampleText_3,
	sExampleText_4,
	sExampleText_5,
	sExampleText_6,
	sExampleText_7,
	sExampleText_8,
	sExampleText_9,
	sExampleText_10,
};

static const u8* sMultichoiceSet2[] =
{
	sExampleText_1,
	sExampleText_2,
	sExampleText_3,
	sExampleText_4,
	sExampleText_5,
	sExampleText_6,
	sExampleText_7,
	sExampleText_8,
	sExampleText_9,
	sExampleText_10,
};

// Multichoice Lists
const struct ScrollingMulti gScrollingSets[] =
{
	{sMultichoiceSet1, ARRAY_COUNT(sMultichoiceSet1)},
	{sMultichoiceSet2, ARRAY_COUNT(sMultichoiceSet2)},
};

//Link number of opts shown at once to the box height
struct ScrollingSizePerOpts
{
	u8 maxShowed;
	u8 height;
};

static const struct ScrollingSizePerOpts sScrollingSizes[] =
{
	{.maxShowed = 2, .height = 3},
	{.maxShowed = 3, .height = 5},
	{.maxShowed = 4, .height = 7},
	{.maxShowed = 5, .height = 9},
	{.maxShowed = 6, .height = 11},
};


#define tMaxShowed 		data[0]
#define tTilemapTop 	data[2]
#define tTilemapLeft 	data[3]
#define tHeight			data[5]

#define MIN_NUM_SHOWED 2
#define MAX_NUM_SHOWED 6

#endif

u32 GetSizeOfMultiList(void)
{
#ifdef SCROLLING_MULTICHOICE
	return gScrollingSets[Var8000].count;
#else
	return 0;
#endif
}

const u8* const* GetScrollingMultiList(void)
{
#ifdef SCROLLING_MULTICHOICE
	return gScrollingSets[Var8000].set;
#else
	return 0;
#endif
}

void SetScrollingListSize(unusedArg u8 taskId)
{
#ifdef SCROLLING_MULTICHOICE
	u8 maxShowed = Var8001;
	maxShowed = MathMin(GetSizeOfMultiList(), maxShowed);
	if (maxShowed < MIN_NUM_SHOWED || maxShowed > MAX_NUM_SHOWED)
		maxShowed = MAX_NUM_SHOWED;

	gTasks[taskId].tMaxShowed = maxShowed;

	for (u8 i = 0; i < ARRAY_COUNT(sScrollingSizes); ++i)
	{
		if (sScrollingSizes[i].maxShowed == gTasks[taskId].tMaxShowed)
		{
			gTasks[taskId].tHeight = sScrollingSizes[i].height;
			break;
		}
	}
	gTasks[taskId].data[1] = 9;
	gTasks[taskId].data[2] = 1;	//x
	gTasks[taskId].data[3] = 1;	//y
	gTasks[taskId].data[4] = 0xC;	//width?
#endif
}
