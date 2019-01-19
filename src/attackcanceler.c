#include "defines.h"
#include "helper_functions.h"

//Update Choose Target or whatever to allow Magic Bounce to reflect, and then moves that target multiple pokes can continue from the attacker's side
//Make sure to restore the backup move effect post Status Z-move
//Z-Move Illusion
//De-activate Z-Move when used by Z-Instruct
//Make sure Powder stops Inferno Overdrive

#define BattleScript_MoveEnd (u8*) 0x81D694E
#define BattleScript_NoPPForMove (u8*) 0x81D8EA8
#define BattleScript_MagicCoatBounce (u8*) 0x81D8FAA
#define BattleScript_SnatchedMove (u8*) 0x81D8FC2
#define BattleScript_TookAttack (u8*) 0x81D938D

#define BattleScript_MoveUsedIsAsleep (u8*) 0x81D9029
#define BattleScript_MoveUsedWokeUp (u8*) 0x81D9036
#define BattleScript_MoveUsedIsFrozen (u8*) 0x81D9080
#define BattleScript_MoveUsedUnfroze (u8*) 0x81D908D
#define BattleScript_MoveUsedLoafingAround (u8*) 0x81D94F2
#define BattleScript_MoveUsedMustRecharge (u8*) 0x81D7342
#define BattleScript_MoveUsedIsDisabled (u8*) 0x81D8C4F
#define BattleScript_MoveUsedIsTaunted (u8*) 0x81D8ECA
#define BattleScript_MoveUsedIsImprisoned (u8*) 0x81D8F94
#define BattleScript_MoveUsedIsConfused (u8*) 0x81D90D3
#define BattleScript_MoveUsedIsConfusedNoMore (u8*) 0x81D9116
#define BattleScript_MoveUsedIsParalyzed (u8*) 0x81D90A1
#define BattleScript_TooScaredToMove (u8*) 0x81D9180
#define BattleScript_GhostGetOut (u8*) 0x81D9192
#define BattleScript_ImmobilizedByLove (u8*) 0x81D914A
#define BattleScript_MoveUsedIsInLove (u8*) 0x81D913D
#define BattleScript_BideStoringEnergy (u8*) 0x81D8BA9
#define BattleScript_BideAttack (u8*) 0x81D8BB4
#define BattleScript_BideNoEnergyToAttack (u8*) 0x81D8BFC
#define BattleScript_IgnoresWhileAsleep (u8*) 0x81D94DA
#define BattleScript_IgnoresAndUsesRandomMove (u8*) 0x81D94EA
#define BattleScript_IgnoresAndFallsAsleep (u8*) 0x81D9504
#define BattleScript_IgnoresAndHitsItself (u8*) 0x81D951B

extern u8 BattleScript_MagicBounce[];
extern u8 BattleScript_MoveUsedFlinched[]; //0x81D90B1
extern u8 BattleScript_MoveUsedGravityPrevents[];
extern u8 BattleScript_MoveUsedHealBlockPrevents[];
extern u8 BattleScript_MoveUsedThroatChopPrevents[];
extern u8 BattleScript_MoveUsedFailedPrimalWeather[];
extern u8 BattleScript_MoveUsedPsychicTerrainPrevents[];
extern u8 BattleScript_MoveUsedPowderPrevents[];
extern u8 BattleScript_StanceChangeToBlade[];
extern u8 BattleScript_StanceChangeToShield[];
extern u8 BattleScript_ZMoveActivateStatus[];
extern u8 BattleScript_ZMoveActivateDamaging[];

extern move_t GravityBanTable[];
extern move_t ParentalBondBanList[];
extern ability_t MoldBreakerIgnoreAbilities[];

extern u8* const gBattleScriptsForMoveEffects[];

extern bool8 ProtectAffects(u16 move, u8 bankAtk, u8 bankDef, bool8 set);
extern s32 CalculateBaseDamage(struct BattlePokemon* attacker, struct BattlePokemon* defender, u32 move, u16 sideStatus, u16 powerOverride, u8 effectivenessFlags, u8 typeOverride, u8 bankAtk, u8 bankDef, pokemon_t* party_data_atk, bool8 PartyCheck, bool8 IgnoreAttacker, bool8 CheckingConfusion);
extern u8 GetMoveTypeSpecial(u8 bankAtk, move_t);
extern s8 PriorityCalc(u8 bank, u8 action, u16 move);
extern u8 CheckMoveLimitations(u8 bank, u8 unusableMoves, u8 check);

void atk00_attackcanceler(void);
u8 AtkCanceller_UnableToUseMove(void);
u8 IsMonDisobedient(void);

