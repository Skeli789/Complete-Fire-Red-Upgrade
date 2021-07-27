#include "defines.h"
#include "../include/battle.h"
#include "../include/string_util.h"
#include "../include/constants/abilities.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/species.h"

#include "../include/new/ability_util.h"

extern const u8 gAbilityNames[][ABILITY_NAME_LENGTH + 1];
extern const u8 gText_AbilityName_AirLock[];
extern const u8 gText_AbilityName_VitalSpirit[];
extern const u8 gText_AbilityName_WhiteSmoke[];
extern const u8 gText_AbilityName_PurePower[];
extern const u8 gText_AbilityName_IronBarbs[];
extern const u8 gText_AbilityName_SolidRock[];
extern const u8 gText_AbilityName_Turboblaze[];
extern const u8 gText_AbilityName_Teravolt[];
extern const u8 gText_AbilityName_GrimNeigh[];
extern const u8 gText_AbilityName_Libero[];
extern const u8 gText_AbilityName_TanglingHair[];
extern const u8 gText_AbilityName_WimpOut[];
extern const u8 gText_AbilityName_QueenlyMajesty[];
extern const u8 gText_AbilityName_PowerOfAlchemy[];
extern const u8 gText_AbilityName_PropellorTail[];

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
			switch (dexNum)
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
					return gText_AbilityName_VitalSpirit;
			}
			break;
		case ABILITY_CLEARBODY:
			switch (dexNum)
			{
				#if (defined NATIONAL_DEX_TORKOAL && defined NATIONAL_DEX_HEATMOR && defined NATIONAL_DEX_SIZZLIPEDE && defined NATIONAL_DEX_CENTISKORCH)
				case NATIONAL_DEX_TORKOAL:
				case NATIONAL_DEX_HEATMOR:
				case NATIONAL_DEX_SIZZLIPEDE:
				case NATIONAL_DEX_CENTISKORCH:
					return gText_AbilityName_WhiteSmoke;
				#endif
			}
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
				#ifdef NATIONAL_DEX_SPECTRIER
				case NATIONAL_DEX_SPECTRIER:
					return gText_AbilityName_GrimNeigh;
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
				#if (defined SPECIES_ARROKUDA && defined SPECIES_BARRASKEWDA)
				case SPECIES_ARROKUDA:
				case SPECIES_BARRASKEWDA:
					return gText_AbilityName_PropellorTail;
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
