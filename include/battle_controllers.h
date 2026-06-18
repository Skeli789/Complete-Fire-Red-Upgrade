#pragma once

#include "global.h"

enum
{
    REQUEST_ALL_BATTLE,
    REQUEST_SPECIES_BATTLE,
    REQUEST_HELDITEM_BATTLE,
    REQUEST_MOVES_PP_BATTLE,
    REQUEST_MOVE1_BATTLE,
    REQUEST_MOVE2_BATTLE,
    REQUEST_MOVE3_BATTLE,
    REQUEST_MOVE4_BATTLE,
    REQUEST_PP_DATA_BATTLE,
    REQUEST_PPMOVE1_BATTLE,
    REQUEST_PPMOVE2_BATTLE,
    REQUEST_PPMOVE3_BATTLE,
    REQUEST_PPMOVE4_BATTLE,
    REQUEST_UNUSED_13_BATTLE,
    REQUEST_UNUSED_14_BATTLE,
    REQUEST_UNUSED_15_BATTLE,
    REQUEST_UNUSED_16_BATTLE,
    REQUEST_OTID_BATTLE,
    REQUEST_EXP_BATTLE,
    REQUEST_HP_EV_BATTLE,
    REQUEST_ATK_EV_BATTLE,
    REQUEST_DEF_EV_BATTLE,
    REQUEST_SPEED_EV_BATTLE,
    REQUEST_SPATK_EV_BATTLE,
    REQUEST_SPDEF_EV_BATTLE,
    REQUEST_FRIENDSHIP_BATTLE,
    REQUEST_POKERUS_BATTLE,
    REQUEST_MET_LOCATION_BATTLE,
    REQUEST_MET_LEVEL_BATTLE,
    REQUEST_MET_GAME_BATTLE,
    REQUEST_POKEBALL_BATTLE,
    REQUEST_ALL_IVS_BATTLE,
    REQUEST_HP_IV_BATTLE,
    REQUEST_ATK_IV_BATTLE,
    REQUEST_DEF_IV_BATTLE,
    REQUEST_SPEED_IV_BATTLE,
    REQUEST_SPATK_IV_BATTLE,
    REQUEST_SPDEF_IV_BATTLE,
    REQUEST_PERSONALITY_BATTLE,
    REQUEST_CHECKSUM_BATTLE,
    REQUEST_STATUS_BATTLE,
    REQUEST_LEVEL_BATTLE,
    REQUEST_HP_BATTLE,
    REQUEST_MAX_HP_BATTLE,
    REQUEST_ATK_BATTLE,
    REQUEST_DEF_BATTLE,
    REQUEST_SPEED_BATTLE,
    REQUEST_SPATK_BATTLE,
    REQUEST_SPDEF_BATTLE,
    REQUEST_COOL_BATTLE,
    REQUEST_BEAUTY_BATTLE,
    REQUEST_CUTE_BATTLE,
    REQUEST_SMART_BATTLE,
    REQUEST_TOUGH_BATTLE,
    REQUEST_SHEEN_BATTLE,
    REQUEST_COOL_RIBBON_BATTLE,
    REQUEST_BEAUTY_RIBBON_BATTLE,
    REQUEST_CUTE_RIBBON_BATTLE,
    REQUEST_SMART_RIBBON_BATTLE,
    REQUEST_FORM_CHANGE_BATTLE, //Was REQUEST_TOUGH_RIBBON_BATTLE
};

#define RESET_ACTION_MOVE_SELECTION     0
#define RESET_ACTION_SELECTION          1
#define RESET_MOVE_SELECTION            2

#define BALL_NO_SHAKES          0
#define BALL_1_SHAKE            1
#define BALL_2_SHAKES           2
#define BALL_3_SHAKES_FAIL      3
#define BALL_3_SHAKES_SUCCESS   4
#define BALL_TRAINER_BLOCK      5

#define RET_VALUE_LEVELED_UP   11

#define INSTANT_HP_BAR_DROP     32767

struct UnusedControllerStruct
{
    u8 field_0 : 7;
    u8 flag_x80 : 1;
};

struct HpAndStatus
{
    u16 hp;
    u32 status;
};

struct MovePpInfo
{
    u16 moves[4];
    u8 pp[4];
    u8 ppBonuses;
};


