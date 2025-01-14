#pragma once

#include "../global.h"

/**
 * \file move_tables.h
 * \brief Contains declarations for many lists of species.
 */

struct SpecialSpeciesFlags
{
	bool8 telekinesisBan : 1;
	bool8 battleTowerStandardBan : 1;
	bool8 gsCupLegendaries : 1;
	bool8 smogonOUBan : 1;
	bool8 smogonNationalDexOUBan : 1;
	bool8 smogonOUDoublesBan : 1;
	bool8 smogonLittleCup : 1;
	bool8 middleCup : 1;
	bool8 smogonMonotypeBan : 1;
	bool8 smogonCamomonsBan : 1;
	bool8 smogonAverageMonsBan : 1;
	bool8 smogon350CupBan : 1;
	bool8 smogonScalemonsBan : 1;
	bool8 smogonBenjaminButterfreeBan : 1;
	bool8 smogonUUBan : 1;
	bool8 smogonRUBan : 1;
	bool8 smogonNUBan : 1;
	bool8 goodForUbers : 1;
	bool8 badForUbers : 1;
	bool8 setPerfectXIVs : 1;
	bool8 smartWild : 1;
	bool8 randomizerBan : 1;
};

extern const struct SpecialSpeciesFlags gSpecialSpeciesFlags[];
