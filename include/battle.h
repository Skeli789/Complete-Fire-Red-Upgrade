#pragma once

#include "global.h"

// should they be included here or included individually by every file?
#include "constants/battle.h"
#include "battle_message.h"
#include "battle_util.h"
#include "battle_util2.h"
#include "battle_script_commands.h"
#include "battle_2.h"
#include "battle_ai_switch_items.h"
#include "battle_gfx_sfx_util.h"
#include "link.h"

/*
    Banks are a name given to what could be called a 'battlerId' or 'monControllerId'.
    Each bank has a value consisting of two bits.
    0x1 bit is responsible for the side, 0 = player's side, 1 = opponent's side.
    0x2 bit is responsible for the id of sent out pokemon. 0 means it's the first sent out pokemon, 1 it's the second one. (Triple battle didn't exist at the time yet.)
*/

#define BATTLE_BANKS_COUNT  4

#define IDENTITY_PLAYER_MON1        0
#define IDENTITY_OPPONENT_MON1      1
#define IDENTITY_PLAYER_MON2        2
#define IDENTITY_OPPONENT_MON2      3

#define SIDE_PLAYER     0x0
#define SIDE_OPPONENT   0x1

#define GET_BANK_IDENTITY(bank)((gBanksByIdentity[bank]))
#define GET_BANK_SIDE(bank)((GetBattlerPosition(bank) & BIT_SIDE))
#define GET_BANK_SIDE2(bank)((GET_BANK_IDENTITY(bank) & BIT_SIDE))


#define TRAINER_OPPONENT_3FE        0x3FE
//#define TRAINER_OPPONENT_C00        0xC00
#define TRAINER_OPPONENT_800        0x800
#define STEVEN_PARTNER_ID           0xC03
#define SECRET_BASE_OPPONENT        0x400

#define BATTLE_WON                  0x1
#define BATTLE_LOST                 0x2
#define BATTLE_DREW                 0x3
#define BATTLE_RAN                  0x4
#define BATTLE_PLAYER_TELEPORTED    0x5
#define BATTLE_POKE_FLED            0x6
#define BATTLE_CAUGHT               0x7
#define BATTLE_SAFARI_OUT_OF_BALLS  0x8
#define BATTLE_FORFEITED            0x9
#define BATTLE_OPPONENT_TELEPORTED  0xA

#define OUTCOME_LINK_BATTLE_RUN      0x80

#define STATUS_NONE             0x0
#define STATUS_SLEEP            0x7
#define STATUS_POISON           0x8
#define STATUS_BURN             0x10
#define STATUS_FREEZE           0x20
#define STATUS_PARALYSIS        0x40
#define STATUS_TOXIC_POISON     0x80
#define STATUS_TOXIC_COUNTER    0xF00

#define STATUS_PSN_ANY          ((STATUS_POISON | STATUS_TOXIC_POISON))
#define STATUS_ANY              ((STATUS_SLEEP | STATUS_POISON | STATUS_BURN | STATUS_FREEZE | STATUS_PARALYSIS | STATUS_TOXIC_POISON))

#define SIDE_STATUS_REFLECT          (1 << 0)
#define SIDE_STATUS_LIGHTSCREEN      (1 << 1)
#define SIDE_STATUS_X4               (1 << 2)
#define SIDE_STATUS_SPIKES           (1 << 4)
#define SIDE_STATUS_SAFEGUARD        (1 << 5)
#define SIDE_STATUS_FUTUREATTACK     (1 << 6)
#define SIDE_STATUS_MIST             (1 << 8)
#define SIDE_STATUS_SPIKES_DAMAGED   (1 << 9)

#define ACTION_USE_MOVE             0
#define ACTION_USE_ITEM             1
#define ACTION_SWITCH               2
#define ACTION_RUN                  3
#define ACTION_WATCHES_CAREFULLY    4
#define ACTION_SAFARI_ZONE_BALL     5
#define ACTION_POKEBLOCK_CASE       6
#define ACTION_GO_NEAR              7
#define ACTION_SAFARI_ZONE_RUN      8
#define ACTION_9                    9
#define ACTION_RUN_BATTLESCRIPT     10 // when executing an action
#define ACTION_TRY_FINISH			11
#define ACTION_CANCEL_PARTNER       12 // when choosing an action
#define ACTION_FINISHED             12 // when executing an action
#define ACTION_NOTHING_FAINTED      13 // when choosing an action
#define ACTION_INIT_VALUE           0xFF

#define MOVESTATUS_MISSED             (1 << 0)
#define MOVESTATUS_SUPEREFFECTIVE     (1 << 1)
#define MOVESTATUS_NOTVERYEFFECTIVE   (1 << 2)
#define MOVESTATUS_NOTAFFECTED        (1 << 3)
#define MOVESTATUS_ONEHITKO           (1 << 4)
#define MOVESTATUS_FAILED             (1 << 5)
#define MOVESTATUS_ENDURED            (1 << 6)
#define MOVESTATUS_HUNGON             (1 << 7)

#define MOVESTATUS_NOEFFECT ((MOVESTATUS_MISSED | MOVESTATUS_NOTAFFECTED | MOVESTATUS_FAILED))

#define MAX_TRAINER_ITEMS 4
//#define MAX_MON_MOVES 4

#define BATTLE_TERRAIN_GRASS        	0
#define BATTLE_TERRAIN_LONG_GRASS   	1
#define BATTLE_TERRAIN_SAND         	2
#define BATTLE_TERRAIN_SNOWY		   	3
#define BATTLE_TERRAIN_WATER        	4
#define BATTLE_TERRAIN_POND         	5
#define BATTLE_TERRAIN_MOUNTAIN     	6
#define BATTLE_TERRAIN_CAVE         	7
#define BATTLE_TERRAIN_INSIDE       	8
#define BATTLE_TERRAIN_PLAIN        	9
#define BATTLE_TERRAIN_INSIDE_2       	0xA
#define BATTLE_TERRAIN_INSIDE_3       	0xB
#define BATTLE_TERRAIN_INSIDE_4	       	0xC // Used by gym leaders  TODO: More interesting graphic here
#define BATTLE_TERRAIN_INSIDE_5	       	0xD
#define BATTLE_TERRAIN_INSIDE_6       	0xE
#define BATTLE_TERRAIN_LORLEI       	0xF
#define BATTLE_TERRAIN_BRUNO 			0x10
#define BATTLE_TERRAIN_AGATHA       	0x11
#define BATTLE_TERRAIN_LANCE 			0x12
#define BATTLE_TERRAIN_CHAMPION       	0x13

#ifdef NEW_BATTLE_BACKGROUNDS

#define BATTLE_TERRAIN_SPOOKY			0x14
#define BATTLE_TERRAIN_DESERT			0x15
#define BATTLE_TERRAIN_TORMA			0x16
#define BATTLE_TERRAIN_TORMA_DEPTHS		0x17
#define BATTLE_TERRAIN_SCALDING_SPA		0x18
#define BATTLE_TERRAIN_GYM				0x19
#define BATTLE_TERRAIN_FOREST			0x1A
#define BATTLE_TERRAIN_FOREST_PERADON	0x1B

#endif

//For Unbound
#ifdef UNBOUND

#define BATTLE_TERRAIN_BATTLE_TOWER			0xA
#define BATTLE_TERRAIN_BATTLE_CIRCUS		0xB
#define BATTLE_TERRAIN_MOLEMAN       		0xF
#define BATTLE_TERRAIN_ELIAS 				0x10
#define BATTLE_TERRAIN_ANABELLE       		0x11
#define BATTLE_TERRAIN_PENNY 				0x12
#define BATTLE_TERRAIN_SNOW_FIELD			0x14
#define BATTLE_TERRAIN_VOLCANO				0x15
#define BATTLE_TERRAIN_DARK_CAVE_WATER		0x16
#define BATTLE_TERRAIN_DARK_CAVE			0x17
#define BATTLE_TERRAIN_SNOW_CAVE			0x18
#define BATTLE_TERRAIN_FOREST				0x19
#define BATTLE_TERRAIN_ICE_IN_CAVE			0x1A
#define BATTLE_TERRAIN_RUINS_OF_VOID		0x1B
#define BATTLE_TERRAIN_DISTORTION_WORLD		0x1C
#define BATTLE_TERRAIN_GRASS_GYM			0x1D
#define BATTLE_TERRAIN_DESERT				0x1E
#define BATTLE_TERRAIN_WATER_IN_CAVE		0x1F
#define BATTLE_TERRAIN_WATER_IN_SNOW_CAVE	0x20
#define BATTLE_TERRAIN_LAVA_IN_VOLCANO		0x21
#define BATTLE_TERRAIN_WATER_IN_FOREST		0x22
#define BATTLE_TERRAIN_AUTUMN_GRASS			0x23
#define BATTLE_TERRAIN_AUTUMN_PLAIN			0x24
#define BATTLE_TERRAIN_SNOW_GRASS			0x25
#define BATTLE_TERRAIN_NIGHTMARE			0x26
#define BATTLE_TERRAIN_ANTISIS_CITY			0x27
#define BATTLE_TERRAIN_ANTISIS_SEWERS		0x28
#define BATTLE_TERRAIN_ANTISIS_SEWERS_WATER	0x29
#define BATTLE_TERRAIN_SKY_BATTLE			0x2A
#define BATTLE_TERRAIN_BOG					0x2B
#define BATTLE_TERRAIN_BOG_WATER			0x2C
#define BATTLE_TERRAIN_DISTORTION_WORLD_FULL 0x2D
#define BATTLE_TERRAIN_CUBE_SPACE			0x2E
#define BATTLE_TERRAIN_CRYSTAL_PEAK			0x2F
#define BATTLE_TERRAIN_CRYSTAL_PEAK_WATER	0x30

