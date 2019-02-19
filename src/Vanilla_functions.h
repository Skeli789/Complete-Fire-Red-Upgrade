#pragma once
#include "defines.h"

#define BC_Menu 0x8014040 | 1

u32 __attribute__((long_call)) get_attr(pokemon_t* poke_address, u8 request, void* dst);
void __attribute__((long_call)) set_attr(pokemon_t* poke_address, u8 request, void* new_value);
void __attribute__((long_call)) calculate_stats_pokemon(pokemon_t* poke);
u16 __attribute__((long_call)) teach_move_in_available_slot(pokemon_t* poke, u16 move);
u16 __attribute__((long_call)) bag_remove_item(u16 itemID, u16 quantity);

/* doing for held item form change */
u8 __attribute__((long_call)) get_poke_ability(pokemon_t* poke);
u8 __attribute__((long_call)) get_item_x12_battle_function(u16 item_id);
u32 __attribute__((long_call)) get_item_lastword(u16 itemID);
u8 __attribute__((long_call)) get_item_quality(u16 itemID);
u8 __attribute__((long_call)) item_is_mail(u16 itemID);
u8 __attribute__((long_call)) prepare_party_mail_to_give(pokemon_t* poke, u16 item);
bool8 __attribute__((long_call)) bag_add_item(u16 item_id, u8 a);
// ends here //


// used for form change //
void __attribute__((long_call)) run_after_graphics();
void __attribute__((long_call)) script_env_2_enable(int taskID);
void __attribute__((long_call)) script_run(void *scriptLoc);
void __attribute__((long_call)) task_delete(u8 taskID);
void* __attribute__((long_call)) memcpy_(void *dst, void *src, u8 size);
void* __attribute__((long_call)) memset_(void *dst, u8 pattern, u8 size);

void* __attribute__((long_call)) Memcpy(void *dst, void *src, u32 size);
void* __attribute__((long_call)) Memset(void *dst, u8 pattern, u32 size);

void __attribute__((long_call)) pokemon_slot_purge(pokemon_t*);
void __attribute__((long_call)) Special_DD_delete_move();
u8 __attribute__((long_call)) party_move_up_no_free_slots_in_between();

//PokeFireRed Function Names

