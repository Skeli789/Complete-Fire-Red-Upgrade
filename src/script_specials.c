#include "defines.h"
#include "../include/fieldmap.h"
#include "../include/list_menu.h"

#include "../include/pokemon_storage_system.h"
#include "../include/new_menu_helpers.h"
#include "../include/naming_screen.h"
#include "../include/hall_of_fame.h"
#include "../include/overworld.h"
#include "../include/region_map.h"
#include "../include/save.h"
#include "../include/sprite.h"
#include "../include/script.h"
#include "../include/sound.h"
#include "../include/string_util.h"
#include "../include/text.h"
#include "../include/window.h"
#include "../include/wild_encounter.h"
#include "../include/constants/pokedex.h"

#include "../include/new/helper_functions.h"
#include "../include/new/catching.h"
#include "../include/new/multi.h"
#include "../include/new/overworld.h"
#include "../include/new/pokemon_storage_system.h"
#include "../include/new/read_keys.h"
#include "../include/new/roamer.h"
#include "../include/new/Vanilla_Functions_battle.h"

struct GbaTimer
{
	u16 init;	// 4000108
	u8 timerFlags;	// 400010A
	u8 empty;		// 400010B
	u16 timerVal;	// 400010C
	u8 timerOn;	// 400010E
};

/*
NOTES: 
	1. Many specials will not work unless you have chosen to expand the save block!
	2. PC selection hack allows a lot all of the attribute getter/setter specials to reference boxed pokemon via var8003
		-var8003 = 1: boxed pokemon (output from pc selection: box num in var8000, slot in var8001)
		-else: menu pokemon (and slot in var8004, etc)
		-Be sure to always set var8003 for data manipulation specials!
		
TO DO:
	-battle specials
	
*/

#define POKERUS_CURED 0x10

#define gGbaTimer ((struct GbaTimer*) 0x4000108)

extern u8 AddPalRef(u8 Type, u16 PalTag);
extern u8 BuildFrontierParty(pokemon_t* party, u16 trainerNum, bool8 firstTrainer, bool8 ForPlayer, u8 side);

extern const struct SwarmData gSwarmTable[];
extern const species_t gSkyBattleBannedSpeciesList[];

static u8 GetTextCaretPosition(void);

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
			attr = GetMonData(&gPlayerParty[Var8004], dataRequest, 0);
	#else
		attr = GetMonData(&gPlayerParty[Var8004], dataRequest, 0);
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

u32* GetBoxedMonAddr(void) {
	return ((u32*) &(gSaveBlock3->boxes[Var8000][Var8001]));
}


