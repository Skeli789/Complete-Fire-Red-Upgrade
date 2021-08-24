#include "defines.h"
#include "defines_battle.h"
#include "../include/string_util.h"
#include "../include/constants/abilities.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/species.h"

#include "../include/new/ability_tables.h"
#include "../include/new/ability_util.h"
#include "../include/new/move_tables.h"
#include "../include/new/battle_util.h"

extern const u8 gAbilityNames[][ABILITY_NAME_LENGTH + 1];
extern const u8 gText_AbilityName_AirLock[];
extern const u8 gText_AbilityName_VitalSpirit[];
extern const u8 gText_AbilityName_WhiteSmoke[];
extern const u8 gText_AbilityName_PurePower[];
extern const u8 gText_AbilityName_IronBarbs[];
extern const u8 gText_AbilityName_SolidRock[];
extern const u8 gText_AbilityName_Turboblaze[];
extern const u8 gText_AbilityName_Teravolt[];
extern const u8 gText_AbilityName_ChillingNeigh[];
extern const u8 gText_AbilityName_Libero[];
extern const u8 gText_AbilityName_TanglingHair[];
extern const u8 gText_AbilityName_WimpOut[];
extern const u8 gText_AbilityName_QueenlyMajesty[];
extern const u8 gText_AbilityName_PowerOfAlchemy[];
extern const u8 gText_AbilityName_PropellorTail[];
extern const u8 gText_AbilityName_SoundWaves[];
extern const u8 gText_AbilityName_IcySkin[];

const u8* GetAbilityNameOverride(const u8 ability, const u16 species) //Bypasses the 255 Ability limitation and implements clone Abilities
{
	u16 dexNum = SpeciesToNationalPokedexNum(species);

	switch (ability)
	{
		case ABILITY_CLOUDNINE:
			switch (dexNum)
			{
				#ifdef NATIONAL_DEX_RAYQUAZA
				case NATIONAL_DEX_RAYQUAZA:
					return gText_AbilityName_AirLock;
				#endif
			}
			break;
		case ABILITY_INSOMNIA:
			if (IsVitalSpiritAbility(ability, species))
				return gText_AbilityName_VitalSpirit;
			break;
		case ABILITY_CLEARBODY:
			if (IsWhiteSmokeAbility(ability, species))
				return gText_AbilityName_WhiteSmoke;
			break;
		case ABILITY_HUGEPOWER:
			switch (dexNum)
			{
				#if (defined NATIONAL_DEX_MEDITITE && defined NATIONAL_DEX_MEDICHAM)
				case NATIONAL_DEX_MEDITITE:
				case NATIONAL_DEX_MEDICHAM:
					return gText_AbilityName_PurePower;
				#endif
			}
			break;
		case ABILITY_ROUGHSKIN:
			switch (dexNum)
			{
				#if (defined NATIONAL_DEX_FERROSEED && defined NATIONAL_DEX_FERROTHORN && defined NATIONAL_DEX_TOGEDEMARU)
				case NATIONAL_DEX_FERROSEED:
				case NATIONAL_DEX_FERROTHORN:
				case NATIONAL_DEX_TOGEDEMARU:
					return gText_AbilityName_IronBarbs;
				#endif
			}
			break;
		case ABILITY_FILTER:
			switch (dexNum)
			{
				#if (defined NATIONAL_DEX_CAMERUPT && defined NATIONAL_DEX_RHYPERIOR && defined NATIONAL_DEX_TIRTOUGA && defined NATIONAL_DEX_CARRACOSTA)
				case NATIONAL_DEX_CAMERUPT:
				case NATIONAL_DEX_RHYPERIOR:
				case NATIONAL_DEX_TIRTOUGA:
				case NATIONAL_DEX_CARRACOSTA:
					return gText_AbilityName_SolidRock;
				#endif
			}
			break;
		case ABILITY_MOLDBREAKER:
			if (SpeciesHasTurboblaze(species))
				return gText_AbilityName_Turboblaze;
			else if (SpeciesHasTeravolt(species))
				return gText_AbilityName_Teravolt;
			break;
		case ABILITY_MOXIE:
			switch (dexNum)
			{
				#ifdef NATIONAL_DEX_GLASTRIER
				case NATIONAL_DEX_GLASTRIER:
					return gText_AbilityName_ChillingNeigh;
				#endif
			}
			break;
		case ABILITY_PROTEAN:
			switch (dexNum)
			{
				#if (defined NATIONAL_DEX_SCORBUNNY && defined NATIONAL_DEX_RABOOT && defined NATIONAL_DEX_CINDERACE)
				case NATIONAL_DEX_SCORBUNNY:
				case NATIONAL_DEX_RABOOT:
				case NATIONAL_DEX_CINDERACE:
					return gText_AbilityName_Libero;
				#endif
			}
			break;
		case ABILITY_GOOEY:
			switch (dexNum)
			{
				#if (defined NATIONAL_DEX_DIGLETT && defined NATIONAL_DEX_DUGTRIO)
				case NATIONAL_DEX_DIGLETT:
				case NATIONAL_DEX_DUGTRIO:
					return gText_AbilityName_TanglingHair;
				#endif
			}
			break;
		case ABILITY_EMERGENCYEXIT:
			switch (dexNum)
			{
				#ifdef NATIONAL_DEX_WIMPOD
				case NATIONAL_DEX_WIMPOD:
					return gText_AbilityName_WimpOut;
				#endif
			}
			break;
		case ABILITY_DAZZLING:
			switch (dexNum)
			{
				#ifdef NATIONAL_DEX_TSAREENA
				case NATIONAL_DEX_TSAREENA:
				#ifdef NATIONAL_DEX_NIDOQUEEN
				case NATIONAL_DEX_NIDOQUEEN:
				#endif
				#ifdef NATIONAL_DEX_VESPIQUEN
				case NATIONAL_DEX_VESPIQUEN:
				#endif
					return gText_AbilityName_QueenlyMajesty;
				#endif
			}
			break;
		case ABILITY_RECEIVER:
			switch (dexNum)
			{
				#if (defined NATIONAL_DEX_GRIMER && defined NATIONAL_DEX_MUK)
				case NATIONAL_DEX_GRIMER:
				case NATIONAL_DEX_MUK:
					return gText_AbilityName_PowerOfAlchemy;
				#endif
			}
			break;
		case ABILITY_STALWART:
			switch (dexNum)
			{
				#if (defined NATIONAL_DEX_ARROKUDA && defined NATIONAL_DEX_BARRASKEWDA)
				case NATIONAL_DEX_ARROKUDA:
				case NATIONAL_DEX_BARRASKEWDA:
					return gText_AbilityName_PropellorTail;
				#endif
			}
			break;
		case ABILITY_PUNKROCK:
			switch (dexNum)
			{
				#if (defined NATIONAL_DEX_VIBRAVA && defined NATIONAL_DEX_FLYGON)
				case NATIONAL_DEX_VIBRAVA:
				case NATIONAL_DEX_FLYGON:
					return gText_AbilityName_SoundWaves;
				#endif
			}
			break;
		case ABILITY_ICESCALES:
			switch (dexNum)
			{
				#if (defined NATIONAL_DEX_GLACEON && defined NATIONAL_DEX_SMOOCHUM && defined NATIONAL_DEX_JYNX)
				case NATIONAL_DEX_GLACEON:
				case NATIONAL_DEX_SMOOCHUM:
				case NATIONAL_DEX_JYNX:
					return gText_AbilityName_IcySkin;
				#endif
			}
			break;
	}

	return NULL;
}