//Battle Functions
u8*  __attribute__((long_call)) StringGetEnd10(u8 *str);
u8*  __attribute__((long_call)) GetExpandedPlaceholder(u32 id);
u8   __attribute__((long_call)) GetMultiplayerId(void);
void __attribute__((long_call)) PrepareBufferDataTransfer(u8 a, u8 *data, u16 size);
void __attribute__((long_call)) PrepareBufferDataTransferLink(u8 a, u16 size, u8 *data);
void __attribute__((long_call)) BtlController_EmitSetMonData(u8 bufferId, u8 requestId, u8 monToCheck, u8 bytes, void *data);
void __attribute__((long_call)) EmitTrainerThrow(u8);
void __attribute__((long_call)) EmitTrainerSlide(u8 a);
void __attribute__((long_call)) EmitTrainerSlideBack(u8 a);
void __attribute__((long_call)) EmitBallThrow(u8 a, u8 shakes);
void __attribute__((long_call)) EmitMoveAnimation(u8 bufferId, u16 move, u8 turnOfMove, u16 movePower, s32 dmg, u8 friendship, struct DisableStruct *disableStructPtr, u8 multihit);
void __attribute__((long_call)) EmitPrintString(u8 a, u16 stringID);
void __attribute__((long_call)) EmitHealthBarUpdate(u8 a, u16 b);
//void __attribute__((long_call)) EmitExpBarUpdate(u8 a, u8 b, u16 c);
void __attribute__((long_call)) EmitStatusAnimation(u8 a, u8 b, u32 c);
void __attribute__((long_call)) EmitTwoReturnValues(u8 a, u8 b, u16 c);
void __attribute__((long_call)) EmitChosenMonReturnValue(u8 a, u8 b, u8* c);
void  __attribute__((long_call)) EmitTrainerBallThrow(u8 a);
void __attribute__((long_call)) EmitSpriteInvisibility(u8 bufferId, bool8 isInvisible);
void __attribute__((long_call)) EmitDrawPartyStatusSummary(u8 bufferId, struct HpAndStatus* hpAndStatus, u8 arg2);
void __attribute__((long_call)) EmitBattleAnimation(u8 a, u8 b, u16 c);
u8 __attribute__((long_call)) LoadBattleBG_SpecialTerrainID(u8 mapType);
void __attribute__((long_call)) sub_8012098(struct Sprite *sprite);
void __attribute__((long_call)) DoBounceEffect(u8 battlerId, u8 b, s8 c, s8 d);
void __attribute__((long_call)) EndBounceEffect(u8 battlerId, bool8 b);
void __attribute__((long_call)) sub_8012044(struct Sprite *sprite);
void __attribute__((long_call)) SwitchInClearSetData(void);
void __attribute__((long_call)) UndoEffectsAfterFainting(void);
void __attribute__((long_call))  BattleIntroRecordMonsToDex(void);
void __attribute__((long_call)) sub_8013F6C(u8 bank);
void __attribute__((long_call)) SwapTurnOrder(u8 a, u8 b);
void __attribute__((long_call)) TurnValuesCleanUp(u8);
void __attribute__((long_call)) SpecialStatusesClear(void);
void __attribute__((long_call)) CheckFocusPunch_ClearVarsBeforeTurnStarts(void);
void __attribute__((long_call)) HandleEndTurn_FinishBattle(void);
bank_t __attribute__((long_call)) GetBattleBank(u8 caseID);
void __attribute__((long_call)) PressurePPLose(u8 defender, u8 attacker, u16 move);
void __attribute__((long_call)) PressurePPLoseOnUsingImprision(u8 bankAtk);
void __attribute__((long_call)) MarkBufferBankForExecution(u8 bank);
void __attribute__((long_call)) CancelMultiTurnMoves(u8 bank);
bool8 __attribute__((long_call)) WasUnableToUseMove(u8 bank);
void __attribute__((long_call)) ResetSentPokesToOpponentValue(void);
void __attribute__((long_call)) sub_8017434(u8 bank);
void __attribute__((long_call)) sub_80174B8(bank_t);
void __attribute__((long_call)) BattleScriptPush(const u8* BS_ptr);
void __attribute__((long_call)) BattleScriptPushCursor(void);
void __attribute__((long_call)) BattleScriptPop(void);
u8 __attribute__((long_call)) IsImprisoned(u8 bank, u16 move);
//bool8 __attribute__((long_call)) HasNoMonsToSwitch(u8 battler, u8 partyIdBattlerOn1, u8 partyIdBattlerOn2);
u8 __attribute__((long_call)) AbilityBattleEffects(u8 caseID, u8 bank, u8 ability, u8 special, u16 move);
void __attribute__((long_call)) BattleScriptExecute(const u8* BS_ptr);
void __attribute__((long_call)) BattleScriptPushCursorAndCallback(const u8* BS_ptr);
//u8 __attribute__((long_call)) ItemBattleEffects(u8 caseID, u8 bank, bool8 moveTurn);
void __attribute__((long_call)) JumpIfMoveFailed(u8 advance_bytes, move_t move);
void __attribute__((long_call)) CheckWonderGuardAndLevitate(void);
void __attribute__((long_call)) SetMoveEffect(bool8 primary, u8 certain);
void __attribute__((long_call)) MoveValuesCleanUp(void);
bool8 __attribute__((long_call)) IsTwoTurnsMove(u16 move);
u8  __attribute__((long_call)) AttacksThisTurn(u8 bank, u16 move);
void __attribute__((long_call)) SetControllerToPlayer(void);
void __attribute__((long_call)) PlayerBufferExecCompleted(void);
//void __attribute__((long_call)) HandleInputChooseTarget(void);
void __attribute__((long_call)) HandleMoveSwitching(void);
void __attribute__((long_call)) sub_802F730(void);
void __attribute__((long_call)) sub_802F768(void);
void __attribute__((long_call)) CompleteOnInactiveTextPrinter(void);
void __attribute__((long_call)) Task_LaunchLvlUpAnim(u8);
void __attribute__((long_call)) DestroyExpTaskAndCompleteOnInactiveTextPrinter(u8);
void __attribute__((long_call)) MoveSelectionDisplayMoveNames(void);
void __attribute__((long_call)) MoveSelectionDisplayPpNumber(void);
/*void __attribute__((long_call)) MoveSelectionDisplayMoveType(void);*/
void __attribute__((long_call)) MoveSelectionCreateCursorAt(u8 cursorPos, u8 arg1);
void __attribute__((long_call)) MoveSelectionDestroyCursorAt(u8 cursorPos);
void __attribute__((long_call)) sub_8033AC8(void);
void __attribute__((long_call)) sub_8033EEC(struct Sprite *sprite);
void __attribute__((long_call)) DecompressTrainerFrontPic(u16 frontPicId, u8 battlerId);
void __attribute__((long_call)) DecompressTrainerBackPic(u16 backPicId, u8 battlerId);
void __attribute__((long_call)) load_gfxc_health_bar(u8);
void __attribute__((long_call)) HandleLowHpMusicChange(struct Pokemon *mon, u8 battlerId);
void __attribute__((long_call)) BattleStopLowHpSound(void);
void __attribute__((long_call)) SetControllerToOpponent(void);
void __attribute__((long_call)) CompleteOnBattlerSpriteCallbackDummy(void);
void __attribute__((long_call)) CompleteOnBankSpriteCallbackDummy2(void);
void __attribute__((long_call)) sub_8035BE8(void);
void __attribute__((long_call)) OpponentBufferExecCompleted(void);
bool8 __attribute__((long_call)) ShouldSwitchIfPerishSong(void);
bool8 __attribute__((long_call)) AreStatsRaised(void);
//void __attribute__((long_call)) AI_TrySwitchOrUseItem(void);
void __attribute__((long_call)) ModulateByTypeEffectiveness(u8 atkType, u8 defType1, u8 defType2, u8 *var);
bool8 __attribute__((long_call)) ShouldUseItem(void);
u8 __attribute__((long_call)) CountAliveMons(u8 caseId);
u8 __attribute__((long_call)) GetDefaultMoveTarget(u8 battlerId);
void __attribute__((long_call)) SetMultiuseSpriteTemplateToTrainerBack(u16 trainerSpriteId, u8 battlerPosition);
u8 __attribute__((long_call)) GetSecretBaseTrainerPicIndex(void);
u8 __attribute__((long_call)) GetSecretBaseTrainerNameIndex(void);
void __attribute__((long_call)) AdjustFriendship(struct Pokemon *mon, u8 event);
u8 __attribute__((long_call)) sub_80447F0(void); //Something Trainer Related
u8 __attribute__((long_call)) GetLinkTrainerFlankId(u8 a);
u8 __attribute__((long_call)) GetBattlerMultiplayerId(u8 bank);
void __attribute__((long_call)) SetBattleBarStruct(u8, u8, u32, u32, u32);
void __attribute__((long_call)) SetHealthboxSpriteVisible(u8 healthboxSpriteId);
void __attribute__((long_call)) UpdateHealthboxAttribute(u8 healthboxSpriteId, struct Pokemon* mon, u8 elementId);
s32 __attribute__((long_call)) MoveBattleBar(u8 battler, u8 healthboxSpriteId, u8 whichBar, u8 arg3);
void __attribute__((long_call)) SpriteCB_ReleaseMonFromBall(struct Sprite* sprite);
bool8 __attribute__((long_call)) IsCryPlayingOrClearCrySongs(void);
void __attribute__((long_call)) DestroyAnimSprite(struct Sprite *sprite);
void __attribute__((long_call)) StoreSpriteCallbackInData6(struct Sprite *sprite, void (*callback)(struct Sprite*));
void __attribute__((long_call)) sub_8074E14(struct Sprite *sprite);
u8 __attribute__((long_call)) GetBattlerSide(u8 bank);
u8 __attribute__((long_call)) GetBattlerPosition(u8 slot);
u8 __attribute__((long_call)) GetBattlerAtPosition(u8);
bool8 __attribute__((long_call)) IsDoubleBattle(void);
void __attribute__((long_call)) InitSpriteDataForLinearTranslation(struct Sprite *sprite);
u8 __attribute__((long_call)) GetBattlerSpriteSubpriority(u8 bank);
void __attribute__((long_call)) StartTheBattle(void);
u8* __attribute__((long_call)) GetTrainerALoseText(void);
u8* __attribute__((long_call)) GetTrainerAWinText(void);
void __attribute__((long_call)) BattleIntroSlide2(u8 taskId);
void __attribute__((long_call)) BattleIntroSlide3(u8 taskId);
void __attribute__((long_call)) BattleIntroSlideLink(u8 taskId);
//void __attribute__((long_call)) BattleAI_SetupAIData(u8 defaultScoreMoves);
u8 __attribute__((long_call)) BattleAI_ChooseMoveOrAction(void);
void __attribute__((long_call)) RecordAbilityBattle(u8 bank, u8 ability);
void __attribute__((long_call)) RecordItemBattle(u8 bank, u8 item_effect);
void __attribute__((long_call)) StrCpyDecodeToDisplayedStringBattle(const u8* textPtr);
u8* __attribute__((long_call)) TryGetStatusString(u8 *src);
void __attribute__((long_call)) ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst);
void __attribute__((long_call)) ChooseMoveUsedParticle(u8* textPtr);
void  __attribute__((long_call)) ChooseTypeOfMoveUsedString(u8* dst);
void __attribute__((long_call)) BattlePutTextOnWindow(const u8* text, u8 arg1);
void __attribute__((long_call)) SetPpNumbersPaletteInMoveSelection(void);
//u8 __attribute__((long_call)) GetBattleTowerTrainerPicIndex(void); //Ruby Leftover
//u8 __attribute__((long_call)) GetBattleTowerTrainerClassId(void); //Ruby Leftover
//void __attribute__((long_call)) GetBattleTowerTrainerName(u8* dst); //Ruby Leftover
u8 __attribute__((long_call)) GetEreaderTrainerFrontSpriteId(void);
u8 __attribute__((long_call)) GetEreaderTrainerClassId(void);
void __attribute__((long_call)) GetEreaderTrainerName(u8* dst);
bool8 __attribute__((long_call)) sub_80EB2E0(u32 a);
//bool8 __attribute__((long_call)) IsLinkDoubleBattle(void);
void __attribute__((long_call)) sub_8127EC4(u8 bank, u8 partyID, u8 r2);
void __attribute__((long_call)) sub_8127FF4(u8 slot, u8 slot2);
u8 __attribute__((long_call)) GetTrainerTowerTrainerClassId(void);
void __attribute__((long_call)) GetTrainerTowerTrainerName(u8* dst);
u8 __attribute__((long_call)) GetTrainerTowerTrainerPicIndex(void);
void __attribute__((long_call)) CopyTrainerTowerPlayerLostText(u8* dst, u8 varset);
void __attribute__((long_call)) CopyTrainerTowerPlayerWonText(u8* dst, u8 varset);

