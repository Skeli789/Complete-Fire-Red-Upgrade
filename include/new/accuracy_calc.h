#pragma once

/**
 * \file accuracy_calc.h
 * \brief Contains functions relating to the calculation of attack accuracies.
 *		  Also contains functions relating to protection moves.
 */
 
//Exported Functions
void atk01_accuracycheck(void);
bool8 JumpIfMoveAffectedByProtect(move_t, bank_t, bank_t);
bool8 ProtectAffects(u16 move, u8 bankAtk, u8 bankDef, bool8 set);
 bool8 DoesProtectionMoveBlockMove(u8 bankAtk, u8 bankDef, u16 atkMove, u16 protectMove);
u32 AccuracyCalc(move_t, bank_t, bank_t);
u32 AccuracyCalcNoTarget(u16 move, u8 bankAtk);
void JumpIfMoveFailed(u8 adder, u16 move);

//Functions hooked in