#define BATTLE_TERRAIN_RANDOM				0xFF
#endif

enum
{
	NO_TERRAIN,
	ELECTRIC_TERRAIN,
	GRASSY_TERRAIN,
	MISTY_TERRAIN,
	PSYCHIC_TERRAIN
};

// array entries for battle communication
#define MULTIUSE_STATE          0x0
#define CURSOR_POSITION         0x1
#define TASK_ID                 0x1 // task Id and cursor position share the same field
#define SPRITES_INIT_STATE1     0x1 // shares the Id as well
#define SPRITES_INIT_STATE2     0x2
#define MOVE_EFFECT_BYTE        0x3
#define ACTIONS_CONFIRMED_COUNT 0x4
#define MULTISTRING_CHOOSER     0x5
#define MSG_DISPLAY             0x7
#define BATTLE_COMMUNICATION_ENTRIES_COUNT  0x8

#define MOVE_TARGET_SELECTED        0x0
#define MOVE_TARGET_DEPENDS         0x1
#define MOVE_TARGET_USER_OR_PARTNER    0x2
#define MOVE_TARGET_RANDOM          0x4
#define MOVE_TARGET_BOTH            0x8
#define MOVE_TARGET_USER            0x10
#define MOVE_TARGET_FOES_AND_ALLY   0x20
#define MOVE_TARGET_ALL   			0x20
#define MOVE_TARGET_OPPONENTS_FIELD 0x40

#define MOVE_TARGET_SPREAD (MOVE_TARGET_BOTH | MOVE_TARGET_ALL)

// defines for the u8 array gTypeEffectiveness
#define TYPE_EFFECT_ATK_TYPE(i)((gTypeEffectiveness[i + 0]))
#define TYPE_EFFECT_DEF_TYPE(i)((gTypeEffectiveness[i + 1]))
#define TYPE_EFFECT_MULTIPLIER(i)((gTypeEffectiveness[i + 2]))

// defines for the gTypeEffectiveness multipliers
#define TYPE_MUL_NO_DATA			0
#define TYPE_MUL_NO_EFFECT          1
#define TYPE_MUL_NOT_EFFECTIVE      5
#define TYPE_MUL_NORMAL             10
#define TYPE_MUL_SUPER_EFFECTIVE    20

// special type table Ids
#define TYPE_FORESIGHT  0xFE
#define TYPE_ENDTABLE   0xFF

// for battle script commands
#define CMP_EQUAL               0x0
#define CMP_NOT_EQUAL           0x1
#define CMP_GREATER_THAN        0x2
#define CMP_LESS_THAN           0x3
#define CMP_COMMON_BITS         0x4
#define CMP_NO_COMMON_BITS      0x5

struct TrainerMoney
{
    u8 trainerClass;
    u8 money;
    u16 field2;
};

struct TrainerMonNoItemDefaultMoves
{
    u16 iv;
    u16 lvl;
    u16 species;
	u16 _;
};

struct TrainerMonItemDefaultMoves
{
    u16 iv;
    u16 lvl;
    u16 species;
    u16 heldItem;
};

struct TrainerMonNoItemCustomMoves
{
    u16 iv;
    u16 lvl;
    u16 species;
    u16 moves[4];
	u16 _; // 0x0000
};

struct TrainerMonItemCustomMoves
{
    u16 iv;
    u16 lvl;
    u16 species;
    u16 heldItem;
    u16 moves[4];
};

union TrainerMonPtr
{
    struct TrainerMonNoItemDefaultMoves* NoItemDefaultMoves;
    struct TrainerMonNoItemCustomMoves* NoItemCustomMoves;
    struct TrainerMonItemDefaultMoves* ItemDefaultMoves;
    struct TrainerMonItemCustomMoves* ItemCustomMoves;
};

struct Trainer
{
    /*0x00*/ u8 partyFlags;
    /*0x01*/ u8 trainerClass;
    /*0x02*/ u8 encounterMusic : 7; // last bit is gender
			 u8 gender : 1;
    /*0x03*/ u8 trainerPic;
    /*0x04*/ u8 trainerName[12];
    /*0x10*/ u16 items[4];
    /*0x18*/ bool8 doubleBattle;
    /*0x1C*/ u32 aiFlags;
    /*0x20*/ u8 partySize;
    /*0x24*/ union TrainerMonPtr party;
};

#define PARTY_FLAG_CUSTOM_MOVES     0x1
#define PARTY_FLAG_HAS_ITEM         0x2

/*
extern const struct Trainer gTrainers[];
*/
#define TRAINER_ENCOUNTER_MUSIC(trainer)((gTrainers[trainer].encounterMusic_gender & 0x7F))

struct UnknownFlags
{
    u32 flags[4];
};

#define RESOURCE_FLAG_FLASH_FIRE 1
/*
struct BattleMove
{
    u8 effect;
    u8 power;
    u8 type;
    u8 accuracy;
    u8 pp;
    u8 secondaryEffectChance;
    u8 target;
    s8 priority;
    u8 flags;
	u8 z_move_power;
	u8 split;
	u8 z_move_effect;
};
*/
extern const struct BattleMove gBattleMoves[];

#define FLAG_MAKES_CONTACT          0x1
#define FLAG_PROTECT_AFFECTED       0x2
#define FLAG_MAGIC_COAT_AFFECTED    0x4
#define FLAG_SNATCH_AFFECTED        0x8
#define FLAG_MIRROR_MOVE_AFFECTED   0x10
#define FLAG_KINGS_ROCK_AFFECTED    0x20
#define FLAG_TRIAGE_AFFECTED     	0x40

#define SPLIT_PHYSICAL 0
#define SPLIT_SPECIAL 1
#define SPLIT_STATUS 2

struct DisableStruct
{
    /*0x00*/ u32 transformedMonPersonality;
    /*0x04*/ u16 disabledMove;
    /*0x06*/ u16 encoredMove;
    /*0x08*/ u8 protectUses;
    /*0x09*/ u8 stockpileCounter;
    /*0x0A*/ u8 substituteHP;
    /*0x0B*/ u8 disableTimer1 : 4;
    /*0x0B*/ u8 disableTimer2 : 4;
    /*0x0C*/ u8 encoredMovePos;
    /*0x0D*/ u8 unkD;
    /*0x0E*/ u8 encoreTimer:4;
    /*0x0E*/ u8 encoreTimerStartValue:4;
    /*0x0F*/ u8 perishSongTimer:4;
    /*0x0F*/ u8 perishSongTimerStartValue:4;
    /*0x10*/ u8 furyCutterCounter;
    /*0x11*/ u8 rolloutTimer:4;
    /*0x11*/ u8 rolloutTimerStartValue:4;
    /*0x12*/ u8 chargeTimer:4;
    /*0x12*/ u8 chargeTimerStartValue:4;
    /*0x13*/ u8 tauntTimer:4;
    /*0x13*/ u8 tauntTimer2:4;
    /*0x14*/ u8 bankPreventingEscape;
    /*0x15*/ u8 bankWithSureHit;
    /*0x16*/ u8 isFirstTurn;
    /*0x17*/ u8 unk17;
    /*0x18*/ u8 truantCounter : 1;
    /*0x18*/ u8 truantSwitchInHack:1;
    /*0x18*/ u8 unk18_a_2 : 2;
    /*0x18*/ u8 mimickedMoves : 4;
    /*0x19*/ u8 rechargeTimer;
    /*0x1A*/ u8 unk1A[2];
};

extern struct DisableStruct gDisableStructs[BATTLE_BANKS_COUNT];

struct ProtectStruct
{
    /* field_0 */
    u32 protected:1;
    u32 endured:1;
    u32 onlyStruggle:1;
    u32 helpingHand:1;
    u32 bounceMove:1;
    u32 stealMove:1;
    u32 flag0Unknown:1;
    u32 prlzImmobility:1;
    /* field_1 */
    u32 confusionSelfDmg:1;
    u32 targetNotAffected:1;
    u32 chargingTurn:1;
    u32 fleeFlag:2; // for RunAway and Smoke Ball
    u32 usedImprisonedMove:1;
    u32 loveImmobility:1;
    u32 usedDisabledMove:1;
    /* field_2 */
    u32 usedTauntedMove:1;      	// 0x1
    u32 flag2Unknown:1;         	// 0x2
    u32 flinchImmobility:1;     	// 0x4
    u32 notFirstStrike:1;       	// 0x8
    u32 palaceUnableToUseMove : 1;	// 0x10 //May be implemented...
    u32 flag_x20 : 1;           	// 0x20
    u32 obstruct : 1;           	// 0x40
    u32 obstructDamage : 1;         // 0x80
	u32 SilkTrap : 1;           	
    u32 SilkTrapDamage : 1;
    /* field_3 */
    u32 KingsShield : 1;
    u32 SpikyShield : 1;
	u32 BanefulBunker : 1;
	u32 BurningBulwark : 1;
    u32 kingsshield_damage : 1;
    u32 spikyshield_damage : 1;
    u32 banefulbunker_damage : 1;
	u32 BurningBulwark_damage : 1;
    u32 enduredSturdy : 1;
    u32 Field3 : 1;

    /* field_4 */ u32 physicalDmg;
    /* field_8 */ u32 specialDmg;
    /* field_C */ u8 physicalBank;
    /* field_D */ u8 specialBank;
    /* field_E */ u16 fieldE;
};

extern struct ProtectStruct gProtectStructs[BATTLE_BANKS_COUNT];

struct SpecialStatus
{
    u8 statLowered : 1;             // 0x1
    u8 lightningRodRedirected : 1;  // 0x2
    u8 restoredBankSprite: 1;       // 0x4
    u8 intimidatedPoke : 1;         // 0x8
    u8 traced : 1;                  // 0x10
    u8 ppNotAffectedByPressure : 1;
    u8 flag40 : 1;
    u8 focusBanded : 1;				// 0x80
    u8 field1[3];
    s32 moveturnLostHP;
    s32 moveturnLostHP_physical;
    s32 moveturnLostHP_special;
    u8 moveturnPhysicalBank;
    u8 moveturnSpecialBank;
    u8 field12 : 1;
    u8 field13;
};