//Poke Ball Functions
void __attribute__((long_call)) SpriteCB_TestBallThrow(struct Sprite *sprite);
void __attribute__((long_call)) PokeBallOpenParticleAnimation(u8 taskId);
void __attribute__((long_call)) GreatBallOpenParticleAnimation(u8 taskId);
void __attribute__((long_call)) SafariBallOpenParticleAnimation(u8 taskId);
void __attribute__((long_call)) UltraBallOpenParticleAnimation(u8 taskId);
void __attribute__((long_call)) MasterBallOpenParticleAnimation(u8 taskId);
void __attribute__((long_call)) DiveBallOpenParticleAnimation(u8 taskId);
void __attribute__((long_call)) RepeatBallOpenParticleAnimation(u8 taskId);
void __attribute__((long_call)) TimerBallOpenParticleAnimation(u8 taskId);
void __attribute__((long_call)) PremierBallOpenParticleAnimation(u8 taskId);

//Pokemon Functions
void __attribute__((long_call)) EmitSetMonData(u8 a, u8 request, u8 c, u8 bytes, void *data);
void __attribute__((long_call)) ZeroPlayerPartyMons(void);
void __attribute__((long_call)) ZeroEnemyPartyMons(void);
void __attribute__((long_call)) CreateMon(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId);
void __attribute__((long_call)) CreateBoxMon(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV, u8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType, u32 fixedOtId);
void __attribute__((long_call)) CalculateMonStats(pokemon_t*);
u8 __attribute__((long_call)) GetMonGender(pokemon_t* mon);
u8 __attribute__((long_call)) GetBoxMonGender(struct BoxPokemon* boxMon);
u8 __attribute__((long_call)) GetGenderFromSpeciesAndPersonality(u16 species, u32 personality);
u32 __attribute__((long_call)) GetMonData(struct Pokemon *, s32, const void *data);
void __attribute__((long_call)) SetMonData(struct Pokemon *mon, s32 field, const void *data);
void __attribute__((long_call)) CopyMon(void *dest, void *src, size_t size);
u8 __attribute__((long_call)) SendMonToPC(struct Pokemon* mon);
u8 __attribute__((long_call)) GetPartyAbility(pokemon_t*);
u8 __attribute__((long_call)) CalculatePlayerPartyCount(void);
u8 __attribute__((long_call)) CalculatePPWithBonus(u16 move, u8 ppBonuses, u8 moveIndex);
u16 __attribute__((long_call)) SpeciesToNationalPokedexNum(u16 species);
void __attribute__((long_call)) MonGainEVs(struct Pokemon *mon, u16 defeatedSpecies);
s8 __attribute__((long_call)) GetPokeFlavourRelation(u32 pid, u8 flavor);
bool8 __attribute__((long_call)) IsTradedMon(struct Pokemon *mon);
bool8 __attribute__((long_call)) IsOtherTrainer(u32 otId, u8* otName);
void __attribute__((long_call)) MonRestorePP(struct Pokemon *mon);
bool8 __attribute__((long_call)) GetSetPokedexFlag(u16 nationalNum, u8 caseID);
u16 __attribute__((long_call)) GetPokedexHeightWeight(u16 dexNum, u8 data);
void __attribute__((long_call)) ReducePartyToThree(void);
u8 __attribute__((long_call)) pokemon_order_func(u8 a);