struct ChooseMoveStruct
{
    u16 moves[MAX_MON_MOVES];
    u8 currentPp[MAX_MON_MOVES];
    u8 maxPp[MAX_MON_MOVES];
    u16 species;
    u8 monType1;
    u8 monType2;

//New Data - Old data is memcpy so don't touch
	u8 moveTypes[MAX_MON_MOVES];
	u8 moveResults[MAX_BATTLERS_COUNT][MAX_MON_MOVES];
	u8 zMoveResults[MAX_BATTLERS_COUNT][MAX_MON_MOVES];
	u16 movePowers[MAX_MON_MOVES];
	u16 moveAcc[MAX_MON_MOVES];
	u8 moveSplit[MAX_MON_MOVES];
	bool8 makesContact[MAX_MON_MOVES];
    u8 monType3;
    bool8 canMegaEvolve;
    u8 megaVariance;
    bool8 zMoveUsed : 1;
    bool8 megaDone : 1;
    bool8 ultraDone : 1;
    bool8 dynamaxDone : 1;
    bool8 dynamaxed : 1;
    bool8 atkIsGrounded : 1;
    bool8 teraDone : 1; // For Terastallization
    bool8 bank;
    u8 zPartyIndex;
    u16 possibleZMoves[MAX_MON_MOVES];
    u8 ability;
    bool8 canDynamax;
    bool8 canTera;      // For Terastallization
    u8 teraType;        // For Terastallization
    u16 possibleMaxMoves[MAX_MON_MOVES];
    u16 maxMovePowers[MAX_MON_MOVES];
    u8 dynamaxPartyIndex;
};


struct ChooseMoveStructOld
{
    u16 moves[4];		//0x0
    u8 currentPp[4];	//0x8
    u8 maxPp[4];		//0xC
    u16 species;		//0x10
    u8 monType1;		//0x12
    u8 monType2;		//0x13
};

enum
{
    CONTROLLER_GETMONDATA,
    CONTROLLER_GETRAWMONDATA,
    CONTROLLER_SETMONDATA,
    CONTROLLER_SETRAWMONDATA,
    CONTROLLER_LOADMONSPRITE,
    CONTROLLER_SWITCHINANIM,
    CONTROLLER_RETURNMONTOBALL,
    CONTROLLER_DRAWTRAINERPIC,
    CONTROLLER_TRAINERSLIDE,
    CONTROLLER_TRAINERSLIDEBACK,
    CONTROLLER_FAINTANIMATION,
    CONTROLLER_PALETTEFADE,
    CONTROLLER_SUCCESSBALLTHROWANIM,
    CONTROLLER_BALLTHROWANIM,
    CONTROLLER_PAUSE,
    CONTROLLER_MOVEANIMATION,
    CONTROLLER_PRINTSTRING,
    CONTROLLER_PRINTSTRINGPLAYERONLY,
    CONTROLLER_CHOOSEACTION,
    CONTROLLER_UNKNOWNYESNOBOX,
    CONTROLLER_CHOOSEMOVE,
    CONTROLLER_OPENBAG,
    CONTROLLER_CHOOSEPOKEMON,
    CONTROLLER_23,
    CONTROLLER_HEALTHBARUPDATE,
    CONTROLLER_EXPUPDATE,
    CONTROLLER_STATUSICONUPDATE,
    CONTROLLER_STATUSANIMATION,
    CONTROLLER_STATUSXOR,
    CONTROLLER_DATATRANSFER,
    CONTROLLER_DMA3TRANSFER,
    CONTROLLER_31,
    CONTROLLER_32,
    CONTROLLER_TWORETURNVALUES,
    CONTROLLER_CHOSENMONRETURNVALUE,
    CONTROLLER_ONERETURNVALUE,
    CONTROLLER_ONERETURNVALUE_DUPLICATE,
    CONTROLLER_37,
    CONTROLLER_38,
    CONTROLLER_39,
    CONTROLLER_40,
    CONTROLLER_HITANIMATION,
    CONTROLLER_42,
    CONTROLLER_EFFECTIVENESSSOUND,
    CONTROLLER_PLAYFANFAREORBGM,
    CONTROLLER_FAINTINGCRY,
    CONTROLLER_INTROSLIDE,
    CONTROLLER_INTROTRAINERBALLTHROW,
    CONTROLLER_DRAWPARTYSTATUSSUMMARY,
    CONTROLLER_49,
    CONTROLLER_50,
    CONTROLLER_SPRITEINVISIBILITY,
    CONTROLLER_BATTLEANIMATION,
    CONTROLLER_LINKSTANDBYMSG,
    CONTROLLER_RESETACTIONMOVESELECTION,
    CONTROLLER_55,
    /*new controllers should go here*/
    CONTROLLER_TERMINATOR_NOP,
    CONTROLLER_CMDS_COUNT
};