const struct SpecialZMoves SpecialZMoveTable[] = 
{
	{PKMN_RAICHU_A,					ITEM_ALORAICHUIUM_Z, 	MOVE_STOKED_SPARKSURFER},
	{PKMN_DECIDUEYE,				ITEM_DECIDIUM_Z, 		MOVE_SINISTER_ARROW_RAID},
	{PKMN_EEVEE,					ITEM_EEVIUM_Z, 			MOVE_EXTREME_EVOBOOST},
	{PKMN_INCINEROAR,				ITEM_INCINIUM_Z, 		MOVE_MALICIOUS_MOONSAULT},
	{PKMN_KOMMO_O,					ITEM_KOMMONIUM_Z,		MOVE_CLANGOROUS_SOULBLAZE},
	{PKMN_LUNALA,					ITEM_LUNALIUM_Z,		MOVE_MENACING_MOONRAZE_MAELSTROM},
	{PKMN_NECROZMA_WINGS,			ITEM_LUNALIUM_Z,		MOVE_MENACING_MOONRAZE_MAELSTROM},
	{PKMN_LYCANROC,					ITEM_LYCANIUM_Z,		MOVE_SPLINTERED_STORMSHARDS},
	{PKMN_LYCANROC_N,				ITEM_LYCANIUM_Z,		MOVE_SPLINTERED_STORMSHARDS},
	{PKMN_LYCANROC_DUSK,			ITEM_LYCANIUM_Z,		MOVE_SPLINTERED_STORMSHARDS},
	{PKMN_MARSHADOW,				ITEM_MARSHADIUM_Z, 		MOVE_SOUL_STEALING_7_STAR_STRIKE},
	{PKMN_MEW,						ITEM_MEWNIUM_Z, 		MOVE_GENESIS_SUPERNOVA},
	{PKMN_MIMIKYU,					ITEM_MIMIKIUM_Z, 		MOVE_LETS_SNUGGLE_FOREVER},
	{PKMN_MIMIKYU_BUSTED,			ITEM_MIMIKIUM_Z, 		MOVE_LETS_SNUGGLE_FOREVER},
	{PKMN_PIKACHU,					ITEM_PIKANIUM_Z, 		MOVE_CATASTROPIKA},
	{PKMN_PIKACHU_CAP_ORIGINAL,		ITEM_PIKANIUM_Z, 		MOVE_CATASTROPIKA},
	{PKMN_PIKACHU_CAP_HOENN,		ITEM_PIKANIUM_Z, 		MOVE_CATASTROPIKA},
	{PKMN_PIKACHU_CAP_SINNOH,		ITEM_PIKANIUM_Z, 		MOVE_CATASTROPIKA},
	{PKMN_PIKACHU_CAP_UNOVA,		ITEM_PIKANIUM_Z, 		MOVE_CATASTROPIKA},
	{PKMN_PIKACHU_CAP_KALOS,		ITEM_PIKANIUM_Z, 		MOVE_CATASTROPIKA},
	{PKMN_PIKACHU_CAP_ALOLA,		ITEM_PIKANIUM_Z, 		MOVE_CATASTROPIKA},
	{PKMN_PIKACHU_CAP_PARTNER,		ITEM_PIKANIUM_Z, 		MOVE_CATASTROPIKA},
	{PKMN_PIKACHU_CAP_ORIGINAL,		ITEM_PIKASHUNIUM_Z, 	MOVE_10000000_VOLT_THUNDERBOLT},
	{PKMN_PIKACHU_CAP_HOENN,		ITEM_PIKASHUNIUM_Z, 	MOVE_10000000_VOLT_THUNDERBOLT},
	{PKMN_PIKACHU_CAP_SINNOH,		ITEM_PIKASHUNIUM_Z, 	MOVE_10000000_VOLT_THUNDERBOLT},
	{PKMN_PIKACHU_CAP_UNOVA,		ITEM_PIKASHUNIUM_Z, 	MOVE_10000000_VOLT_THUNDERBOLT},
	{PKMN_PIKACHU_CAP_KALOS,		ITEM_PIKASHUNIUM_Z, 	MOVE_10000000_VOLT_THUNDERBOLT},
	{PKMN_PIKACHU_CAP_ALOLA,		ITEM_PIKASHUNIUM_Z, 	MOVE_10000000_VOLT_THUNDERBOLT},
	{PKMN_PIKACHU_CAP_PARTNER,		ITEM_PIKASHUNIUM_Z, 	MOVE_10000000_VOLT_THUNDERBOLT},
	{PKMN_PRIMARINA,				ITEM_PRIMARIUM_Z, 		MOVE_OCEANIC_OPERETTA},
	{PKMN_SNORLAX,					ITEM_SNORLIUM_Z, 		MOVE_PULVERIZING_PANCAKE},
	{PKMN_SOLGALEO,					ITEM_SOLGANIUM_Z, 		MOVE_SEARING_SUNRAZE_SMASH},
	{PKMN_NECROZMA_MANE,			ITEM_SOLGANIUM_Z, 		MOVE_SEARING_SUNRAZE_SMASH},
	{PKMN_TAPU_KOKO,				ITEM_TAPUNIUM_Z, 		MOVE_GUARDIAN_OF_ALOLA},
	{PKMN_TAPU_BULU,				ITEM_TAPUNIUM_Z, 		MOVE_GUARDIAN_OF_ALOLA},
	{PKMN_TAPU_LELE,				ITEM_TAPUNIUM_Z, 		MOVE_GUARDIAN_OF_ALOLA},
	{PKMN_TAPU_FINI,				ITEM_TAPUNIUM_Z, 		MOVE_GUARDIAN_OF_ALOLA},
	{PKMN_NECROZMA_ULTRA,			ITEM_ULTRANECROZIUM_Z, 	MOVE_LIGHT_THAT_BURNS_THE_SKY},
	{0xFFFF,						0xFFFF, 				0xFFFF}
};