//Item Functions
bool8 __attribute__((long_call)) IsMail(u16 item_id);
void __attribute__((long_call)) CopyItemName(u16 itemId, u8* dst);
bool8 __attribute__((long_call)) CheckBagHasItem(u16 itemId, u16 count);
u16 __attribute__((long_call)) SanitizeItemId(u16 item_id);
u8 __attribute__((long_call)) ItemId_GetHoldEffect(u16 item_id);
u8 __attribute__((long_call)) ItemId_GetHoldEffectParam(u16 item_id);
u8 __attribute__((long_call)) ItemId_GetMystery2(u16 item_id);
u8 __attribute__((long_call)) GetPocketByItemId(u16 item_id);
u8 __attribute__((long_call)) ItemId_GetType(u16 item_id);
u8 __attribute__((long_call)) FlagSet(u16 id);
u8 __attribute__((long_call)) FlagClear(u16 id);
bool8 __attribute__((long_call)) FlagGet(u16 id);
void __attribute__((long_call)) AddMoney(u32* moneySaveblock, u32 to_give);

/*Overworld Functions*/
u8 __attribute__((long_call)) GetTrainerEncounterMusicId(u16 trainerOpponentId);
u8 __attribute__((long_call)) Overworld_GetMapTypeOfSaveblockLocation(void);
u8 __attribute__((long_call)) GetCurrentMapBattleScene(void);
u32 __attribute__((long_call)) MapGridGetMetatileBehaviorAt(int, int);
bool8 __attribute__((long_call)) MetatileBehavior_IsSandOrDeepSand(u8 metatileBehavior);
bool8 __attribute__((long_call)) MetatileBehavior_IsSurfableWaterOrUnderwater(u8 metatileBehavior);
bool8 __attribute__((long_call)) MetatileBehavior_IsTallGrass(u8 metatileBehavior);
bool8 __attribute__((long_call)) MetatileBehavior_IsLongGrass(u8 metatileBehavior);
bool8 __attribute__((long_call)) MetatileBehavior_IsIndoorEncounter(u8 metatileBehavior);
bool8 __attribute__((long_call)) MetatileBehavior_IsMountain(u8 metatileBehavior);
bool8 __attribute__((long_call)) MetatileBehavior_IsDeepOrOceanWater(u8 metatileBehavior);
void __attribute__((long_call)) PlayerGetDestCoords(s16* x, s16* y);
u8 __attribute__((long_call)) TestPlayerAvatarFlags(u8 a);
u8* __attribute__((long_call)) GetEventObjectScriptPointerByEventObjectId(u8);
u8 __attribute__((long_call)) GetRunningDirectionAnimNum(u8 direction);
void __attribute__((long_call)) npc_apply_anim_looping(struct MapObject *eventObject, struct Sprite *sprite, u8 animNum);
void __attribute__((long_call)) npc_apply_direction(struct MapObject *eventObject, struct Sprite *sprite, u8 direction, u8 speed);
bool8 __attribute__((long_call)) npc_obj_ministep_stop_on_arrival(struct MapObject *eventObject, struct Sprite *sprite);
void __attribute__((long_call)) ScriptContext1_SetupScript(u8 *ptr);
void __attribute__((long_call)) ScriptContext2_Enable(void);
void __attribute__((long_call)) ScriptContext1_Stop(void);
void __attribute__((long_call)) PlayNewMapMusic(u16 musicId);