// general functions
/*
void HandleLinkBattleSetup(void);
void SetUpBattleVarsAndBirchZigzagoon(void);
void sub_8032768(void);
void sub_8033648(void);
*/
void __attribute__((long_call)) PrepareBufferDataTransfer(u8 a, u8 *data, u16 size);
void __attribute__((long_call)) PrepareBufferDataTransferLink(u8 bufferId, u16 size, u8 *data);

// emitters
void __attribute__((long_call)) EmitGetMonData(u8 bufferId, u8 requestId, u8 monToCheck);
void __attribute__((long_call)) EmitGetRawMonData(u8 bufferId, u8 monId, u8 bytes); // unused
void __attribute__((long_call)) EmitSetRawMonData(u8 bufferId, u8 monId, u8 bytes, void *data); // unused
void __attribute__((long_call)) EmitLoadMonSprite(u8 bufferId);
void __attribute__((long_call)) EmitSwitchInAnim(u8 bufferId, u8 partyId, bool8 dontClearSubstituteBit);
void __attribute__((long_call)) EmitReturnMonToBall(u8 bufferId, u8 arg1);
void __attribute__((long_call)) EmitDrawTrainerPic(u8 bufferId);
void __attribute__((long_call)) EmitTrainerSlide(u8 bufferId);
void __attribute__((long_call)) EmitTrainerSlideBack(u8 bufferId);
void __attribute__((long_call)) EmitFaintAnimation(u8 bufferId);
void __attribute__((long_call)) EmitPaletteFade(u8 bufferId); // unused
void __attribute__((long_call)) EmitSuccessBallThrowAnim(u8 bufferId); // unused
void __attribute__((long_call)) EmitBallThrowAnim(u8 bufferId, u8 caseId);
void __attribute__((long_call)) EmitPause(u8 bufferId, u8 toWait, void *data); // unused
void __attribute__((long_call)) EmitMoveAnimation(u8 bufferId, u16 move, u8 turnOfMove, u16 movePower, s32 dmg, u8 friendship, struct DisableStruct *disableStructPtr, u8 multihit);
void __attribute__((long_call)) EmitPrintString(u8 bufferId, u16 stringId);
void __attribute__((long_call)) EmitPrintSelectionString(u8 bufferId, u16 stringId);
void __attribute__((long_call)) EmitChooseAction(u8 bufferId, u8 arg1, u16 arg2);
void __attribute__((long_call)) EmitUnknownYesNoBox(u8 bufferId);
//void EmitChooseMove(u8 bufferId, bool8 isDoubleBattle, bool8 NoPpNumber, struct ChooseMoveStruct *movePpData);
void __attribute__((long_call)) EmitChooseItem(u8 bufferId, u8* arg1);
void __attribute__((long_call)) EmitChoosePokemon(u8 bufferId, u8 caseId, u8 arg2, u8 abilityId, u8* arg4);
void __attribute__((long_call)) EmitCmd23(u8 bufferId); // unused
void __attribute__((long_call)) EmitHealthBarUpdate(u8 bufferId, u16 hpValue);
//void EmitExpUpdate(u8 bufferId, u8 partyId, u16 expPoints);
void __attribute__((long_call)) EmitStatusIconUpdate(u8 bufferId, u32 status1, u32 status2);
void __attribute__((long_call)) EmitStatusAnimation(u8 bufferId, bool8 status2, u32 status);
void __attribute__((long_call)) EmitStatusXor(u8 bufferId, u8 b); // unused
//void __attribute__((long_call)) EmitDataTransfer(u8 bufferId, u16 size, void *data);
void __attribute__((long_call)) EmitDMA3Transfer(u8 bufferId, void *dst, u16 size, void *data); // unused
void __attribute__((long_call)) EmitPlayBGM(u8 bufferId, u16 songId, void *unusedDumbDataParameter); // unused
void __attribute__((long_call)) EmitCmd32(u8 bufferId, u16 size, void *c); // unused
void __attribute__((long_call)) EmitTwoReturnValues(u8 bufferId, u8 arg1, u16 arg2);
void __attribute__((long_call)) EmitChosenMonReturnValue(u8 bufferId, u8 b, u8 *c);
void __attribute__((long_call)) EmitOneReturnValue(u8 bufferId, u16 arg1);
void __attribute__((long_call)) EmitOneReturnValue_Duplicate(u8 bufferId, u16 b);
void __attribute__((long_call)) EmitCmd37(u8 bufferId); // unused
void __attribute__((long_call)) EmitCmd38(u8 bufferId, u8 b); // unused
void __attribute__((long_call)) EmitCmd39(u8 bufferId); // unused
void __attribute__((long_call)) EmitCmd40(u8 bufferId); // unused
void __attribute__((long_call)) EmitHitAnimation(u8 bufferId);
void __attribute__((long_call)) EmitCmd42(u8 bufferId);
void __attribute__((long_call)) EmitPlaySE(u8 bufferId, u16 songId);
void __attribute__((long_call)) EmitPlayFanfareOrBGM(u8 bufferId, u16 songId, bool8 playBGM);
void __attribute__((long_call)) EmitFaintingCry(u8 bufferId);
void __attribute__((long_call)) EmitIntroSlide(u8 bufferId, u8 terrainId);
void __attribute__((long_call)) EmitIntroTrainerBallThrow(u8 bufferId);
void __attribute__((long_call)) EmitDrawPartyStatusSummary(u8 bufferId, struct HpAndStatus* hpAndStatus, u8 arg2);
void __attribute__((long_call)) EmitHidePartyStatusSummary(u8 bufferId);
void __attribute__((long_call)) EmitEndBounceEffect(u8 bufferId);
void __attribute__((long_call)) EmitSpriteInvisibility(u8 bufferId, bool8 isInvisible);
void __attribute__((long_call)) EmitBattleAnimation(u8 bufferId, u8 animationId, u16 argument);
void __attribute__((long_call)) EmitLinkStandbyMsg(u8 bufferId, u8 arg1, bool32 arg2);
void __attribute__((long_call)) EmitResetActionMoveSelection(u8 bufferId, u8 caseId);
void __attribute__((long_call)) EmitCmd55(u8 bufferId, u8 arg1);

