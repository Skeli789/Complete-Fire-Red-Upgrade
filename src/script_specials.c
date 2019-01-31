#include "defines.h"
#include "helper_functions.h"
#include "catching.h"

//Pokemon Specials//
///////////////////////////////////////////////////////////////////////////////////

/*
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

u8 sp007_PokemonEVContestStatsChecker(void) {
	u16 mon = Var8004;
	u16 stat = Var8005;
	
	if (mon >= 6)
		return 0;
	
	switch(stat) {
		case CheckEVs_HP:
			return gPlayerParty[mon].hpEv;
		case CheckEVs_Atk:
			return gPlayerParty[mon].atkEv;
		case CheckEVs_Def:
			return gPlayerParty[mon].defEv;
		case CheckEVs_Spd:
			return gPlayerParty[mon].spdEv;
		case CheckEVs_SpAtk:
			return gPlayerParty[mon].spAtkEv;
		case CheckEVs_SpDef:
			return gPlayerParty[mon].spDefEv;
		case CheckEVs_Coolness:
			return gPlayerParty[mon].coolness;
		case CheckEVs_Beauty:
			return gPlayerParty[mon].beauty;
		case CheckEVs_Cuteness:
			return gPlayerParty[mon].cuteness;
		case CheckEVs_Smartness:
			return gPlayerParty[mon].smartness;
		case CheckEVs_Toughness:
			return gPlayerParty[mon].toughness;
		case CheckEVs_Luster:
			return gPlayerParty[mon].feel;
		default:
			return 0;
	}
}

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

u8 sp008_PokemonIVChecker(void) {
	u16 mon = Var8004;
	u16 stat = Var8005;
	
	if (mon >= 6)
		return 0;
	
	switch(stat) {
		case CheckIVs_HP:
			return gPlayerParty[mon].hpIV;
		case CheckIVs_Atk:
			return gPlayerParty[mon].attackIV;
		case CheckIVs_Def:
			return gPlayerParty[mon].defenseIV;
		case CheckIVs_Spd:
			return gPlayerParty[mon].speedIV;
		case CheckIVs_SpAtk:
			return gPlayerParty[mon].spAttackIV;
		case CheckIVs_SpDef:
			return gPlayerParty[mon].spDefenseIV;
		default:
			return 0;
	}
}

bool8 sp009_PokemonRibbonChecker(void) {
	u16 mon = Var8004;
	u16 ribbon = Var8005;
	
	if (mon >= 6)
		return FALSE;
	
	if (ribbon > 0x1F)
		return FALSE;
		
	(u32*) pointer = (u32*) &gPlayerParty[mon];
	pointer += (76 / sizeof(u32));
	u32 data = *pointer;
	
	if (data & gBitTable[ribbon])
		return TRUE;
	else
		return FALSE;
}

#define POKERUS_CURED 0x10

u8 sp00A_CheckPokerusTimer(void) {
	u16 mon = Var8004;
	
	if (mon >= 6)
		return 0;
		
	if (gPlayerParty[mon].pokerusDays == 0
	&&  gPlayerParty[mon].pokerusStrain != 0)
		return POKERUS_CURED;
		
	else
		return gPlayerParty[mon].pokerusDays == 0;
}

u16 sp00B_CheckPokeball(void) {
	u16 mon = Var8004;
	
	if (mon >= 6)
		return 0;
		
	return GetMonData(&gPlayerParty[mon], REQ_POKEBALL, 0);
}

u8 sp00C_CheckCaptureLocation(void) {
	u16 mon = Var8004;
	
	if (mon >= 6)
		return 0;
		
	return gPlayerParty[mon].metLocation;
}

u8 sp00D_CheckHappiness(void) {
	u16 mon = Var8004;
	
	if (mon >= 6)
		return 0;
		
	return gPlayerParty[mon].friendship;
}

item_t sp00E_CheckHeldItem(void) {
	u16 mon = Var8004;
	
	if (mon >= 6)
		return 0;
		
	return gPlayerParty[mon].item;
}

void sp00F_EVAdderSubtracter(void) {
	u16 mon = Var8004;
	u16 stat = Var8005;
	u16 amount = Var8006;
	u8* modify;
	s32 calc;
	
	if (amount & 0x100) 
	{
		amount ^= 0x100;
		amount *= -1;
		if (amount < -255)
			amount = -255;
	}
	else if (amount > 255)
		amount = 255
	
	if (mon >= 6)
		return;
	
	switch(stat) {
		case CheckEVs_HP:
			modify = &gPlayerParty[mon].hpEv;
			break;
		case CheckEVs_Atk:
			modify = &gPlayerParty[mon].atkEv;
			break;
		case CheckEVs_Def:
			modify = &gPlayerParty[mon].defEv;
			break;
		case CheckEVs_Spd:
			modify = &gPlayerParty[mon].spdEv;
			break;
		case CheckEVs_SpAtk:
			modify = &gPlayerParty[mon].spAtkEv;
			break;
		case CheckEVs_SpDef:
			modify = &gPlayerParty[mon].spDefEv;
			break;
		case CheckEVs_Coolness:
			modify = &gPlayerParty[mon].coolness;
			break;
		case CheckEVs_Beauty:
			modify = &gPlayerParty[mon].beauty;
			break;
		case CheckEVs_Cuteness:
			modify = &gPlayerParty[mon].cuteness;
			break;
		case CheckEVs_Smartness:
			modify = &gPlayerParty[mon].smartness;
			break;
		case CheckEVs_Toughness:
			modify = &gPlayerParty[mon].toughness;
			break;
		case CheckEVs_Luster:
			modify = &gPlayerParty[mon].feel;
			break;
		default:
			return;
		
		calc = *modify;
		calc += amount;
		
		if (calc < 0)
			calc = 0;
		
		if (calc > 255)
			calc = 255;
			
		*modify = calc;
	}
}

void sp010_IVSetter(void) {
	u16 mon = Var8004;
	u16 stat = Var8005;
	u16 amount = Var8006;
	
	if (amount > 31)
		amount = 31
	
	if (mon >= 6)
		return;
	
	switch(stat) {
		case CheckIVs_HP:
			gPlayerParty[mon].hpIV = amount;
			break;
		case CheckIVs_Atk:
			gPlayerParty[mon].attackIV = amount;
			break;
		case CheckIVs_Def:
			gPlayerParty[mon].defenseIV = amount;
			break;
		case CheckIVs_Spd:
			gPlayerParty[mon].speedIV = amount;
			break;
		case CheckIVs_SpAtk:
			gPlayerParty[mon].spAttackIV = amount;
			break;
		case CheckIVs_SpDef:
			gPlayerParty[mon].spDefenseIV = amount;
			break;
		case SetAllIVs:
			gPlayerParty[mon].hpIV = amount;
			gPlayerParty[mon].attackIV = amount;
			gPlayerParty[mon].defenseIV = amount;
			gPlayerParty[mon].speedIV = amount;
			gPlayerParty[mon].spAttackIV = amount;
			gPlayerParty[mon].spDefenseIV = amount;
	}
}

void sp011_RibbonSetterCleaner(void) {
	u16 mon = Var8004;
	u16 ribbon = Var8005;
	bool8 clear = FALSE;
	
	if (mon >= 6)
		return;
		
	if (ribbon & 0x100) {
		ribbon ^= 0x100;
		clear = TRUE;
	}
	
	if (ribbon > 0x1F)
		return;
		
	(u32*) pointer = (u32*) &gPlayerParty[mon];
	pointer += (76 / sizeof(u32));
	
	if (clear)
		*pointer &= ~(gBitTable[ribbon]);
	else
		*pointer |= gBitTable[ribbon];
}

void sp012_PokerusSetter(void) {
	u16 mon = Var8004;
	u16 timer = Var8005;
	u8 strain;
	
	if (mon >= 6)
		return;
		
	if (timer > 0x10) 
	{
		gPlayerParty[mon].pokerusDays = 0;
		gPlayerParty[mon].pokerusStrain = 1;
	}
	else 
	{
		gPlayerParty[mon].pokerusDays = timer;
		
		//Get appropriate strain depending on the timer
		switch (timer) {
			case 0:
			case 4:
			case 8:
			case 12:
				strain = (1 << 0);
				break;
			case 1:
			case 5:
			case 9:
			case 13:
				strain = (1 << 1);
				break;
			case 2:
			case 6:
			case 10:
			case 14:
				strain = (1 << 2);
				break;
			default:
				strain = (1 << 3);
				break;
		}
		
		gPlayerParty[mon].pokerusStrain = strain;
	}
}

void sp013_IncreaseDecreaseHappiness(void) {
	u16 mon = Var8004;
	u16 amount = Var8005;
	u8* modify;
	s32 calc;

	if (mon >= 6)
		return 0;
		
	if (amount & 0x100) {
		amount ^= 0x100;
		amount = -1 * MathMax(amount, 255);
	}
	
	u8* modify = &gPlayerParty[mon].friendship;
	calc = *modify + amount;
	
	if (calc < 0)
		calc = 0;
	else if (calc > 255)
		calc = 255
		
	*modify = calc;
}

void sp014_ChangeCapturedBall(void) {
	u16 mon = Var8004;
	u16 ball = Var8005;
	
	if (mon >= 6)
		return;
		
	if (ball > NUM_BALLS)
		return;
		
	gPlayerParty[mon].pokeball = ball;
}

void sp015_ModifyHeldItem(void) {
	u16 mon = Var8004;
	u16 item = Var8005;
	
	VarLastResult = 1;
	
	if (mon >= 6)
		return;
	
	//Can't be used if mon already has item
	if (gPlayerParty[mon].item && item)
		return;
	
	gPlayerParty[mon].item = item;

	VarLastResult = 0;
}

void sp016_ChangePokemonSpecies(void) {
	u16 mon = Var8004;
	u16 species = Var8005;

	if (mon >= 6)
		return;

	gPlayerParty[mon].species = species;
}

void sp017_ChangePokemonAttacks(void) {
	u16 mon = Var8004;
	u16 slot = Var8005;
	u16 move = Var8006;
	int i;

	if (mon >= 6)
		return;

	if (move != 0)
	{
		for (int i = 0; i < MAX_MON_MOVES; ++i) {
			if (gPlayerParty[mon].moves[i] == 0) {
				gPlayerParty[mon].moves[i] = move;
				gPlayerParty[mon].pp[i] = gBattleMoves[move].pp;
				return;
			}
		}
	}
	
	gPlayerParty[mon].moves[slot] = move;
	gPlayerParty[mon].pp[slot] = gBattleMoves[move].pp;
	gPlayerParty[mon].ppBonuses &= ~(gBitTable[slot * 2] | gBitTable[slot * 2 + 1])
	
	//Clean Up Moves
	u8 moves[4] = {0};
	u8 pps[4] = {0};
	u8 ppBonuses = 0;
	u8 availableSlot = 0;
	
	for (i = 0; i < MAX_MON_MOVES; ++i) {
		if (gPlayerParty[mon].moves[i] != 0) {
			moves[availableSlot] = gPlayerParty[mon].moves[i];
			pps[availableSlot] = gPlayerParty[mon].pp[i];
			ppBonuses |= (((gBitTable[i * 2] & gPlayerParty[mon].ppBonuses) | (gBitTable[i * 2 + 1] & gPlayerParty[mon].ppBonuses)) << (availableSlot *2));
		}
	}
	
	for (i = 0; i < MAX_MON_MOVES; ++i) {
		gPlayerParty[mon].moves[i] = moves[i];
		gPlayerParty[mon].pp[i] = pps[i];
		gPlayerParty[mon].ppBonuses = ppBonuses;
	}
}

species_t sp018_CheckPokemonSpecies(void) {
	u16 mon = Var8004;

	if (mon >= 6)
		return;

	return gPlayerParty[mon].species;
}

//Add pp bonus calc?
void sp019_CheckAttackPP(void) {
	u16 mon = Var8004;
	u16 slot = Var8005;

	if (mon >= 6)
		return;

	if (slot >= MAX_MON_MOVES)
		return;
	
	return gPlayerParty[mon].pp[slot];
}

//Party Specials//
///////////////////////////////////////////////////////////////////////////////////

void sp062_PokemonEraser(void) {

}

void sp063_StatusChecker(void) {

}

void sp064_InflictStatus(void) {

}

void sp065_CheckMonHP(void) {

}

void sp066_InflictPartyDamage(void) {
	
}

//Key Specials//
///////////////////////////////////////////////////////////////////////////////////

//Special 0x2b checks for the A and B keys. Returns
//0x0 if none pressed
//0x1 if A pressed
//0x2 if B pressed
//0x3 if both are pressed
u16 sp02B_CheckABButtons(void) {
	return ~(gKeyReg) & 3;
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
u16 sp02C_CheckDPad(void) {
	
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
		default:
			return 0;
	}
}

//Special 0x2d checks for the start select buttons
//0x0 if none is pressed
//0x1 if select is pressed
//0x2 if start is pressed
//0x3 if both are pressed
u16 sp02D_CheckStartSelect(void) {
	return (~(gKeyReg) >> 2) & 3;
}

//Special 0x2e checks for the L-R buttons
//0x0 if none is pressed
//0x1 if R is pressed
//0x2 if L is pressed
//0x3 if both are pressed
u16 sp02E_CheckLRButtons(void) {
	return (~(gKeyReg) >> 8) & 3;
}

void sp02F_KeyDump(void) {
	VarLastResult = ~(gKeyReg) & 0x3FF;
}

void sp0C9_ForceOneKeyInput(void) {

}

void sp0CA_IgnoreKeys(void) {

}

void sp0CB_PlaceKeyScript(void) {

}

//Variable Math Specials//
///////////////////////////////////////////////////////////////////////////////////

void sp03E_AddVariables(void) {
	var1 = Var8004; //Var contained in Var8004
	var2 = Var8005; //Var contained in Var8005
	bool8 overflow = FALSE;
	u32 sum;
	
	var1 = VarGet(var1);
	var2 = VarGet(var2);
	
	sum = var1 + var2;
	
	if (sum > 0xFFFF) {
		overflow = TRUE;
		sum = 0xFFFF
	}
	
	VarSet(Var8004, sum); //Set var in Var8004
	return overflow;
}

void sp03F_SubtractVariables(void) {
	var1 = Var8004; //Var contained in Var8004
	var2 = Var8005; //Var contained in Var8005
	bool8 overflow = FALSE;
	u32 diff;
	
	var1 = VarGet(var1);
	var2 = VarGet(var2);
	
	if (var2 > var1) {
		overflow = TRUE;
		diff = 0;
	}
	else
		diff = var1 - var2;
	
	VarSet(Var8004, diff); //Set var in Var8004
	return overflow;
}

void sp040_MultiplyVariables(void) {
	var1 = Var8004; //Var contained in Var8004
	var2 = Var8005; //Var contained in Var8005
	bool8 overflow = FALSE;
	u32 prod;
	
	var1 = VarGet(var1);
	var2 = VarGet(var2);
	
	prod = var1 * var2;
	
	if (prod > 0xFFFF) {
		prod = 0xFFFF;
		overflow = TRUE;
	}
	
	VarSet(Var8004, prod); //Set var in Var8004
	return overflow;
}


void sp041_DivideVariables(void) {
	var1 = Var8004; //Var contained in Var8004
	var2 = Var8005; //Var contained in Var8005
	u32 div;
	
	var1 = VarGet(var1);
	var2 = VarGet(var2);
	
	div = udivsi(var1, var2);
	
	VarSet(Var8004, div); //Set var in Var8004
	return umodsi(var1, var2);
}


void sp042_ANDVariables(void) {
	var1 = Var8004; //Var contained in Var8004
	var2 = Var8005; //Var contained in Var8005
	u32 result;
	
	var1 = VarGet(var1);
	var2 = VarGet(var2);
	
	return var1 & var2;
}

void sp043_ORVariables(void) {
	var1 = Var8004; //Var contained in Var8004
	var2 = Var8005; //Var contained in Var8005
	u32 result;
	
	var1 = VarGet(var1);
	var2 = VarGet(var2);
	
	return var1 | var2;
}

void sp044_XORVariables(void) {
	var1 = Var8004; //Var contained in Var8004
	var2 = Var8005; //Var contained in Var8005
	u32 result;
	
	var1 = VarGet(var1);
	var2 = VarGet(var2);
	
	return var1 ^ var2;
}

//Other Specials//
///////////////////////////////////////////////////////////////////////////////////

void sp024_AddTextByVariable(void) {

}

void sp025_AddTextByPointer(void) {

}

void sp026_TurnMultiBox20to26IntoRamPointed(void) {

}

void sp075_MeasurePokemon1(void) {

}

void sp076_MeasurePokemon2(void) {

}

void sp09C_OldManBattleModifier(void) {

}

void sp18B_DisplayImagesFromTable(void) {

}

//Battle Specials//
///////////////////////////////////////////////////////////////////////////////////

void sp051_WildShinyBattle(void) {

}

void sp052_TemporaryStatusInducer(void) {

}

void sp053_TemporaryStatusCanceller(void) {

}

void sp054_PermanentStatusInducer(void) {

}

void sp055_PermanentStatusCanceller(void) {

}

void sp056_RoamChanger(void) {

}

void sp057_RoamCanceller(void) {

}

void sp058_WildDataSwitch(void) {

}

void sp059_WildDataSwitchCanceller(void) {

}


//Timer Specials//
///////////////////////////////////////////////////////////////////////////////////

//@Details: Starts the timer
void sp046_StartTimer(void) {
	GBATimer[3] = 0x84;
	GBATimer[2] = 0x0;
	GBATimer[1] = 0x83;
	GBATimer[0] = 0x0
}

//@Details: Pauses the timer
void sp047_HaltTimer(void) {
	GBATimer[3] = 0x4;
	GBATimer[1] = 0x3;
}

//@Details: Unpauses the timer
void sp048_ResumeTimer(void) {
	GBATimer[2] = GBATimer[2];
	GBATimer[3] = 0x84;
	GBATimer[1] = 0x83;
}

//@Details:	Stops the timer.
//@Returns: The time on the timer.
u16 sp049_StopTimer(void) {
	u16 time = GBATimer[2]
	GBATimer[3] = 0;
	GBATimer[1] = 0;
	GBATimer[0] = 0;
	GBATimer[2] = 0;
	
	return time;
}

//@Returns: The time on the timer.
u16 sp04A_GetTimerValue(void) {
	return GBATimer[2] = 0;;
}

void sp04B_StopAndUpdatePlaytime(void) {

}

void sp04C_UpdatePlaytime(void) {

}

//@Details: Checks if the timer has reached a value
//			sorted in var 0x8010.
//@Returns: True or False
void sp04D_TimerValueReached(void) {
	u16 time = GBATimer[2];
	
	if (time < Var8010)
		return FALSE;
		
	return TRUE;
}


//@Details: Saves the value in the seconds timer to a 
//			specific memory address.
void sp04E_SaveTimerValue(void) {
	ScriptSpecialsStruct.savedTimer = sp049_StopTimer();
}

//@Details: Starts the timer with the value stored by
//			Special 0x4E.
void sp04F_StartTimerAtTime(void) {
	StartTimer();
	GBATimer[3] = 0;
	GBATimer[2] = ScriptSpecialsStruct.savedTimer;
	GBATimer[3] = 0x84;
}

//@Details: Stores the timer value stored by
//			Special 0x4E.
//@Returns: Var 0x8006 - Timer time.
void sp050_StoreTimerToVariable(void) {
	u16 var = Var8006; //Var in Var8006
	VarSet(var, ScriptSpecialsStruct.savedTimer);
}

//@Details: Loads the value at a given variable and stores
//			it to the saved timer.
//@Input:	Var 0x8006 - Timer value.
void sp061_LoadTimerFromVariable(void) {
	u16 var = Var8006 //Var in Var8006
	var = VarGet(var);
	ScriptSpecialsStruct.savedTimer = var;
}

//Safari Specials//
///////////////////////////////////////////////////////////////////////////////////


//@Details: The check Safari ball quantity. 
//@Returns: 1. Var 0x8004 - Normal Safari Ball number.
//		  2. Var 0x8005 - The extra ball slot number
//		  3. To a given variable the number as a full integer. Max value is 0x63FF
u16 sp086_GetSafariBalls(void) {
	Var8004 = gSafariBalls;
	Var8005 = *(&gSafariBalls + 1);
	return Var8004 + Var8005;
}


//@Details: An increase\decrease safari balls code.
//		  This code doesn't allow people to go over 100 max balls 
//		  as a safety measure.
//@Input: Var 0x8004 as the number to increase or decrease the balls by. 
//	    0x1aa decreases the balls by aa, and 0x0aa increases them by aa. 
//@Returns: None
void sp087_ChangeSafariBalls(void) {
	u16 input = Var8004;
	s32 calc;
	
	if (input & 0x100) {
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
}

//@Details: The get safari pedometer special.
//@Returns: To a given variable the number of remaining steps
void sp088_GetSafariCounter(void) {
	return gSafariSteps;
}

//@Details: The Set Safari steps special. 
//	      It allows you to set a specific ammount of steps 
//		  until the safari handler is called.
//Input: Var 0x8004 is the amount of steps to place.
void sp089_SetSafariCounter(void) {
	u16 input = Var8004;
	gSafariSteps = input;
}

//Walking Specials//
///////////////////////////////////////////////////////////////////////////////////


//@Details: Tile Number getter, gets the tile's number at (0x8004, 0x8005) location. 
//@Input: 1. Var 0x8004 - X-Coordinate
//	    2. Var 0x8005 - Y-Coordinate
//@Returns: Returns to a given variable the desired tile number
void sp07E_GetTileNumber(void) {
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
void sp07F_GetTileBehaviour(void) {
	u16 x = Var8004;
	u16 y = Var8005;
	u32 field = MapGridGetMetatileField(x + 7, y + 7, field);
	
	Var8004 = (field & 0xFF000000) >> 0x18;
	Var8005 = (field & 0xFFFF);
	return Var8004 & 3;
}

void sp097_StartGroundBattle(void) {

}

void sp098_StartWaterBattle(void) {

}

void sp081_SetWalkingScript(void) {

}
*/