//void __attribute__((long_call)) InitTrainerBattleVariables(void);
void __attribute__((long_call)) TrainerBattleLoadArgs(const struct TrainerBattleParameter* specs, const u8 *data);
u16 __attribute__((long_call)) TrainerBattleLoadArg16(const u8 *ptr);
u8 __attribute__((long_call)) TrainerBattleLoadArg8(const u8 *ptr);
void __attribute__((long_call)) SetMapVarsToTrainer(void);
//u8* __attribute__((long_call)) BattleSetup_ConfigureTrainerBattle(u8 *data);
void __attribute__((long_call)) TrainerWantsBattle(u8 trainerEventObjId, const u8 *trainerScript);
//u32 __attribute__((long_call)) GetTrainerFlagFromScriptPointer(const u8 *data);
void __attribute__((long_call)) CB2_EndTrainerBattle(void);
bool8 __attribute__((long_call)) TrainerCanApproachPlayer(struct MapObject* trainerObj);
void __attribute__((long_call)) TrainerApproachPlayer(struct MapObject* trainerObj, u8 range);
u16 __attribute__((long_call)) GetRematchTrainerId(u16 opponentId);

//Quest Log
void __attribute__((long_call)) QuestLogRemtachBattleStore(void);
bool8 __attribute__((long_call)) IsQuestLogActive(void);