enum
{
	CANCELLER_FLAGS,
	CANCELLER_ASLEEP,
	CANCELLER_FROZEN,
	CANCELLER_TRUANT,
	CANCELLER_RECHARGE,
	CANCELLER_FLINCH,
	CANCELLER_GRAVITY,
	CANCELLER_DISABLED,
	CANCELLER_HEAL_BLOCKED,
	CANCELLER_THROAT_CHOP,
	CANCELLER_TAUNTED,
	CANCELLER_IMPRISONED,
	CANCELLER_CONFUSED,
	CANCELLER_PARALYSED,
	CANCELLER_GHOST,
	CANCELLER_IN_LOVE,
	CANCELLER_STANCE_CHANGE,
	CANCELLER_BIDE,
	CANCELLER_THAW,
	CANCELLER_Z_MOVES,
	CANCELLER_GRAVITY_Z_MOVES,
	CANCELLER_POWDER,
	CANCELLER_PRIMAL_WEATHER,
	CANCELLER_PSYCHIC_TERRAIN,
	CANCELLER_END,
};

void atk00_attackcanceler(void)
{
    int i;

    if (gBattleOutcome != 0)
    {
        gCurrentActionFuncId = ACTION_FINISHED;
        return;
    }
    if ((gBattleMons[gBankAttacker].hp == 0 || (gStatuses3[gBankAttacker] & STATUS3_SKY_DROP_TARGET)) && !(gHitMarker & HITMARKER_NO_ATTACKSTRING))
    {
        gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
        gBattlescriptCurrInstr = BattleScript_MoveEnd;
        return;
    }
	
	if (ABILITY(gBankAttacker) == ABILITY_MOLDBREAKER
	||  ABILITY(gBankAttacker) == ABILITY_TERAVOLT
	||  ABILITY(gBankAttacker) == ABILITY_TURBOBLAZE) {
		for (int i = 0; i < gBattlersCount; ++i) {
			if (i == gBankAttacker) continue;
			
			if (CheckTableForAbility(ABILITY(i), MoldBreakerIgnoreAbilities)) {
				DisabledMoldBreakerAbilities[i] = ABILITY(i); //Temporarily disable all relevant abilities on the field
				gBattleMons[i].ability = 0;
			}
		}
	}
	
	gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gBankAttacker, gCurrentMove);
    if (AtkCanceller_UnableToUseMove())
        return;
    else if (AbilityBattleEffects(ABILITYEFFECT_MOVES_BLOCK, gBankTarget, 0, 0, 0))
        return;
	
	else if (!ParentalBondOn
	&& ABILITY(gBankAttacker) == ABILITY_PARENTALBOND 
	&& SPLIT(gCurrentMove) != SPLIT_STATUS
	&& !CheckTableForMove(gCurrentMove, ParentalBondBanList)
	&& !IsTwoTurnsMove(gCurrentMove)
	&& gBattleMoves[gCurrentMove].effect != EFFECT_MULTI_HIT
	&& gBattleMoves[gCurrentMove].effect != EFFECT_DOUBLE_HIT
	&& !(gAbsentBattlerFlags & gBitTable[gBankTarget]))
	{
		if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {
			switch (gBattleMoves[gCurrentMove].target) {
				case MOVE_TARGET_BOTH:
					if (CountAliveMons(2) <= 1) { //Check for single target
						ParentalBondOn = 2;
						gMultiHitCounter = 2;			
					}
					break;
				case MOVE_TARGET_FOES_AND_ALLY:
					if (CountAliveMons(1) +  CountAliveMons(2) <= 2) { //Count mons on both sides; ignore attacker
						ParentalBondOn = 2;
						gMultiHitCounter = 2;			
					}
					break;
				default:
					ParentalBondOn = 2;
					gMultiHitCounter = 2;
			}
		}
		
		else {
			ParentalBondOn = 2;
			gMultiHitCounter = 2;
		}
	}
	
    if (!gBattleMons[gBankAttacker].pp[gCurrMovePos] 
	&& gCurrentMove != MOVE_STRUGGLE 
	&& !(gHitMarker & (HITMARKER_x800000 | HITMARKER_NO_ATTACKSTRING))
    && !(gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS)
	&& !DancerByte)
    {
        gBattlescriptCurrInstr = BattleScript_NoPPForMove;
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        return;
    }

    gHitMarker &= ~(HITMARKER_x800000);

    if (!(gHitMarker & HITMARKER_OBEYS) && !(gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS))
    {
        switch (IsMonDisobedient()) {
			case 0:
				break;
			case 2:
				gHitMarker |= HITMARKER_OBEYS;
				return;
			default:
				gMoveResultFlags |= MOVE_RESULT_MISSED;
				return;
        }
    }

    gHitMarker |= HITMARKER_OBEYS;

    if (!MoveBounceInProgress && gBattleMoves[gCurrentMove].flags & FLAG_MAGIC_COAT_AFFECTED)
	{
		if (gBattleMoves[gCurrentMove].target == MOVE_TARGET_OPPONENTS_FIELD)
		{
			if (gProtectStructs[SIDE(gBankAttacker) ^ BIT_SIDE].bounceMove)
			{
				PressurePPLose(gBankAttacker, SIDE(gBankAttacker) ^ BIT_SIDE, MOVE_MAGICCOAT);
				gProtectStructs[SIDE(gBankAttacker) ^ BIT_SIDE].bounceMove = 0;
				MoveBounceInProgress = TRUE;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MagicCoatBounce;
				return;
			}
			else if (gProtectStructs[PARTNER(SIDE(gBankAttacker)) ^ BIT_SIDE].bounceMove)
			{
				PressurePPLose(gBankAttacker, PARTNER(SIDE(gBankAttacker)) ^ BIT_SIDE, MOVE_MAGICCOAT);
				gProtectStructs[PARTNER(SIDE(gBankAttacker)) ^ BIT_SIDE].bounceMove = 0;
				MoveBounceInProgress = TRUE;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MagicCoatBounce;
			}
			else if (ABILITY(SIDE(gBankAttacker) ^ BIT_SIDE) == ABILITY_MAGICBOUNCE && !(gStatuses3[SIDE(gBankAttacker) ^ BIT_SIDE] & STATUS3_SEMI_INVULNERABLE))
			{
				MoveBounceInProgress = TRUE;
				gLastUsedAbility = ABILITY_MAGICBOUNCE;
				RecordAbilityBattle(SIDE(gBankAttacker) ^ BIT_SIDE, gLastUsedAbility);
				gBattleScripting->bank = SIDE(gBankAttacker) ^ BIT_SIDE;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MagicBounce;
			}
			else if (ABILITY(PARTNER(SIDE(gBankAttacker)) ^ BIT_SIDE) == ABILITY_MAGICBOUNCE && !(gStatuses3[PARTNER(SIDE(gBankAttacker)) ^ BIT_SIDE] & STATUS3_SEMI_INVULNERABLE))
			{
				MoveBounceInProgress = TRUE;
				gLastUsedAbility = ABILITY_MAGICBOUNCE;
				RecordAbilityBattle(PARTNER(SIDE(gBankAttacker)) ^ BIT_SIDE, gLastUsedAbility);
				gBattleScripting->bank = SIDE(gBankAttacker) ^ BIT_SIDE;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MagicBounce;
			}
		}
		else
		{
			if (gProtectStructs[gBankTarget].bounceMove)
			{
				PressurePPLose(gBankAttacker, gBankTarget, MOVE_MAGICCOAT);
				gProtectStructs[gBankTarget].bounceMove = 0;
				MoveBounceInProgress = TRUE;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MagicCoatBounce;
				return;
			}
			else if (ABILITY(gBankTarget) == ABILITY_MAGICBOUNCE && !(gStatuses3[gBankTarget] & STATUS3_SEMI_INVULNERABLE))
			{
				MoveBounceInProgress = TRUE;
				gLastUsedAbility = ABILITY_MAGICBOUNCE;
				RecordAbilityBattle(gBankTarget, gLastUsedAbility);
				gBattleScripting->bank = gBankTarget;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_MagicBounce;
			}
		}
	}
	else
		MoveBounceInProgress = FALSE;

    for (i = 0; i < gBattlersCount; i++)
    {
        if ((gProtectStructs[gBanksByTurnOrder[i]].stealMove) && gBattleMoves[gCurrentMove].flags & FLAG_SNATCH_AFFECTED)
        {
            PressurePPLose(gBankAttacker, gBanksByTurnOrder[i], MOVE_SNATCH);
            gProtectStructs[gBanksByTurnOrder[i]].stealMove = 0;
            gBattleScripting->bank = gBanksByTurnOrder[i];
            BattleScriptPushCursor();
            gBattlescriptCurrInstr = BattleScript_SnatchedMove;
            return;
        }
    }

    if (gSpecialStatuses[gBankTarget].lightningRodRedirected)
    {
        gSpecialStatuses[gBankTarget].lightningRodRedirected = 0;
        gLastUsedAbility = ABILITY(gBankTarget);
        BattleScriptPushCursor();
        gBattlescriptCurrInstr = BattleScript_TookAttack;
        RecordAbilityBattle(gBankTarget, gLastUsedAbility);
    }
    else if (ProtectAffects(gCurrentMove, gBankAttacker, gBankTarget, FALSE)
     && (gCurrentMove != MOVE_CURSE || IsOfType(gBankAttacker, TYPE_GHOST))
     && ((!IsTwoTurnsMove(gCurrentMove) || (gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS))))
    {
        CancelMultiTurnMoves(gBankAttacker);
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gLastLandedMoves[gBankTarget] = 0;
        gLastHitByType[gBankTarget] = 0;
        gBattleCommunication[6] = 1;
        gBattlescriptCurrInstr++;
    }
    else
    {
        gBattlescriptCurrInstr++;
    }
}