const u8* GetAbilityName(const u8 ability, const u16 species)
{
	const u8* override = GetAbilityNameOverride(ability, species);
	if (override != NULL)
		return override;

	const u8* ptr = gAbilityNames[ability];

	if (ptr[3] == 0x8 || ptr[3] == 0x9) //Expanded Ability Names
		ptr = T1_READ_PTR(ptr);

	return ptr;
}

void CopyAbilityName(u8* dst, const u8 ability, const u16 species)
{
	StringCopy(dst, GetAbilityName(ability, species));
}

u16 GetProperAbilityPopUpSpecies(u8 bank)
{
	if (gNewBS->tookAbilityFrom[bank] != SPECIES_NONE)
		return gNewBS->tookAbilityFrom[bank];
	else
		return SPECIES(bank);
}

void SetProperAbilityPopUpSpecies(u8 bank)
{
	gAbilityPopUpSpecies = GetProperAbilityPopUpSpecies(bank);
}

void SetTookAbilityFrom(u8 taker, u8 takenFrom)
{
	if (gNewBS->tookAbilityFrom[takenFrom] != SPECIES_NONE)
		gNewBS->tookAbilityFrom[taker] = gNewBS->tookAbilityFrom[takenFrom]; //Pass along species
	else
		gNewBS->tookAbilityFrom[taker] = SPECIES(takenFrom);
}

void SwapTookAbilityFrom(u8 bank1, u8 bank2)
{
	u16 species1 = GetProperAbilityPopUpSpecies(bank1);
	u16 species2 = GetProperAbilityPopUpSpecies(bank2);

	gNewBS->tookAbilityFrom[bank1] = species2;
	gNewBS->tookAbilityFrom[bank2] = species1;
}

void ResetTookAbilityFrom(u8 bank)
{
	gNewBS->tookAbilityFrom[bank] = SPECIES_NONE;
}