extern struct SpecialStatus gSpecialStatuses[BATTLE_BANKS_COUNT];

struct SideTimer
{
    /*0x00*/ u8 reflectTimer;
    /*0x01*/ u8 reflectBank;
    /*0x02*/ u8 lightscreenTimer;
    /*0x03*/ u8 lightscreenBank;
    /*0x04*/ u8 mistTimer;
    /*0x05*/ u8 mistBank;
    /*0x06*/ u8 safeguardTimer;
    /*0x07*/ u8 safeguardBank;
    /*0x08*/ u8 followmeTimer;
    /*0x09*/ u8 followmeTarget;
    /*0x0A*/ u8 spikesAmount : 2;
			 u8 tspikesAmount : 2;
			 u8 srAmount : 1;
			 u8 stickyWeb : 1;
			 u8 steelsurge: 1;
	/*0x0B*/ u8 stickyWebBank;
};

extern struct SideTimer gSideTimers[];

struct WishFutureKnock
{
    u8 futureSightCounter[BATTLE_BANKS_COUNT];
    u8 futureSightAttacker[BATTLE_BANKS_COUNT];
    u32 futureSightPartyIndex[BATTLE_BANKS_COUNT]; //was s32 futureSightDmg[BATTLE_BANKS_COUNT];
    u16 futureSightMove[BATTLE_BANKS_COUNT];
    u8 wishCounter[BATTLE_BANKS_COUNT];
    u8 wishMonId[BATTLE_BANKS_COUNT];
    u8 weatherDuration;
    u8 knockedOffPokes[2];
};

extern struct WishFutureKnock gWishFutureKnock;

struct AI_ThinkingStruct
{
    u8 aiState;
    u8 movesetIndex;
    u16 moveConsidered;
    s8 score[4];
    u32 funcResult;
    u32 aiFlags;
    u8 aiAction;
    u8 aiLogicId;
    u8 filler12[6];
    u8 simulatedRNG[4];
};

extern u8 gActiveBattler;
extern u8 gBattlerTarget;
extern u8 gAbsentBattlerFlags;

// script's table id to bit
#define AI_SCRIPT_CHECK_BAD_MOVE (1 << 0)
#define AI_SCRIPT_SEMI_SMART (1 << 1)
#define AI_SCRIPT_CHECK_GOOD_MOVE (1 << 2)

// 10 - 28 are not used
#define AI_SCRIPT_ROAMING (1 << 29)
#define AI_SCRIPT_SAFARI (1 << 30)
#define AI_SCRIPT_FIRST_BATTLE (1 << 31)

extern struct BattlePokemon gBattleMons[MAX_BATTLERS_COUNT];

struct BattleHistory //0x20003D0
{
    /*0x00*/ u16 usedMoves[MAX_BATTLERS_COUNT][MAX_MON_MOVES]; // 0xFFFF means move not used (confuse self hit, etc)
    /*0x20*/ u8 abilities[MAX_BATTLERS_COUNT/* / 2*/];
    /*0x22*/ //u8 itemEffects[MAX_BATTLERS_COUNT / 2]; //Moved to gNewBS
    /*0x24*/ u16 trainerItems[MAX_BATTLERS_COUNT]; //0x20003F4
    /*0x2C*/ u8 itemsNo;
};

struct BattleScriptsStack
{
    const u8 *ptr[8];
    u8 size;
};

struct BattleCallbacksStack
{
    void (*function[8])(void);
    u8 size;
};

struct StatsArray
{
    u16 hp;
    u16 atk;
    u16 def;
    u16 spd;
    u16 spAtk;
    u16 spDef;
};

struct StatFractions
{
    u8 dividend;
    u8 divisor;
	u16 _;
};

struct SecretBaseRecord
{
    /*0x1A9C*/ u8 secretBaseId;
    /*0x1A9D*/ u8 sbr_field_1_0:4;
    /*0x1A9D*/ u8 gender:1;
    /*0x1A9D*/ u8 sbr_field_1_5:1;
    /*0x1A9D*/ u8 sbr_field_1_6:2;
    /*0x1A9E*/ u8 trainerName[PLAYER_NAME_LENGTH];
    /*0x1AA5*/ u8 trainerId[4]; // byte 0 is used for determining trainer class
    /*0x1AA9*/ u8 language;
    /*0x1AAA*/ u16 sbr_field_e;
    /*0x1AAC*/ u8 sbr_field_10;
    /*0x1AAD*/ u8 sbr_field_11;
    /*0x1AAE*/ u8 decorations[16];
    /*0x1ABE*/ u8 decorationPos[16];
    /*0x1AD0*/ struct SecretBaseParty party;
};

struct BattleResources
{
    struct SecretBaseRecord* secretBase;
    struct UnknownFlags* flags;
    struct BattleScriptsStack* battleScriptsStack;
    struct BattleCallbacksStack* battleCallbackStack;
    struct StatsArray* statsBeforeLvlUp;
    struct AI_ThinkingStruct *ai;
    struct BattleHistory *battleHistory;
    struct BattleScriptsStack *AIScriptsStack;
 //   u8 bufferA[MAX_BATTLERS_COUNT][0x200];
 //   u8 bufferB[MAX_BATTLERS_COUNT][0x200];
};

extern struct BattleResources* gBattleResources;

#define BATTLESCRIPTS_STACK     (gBattleResources->battleScriptsStack)
#define BATTLE_CALLBACKS_STACK  (gBattleResources->battleCallbackStack)
#define BATTLE_LVLUP_STATS      (gBattleResources->statsBeforeLvlUp)

struct BattleResults
{
	u8 playerFaintCounter;    // 0x0
	u8 opponentFaintCounter;  // 0x1
	u8 playerSwitchesCounter; // 0x2
	u8 numHealingItemsUsed;   // 0x3
	u8 numRevivesUsed;        // 0x4
	u8 playerMonWasDamaged:1; // 0x5
	u8 usedMasterBall:1;      // 0x5
	u8 caughtMonBall:4;       // 0x5
	u8 shinyWildMon:1;        // 0x5
	u8 unk5_7:1;              // 0x5
	u16 playerMon1Species;    // 0x6
	u8 playerMon1Name[POKEMON_NAME_LENGTH + 1];    // 0x8
	u8 battleTurnCounter;     // 0x13
	u8 playerMon2Name[POKEMON_NAME_LENGTH + 1];    // 0x14
	u8 pokeblockThrows;       // 0x1F
	u16 lastOpponentSpecies;  // 0x20
	u16 lastUsedMovePlayer;   // 0x22
	u16 lastUsedMoveOpponent; // 0x24
	u16 playerMon2Species;    // 0x26
	u16 caughtMonSpecies;     // 0x28
	u8 caughtMonNick[POKEMON_NAME_LENGTH + 1];     // 0x2A
	u8 filler35[1];           // 0x35
	u8 catchAttempts[11];     // 0x36
};

extern struct BattleResults gBattleResults;

#define MAX_NUM_RAID_SHIELDS 5

struct BattleStruct
{
	u8 turnEffectsTracker;
	u8 turnEffectsBank;
	u8 filler2;
	u8 turncountersTracker;
	u16 wrappedMove[4];
	u8 moveTarget[4];
	u8 expGetterMonId;
	u8 field_11;
	u8 wildVictorySong;
	u8 dynamicMoveType;
	u8 wrappedBy[4];
	u16 assistPossibleMoves[PARTY_SIZE * MAX_MON_MOVES]; // 5 mons, each of them knowing 4 moves
	u8 focusPunchBank;
	u8 battlerPreventingSwitchout;
	u8 moneyMultiplier;
	u8 savedTurnActionNumber;
	u8 switchInAbilitiesCounter;
	u8 faintedActionsState;
	u8 faintedActionsBank;
	u8 field_4F;
	u16 expValue;
	u8 field_52;
	u8 sentInPokes;
	bool8 selectionScriptFinished[BATTLE_BANKS_COUNT];
	u8 switchoutPartyIndex[BATTLE_BANKS_COUNT];
	u8 monToSwitchIntoId[BATTLE_BANKS_COUNT];
	u8 battlerPartyOrders[MAX_BATTLERS_COUNT][3];
	u8 runTries;
	u8 caughtMonNick[11]; //0x200007D
	u8 field_78;
	u8 safariGoNearCounter;
	u8 safariPkblThrowCounter;
	u8 safariEscapeFactor;
	u8 safariCatchFactor;
	u8 linkBattleVsSpriteId_V;
	u8 linkBattleVsSpriteId_S;
	u8 castformToChangeInto;
	u8 chosenMovePositions[BATTLE_BANKS_COUNT]; //0x2000090
	u8 stateIdAfterSelScript[BATTLE_BANKS_COUNT];
	u8 field_88;
	u8 field_89;
	u8 field_8A;
	u8 field_8B;
	u8 field_8C;
	u8 field_8D;
	u8 stringMoveType;
	u8 expGetterBank;
	u8 field_90;
	u8 field_91;
	u8 switchoutIndex[2];
	u8 wallyBattleState;
	u8 wallyMovesState;
	u8 wallyWaitFrames;
	u8 wallyMoveFrames;
	u16 lastTakenMove[MAX_BATTLERS_COUNT]; //Used by mirror move
	u8 field_A0;
	u8 field_A1;
	u8 field_A2;
	u8 field_A3;
	u8 field_A4;
	u8 field_A5;
	u8 field_A6;
	u8 field_A7;
	u16 hpOnSwitchout[2];
	u8 abilityPreventingSwitchout;
	u8 hpScale;
	u16 savedBattleTypeFlags;
	void (*savedCallback)(void);
	u8 synchronizeMoveEffect;
	u8 multiplayerId;
	u8 overworldWeatherDone;
	u8 atkCancellerTracker;
	u16 usedHeldItems[MAX_BATTLERS_COUNT];
	u16 chosenItem[MAX_BATTLERS_COUNT / 2]; //Why is this a u8?
	u8 AI_itemType[MAX_BATTLERS_COUNT / 2];
	u8 AI_itemFlags[MAX_BATTLERS_COUNT / 2];
	u16 choicedMove[MAX_BATTLERS_COUNT];
	u16 changedItems[MAX_BATTLERS_COUNT];
	u8 intimidateBank;
	u8 switchInItemsCounter;
	u8 field_DA; //Battle Tower Related
	u8 turnSideTracker;
	u8 fillerDC[0xDF-0xDC];
	u8 givenExpMons; //The party indices in the opponent's party that have fainted and been given exp for
	u16 lastTakenMoveFrom[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; // 4 sub-arrays containing the moves that bank was hit by, by each bank //used by Mirror Move
	u16 castformPalette[MAX_BATTLERS_COUNT][16];
	u8 wishPerishSongState;
	u8 wishPerishSongBattlerId;
	u8 field_182;
	u8 field_183;
	u8 field_184;
	u8 field_185;
	u8 field_186;
	u8 field_187;
	struct BattleEnigmaBerry battleEnigmaBerry;
	u8 field_1A4[0x5C]; // currently unknown
}; //size == 0x200 bytes

extern struct BattleStruct* gBattleStruct;

struct NewBattleStruct
{
	//Field Counters
	u8 MudSportTimer;
	u8 WaterSportTimer;
	u8 GravityTimer;
	u8 TrickRoomTimer;
	u8 MagicRoomTimer;
	u8 WonderRoomTimer;
	u8 FairyLockTimer;
	u8 IonDelugeTimer;
	//u8 TerrainType;
	u8 TerrainTimer;

