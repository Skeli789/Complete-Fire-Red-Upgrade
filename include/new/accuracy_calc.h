#pragma once

/**
 * \file accuracy_calc.h
 * \brief Contains functions relating to the calculation of attack accuracies.
 *		  Also contains functions relating to protection moves.
 */

//Exported Functions
bool8 JumpIfMoveAffectedByProtect(move_t, bank_t, bank_t, bool8 actuallyJump);
bool8 ProtectAffects(u16 move, u8 bankAtk, u8 bankDef, bool8 set);
bool8 DoesProtectionMoveBlockMove(u8 bankAtk, u8 bankDef, u16 atkMove, u16 protectMove);
bool8 MissesDueToSemiInvulnerability(u8 bankAtk, u8 bankDef, u16 move);
u32 AccuracyCalc(move_t, bank_t, bank_t);
u32 VisualAccuracyCalc(u16 move, u8 bankAtk, u8 bankDef);
u32 VisualAccuracyCalc_NoTarget(u16 move, u8 bankAtk);
void JumpIfMoveFailed(u8 adder, u16 move);

//Functions Hooked In
void atk01_accuracycheck(void);