bool8 IsTargetAbilityIgnored(u8 defAbility, u8 atkAbility, u16 move)
{
	return IS_MOLD_BREAKER(atkAbility, move) && gSpecialAbilityFlags[defAbility].gMoldBreakerIgnoredAbilities;
}

bool8 SpeciesHasTurboblaze(u16 species)
{
	#if (defined SPECIES_RESHIRAM && defined SPECIES_KYUREM_WHITE)
	return species == SPECIES_RESHIRAM
		|| species == SPECIES_KYUREM_WHITE;
	#else
	return FALSE;
	#endif
}

bool8 SpeciesHasTeravolt(u16 species)
{
	#if (defined SPECIES_ZEKROM && defined SPECIES_KYUREM_BLACK)
	return species == SPECIES_ZEKROM
		|| species == SPECIES_KYUREM_BLACK;
	#else
	return FALSE;
	#endif
}

bool8 IsClearBodyAbility(u8 ability)
{
	return ability == ABILITY_CLEARBODY
		#ifdef ABILITY_FULLMETALBODY
		|| ability == ABILITY_FULLMETALBODY
		#endif
		#ifdef ABILITY_WHITESMOKE
		|| ability == ABILITY_WHITESMOKE
		#endif
		;
}

bool8 IsMoldBreakerAbility(u8 ability)
{
	return ability == ABILITY_MOLDBREAKER
		#ifdef ABILITY_TURBOBLAZE
		|| ability == ABILITY_TURBOBLAZE
		#endif
		#ifdef ABILITY_TERAVOLT
		|| ability == ABILITY_TERAVOLT
		#endif
		;
}

bool8 IsMoxieAbility(u8 ability)
{
	switch (ability)
	{
		case ABILITY_MOXIE:
		#ifdef ABILITY_GRIMNEIGH
		case ABILITY_GRIMNEIGH:
		#endif
		#ifdef ABILITY_CHILLINGNEIGH
		case ABILITY_CHILLINGNEIGH:
		#endif
		#ifdef ABILITY_ASONE_GRIM
		case ABILITY_ASONE_GRIM:
		#endif
		#ifdef ABILITY_ASONE_CHILLING
		case ABILITY_ASONE_CHILLING:
		#endif
		case ABILITY_BEASTBOOST:
		case ABILITY_SOULHEART:
		case ABILITY_BATTLEBOND:
			return TRUE;
		default:
			return FALSE;
	}
}

bool8 IsChoiceAbility(u8 ability)
{
	return ability == ABILITY_GORILLATACTICS;
}

bool8 IsHPAbsorptionAbility(u8 ability)
{
	switch (ability)
	{
		case ABILITY_WATERABSORB:
		case ABILITY_VOLTABSORB:
			return TRUE;
		default:
			return FALSE;
	}
}

bool8 IsElectricAbsorptionAblity(u8 ability)
{
	switch (ability)
	{
		case ABILITY_VOLTABSORB:
		case ABILITY_MOTORDRIVE:
		case ABILITY_LIGHTNINGROD:
			return TRUE;
		default:
			return FALSE;
	}
}

bool8 IsPlusMinusAbility(u8 ability)
{
	switch (ability)
	{
		case ABILITY_PLUS:
		case ABILITY_MINUS:
			return TRUE;
		default:
			return FALSE;
	}
}

bool8 IsMultiscaleAbility(u8 ability)
{
	switch (ability)
	{
		case ABILITY_MULTISCALE:
		case ABILITY_SHADOWSHIELD:
			return TRUE;
		default:
			return FALSE;
	}
}

bool8 IsUnnerveAbility(u8 ability)
{
	return ability == ABILITY_UNNERVE
		#ifdef ABILITY_ASONE_GRIM
		|| ability == ABILITY_ASONE_GRIM
		#endif
		#ifdef ABILITY_ASONE_CHILLING
		|| ability == ABILITY_ASONE_CHILLING
		#endif
		;
}

bool8 UnnerveOnOpposingField(u8 bank)
{
	return ABILITY_ON_OPPOSING_FIELD(bank, ABILITY_UNNERVE)
		#ifdef ABILITY_ASONE_GRIM
		|| ABILITY_ON_OPPOSING_FIELD(bank, ABILITY_ASONE_GRIM)
		#endif
		#ifdef ABILITY_ASONE_CHILLING
		|| ABILITY_ON_OPPOSING_FIELD(bank, ABILITY_ASONE_CHILLING)
		#endif
		;
}

bool8 AbilityIncreasesWildItemChance(u8 ability)
{
	return ability == ABILITY_COMPOUNDEYES || ability == ABILITY_SUPERLUCK;
}