	//Team Counters
	u8 SeaOfFireTimers[NUM_BATTLE_SIDES];
	u8 SwampTimers[NUM_BATTLE_SIDES];
	u8 RainbowTimers[NUM_BATTLE_SIDES];
	u8 RetaliateCounters[NUM_BATTLE_SIDES];
	u8 LuckyChantTimers[NUM_BATTLE_SIDES];
	u8 TailwindTimers[NUM_BATTLE_SIDES];
	u8 AuroraVeilTimers[NUM_BATTLE_SIDES];
	u8 maxVineLashTimers[NUM_BATTLE_SIDES];
	u8 maxWildfireTimers[NUM_BATTLE_SIDES];
	u8 maxCannonadeTimers[NUM_BATTLE_SIDES];
	u8 maxVolcalithTimers[NUM_BATTLE_SIDES];
	u8 ragePowdered;

	//Personal Counters
	u8 TelekinesisTimers[MAX_BATTLERS_COUNT];
	u8 MagnetRiseTimers[MAX_BATTLERS_COUNT];
	u8 HealBlockTimers[MAX_BATTLERS_COUNT];
	u8 LaserFocusTimers[MAX_BATTLERS_COUNT];
	u8 ThroatChopTimers[MAX_BATTLERS_COUNT];
	u8 EmbargoTimers[MAX_BATTLERS_COUNT];
	u8 ElectrifyTimers[MAX_BATTLERS_COUNT];
	u8 SlowStartTimers[MAX_BATTLERS_COUNT];
	u8 StakeoutCounters[MAX_BATTLERS_COUNT];
	u8 StompingTantrumTimers[MAX_BATTLERS_COUNT];
	u8 splinterTimer[MAX_BATTLERS_COUNT];
	u8 splinterAttackerBank[MAX_BATTLERS_COUNT];
	u8 splinterAttackerMonId[MAX_BATTLERS_COUNT];
	u16 splinterMove[MAX_BATTLERS_COUNT];
	u8 NimbleCounters[MAX_BATTLERS_COUNT];
	u8 DestinyBondCounters[MAX_BATTLERS_COUNT];
	u8 MetronomeCounter[MAX_BATTLERS_COUNT];
	u8 metronomeItemBonus[MAX_BATTLERS_COUNT]; //Used to help with multi-turn attacks
	u8 IncinerateCounters[MAX_BATTLERS_COUNT];
	u8 LastUsedTypes[MAX_BATTLERS_COUNT];
	u8 lastTargeted[MAX_BATTLERS_COUNT];
	u8 usedMoveIndices[MAX_BATTLERS_COUNT];
	u8 DisabledMoldBreakerAbilities[MAX_BATTLERS_COUNT];
	u8 SuppressedAbilities[MAX_BATTLERS_COUNT];
	u8 neutralizingGasBlockedAbilities[MAX_BATTLERS_COUNT];
	u8 skyDropAttackersTarget[MAX_BATTLERS_COUNT]; //skyDropAttackersTarget[gBankAttacker] = gBankTarget
	u8 skyDropTargetsAttacker[MAX_BATTLERS_COUNT]; //skyDropTargetsAttacker[gBankTarget] = gBankAttacker
	u8 pickupStack[MAX_BATTLERS_COUNT];
	u8 synchronizeTarget[MAX_BATTLERS_COUNT]; //Bank + 1 that statused given bank
	u8 leftoverHealingDone[MAX_BATTLERS_COUNT]; //Leftovers already restored health this turn or Sticky Barb did damage
	u8 statRoseThisRound[MAX_BATTLERS_COUNT];
	u8 statFellThisTurn[MAX_BATTLERS_COUNT];
	u8 statFellThisRound[MAX_BATTLERS_COUNT];
	u8 recalculatedBestDoublesKillingScores[MAX_BATTLERS_COUNT];
	s8 lastBracketCalc[MAX_BATTLERS_COUNT]; //~0x2017A4C
	u8 chiStrikeCritBoosts[MAX_BATTLERS_COUNT];
	u8 sandblastCentiferno[MAX_BATTLERS_COUNT]; //Records if any banks are trapped by G-Max Centiferno or G-Max Sandblast
	u8 disguisedAs[MAX_BATTLERS_COUNT]; //The party index + 1 the mon with Illusion is disguised as
	u8 quickClawRandomNumber[MAX_BATTLERS_COUNT];
	u8 quickDrawRandomNumber[MAX_BATTLERS_COUNT];
	u8 powerShifted[MAX_BATTLERS_COUNT];
	u16 tookAbilityFrom[MAX_BATTLERS_COUNT]; //Helps display the correct Ability when one has been passed around
	u8 GlaiveRushTimers[MAX_BATTLERS_COUNT];
	u8 rageFistCounter[MAX_BATTLERS_COUNT];
	u8 SaltcureTimers[MAX_BATTLERS_COUNT];
	u8 CudChewCounter[MAX_BATTLERS_COUNT];
	u8 ElectroCounter[MAX_BATTLERS_COUNT];

	//Bit Fields for Banks
	u8 MicleBerryBits;
	u8 UnburdenBoosts;
	u8 BeakBlastByte;
	u8 playedFocusPunchMessage;
	u8 playedShellTrapMessage;
	u8 RoostCounter;
	u8 quickClawCustapIndicator;
	u8 quickDrawIndicator;
	u8 ateCustapBerry;
	u8 HealingWishLoc;
	u8 PowderByte;
	u8 turnOrderLocked;
	u8 tarShotBits;
	u8 trappedByOctolock;
	u8 trappedByNoRetreat;
	u8 AbsentBattlerHelper;
	u8 activeAbilityPopUps;
	u8 NoMoreMovingThisTurn;
	u8 handleSetSwitchIns;
	u8 brokeFreeMessage;
	u8 doSwitchInEffects;
	u8 devolveForgotMove;
	u8 hiddenAnimBattlerSprites;
	u8 enduredDamage;

	//Bit Fields for Party
	u8 canBelch[NUM_BATTLE_SIDES];
	u8 corrodedItems[NUM_BATTLE_SIDES];
	u8 revealedEnemyMons;

	//Other Helpers
	u8 switchOutAbilitiesState; //For tracking effects that happen on switch-out
	u8 switchInEffectsState; //For tracking effects that happen on switch-in
	u8 preFaintEffectsState; //For tracking effects that happen right before the target faints
	u8 faintEffectsState; //For tracking effects that happen after the target faints
	u8 endTurnBlockState; //For tracking sub-blocks utilized by the end turn function
	u8 IllusionBroken;
	u8 SentInBackup;
	u8 OriginalAttackerTargetCount;
	u8 MoveBounceTargetCount;
	u8 EchoedVoiceCounter;
	u8 EchoedVoiceDamageScale;
	u8 DancerBankCount;
	u8 CurrentTurnAttacker : 4;
	u8 CurrentTurnTarget : 4;
	u8 targetsToBringDown;
	u8 backupMoveEffect;
	u8 savedObjId;
	u8 lastFainted;
	s8 intimidateActive;
	u8 backupAbility;
	u8 switchOutBankLooper;
	u8 skipBankStatAnim;
	u8 maxGoldrushUses;
	u8 playerItemUsedCount;
	u8 originalAttackerBackup : 2;
	u8 originalTargetBackup : 2;
	u8 backupBattlerPosition : 2; //For Neutralizing Gas

