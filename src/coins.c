#include "defines.h"
#include "../include/script.h"
#include "../include/event_data.h"
#include "../include/string_util.h"

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

u32 GetCoins(void)
{
	return gPlayerCoins;
}


void SetCoins(u32 numCoins)
{
	gPlayerCoins = numCoins;
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
	u32 amount;
	u16 arg = ScriptReadHalfword(ctx);
	if (arg == 0xFFFF)
		amount = (Var8000 << 16) + Var8001;
	else
		amount = VarGet(arg);
	
	if (GetCoins() >= amount)
		gSpecialVar_LastResult = TRUE;
	else
		gSpecialVar_LastResult = FALSE;
    return FALSE;	
}


// Add a certain number of coins
bool8 scrB4_AddCoins(struct ScriptContext *ctx)
{
	u32 amount;
	u16 arg = ScriptReadHalfword(ctx);
	if (arg == 0xFFFF)
		amount = (Var8000 << 16) + Var8001;
	else
		amount = VarGet(arg);
	
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
	if (arg == 0xFFFF)
		amount = (Var8000 << 16) + Var8001;
	else
		amount = VarGet(arg);
	
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