u8 sp007_PokemonEVContestStatsChecker(void) {
	u16 mon = Var8004;
	u16 stat = Var8005;
	
	if (mon >= 6)
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
	if (mon >= 6)
		return FALSE;
	else if (ribbon > 0x1F)
		return FALSE;
	return (GetMonDataFromVar8003(MON_DATA_RIBBONS) & ribbon);
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
		
	return GetMonDataFromVar8003(REQ_POKEBALL);
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
	u16 amount = Var8006;
	//u16 new;
	
	if (mon >= 6)
		return;	
	
	//u8 curr = sp007_PokemonEVContestStatsChecker();	// current val
	if (amount & 0x100) 
	{
		// subtracting
		amount ^= 0x100;
		Var8005 = Var8006 - amount;
	}
	else 
		Var8005 = Var8006 + amount;
	// limit to [0,EV_CAP]
	if (Var8005 > EV_CAP)
		Var8005 = EV_CAP;
	else if (Var8005 < 0)
		Var8005 = 0;
	
	// setter at Var8005
	Var8006 = 0;
	switch(stat)
	{
		case CheckIVs_HP:
			return SetMonDataFromVar8003(MON_DATA_HP_IV);
		case CheckIVs_Atk:
			return SetMonDataFromVar8003(MON_DATA_ATK_IV);
		case CheckIVs_Def:
			return SetMonDataFromVar8003(MON_DATA_DEF_IV);
		case CheckIVs_Spd:
			return SetMonDataFromVar8003(MON_DATA_SPEED_IV);
		case CheckIVs_SpAtk:
			return SetMonDataFromVar8003(MON_DATA_SPATK_IV);
		case CheckIVs_SpDef:
			return SetMonDataFromVar8003(MON_DATA_SPDEF_IV);
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
	//bool8 clear = FALSE;
	
	if (mon >= 6)
		return;
	else if (ribbon > 0x1F)
		return;
	
	// check set vs clear
	if (ribbon & 0x100)
		Var8005 = 0;
	else
		Var8005 = 1;
	SetMonDataFromVar8003(MON_DATA_RIBBONS);
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
	else if (ball > NUM_BALLS)
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
	
	if (GetMonDataFromVar8003(MON_DATA_HELD_ITEM) != 0)
	{
		if (Var8005 != 0)
			return;	// cant change existing item
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
//			0 for store to free ram
//			1 for return to party from free ram
//			2 for store from free ram to box
//			3 for store from box to free ram
// 		Var8005: slot num (for special 0xFE, activate trade)
// Outputs:
//		Var800D: success (0) or failure (1)
//
void sp01A_CopyPartyData(void) {
#ifdef SELECT_FROM_PC
	u8 slot = Var8005;
	Var800D = 1;
	if (slot >= 6)
		return;
	
	if (Var8002 == 0)
		// save party slot data to last enemy slot
		Memcpy((void*) &gEnemyParty[5], (void*) &gPlayerParty[slot], 100);
	else if (Var8002 == 1)
		// return last enemy slot data to given party slot
		Memcpy((void*) &gPlayerParty[slot], (void*) &gEnemyParty[5], 100);
	else if (Var8002 == 2)
	{
		// enemy ram to box
		Memcpy((void*) GetBoxedMonAddr(), (void*) &gEnemyParty[5], 80);
		CalculateMonStats((pokemon_t*) GetBoxedMonAddr());
	}
	else if (Var8003 == 3)
	{
		// box to free ram
		Memcpy((void*) &gEnemyParty[5], (void*) GetBoxedMonAddr(), 80);
		CalculateMonStats((pokemon_t*) &gEnemyParty[5]);
	}
	else
		return;
	Var800D = 0;
	return;
#else
	return;
#endif
}


// replace a pokemon party slot with boxed mon
//Inputs:
//	var8000: box num
//	var8001: box index
//	var8002: withdraw (0) or deposit (1)
//	var8005: party slot num
//Outputs:
//	var800d: success(0) or failure(1)
//
void sp01B_SwapPartyAndBoxData(void) {
#ifdef SELECT_FROM_PC
	u8 slot = Var8005;
	Var800D = 1;
	if (slot >= 6)
		return;
	
	if (Var8002 == 1)
	{
		// deposit from party to box
		Memcpy((void*) GetBoxedMonAddr(), (void*) &gPlayerParty[slot], 80);
		CalculateMonStats((pokemon_t*) GetBoxedMonAddr());
		Var800D = 0;
	}
	else
	{
		Memcpy((void*) &gPlayerParty[slot], (void*) GetBoxedMonAddr(), 80);
		CalculateMonStats((pokemon_t*) &gPlayerParty[slot]);
		Var800D = 0;
	}
	return;
#else
	return;
#endif	
}

//Nicknaming Specials//
///////////////////////////////////////////////////////////////////////////////////
void sp07C_BufferNickname(void) {
	void* src;
#ifdef SELECT_FROM_PC
	if (Var8003 == 1)
		src = GetBoxedMonAddr();
	else
		src = &gPlayerParty[Var8004];
#else
	src = &gPlayerParty[Var8004];
#endif	
	GetMonData(src, MON_DATA_NICKNAME, gStringVar1);
	StringGetEnd10(gStringVar1);
}


bool8 sp07D_CheckTradedPokemon(void) {
	void* src;
#ifdef SELECT_FROM_PC
	if (Var8003 == 1)
		src = GetBoxedMonAddr();
	else
		src = &gPlayerParty[Var8004];
#else
	src = &gPlayerParty[Var8004];
#endif
	u16 TID = GetCombinedOTID();
	u16 monOTID = GetMonData(src, MON_DATA_OT_ID, 0);
	if (monOTID == TID)
		return TRUE;
	else
		return FALSE;
}


void NicknameFunc(void) {
	void* src;
#ifdef SELECT_FROM_PC
	if (Var8003 == 1)
		src = GetBoxedMonAddr();
	else
		src = &gPlayerParty[Var8004];
#else
	src = &gPlayerParty[Var8004];
#endif
	SetMonData(src, MON_DATA_NICKNAME, gStringVar2);
	ReturnToFieldContinueScriptPlayMapMusic();
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
	NicknameMalloc(3, gStringVar2, species, gender, PID, (void*) NicknameFunc);
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
		PokemonSlotPurge(&gPlayerParty[slot]);
		gPlayerPartyCount -= 1;
		CompactPartySlots();
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
	BufferPokeNameSize(species, &Var8005);
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
u8 sp076_MeasurePokemon2(void) 
{
	u16 species = Var8006;
	return CalculateHeight(species, &Var8005);;
}



// old man battle with given species/level
// Inputs:
//		var8004: species
//		var8005: level
void sp09C_OldManBattleModifier(void) 
{
	CreateMaleMon(&gEnemyParty[0], Var8004, Var8005);
	ScriptContext2_Enable();
	gMain.savedCallback = ReturnToFieldContinueScriptPlayMapMusic;
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
	||  GetPartyAbility(mon) == ABILITY_LEVITATE)
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


//u16 sp052_GenerateTowerTrainerId(void); //Code in "src/frontier.c"
//void sp053_LoadFrontierIntroBattleMessage(void); //Code in "src/frontier.c"
//void sp054_GetBattleTowerStreak(void) //Code in "src/frontier.c"
//void sp055_UpdateBattleTowerStreak(void) //Code in "src/frontier.c"
//void sp056_DetermineBattlePointsToGive(void) //Code in "src/frontier.c"

//@Details: Buffers the map name where there is currently a swarm to buffer1,
//			and the species name where there is currently a swarm to buffer2.
void sp058_BufferSwarmText(void) 
{
	u8 index = VarGet(SWARM_INDEX_VAR);
	u8 mapName = gSwarmTable[index].mapName;
	u16 species = gSwarmTable[index].species;
	
	GetMapName(sScriptStringVars[0], mapName, 0);
	StringCopy(sScriptStringVars[1], gSpeciesNames[species]);
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

void sp05A_WildDataSwitch(void) {
	return;
}

void sp05B_WildDataSwitchCanceller(void) {
	return;
}

void sp0AC_LoadTrainerBDefeatText(void)
{
	sTrainerDefeatSpeech_B = (u8*) gLoadPointer;
}


//Timer Specials//
///////////////////////////////////////////////////////////////////////////////////

//@Details: Starts the timer
void sp046_StartTimer(void) 
{
	gGbaTimer->init = 0xC000;
	gGbaTimer->timerFlags = 0x83;
	gGbaTimer->timerVal = 0;
	gGbaTimer->timerOn = 0x84;
	return;
}

//@Details: Pauses the timer
void sp047_HaltTimer(void) 
{
	gGbaTimer->timerOn = 4;
	gGbaTimer->timerFlags = 3;
	return;
}


//@Details: Unpauses the timer
void sp048_ResumeTimer(void) 
{
	gGbaTimer->timerVal = gGbaTimer->timerVal;
	gGbaTimer->timerOn = 0x84;
	gGbaTimer->timerFlags = 0x83;
	return;
}

//@Details:	Stops the timer.
//@Returns: The time on the timer.
u16 sp049_StopTimer(void) 
{
	gGbaTimer->timerOn = 0;
	gGbaTimer->timerFlags = 0;
	u16 time = gGbaTimer->timerVal;
	gGbaTimer->init = time;
	gGbaTimer->timerVal = time;
	return time;
}

//@Returns: The time on the timer.
u16 sp04A_GetTimerValue(void) 
{
	return gGbaTimer->timerVal;
}



void sp04C_UpdatePlaytime(void) 
{
	u8 secs = gGbaTimer->timerVal + gSaveBlock2->playTimeSeconds;	
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
//			Special 0x4E.
void sp04F_StartTimerAtTime(void) 
{
#ifdef SAVE_BLOCK_EXPANSION
	sp046_StartTimer();
	gGbaTimer->timerOn = 0;
	gGbaTimer->timerVal = gTimerValue;
	gGbaTimer->timerOn = 0x84;
#endif
}


//@Details: Stores the timer value stored by
//			Special 0x4E.
//@Returns: Var 0x8006 - Timer time.
void sp050_StoreTimerToVariable(void) 
{
#ifdef SAVE_BLOCK_EXPANSION
	VarSet(Var8006, gTimerValue);
#endif
}


//@Details: Loads the value at a given variable and stores
//			it to the saved timer.
//@Input:	Var 0x8006 - Variable that is holding timer.
void sp061_LoadTimerFromVariable(void) {
#ifdef SAVE_BLOCK_EXPANSION
	gTimerValue = VarGet(Var8006);
#endif
}

//Safari Specials//
///////////////////////////////////////////////////////////////////////////////////


//@Details: The check Safari ball quantity. 
//@Returns: 1. Var 0x8004 - Normal Safari Ball number.
//		  2. Var 0x8005 - The extra ball slot number
//		  3. To a given variable the number as a full integer. Max value is 0x63FF
u16 sp086_GetSafariBalls(void) 
{
	Var8004 = gSafariBallNumber;
	Var8005 = *(&gSafariBallNumber + 1);
	return Var8004 + Var8005;
}


//@Details: An increase\decrease safari balls code.
//		  This code doesn't allow people to go over 100 max balls 
//		  as a safety measure.
//@Input: Var 0x8004 as the number to increase or decrease the balls by. 
//	    0x1aa decreases the balls by aa, and 0x0aa increases them by aa. 
//@Returns: None
void sp087_ChangeSafariBalls(void) 
{
	u16 input = Var8004;
	s32 calc;
	
	if (input & 0x100)
	{
		input ^= 0x100;
		input = MathMax(100, input);
		input *= -1;
	}
	else
		input = MathMax(100, input);
	
	calc = gSafariBallNumber + input;
	if (calc > 100)
		calc = 100;
	else if (calc < 0)
		calc = 0;
	gSafariBallNumber = calc;
	return;
}

//@Details: The get safari pedometer special.
//@Returns: To a given variable the number of remaining steps
u16 sp088_GetSafariCounter(void) 
{
	return gSafariSteps;
}

//@Details: The Set Safari steps special. 
//	      It allows you to set a specific ammount of steps 
//		  until the safari handler is called.
//Input: Var 0x8004 is the amount of steps to place.
void sp089_SetSafariCounter(void) 
{
	u16 input = Var8004;
	gSafariSteps = input;
}

//Walking Specials//
///////////////////////////////////////////////////////////////////////////////////


//@Details: Tile Number getter, gets the tile's number at (0x8004, 0x8005) location. 
//@Input: 1. Var 0x8004 - X-Coordinate
//	    2. Var 0x8005 - Y-Coordinate
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
//	    2. Var 0x8005 - Y-Coordinate
//@Returns: 1. Var 0x8004 - The tile's background byte
//		  2. Var 0x8005 - The tile's behaviour bytes.
//		  3. To a given variable the background byte.
u16 sp07F_GetTileBehaviour(void) 
{
	u16 x = Var8004;
	u16 y = Var8005;
	u32 field = MapGridGetMetatileField(x + 7, y + 7, 0xFF);
	Var8004 = (field & 0xFF000000) >> 0x18;
	Var8005 = (field & 0xFFFF);
	return Var8004 & 3;
}



/*	// in src/Assembly/script.s
void sp097_StartGroundBattle(void) {
	return;
}

void sp098_StartWaterBattle(void) {
	return;
}
*/

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

u32 GetMinuteDifference(u32 startYear, u8 startMonth, u8 startDay, u8 startHour, u8 startMin, u32 endYear, u8 endMonth, u8 endDay, u8 endHour, u8 endMin);
u32 GetHourDifference(u32 startYear, u8 startMonth, u8 startDay, u8 startHour, u32 endYear, u8 endMonth, u8 endDay, u8 endHour);
u32 GetDayDifference(u32 startYear, u8 startMonth, u8 startDay, u32 endYear, u8 endMonth, u8 endDay);
u32 GetMonthDifference(u32 startYear, u8 startMonth, u32 endYear, u8 endMonth);
u32 GetYearDifference(u32 startYear, u32 endYear);
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
	struct DailyEventVar* dailyData = (struct DailyEventVar*) VarGetAddress(eventVar);
	
	u8 dailyDay = dailyData->day;
	u8 dailyMonth = dailyData->month;
	u32 dailyYear = dailyData->year + dailyData->century * 100;
	
	if (dailyYear > Clock->year
	|| (dailyYear == Clock->year && dailyMonth > Clock->month)
	|| (dailyYear == Clock->year && dailyMonth == Clock->month && dailyDay > Clock->day))
		return FALSE; //Player changed date on their computer.
	
	if (dailyDay != Clock->day
	||  dailyMonth != Clock->month
	||  dailyYear != Clock->year)
	{
		if (setDailyEventVar)
		{
			dailyData->minute = Clock->minute;
			dailyData->hour = Clock->hour;
			dailyData->day = Clock->day;
			dailyData->month = Clock->month;
			dailyData->year = Clock->year % 100;
			dailyData->century = Clock->year / 100;
		}
		toReturn = TRUE;
	}

	return toReturn;
}

//@Details: Updates the time stored in a pair of vars.
//@Input: Var 0x8000 - A var containing the daily event data. 
//					   The var after this one is used as well.
void sp0A1_UpdateTimeInVars(void)
{
	u16 eventVar = Var8000; //Var contained in Var8000
	struct DailyEventVar* data = (struct DailyEventVar*) VarGetAddress(eventVar);

	data->minute = Clock->minute;
	data->hour = Clock->hour;
	data->day = Clock->day;
	data->month = Clock->month;
	data->year = Clock->year % 100;
	data->century = Clock->year / 100;
}

//@Details: Gets the time difference between the data stored in a var and the current time.
//@Input: Var 0x8000 - A var containing the time data. 
//					   The var after this one is used as well.
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
	struct DailyEventVar* startTime = (struct DailyEventVar*) VarGetAddress(eventVar);

	u8 startMinute = startTime->minute;
	u8 startHour = startTime->hour;
	u8 startDay = startTime->day;
	u8 startMonth = startTime->month;
	u32 startYear = startTime->year + startTime->century * 100;
	
	switch (Var8001) {
		case 0: //Get minute difference
			difference = GetMinuteDifference(startYear, startMonth, startDay, startHour, startMinute, Clock->year, Clock->month, Clock->day, Clock->hour, Clock->minute);
			break;
		case 1: //Get hour difference
			difference = GetMinuteDifference(startYear, startMonth, startDay, startHour, startMinute, Clock->year, Clock->month, Clock->day, Clock->hour, Clock->minute) / 60;
			break;
		case 2: //Get day difference.
			difference = GetMinuteDifference(startYear, startMonth, startDay, startHour, startMinute, Clock->year, Clock->month, Clock->day, Clock->hour, Clock->minute) / 60 / 24;
			break;
		case 3: //Get month difference.
			difference = GetMonthDifference(startYear, startMonth, Clock->year, Clock->month);
			break;
		case 4: //Get year difference.
			difference = GetYearDifference(startYear, Clock->year);
			break;
	}

	return difference;
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
	u8 windowId = PicBoxWindow(Var8005, Var8006, 8, 8);
	gTasks[taskId].data[5] = windowId;
	gTasks[taskId].data[0] = 0;
	gTasks[taskId].data[2] = objId;
	
	SetStandardWindowBorderStyle(windowId, 1);
	BgIdMarkForSync(0);
	
	return TRUE;
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
    //     field6 += 10;

	spriteId = CreatePicSprite2(currMon->species, currMon->tid, currMon->personality, 1, xPos, yPos, currMonId, 0xFFFF);
    gSprites[spriteId].tDestinationX = field4;
    gSprites[spriteId].tDestinationY = field6;
    gSprites[spriteId].data[0] = 0;
    // gSprites[spriteId].tSpecies = currMon->species;
    gSprites[spriteId].callback = SpriteCB_GetOnScreenAndAnimate;
    gTasks[taskId].tMonSpriteId(currMonId) = spriteId;
    DeleteWindow(0, TRUE);
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

            spriteId = CreatePicSprite2(currMon->species, currMon->tid, currMon->personality, 1, posX, posY, i, 0xFFFF);
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
        AddTextPrinterParameterized3(0, 2, 0x10, 1, sUnknown_0840C23C, 0, text);
    }

    // nick, species names, gender and level
    Memcpy(text, currMon->nick, POKEMON_NAME_LENGTH);
    text[POKEMON_NAME_LENGTH] = EOS;
    if (currMon->species == SPECIES_EGG) {
        width = 128 - GetStringWidth(2, text, GetFontAttribute(2, 2)) / 2;
        AddTextPrinterParameterized3(0, 2, width, 1, sUnknown_0840C23C, 0, text);
        CopyWindowToVram(0, 3);
    } else {
        width = -128 - GetStringWidth(2, text, GetFontAttribute(2, 2));
        AddTextPrinterParameterized3(0, 2, width, 1, sUnknown_0840C23C, 0, text);

        text[0] = CHAR_SLASH;
        stringPtr = StringCopy(text + 1, gSpeciesNames[currMon->species]);

        if (currMon->species != NATIONAL_DEX_NIDORAN_M && currMon->species != NATIONAL_DEX_NIDORAN_F) {
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
        }

        stringPtr[0] = EOS;
        AddTextPrinterParameterized3(0, 2, 0x80, 1, sUnknown_0840C23C, 0, text);

        stringPtr = StringCopy(text, gText_Level);
        ConvertIntToDecimalStringN(stringPtr, currMon->lvl, STR_CONV_MODE_LEFT_ALIGN, 3);
        AddTextPrinterParameterized3(0, 2, 0x20, 0x11, sUnknown_0840C23C, 0, text);

        stringPtr = StringCopy(text, gText_IDNumber);
        ConvertIntToDecimalStringN(stringPtr, (u16)(currMon->tid), STR_CONV_MODE_LEADING_ZEROS, 5);
        AddTextPrinterParameterized3(0, 2, 0x60, 0x11, sUnknown_0840C23C, 0, text);

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
        bool8 var = NameChooserSelectLetter();

		#ifdef AUTO_NAMING_SCREEN_SWAP
        if (gNamingScreenData->currentPage == PAGE_UPPER && GetTextCaretPosition() == 1)
			gNamingScreenData->state = MAIN_STATE_START_PAGE_SWAP;
		#endif

        sub_809EAA8();
        if (var)
        {
            SetInputState(INPUT_STATE_DISABLED); //In Emerald it's INPUT_STATE_2 for some reason
            gNamingScreenData->state = MAIN_STATE_MOVE_TO_OK_BUTTON;
        }
    }
    return FALSE;
}

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