	//Booleans
	bool8 MoveBounceInProgress : 2;
	bool8 moveWasBouncedThisTurn : 1;
	bool8 AttackerDidDamageAtLeastOnce : 1;
	bool8 PledgeHelper : 3;
	bool8 ParentalBondOn : 2;
	bool8 MeFirstByte : 1;
	bool8 ReceiverActivated : 1;
	bool8 consumedGem : 1;
	bool8 fusionFlareUsedPrior : 1;
	bool8 fusionBoltUsedPrior : 1;
	bool8 endTurnDone : 1;
	bool8 usedAmuletCoin : 1;
	bool8 usedHappyHour : 1;
	bool8 attackAnimationPlayed : 1;
	bool8 DancerInProgress : 1;
	bool8 DancerByte : 1;
	bool8 InstructInProgress : 1;
	bool8 NoSymbiosisByte : 1;
	bool8 SpectralThiefActive : 1;
	bool8 MultiHitOn : 1;
	bool8 secondaryEffectApplied : 1;
	bool8 bypassSubstitute : 1;
	bool8 criticalCapture : 1;
	bool8 criticalCaptureSuccess : 1;
	bool8 trainerSlideLowHpMsgDone : 1;
	bool8 TeleportBit : 1;
	bool8 restartEndTurnSwitching : 1;
	bool8 skipCertainSwitchInAbilities : 1;
	bool8 roundUsed : 1; //0x2017653
	bool8 activatedCustapQuickClaw : 1;
	bool8 calculatedAIPredictions : 1;
	bool8 batonPassing : 1;
	bool8 activateBlunderPolicy : 1;
	bool8 tempIgnoreAnimations : 1;
	bool8 firstFailedPokeBallStored : 1;
	bool8 fogBlownAwayByDefog : 1;
	bool8 terrainForcefullyRemoved : 1;
	bool8 printedNeutralizingGasOverMsg : 1;
	bool8 doneDoublesSpreadHit : 1; //For when the HP bars all go down during spread moves
	bool8 calculatedSpreadMoveData : 1; //After the damage has been calculated for all Pokemon hit by a spread move
	bool8 calculatedSpreadMoveAccuracy : 1;  //After the accuracy has been calculated for all Pokemon hit by a spread move
	bool8 breakDisguiseSpecialDmg : 1;
	bool8 handlingFaintSwitching : 1;
	bool8 doingPluckItemEffect : 1;
	bool8 usedXSpDef : 1; //Needed because it's hooked into the X Sp. Atk
	bool8 lessThanHalfHPBeforeShellBell : 1; //For Emergency Exit
	bool8 statBuffEffectNotProtectAffected : 1; //For Max Moves
	bool8 rolloutFinalHit : 1; //Helps with Rollout 5th hit damage calc
	bool8 usedLastBall : 1; //Helps prevent the bag from opening
	bool8 threwBall : 1; //Last Used Ball only appears once a ball has been thrown once
	bool8 totemOmniboostActive : 1; //Allows Contrary mons to get the correct animation
	bool8 dontActivateMoldBreakersAnymoreThisTurn : 1;
	bool8 trainerSlideInProgress : 1;
	bool8 stickyWebActive : 1;
	bool8 inPivotingMove : 1;
	bool8 triedToTakeWildItem : 1;
	bool8 printedStrongWindsWeakenedAttack : 1;
	bool8 isTrainerBattle : 1;
	bool8 cottonDownActive : 1;
	bool8 cramorantTransformed : 1;
	bool8 activateTemperFlare : 1;

	//Other
	u16 LastUsedMove;
	u16 NewWishHealthSave;
	u32 totalDamageGiven;
	u32 selfInflictedDamage; //For Emergency Exit
	u8 DancerTurnOrder[MAX_BATTLERS_COUNT];
	u8 PayDayByPartyIndices[PARTY_SIZE];
	item_t SavedConsumedItems[PARTY_SIZE];
	u8 expHelper[MAX_BATTLERS_COUNT];
	u8 megaIndicatorObjIds[MAX_BATTLERS_COUNT];
	u8 abilityPopUpIds[MAX_BATTLERS_COUNT][2];
	u8 backupSynchronizeBanks[2];
	u16 failedThrownPokeBall;
	u32 maxGoldrushMoney;
	u16 itemBackup[PARTY_SIZE];
	u8 hiddenHealthboxFlags[MAX_SPRITES / 8]; //~2017AD8
	u8 ringChallengeBannedTypes[3];
	u16 knockedOffWildItem;
	u8 criticalHitsThisBattle[PARTY_SIZE];
	u8 pickupMonId;
	u8 shellSideArmSplit[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT];

	//Things for Spread Moves
	s32 DamageTaken[MAX_BATTLERS_COUNT];
	s32 turnDamageTaken[MAX_BATTLERS_COUNT]; //Specifically for multi-hit moves and Emergency Exit
	u8 criticalMultiplier[MAX_BATTLERS_COUNT];
	u8 ResultFlags[MAX_BATTLERS_COUNT];
	u8 missStringId[MAX_BATTLERS_COUNT];
	u8 EnduranceHelper[MAX_BATTLERS_COUNT];
	bool8 noResultString[MAX_BATTLERS_COUNT];
	u8 foeSpreadTargets;
	u8 allSpreadTargets;

	struct
	{
		u8 chosen[4];
		u8 done[4];
		u8 state;
		u8 activeBank;
		const u8* script;
		bool8 megaEvoInProgress : 1; //Used to tell the game whether or not the turn order should be recalculated
	} megaData;

	struct
	{
		u8 chosen[4];
		u8 done[4];
	} ultraData;

	struct
	{
		u8 toBeUsed[4];
		u8 used[4];
		u8 partyIndex[2]; //Index of party member who used Z-Move
		u8 effect;
		u8 state;
		bool8 active : 1;
		bool8 effectApplied : 1; //Used so moves like Sleep Talk don't give 2 z-effects, clear in CMD49
		bool8 healReplacement : 1;
		bool8 viewing : 1;
		bool8 runningZEffect : 1;
		bool8 viewingDetails : 1; //Not actually related to Z-Moves, I just felt like putting it here
	} zMoveData;

	struct DynamaxData
	{
		bool8 toBeUsed[MAX_BATTLERS_COUNT];
		bool8 used[MAX_BATTLERS_COUNT];
		s8 timer[MAX_BATTLERS_COUNT]; //Negative number means permanent
		u8 partyIndex[NUM_BATTLE_SIDES];
		u8 shieldSpriteIds[MAX_NUM_RAID_SHIELDS]; //Shields for raid battles
		u8 shieldCount;					//The amount of shields created in a Raid Battle
		u8 shieldsDestroyed;			//The amount of shields destroyed in a Raid Battle
		u8 stormLevel;					//The number of Pokemon the raid boss has KO'd.
		u8 repeatedAttacks;				//The amount of times the raid boss took another attack
		bool8 active : 1;
		bool8 viewing : 1;
		bool8 raidShieldsUp : 1;
		bool8 attackAgain : 1;
		bool8 nullifiedStats : 1;
		u8 backupMoveSelectionCursorPos;
		u16 turnStartHP;
		u16 backupRaidMonItem;
	} dynamaxData;