//Party Menu Functions
void __attribute__((long_call)) sub_811FA78(u8 taskId);
s8 __attribute__((long_call)) sub_812029C(s8 a, s8 b);
void __attribute__((long_call)) sub_8122D78(void);

//Palette Functions
void __attribute__((long_call)) LoadCompressedPalette(const void* src, u16 offset, u16 size);
void __attribute__((long_call)) LoadPalette(const void *src, u16 offset, u16 size);
void __attribute__((long_call)) FillPalette(u16 value, u16 offset, u16 size);
void __attribute__((long_call)) TransferPlttBuffer(void);
u8 __attribute__((long_call)) UpdatePaletteFade(void);
void __attribute__((long_call)) ResetPaletteFade(void);
void __attribute__((long_call)) ReadPlttIntoBuffers(void);
bool8 __attribute__((long_call)) BeginNormalPaletteFade(u32 selectedPalettes, s8 delay, u8 startY, u8 targetY, u16 blendColor);
void __attribute__((long_call)) ResetPaletteStructByUid(u16 a1);
void __attribute__((long_call)) ResetPaletteStruct(u8 paletteNum);
void __attribute__((long_call)) ResetPaletteFadeControl(void);
u8 __attribute__((long_call)) GetPaletteNumByUid(u16 uid);
u8 __attribute__((long_call)) UpdateNormalPaletteFade(void);
void __attribute__((long_call)) InvertPlttBuffer(u32 selectedPalettes);
void __attribute__((long_call)) TintPlttBuffer(u32 selectedPalettes, s8 r, s8 g, s8 b);
void __attribute__((long_call)) UnfadePlttBuffer(u32 selectedPalettes);
void __attribute__((long_call)) BeginFastPaletteFade(u8 submode);
void __attribute__((long_call)) BeginFastPaletteFadeInternal(u8 submode);
u8 __attribute__((long_call)) UpdateFastPaletteFade(void);
void __attribute__((long_call)) BeginHardwarePaletteFade(u8 blendCnt, u8 delay, u8 y, u8 targetY, u8 shouldResetBlendRegisters);
u8 __attribute__((long_call)) UpdateHardwarePaletteFade(void);
void __attribute__((long_call)) UpdateBlendRegisters(void);
bool8 __attribute__((long_call)) IsSoftwarePaletteFadeFinishing(void);
void __attribute__((long_call)) BlendPalettes(u32 selectedPalettes, u8 coeff, u16 color);
void __attribute__((long_call)) BlendPalettesUnfaded(u32 selectedPalettes, u8 coeff, u16 color);
void __attribute__((long_call)) TintPalette_GrayScale(u16 *palette, u16 count);
void __attribute__((long_call)) TintPalette_GrayScale2(u16 *palette, u16 count);
void __attribute__((long_call)) TintPalette_SepiaTone(u16 *palette, u16 count);
void __attribute__((long_call)) TintPalette_CustomTone(u16 *palette, u16 count, u16 rTone, u16 gTone, u16 bTone);

