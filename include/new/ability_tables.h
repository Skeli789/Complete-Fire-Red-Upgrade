#pragma once

#include "../global.h"

/**
 * \file ability_tables.h
 * \brief Contains declarations for many lists of abilities.
 */


struct SpecialAbilityFlags
{
	bool8 gMoldBreakerIgnoredAbilities : 1;
	bool8 gRandomizerBannedOriginalAbilities : 1;
	bool8 gRandomizerBannedNewAbilities : 1;
	bool8 gRolePlayBannedAbilities : 1;
	bool8 gRolePlayAttackerBannedAbilities : 1;
	bool8 gSkillSwapBannedAbilities : 1;
	bool8 gWorrySeedBannedAbilities : 1;
	bool8 gGastroAcidBannedAbilities : 1;
	bool8 gEntrainmentBannedAbilitiesAttacker : 1;
	bool8 gEntrainmentBannedAbilitiesTarget : 1;
	bool8 gSimpleBeamBannedAbilities : 1;
	bool8 gReceiverBannedAbilities : 1;
	bool8 gTraceBannedAbilities : 1;
	bool8 gNeutralizingGasBannedAbilities : 1;
};

extern const struct SpecialAbilityFlags gSpecialAbilityFlags[];
extern const s8 gAbilityRatings[];