	struct 
	{
		u16 zMoveHelper; //~0x2017B74
		u32 randSeed; //Seeded every frame regardless of whether or not the Random seed is normally
		bool8 sideSwitchedThisRound;
		bool8 goodToPivot;
		u8 pivotTo[PARTY_SIZE]; //Should be MAX_BATTLERS_COUNT
		u8 playerSwitchedCount;
		u8 switchingCooldown[MAX_BATTLERS_COUNT]; //Used for anti-AI cheesing
		u8 typeAbsorbSwitchingCooldown[MAX_BATTLERS_COUNT]; //Prevent a type absorb switching loop
		u8 switchesInARow[MAX_BATTLERS_COUNT];
		u8 previousMonIn[MAX_BATTLERS_COUNT];
		u8 secondPreviousMonIn[MAX_BATTLERS_COUNT];
		bool8 suckerPunchOkay[MAX_BATTLERS_COUNT];
		u8 itemEffects[MAX_BATTLERS_COUNT];
		u8 backupAbilities[MAX_BATTLERS_COUNT]; //For when Pokemon are temp Mega Evolved
		u16 movePredictions[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //movePredictions[bankAtk][bankDef]
		u16 previousMovePredictions[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //previousMovePredictions[bankAtk][bankDef]
		u16 strongestMove[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //strongestMove[bankAtk][bankDef]
		u16 strongestMoveGoesFirst[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //strongestMoveGoesFirst[bankAtk][bankDef]
		bool8 moveKnocksOut1Hit[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT][MAX_MON_MOVES]; //moveKnocksOut1Hit[bankAtk][bankDef][monMoveIndex]
		bool8 moveKnocksOut2Hits[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT][MAX_MON_MOVES]; //moveKnocksOut2Hits[bankAtk][bankDef][monMoveIndex]
		u32 damageByMove[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT][MAX_MON_MOVES]; //damageByMove[bankAtk][bankDef][monMoveIndex]
		u32 monDamageByMove[NUM_BATTLE_SIDES][PARTY_SIZE][MAX_BATTLERS_COUNT][MAX_MON_MOVES]; //monDamageByMove[side][atkMonId][bankDef][monMoveIndex]
		u32 monMaxDamage[NUM_BATTLE_SIDES][PARTY_SIZE][MAX_BATTLERS_COUNT]; //monMaxDamage[side][atkMonId][bankDef]
		u16 bestDoublesKillingMoves[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //bestDoublesKillingMoves[bankAtk][bankDef]
		s8 bestDoublesKillingScores[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //bestDoublesKillingScores[bankAtk][bankDef][bankDef / bankDefPartner / bankAtkPartner]
		u32 secondaryEffectDamage[MAX_BATTLERS_COUNT]; //secondaryEffectDamage[bank]
		bool8 canKnockOut[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //canKnockOut[bankAtk][bankDef]
		bool8 can2HKO[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //can2HKO[bankAtk][bankDef]
		u8 bestMonIdToSwitchInto[MAX_BATTLERS_COUNT][2]; //bestMonIdToSwitchInto[bankAtk][first or second choice] //~0x2017DD8
		s16 bestMonIdToSwitchIntoScores[MAX_BATTLERS_COUNT][2]; //bestMonIdToSwitchIntoScores[bankAtk][first or second choice]
		u8 bestMonIdToSwitchIntoFlags[MAX_BATTLERS_COUNT][2]; //bestMonIdToSwitchIntoScores[bankAtk][first or second choice]
		s16 monIdToSwitchIntoScores[NUM_BATTLE_SIDES][PARTY_SIZE]; //monIdToSwitchIntoScores[sideToSwitchFrom][monIdToSwitchTo]
		u8 monIdToSwitchIntoFlags[NUM_BATTLE_SIDES][PARTY_SIZE]; //monIdToSwitchIntoFlags[sideToSwitchFrom][monIdToSwitchTo]
		u8 calculatedAISwitchings[MAX_BATTLERS_COUNT]; //calculatedAISwitchings[bankSwitch]
		u8 fightingStyle[MAX_BATTLERS_COUNT]; //fightingStyle[bankAtk]
		u8 dynamaxMonId[NUM_BATTLE_SIDES]; //dynamaxMonId[SIDE(bankAtk)]
		u8 didTypeAbsorbSwitchToMonBefore[NUM_BATTLE_SIDES]; //didTypeAbsorbSwitchToMonBefore[SIDE(bankAtk)] & gBitTable[monAtk]
		bool8 onlyBadMovesLeft[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //onlyBadMovesLeft[bankAtk][bankDef]
		bool8 usingDesperateMove[MAX_BATTLERS_COUNT]; //usingDesperateMove[bankAtk]
		bool8 shouldFreeChoiceLockWithDynamax[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //shouldFreeChoiceLockWithDynamax[bankAtk][bankDef]
		bool8 shouldUseZMove[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT][MAX_MON_MOVES]; //shouldUseZMove[bankAtk][bankDef][monMoveIndex]
		bool8 dynamaxPotential[MAX_BATTLERS_COUNT][MAX_BATTLERS_COUNT]; //dynamaxPotential[bankAtk][bankDef]
		const void* megaPotential[MAX_BATTLERS_COUNT]; //aiMegaPotential[bankAtk] - stores evolution data of attacker
	} ai;

	struct Pokemon** foePartyBackup; //Pointer to dynamically allocated memory
};

extern struct NewBattleStruct* gNewBS; //0x203E038

struct TrainerSpotted {
    /* NPC state id */
    u8 id;

    /* Distance from trainer to player */
    u8 distance;

    /* The script the on the trainer NPC. */
    u8* script;
};

struct BattleExtensionState //Clear After Battle
{
    struct {
        u8 count;
		u8 approachingId;
		u8 firstTrainerNPCId; //Used in trainerbattle 0xB
		u8 secondTrainerNPCId;
        struct TrainerSpotted trainers[2];
    } spotted;

    void* partyBackup;
	pokemon_t* skyBattlePartyBackup;
	u8* trainerBIntroSpeech;
	u8* trainerBDefeatSpeech;
	u8* trainerBVictorySpeech;
	u8* trainerBCantBattleSpeech;
	u8* trainerBRetAddress;

	u16   partnerTrainerId;
	u16   partnerBackSpriteId;
	u16   trainerBTrainerId;
	u8	  multiTaskStateHelper;
};

extern struct BattleExtensionState ExtensionState;

struct NaturalGiftStruct
{
	u16 berry;
	u8 type;
	u8 power;
};

struct FlingStruct
{
	u8 power;
	u8 effect;
};

#define GET_MOVE_TYPE(move, typeArg)                        \
{                                                           \
    typeArg = gBattleStruct->dynamicMoveType & 0x3F;    	\
}

#define IS_TYPE_PHYSICAL(moveType)(moveType < TYPE_MYSTERY)
#define IS_TYPE_SPECIAL(moveType)(moveType > TYPE_MYSTERY)

/*
#define MOVE_EFFECT_SLEEP               0x1
#define MOVE_EFFECT_POISON              0x2
#define MOVE_EFFECT_BURN                0x3
#define MOVE_EFFECT_FREEZE              0x4
#define MOVE_EFFECT_PARALYSIS           0x5
#define MOVE_EFFECT_TOXIC               0x6
#define MOVE_EFFECT_CONFUSION           0x7
#define MOVE_EFFECT_FLINCH              0x8
#define MOVE_EFFECT_TRI_ATTACK          0x9
#define MOVE_EFFECT_UPROAR              0xA
#define MOVE_EFFECT_PAYDAY              0xB
#define MOVE_EFFECT_CHARGING            0xC
#define MOVE_EFFECT_WRAP                0xD
#define MOVE_EFFECT_RECOIL_25           0xE
#define MOVE_EFFECT_ATK_PLUS_1          0xF
#define MOVE_EFFECT_DEF_PLUS_1          0x10
#define MOVE_EFFECT_SPD_PLUS_1          0x11
#define MOVE_EFFECT_SP_ATK_PLUS_1       0x12
#define MOVE_EFFECT_SP_DEF_PLUS_1       0x13
#define MOVE_EFFECT_ACC_PLUS_1          0x14
#define MOVE_EFFECT_EVS_PLUS_1          0x15
#define MOVE_EFFECT_ATK_MINUS_1         0x16
#define MOVE_EFFECT_DEF_MINUS_1         0x17
#define MOVE_EFFECT_SPD_MINUS_1         0x18
#define MOVE_EFFECT_SP_ATK_MINUS_1      0x19
#define MOVE_EFFECT_SP_DEF_MINUS_1      0x1A
#define MOVE_EFFECT_ACC_MINUS_1         0x1B
#define MOVE_EFFECT_EVS_MINUS_1         0x1C
#define MOVE_EFFECT_RECHARGE            0x1D
#define MOVE_EFFECT_RAGE                0x1E
#define MOVE_EFFECT_STEAL_ITEM          0x1F
#define MOVE_EFFECT_PREVENT_ESCAPE      0x20
#define MOVE_EFFECT_NIGHTMARE           0x21
#define MOVE_EFFECT_ALL_STATS_UP        0x22
#define MOVE_EFFECT_RAPIDSPIN           0x23
#define MOVE_EFFECT_REMOVE_PARALYSIS    0x24
#define MOVE_EFFECT_ATK_DEF_DOWN        0x25
#define MOVE_EFFECT_RECOIL_33_PARALYSIS 0x26
#define MOVE_EFFECT_ATK_PLUS_2          0x27
#define MOVE_EFFECT_DEF_PLUS_2          0x28
#define MOVE_EFFECT_SPD_PLUS_2          0x29
#define MOVE_EFFECT_SP_ATK_PLUS_2       0x2A
#define MOVE_EFFECT_SP_DEF_PLUS_2       0x2B
#define MOVE_EFFECT_ACC_PLUS_2          0x2C
#define MOVE_EFFECT_EVS_PLUS_2          0x2D
#define MOVE_EFFECT_ATK_MINUS_2         0x2E
#define MOVE_EFFECT_DEF_MINUS_2         0x2F
#define MOVE_EFFECT_SPD_MINUS_2         0x30
#define MOVE_EFFECT_SP_ATK_MINUS_2      0x31
#define MOVE_EFFECT_SP_DEF_MINUS_2      0x32
#define MOVE_EFFECT_ACC_MINUS_2         0x33
#define MOVE_EFFECT_EVS_MINUS_2         0x34
#define MOVE_EFFECT_THRASH              0x35
#define MOVE_EFFECT_KNOCK_OFF           0x36
#define MOVE_EFFECT_NOTHING_37          0x37
#define MOVE_EFFECT_NOTHING_38          0x38
#define MOVE_EFFECT_NOTHING_39          0x39
#define MOVE_EFFECT_NOTHING_3A          0x3A
#define MOVE_EFFECT_SP_ATK_TWO_DOWN     0x3B
#define MOVE_EFFECT_NOTHING_3C          0x3C
#define MOVE_EFFECT_NOTHING_3D          0x3D
#define MOVE_EFFECT_NOTHING_3E          0x3E
#define MOVE_EFFECT_NOTHING_3F          0x3F
#define MOVE_EFFECT_AFFECTS_USER        0x40
#define MOVE_EFFECT_CERTAIN             0x80
*/
// table ids for general animations
#define B_ANIM_CASTFORM_CHANGE          0x0
#define B_ANIM_STATS_CHANGE             0x1
#define B_ANIM_SUBSTITUTE_FADE          0x2
#define B_ANIM_SUBSTITUTE_APPEAR        0x3
#define B_ANIM_x4                       0x4
#define B_ANIM_ITEM_KNOCKOFF            0x5
#define B_ANIM_TURN_TRAP                0x6
#define B_ANIM_ITEM_EFFECT              0x7
#define B_ANIM_SMOKEBALL_ESCAPE         0x8
#define B_ANIM_HANGED_ON                0x9
#define B_ANIM_RAIN_CONTINUES           0xA
#define B_ANIM_SUN_CONTINUES            0xB
#define B_ANIM_SANDSTORM_CONTINUES      0xC
#define B_ANIM_HAIL_CONTINUES           0xD
#define B_ANIM_LEECH_SEED_DRAIN         0xE
#define B_ANIM_MON_HIT                  0xF
#define B_ANIM_ITEM_STEAL               0x10
#define B_ANIM_SNATCH_MOVE              0x11
#define B_ANIM_FUTURE_SIGHT_HIT         0x12
#define B_ANIM_DOOM_DESIRE_HIT          0x13
#define B_ANIM_FOCUS_PUNCH_SET_UP       0x14
#define B_ANIM_INGRAIN_HEAL             0x15
#define B_ANIM_WISH_HEAL                0x16

#define B_ANIM_MON_SCARED 0x17
#define B_ANIM_GHOST_GET_OUT 0x18
#define B_ANIM_SILPH_SCOPED 0x19
#define B_ANIM_TURN_INTO_ROCK 0x1A
#define B_ANIM_WAITING_WAGGLE 0x1B
#define B_ANIM_LEVEL_UP_SHINE 0x1C
#define B_ANIM_CALL_BACK_POKEMON 0x1D
#define B_ANIM_CALL_BACK_POKEMON_2 0x1E
#define B_ANIM_CAPTURE_POKEMON 0x1F
#define B_ANIM_TURN_INTO_POKEBALL 0x20
#define B_ANIM_SWITCH 0x21
#define B_ANIM_MON_TO_SUBSTITUTE_2 0x22
#define B_ANIM_TRANSFORM 0x23
#define B_ANIM_WISHIWASHI_FISH 0x24
#define B_ANIM_ZYGARDE_CELL_SWIRL 0x25
#define B_ANIM_STRONG_WINDS_CONTINUE 0x26
#define B_ANIM_ELECTRIC_SURGE 0x27
#define B_ANIM_GRASSY_SURGE 0x28
#define B_ANIM_MISTY_SURGE 0x29
#define B_ANIM_PSYCHIC_SURGE 0x2A
#define B_ANIM_SEA_OF_FIRE 0x2B
#define B_ANIM_HEALING_SPARKLES 0x2C
#define B_ANIM_LUNAR_DANCE_HEAL 0x2D
#define B_ANIM_HEALING_WISH_HEAL 0x2E
#define B_ANIM_RED_PRIMAL_REVERSION 0x2F
#define B_ANIM_BLUE_PRIMAL_REVERSION 0x30
#define B_ANIM_GRASSY_TERRAIN_HEAL 0x31
#define B_ANIM_POWDER_EXPLOSION 0x32
#define B_ANIM_BEAK_BLAST_WARM_UP 0x33
#define B_ANIM_SHELL_TRAP_SET 0x34
#define B_ANIM_BERRY_EAT 0x35
#define B_ANIM_FOG_CONTINUES 0x36
#define B_ANIM_AQUA_RING_HEAL 0x37
#define B_ANIM_ELECTRIC_TERRAIN_ACTIVE 0x38
#define B_ANIM_GRASSY_TERRAIN_ACTIVE 0x39
#define B_ANIM_MISTY_TERRAIN_ACTIVE 0x3A
#define B_ANIM_PSYCHIC_TERRAIN_ACTIVE 0x3B
#define B_ANIM_BATON_PASS 0x3C
#define B_ANIM_DRAGON_TAIL_BLOW_AWAY 0x3D
#define B_ANIM_ZMOVE_ACTIVATE 0x3E
#define B_ANIM_MEGA_EVOLUTION 0x3F
#define B_ANIM_ULTRA_BURST 0x40
#define B_ANIM_LOAD_DEFAULT_BG 0x41
#define B_ANIM_LOAD_ABILITY_POP_UP 0x42
#define B_ANIM_DESTROY_ABILITY_POP_UP 0x43
#define B_ANIM_TOTEM_BOOST 0x44
#define B_ANIM_DYNAMAX_START 0x45
#define B_ANIM_DYNAMAX_ENERGY_SWIRL 0x46
#define B_ANIM_RAID_BATTLE_STORM 0x47
#define B_ANIM_RAID_BATTLE_ENERGY_BURST 0x48
#define B_ANIM_RAID_BATTLE_BLOW_AWAY 0x49
#define B_ANIM_G_MAX_VINE_LASH 0x4A
#define B_ANIM_G_MAX_WILDFIRE 0x4B
#define B_ANIM_G_MAX_CANNONADE 0x4C
#define B_ANIM_G_MAX_VOLCALITH 0x4D
#define B_ANIM_AI_ITEM_HEAL 0x4E
#define B_ANIM_HOOPA_RING_SPAWN 0x4F
#define B_ANIM_SPLINTER_DAMAGE 0x50

#define B_ANIM_TRANSFORM_MOVE 0xFF

// special animations table
#define B_ANIM_LVL_UP                   0x0
#define B_ANIM_SWITCH_OUT_PLAYER_MON    0x1
#define B_ANIM_SWITCH_OUT_OPPONENT_MON  0x2
#define B_ANIM_BALL_THROW               0x3
#define B_ANIM_SAFARI_BALL_THROW        0x4
#define B_ANIM_SUBSTITUTE_TO_MON        0x5
#define B_ANIM_MON_TO_SUBSTITUTE        0x6
#define B_ANIM_CRITICAL_CAPTURE_THROW   0x7

// status animation table
#define B_ANIM_STATUS_PSN               0x0
#define B_ANIM_STATUS_CONFUSION         0x1
#define B_ANIM_STATUS_BRN               0x2
#define B_ANIM_STATUS_INFATUATION       0x3
#define B_ANIM_STATUS_SLP               0x4
#define B_ANIM_STATUS_PRZ               0x5
#define B_ANIM_STATUS_FRZ               0x6
#define B_ANIM_STATUS_CURSED            0x7
#define B_ANIM_STATUS_NIGHTMARE         0x8
#define B_ANIM_STATUS_WRAPPED           0x9

#define GET_STAT_BUFF_ID(n)((n & 7))              // first three bits 0x1, 0x2, 0x4
#define GET_STAT_BUFF_VALUE_WITH_SIGN(n)((n & 0xF8))
#define GET_STAT_BUFF_VALUE(n)(((n >> 4) & 7))      // 0x8, 0x10, 0x20, 0x40
#define STAT_BUFF_NEGATIVE 0x80                     // 0x80, the sign bit

#define SET_STAT_BUFF_VALUE(n)(((s8)(((s8)(n) << 4)) & 0xF0))

#define SET_STATCHANGER(statId, stage, goesDown) (gBattleScripting.statChanger = (statId) + (stage << 4) + (goesDown << 7))

// used in many battle files, it seems as though Hisashi Sogabe wrote
// some sort of macro to replace the use of actually calling memset.
// Perhaps it was thought calling memset was much slower?

// The compiler wont allow us to locally declare ptr in this macro; some
// functions that invoke this macro will not match without this egregeous
// assumption about the variable names, so in order to avoid this assumption,
// we opt to pass the variables themselves, even though it is likely that
// Sogabe assumed the variables were named src and dest. Trust me: I tried to
// avoid assuming variable names, but the ROM just will not match without the
// assumptions. Therefore, these macros are bad practice, but I'm putting them
// here anyway.
#define MEMSET_ALT(data, c, size, var, dest)    \
{    \
    dest = (u8 *)data;    \
    for(var = 0; var < (u32)size; var++)    \
        dest[var] = c;    \
}    \

#define MEMCPY_ALT(data, dest, size, var, src)    \
{    \
    src = (u8 *)data;    \
    for(var = 0; var < (u32)size; var++)    \
        dest[var] = src[var];    \
}    \

// defines for the 'DoBounceEffect' function
#define BOUNCE_MON          0x0
#define BOUNCE_HEALTHBOX    0x1

struct BattleScripting
{
    s32 painSplitHp;
    s32 bideDmg;
    u8 multihitString[6];
    u8 dmgMultiplier;
    u8 twoTurnsMoveStringId;
    u8 animArg1;
    u8 animArg2;
    u16 tripleKickPower;
    u8 atk49_state;
    u8 bankWithAbility;
    u8 multihitMoveEffect;
    u8 bank;
    u8 animTurn;
    u8 animTargetsHit;
    u8 statChanger;
    u8 statAnimPlayed;		//0x1B
    u8 expStateTracker;
    u8 battleStyle;
    u8 atk6C_state;
    u8 learnMoveState;
    u8 field_20;
    u8 reshowMainState;
    u8 reshowHelperState;
    u8 field_23;
    u8 field_24;
    u8 multiplayerId;
};

extern struct BattleScripting gBattleScripting;

// functions

// battle_1
//void LoadBattleTextboxAndBackground(void);
//void LoadBattleEntryBackground(void);
//void ApplyPlayerChosenFrameToBattleMenu(void);
//bool8 LoadChosenBattleElement(u8 caseId);
void __attribute__((long_call)) DrawMainBattleBackground(void);
//void task00_0800F6FC(u8 taskId);
void __attribute__((long_call)) BattleMainCB2(void);

enum BackSprites
{
    BACK_PIC_RED,
    BACK_PIC_LEAF,
    BACK_PIC_BRENDAN,
    BACK_PIC_MAY,
    BACK_PIC_PRIMO,
    BACK_PIC_OLD_MAN,
};

// rom_80A5C6C
u8 GetBattlerSide(u8 bank);
u8 GetBattlerPosition(u8 bank);
u8 GetBattlerAtPosition(u8 bank);

struct BattleSpriteInfo
{
    u16 invisible : 1; // 0x1
    u16 lowHpSong : 1; // 0x2
    u16 behindSubstitute : 1; // 0x4
    u16 substituteOffScreen : 1; // 0x8
    u16 hpNumbersNoBars : 1; // 0x10
    u16 transformSpecies; //0x20054A6
};

struct BattleAnimationInfo
{
    u16 animArg; // to fill up later
    u8 field_2;
    u8 field_3;
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 field_7;
    u8 ballThrowCaseId;
    u8 introAnimActive : 1;
    u8 wildMonInvisible : 1;
    u8 field_9_x1C : 3;
    u8 field_9_x20 : 1;
    u8 field_9_x40 : 1;
    u8 field_9_x80 : 1;
    u8 numBallParticles;
    u8 field_B;
    s16 ballSubpx;
    u8 field_E;
    u8 field_F;
};

struct BattleHealthboxInfo
{
    u8 partyStatusSummaryShown : 1; // x1
    u8 healthboxIsBouncing : 1; // x2
    u8 battlerIsBouncing : 1; // x4
    u8 ballAnimActive : 1; // 0x8
    u8 statusAnimActive : 1; // x10
    u8 animFromTableActive : 1; // x20
    u8 specialAnimActive : 1; // x40
    u8 triedShinyMonAnim : 1; // x80
    u8 finishedShinyMonAnim : 1; // x1
	u8 opponentDrawPartyStatusSummaryDelay : 4;
    u8 bgmRestored : 1;
    u8 waitForCry : 1;
    u8 healthboxSlideInStarted : 1;
    u8 healthboxBounceSpriteId;
    u8 battlerBounceSpriteId;
    u8 animationState;
    u8 partyStatusDelayTimer;
    u8 matrixNum;
    u8 shadowSpriteId;
    u8 soundTimer;
    u8 introEndDelay;
    u8 field_A;
    u8 field_B;
};

struct BattleBarInfo
{
    u8 healthboxSpriteId;
    s32 maxValue;
    s32 oldValue;
    s32 receivedValue;
    s32 currValue;
};

struct BattleSpriteData
{
    struct BattleSpriteInfo* bankData;
    struct BattleHealthboxInfo* healthBoxesData;
    struct BattleAnimationInfo* animationData;
    struct BattleBarInfo* battleBars;
};

//#define gBattleSpritesDataPtr ((struct BattleSpriteData*) *((u32*) 0x2024018)) //extern struct BattleSpriteData* gBattleSpritesDataPtr;

#define BATTLE_BUFFER_LINK_SIZE 0x1000

extern u8 *gLinkBattleSendBuffer;
extern u8 *gLinkBattleRecvBuffer;

extern u8 *gUnknown_0202305C;
extern u8 *gUnknown_02023060;

// Move this somewhere else

#include "sprite.h"

struct MonSpritesGfx
{
    void* firstDecompressed; // ptr to the decompressed sprite of the first pokemon
    void* sprites[4];
    struct SpriteTemplate templates[4];
    struct SpriteFrameImage field_74[4][4];
    u8 field_F4[0x80];
    u8 *barFontGfx;
    void *field_178;
    void *field_17C;
};

enum EnduranceListings {ENUDRE_REG, ENDURE_STURDY, ENDURE_FOCUS_SASH};

extern u16 gBattle_BG0_X;
extern u16 gBattle_BG0_Y;
extern u16 gBattle_BG1_X;
extern u16 gBattle_BG1_Y;
extern u16 gBattle_BG2_X;
extern u16 gBattle_BG2_Y;
extern u16 gBattle_BG3_X;
extern u16 gBattle_BG3_Y;
extern u16 gBattle_WIN0H;
extern u16 gBattle_WIN0V;
extern u16 gBattle_WIN1H;
extern u16 gBattle_WIN1V;
extern u8 gDisplayedStringBattle[300];
extern u8 gBattleTextBuff1[TEXT_BUFF_ARRAY_COUNT];
extern u8 gBattleTextBuff2[TEXT_BUFF_ARRAY_COUNT];
extern u8 gBattleTextBuff3[TEXT_BUFF_ARRAY_COUNT];
extern u32 gBattleTypeFlags;
extern u8 gBattleTerrain;
extern u32 gUnknown_02022FF4;
extern u8 *gUnknown_0202305C;
extern u8 *gUnknown_02023060;
extern u8 gBattleBufferA[MAX_BATTLERS_COUNT][0x200];
extern u8 gBattleBufferB[MAX_BATTLERS_COUNT][0x200];
extern u8 gActiveBattler;
extern u32 gBattleExecBuffer;// gBattleControllerExecFlags
extern u8 gBattlersCount;
extern u16 gBattlerPartyIndexes[MAX_BATTLERS_COUNT];
extern u8 gBattlerPositions[MAX_BATTLERS_COUNT];
extern u8 gActionsByTurnOrder[MAX_BATTLERS_COUNT];
extern u8 gBanksByTurnOrder[MAX_BATTLERS_COUNT];
extern u8 gCurrentTurnActionNumber;
extern u8 gCurrentActionFuncId;
extern struct BattlePokemon gBattleMons[MAX_BATTLERS_COUNT];
extern u8 gBattlerSpriteIds[MAX_BATTLERS_COUNT];
extern u8 gCurrMovePos;
extern u8 gChosenMovePos;
extern u16 gCurrentMove;
extern u16 gChosenMove;
extern u16 gCalledMove;
extern s32 gBattleMoveDamage;
extern s32 gHpDealt;
extern s32 gTakenDmg[MAX_BATTLERS_COUNT];
extern u16 gLastUsedItem;
extern u8 gLastUsedAbility;
extern u8 gBankAttacker;
extern u8 gBankTarget;
extern u8 gBankFainted;
extern u8 gEffectBank;
extern u8 gStringBank;
extern u8 gPotentialItemEffectBattler;
extern u8 gAbsentBattlerFlags;
extern u8 gCritMultiplier;
extern u8 gMultiHitCounter;
extern const u8 *gBattlescriptCurrInstr;
extern u32 gUnusedBattleMainVar;
extern u8 gChosenActionByBank[MAX_BATTLERS_COUNT];
extern const u8 *gSelectionBattleScripts[MAX_BATTLERS_COUNT];
extern const u8 *gPalaceSelectionBattleScripts[MAX_BATTLERS_COUNT];
extern u16 gLastPrintedMoves[MAX_BATTLERS_COUNT];
extern u16 gLastUsedMoves[MAX_BATTLERS_COUNT];
extern u16 gLastLandedMoves[MAX_BATTLERS_COUNT];
extern u16 gLastHitByType[MAX_BATTLERS_COUNT];
extern u16 gLastResultingMoves[MAX_BATTLERS_COUNT];
extern u16 gLockedMoves[MAX_BATTLERS_COUNT];
extern u8 gLastHitBy[MAX_BATTLERS_COUNT];
extern u16 gChosenMovesByBanks[MAX_BATTLERS_COUNT];
extern u8 gMoveResultFlags;
extern u32 gHitMarker;
extern u8 gTakenDmgBanks[MAX_BATTLERS_COUNT];
extern u8 gUnusedFirstBattleVar2;
extern u16 gSideStatuses[2];
extern struct SideTimer gSideTimers[2];
extern u32 gStatuses3[MAX_BATTLERS_COUNT];
extern struct DisableStruct gDisableStructs[MAX_BATTLERS_COUNT];
extern u16 gPauseCounterBattle;
extern u16 gPaydayMoney;
extern u16 gRandomTurnNumber;
extern u8 gBattleCommunication[BATTLE_COMMUNICATION_ENTRIES_COUNT];
extern u8 gBattleOutcome;
extern struct ProtectStruct gProtectStructs[MAX_BATTLERS_COUNT];
extern struct SpecialStatus gSpecialStatuses[MAX_BATTLERS_COUNT];
extern u16 gBattleWeather;
extern struct WishFutureKnock gWishFutureKnock;
extern u16 gIntroSlideFlags;
extern u8 gSentPokesToOpponent[2];
extern u16 gDynamicBasePower;
extern u16 gExpShareExp;
extern struct BattleEnigmaBerry gEnigmaBerries[MAX_BATTLERS_COUNT];
extern struct BattleScripting gBattleScripting;
extern struct BattleStruct *gBattleStruct;
extern u8 *gLinkBattleSendBuffer;
extern u8 *gLinkBattleRecvBuffer;
extern struct BattleResources *gBattleResources;
extern u8 gActionSelectionCursor[MAX_BATTLERS_COUNT];
extern u8 gMoveSelectionCursor[MAX_BATTLERS_COUNT];
extern u8 gBattlerStatusSummaryTaskId[MAX_BATTLERS_COUNT];
extern u8 gBattlerInMenuId;
extern bool8 gDoingBattleAnim;
extern u32 gTransformedPersonalities[MAX_BATTLERS_COUNT];
extern u8 gPlayerDpadHoldFrames;
extern struct BattleSpriteData *gBattleSpritesDataPtr;
extern struct MonSpritesGfx *gMonSpritesGfxPtr;
extern struct BattleHealthboxInfo *gUnknown_020244D8;
extern struct BattleHealthboxInfo *gUnknown_020244DC;
extern u16 gBattleMovePower;
extern u16 gMoveToLearn;
extern u8 gBattleMonForms[MAX_BATTLERS_COUNT];

extern void (*gPreBattleCallback1)(void);
extern void (*gBattleMainFunc)(void);
extern struct BattleResults gBattleResults;
extern u8 gLeveledUpInBattle;
extern void (*gBattlerControllerFuncs[MAX_BATTLERS_COUNT])(void);
extern u8 gHealthboxSpriteIds[MAX_BATTLERS_COUNT];
extern u8 gMultiUsePlayerCursor;
extern u8 gNumberOfMovesToChoose;
extern u8 gBattleControllerData[MAX_BATTLERS_COUNT];

extern u8 gUnknown_2023E8A;

extern void (*gAnimScriptCallback)(void);
extern bool8 gAnimScriptActive;
extern u8 gAnimVisualTaskCount;
extern u8 gAnimSoundTaskCount;
extern struct DisableStruct *gAnimDisableStructPtr;
extern s32 gAnimMoveDmg;
extern u16 gAnimMovePower;
extern u8 gAnimFriendship;
extern u16 gWeatherMoveAnim;
extern s16 gBattleAnimArgs[/*ANIM_ARGS_COUNT*/];
extern u8 gAnimMoveTurn;
extern u8 gBattleAnimAttacker;
extern u8 gBattleAnimTarget;
extern u16 gAnimBattlerSpecies[MAX_BATTLERS_COUNT];
extern u8 gAnimCustomPanning;

extern u8 gBattleBuffersTransferData[0x100];

#define BATTLE_HISTORY ((struct BattleHistory*) (gBattleResources->battleHistory))
