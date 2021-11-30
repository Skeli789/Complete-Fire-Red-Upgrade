#pragma once

#include "../global.h"
#include "ai_scripts.h"

/**
 * \file ai_switching.h
 * \brief Contains functions for determining when the AI should switch out.
 */

//Exported Functions
bool8 ShouldSwitch(struct Pokemon* party, u8 firstId, u8 lastId);
u8 GetMostSuitableMonToSwitchInto(void);
u8 GetMostSuitableMonToSwitchIntoByParty(const struct Pokemon* party);
u8 GetSecondMostSuitableMonToSwitchInto(void);
u8 GetSecondMostSuitableMonToSwitchIntoByParty(const struct Pokemon* party);
s16 GetMostSuitableMonToSwitchIntoScore(void);
u8 GetMostSuitableMonToSwitchIntoFlags(void);
u8 GetSecondMostSuitableMonToSwitchIntoFlags(void);
u8 CalcMostSuitableMonToSwitchInto(void);
void ResetBestMonToSwitchInto(u8 bank);
void RemoveBestMonToSwitchInto(u8 bank);
void ShouldDoAIShiftSwitch(void);

#define OFFENSIVE_STAT_MIN_NUM 3 //Switch when offensive stat is -3 or less

//These numbers were painstakingly calculated using Python - DON'T MODIFY! If you do, you will break something.
#define SWITCHING_INCREASE_KO_FOE 31
#define SWITCHING_INCREASE_RESIST_ALL_MOVES 17
#define SWITCHING_INCREASE_REVENGE_KILL 8 //Can only happen if can KO in the first place
#define SWITCHING_INCREASE_WALLS_FOE 2 //Can only wall if no Super-Effective moves against foe
#define SWITCHING_INCREASE_CAN_2HKO 2
#define SWITCHING_INCREASE_CAN_REMOVE_HAZARDS (SWITCHING_SCORE_MAX + 1)
#define SWITCHING_INCREASE_OUTSPEEDS 14

#define SWITCHING_DECREASE_WEAK_TO_MOVE 1
#define SWITCHING_DECREASE_FAINTS_FROM_FOE 39
#define SWITCHING_DECREASE_FAINTS_FROM_FOE_BUT_OUTSPEEDS 15

#define SWITCHING_SCORE_MAX (SWITCHING_INCREASE_KO_FOE + SWITCHING_INCREASE_RESIST_ALL_MOVES + SWITCHING_INCREASE_REVENGE_KILL + SWITCHING_INCREASE_OUTSPEEDS)

#define SWITCHING_FLAG_KO_FOE                   (1 << 0) //0x1
#define SWITCHING_FLAG_RESIST_ALL_MOVES         (1 << 1) //0x2
#define SWITCHING_FLAG_REVENGE_KILL             (1 << 2) //0x4
#define SWITCHING_FLAG_WALLS_FOE                (1 << 3) //0x8
#define SWITCHING_FLAG_CAN_2HKO                 (1 << 4) //0x10
#define SWITCHING_FLAG_CAN_REMOVE_HAZARDS       (1 << 5) //0x20
#define SWITCHING_FLAG_OUTSPEEDS                (1 << 6) //0x40
#define SWITCHING_FLAG_FAINTS_FROM_FOE          (1 << 7) //0x80