bool8 AbilityBlocksIntimidate(u8 ability)
{
	return ability == ABILITY_INNERFOCUS
		|| ability == ABILITY_OWNTEMPO
		|| ability == ABILITY_OBLIVIOUS
		|| ability == ABILITY_SCRAPPY;
}

bool8 AbilityPreventsLoweringAtk(u8 ability)
{
	return ability == ABILITY_HYPERCUTTER
		|| ability == ABILITY_MIRRORARMOR
		|| IsClearBodyAbility(ability);
}

bool8 AbilityPreventsLoweringStat(u8 ability, u8 statId)
{
	switch (ability)
	{
		case ABILITY_HYPERCUTTER:
			return statId == STAT_STAGE_ATK;
		case ABILITY_BIGPECKS:
			return statId == STAT_STAGE_DEF;
		case ABILITY_KEENEYE:
			return statId == STAT_STAGE_ACC;
		default:
			return FALSE;
	}
}

bool8 AbilityRaisesOneStatWhenSomeStatIsLowered(u8 ability)
{
	return ability == ABILITY_DEFIANT
		|| ability == ABILITY_COMPETITIVE;
}

bool8 IsAffectedBySturdy(u8 defAbility, u8 bankDef)
{
	return defAbility == ABILITY_STURDY
		&& BATTLER_MAX_HP(bankDef);
}

bool8 IsAffectedByDisguse(unusedArg u8 defAbility, unusedArg u16 defSpecies, unusedArg u8 moveSplit)
{
	#ifdef SPECIES_MIMIKYU
	if (defAbility == ABILITY_DISGUISE && defSpecies == SPECIES_MIMIKYU)
		return TRUE;
	#endif
	#ifdef SPECIES_EISCUE
	if (defAbility == ABILITY_ICEFACE && defSpecies == SPECIES_EISCUE && moveSplit == SPLIT_PHYSICAL)
		return TRUE;
	#endif

	return FALSE;
}

bool8 IsWhiteSmokeAbility(u8 ability, u16 species)
{
	if (!IsClearBodyAbility(ability))
		return FALSE;

	switch (SpeciesToNationalPokedexNum(species))
	{
		#if (defined NATIONAL_DEX_TORKOAL && defined NATIONAL_DEX_HEATMOR && defined NATIONAL_DEX_SIZZLIPEDE && defined NATIONAL_DEX_CENTISKORCH)
		case NATIONAL_DEX_TORKOAL:
		case NATIONAL_DEX_HEATMOR:
		case NATIONAL_DEX_SIZZLIPEDE:
		case NATIONAL_DEX_CENTISKORCH:
			return TRUE;
		#endif
	}

	return FALSE;
}

bool8 IsVitalSpiritAbility(u8 ability, u16 species)
{
	if (ability != ABILITY_INSOMNIA)
		return FALSE;

	switch (SpeciesToNationalPokedexNum(species))
	{
		#ifdef NATIONAL_DEX_MANKEY
		case NATIONAL_DEX_MANKEY:
		#endif
		#ifdef NATIONAL_DEX_PRIMEAPE
		case NATIONAL_DEX_PRIMEAPE:
		#endif
		#ifdef NATIONAL_DEX_MR_MIME
		case NATIONAL_DEX_MR_MIME:
		#endif
		#ifdef NATIONAL_DEX_ELECTABUZZ
		case NATIONAL_DEX_ELECTABUZZ:
		#endif
		#ifdef NATIONAL_DEX_ELECTABUZZ
		case NATIONAL_DEX_MAGMAR:
		#endif
		#ifdef NATIONAL_DEX_DELIBIRD
		case NATIONAL_DEX_DELIBIRD:
		#endif
		#ifdef NATIONAL_DEX_TYROGUE
		case NATIONAL_DEX_TYROGUE:
		#endif
		#ifdef NATIONAL_DEX_ELEKID
		case NATIONAL_DEX_ELEKID:
		#endif
		#ifdef NATIONAL_DEX_MAGBY
		case NATIONAL_DEX_MAGBY:
		#endif
		#ifdef NATIONAL_DEX_VIGOROTH
		case NATIONAL_DEX_VIGOROTH:
		#endif
		#ifdef NATIONAL_DEX_ELECTIVIRE
		case NATIONAL_DEX_ELECTIVIRE:
		#endif
		#ifdef NATIONAL_DEX_MAGMORTAR
		case NATIONAL_DEX_MAGMORTAR:
		#endif
		#ifdef NATIONAL_DEX_LILLIPUP
		case NATIONAL_DEX_LILLIPUP:
		#endif
		#ifdef NATIONAL_DEX_ROCKRUFF
		case NATIONAL_DEX_ROCKRUFF:
		#endif
		#ifdef NATIONAL_DEX_LYCANROC
		case NATIONAL_DEX_LYCANROC:
		#endif
			return TRUE;
	}

	return FALSE;
}
