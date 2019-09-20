#include "defines.h"
#include "defines_battle.h"

#include "../include/new/battle_util.h"
#include "../include/new/Helper_Functions.h"

#define IS_BATTLE_CIRCUS (gBattleTypeFlags & BATTLE_TYPE_BATTLE_CIRCUS)

bool8 IsTrickRoomActive(void)
{
	return gNewBS->TrickRoomTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TRICK_ROOM);
}

bool8 IsMagicRoomActive(void)
{
	return gNewBS->MagicRoomTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_MAGIC_ROOM);
}

bool8 IsWonderRoomActive(void)
{
	return gNewBS->WonderRoomTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_WONDER_ROOM);
}

bool8 IsGravityActive(void)
{
	return gNewBS->GravityTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_GRAVITY);
}

bool8 IsIonDelugeActive(void)
{
	return gNewBS->IonDelugeTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_ION_DELUGE);
}

bool8 IsFairyLockActive(void)
{
	return gNewBS->FairyLockTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_FAIRY_LOCK);
}

bool8 IsMudSportActive(void)
{
	return gNewBS->MudSportTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_MUD_SPORT);
}

bool8 IsWaterSportActive(void)
{
	return gNewBS->WaterSportTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_WATER_SPORT);
}

bool8 IsInverseBattle(void)
{
	return FlagGet(INVERSE_FLAG)
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_INVERSE);
}

bool8 BankSideHasSeaOfFire(u8 bank)
{
	return gNewBS->SeaOfFireTimers[SIDE(bank)]
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_SEA_OF_FIRE);
}

bool8 BankSideHasRainbow(u8 bank)
{
	return gNewBS->RainbowTimers[SIDE(bank)]
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_RAINBOW);
}

bool8 BankSideHasSwamp(u8 bank)
{
	return SideHasSwamp(SIDE(bank));
}

bool8 SideHasSwamp(u8 side)
{
	return gNewBS->SwampTimers[side]
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_SWAMP);
}

bool8 IsConfused(u8 bank)
{
	return (gBattleMons[bank].status2 & STATUS2_CONFUSION) != 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_CONFUSED && ABILITY(bank) != ABILITY_OWNTEMPO);
}

bool8 IsTaunted(u8 bank)
{
	return gDisableStructs[bank].tauntTimer > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TAUNT && ABILITY(bank) != ABILITY_OBLIVIOUS);
}

bool8 IsTormented(u8 bank)
{
	return (gBattleMons[bank].status2 & STATUS2_TORMENT) != 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_TORMENT);
}

bool8 IsHealBlocked(u8 bank)
{
	return gNewBS->HealBlockTimers[bank] > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_HEAL_BLOCK);
}

bool8 CantUseSoundMoves(u8 bank)
{
	return gNewBS->ThroatChopTimers[bank] > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_THROAT_CHOP);
}

bool8 IsLaserFocused(u8 bank)
{
	return gNewBS->LaserFocusTimers[bank] > 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_ALWAYS_CRIT);
}

bool8 IsAbilitySuppressed(u8 bank)
{
	return (gStatuses3[bank] & STATUS3_ABILITY_SUPPRESS) != 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_ABILITY_SUPPRESSION);
}

bool8 CantScoreACrit(u8 bank, struct Pokemon* mon)
{
	if (mon != NULL)
		return FALSE;

	return (gStatuses3[bank] & STATUS3_CANT_SCORE_A_CRIT) != 0
		|| (IS_BATTLE_CIRCUS && gBattleCircusFlags & BATTLE_CIRCUS_NO_CRITS);
}