// player controller

void __attribute__((long_call)) SetControllerToPlayer(void);
void __attribute__((long_call)) nullsub_21(void);
void __attribute__((long_call)) PlayerHandleGetRawMonData(void);
void __attribute__((long_call)) sub_80587B0(void);
void __attribute__((long_call)) sub_805CC00(struct Sprite *sprite);
void __attribute__((long_call)) SetCB2ToReshowScreenAfterMenu(void);
void __attribute__((long_call)) SetCB2ToReshowScreenAfterMenu2(void);
void __attribute__((long_call)) c3_0802FDF4(u8 taskId);
void __attribute__((long_call)) ActionSelectionCreateCursorAt(u8 cursorPos, u8 unused);
void __attribute__((long_call)) ActionSelectionDestroyCursorAt(u8 cursorPos);
void __attribute__((long_call)) InitMoveSelectionsVarsAndStrings(void);
void __attribute__((long_call)) PlayerHandlePrintSelectionString(void);

// recorded player controller
void __attribute__((long_call)) SetControllerToRecordedPlayer(void);

// opponent controller
void __attribute__((long_call)) SetControllerToOpponent(void);

// player partner controller
//void SetControllerToPlayerPartner(void);

// safari controller
void __attribute__((long_call)) SetControllerToSafari(void);

// wally controller
void __attribute__((long_call)) SetControllerToWally(void);

// recorded opponent controller
void __attribute__((long_call)) SetControllerToRecordedOpponent(void);

// link opponent
void __attribute__((long_call)) SetControllerToLinkOpponent(void);

// link partner
void __attribute__((long_call)) SetControllerToLinkPartner(void);