//Other Functions
void* __attribute__((long_call)) Malloc(u32 size);
void* __attribute__((long_call)) Calloc(u32 size);
void __attribute__((long_call)) Free(void *pointer);
u8 __attribute__((long_call)) CreateSprite(const struct SpriteTemplate* template, s16 x, s16 y, u8 subpriority);
void __attribute__((long_call)) SpriteCallbackDummy(struct Sprite *sprite);
void __attribute__((long_call)) FreeSpriteTilesByTag(u16 tag);
void __attribute__((long_call)) FreeSpritePaletteByTag(u16 tag);
u16 __attribute__((long_call)) GetSpriteTileStartByTag(u16 tag);
u8 __attribute__((long_call)) IndexOfSpritePaletteTag(u16 tag);
u8* __attribute__((long_call)) StringCopy(u8* dest, u8* src);
u8* __attribute__((long_call)) ConvertIntToDecimalStringN(u8 *dest, s32 value, enum StringConvertMode mode, u8 n);
void __attribute__((long_call)) LZDecompressVram(const void *src, void *dest);
bool8 __attribute__((long_call)) LoadCompressedSpriteSheetUsingHeap(const struct CompressedSpriteSheet* src);
bool8 __attribute__((long_call)) LoadCompressedSpritePaletteUsingHeap(const struct CompressedSpritePalette *src);
int __attribute__((long_call)) CountTrailingZeroBits(u32 value);
void __attribute__((long_call)) nullsub_8(void);
u8 __attribute__((long_call)) GetEventObjectIdByLocalIdAndMapInternal(u8 localId, u8 mapNum, u8 mapGroupId);
u8 __attribute__((long_call)) GetEventObjectIdByLocalId(u8 localId);
u16 __attribute__((long_call)) VarGet(u16 var);
bool8 __attribute__((long_call)) VarSet(u16 var, u16 value);
u16 __attribute__((long_call)) Random();
void __attribute__((long_call)) PlayBGM(u16 songNum);
void __attribute__((long_call)) PlaySE(u16 songNum);
void __attribute__((long_call)) ResetTasks(void);
u8 __attribute__((long_call)) CreateTask(TaskFunc func, u8 priority);
void __attribute__((long_call)) InsertTask(u8 newTaskId);
void __attribute__((long_call)) DestroyTask(u8 taskId);
void __attribute__((long_call)) RunTasks(void);
u8 __attribute__((long_call)) FindFirstActiveTask(void);
void __attribute__((long_call)) TaskDummy(u8 taskId);
void __attribute__((long_call)) SetTaskFuncWithFollowupFunc(u8 taskId, TaskFunc func, TaskFunc followupFunc);
void __attribute__((long_call)) SwitchTaskToFollowupFunc(u8 taskId);
bool8 __attribute__((long_call)) FuncIsActiveTask(TaskFunc func);
u8 __attribute__((long_call)) FindTaskIdByFunc(TaskFunc func);
u8 __attribute__((long_call)) GetTaskCount(void);
u8* __attribute__((long_call)) ReturnEmptyStringIfNull(const u8* string);
void __attribute__((long_call)) m4aSongNumStop(u16 n);
void __attribute__((long_call)) m4aMPlayContinue(struct MusicPlayerInfo *mplayInfo);
void __attribute__((long_call)) m4aMPlayVolumeControl(struct MusicPlayerInfo *mplayInfo, u16 trackBits, u16 volume);
void __attribute__((long_call)) CpuSet(const void* src, void* dest, u32 mode);
u16 __attribute__((long_call)) Sqrt(u32);
u32 __attribute__((long_call)) udivsi(u32, u32);
u32 __attribute__((long_call)) umodsi(u32, u32);

//The Deal Breaker
void __attribute__((long_call)) break_func(u32);