u8 AtkCanceller_UnableToUseMove(void)
{
	int i;
    u8 effect = 0;
	
    do
    {
        switch (gBattleStruct->atkCancellerTracker) {
        case CANCELLER_FLAGS: // flags clear
            gBattleMons[gBankAttacker].status2 &= ~(STATUS2_DESTINY_BOND);
            gStatuses3[gBankAttacker] &= ~(STATUS3_GRUDGE);
            gBattleStruct->atkCancellerTracker++;
            break;
		
        case CANCELLER_ASLEEP: // check being asleep
            if (gBattleMons[gBankAttacker].status1 & STATUS1_SLEEP)
            {
                u8 toSub;
                if (ABILITY(gBankAttacker) == ABILITY_EARLYBIRD)
                    toSub = 2;
                else
                    toSub = 1;
                
				if ((gBattleMons[gBankAttacker].status1 & STATUS1_SLEEP) < toSub)
                    gBattleMons[gBankAttacker].status1 &= ~(STATUS1_SLEEP);
                else
                    gBattleMons[gBankAttacker].status1 -= toSub;
                
				if (gBattleMons[gBankAttacker].status1 & STATUS1_SLEEP)
                {
                    if (gCurrentMove != MOVE_SNORE && gCurrentMove != MOVE_SLEEPTALK)
                    {
                        gBattlescriptCurrInstr = BattleScript_MoveUsedIsAsleep;
                        gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                        effect = 2;
                    }
					else
						effect = 2;
                }
                else
                {
                    gBattleMons[gBankAttacker].status2 &= ~(STATUS2_NIGHTMARE);
                    BattleScriptPushCursor();
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    gBattlescriptCurrInstr = BattleScript_MoveUsedWokeUp;
                    effect = 2;
                }
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
        case CANCELLER_FROZEN:
            if (gBattleMons[gBankAttacker].status1 & STATUS1_FREEZE)
            {
                if (umodsi(Random(), 5))
                {
                    if (gBattleMoves[gCurrentMove].effect != EFFECT_THAW_HIT) // unfreezing via a move effect happens in case 13
                    {
                        gBattlescriptCurrInstr = BattleScript_MoveUsedIsFrozen;
                        gHitMarker |= HITMARKER_NO_ATTACKSTRING;
                    }
                    else
                    {
                        gBattleStruct->atkCancellerTracker++;
                        break;
                    }
                }
                else // unfreeze
                {
                    gBattleMons[gBankAttacker].status1 &= ~(STATUS1_FREEZE);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_MoveUsedUnfroze;
                    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                }
                effect = 2;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
        case CANCELLER_TRUANT: // truant
            if (gBattleMons[gBankAttacker].ability == ABILITY_TRUANT && gDisableStructs[gBankAttacker].truantCounter)
            {
                CancelMultiTurnMoves(gBankAttacker);
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
                gMoveResultFlags |= MOVE_RESULT_MISSED;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
        case CANCELLER_RECHARGE: // recharge
            if (gBattleMons[gBankAttacker].status2 & STATUS2_RECHARGE)
            {
                gBattleMons[gBankAttacker].status2 &= ~(STATUS2_RECHARGE);
                gDisableStructs[gBankAttacker].rechargeCounter = 0;
                CancelMultiTurnMoves(gBankAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedMustRecharge;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
        case CANCELLER_FLINCH: // flinch
            if (gBattleMons[gBankAttacker].status2 & STATUS2_FLINCHED)
            {
                gBattleMons[gBankAttacker].status2 &= ~(STATUS2_FLINCHED);
                gProtectStructs[gBankAttacker].flinchImmobility = 1;
                CancelMultiTurnMoves(gBankAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedFlinched;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
        case CANCELLER_DISABLED: // disabled move
            if (gDisableStructs[gBankAttacker].disabledMove == gCurrentMove && gDisableStructs[gBankAttacker].disabledMove != 0 && !ZMoveData->active)
            {
                gProtectStructs[gBankAttacker].usedDisabledMove = 1;
                gBattleScripting->bank = gBankAttacker;
                CancelMultiTurnMoves(gBankAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsDisabled;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
        case CANCELLER_HEAL_BLOCKED:
            if (HealBlockTimers[gBankAttacker] && CheckHealingMove(gCurrentMove) && !ZMoveData->active)
            {
                gBattleScripting->bank = gBankAttacker;
                CancelMultiTurnMoves(gBankAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedHealBlockPrevents;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
		case CANCELLER_THROAT_CHOP:
            if (ThroatChopTimers[gBankAttacker] && CheckSoundMove(gCurrentMove) && !ZMoveData->active)
            {
                gBattleScripting->bank = gBankAttacker;
                CancelMultiTurnMoves(gBankAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedThroatChopPrevents;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
		case CANCELLER_GRAVITY:
            if (GravityTimer && CheckTableForMove(gCurrentMove, GravityBanTable) && !ZMoveData->active) //Gravity stops Z-Moves, so there will be
            {																							//a second check later on
                gBattleScripting->bank = gBankAttacker;
                CancelMultiTurnMoves(gBankAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedGravityPrevents;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
        case CANCELLER_TAUNTED: // taunt
            if (gDisableStructs[gBankAttacker].tauntTimer1 && SPLIT(gCurrentMove) == SPLIT_STATUS && !ZMoveData->active)
            {
                gProtectStructs[gBankAttacker].usedTauntedMove = 1;
                CancelMultiTurnMoves(gBankAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsTaunted;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
        case CANCELLER_IMPRISONED: // imprisoned
            if (IsImprisoned(gBankAttacker, gCurrentMove) && !ZMoveData->active)
            {
                gProtectStructs[gBankAttacker].usedImprisionedMove = 1;
                CancelMultiTurnMoves(gBankAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsImprisoned;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
        case CANCELLER_CONFUSED: // confusion
            if (gBattleMons[gBankAttacker].status2 & STATUS2_CONFUSION)
            {
                gBattleMons[gBankAttacker].status2--;
                if (gBattleMons[gBankAttacker].status2 & STATUS2_CONFUSION)
                {
					#ifdef OLD_CONFUSION_CHANCE
                    if (Random() & 1) //50 %
                    {
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                        BattleScriptPushCursor();
                    }
					#else
                    if (umodsi(Random(), 100) > 33) //33 %
                    {
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                        BattleScriptPushCursor();
                    }
					#endif
                    else // confusion dmg
                    {
                        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                        gBankTarget = gBankAttacker;
						gBattleMoveDamage = CalculateBaseDamage(&gBattleMons[gBankAttacker], &gBattleMons[gBankAttacker], MOVE_POUND,
                                            0, 40,
											0,
                                            TYPE_MYSTERY, gBankAttacker, gBankTarget,
											GetBankPartyData(gBankAttacker), FALSE, FALSE, TRUE);
                        gProtectStructs[gBankAttacker].confusionSelfDmg = 1;
                        gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                    }
                    gBattlescriptCurrInstr = BattleScript_MoveUsedIsConfused;
                }
                else // snapped out of confusion
                {
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_MoveUsedIsConfusedNoMore;
                }
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
	
        case CANCELLER_PARALYSED: // paralysis
            if ((gBattleMons[gBankAttacker].status1 & STATUS1_PARALYSIS) && umodsi(Random(), 4) == 0)
            {
                gProtectStructs[gBankAttacker].prlzImmobility = 1;
                CancelMultiTurnMoves(gBankAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsParalyzed;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
		case CANCELLER_GHOST: //Lavender Town Ghost battle
		#ifndef NO_GHOST_BATTLES
			if ((gBattleTypeFlags & (BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_GHOST)) == BATTLE_TYPE_GHOST) 
			{
				if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
                    gBattlescriptCurrInstr = BattleScript_TooScaredToMove;
				else
					gBattlescriptCurrInstr = BattleScript_GhostGetOut;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
		#endif
			break;
		
        case CANCELLER_IN_LOVE: // infatuation
            if (gBattleMons[gBankAttacker].status2 & STATUS2_INFATUATION)
            {
                gBattleScripting->bank = CountTrailingZeroBits((gBattleMons[gBankAttacker].status2 & STATUS2_INFATUATION) >> 0x10);
                if (Random() & 1)
                {
                    BattleScriptPushCursor();
                }
                else
                {
                    BattleScriptPush(BattleScript_ImmobilizedByLove);
                    gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                    gProtectStructs[gBankAttacker].loveImmobility = 1;
                    CancelMultiTurnMoves(gBankAttacker);
                }
                gBattlescriptCurrInstr = BattleScript_MoveUsedIsInLove;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
		case CANCELLER_STANCE_CHANGE:
			if (gBattleMons[gBankAttacker].ability == ABILITY_STANCECHANGE && !(gBattleMons[gBankAttacker].status2 & STATUS2_TRANSFORMED)) {
				switch (gBattleMons[gBankAttacker].species) {
					case PKMN_AEGISLASH:
						if (SPLIT(gCurrentMove) != SPLIT_STATUS) {
							DoFormChange(gBankAttacker, PKMN_AEGISLASH_BLADE, FALSE);
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_StanceChangeToBlade;
						}
						break;
					
					case PKMN_AEGISLASH_BLADE:
						if (gCurrentMove == MOVE_KINGSSHIELD) {
							DoFormChange(gBankAttacker, PKMN_AEGISLASH, FALSE);
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_StanceChangeToShield;
						}
						break;
				}
			}
			gBattleStruct->atkCancellerTracker++;
			break;
		
        case CANCELLER_BIDE: // bide
            if (gBattleMons[gBankAttacker].status2 & STATUS2_BIDE)
            {
                gBattleMons[gBankAttacker].status2 -= 0x100;
                if (gBattleMons[gBankAttacker].status2 & STATUS2_BIDE)
                {
                    gBattlescriptCurrInstr = BattleScript_BideStoringEnergy;
                }
                else
                {
                    gBattleMons[gBankAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);
                    if (gTakenDmg[gBankAttacker])
                    {
                        gCurrentMove = MOVE_BIDE;
                        gBattleScripting->bideDmg = gTakenDmg[gBankAttacker] * 2;
                        gBankTarget = gTakenDmgBanks[gBankAttacker];
                        if (gAbsentBattlerFlags & gBitTable[gBankTarget])
                            gBankTarget = GetMoveTarget(MOVE_BIDE, 1);
                        gBattlescriptCurrInstr = BattleScript_BideAttack;
                    }
                    else
                    {
                        gBattlescriptCurrInstr = BattleScript_BideNoEnergyToAttack;
                    }
                }
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
        case CANCELLER_THAW: // move thawing
            if (gBattleMons[gBankAttacker].status1 & STATUS1_FREEZE)
            {
                if (gBattleMoves[gCurrentMove].effect == EFFECT_THAW_HIT)
                {
                    gBattleMons[gBankAttacker].status1 &= ~(STATUS1_FREEZE);
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_MoveUsedUnfroze;
                    gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                }
                effect = 2;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
		case CANCELLER_Z_MOVES:
			if (ZMoveData->toBeUsed[gBankAttacker]) {
				ZMoveData->used[gBankAttacker] = TRUE;
				ZMoveData->toBeUsed[gBankAttacker] = 0;
				ZMoveData->active = TRUE;
				ZMoveData->state = 1;
				ZMoveData->partyIndex[gBankAttacker] |= gBitTable[gBattlerPartyIndexes[gBankAttacker]]; //Stops Rayquaza from Mega Evolving
				if (SIDE(gBankAttacker) == B_SIDE_PLAYER && !(gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER | BATTLE_TYPE_MULTI))) //In team Battles, both players can use Z-moves
				{
					ZMoveData->used[PARTNER(gBankAttacker)] = TRUE;
					ZMoveData->toBeUsed[PARTNER(gBankAttacker)] = 0;
					ZMoveData->partyIndex[SIDE(gBankAttacker)] |= gBitTable[gBattlerPartyIndexes[PARTNER(gBankAttacker)]]; //Stops Rayquaza from Mega Evolving
				}
				else if (SIDE(gBankAttacker) == B_SIDE_OPPONENT && !(gBattleTypeFlags & (BATTLE_TYPE_TWO_OPPONENTS | BATTLE_TYPE_MULTI)))
				{
					ZMoveData->used[PARTNER(gBankAttacker)] = TRUE;
					ZMoveData->toBeUsed[PARTNER(gBankAttacker)] = 0;
					ZMoveData->partyIndex[SIDE(gBankAttacker)] |= gBitTable[gBattlerPartyIndexes[PARTNER(gBankAttacker)]]; //Stops Rayquaza from Mega Evolving
				}
			}
			
			if (ZMoveData->active) {
				if (SPLIT(gCurrentMove) == SPLIT_STATUS) 
				{
					if (!ZMoveData->effectApplied) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_ZMoveActivateStatus;
						ZMoveData->effect = gBattleMoves[gCurrentMove].z_move_effect; 
						ZMoveData->effectApplied = TRUE;
					}
					else {
						gBattleStruct->atkCancellerTracker++;
						break;
					}
				}
				else 
				{
					for (i = 0; SpecialZMoveTable[i].item != 0xFFFF; ++i) {
						if (SpecialZMoveTable[i].item == ITEM(gBankAttacker)) //No need to check for correct species here as the check;
							gRandomMove = SpecialZMoveTable[i].move;		  //it should already have been carried out during move selection.
					}
							
					if (SpecialZMoveTable[i].item == 0xFFFF) { //No special Z-Move
						u16 moveReplaced = gBattleMons[gBankAttacker].moves[gCurrMovePos];
						u8 moveType = gBattleMoves[moveReplaced].type;
						if (moveType < TYPE_FIRE)
							gRandomMove = MOVE_BREAKNECK_BLITZ_P + (moveType * 2) + SPLIT(moveReplaced);
						else
							gRandomMove = MOVE_BREAKNECK_BLITZ_P + ((moveType - 1) * 2) + SPLIT(moveReplaced);
					}
					gCurrentMove = gChosenMove = gRandomMove;
					gBattleCommunication[MOVE_EFFECT_BYTE] = 0; //Remove secondary effects
					gBattleStruct->dynamicMoveType = GetMoveTypeSpecial(gBankAttacker, gCurrentMove);
					gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_ZMoveActivateDamaging;
					ZMoveData->state = 0;
				}
				effect = 1;
			}
			gBattleStruct->atkCancellerTracker++;
			break;
		
		case CANCELLER_GRAVITY_Z_MOVES:
            if (GravityTimer && CheckTableForMove(gCurrentMove, GravityBanTable) && ZMoveData->active) //Gravity stops Z-Moves after they apply their effect
            {																						
                gBattleScripting->bank = gBankAttacker;
                CancelMultiTurnMoves(gBankAttacker);
                gBattlescriptCurrInstr = BattleScript_MoveUsedGravityPrevents;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
			
		case CANCELLER_POWDER:
            if ((PowderByte & gBitTable[gBankAttacker]) && (gBattleStruct->dynamicMoveType == TYPE_FIRE))
            {
                gBattleMoveDamage = gBattleMons[gBankAttacker].maxHP / 4;
				gBattlescriptCurrInstr = BattleScript_MoveUsedPowderPrevents;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
		case CANCELLER_PRIMAL_WEATHER:
			if (WEATHER_HAS_EFFECT) {
				if ((gBattleStruct->dynamicMoveType == TYPE_FIRE && gBattleWeather & WEATHER_RAIN_PRIMAL)
				||  (gBattleStruct->dynamicMoveType == TYPE_WATER && gBattleWeather & WEATHER_SUN_PRIMAL))
				{
					CancelMultiTurnMoves(gBankAttacker);
					gBattlescriptCurrInstr = BattleScript_MoveUsedFailedPrimalWeather;
					gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
					effect = 1;
				}
			}
            gBattleStruct->atkCancellerTracker++;
            break;
		
		case CANCELLER_PSYCHIC_TERRAIN:
		    if (TerrainType == PSYCHIC_TERRAIN
            && CheckGrounding(gBankTarget)
			&& gBankAttacker != gBankTarget
            && PriorityCalc(gBankAttacker, TRUE, gCurrentMove)
            && gBankAttacker != gBankTarget)
            {
                CancelMultiTurnMoves(gBankAttacker);
				gBattlescriptCurrInstr = BattleScript_MoveUsedPsychicTerrainPrevents;
                gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
                effect = 1;
            }
            gBattleStruct->atkCancellerTracker++;
            break;
		
        case CANCELLER_END:
            break;
        }

    } while (gBattleStruct->atkCancellerTracker != CANCELLER_END && effect == 0);

    if (effect == 2)
    {
        gActiveBattler = gBankAttacker;
        EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
        MarkBufferBankForExecution(gActiveBattler);
    }
    return effect;
}

u8 IsMonDisobedient(void)
{
    s32 rnd;
    s32 calc;
    u8 obedienceLevel = 0;

    if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_POKE_DUDE | BATTLE_TYPE_FRONTIER))
        return 0;
	else if (gBattleTypeFlags & (BATTLE_TYPE_INGAME_PARTNER) && GetBattlerPosition(gBankAttacker) == B_POSITION_PLAYER_RIGHT)
		return 0;
	else if (gBattleTypeFlags & (BATTLE_TYPE_MOCK_BATTLE) && GetBattlerPosition(gBankAttacker) == B_POSITION_PLAYER_LEFT)
		return 0;
    else if (SIDE(gBankAttacker) == B_SIDE_OPPONENT)
        return 0;
	else if (InstructInProgress || DancerByte || ZMoveData->active)
		return 0;

	#ifndef OBEDIENCE_CHECK_FOR_PLAYER_ORIGINAL_POKEMON
    if (!IsOtherTrainer(gBattleMons[gBankAttacker].otId, gBattleMons[gBankAttacker].otName))
        return 0;
	#endif
	
    if (FlagGet(FLAG_BADGE08_GET))
        return 0;
	else if (FlagGet(FLAG_BADGE07_GET))
        obedienceLevel = BADGE_7_OBEDIENCE_LEVEL;
	else if (FlagGet(FLAG_BADGE06_GET))
        obedienceLevel = BADGE_6_OBEDIENCE_LEVEL;
	else if (FlagGet(FLAG_BADGE05_GET))
        obedienceLevel = BADGE_5_OBEDIENCE_LEVEL;
	else if (FlagGet(FLAG_BADGE04_GET))
        obedienceLevel = BADGE_4_OBEDIENCE_LEVEL;
	else if (FlagGet(FLAG_BADGE03_GET))
        obedienceLevel = BADGE_3_OBEDIENCE_LEVEL;
	else if (FlagGet(FLAG_BADGE02_GET))
        obedienceLevel = BADGE_2_OBEDIENCE_LEVEL;
	else if (FlagGet(FLAG_BADGE01_GET))
        obedienceLevel = BADGE_1_OBEDIENCE_LEVEL;
	else
		obedienceLevel = BASE_OBEDIENCE_LEVEL;

    if (gBattleMons[gBankAttacker].level <= obedienceLevel)
        return 0;
    rnd = (Random() & 255);
    calc = (gBattleMons[gBankAttacker].level + obedienceLevel) * rnd >> 8;
    if (calc < obedienceLevel)
        return 0;

    // is not obedient
    if (gCurrentMove == MOVE_RAGE)
        gBattleMons[gBankAttacker].status2 &= ~(STATUS2_RAGE);
    if (gBattleMons[gBankAttacker].status1 & STATUS1_SLEEP && (gCurrentMove == MOVE_SNORE || gCurrentMove == MOVE_SLEEPTALK))
    {
        gBattlescriptCurrInstr = BattleScript_IgnoresWhileAsleep;
        return 1;
    }

    rnd = (Random() & 255);
    calc = (gBattleMons[gBankAttacker].level + obedienceLevel) * rnd >> 8;
    if (calc < obedienceLevel && gCurrentMove != MOVE_FOCUSPUNCH && gCurrentMove != MOVE_BEAKBLAST && gCurrentMove != MOVE_SHELLTRAP)
    {
        calc = CheckMoveLimitations(gBankAttacker, gBitTable[gCurrMovePos], 0xFF);
        if (calc == 0xF) // all moves cannot be used
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = Random() & 3;
            gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
            return 1;
        }
        else // use a random move
        {
            do
            {
                gCurrMovePos = gChosenMovePos = Random() & 3;
            } while (gBitTable[gCurrMovePos] & calc);

            gRandomMove = gBattleMons[gBankAttacker].moves[gCurrMovePos];
            gBattlescriptCurrInstr = BattleScript_IgnoresAndUsesRandomMove;
            gBankTarget = GetMoveTarget(gRandomMove, 0);
            gHitMarker |= HITMARKER_x200000;
            return 2;
        }
    }
    else
    {
        obedienceLevel = gBattleMons[gBankAttacker].level - obedienceLevel;

        calc = (Random() & 255);
        if (calc < obedienceLevel && CanBePutToSleep(gBankAttacker))
        {
            // try putting asleep
            int i;
            for (i = 0; i < gBattlersCount; i++)
            {
                if (gBattleMons[i].status2 & STATUS2_UPROAR)
                    break;
            }
            if (i == gBattlersCount)
            {
                gBattlescriptCurrInstr = BattleScript_IgnoresAndFallsAsleep;
                return 1;
            }
        }
        calc -= obedienceLevel;
        if (calc < obedienceLevel)
        {
            gBattleMoveDamage = CalculateBaseDamage(&gBattleMons[gBankAttacker], &gBattleMons[gBankAttacker], MOVE_POUND,
													0, 40,
													0,
													TYPE_MYSTERY, gBankAttacker, gBankTarget,
													GetBankPartyData(gBankAttacker), FALSE, FALSE, TRUE);
            gBankTarget = gBankAttacker;
            gBattlescriptCurrInstr = BattleScript_IgnoresAndHitsItself;
            gHitMarker |= HITMARKER_UNABLE_TO_USE_MOVE;
            return 2;
        }
        else
        {
            gBattleCommunication[MULTISTRING_CHOOSER] = Random() & 3;
            gBattlescriptCurrInstr = BattleScript_MoveUsedLoafingAround;
            return 1;
        }
    }
}