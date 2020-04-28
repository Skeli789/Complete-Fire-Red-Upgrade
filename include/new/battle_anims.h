#pragma once

#include "../global.h"

/**
 * \file battle_anims.h
 * \brief Contains functions relating to various in-battle animations. Contains
 *		  animation tasks and sprite callbacks for attack animations, as well as
 *		  the code responsible for hiding the healthboxes during animations.
 */

//Exported Functions
void AnimTask_TechnoBlast(u8 taskId);
void AnimTask_GetTimeOfDay(u8 taskId);
void AnimTask_GetLycanrocForm(u8 taskId);
void AnimTask_IsTargetPartner(u8 taskId);
void AnimTask_GetTrappedMoveAnimId(u8 taskId);
void AnimTask_ReloadAttackerSprite(u8 taskId);
void AnimTask_PlayAttackerCry(u8 taskId);
void AnimTask_GetSecretPowerAnimation(u8 taskId);
void AnimTask_SetCamouflageBlend(u8 taskId);
void AnimTask_CreateFlingItem(u8 taskId);
void AnimTask_CreateStealItem(u8 taskId);
void AnimTask_GrowTarget(u8 taskId);
void AnimTask_AllBanksInvisible(u8 taskId);
void AnimTask_AllBanksVisible(u8 taskId);
void AnimTask_AllBanksInvisibleExceptAttackerAndTarget(u8 taskId);

void SpriteCB_TranslateAnimSpriteToTargetMonLocationDoubles(struct Sprite* sprite);
void SpriteCB_SpriteToCentreOfSide(struct Sprite* sprite);
void SpriteCB_RandomCentredHits(struct Sprite* sprite);
void SpriteCB_CentredElectricity(struct Sprite* sprite);
void SpriteCB_SearingShotRock(struct Sprite* sprite);
void SpriteCB_CoreEnforcerHits(struct Sprite* sprite);
void SpriteCB_CoreEnforcerBeam(struct Sprite* sprite);
void CoreEnforcerLoadBeamTarget(struct Sprite* sprite);
void SpriteCB_FlareBlitzUpFlames(struct Sprite* sprite);
void SpriteCB_SunsteelStrikeRings(struct Sprite* sprite);
void SpriteCB_MindBlownBall(struct Sprite *sprite);
void SpriteCB_MindBlownExplosion(struct Sprite* sprite);
void SpriteCB_HydroCannonImpact(struct Sprite *sprite);
void SpriteCB_SoulStealingStar(struct Sprite *sprite);
void SpriteCB_GrowingSuperpower(struct Sprite *sprite);
void SpriteCB_DracoMeteorRock(struct Sprite *sprite);

bool8 IsAnimMoveIonDeluge(void);
bool8 DoesAnimMoveGetMaxRollout(void);
bool8 IsAnimMoveBloomDoom(void);
bool8 IsAnimMoveOceanicOperretta(void);
bool8 IsMoveNeverEndingNightmareOrDevastatingDrakeOrLightThatBurnsTheSky(void);
bool8 IsAnimMoveDestinyBond(void);
bool8 IsAnimMoveThunderWave(void);
bool8 IsAnimMoveGrudge(void);
bool8 IsAnimMoveFairyLock(void);
bool8 IsAnimMoveFlashCannon(void);
bool8 IsAnimMoveSkillSwap(void);
bool8 IsAnimMovePowerSwap(void);
bool8 IsAnimMoveHeartSwap(void);
bool8 IsAnimMoveMudBomb(void);
bool8 IsAnimMoveCoreEnforcer(void);
bool8 IsAnimMoveBulletSeed(void);
bool8 IsAnimMoveKingsShield(void);

//Functions hooked in
bank_t LoadBattleAnimTarget(u8 arg);
void ScriptCmd_loadspritegfx(void);
bool8 ShadowSneakAnimHelper(void);
void UpdatedAnimStealItemFinalCallback(struct Sprite* sprite);
void UpdateOamPriorityInAllHealthboxes(u8 priority);
bool8 TryHandleLaunchBattleTableAnimation(u8 activeBattler, u8 bankAtk, u8 bankDef, u8 tableId, u16 argument);
