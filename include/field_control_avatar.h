#pragma once

#include "global.h"
#include "global.fieldmap.h"

struct FieldInput
{
	bool8 pressedAButton:1;
	bool8 checkStandardWildEncounter:1;
	bool8 pressedStartButton:1;
	bool8 pressedSelectButton:1;
	bool8 heldDirection:1;
	bool8 heldDirection2:1;
	bool8 tookStep:1;
	bool8 pressedBButton:1;
	bool8 pressedRButton:1;
	bool8 pressedLButton:1;
	bool8 input_field_1_1:1;
	bool8 input_field_1_2:1;
	bool8 input_field_1_3:1;
	u8 dpadDirection;
};

extern struct FieldInput gInputToStoreInQuestLogMaybe;

void __attribute__((long_call)) HandleBoulderFallThroughHole(struct EventObject*);
u8 __attribute__((long_call)) TrySetDiveWarp(void);
void __attribute__((long_call)) GetPlayerPosition(struct MapPosition *);
void __attribute__((long_call)) SetupWarp(struct MapHeader*, s8, struct MapPosition*);
s8 __attribute__((long_call)) GetWarpEventAtMapPosition(struct MapHeader*, struct MapPosition*);
void __attribute__((long_call)) HandleBoulderActivateVictoryRoadSwitch(u16 x, u16 y);
bool8 __attribute__((long_call)) dive_warp(struct MapPosition *position, u16 metatileBehavior);

/*
void FieldClearPlayerInput(struct FieldInput *pStruct);
void FieldGetPlayerInput(struct FieldInput *pStruct, u16 keys, u16 heldKeys);
int ProcessPlayerFieldInput(struct FieldInput *pStruct);
u8 *sub_80682A8(struct MapPosition *, u8, u8);
void overworld_poison_timer_set(void);
void RestartWildEncounterImmunitySteps(void);
u8 *sub_8068E24(struct MapPosition *);
const u8 *GetEventObjectScriptPointerPlayerFacing(void);
bool8 sub_8068870(u16 a);
bool8 sub_8068894(void);
bool8 sub_8068A64(struct MapPosition *, u16);
u8 sub_8068F18(void);
bool8 dive_warp(struct MapPosition *position, u16 b);
int SetCableClubWarp(void);
const u8 *GetInteractedLinkPlayerScript(struct MapPosition *position, u8 metatileBehavior, u8 direction);
u8 *GetCoordEventScriptAtMapPosition(struct MapPosition *position);
*/
