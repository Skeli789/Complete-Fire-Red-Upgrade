#include "defines.h"
#include "defines_battle.h"
#include "../include/new/helper_functions.h"
#include "../include/new/z_move_effects.h"

extern u8 BattleScript_StatsResetZMove[];
extern u8 BattleScript_AllStatsUpZMove[];
extern u8 BattleScript_BoostCritsZMove[];
extern u8 BattleScript_FollowMeZMove[];
extern u8 BattleScript_RecoverHPZMove[];
extern u8 BattleScript_StatUpZMove[];
extern u8 BattleScript_SetUpHealReplacementZMove[];

extern u8 ChangeStatBuffs(s8 statValue, u8 statId, u8 flags, u8* BS_ptr);

void SetZEffect(void) {
	int i;
	
	gNewBS->ZMoveData->runningZEffect = TRUE;

	if (gNewBS->ZMoveData->effect == Z_EFFECT_CURSE) {
		if (IsOfType(gBankAttacker, TYPE_GHOST))
			gNewBS->ZMoveData->effect = Z_EFFECT_RECOVER_HP;
		else
			gNewBS->ZMoveData->effect = Z_EFFECT_ATK_UP_1;
	}
	
	gBattleScripting->bank = gBankAttacker;
	
	switch (gNewBS->ZMoveData->effect) {
		case Z_EFFECT_RESET_STATS:
			for (i = 0; i < BATTLE_STATS_NO-1; ++i) {
				if (gBattleMons[gBankAttacker].statStages[i] < 6) {
					gBattleMons[gBankAttacker].statStages[i] = 6;
				}
			}
			BattleScriptPush(gBattlescriptCurrInstr + 5);
			gBattlescriptCurrInstr = BattleScript_StatsResetZMove - 5;
			break;
		
		case Z_EFFECT_ALL_STATS_UP_1:
			if (!StatsMaxed(gBankAttacker)) {
				for (i = 0; i < BATTLE_STATS_NO-1; ++i) {
					if (gBattleMons[gBankAttacker].statStages[i] < 12)
					++gBattleMons[gBankAttacker].statStages[i];
				}
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_AllStatsUpZMove - 5;
			}
			break;
			
		case Z_EFFECT_BOOST_CRITS:
			if (!(gBattleMons[gBankAttacker].status2 & STATUS2_FOCUS_ENERGY)) {
				gBattleMons[gBankAttacker].status2 |= STATUS2_FOCUS_ENERGY;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_BoostCritsZMove - 5;
			}
			break;
			
		case Z_EFFECT_FOLLOW_ME:
			gSideTimers[SIDE(gBankAttacker)].followmeTimer = 1;
			gSideTimers[SIDE(gBankAttacker)].followmeTarget = gBankAttacker;
			BattleScriptPush(gBattlescriptCurrInstr + 5);
            gBattlescriptCurrInstr = BattleScript_FollowMeZMove - 5;
			break;
			
		case Z_EFFECT_RECOVER_HP:
			if (gBattleMons[gBankAttacker].hp != gBattleMons[gBankAttacker].maxHP) {
				gBattleMoveDamage = (-1) * gBattleMons[gBankAttacker].maxHP;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_RecoverHPZMove - 5;
			}
			break;
			
		case Z_EFFECT_RESTORE_REPLACEMENT_HP:
			gNewBS->ZMoveData->healReplacement = TRUE;
			BattleScriptPush(gBattlescriptCurrInstr + 5);
            gBattlescriptCurrInstr = BattleScript_SetUpHealReplacementZMove - 5;
			break;
		
		case Z_EFFECT_ATK_UP_1:
		case Z_EFFECT_DEF_UP_1:
		case Z_EFFECT_SPD_UP_1:
		case Z_EFFECT_SPATK_UP_1:
		case Z_EFFECT_SPDEF_UP_1:
		case Z_EFFECT_ACC_UP_1:
		case Z_EFFECT_EVSN_UP_1:
			if (!ChangeStatBuffs(SET_STAT_BUFF_VALUE(1), gNewBS->ZMoveData->effect - Z_EFFECT_ATK_UP_1 + 1, MOVE_EFFECT_AFFECTS_USER, 0)) {
				gBattleScripting->animArg1 = 0xE + (gNewBS->ZMoveData->effect - Z_EFFECT_ATK_UP_1 + 1);
				gBattleScripting->animArg2 = 0;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_StatUpZMove - 5;
			}
			break;
			
		case Z_EFFECT_ATK_UP_2:
		case Z_EFFECT_DEF_UP_2:
		case Z_EFFECT_SPD_UP_2:
		case Z_EFFECT_SPATK_UP_2:
		case Z_EFFECT_SPDEF_UP_2:
		case Z_EFFECT_ACC_UP_2:
		case Z_EFFECT_EVSN_UP_2:
			if (!ChangeStatBuffs(SET_STAT_BUFF_VALUE(2), gNewBS->ZMoveData->effect - Z_EFFECT_ATK_UP_2 + 1, MOVE_EFFECT_AFFECTS_USER, 0)) {
				gBattleScripting->animArg1 = 0xE + (gNewBS->ZMoveData->effect - Z_EFFECT_ATK_UP_2 + 1);
				gBattleScripting->animArg2 = 0;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_StatUpZMove - 5;
			}
			break;
			
		case Z_EFFECT_ATK_UP_3:
		case Z_EFFECT_DEF_UP_3:
		case Z_EFFECT_SPD_UP_3:
		case Z_EFFECT_SPATK_UP_3:
		case Z_EFFECT_SPDEF_UP_3:
		case Z_EFFECT_ACC_UP_3:
		case Z_EFFECT_EVSN_UP_3:
			if (!ChangeStatBuffs(SET_STAT_BUFF_VALUE(3), gNewBS->ZMoveData->effect - Z_EFFECT_ATK_UP_3 + 1, MOVE_EFFECT_AFFECTS_USER, 0)) {
				gBattleScripting->animArg1 = 0xE + (gNewBS->ZMoveData->effect - Z_EFFECT_ATK_UP_3 + 1);
				gBattleScripting->animArg2 = 0;
				BattleScriptPush(gBattlescriptCurrInstr + 5);
				gBattlescriptCurrInstr = BattleScript_StatUpZMove - 5;
			}
			break;
	}
	gNewBS->ZMoveData->runningZEffect = FALSE;
}
