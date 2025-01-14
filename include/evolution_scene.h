#ifndef GUARD_EVOLUTION_SCENE_H
#define GUARD_EVOLUTION_SCENE_H

#include "global.h"

void __attribute__((long_call)) CB2_EvolutionSceneLoadGraphics(void);
void __attribute__((long_call)) CB2_BeginEvolutionScene(void);
void __attribute__((long_call)) CB2_EvolutionSceneUpdate(void);
void __attribute__((long_call)) CB2_TradeEvolutionSceneUpdate(void);
void __attribute__((long_call)) BeginEvolutionScene(struct Pokemon* mon, u16 speciesToEvolve, bool8 canStopEvo, u8 partyID);
void __attribute__((long_call)) EvolutionScene(struct Pokemon* mon, u16 speciesToEvolve, bool8 canStopEvo, u8 partyID);
void __attribute__((long_call)) TradeEvolutionScene(struct Pokemon* mon, u16 speciesToEvolve, u8 preEvoSpriteID, u8 partyID);

extern void (*gCB2_AfterEvolution)(void);

#endif // GUARD_EVOLUTION_SCENE_H
