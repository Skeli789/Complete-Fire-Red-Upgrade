#include "../../include/battle.h"
#include "../../include/global.h"

#include "../../include/constants/items.h"
#include "../../include/constants/moves.h"
#include "../../include/constants/species.h"
#include "../../include/constants/trainer_classes.h"
#include "../../include/new/build_pokemon_2.h"
#include "../../src/config.h"

#include "trainer_defines.h"

#ifdef EXPAND_TRAINERS



static const struct TrainerMonNoItemDefaultMoves sParty_TrainerAquaLeader[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerAquaGruntM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerAquaGruntF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsAromaLady[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsRuinManiac[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerInterviewer[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsTuberF[] = {
    {
        .lvl = 38,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsTuberM[] = {
    {
        .lvl = 38,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsCooltrainerM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsCooltrainerF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHexManiac[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsLady[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsBeauty[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRichBoy[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsPokemaniac[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsSwimmerM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsBlackBelt[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGuitarist[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerKindler[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsCamper[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugManiac[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsPsychicM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsPsychicF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsGentleman[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerEliteFourSidney[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerEliteFourPhoebe[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLeaderRoxanne[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLeaderBrawly[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLeaderTateLiza[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSchoolKidM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSchoolKidF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSrAndJr[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokefanM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokefanF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerExpertM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerExpertF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsYoungster[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsChampion[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsFisherman[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCyclingTriathleteM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCyclingTriathleteF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRunningTriathleteM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRunningTriathleteF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmingTriathleteM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmingTriathleteF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerDragonTamer[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsBirdKeeper[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerNinjaBoy[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBattleGirl[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerParasolLady[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsSwimmerF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsPicnicker[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsTwins[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsSailor[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBoarderM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBoarderF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCollector[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerWally[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBrendan[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBrendan2[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBrendan3[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerMay[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerMay2[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerMay3[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsPkmnBreederM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsPkmnBreederF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsPkmnRangerM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsPkmnRangerF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerMagmaLeader[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerMagmaGruntM[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerMagmaGruntF[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsLass[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsBugCatcher[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsHiker[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsYoungCouple[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerOldCouple[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRsSisAndBro[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerAquaAdminMatt[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerAquaAdminShelly[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerMagmaAdminTabitha[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerMagmaAdminCourtney[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLeaderWattson[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLeaderFlannery[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLeaderNorman[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLeaderWinona[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLeaderWallace[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerEliteFourGlacia[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerEliteFourDrake[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterBen[] = {
    {
        .lvl = 11,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 11,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterCalvin[] = {
    {
        .lvl = 14,
        .species = SPECIES_SPEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterJosh[] = {
    {
        .lvl = 10,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 10,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 10,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterTimmy[] = {
    {
        .lvl = 14,
        .species = SPECIES_SANDSHREW,
    },
    {
        .lvl = 14,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterJoey[] = {
    {
        .lvl = 15,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 15,
        .species = SPECIES_SPEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterDan[] = {
    {
        .lvl = 17,
        .species = SPECIES_SLOWPOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterChad[] = {
    {
        .lvl = 14,
        .species = SPECIES_EKANS,
    },
    {
        .lvl = 14,
        .species = SPECIES_SANDSHREW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterTyler[] = {
    {
        .lvl = 21,
        .species = SPECIES_NIDORAN_M,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterEddie[] = {
    {
        .lvl = 21,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterDillon[] = {
    {
        .lvl = 19,
        .species = SPECIES_SANDSHREW,
    },
    {
        .lvl = 19,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterYasu[] = {
    {
        .lvl = 17,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 17,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 17,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterDave[] = {
    {
        .lvl = 18,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .lvl = 18,
        .species = SPECIES_NIDORINO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterBen2[] = {
    {
        .iv = 20,
        .lvl = 17,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = 20,
        .lvl = 17,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherRick[] = {
    {
        .lvl = 6,
        .species = SPECIES_WEEDLE,
    },
    {
        .lvl = 6,
        .species = SPECIES_CATERPIE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherDoug[] = {
    {
        .lvl = 7,
        .species = SPECIES_WEEDLE,
    },
    {
        .lvl = 7,
        .species = SPECIES_KAKUNA,
    },
    {
        .lvl = 7,
        .species = SPECIES_WEEDLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherSammy[] = {
    {
        .lvl = 9,
        .species = SPECIES_WEEDLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherColton[] = {
    {
        .lvl = 10,
        .species = SPECIES_CATERPIE,
    },
    {
        .lvl = 10,
        .species = SPECIES_WEEDLE,
    },
    {
        .lvl = 10,
        .species = SPECIES_CATERPIE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherGreg[] = {
    {
        .lvl = 9,
        .species = SPECIES_WEEDLE,
    },
    {
        .lvl = 9,
        .species = SPECIES_KAKUNA,
    },
    {
        .lvl = 9,
        .species = SPECIES_CATERPIE,
    },
    {
        .lvl = 9,
        .species = SPECIES_METAPOD,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherJames[] = {
    {
        .lvl = 11,
        .species = SPECIES_CATERPIE,
    },
    {
        .lvl = 11,
        .species = SPECIES_METAPOD,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherKent[] = {
    {
        .lvl = 11,
        .species = SPECIES_WEEDLE,
    },
    {
        .lvl = 11,
        .species = SPECIES_KAKUNA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherRobby[] = {
    {
        .lvl = 10,
        .species = SPECIES_CATERPIE,
    },
    {
        .lvl = 10,
        .species = SPECIES_METAPOD,
    },
    {
        .lvl = 10,
        .species = SPECIES_CATERPIE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherCale[] = {
    {
        .lvl = 10,
        .species = SPECIES_CATERPIE,
    },
    {
        .lvl = 10,
        .species = SPECIES_WEEDLE,
    },
    {
        .lvl = 10,
        .species = SPECIES_METAPOD,
    },
    {
        .lvl = 10,
        .species = SPECIES_KAKUNA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherKeigo[] = {
    {
        .lvl = 16,
        .species = SPECIES_WEEDLE,
    },
    {
        .lvl = 16,
        .species = SPECIES_CATERPIE,
    },
    {
        .lvl = 16,
        .species = SPECIES_WEEDLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherElijah[] = {
    {
        .lvl = 20,
        .species = SPECIES_BUTTERFREE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcher2[] = {
    {
        .lvl = 18,
        .species = SPECIES_METAPOD,
    },
    {
        .lvl = 18,
        .species = SPECIES_CATERPIE,
    },
    {
        .lvl = 18,
        .species = SPECIES_VENONAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherBrent[] = {
    {
        .lvl = 19,
        .species = SPECIES_BEEDRILL,
    },
    {
        .lvl = 19,
        .species = SPECIES_BEEDRILL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherConner[] = {
    {
        .lvl = 20,
        .species = SPECIES_CATERPIE,
    },
    {
        .lvl = 20,
        .species = SPECIES_WEEDLE,
    },
    {
        .lvl = 20,
        .species = SPECIES_VENONAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassJanice[] = {
    {
        .lvl = 9,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 9,
        .species = SPECIES_PIDGEY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassSally[] = {
    {
        .lvl = 10,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 10,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassRobin[] = {
    {
        .lvl = 14,
        .species = SPECIES_JIGGLYPUFF,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassCrissy[] = {
    {
        .lvl = 31,
        .species = SPECIES_PARAS,
    },
    {
        .lvl = 31,
        .species = SPECIES_PARAS,
    },
    {
        .lvl = 31,
        .species = SPECIES_PARASECT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassMiriam[] = {
    {
        .lvl = 11,
        .species = SPECIES_ODDISH,
    },
    {
        .lvl = 11,
        .species = SPECIES_BELLSPROUT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassIris[] = {
    {
        .lvl = 14,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassReli[] = {
    {
        .lvl = 16,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .lvl = 16,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassAli[] = {
    {
        .lvl = 12,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 12,
        .species = SPECIES_ODDISH,
    },
    {
        .lvl = 12,
        .species = SPECIES_BELLSPROUT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLass2[] = {
    {
        .lvl = 15,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .lvl = 15,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassHaley[] = {
    {
        .lvl = 13,
        .species = SPECIES_ODDISH,
    },
    {
        .lvl = 13,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 13,
        .species = SPECIES_ODDISH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassAnn[] = {
    {
        .lvl = 18,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 18,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassDawn[] = {
    {
        .lvl = 18,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 18,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassPaige[] = {
    {
        .lvl = 23,
        .species = SPECIES_NIDORAN_F,
    },
    {
        .lvl = 23,
        .species = SPECIES_NIDORINA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassAndrea[] = {
    {
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
    {
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
    {
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassMegan[] = {
    {
        .lvl = 19,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 19,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 19,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .lvl = 19,
        .species = SPECIES_MEOWTH,
    },
    {
        .lvl = 19,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassJulia[] = {
    {
        .lvl = 22,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .lvl = 22,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassKay[] = {
    {
        .lvl = 23,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .lvl = 23,
        .species = SPECIES_WEEPINBELL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassLisa[] = {
    {
        .lvl = 23,
        .species = SPECIES_ODDISH,
    },
    {
        .lvl = 23,
        .species = SPECIES_GLOOM,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSailorEdmond[] = {
    {
        .lvl = 18,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 18,
        .species = SPECIES_SHELLDER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSailorTrevor[] = {
    {
        .lvl = 17,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 17,
        .species = SPECIES_TENTACOOL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSailorLeonard[] = {
    {
        .lvl = 21,
        .species = SPECIES_SHELLDER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSailorDuncan[] = {
    {
        .lvl = 17,
        .species = SPECIES_HORSEA,
    },
    {
        .lvl = 17,
        .species = SPECIES_SHELLDER,
    },
    {
        .lvl = 17,
        .species = SPECIES_TENTACOOL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSailorHuey[] = {
    {
        .lvl = 18,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 18,
        .species = SPECIES_STARYU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSailorDylan[] = {
    {
        .lvl = 17,
        .species = SPECIES_HORSEA,
    },
    {
        .lvl = 17,
        .species = SPECIES_HORSEA,
    },
    {
        .lvl = 17,
        .species = SPECIES_HORSEA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSailorPhillip[] = {
    {
        .lvl = 20,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSailorDwayne[] = {
    {
        .lvl = 21,
        .species = SPECIES_PIKACHU,
    },
    {
        .lvl = 21,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCamperLiam[] = {
    {
        .lvl = 10,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_TACKLE, MOVE_DEFENSECURL, MOVE_NONE, MOVE_NONE},
    },
    {
        .lvl = 11,
        .species = SPECIES_SANDSHREW,
        .moves = {MOVE_SCRATCH, MOVE_DEFENSECURL, MOVE_SANDATTACK, MOVE_NONE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperShane[] = {
    {
        .lvl = 14,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 14,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperEthan[] = {
    {
        .lvl = 18,
        .species = SPECIES_MANKEY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperRicky[] = {
    {
        .lvl = 20,
        .species = SPECIES_SQUIRTLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperJeff[] = {
    {
        .lvl = 16,
        .species = SPECIES_SPEAROW,
    },
    {
        .lvl = 16,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamper2[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};


static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperChris[] = {
    {
        .lvl = 21,
        .species = SPECIES_GROWLITHE,
    },
    {
        .lvl = 21,
        .species = SPECIES_CHARMANDER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperDrew[] = {
    {
        .lvl = 19,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 19,
        .species = SPECIES_SANDSHREW,
    },
    {
        .lvl = 19,
        .species = SPECIES_EKANS,
    },
    {
        .lvl = 19,
        .species = SPECIES_SANDSHREW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerDiana[] = {
    {
        .lvl = 19,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerNancy[] = {
    {
        .lvl = 16,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 16,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerIsabelle[] = {
    {
        .lvl = 16,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 16,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 16,
        .species = SPECIES_PIDGEY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerKelsey[] = {
    {
        .lvl = 15,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .lvl = 15,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerAlicia[] = {
    {
        .lvl = 18,
        .species = SPECIES_ODDISH,
    },
    {
        .lvl = 18,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .lvl = 18,
        .species = SPECIES_ODDISH,
    },
    {
        .lvl = 18,
        .species = SPECIES_BELLSPROUT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerCaitlin[] = {
    {
        .lvl = 23,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerHeidi[] = {
    {
        .lvl = 20,
        .species = SPECIES_PIKACHU,
    },
    {
        .lvl = 20,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerCarol[] = {
    {
        .lvl = 21,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 21,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerSofia[] = {
    {
        .lvl = 21,
        .species = SPECIES_JIGGLYPUFF,
    },
    {
        .lvl = 21,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 21,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerMartha[] = {
    {
        .lvl = 22,
        .species = SPECIES_ODDISH,
    },
    {
        .lvl = 22,
        .species = SPECIES_BULBASAUR,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerTina[] = {
    {
        .lvl = 24,
        .species = SPECIES_BULBASAUR,
    },
    {
        .lvl = 24,
        .species = SPECIES_IVYSAUR,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerHannah[] = {
    {
        .lvl = 24,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
    {
        .lvl = 24,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 24,
        .species = SPECIES_PIKACHU,
    },
    {
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokemaniacMark[] = {
    {
        .iv = 30,
        .lvl = 29,
        .species = SPECIES_RHYHORN,
    },
    {
        .iv = 30,
        .lvl = 29,
        .species = SPECIES_LICKITUNG,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokemaniacHerman[] = {
    {
        .iv = 30,
        .lvl = 20,
        .species = SPECIES_CUBONE,
    },
    {
        .iv = 30,
        .lvl = 20,
        .species = SPECIES_SLOWPOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokemaniacCooper[] = {
    {
        .iv = 30,
        .lvl = 20,
        .species = SPECIES_SLOWPOKE,
    },
    {
        .iv = 30,
        .lvl = 20,
        .species = SPECIES_SLOWPOKE,
    },
    {
        .iv = 30,
        .lvl = 20,
        .species = SPECIES_SLOWPOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokemaniacSteve[] = {
    {
        .iv = 30,
        .lvl = 22,
        .species = SPECIES_CHARMANDER,
    },
    {
        .iv = 30,
        .lvl = 22,
        .species = SPECIES_CUBONE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokemaniacWinston[] = {
    {
        .iv = 30,
        .lvl = 25,
        .species = SPECIES_SLOWPOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokemaniacDawson[] = {
    {
        .iv = 30,
        .lvl = 40,
        .species = SPECIES_CHARMELEON,
    },
    {
        .iv = 30,
        .lvl = 40,
        .species = SPECIES_LAPRAS,
    },
    {
        .iv = 30,
        .lvl = 40,
        .species = SPECIES_LICKITUNG,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokemaniacAshton[] = {
    {
        .iv = 30,
        .lvl = 23,
        .species = SPECIES_CUBONE,
    },
    {
        .iv = 30,
        .lvl = 23,
        .species = SPECIES_SLOWPOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSuperNerdJovan[] = {
    {
        .lvl = 11,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .lvl = 11,
        .species = SPECIES_VOLTORB,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSuperNerdMiguel[] = {
    {
        .lvl = 12,
        .species = SPECIES_GRIMER,
    },
    {
        .lvl = 12,
        .species = SPECIES_VOLTORB,
    },
    {
        .lvl = 12,
        .species = SPECIES_KOFFING,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerSuperNerdAidan[] = {
    {
        .lvl = 20,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SONICBOOM, MOVE_SCREECH, MOVE_TACKLE, MOVE_CHARGE},
    },
    {
        .lvl = 20,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE, MOVE_POISONGAS},
    },
    {
        .lvl = 20,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SONICBOOM, MOVE_SCREECH, MOVE_TACKLE, MOVE_CHARGE},
    },
    {
        .lvl = 20,
        .species = SPECIES_MAGNEMITE,
        .moves = {MOVE_SONICBOOM, MOVE_SUPERSONIC, MOVE_THUNDERSHOCK, MOVE_TACKLE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSuperNerdGlenn[] = {
    {
        .lvl = 22,
        .species = SPECIES_GRIMER,
    },
    {
        .lvl = 22,
        .species = SPECIES_MUK,
    },
    {
        .lvl = 22,
        .species = SPECIES_GRIMER,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerSuperNerdLeslie[] = {
    {
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SLUDGE, MOVE_SMOKESCREEN, MOVE_SMOG, MOVE_SELFDESTRUCT},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSuperNerd1[] = {
    {
        .lvl = 22,
        .species = SPECIES_KOFFING,
    },
    {
        .lvl = 22,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .lvl = 22,
        .species = SPECIES_WEEZING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSuperNerd2[] = {
    {
        .lvl = 20,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .lvl = 20,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .lvl = 20,
        .species = SPECIES_KOFFING,
    },
    {
        .lvl = 20,
        .species = SPECIES_MAGNEMITE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSuperNerd3[] = {
    {
        .lvl = 24,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .lvl = 24,
        .species = SPECIES_VOLTORB,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSuperNerdErik[] = {
    {
        .lvl = 36,
        .species = SPECIES_VULPIX,
    },
    {
        .lvl = 36,
        .species = SPECIES_VULPIX,
    },
    {
        .lvl = 36,
        .species = SPECIES_NINETALES,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSuperNerdAvery[] = {
    {
        .lvl = 34,
        .species = SPECIES_PONYTA,
    },
    {
        .lvl = 34,
        .species = SPECIES_CHARMANDER,
    },
    {
        .lvl = 34,
        .species = SPECIES_VULPIX,
    },
    {
        .lvl = 34,
        .species = SPECIES_GROWLITHE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSuperNerdDerek[] = {
    {
        .lvl = 41,
        .species = SPECIES_RAPIDASH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSuperNerdZac[] = {
    {
        .lvl = 37,
        .species = SPECIES_GROWLITHE,
    },
    {
        .lvl = 37,
        .species = SPECIES_VULPIX,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerMarcos[] = {
    {
        .lvl = 10,
        .species = SPECIES_GEODUDE,
    },
    {
        .lvl = 10,
        .species = SPECIES_GEODUDE,
    },
    {
        .lvl = 10,
        .species = SPECIES_ONIX,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerFranklin[] = {
    {
        .lvl = 15,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 15,
        .species = SPECIES_GEODUDE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerNob[] = {
    {
        .lvl = 13,
        .species = SPECIES_GEODUDE,
    },
    {
        .lvl = 13,
        .species = SPECIES_GEODUDE,
    },
    {
        .lvl = 13,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 13,
        .species = SPECIES_GEODUDE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerWayne[] = {
    {
        .lvl = 17,
        .species = SPECIES_ONIX,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerHikerAlan[] = {
    {
        .lvl = 21,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCKTHROW, MOVE_MUDSPORT, MOVE_DEFENSECURL},
    },
    {
        .lvl = 21,
        .species = SPECIES_ONIX,
        .moves = {MOVE_HARDEN, MOVE_ROCKTHROW, MOVE_BIND, MOVE_SCREECH},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerBrice[] = {
    {
        .lvl = 20,
        .species = SPECIES_GEODUDE,
    },
    {
        .lvl = 20,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 20,
        .species = SPECIES_GEODUDE,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerHikerClark[] = {
    {
        .lvl = 21,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCKTHROW, MOVE_MUDSPORT, MOVE_DEFENSECURL},
    },
    {
        .lvl = 21,
        .species = SPECIES_ONIX,
        .moves = {MOVE_HARDEN, MOVE_ROCKTHROW, MOVE_BIND, MOVE_SCREECH},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerTrent[] = {
    {
        .lvl = 19,
        .species = SPECIES_ONIX,
    },
    {
        .lvl = 19,
        .species = SPECIES_GRAVELER,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerHikerDudley[] = {
    {
        .lvl = 21,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCKTHROW, MOVE_MUDSPORT, MOVE_DEFENSECURL},
    },
    {
        .lvl = 21,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCKTHROW, MOVE_MUDSPORT, MOVE_DEFENSECURL},
    },
    {
        .lvl = 21,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCKTHROW, MOVE_MUDSPORT, MOVE_DEFENSECURL},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerHikerAllen[] = {
    {
        .lvl = 25,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCKTHROW, MOVE_MUDSPORT, MOVE_DEFENSECURL},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerEric[] = {
    {
        .lvl = 20,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 20,
        .species = SPECIES_ONIX,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerLenny[] = {
    {
        .lvl = 19,
        .species = SPECIES_GEODUDE,
    },
    {
        .lvl = 19,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 19,
        .species = SPECIES_GEODUDE,
    },
    {
        .lvl = 19,
        .species = SPECIES_GEODUDE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerOliver[] = {
    {
        .lvl = 20,
        .species = SPECIES_ONIX,
    },
    {
        .lvl = 20,
        .species = SPECIES_ONIX,
    },
    {
        .lvl = 20,
        .species = SPECIES_GEODUDE,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerHikerLucas[] = {
    {
        .lvl = 21,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCKTHROW, MOVE_MUDSPORT, MOVE_DEFENSECURL},
    },
    {
        .lvl = 21,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_MAGNITUDE, MOVE_ROCKTHROW, MOVE_MUDSPORT, MOVE_DEFENSECURL},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerJared[] = {
    {
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_POISONGAS},
    },
    {
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerMalik[] = {
    {
        .lvl = 29,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 29,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerErnest[] = {
    {
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 25,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 25,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE, MOVE_POUND},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerAlex[] = {
    {
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 28,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
    {
        .lvl = 28,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerLao[] = {
    {
        .lvl = 29,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
    {
        .lvl = 29,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBiker1[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};


static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBikerHideo[] = {
    {
        .lvl = 33,
        .species = SPECIES_WEEZING,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerRuben[] = {
    {
        .lvl = 28,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 28,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBikerBilly[] = {
    {
        .lvl = 33,
        .species = SPECIES_MUK,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerNikolas[] = {
    {
        .lvl = 29,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SPARK, MOVE_SONICBOOM, MOVE_SCREECH, MOVE_CHARGE},
    },
    {
        .lvl = 29,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SPARK, MOVE_SONICBOOM, MOVE_SCREECH, MOVE_CHARGE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerJaxon[] = {
    {
        .lvl = 29,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SLUDGE, MOVE_SMOKESCREEN, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 29,
        .species = SPECIES_MUK,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerWilliam[] = {
    {
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 25,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 25,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SELFDESTRUCT, MOVE_TACKLE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerLukas[] = {
    {
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 26,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
    {
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerIsaac[] = {
    {
        .lvl = 28,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
    {
        .lvl = 28,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
    {
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerGerald[] = {
    {
        .lvl = 29,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 29,
        .species = SPECIES_MUK,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBurglar1[] = {
    {
        .lvl = 29,
        .species = SPECIES_GROWLITHE,
    },
    {
        .lvl = 29,
        .species = SPECIES_VULPIX,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBurglar2[] = {
    {
        .lvl = 33,
        .species = SPECIES_GROWLITHE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBurglar3[] = {
    {
        .lvl = 28,
        .species = SPECIES_VULPIX,
    },
    {
        .lvl = 28,
        .species = SPECIES_CHARMANDER,
    },
    {
        .lvl = 28,
        .species = SPECIES_PONYTA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBurglarQuinn[] = {
    {
        .lvl = 36,
        .species = SPECIES_GROWLITHE,
    },
    {
        .lvl = 36,
        .species = SPECIES_VULPIX,
    },
    {
        .lvl = 36,
        .species = SPECIES_NINETALES,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBurglarRamon[] = {
    {
        .lvl = 41,
        .species = SPECIES_PONYTA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBurglarDusty[] = {
    {
        .lvl = 37,
        .species = SPECIES_VULPIX,
    },
    {
        .lvl = 37,
        .species = SPECIES_GROWLITHE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBurglarArnie[] = {
    {
        .lvl = 34,
        .species = SPECIES_CHARMANDER,
    },
    {
        .lvl = 34,
        .species = SPECIES_CHARMELEON,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBurglar4[] = {
    {
        .lvl = 34,
        .species = SPECIES_CHARMANDER,
    },
    {
        .lvl = 34,
        .species = SPECIES_CHARMELEON,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBurglarSimon[] = {
    {
        .lvl = 38,
        .species = SPECIES_NINETALES,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBurglarLewis[] = {
    {
        .lvl = 34,
        .species = SPECIES_GROWLITHE,
    },
    {
        .lvl = 34,
        .species = SPECIES_PONYTA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerEngineerBaily[] = {
    {
        .lvl = 21,
        .species = SPECIES_VOLTORB,
    },
    {
        .lvl = 21,
        .species = SPECIES_MAGNEMITE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerEngineerBraxton[] = {
    {
        .lvl = 21,
        .species = SPECIES_MAGNEMITE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerEngineerBernie[] = {
    {
        .lvl = 18,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .lvl = 18,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .lvl = 18,
        .species = SPECIES_MAGNETON,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanDale[] = {
    {
        .lvl = 17,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 17,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 17,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanBarny[] = {
    {
        .lvl = 17,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 17,
        .species = SPECIES_STARYU,
    },
    {
        .lvl = 17,
        .species = SPECIES_SHELLDER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanNed[] = {
    {
        .lvl = 22,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 22,
        .species = SPECIES_POLIWAG,
    },
    {
        .lvl = 22,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanChip[] = {
    {
        .lvl = 24,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 24,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanHank[] = {
    {
        .lvl = 27,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanElliot[] = {
    {
        .lvl = 21,
        .species = SPECIES_POLIWAG,
    },
    {
        .lvl = 21,
        .species = SPECIES_SHELLDER,
    },
    {
        .lvl = 21,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 21,
        .species = SPECIES_HORSEA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanRonald[] = {
    {
        .lvl = 28,
        .species = SPECIES_SEAKING,
    },
    {
        .lvl = 28,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 28,
        .species = SPECIES_SEAKING,
    },
    {
        .lvl = 28,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanClaude[] = {
    {
        .lvl = 31,
        .species = SPECIES_SHELLDER,
    },
    {
        .lvl = 31,
        .species = SPECIES_CLOYSTER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanWade[] = {
    {
        .lvl = 27,
        .species = SPECIES_MAGIKARP,
    },
    {
        .lvl = 27,
        .species = SPECIES_MAGIKARP,
    },
    {
        .lvl = 27,
        .species = SPECIES_MAGIKARP,
    },
    {
        .lvl = 27,
        .species = SPECIES_MAGIKARP,
    },
    {
        .lvl = 27,
        .species = SPECIES_MAGIKARP,
    },
    {
        .lvl = 27,
        .species = SPECIES_MAGIKARP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanNolan[] = {
    {
        .lvl = 33,
        .species = SPECIES_SEAKING,
    },
    {
        .lvl = 33,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanAndrew[] = {
    {
        .lvl = 24,
        .species = SPECIES_MAGIKARP,
    },
    {
        .lvl = 24,
        .species = SPECIES_MAGIKARP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleLuis[] = {
    {
        .lvl = 16,
        .species = SPECIES_HORSEA,
    },
    {
        .lvl = 16,
        .species = SPECIES_SHELLDER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleRichard[] = {
    {
        .lvl = 30,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 30,
        .species = SPECIES_SHELLDER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleReece[] = {
    {
        .lvl = 29,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 29,
        .species = SPECIES_HORSEA,
    },
    {
        .lvl = 29,
        .species = SPECIES_STARYU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleMatthew[] = {
    {
        .lvl = 30,
        .species = SPECIES_POLIWAG,
    },
    {
        .lvl = 30,
        .species = SPECIES_POLIWHIRL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleDouglas[] = {
    {
        .lvl = 27,
        .species = SPECIES_HORSEA,
    },
    {
        .lvl = 27,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 27,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 27,
        .species = SPECIES_GOLDEEN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleDavid[] = {
    {
        .lvl = 29,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 29,
        .species = SPECIES_SHELLDER,
    },
    {
        .lvl = 29,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleTony[] = {
    {
        .lvl = 30,
        .species = SPECIES_HORSEA,
    },
    {
        .lvl = 30,
        .species = SPECIES_HORSEA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleAxle[] = {
    {
        .lvl = 27,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 27,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 27,
        .species = SPECIES_STARYU,
    },
    {
        .lvl = 27,
        .species = SPECIES_HORSEA,
    },
    {
        .lvl = 27,
        .species = SPECIES_TENTACRUEL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleBarry[] = {
    {
        .lvl = 31,
        .species = SPECIES_SHELLDER,
    },
    {
        .lvl = 31,
        .species = SPECIES_CLOYSTER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleDean[] = {
    {
        .lvl = 35,
        .species = SPECIES_STARYU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleDarrin[] = {
    {
        .lvl = 28,
        .species = SPECIES_HORSEA,
    },
    {
        .lvl = 28,
        .species = SPECIES_HORSEA,
    },
    {
        .lvl = 28,
        .species = SPECIES_SEADRA,
    },
    {
        .lvl = 28,
        .species = SPECIES_HORSEA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleSpencer[] = {
    {
        .lvl = 33,
        .species = SPECIES_SEADRA,
    },
    {
        .lvl = 33,
        .species = SPECIES_TENTACRUEL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleJack[] = {
    {
        .lvl = 37,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleJerome[] = {
    {
        .lvl = 33,
        .species = SPECIES_STARYU,
    },
    {
        .lvl = 33,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleRoland[] = {
    {
        .lvl = 32,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .lvl = 32,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 32,
        .species = SPECIES_SEADRA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCueBallKoji[] = {
    {
        .lvl = 28,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 28,
        .species = SPECIES_MANKEY,
    },
    {
        .lvl = 28,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCueBallLuke[] = {
    {
        .lvl = 29,
        .species = SPECIES_MANKEY,
    },
    {
        .lvl = 29,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCueBallCamron[] = {
    {
        .lvl = 29,
        .species = SPECIES_MANKEY,
    },
    {
        .lvl = 29,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCueBallRaul[] = {
    {
        .lvl = 29,
        .species = SPECIES_MANKEY,
    },
    {
        .lvl = 29,
        .species = SPECIES_PRIMEAPE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCueBallIsaiah[] = {
    {
        .lvl = 29,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 29,
        .species = SPECIES_MACHAMP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCueBallZeek[] = {
    {
        .lvl = 33,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCueBallJamal[] = {
    {
        .lvl = 26,
        .species = SPECIES_MANKEY,
    },
    {
        .lvl = 26,
        .species = SPECIES_MANKEY,
    },
    {
        .lvl = 26,
        .species = SPECIES_MACHAMP,
    },
    {
        .lvl = 26,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCueBallCorey[] = {
    {
        .lvl = 29,
        .species = SPECIES_PRIMEAPE,
    },
    {
        .lvl = 29,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCueBallChase[] = {
    {
        .lvl = 31,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 31,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 31,
        .species = SPECIES_TENTACRUEL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGamerHugo[] = {
    {
        .lvl = 18,
        .species = SPECIES_POLIWAG,
    },
    {
        .lvl = 18,
        .species = SPECIES_HORSEA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGamerJasper[] = {
    {
        .lvl = 18,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .lvl = 18,
        .species = SPECIES_ODDISH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGamerDirk[] = {
    {
        .lvl = 18,
        .species = SPECIES_VOLTORB,
    },
    {
        .lvl = 18,
        .species = SPECIES_MAGNEMITE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGamerDarian[] = {
    {
        .lvl = 18,
        .species = SPECIES_GROWLITHE,
    },
    {
        .lvl = 18,
        .species = SPECIES_VULPIX,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGamerStan[] = {
    {
        .lvl = 22,
        .species = SPECIES_POLIWAG,
    },
    {
        .lvl = 22,
        .species = SPECIES_POLIWAG,
    },
    {
        .lvl = 22,
        .species = SPECIES_POLIWHIRL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGamer1[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};


static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGamerRich[] = {
    {
        .lvl = 24,
        .species = SPECIES_GROWLITHE,
    },
    {
        .lvl = 24,
        .species = SPECIES_VULPIX,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBeautyBridget[] = {
    {
        .lvl = 21,
        .species = SPECIES_ODDISH,
    },
    {
        .lvl = 21,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .lvl = 21,
        .species = SPECIES_ODDISH,
    },
    {
        .lvl = 21,
        .species = SPECIES_BELLSPROUT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBeautyTamia[] = {
    {
        .lvl = 24,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .lvl = 24,
        .species = SPECIES_BELLSPROUT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBeautyLori[] = {
    {
        .lvl = 24,
        .species = SPECIES_EXEGGCUTE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBeautyLola[] = {
    {
        .lvl = 27,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 27,
        .species = SPECIES_PIKACHU,
    },
    {
        .lvl = 27,
        .species = SPECIES_RATTATA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBeautySheila[] = {
    {
        .lvl = 29,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .lvl = 29,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleTiffany[] = {
    {
        .lvl = 35,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleNora[] = {
    {
        .lvl = 30,
        .species = SPECIES_SHELLDER,
    },
    {
        .lvl = 30,
        .species = SPECIES_SHELLDER,
    },
    {
        .lvl = 30,
        .species = SPECIES_CLOYSTER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleMelissa[] = {
    {
        .lvl = 31,
        .species = SPECIES_POLIWAG,
    },
    {
        .lvl = 31,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBeautyGrace[] = {
    {
        .lvl = 29,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .lvl = 29,
        .species = SPECIES_WIGGLYTUFF,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBeautyOlivia[] = {
    {
        .lvl = 29,
        .species = SPECIES_BULBASAUR,
    },
    {
        .lvl = 29,
        .species = SPECIES_IVYSAUR,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBeautyLauren[] = {
    {
        .lvl = 33,
        .species = SPECIES_WEEPINBELL,
    },
    {
        .lvl = 33,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .lvl = 33,
        .species = SPECIES_WEEPINBELL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleAnya[] = {
    {
        .lvl = 27,
        .species = SPECIES_POLIWAG,
    },
    {
        .lvl = 27,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 27,
        .species = SPECIES_SEAKING,
    },
    {
        .lvl = 27,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 27,
        .species = SPECIES_POLIWAG,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleAlice[] = {
    {
        .lvl = 30,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 30,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleConnie[] = {
    {
        .lvl = 29,
        .species = SPECIES_STARYU,
    },
    {
        .lvl = 29,
        .species = SPECIES_STARYU,
    },
    {
        .lvl = 29,
        .species = SPECIES_STARYU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleShirley[] = {
    {
        .lvl = 30,
        .species = SPECIES_SEADRA,
    },
    {
        .lvl = 30,
        .species = SPECIES_HORSEA,
    },
    {
        .lvl = 30,
        .species = SPECIES_SEADRA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPsychicJohan[] = {
    {
        .iv = 50,
        .lvl = 31,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = 50,
        .lvl = 31,
        .species = SPECIES_SLOWPOKE,
    },
    {
        .iv = 50,
        .lvl = 31,
        .species = SPECIES_MR_MIME,
    },
    {
        .iv = 50,
        .lvl = 31,
        .species = SPECIES_KADABRA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPsychicTyron[] = {
    {
        .iv = 50,
        .lvl = 34,
        .species = SPECIES_MR_MIME,
    },
    {
        .iv = 50,
        .lvl = 34,
        .species = SPECIES_KADABRA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPsychicCameron[] = {
    {
        .iv = 50,
        .lvl = 33,
        .species = SPECIES_SLOWPOKE,
    },
    {
        .iv = 50,
        .lvl = 33,
        .species = SPECIES_SLOWPOKE,
    },
    {
        .iv = 50,
        .lvl = 33,
        .species = SPECIES_SLOWBRO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPsychicPreston[] = {
    {
        .iv = 50,
        .lvl = 38,
        .species = SPECIES_SLOWBRO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRockerRandall[] = {
    {
        .lvl = 20,
        .species = SPECIES_VOLTORB,
    },
    {
        .lvl = 20,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .lvl = 20,
        .species = SPECIES_VOLTORB,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRockerLuca[] = {
    {
        .lvl = 29,
        .species = SPECIES_VOLTORB,
    },
    {
        .lvl = 29,
        .species = SPECIES_ELECTRODE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerJugglerDalton[] = {
    {
        .lvl = 29,
        .species = SPECIES_KADABRA,
    },
    {
        .lvl = 29,
        .species = SPECIES_MR_MIME,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerJugglerNelson[] = {
    {
        .lvl = 41,
        .species = SPECIES_DROWZEE,
    },
    {
        .lvl = 41,
        .species = SPECIES_HYPNO,
    },
    {
        .lvl = 41,
        .species = SPECIES_KADABRA,
    },
    {
        .lvl = 41,
        .species = SPECIES_KADABRA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerJugglerKirk[] = {
    {
        .lvl = 31,
        .species = SPECIES_DROWZEE,
    },
    {
        .lvl = 31,
        .species = SPECIES_DROWZEE,
    },
    {
        .lvl = 31,
        .species = SPECIES_KADABRA,
    },
    {
        .lvl = 31,
        .species = SPECIES_DROWZEE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerJugglerShawn[] = {
    {
        .lvl = 34,
        .species = SPECIES_DROWZEE,
    },
    {
        .lvl = 34,
        .species = SPECIES_HYPNO,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerJugglerGregory[] = {
    {
        .lvl = 48,
        .species = SPECIES_MR_MIME,
        .moves = {MOVE_PSYCHIC, MOVE_ROLEPLAY, MOVE_DOUBLESLAP, MOVE_ENCORE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerJugglerEdward[] = {
    {
        .lvl = 46,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SWIFT, MOVE_LIGHTSCREEN, MOVE_SPARK, MOVE_SONICBOOM},
    },
    {
        .lvl = 46,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SWIFT, MOVE_LIGHTSCREEN, MOVE_SPARK, MOVE_SONICBOOM},
    },
    {
        .lvl = 47,
        .species = SPECIES_ELECTRODE,
        .moves = {MOVE_SWIFT, MOVE_SPARK, MOVE_SELFDESTRUCT, MOVE_SONICBOOM},
    },
    {
        .lvl = 48,
        .species = SPECIES_MR_MIME,
        .moves = {MOVE_PSYCHIC, MOVE_ROLEPLAY, MOVE_REFLECT, MOVE_ENCORE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerJugglerKayden[] = {
    {
        .lvl = 38,
        .species = SPECIES_HYPNO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerJugglerNate[] = {
    {
        .lvl = 34,
        .species = SPECIES_DROWZEE,
    },
    {
        .lvl = 34,
        .species = SPECIES_KADABRA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTamerPhil[] = {
    {
        .iv = 40,
        .lvl = 34,
        .species = SPECIES_SANDSLASH,
    },
    {
        .iv = 40,
        .lvl = 34,
        .species = SPECIES_ARBOK,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTamerEdgar[] = {
    {
        .iv = 40,
        .lvl = 33,
        .species = SPECIES_ARBOK,
    },
    {
        .iv = 40,
        .lvl = 33,
        .species = SPECIES_SANDSLASH,
    },
    {
        .iv = 40,
        .lvl = 33,
        .species = SPECIES_ARBOK,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTamerJason[] = {
    {
        .iv = 40,
        .lvl = 43,
        .species = SPECIES_RHYHORN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTamerCole[] = {
    {
        .iv = 40,
        .lvl = 39,
        .species = SPECIES_ARBOK,
    },
    {
        .iv = 40,
        .lvl = 39,
        .species = SPECIES_TAUROS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTamerVincent[] = {
    {
        .iv = 40,
        .lvl = 44,
        .species = SPECIES_PERSIAN,
    },
    {
        .iv = 40,
        .lvl = 44,
        .species = SPECIES_GOLDUCK,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTamerJohn[] = {
    {
        .iv = 40,
        .lvl = 42,
        .species = SPECIES_RHYHORN,
    },
    {
        .iv = 40,
        .lvl = 42,
        .species = SPECIES_PRIMEAPE,
    },
    {
        .iv = 40,
        .lvl = 42,
        .species = SPECIES_ARBOK,
    },
    {
        .iv = 40,
        .lvl = 42,
        .species = SPECIES_TAUROS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperSebastian[] = {
    {
        .lvl = 29,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 29,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperPerry[] = {
    {
        .lvl = 25,
        .species = SPECIES_SPEAROW,
    },
    {
        .lvl = 25,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 25,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 25,
        .species = SPECIES_SPEAROW,
    },
    {
        .lvl = 25,
        .species = SPECIES_SPEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperRobert[] = {
    {
        .lvl = 26,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 26,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .lvl = 26,
        .species = SPECIES_SPEAROW,
    },
    {
        .lvl = 26,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperDonald[] = {
    {
        .lvl = 33,
        .species = SPECIES_FARFETCHD,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperBenny[] = {
    {
        .lvl = 29,
        .species = SPECIES_SPEAROW,
    },
    {
        .lvl = 29,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperEdwin[] = {
    {
        .lvl = 26,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .lvl = 26,
        .species = SPECIES_FARFETCHD,
    },
    {
        .lvl = 26,
        .species = SPECIES_DODUO,
    },
    {
        .lvl = 26,
        .species = SPECIES_PIDGEY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperChester[] = {
    {
        .lvl = 28,
        .species = SPECIES_DODRIO,
    },
    {
        .lvl = 28,
        .species = SPECIES_DODUO,
    },
    {
        .lvl = 28,
        .species = SPECIES_DODUO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperWilton[] = {
    {
        .lvl = 29,
        .species = SPECIES_SPEAROW,
    },
    {
        .lvl = 29,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperRamiro[] = {
    {
        .lvl = 34,
        .species = SPECIES_DODRIO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperJacob[] = {
    {
        .lvl = 26,
        .species = SPECIES_SPEAROW,
    },
    {
        .lvl = 26,
        .species = SPECIES_SPEAROW,
    },
    {
        .lvl = 26,
        .species = SPECIES_FEAROW,
    },
    {
        .lvl = 26,
        .species = SPECIES_SPEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperRoger[] = {
    {
        .lvl = 30,
        .species = SPECIES_FEAROW,
    },
    {
        .lvl = 30,
        .species = SPECIES_FEAROW,
    },
    {
        .lvl = 30,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperReed[] = {
    {
        .lvl = 39,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .lvl = 39,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .lvl = 39,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 39,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperKeith[] = {
    {
        .lvl = 39,
        .species = SPECIES_FARFETCHD,
    },
    {
        .lvl = 39,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperCarter[] = {
    {
        .lvl = 28,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 28,
        .species = SPECIES_DODUO,
    },
    {
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperMitch[] = {
    {
        .lvl = 26,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 26,
        .species = SPECIES_SPEAROW,
    },
    {
        .lvl = 26,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 26,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperBeck[] = {
    {
        .lvl = 29,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .lvl = 29,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperMarlon[] = {
    {
        .lvl = 28,
        .species = SPECIES_SPEAROW,
    },
    {
        .lvl = 28,
        .species = SPECIES_DODUO,
    },
    {
        .lvl = 28,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltKoichi[] = {
    {
        .iv = 100,
        .lvl = 37,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 100,
        .lvl = 37,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltMike[] = {
    {
        .iv = 100,
        .lvl = 31,
        .species = SPECIES_MANKEY,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 100,
        .lvl = 31,
        .species = SPECIES_MANKEY,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 100,
        .lvl = 31,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltHideki[] = {
    {
        .iv = 100,
        .lvl = 32,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 100,
        .lvl = 32,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltAaron[] = {
    {
        .iv = 100,
        .lvl = 36,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltHitoshi[] = {
    {
        .iv = 100,
        .lvl = 31,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 100,
        .lvl = 31,
        .species = SPECIES_MANKEY,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 100,
        .lvl = 31,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltAtsushi[] = {
    {
        .iv = 100,
        .lvl = 40,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 100,
        .lvl = 40,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltKiyo[] = {
    {
        .iv = 100,
        .lvl = 43,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltTakashi[] = {
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltDaisuke[] = {
    {
        .iv = 100,
        .lvl = 43,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 100,
        .lvl = 43,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 100,
        .lvl = 43,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRivalOaksLabSquirtle[] = {
    {
        .lvl = 5,
        .species = SPECIES_SQUIRTLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRivalOaksLabBulbasaur[] = {
    {
        .lvl = 5,
        .species = SPECIES_BULBASAUR,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRivalOaksLabCharmander[] = {
    {
        .lvl = 5,
        .species = SPECIES_CHARMANDER,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerRivalRoute22EarlySquirtle[] = {
    {
        .iv = 50,
        .lvl = 9,
        .species = SPECIES_PIDGEY,
        .moves = {MOVE_TACKLE, MOVE_SANDATTACK, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = 50,
        .lvl = 9,
        .species = SPECIES_SQUIRTLE,
        .moves = {MOVE_TACKLE, MOVE_TAILWHIP, MOVE_NONE, MOVE_NONE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerRivalRoute22EarlyBulbasaur[] = {
    {
        .iv = 50,
        .lvl = 9,
        .species = SPECIES_PIDGEY,
        .moves = {MOVE_TACKLE, MOVE_SANDATTACK, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = 50,
        .lvl = 9,
        .species = SPECIES_BULBASAUR,
        .moves = {MOVE_TACKLE, MOVE_GROWL, MOVE_NONE, MOVE_NONE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerRivalRoute22EarlyCharmander[] = {
    {
        .iv = 50,
        .lvl = 9,
        .species = SPECIES_PIDGEY,
        .moves = {MOVE_TACKLE, MOVE_SANDATTACK, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = 50,
        .lvl = 9,
        .species = SPECIES_CHARMANDER,
        .moves = {MOVE_SCRATCH, MOVE_GROWL, MOVE_NONE, MOVE_NONE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerRivalCeruleanSquirtle[] = {
    {
        .iv = 50,
        .lvl = 17,
        .species = SPECIES_PIDGEOTTO,
        .moves = {MOVE_TACKLE, MOVE_SANDATTACK, MOVE_GUST, MOVE_QUICKATTACK},
    },
    {
        .iv = 50,
        .lvl = 16,
        .species = SPECIES_ABRA,
        .moves = {MOVE_TELEPORT, MOVE_NONE, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = 50,
        .lvl = 15,
        .species = SPECIES_RATTATA,
        .moves = {MOVE_TACKLE, MOVE_TAILWHIP, MOVE_QUICKATTACK, MOVE_NONE},
    },
    {
        .iv = 100,
        .lvl = 18,
        .species = SPECIES_SQUIRTLE,
        .moves = {MOVE_TACKLE, MOVE_TAILWHIP, MOVE_WITHDRAW, MOVE_WATERGUN},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerRivalCeruleanBulbasaur[] = {
    {
        .iv = 50,
        .lvl = 17,
        .species = SPECIES_PIDGEOTTO,
        .moves = {MOVE_TACKLE, MOVE_SANDATTACK, MOVE_GUST, MOVE_QUICKATTACK},
    },
    {
        .iv = 50,
        .lvl = 16,
        .species = SPECIES_ABRA,
        .moves = {MOVE_TELEPORT, MOVE_NONE, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = 50,
        .lvl = 15,
        .species = SPECIES_RATTATA,
        .moves = {MOVE_TACKLE, MOVE_TAILWHIP, MOVE_QUICKATTACK, MOVE_NONE},
    },
    {
        .iv = 100,
        .lvl = 18,
        .species = SPECIES_BULBASAUR,
        .moves = {MOVE_SLEEPPOWDER, MOVE_POISONPOWDER, MOVE_VINEWHIP, MOVE_LEECHSEED},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerRivalCeruleanCharmander[] = {
    {
        .iv = 50,
        .lvl = 17,
        .species = SPECIES_PIDGEOTTO,
        .moves = {MOVE_TACKLE, MOVE_SANDATTACK, MOVE_GUST, MOVE_QUICKATTACK},
    },
    {
        .iv = 50,
        .lvl = 16,
        .species = SPECIES_ABRA,
        .moves = {MOVE_TELEPORT, MOVE_NONE, MOVE_NONE, MOVE_NONE},
    },
    {
        .iv = 50,
        .lvl = 15,
        .species = SPECIES_RATTATA,
        .moves = {MOVE_TACKLE, MOVE_TAILWHIP, MOVE_QUICKATTACK, MOVE_NONE},
    },
    {
        .iv = 100,
        .lvl = 18,
        .species = SPECIES_CHARMANDER,
        .moves = {MOVE_METALCLAW, MOVE_EMBER, MOVE_GROWL, MOVE_SCRATCH},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerScientistTed[] = {
    {
        .lvl = 29,
        .species = SPECIES_ELECTRODE,
    },
    {
        .lvl = 29,
        .species = SPECIES_WEEZING,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerScientistConnor[] = {
    {
        .lvl = 26,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE},
    },
    {
        .lvl = 26,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_SELFDESTRUCT},
    },
    {
        .lvl = 26,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerScientistJerry[] = {
    {
        .lvl = 28,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .lvl = 28,
        .species = SPECIES_VOLTORB,
    },
    {
        .lvl = 28,
        .species = SPECIES_MAGNETON,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerScientistJose[] = {
    {
        .lvl = 29,
        .species = SPECIES_ELECTRODE,
        .moves = {MOVE_SPARK, MOVE_SONICBOOM, MOVE_SCREECH, MOVE_TACKLE},
    },
    {
        .lvl = 29,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerScientistRodney[] = {
    {
        .lvl = 33,
        .species = SPECIES_ELECTRODE,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerScientistBeau[] = {
    {
        .lvl = 26,
        .species = SPECIES_MAGNETON,
        .moves = {MOVE_SPARK, MOVE_THUNDERWAVE, MOVE_SONICBOOM, MOVE_SUPERSONIC},
    },
    {
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 26,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 26,
        .species = SPECIES_MAGNEMITE,
        .moves = {MOVE_SPARK, MOVE_THUNDERWAVE, MOVE_SONICBOOM, MOVE_SUPERSONIC},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerScientistTaylor[] = {
    {
        .lvl = 25,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SPARK, MOVE_SONICBOOM, MOVE_SCREECH, MOVE_TACKLE},
    },
    {
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 25,
        .species = SPECIES_MAGNETON,
        .moves = {MOVE_THUNDERWAVE, MOVE_SONICBOOM, MOVE_SUPERSONIC, MOVE_THUNDERSHOCK},
    },
    {
        .lvl = 25,
        .species = SPECIES_MAGNEMITE,
        .moves = {MOVE_THUNDERWAVE, MOVE_SONICBOOM, MOVE_SUPERSONIC, MOVE_THUNDERSHOCK},
    },
    {
        .lvl = 25,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_SELFDESTRUCT},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerScientistJoshua[] = {
    {
        .lvl = 29,
        .species = SPECIES_ELECTRODE,
    },
    {
        .lvl = 29,
        .species = SPECIES_MUK,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerScientistParker[] = {
    {
        .lvl = 29,
        .species = SPECIES_GRIMER,
    },
    {
        .lvl = 29,
        .species = SPECIES_ELECTRODE,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerScientistEd[] = {
    {
        .lvl = 28,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SPARK, MOVE_SONICBOOM, MOVE_SCREECH, MOVE_TACKLE},
    },
    {
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_TACKLE, MOVE_SMOG},
    },
    {
        .lvl = 28,
        .species = SPECIES_MAGNETON,
        .moves = {MOVE_SPARK, MOVE_THUNDERWAVE, MOVE_SONICBOOM, MOVE_SUPERSONIC},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerScientistTravis[] = {
    {
        .lvl = 29,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .lvl = 29,
        .species = SPECIES_KOFFING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerScientistBraydon[] = {
    {
        .lvl = 33,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .lvl = 33,
        .species = SPECIES_MAGNETON,
    },
    {
        .lvl = 33,
        .species = SPECIES_VOLTORB,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerScientistIvan[] = {
    {
        .lvl = 34,
        .species = SPECIES_MAGNEMITE,
    },
    {
        .lvl = 34,
        .species = SPECIES_ELECTRODE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBossGiovanni[] = {
    {
        .lvl = 25,
        .species = SPECIES_ONIX,
    },
    {
        .lvl = 24,
        .species = SPECIES_RHYHORN,
    },
    {
        .lvl = 29,
        .species = SPECIES_KANGASKHAN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBossGiovanni2[] = {
    {
        .lvl = 37,
        .species = SPECIES_NIDORINO,
    },
    {
        .lvl = 35,
        .species = SPECIES_KANGASKHAN,
    },
    {
        .lvl = 37,
        .species = SPECIES_RHYHORN,
    },
    {
        .lvl = 41,
        .species = SPECIES_NIDOQUEEN,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerLeaderGiovanni[] = {
    {
        .lvl = 45,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKEDOWN, MOVE_ROCKBLAST, MOVE_SCARYFACE, MOVE_EARTHQUAKE},
    },
    {
        .lvl = 42,
        .species = SPECIES_DUGTRIO,
        .moves = {MOVE_SLASH, MOVE_SANDTOMB, MOVE_MUDSLAP, MOVE_EARTHQUAKE},
    },
    {
        .lvl = 44,
        .species = SPECIES_NIDOQUEEN,
        .moves = {MOVE_BODYSLAM, MOVE_DOUBLEKICK, MOVE_POISONSTING, MOVE_EARTHQUAKE},
    },
    {
        .lvl = 45,
        .species = SPECIES_NIDOKING,
        .moves = {MOVE_THRASH, MOVE_DOUBLEKICK, MOVE_POISONSTING, MOVE_EARTHQUAKE},
    },
    {
        .lvl = 50,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKEDOWN, MOVE_ROCKBLAST, MOVE_SCARYFACE, MOVE_EARTHQUAKE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt[] = {
    {
        .lvl = 13,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 13,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt2[] = {
    {
        .lvl = 11,
        .species = SPECIES_SANDSHREW,
    },
    {
        .lvl = 11,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 11,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt3[] = {
    {
        .lvl = 11,
        .species = SPECIES_ZUBAT,
    },
    {
        .lvl = 11,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt4[] = {
    {
        .lvl = 13,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 13,
        .species = SPECIES_SANDSHREW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt5[] = {
    {
        .lvl = 17,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 17,
        .species = SPECIES_DROWZEE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt6[] = {
    {
        .lvl = 15,
        .species = SPECIES_EKANS,
    },
    {
        .lvl = 15,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt7[] = {
    {
        .lvl = 20,
        .species = SPECIES_RATICATE,
    },
    {
        .lvl = 20,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt8[] = {
    {
        .lvl = 21,
        .species = SPECIES_DROWZEE,
    },
    {
        .lvl = 21,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt9[] = {
    {
        .lvl = 21,
        .species = SPECIES_RATICATE,
    },
    {
        .lvl = 21,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerTeamRocketGrunt10[] = {
    {
        .lvl = 20,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE, MOVE_HARDEN},
    },
    {
        .lvl = 20,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOG, MOVE_TACKLE, MOVE_POISONGAS, MOVE_NONE},
    },
    {
        .lvl = 20,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOG, MOVE_TACKLE, MOVE_POISONGAS, MOVE_NONE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt11[] = {
    {
        .lvl = 19,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 19,
        .species = SPECIES_RATICATE,
    },
    {
        .lvl = 19,
        .species = SPECIES_RATICATE,
    },
    {
        .lvl = 19,
        .species = SPECIES_RATTATA,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerTeamRocketGrunt12[] = {
    {
        .lvl = 22,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE, MOVE_HARDEN},
    },
    {
        .lvl = 22,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE, MOVE_POISONGAS},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerTeamRocketGrunt13[] = {
    {
        .lvl = 17,
        .species = SPECIES_ZUBAT,
        .moves = {MOVE_BITE, MOVE_ASTONISH, MOVE_SUPERSONIC, MOVE_LEECHLIFE},
    },
    {
        .lvl = 17,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOG, MOVE_TACKLE, MOVE_POISONGAS, MOVE_NONE},
    },
    {
        .lvl = 17,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_SLUDGE, MOVE_DISABLE, MOVE_HARDEN, MOVE_POUND},
    },
    {
        .lvl = 17,
        .species = SPECIES_ZUBAT,
        .moves = {MOVE_BITE, MOVE_ASTONISH, MOVE_SUPERSONIC, MOVE_LEECHLIFE},
    },
    {
        .lvl = 17,
        .species = SPECIES_RATICATE,
        .moves = {MOVE_HYPERFANG, MOVE_QUICKATTACK, MOVE_TAILWHIP, MOVE_TACKLE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt14[] = {
    {
        .lvl = 20,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 20,
        .species = SPECIES_RATICATE,
    },
    {
        .lvl = 20,
        .species = SPECIES_DROWZEE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt15[] = {
    {
        .lvl = 21,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 21,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt16[] = {
    {
        .lvl = 23,
        .species = SPECIES_SANDSHREW,
    },
    {
        .lvl = 23,
        .species = SPECIES_EKANS,
    },
    {
        .lvl = 23,
        .species = SPECIES_SANDSLASH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt17[] = {
    {
        .lvl = 23,
        .species = SPECIES_EKANS,
    },
    {
        .lvl = 23,
        .species = SPECIES_SANDSHREW,
    },
    {
        .lvl = 23,
        .species = SPECIES_ARBOK,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt18[] = {
    {
        .lvl = 21,
        .species = SPECIES_KOFFING,
    },
    {
        .lvl = 21,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt19[] = {
    {
        .lvl = 25,
        .species = SPECIES_ZUBAT,
    },
    {
        .lvl = 25,
        .species = SPECIES_ZUBAT,
    },
    {
        .lvl = 25,
        .species = SPECIES_GOLBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt20[] = {
    {
        .lvl = 26,
        .species = SPECIES_KOFFING,
    },
    {
        .lvl = 26,
        .species = SPECIES_DROWZEE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt21[] = {
    {
        .lvl = 23,
        .species = SPECIES_ZUBAT,
    },
    {
        .lvl = 23,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 23,
        .species = SPECIES_RATICATE,
    },
    {
        .lvl = 23,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt22[] = {
    {
        .lvl = 26,
        .species = SPECIES_DROWZEE,
    },
    {
        .lvl = 26,
        .species = SPECIES_KOFFING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt23[] = {
    {
        .lvl = 29,
        .species = SPECIES_CUBONE,
    },
    {
        .lvl = 29,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt24[] = {
    {
        .lvl = 25,
        .species = SPECIES_GOLBAT,
    },
    {
        .lvl = 25,
        .species = SPECIES_ZUBAT,
    },
    {
        .lvl = 25,
        .species = SPECIES_ZUBAT,
    },
    {
        .lvl = 25,
        .species = SPECIES_RATICATE,
    },
    {
        .lvl = 25,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt25[] = {
    {
        .lvl = 28,
        .species = SPECIES_RATICATE,
    },
    {
        .lvl = 28,
        .species = SPECIES_HYPNO,
    },
    {
        .lvl = 28,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt26[] = {
    {
        .lvl = 29,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 29,
        .species = SPECIES_DROWZEE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt27[] = {
    {
        .lvl = 28,
        .species = SPECIES_EKANS,
    },
    {
        .lvl = 28,
        .species = SPECIES_ZUBAT,
    },
    {
        .lvl = 28,
        .species = SPECIES_CUBONE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt28[] = {
    {
        .lvl = 33,
        .species = SPECIES_ARBOK,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt29[] = {
    {
        .lvl = 33,
        .species = SPECIES_HYPNO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt30[] = {
    {
        .lvl = 29,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 29,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt31[] = {
    {
        .lvl = 28,
        .species = SPECIES_ZUBAT,
    },
    {
        .lvl = 28,
        .species = SPECIES_ZUBAT,
    },
    {
        .lvl = 28,
        .species = SPECIES_GOLBAT,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerTeamRocketGrunt32[] = {
    {
        .lvl = 26,
        .species = SPECIES_RATICATE,
        .moves = {MOVE_SCARYFACE, MOVE_HYPERFANG, MOVE_QUICKATTACK, MOVE_TAILWHIP},
    },
    {
        .lvl = 26,
        .species = SPECIES_ARBOK,
        .moves = {MOVE_GLARE, MOVE_BITE, MOVE_POISONSTING, MOVE_LEER},
    },
    {
        .lvl = 26,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 26,
        .species = SPECIES_GOLBAT,
        .moves = {MOVE_WINGATTACK, MOVE_BITE, MOVE_ASTONISH, MOVE_SUPERSONIC},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt33[] = {
    {
        .lvl = 29,
        .species = SPECIES_CUBONE,
    },
    {
        .lvl = 29,
        .species = SPECIES_CUBONE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt34[] = {
    {
        .lvl = 29,
        .species = SPECIES_SANDSHREW,
    },
    {
        .lvl = 29,
        .species = SPECIES_SANDSLASH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt35[] = {
    {
        .lvl = 26,
        .species = SPECIES_RATICATE,
    },
    {
        .lvl = 26,
        .species = SPECIES_ZUBAT,
    },
    {
        .lvl = 26,
        .species = SPECIES_GOLBAT,
    },
    {
        .lvl = 26,
        .species = SPECIES_RATTATA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt36[] = {
    {
        .lvl = 28,
        .species = SPECIES_WEEZING,
    },
    {
        .lvl = 28,
        .species = SPECIES_GOLBAT,
    },
    {
        .lvl = 28,
        .species = SPECIES_KOFFING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt37[] = {
    {
        .lvl = 28,
        .species = SPECIES_DROWZEE,
    },
    {
        .lvl = 28,
        .species = SPECIES_GRIMER,
    },
    {
        .lvl = 28,
        .species = SPECIES_MACHOP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt38[] = {
    {
        .lvl = 28,
        .species = SPECIES_GOLBAT,
    },
    {
        .lvl = 28,
        .species = SPECIES_DROWZEE,
    },
    {
        .lvl = 28,
        .species = SPECIES_HYPNO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt39[] = {
    {
        .lvl = 33,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt40[] = {
    {
        .lvl = 25,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 25,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 25,
        .species = SPECIES_ZUBAT,
    },
    {
        .lvl = 25,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 25,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt41[] = {
    {
        .lvl = 32,
        .species = SPECIES_CUBONE,
    },
    {
        .lvl = 32,
        .species = SPECIES_DROWZEE,
    },
    {
        .lvl = 32,
        .species = SPECIES_MAROWAK,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerSamuel[] = {
    {
        .iv = 100,
        .lvl = 37,
        .species = SPECIES_SANDSLASH,
        .moves = {MOVE_SLASH, MOVE_SWIFT, MOVE_SANDATTACK, MOVE_POISONSTING},
    },
    {
        .iv = 100,
        .lvl = 37,
        .species = SPECIES_SANDSLASH,
        .moves = {MOVE_SLASH, MOVE_SWIFT, MOVE_SANDATTACK, MOVE_POISONSTING},
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKEDOWN, MOVE_ROCKBLAST, MOVE_FURYATTACK, MOVE_SCARYFACE},
    },
    {
        .iv = 100,
        .lvl = 39,
        .species = SPECIES_NIDORINO,
        .moves = {MOVE_FURYATTACK, MOVE_HORNATTACK, MOVE_POISONSTING, MOVE_DOUBLEKICK},
    },
    {
        .iv = 100,
        .lvl = 39,
        .species = SPECIES_NIDOKING,
        .moves = {MOVE_THRASH, MOVE_DOUBLEKICK, MOVE_POISONSTING, MOVE_FOCUSENERGY},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerGeorge[] = {
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_EXEGGUTOR,
        .moves = {MOVE_EGGBOMB, MOVE_CONFUSION, MOVE_STUNSPORE, MOVE_SLEEPPOWDER},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_SANDSLASH,
        .moves = {MOVE_FURYSWIPES, MOVE_SWIFT, MOVE_POISONSTING, MOVE_SANDATTACK},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_CLOYSTER,
        .moves = {MOVE_SPIKECANNON, MOVE_SPIKES, MOVE_AURORABEAM, MOVE_SUPERSONIC},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_ELECTRODE,
        .moves = {MOVE_SPARK, MOVE_SONICBOOM, MOVE_SCREECH, MOVE_LIGHTSCREEN},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_ARCANINE,
        .moves = {MOVE_FLAMEWHEEL, MOVE_ROAR, MOVE_BITE, MOVE_TAKEDOWN},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerColby[] = {
    {
        .iv = 100,
        .lvl = 41,
        .species = SPECIES_KINGLER,
        .moves = {MOVE_GUILLOTINE, MOVE_STOMP, MOVE_MUDSHOT, MOVE_BUBBLE},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_POLIWHIRL,
        .moves = {MOVE_BODYSLAM, MOVE_DOUBLESLAP, MOVE_WATERGUN, MOVE_HYPNOSIS},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_TENTACRUEL,
        .moves = {MOVE_BARRIER, MOVE_WRAP, MOVE_BUBBLEBEAM, MOVE_ACID},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_SEADRA,
        .moves = {MOVE_WATERGUN, MOVE_SMOKESCREEN, MOVE_TWISTER, MOVE_LEER},
    },
    {
        .iv = 100,
        .lvl = 43,
        .species = SPECIES_BLASTOISE,
        .moves = {MOVE_WATERGUN, MOVE_BITE, MOVE_RAPIDSPIN, MOVE_RAINDANCE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerPaul[] = {
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_SLOWPOKE,
        .moves = {MOVE_HEADBUTT, MOVE_CONFUSION, MOVE_WATERGUN, MOVE_DISABLE},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_SHELLDER,
        .moves = {MOVE_AURORABEAM, MOVE_CLAMP, MOVE_SUPERSONIC, MOVE_LEER},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_KINGLER,
        .moves = {MOVE_GUILLOTINE, MOVE_STOMP, MOVE_MUDSHOT, MOVE_BUBBLE},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_STARMIE,
        .moves = {MOVE_BUBBLEBEAM, MOVE_SWIFT, MOVE_RECOVER, MOVE_RAPIDSPIN},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_GOLDUCK,
        .moves = {MOVE_CONFUSION, MOVE_SCRATCH, MOVE_SCREECH, MOVE_DISABLE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerRolando[] = {
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_RATICATE,
        .moves = {MOVE_SUPERFANG, MOVE_PURSUIT, MOVE_SCARYFACE, MOVE_QUICKATTACK},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_IVYSAUR,
        .moves = {MOVE_RAZORLEAF, MOVE_SLEEPPOWDER, MOVE_SWEETSCENT, MOVE_SYNTHESIS},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_WARTORTLE,
        .moves = {MOVE_WATERGUN, MOVE_BITE, MOVE_RAPIDSPIN, MOVE_TAILWHIP},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_CHARMELEON,
        .moves = {MOVE_FLAMETHROWER, MOVE_SLASH, MOVE_SMOKESCREEN, MOVE_SCARYFACE},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_CHARIZARD,
        .moves = {MOVE_FLAMETHROWER, MOVE_WINGATTACK, MOVE_SMOKESCREEN, MOVE_SCARYFACE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerGilbert[] = {
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_PIDGEOTTO,
        .moves = {MOVE_WINGATTACK, MOVE_FEATHERDANCE, MOVE_WHIRLWIND, MOVE_QUICKATTACK},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_FEAROW,
        .moves = {MOVE_DRILLPECK, MOVE_MIRRORMOVE, MOVE_PURSUIT, MOVE_LEER},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_PERSIAN,
        .moves = {MOVE_PAYDAY, MOVE_FEINTATTACK, MOVE_SCREECH, MOVE_BITE},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_LICKITUNG,
        .moves = {MOVE_SLAM, MOVE_DISABLE, MOVE_WRAP, MOVE_SUPERSONIC},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_TAUROS,
        .moves = {MOVE_HORNATTACK, MOVE_SCARYFACE, MOVE_SWAGGER, MOVE_TAILWHIP},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerOwen[] = {
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_NIDORINO,
        .moves = {MOVE_SCRATCH, MOVE_POISONSTING, MOVE_DOUBLEKICK, MOVE_BITE},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_NIDORINA,
        .moves = {MOVE_HORNATTACK, MOVE_POISONSTING, MOVE_DOUBLEKICK, MOVE_LEER},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_RATICATE,
        .moves = {MOVE_SUPERFANG, MOVE_PURSUIT, MOVE_SCARYFACE, MOVE_QUICKATTACK},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_SANDSLASH,
        .moves = {MOVE_FURYSWIPES, MOVE_SWIFT, MOVE_SLASH, MOVE_POISONSTING},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_ROCKBLAST, MOVE_SCARYFACE, MOVE_STOMP, MOVE_TAILWHIP},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerBerke[] = {
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_SEEL,
        .moves = {MOVE_TAKEDOWN, MOVE_AURORABEAM, MOVE_ICYWIND, MOVE_GROWL},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_ROCKBLAST, MOVE_MAGNITUDE, MOVE_ROCKTHROW, MOVE_MUDSPORT},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_KINGLER,
        .moves = {MOVE_GUILLOTINE, MOVE_STOMP, MOVE_MUDSHOT, MOVE_BUBBLE},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_ONIX,
        .moves = {MOVE_SLAM, MOVE_SANDSTORM, MOVE_DRAGONBREATH, MOVE_ROCKTHROW},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_CLOYSTER,
        .moves = {MOVE_SPIKECANNON, MOVE_AURORABEAM, MOVE_SUPERSONIC, MOVE_PROTECT},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerYuji[] = {
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_SANDSLASH,
        .moves = {MOVE_SLASH, MOVE_SWIFT, MOVE_SANDATTACK, MOVE_POISONSTING},
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_ROCKBLAST, MOVE_MAGNITUDE, MOVE_MUDSPORT, MOVE_DEFENSECURL},
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_ONIX,
        .moves = {MOVE_DRAGONBREATH, MOVE_SANDSTORM, MOVE_ROCKTHROW, MOVE_BIND},
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_ROCKBLAST, MOVE_MAGNITUDE, MOVE_ROLLOUT, MOVE_DEFENSECURL},
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_MAROWAK,
        .moves = {MOVE_BONEMERANG, MOVE_HEADBUTT, MOVE_LEER, MOVE_GROWL},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerWarren[] = {
    {
        .iv = 100,
        .lvl = 37,
        .species = SPECIES_MAROWAK,
        .moves = {MOVE_BONEMERANG, MOVE_HEADBUTT, MOVE_LEER, MOVE_GROWL},
    },
    {
        .iv = 100,
        .lvl = 37,
        .species = SPECIES_MAROWAK,
        .moves = {MOVE_BONEMERANG, MOVE_HEADBUTT, MOVE_LEER, MOVE_GROWL},
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKEDOWN, MOVE_ROCKBLAST, MOVE_FURYATTACK, MOVE_SCARYFACE},
    },
    {
        .iv = 100,
        .lvl = 39,
        .species = SPECIES_NIDORINA,
        .moves = {MOVE_FURYSWIPES, MOVE_BITE, MOVE_POISONSTING, MOVE_DOUBLEKICK},
    },
    {
        .iv = 100,
        .lvl = 39,
        .species = SPECIES_NIDOQUEEN,
        .moves = {MOVE_BODYSLAM, MOVE_BITE, MOVE_POISONSTING, MOVE_DOUBLEKICK},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerMary[] = {
    {
        .iv = 100,
        .lvl = 22,
        .species = SPECIES_BELLSPROUT,
        .moves = {MOVE_WRAP, MOVE_STUNSPORE, MOVE_POISONPOWDER, MOVE_GROWTH},
    },
    {
        .iv = 100,
        .lvl = 22,
        .species = SPECIES_ODDISH,
        .moves = {MOVE_POISONPOWDER, MOVE_STUNSPORE, MOVE_ABSORB, MOVE_SWEETSCENT},
    },
    {
        .iv = 100,
        .lvl = 22,
        .species = SPECIES_WEEPINBELL,
        .moves = {MOVE_VINEWHIP, MOVE_STUNSPORE, MOVE_POISONPOWDER, MOVE_GROWTH},
    },
    {
        .iv = 100,
        .lvl = 22,
        .species = SPECIES_GLOOM,
        .moves = {MOVE_ABSORB, MOVE_STUNSPORE, MOVE_POISONPOWDER, MOVE_SWEETSCENT},
    },
    {
        .iv = 100,
        .lvl = 22,
        .species = SPECIES_IVYSAUR,
        .moves = {MOVE_RAZORLEAF, MOVE_SLEEPPOWDER, MOVE_VINEWHIP, MOVE_LEECHSEED},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerCaroline[] = {
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_BELLSPROUT,
        .moves = {MOVE_RAZORLEAF, MOVE_ACID, MOVE_STUNSPORE, MOVE_POISONPOWDER},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_WEEPINBELL,
        .moves = {MOVE_RAZORLEAF, MOVE_ACID, MOVE_SLEEPPOWDER, MOVE_POISONPOWDER},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_VICTREEBEL,
        .moves = {MOVE_RAZORLEAF, MOVE_ACID, MOVE_STUNSPORE, MOVE_SLEEPPOWDER},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_PARAS,
        .moves = {MOVE_GROWTH, MOVE_SLASH, MOVE_LEECHLIFE, MOVE_STUNSPORE},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_PARASECT,
        .moves = {MOVE_SPORE, MOVE_SLASH, MOVE_LEECHLIFE, MOVE_POISONPOWDER},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerAlexa[] = {
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_CLEFAIRY,
        .moves = {MOVE_LIGHTSCREEN, MOVE_COSMICPOWER, MOVE_DOUBLESLAP, MOVE_ENCORE},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_JIGGLYPUFF,
        .moves = {MOVE_SING, MOVE_BODYSLAM, MOVE_ROLLOUT, MOVE_DISABLE},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_PERSIAN,
        .moves = {MOVE_PAYDAY, MOVE_FEINTATTACK, MOVE_BITE, MOVE_SCREECH},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_DEWGONG,
        .moves = {MOVE_SHEERCOLD, MOVE_TAKEDOWN, MOVE_REST, MOVE_AURORABEAM},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_CHANSEY,
        .moves = {MOVE_SING, MOVE_EGGBOMB, MOVE_SOFTBOILED, MOVE_MINIMIZE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerShannon[] = {
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_BEEDRILL,
        .moves = {MOVE_PINMISSILE, MOVE_TWINEEDLE, MOVE_AGILITY, MOVE_PURSUIT},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_BUTTERFREE,
        .moves = {MOVE_SAFEGUARD, MOVE_PSYBEAM, MOVE_GUST, MOVE_SUPERSONIC},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_PARASECT,
        .moves = {MOVE_SPORE, MOVE_LEECHLIFE, MOVE_SLASH, MOVE_GROWTH},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_VENONAT,
        .moves = {MOVE_PSYBEAM, MOVE_STUNSPORE, MOVE_LEECHLIFE, MOVE_DISABLE},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_VENOMOTH,
        .moves = {MOVE_PSYBEAM, MOVE_GUST, MOVE_SUPERSONIC, MOVE_LEECHLIFE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerNaomi[] = {
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_PERSIAN,
        .moves = {MOVE_FURYSWIPES, MOVE_SCREECH, MOVE_FEINTATTACK, MOVE_PAYDAY},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_PONYTA,
        .moves = {MOVE_AGILITY, MOVE_TAKEDOWN, MOVE_FIRESPIN, MOVE_STOMP},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_RAPIDASH,
        .moves = {MOVE_FURYATTACK, MOVE_FIRESPIN, MOVE_STOMP, MOVE_GROWL},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_VULPIX,
        .moves = {MOVE_FLAMETHROWER, MOVE_CONFUSERAY, MOVE_QUICKATTACK, MOVE_IMPRISON},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_NINETALES,
        .moves = {MOVE_SAFEGUARD, MOVE_WILLOWISP, MOVE_CONFUSERAY, MOVE_FIRESPIN},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerBrooke[] = {
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_TANGELA,
        .moves = {MOVE_SLAM, MOVE_BIND, MOVE_MEGADRAIN, MOVE_INGRAIN},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_GLOOM,
        .moves = {MOVE_ACID, MOVE_MOONLIGHT, MOVE_SLEEPPOWDER, MOVE_STUNSPORE},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_VILEPLUME,
        .moves = {MOVE_MEGADRAIN, MOVE_ACID, MOVE_STUNSPORE, MOVE_AROMATHERAPY},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_IVYSAUR,
        .moves = {MOVE_RAZORLEAF, MOVE_SWEETSCENT, MOVE_GROWL, MOVE_LEECHSEED},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_VENUSAUR,
        .moves = {MOVE_RAZORLEAF, MOVE_GROWTH, MOVE_SLEEPPOWDER, MOVE_POISONPOWDER},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerAustina[] = {
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_HORNDRILL, MOVE_ROCKBLAST, MOVE_SCARYFACE, MOVE_STOMP},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_NIDORINA,
        .moves = {MOVE_DOUBLEKICK, MOVE_FURYSWIPES, MOVE_BITE, MOVE_FLATTER},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_NIDOQUEEN,
        .moves = {MOVE_BODYSLAM, MOVE_DOUBLEKICK, MOVE_BITE, MOVE_GROWL},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_NIDORINO,
        .moves = {MOVE_HORNATTACK, MOVE_POISONSTING, MOVE_FOCUSENERGY, MOVE_LEER},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_NIDOKING,
        .moves = {MOVE_THRASH, MOVE_DOUBLEKICK, MOVE_POISONSTING, MOVE_PECK},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerJulie[] = {
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_PERSIAN,
        .moves = {MOVE_FURYSWIPES, MOVE_BITE, MOVE_SCREECH, MOVE_FEINTATTACK},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_NINETALES,
        .moves = {MOVE_FLAMETHROWER, MOVE_WILLOWISP, MOVE_CONFUSERAY, MOVE_GRUDGE},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_RAPIDASH,
        .moves = {MOVE_FURYATTACK, MOVE_FIRESPIN, MOVE_TAKEDOWN, MOVE_AGILITY},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_PIKACHU,
        .moves = {MOVE_THUNDERBOLT, MOVE_THUNDERWAVE, MOVE_DOUBLETEAM, MOVE_QUICKATTACK},
    },
    {
        .iv = 100,
        .lvl = 42,
        .species = SPECIES_RAICHU,
        .moves = {MOVE_THUNDER, MOVE_THUNDERWAVE, MOVE_SLAM, MOVE_DOUBLETEAM},
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerEliteFourLorelei[] = {
    {
        .lvl = 52,
        .species = SPECIES_DEWGONG,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_ICEBEAM, MOVE_SURF, MOVE_HAIL, MOVE_SAFEGUARD},
        .teraType = TYPE_ICE,
    },
    {
        .lvl = 51,
        .species = SPECIES_CLOYSTER,
        .ability = Ability_RandomAll,
        .nature = NATURE_BOLD,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SPIKES, MOVE_PROTECT, MOVE_HAIL, MOVE_DIVE},
        .teraType = TYPE_ICE,
    },
    {
        .lvl = 52,
        .species = SPECIES_SLOWBRO,
        .ability = Ability_RandomAll,
        .nature = NATURE_CALM,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_ICEBEAM, MOVE_SURF, MOVE_AMNESIA, MOVE_YAWN},
        .teraType = TYPE_ICE,
    },
    {
        .lvl = 54,
        .species = SPECIES_JYNX,
        .ability = Ability_RandomAll,
        .nature = NATURE_TIMID,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_ICEPUNCH, MOVE_DOUBLESLAP, MOVE_LOVELYKISS, MOVE_ATTRACT},
        .teraType = TYPE_ICE,
    },
    {
        .lvl = 54,
        .species = SPECIES_LAPRAS,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CONFUSERAY, MOVE_ICEBEAM, MOVE_SURF, MOVE_BODYSLAM},
        .teraType = TYPE_ICE,
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerEliteFourBruno[] = {
    {
        .lvl = 51,
        .species = SPECIES_ONIX,
        .ability = Ability_RandomAll,
        .nature = NATURE_IMPISH, // Def é o maior
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_EARTHQUAKE, MOVE_ROCKTOMB, MOVE_IRONTAIL, MOVE_ROAR},
        .teraType = TYPE_FIGHTING,
    },
    {
        .lvl = 53,
        .species = SPECIES_HITMONCHAN,
        .ability = Ability_RandomAll,
        .nature = NATURE_CAREFUL, // SpD é o maior
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SKYUPPERCUT, MOVE_MACHPUNCH, MOVE_ROCKTOMB, MOVE_COUNTER},
        .teraType = TYPE_FIGHTING,
    },
    {
        .lvl = 53,
        .species = SPECIES_HITMONLEE,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT, // Atk é o maior
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_MEGAKICK, MOVE_FORESIGHT, MOVE_BRICKBREAK, MOVE_FACADE},
        .teraType = TYPE_FIGHTING,
    },
    {
        .lvl = 54,
        .species = SPECIES_ONIX,
        .ability = Ability_RandomAll,
        .nature = NATURE_IMPISH, // Def é o maior
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_DOUBLEEDGE, MOVE_EARTHQUAKE, MOVE_IRONTAIL, MOVE_SANDTOMB},
        .teraType = TYPE_FIGHTING,
    },
    {
        .lvl = 56,
        .species = SPECIES_MACHAMP,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT, // Atk é o maior
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSSCHOP, MOVE_BULKUP, MOVE_SCARYFACE, MOVE_ROCKTOMB},
        .teraType = TYPE_FIGHTING,
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerEliteFourAgatha[] = {
    {
        .lvl = 54,
        .species = SPECIES_GENGAR,
        .ability = Ability_RandomAll,
        .nature = NATURE_TIMID,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SHADOWPUNCH, MOVE_CONFUSERAY, MOVE_TOXIC, MOVE_DOUBLETEAM},
        .teraType = TYPE_GHOST,
    },
    {
        .lvl = 54,
        .species = SPECIES_GOLBAT,
        .ability = Ability_RandomAll,
        .nature = NATURE_JOLLY,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_CONFUSERAY, MOVE_POISONFANG, MOVE_AIRCUTTER, MOVE_BITE},
        .teraType = TYPE_GHOST,
    },
    {
        .lvl = 53,
        .species = SPECIES_HAUNTER,
        .ability = Ability_RandomAll,
        .nature = NATURE_TIMID,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPNOSIS, MOVE_DREAMEATER, MOVE_CURSE, MOVE_MEANLOOK},
        .teraType = TYPE_GHOST,
    },
    {
        .lvl = 56,
        .species = SPECIES_ARBOK,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SLUDGEBOMB, MOVE_SCREECH, MOVE_IRONTAIL, MOVE_BITE},
        .teraType = TYPE_GHOST,
    },
    {
        .lvl = 58,
        .species = SPECIES_GENGAR,
        .ability = Ability_RandomAll,
        .nature = NATURE_TIMID,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SHADOWBALL, MOVE_SLUDGEBOMB, MOVE_HYPNOSIS, MOVE_NIGHTMARE},
        .teraType = TYPE_GHOST,
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerEliteFourLance[] = {
    {
        .lvl = 56,
        .species = SPECIES_GYARADOS,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPERBEAM, MOVE_DRAGONRAGE, MOVE_TWISTER, MOVE_BITE},
        .teraType = TYPE_DRAGON,
    },
    {
        .lvl = 54,
        .species = SPECIES_DRAGONAIR,
        .ability = Ability_RandomAll,
        .nature = NATURE_CALM,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPERBEAM, MOVE_SAFEGUARD, MOVE_DRAGONRAGE, MOVE_OUTRAGE},
        .teraType = TYPE_DRAGON,
    },
    {
        .lvl = 54,
        .species = SPECIES_DRAGONAIR,
        .ability = Ability_RandomAll,
        .nature = NATURE_CALM,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPERBEAM, MOVE_SAFEGUARD, MOVE_THUNDERWAVE, MOVE_OUTRAGE},
        .teraType = TYPE_DRAGON,
    },
    {
        .lvl = 58,
        .species = SPECIES_AERODACTYL,
        .ability = Ability_RandomAll,
        .nature = NATURE_JOLLY,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPERBEAM, MOVE_ANCIENTPOWER, MOVE_WINGATTACK, MOVE_SCARYFACE},
        .teraType = TYPE_DRAGON,
    },
    {
        .lvl = 60,
        .species = SPECIES_DRAGONITE,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYPERBEAM, MOVE_SAFEGUARD, MOVE_OUTRAGE, MOVE_WINGATTACK},
        .teraType = TYPE_DRAGON,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerLeaderBrock[] = {
    {
        .lvl = 12,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_TACKLE, MOVE_DEFENSECURL, MOVE_NONE, MOVE_NONE},
    },
    {
        .lvl = 14,
        .species = SPECIES_ONIX,
        .moves = {MOVE_TACKLE, MOVE_BIND, MOVE_ROCKTOMB, MOVE_NONE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerLeaderMisty[] = {
    {
        .lvl = 18,
        .species = SPECIES_STARYU,
        .moves = {MOVE_TACKLE, MOVE_HARDEN, MOVE_RECOVER, MOVE_WATERPULSE},
    },
    {
        .lvl = 21,
        .species = SPECIES_STARMIE,
        .moves = {MOVE_SWIFT, MOVE_RECOVER, MOVE_RAPIDSPIN, MOVE_WATERPULSE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerLeaderLtSurge[] = {
    {
        .lvl = 21,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SONICBOOM, MOVE_TACKLE, MOVE_SCREECH, MOVE_SHOCKWAVE},
    },
    {
        .lvl = 18,
        .species = SPECIES_PIKACHU,
        .moves = {MOVE_QUICKATTACK, MOVE_THUNDERWAVE, MOVE_DOUBLETEAM, MOVE_SHOCKWAVE},
    },
    {
        .lvl = 24,
        .species = SPECIES_RAICHU,
        .moves = {MOVE_QUICKATTACK, MOVE_THUNDERWAVE, MOVE_DOUBLETEAM, MOVE_SHOCKWAVE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerLeaderErika[] = {
    {
        .lvl = 29,
        .species = SPECIES_VICTREEBEL,
        .moves = {MOVE_STUNSPORE, MOVE_ACID, MOVE_POISONPOWDER, MOVE_GIGADRAIN},
    },
    {
        .lvl = 24,
        .species = SPECIES_TANGELA,
        .moves = {MOVE_POISONPOWDER, MOVE_CONSTRICT, MOVE_INGRAIN, MOVE_GIGADRAIN},
    },
    {
        .lvl = 29,
        .species = SPECIES_VILEPLUME,
        .moves = {MOVE_SLEEPPOWDER, MOVE_ACID, MOVE_STUNSPORE, MOVE_GIGADRAIN},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerLeaderKoga[] = {
    {
        .lvl = 37,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SELFDESTRUCT, MOVE_SLUDGE, MOVE_SMOKESCREEN, MOVE_TOXIC},
    },
    {
        .lvl = 39,
        .species = SPECIES_MUK,
        .moves = {MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_ACIDARMOR, MOVE_TOXIC},
    },
    {
        .lvl = 37,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SELFDESTRUCT, MOVE_SLUDGE, MOVE_SMOKESCREEN, MOVE_TOXIC},
    },
    {
        .lvl = 43,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_TACKLE, MOVE_SLUDGE, MOVE_SMOKESCREEN, MOVE_TOXIC},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerLeaderBlaine[] = {
    {
        .lvl = 42,
        .species = SPECIES_GROWLITHE,
        .moves = {MOVE_BITE, MOVE_ROAR, MOVE_TAKEDOWN, MOVE_FIREBLAST},
    },
    {
        .lvl = 40,
        .species = SPECIES_PONYTA,
        .moves = {MOVE_STOMP, MOVE_BOUNCE, MOVE_FIRESPIN, MOVE_FIREBLAST},
    },
    {
        .lvl = 42,
        .species = SPECIES_RAPIDASH,
        .moves = {MOVE_STOMP, MOVE_BOUNCE, MOVE_FIRESPIN, MOVE_FIREBLAST},
    },
    {
        .lvl = 47,
        .species = SPECIES_ARCANINE,
        .moves = {MOVE_BITE, MOVE_ROAR, MOVE_TAKEDOWN, MOVE_FIREBLAST},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerLeaderSabrina[] = {
    {
        .lvl = 38,
        .species = SPECIES_KADABRA,
        .moves = {MOVE_PSYBEAM, MOVE_REFLECT, MOVE_FUTURESIGHT, MOVE_CALMMIND},
    },
    {
        .lvl = 37,
        .species = SPECIES_MR_MIME,
        .moves = {MOVE_BARRIER, MOVE_PSYBEAM, MOVE_BATONPASS, MOVE_CALMMIND},
    },
    {
        .lvl = 38,
        .species = SPECIES_VENOMOTH,
        .moves = {MOVE_PSYBEAM, MOVE_GUST, MOVE_LEECHLIFE, MOVE_SUPERSONIC},
    },
    {
        .lvl = 43,
        .species = SPECIES_ALAKAZAM,
        .moves = {MOVE_PSYCHIC, MOVE_RECOVER, MOVE_FUTURESIGHT, MOVE_CALMMIND},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGentlemanThomas[] = {
    {
        .lvl = 18,
        .species = SPECIES_GROWLITHE,
    },
    {
        .lvl = 18,
        .species = SPECIES_GROWLITHE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGentlemanArthur[] = {
    {
        .lvl = 19,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .lvl = 19,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGentlemanTucker[] = {
    {
        .lvl = 23,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGentlemanNorton[] = {
    {
        .lvl = 48,
        .species = SPECIES_PERSIAN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGentlemanWalter[] = {
    {
        .lvl = 17,
        .species = SPECIES_GROWLITHE,
    },
    {
        .lvl = 17,
        .species = SPECIES_PONYTA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRivalSsAnneSquirtle[] = {
    {
        .iv = 50,
        .lvl = 19,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 50,
        .lvl = 16,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 50,
        .lvl = 18,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = 100,
        .lvl = 20,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRivalSsAnneBulbasaur[] = {
    {
        .iv = 50,
        .lvl = 19,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 50,
        .lvl = 16,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 50,
        .lvl = 18,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = 100,
        .lvl = 20,
        .species = SPECIES_IVYSAUR,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRivalSsAnneCharmander[] = {
    {
        .iv = 50,
        .lvl = 19,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 50,
        .lvl = 16,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 50,
        .lvl = 18,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = 100,
        .lvl = 20,
        .species = SPECIES_CHARMELEON,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRivalPokemonTowerSquirtle[] = {
    {
        .iv = 100,
        .lvl = 25,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 100,
        .lvl = 23,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = 100,
        .lvl = 22,
        .species = SPECIES_EXEGGCUTE,
    },
    {
        .iv = 100,
        .lvl = 20,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = 100,
        .lvl = 25,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRivalPokemonTowerBulbasaur[] = {
    {
        .iv = 100,
        .lvl = 25,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 100,
        .lvl = 23,
        .species = SPECIES_GYARADOS,
    },
    {
        .iv = 100,
        .lvl = 22,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = 100,
        .lvl = 20,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = 100,
        .lvl = 25,
        .species = SPECIES_IVYSAUR,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRivalPokemonTowerCharmander[] = {
    {
        .iv = 100,
        .lvl = 25,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 100,
        .lvl = 23,
        .species = SPECIES_EXEGGCUTE,
    },
    {
        .iv = 100,
        .lvl = 22,
        .species = SPECIES_GYARADOS,
    },
    {
        .iv = 100,
        .lvl = 20,
        .species = SPECIES_KADABRA,
    },
    {
        .iv = 100,
        .lvl = 25,
        .species = SPECIES_CHARMELEON,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRivalSilphSquirtle[] = {
    {
        .iv = 100,
        .lvl = 37,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = 100,
        .lvl = 35,
        .species = SPECIES_EXEGGCUTE,
    },
    {
        .iv = 100,
        .lvl = 35,
        .species = SPECIES_ALAKAZAM,
    },
    {
        .iv = 150,
        .lvl = 40,
        .species = SPECIES_BLASTOISE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRivalSilphBulbasaur[] = {
    {
        .iv = 100,
        .lvl = 37,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_GYARADOS,
    },
    {
        .iv = 100,
        .lvl = 35,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = 100,
        .lvl = 35,
        .species = SPECIES_ALAKAZAM,
    },
    {
        .iv = 150,
        .lvl = 40,
        .species = SPECIES_VENUSAUR,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRivalSilphCharmander[] = {
    {
        .iv = 100,
        .lvl = 37,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_EXEGGCUTE,
    },
    {
        .iv = 100,
        .lvl = 35,
        .species = SPECIES_GYARADOS,
    },
    {
        .iv = 100,
        .lvl = 35,
        .species = SPECIES_ALAKAZAM,
    },
    {
        .iv = 150,
        .lvl = 40,
        .species = SPECIES_CHARIZARD,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerRivalRoute22LateSquirtle[] = {
    {
        .iv = 150,
        .lvl = 47,
        .species = SPECIES_PIDGEOT,
        .moves = {MOVE_FEATHERDANCE, MOVE_WINGATTACK, MOVE_GUST, MOVE_QUICKATTACK},
    },
    {
        .iv = 150,
        .lvl = 45,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKEDOWN, MOVE_HORNDRILL, MOVE_ROCKBLAST, MOVE_FURYATTACK},
    },
    {
        .iv = 150,
        .lvl = 45,
        .species = SPECIES_GROWLITHE,
        .moves = {MOVE_FLAMEWHEEL, MOVE_TAKEDOWN, MOVE_LEER, MOVE_AGILITY},
    },
    {
        .iv = 150,
        .lvl = 45,
        .species = SPECIES_EXEGGCUTE,
        .moves = {MOVE_SOLARBEAM, MOVE_SLEEPPOWDER, MOVE_POISONPOWDER, MOVE_STUNSPORE},
    },
    {
        .iv = 150,
        .lvl = 47,
        .species = SPECIES_ALAKAZAM,
        .moves = {MOVE_PSYCHIC, MOVE_CALMMIND, MOVE_FUTURESIGHT, MOVE_DISABLE},
    },
    {
        .lvl = 53,
        .species = SPECIES_BLASTOISE,
        .moves = {MOVE_WATERGUN, MOVE_RAINDANCE, MOVE_BITE, MOVE_RAPIDSPIN},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerRivalRoute22LateBulbasaur[] = {
    {
        .iv = 150,
        .lvl = 47,
        .species = SPECIES_PIDGEOT,
        .moves = {MOVE_FEATHERDANCE, MOVE_WINGATTACK, MOVE_GUST, MOVE_QUICKATTACK},
    },
    {
        .iv = 150,
        .lvl = 45,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKEDOWN, MOVE_HORNDRILL, MOVE_ROCKBLAST, MOVE_FURYATTACK},
    },
    {
        .iv = 150,
        .lvl = 45,
        .species = SPECIES_GYARADOS,
        .moves = {MOVE_HYDROPUMP, MOVE_TWISTER, MOVE_LEER, MOVE_RAINDANCE},
    },
    {
        .iv = 150,
        .lvl = 45,
        .species = SPECIES_GROWLITHE,
        .moves = {MOVE_FLAMEWHEEL, MOVE_TAKEDOWN, MOVE_LEER, MOVE_AGILITY},
    },
    {
        .iv = 150,
        .lvl = 47,
        .species = SPECIES_ALAKAZAM,
        .moves = {MOVE_PSYCHIC, MOVE_CALMMIND, MOVE_FUTURESIGHT, MOVE_DISABLE},
    },
    {
        .lvl = 53,
        .species = SPECIES_VENUSAUR,
        .moves = {MOVE_RAZORLEAF, MOVE_SWEETSCENT, MOVE_GROWTH, MOVE_SYNTHESIS},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerRivalRoute22LateCharmander[] = {
    {
        .iv = 150,
        .lvl = 47,
        .species = SPECIES_PIDGEOT,
        .moves = {MOVE_FEATHERDANCE, MOVE_WINGATTACK, MOVE_GUST, MOVE_QUICKATTACK},
    },
    {
        .iv = 150,
        .lvl = 45,
        .species = SPECIES_RHYHORN,
        .moves = {MOVE_TAKEDOWN, MOVE_HORNDRILL, MOVE_ROCKBLAST, MOVE_FURYATTACK},
    },
    {
        .iv = 150,
        .lvl = 45,
        .species = SPECIES_EXEGGCUTE,
        .moves = {MOVE_SOLARBEAM, MOVE_SLEEPPOWDER, MOVE_POISONPOWDER, MOVE_STUNSPORE},
    },
    {
        .iv = 150,
        .lvl = 45,
        .species = SPECIES_GYARADOS,
        .moves = {MOVE_HYDROPUMP, MOVE_TWISTER, MOVE_LEER, MOVE_RAINDANCE},
    },
    {
        .iv = 150,
        .lvl = 47,
        .species = SPECIES_ALAKAZAM,
        .moves = {MOVE_PSYCHIC, MOVE_CALMMIND, MOVE_FUTURESIGHT, MOVE_DISABLE},
    },
    {
        .lvl = 53,
        .species = SPECIES_CHARIZARD,
        .moves = {MOVE_FLAMETHROWER, MOVE_WINGATTACK, MOVE_SLASH, MOVE_SCARYFACE},
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerChampionFirstSquirtle[] = {
    {
        .lvl = 59,
        .species = SPECIES_PIDGEOT,
        .ability = Ability_RandomAll,
        .nature = NATURE_JOLLY,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_AERIALACE, MOVE_FEATHERDANCE, MOVE_SANDATTACK, MOVE_WHIRLWIND},
        .teraType = TYPE_NORMAL,
    },
    {
        .lvl = 57,
        .species = SPECIES_ALAKAZAM,
        .ability = Ability_RandomAll,
        .nature = NATURE_TIMID,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_PSYCHIC, MOVE_FUTURESIGHT, MOVE_RECOVER, MOVE_REFLECT},
        .teraType = TYPE_PSYCHIC,
    },
    {
        .lvl = 59,
        .species = SPECIES_RHYDON,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_TAKEDOWN, MOVE_EARTHQUAKE, MOVE_ROCKTOMB, MOVE_SCARYFACE},
        .teraType = TYPE_GROUND,
    },
    {
        .lvl = 59,
        .species = SPECIES_ARCANINE,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_EXTREMESPEED, MOVE_FLAMETHROWER, MOVE_ROAR, MOVE_BITE},
        .teraType = TYPE_FIRE,
    },
    {
        .lvl = 61,
        .species = SPECIES_EXEGGUTOR,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_GIGADRAIN, MOVE_EGGBOMB, MOVE_SLEEPPOWDER, MOVE_LIGHTSCREEN},
        .teraType = TYPE_GRASS,
    },
    {
        .lvl = 63,
        .species = SPECIES_BLASTOISE,
        .ability = Ability_RandomAll,
        .nature = NATURE_BOLD,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDROPUMP, MOVE_RAINDANCE, MOVE_SKULLBASH, MOVE_BITE},
        .teraType = TYPE_WATER,
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerChampionFirstBulbasaur[] = {
    {
        .lvl = 59,
        .species = SPECIES_PIDGEOT,
        .ability = Ability_RandomAll,
        .nature = NATURE_JOLLY,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_AERIALACE, MOVE_FEATHERDANCE, MOVE_SANDATTACK, MOVE_WHIRLWIND},
        .teraType = TYPE_NORMAL,
    },
    {
        .lvl = 57,
        .species = SPECIES_ALAKAZAM,
        .ability = Ability_RandomAll,
        .nature = NATURE_TIMID,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_PSYCHIC, MOVE_FUTURESIGHT, MOVE_RECOVER, MOVE_REFLECT},
        .teraType = TYPE_PSYCHIC,
    },
    {
        .lvl = 59,
        .species = SPECIES_RHYDON,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_TAKEDOWN, MOVE_EARTHQUAKE, MOVE_ROCKTOMB, MOVE_SCARYFACE},
        .teraType = TYPE_GROUND,
    },
    {
        .lvl = 59,
        .species = SPECIES_GYARADOS,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYDROPUMP, MOVE_DRAGONRAGE, MOVE_BITE, MOVE_THRASH},
        .teraType = TYPE_WATER,
    },
    {
        .lvl = 61,
        .species = SPECIES_ARCANINE,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_EXTREMESPEED, MOVE_FLAMETHROWER, MOVE_ROAR, MOVE_BITE},
        .teraType = TYPE_FIRE,
    },
    {
        .lvl = 63,
        .species = SPECIES_VENUSAUR,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SOLARBEAM, MOVE_SYNTHESIS, MOVE_SUNNYDAY, MOVE_GROWTH},
        .teraType = TYPE_GRASS,
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerChampionFirstCharmander[] = {
    {
        .lvl = 59,
        .species = SPECIES_PIDGEOT,
        .ability = Ability_RandomAll,
        .nature = NATURE_JOLLY,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_AERIALACE, MOVE_FEATHERDANCE, MOVE_SANDATTACK, MOVE_WHIRLWIND},
        .teraType = TYPE_NORMAL,
    },
    {
        .lvl = 57,
        .species = SPECIES_ALAKAZAM,
        .ability = Ability_RandomAll,
        .nature = NATURE_TIMID,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_PSYCHIC, MOVE_FUTURESIGHT, MOVE_RECOVER, MOVE_REFLECT},
        .teraType = TYPE_PSYCHIC,
    },
    {
        .lvl = 59,
        .species = SPECIES_RHYDON,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_TAKEDOWN, MOVE_EARTHQUAKE, MOVE_ROCKTOMB, MOVE_SCARYFACE},
        .teraType = TYPE_GROUND,
    },
    {
        .lvl = 59,
        .species = SPECIES_EXEGGUTOR,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_GIGADRAIN, MOVE_EGGBOMB, MOVE_SLEEPPOWDER, MOVE_LIGHTSCREEN},
        .teraType = TYPE_GRASS,
    },
    {
        .lvl = 61,
        .species = SPECIES_GYARADOS,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYDROPUMP, MOVE_DRAGONRAGE, MOVE_BITE, MOVE_THRASH},
        .teraType = TYPE_WATER,
    },
    {
        .lvl = 63,
        .species = SPECIES_CHARIZARD,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIREBLAST, MOVE_AERIALACE, MOVE_SLASH, MOVE_FIRESPIN},
        .teraType = TYPE_FIRE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerPatricia[] = {
    {
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerCarly[] = {
    {
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerHope[] = {
    {
        .lvl = 23,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerPaula[] = {
    {
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerLaurel[] = {
    {
        .lvl = 23,
        .species = SPECIES_GASTLY,
    },
    {
        .lvl = 23,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerJody[] = {
    {
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerTammy[] = {
    {
        .lvl = 23,
        .species = SPECIES_HAUNTER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerRuth[] = {
    {
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerKarina[] = {
    {
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerJanae[] = {
    {
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerAngelica[] = {
    {
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
    {
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
    {
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerEmilia[] = {
    {
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerJennifer[] = {
    {
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChanneler1[] = {
    {
        .lvl = 23,
        .species = SPECIES_HAUNTER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChanneler2[] = {
    {
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChanneler3[] = {
    {
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChanneler4[] = {
    {
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChanneler5[] = {
    {
        .lvl = 22,
        .species = SPECIES_HAUNTER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChanneler6[] = {
    {
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
    {
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
    {
        .lvl = 22,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChanneler7[] = {
    {
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChanneler8[] = {
    {
        .lvl = 24,
        .species = SPECIES_GASTLY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerAmanda[] = {
    {
        .lvl = 34,
        .species = SPECIES_GASTLY,
    },
    {
        .lvl = 34,
        .species = SPECIES_HAUNTER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerStacy[] = {
    {
        .lvl = 38,
        .species = SPECIES_HAUNTER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerChannelerTasha[] = {
    {
        .lvl = 33,
        .species = SPECIES_GASTLY,
    },
    {
        .lvl = 33,
        .species = SPECIES_GASTLY,
    },
    {
        .lvl = 33,
        .species = SPECIES_HAUNTER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerJeremy[] = {
    {
        .lvl = 20,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 20,
        .species = SPECIES_ONIX,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerAlma[] = {
    {
        .lvl = 28,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 28,
        .species = SPECIES_POLIWAG,
    },
    {
        .lvl = 28,
        .species = SPECIES_HORSEA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerSusie[] = {
    {
        .lvl = 24,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
    {
        .lvl = 24,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 24,
        .species = SPECIES_PIKACHU,
    },
    {
        .lvl = 24,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerValerie[] = {
    {
        .lvl = 30,
        .species = SPECIES_POLIWAG,
    },
    {
        .lvl = 30,
        .species = SPECIES_POLIWAG,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerGwen[] = {
    {
        .lvl = 27,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 27,
        .species = SPECIES_MEOWTH,
    },
    {
        .lvl = 27,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 27,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerVirgil[] = {
    {
        .lvl = 28,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 28,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
    {
        .lvl = 28,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperFlint[] = {
    {
        .lvl = 14,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 14,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerMissy[] = {
    {
        .lvl = 31,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 31,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerIrene[] = {
    {
        .lvl = 30,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 30,
        .species = SPECIES_HORSEA,
    },
    {
        .lvl = 30,
        .species = SPECIES_SEEL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerDana[] = {
    {
        .lvl = 20,
        .species = SPECIES_MEOWTH,
    },
    {
        .lvl = 20,
        .species = SPECIES_ODDISH,
    },
    {
        .lvl = 20,
        .species = SPECIES_PIDGEY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerAriana[] = {
    {
        .lvl = 19,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 19,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 19,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 19,
        .species = SPECIES_BELLSPROUT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerLeah[] = {
    {
        .lvl = 22,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .lvl = 22,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperJustin[] = {
    {
        .lvl = 29,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .lvl = 29,
        .species = SPECIES_NIDORINO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerYazmin[] = {
    {
        .lvl = 29,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .lvl = 29,
        .species = SPECIES_ODDISH,
    },
    {
        .lvl = 29,
        .species = SPECIES_TANGELA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerKindra[] = {
    {
        .lvl = 28,
        .species = SPECIES_GLOOM,
    },
    {
        .lvl = 28,
        .species = SPECIES_ODDISH,
    },
    {
        .lvl = 28,
        .species = SPECIES_ODDISH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerBecky[] = {
    {
        .lvl = 29,
        .species = SPECIES_PIKACHU,
    },
    {
        .lvl = 29,
        .species = SPECIES_RAICHU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerCelia[] = {
    {
        .lvl = 33,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGentlemanBrooks[] = {
    {
        .lvl = 23,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGentlemanLamar[] = {
    {
        .lvl = 17,
        .species = SPECIES_GROWLITHE,
    },
    {
        .lvl = 17,
        .species = SPECIES_PONYTA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTwinsEliAnne[] = {
    {
        .lvl = 22,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .lvl = 22,
        .species = SPECIES_JIGGLYPUFF,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCoolCoupleRayTyra[] = {
    {
        .iv = 100,
        .lvl = 45,
        .species = SPECIES_NIDOQUEEN,
        .moves = {MOVE_SUPERPOWER, MOVE_BODYSLAM, MOVE_DOUBLEKICK, MOVE_POISONSTING},
    },
    {
        .iv = 100,
        .lvl = 45,
        .species = SPECIES_NIDOKING,
        .moves = {MOVE_MEGAHORN, MOVE_THRASH, MOVE_DOUBLEKICK, MOVE_POISONSTING},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungCoupleGiaJes[] = {
    {
        .lvl = 24,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .lvl = 24,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTwinsKiriJan[] = {
    {
        .lvl = 29,
        .species = SPECIES_CHARMANDER,
    },
    {
        .lvl = 29,
        .species = SPECIES_SQUIRTLE,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushKinRonMya[] = {
    {
        .iv = 50,
        .lvl = 29,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 50,
        .lvl = 29,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungCoupleLeaJed[] = {
    {
        .lvl = 29,
        .species = SPECIES_RAPIDASH,
    },
    {
        .lvl = 29,
        .species = SPECIES_NINETALES,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSisAndBroLiaLuc[] = {
    {
        .lvl = 30,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 30,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSisAndBroLilIan[] = {
    {
        .lvl = 33,
        .species = SPECIES_SEADRA,
    },
    {
        .lvl = 33,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcher3[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcher4[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcher5[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcher6[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcher7[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcher8[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterBen3[] = {
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterBen4[] = {
    {
        .iv = 120,
        .lvl = 48,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 120,
        .lvl = 48,
        .species = SPECIES_ARBOK,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterChad2[] = {
    {
        .iv = 20,
        .lvl = 20,
        .species = SPECIES_EKANS,
    },
    {
        .iv = 20,
        .lvl = 20,
        .species = SPECIES_SANDSHREW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassReli2[] = {
    {
        .iv = 20,
        .lvl = 20,
        .species = SPECIES_PIDGEY,
    },
    {
        .iv = 20,
        .lvl = 20,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassReli3[] = {
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_NIDORINA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterTimmy2[] = {
    {
        .iv = 20,
        .lvl = 19,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 20,
        .lvl = 19,
        .species = SPECIES_EKANS,
    },
    {
        .iv = 20,
        .lvl = 19,
        .species = SPECIES_ZUBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterTimmy3[] = {
    {
        .iv = 60,
        .lvl = 27,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 60,
        .lvl = 27,
        .species = SPECIES_EKANS,
    },
    {
        .iv = 60,
        .lvl = 27,
        .species = SPECIES_GOLBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterTimmy4[] = {
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_ARBOK,
    },
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_GOLBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterChad3[] = {
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_ARBOK,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_SANDSHREW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassJanice2[] = {
    {
        .iv = 20,
        .lvl = 20,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 20,
        .lvl = 20,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassJanice3[] = {
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterChad4[] = {
    {
        .iv = 80,
        .lvl = 48,
        .species = SPECIES_ARBOK,
    },
    {
        .iv = 80,
        .lvl = 48,
        .species = SPECIES_SANDSLASH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerFranklin2[] = {
    {
        .iv = 40,
        .lvl = 25,
        .species = SPECIES_MACHOKE,
    },
    {
        .iv = 40,
        .lvl = 25,
        .species = SPECIES_GRAVELER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPkmnProfProfOak[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPlayerBrendan[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPlayerMay[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPlayerRed[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPlayerLeaf[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};


static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt42[] = {
    {
        .lvl = 49,
        .species = SPECIES_HOUNDOUR,
    },
    {
        .lvl = 49,
        .species = SPECIES_HOUNDOUR,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPsychicJaclyn[] = {
    {
        .iv = 100,
        .lvl = 48,
        .species = SPECIES_NATU,
        .moves = {MOVE_NIGHTSHADE, MOVE_CONFUSERAY, MOVE_FUTURESIGHT, MOVE_WISH},
    },
    {
        .iv = 100,
        .lvl = 48,
        .species = SPECIES_SLOWBRO,
        .moves = {MOVE_PSYCHIC, MOVE_HEADBUTT, MOVE_AMNESIA, MOVE_YAWN},
    },
    {
        .iv = 100,
        .lvl = 49,
        .species = SPECIES_KADABRA,
        .moves = {MOVE_PSYCHIC, MOVE_FUTURESIGHT, MOVE_RECOVER, MOVE_REFLECT},
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushGirlSharon[] = {
    {
        .iv = 50,
        .lvl = 37,
        .species = SPECIES_MANKEY,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 50,
        .lvl = 37,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTuberAmira[] = {
    {
        .lvl = 34,
        .species = SPECIES_POLIWAG,
    },
    {
        .lvl = 35,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .lvl = 34,
        .species = SPECIES_POLIWAG,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPkmnBreederAlize[] = {
    {
        .iv = 30,
        .lvl = 48,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = 30,
        .lvl = 48,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = 30,
        .lvl = 48,
        .species = SPECIES_MARILL,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPkmnRangerNicolas[] = {
    {
        .iv = 100,
        .lvl = 51,
        .species = SPECIES_WEEPINBELL,
        .moves = {MOVE_RAZORLEAF, MOVE_ACID, MOVE_SWEETSCENT, MOVE_WRAP},
    },
    {
        .iv = 100,
        .lvl = 51,
        .species = SPECIES_VICTREEBEL,
        .moves = {MOVE_RAZORLEAF, MOVE_ACID, MOVE_SLEEPPOWDER, MOVE_STUNSPORE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPkmnRangerMadeline[] = {
    {
        .iv = 100,
        .lvl = 51,
        .species = SPECIES_GLOOM,
        .moves = {MOVE_PETALDANCE, MOVE_ACID, MOVE_SWEETSCENT, MOVE_POISONPOWDER},
    },
    {
        .iv = 100,
        .lvl = 51,
        .species = SPECIES_VILEPLUME,
        .moves = {MOVE_PETALDANCE, MOVE_MOONLIGHT, MOVE_ACID, MOVE_STUNSPORE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerAromaLadyNikki[] = {
    {
        .lvl = 37,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .lvl = 37,
        .species = SPECIES_WEEPINBELL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRuinManiacStanly[] = {
    {
        .lvl = 48,
        .species = SPECIES_GRAVELER,
    },
    {
        .lvl = 48,
        .species = SPECIES_ONIX,
    },
    {
        .lvl = 48,
        .species = SPECIES_GRAVELER,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerLadyJacki[] = {
    {
        .lvl = 48,
        .species = SPECIES_HOPPIP,
        .heldItem = ITEM_STARDUST,
    },
    {
        .lvl = 50,
        .species = SPECIES_SKIPLOOM,
        .heldItem = ITEM_STARDUST,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPainterDaisy[] = {
    {
        .iv = 50,
        .lvl = 50,
        .species = SPECIES_SMEARGLE,
        .moves = {MOVE_DYNAMICPUNCH, MOVE_DIZZYPUNCH, MOVE_FOCUSPUNCH, MOVE_MEGAPUNCH},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerGoon[] = {
    {
        .lvl = 37,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_HAZE, MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_TACKLE},
    },
    {
        .lvl = 37,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_ACIDARMOR, MOVE_SCREECH, MOVE_MINIMIZE, MOVE_SLUDGE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerGoon2[] = {
    {
        .lvl = 38,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_HAZE, MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_TACKLE},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBikerGoon3[] = {
    {
        .lvl = 38,
        .species = SPECIES_GRIMER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBiker2[] = {
    {
        .lvl = 5,
        .species = SPECIES_EKANS,
    },
};


static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherAnthony[] = {
    {
        .lvl = 7,
        .species = SPECIES_CATERPIE,
    },
    {
        .lvl = 8,
        .species = SPECIES_CATERPIE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherCharlie[] = {
    {
        .lvl = 7,
        .species = SPECIES_METAPOD,
    },
    {
        .lvl = 7,
        .species = SPECIES_CATERPIE,
    },
    {
        .lvl = 7,
        .species = SPECIES_METAPOD,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTwinsEliAnne2[] = {
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_JIGGLYPUFF,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterJohnson[] = {
    {
        .lvl = 33,
        .species = SPECIES_EKANS,
    },
    {
        .lvl = 33,
        .species = SPECIES_EKANS,
    },
    {
        .lvl = 34,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerBikerRicardo[] = {
    {
        .lvl = 22,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE, MOVE_POISONGAS},
    },
    {
        .lvl = 22,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_SLUDGE, MOVE_SMOG, MOVE_TACKLE, MOVE_POISONGAS},
    },
    {
        .lvl = 23,
        .species = SPECIES_GRIMER,
        .moves = {MOVE_MINIMIZE, MOVE_SLUDGE, MOVE_DISABLE, MOVE_POUND},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBikerJaren[] = {
    {
        .lvl = 24,
        .species = SPECIES_GRIMER,
    },
    {
        .lvl = 24,
        .species = SPECIES_GRIMER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt43[] = {
    {
        .lvl = 37,
        .species = SPECIES_CUBONE,
    },
    {
        .lvl = 37,
        .species = SPECIES_MAROWAK,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt44[] = {
    {
        .lvl = 35,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 35,
        .species = SPECIES_RATICATE,
    },
    {
        .lvl = 35,
        .species = SPECIES_SANDSHREW,
    },
    {
        .lvl = 35,
        .species = SPECIES_SANDSLASH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt45[] = {
    {
        .lvl = 38,
        .species = SPECIES_ZUBAT,
    },
    {
        .lvl = 38,
        .species = SPECIES_ZUBAT,
    },
    {
        .lvl = 38,
        .species = SPECIES_GOLBAT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt46[] = {
    {
        .lvl = 48,
        .species = SPECIES_MUK,
    },
    {
        .lvl = 48,
        .species = SPECIES_GOLBAT,
    },
    {
        .lvl = 48,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt47[] = {
    {
        .lvl = 48,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 48,
        .species = SPECIES_MACHOP,
    },
    {
        .lvl = 48,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt48[] = {
    {
        .lvl = 49,
        .species = SPECIES_HYPNO,
    },
    {
        .lvl = 49,
        .species = SPECIES_HYPNO,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerTeamRocketAdmin[] = {
    {
        .iv = 150,
        .lvl = 52,
        .species = SPECIES_MUK,
        .moves = {MOVE_SLUDGEBOMB, MOVE_SCREECH, MOVE_MINIMIZE, MOVE_ROCKTOMB},
    },
    {
        .iv = 150,
        .lvl = 53,
        .species = SPECIES_ARBOK,
        .moves = {MOVE_SLUDGEBOMB, MOVE_BITE, MOVE_EARTHQUAKE, MOVE_IRONTAIL},
    },
    {
        .iv = 150,
        .lvl = 54,
        .species = SPECIES_VILEPLUME,
        .moves = {MOVE_SLUDGEBOMB, MOVE_GIGADRAIN, MOVE_SLEEPPOWDER, MOVE_STUNSPORE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerTeamRocketAdmin2[] = {
    {
        .iv = 200,
        .lvl = 53,
        .species = SPECIES_GOLBAT,
        .moves = {MOVE_CONFUSERAY, MOVE_SLUDGEBOMB, MOVE_AIRCUTTER, MOVE_SHADOWBALL},
    },
    {
        .iv = 200,
        .lvl = 54,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_SLUDGEBOMB, MOVE_THUNDERBOLT, MOVE_EXPLOSION, MOVE_SHADOWBALL},
    },
    {
        .iv = 200,
        .lvl = 55,
        .species = SPECIES_HOUNDOOM,
        .moves = {MOVE_FLAMETHROWER, MOVE_CRUNCH, MOVE_IRONTAIL, MOVE_SHADOWBALL},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerScientistGideon[] = {
    {
        .lvl = 46,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SWIFT, MOVE_SCREECH, MOVE_SPARK, MOVE_SONICBOOM},
    },
    {
        .lvl = 46,
        .species = SPECIES_ELECTRODE,
        .moves = {MOVE_SPARK, MOVE_SONICBOOM, MOVE_SCREECH, MOVE_CHARGE},
    },
    {
        .lvl = 46,
        .species = SPECIES_MAGNEMITE,
        .moves = {MOVE_SCREECH, MOVE_SWIFT, MOVE_SPARK, MOVE_THUNDERWAVE},
    },
    {
        .lvl = 46,
        .species = SPECIES_MAGNETON,
        .moves = {MOVE_TRIATTACK, MOVE_SPARK, MOVE_THUNDERWAVE, MOVE_SONICBOOM},
    },
    {
        .lvl = 46,
        .species = SPECIES_PORYGON,
        .moves = {MOVE_TRIATTACK, MOVE_CONVERSION, MOVE_RECOVER, MOVE_PSYBEAM},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleAmara[] = {
    {
        .lvl = 36,
        .species = SPECIES_SEEL,
    },
    {
        .lvl = 36,
        .species = SPECIES_SEEL,
    },
    {
        .lvl = 36,
        .species = SPECIES_DEWGONG,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleMaria[] = {
    {
        .lvl = 37,
        .species = SPECIES_SEADRA,
    },
    {
        .lvl = 37,
        .species = SPECIES_SEADRA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleAbigail[] = {
    {
        .lvl = 35,
        .species = SPECIES_PSYDUCK,
    },
    {
        .lvl = 36,
        .species = SPECIES_PSYDUCK,
    },
    {
        .lvl = 37,
        .species = SPECIES_GOLDUCK,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleFinn[] = {
    {
        .lvl = 38,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleGarrett[] = {
    {
        .lvl = 35,
        .species = SPECIES_SHELLDER,
    },
    {
        .lvl = 35,
        .species = SPECIES_CLOYSTER,
    },
    {
        .lvl = 38,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanTommy[] = {
    {
        .lvl = 33,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 33,
        .species = SPECIES_GOLDEEN,
    },
    {
        .lvl = 35,
        .species = SPECIES_SEAKING,
    },
    {
        .lvl = 35,
        .species = SPECIES_SEAKING,
    },
    {
        .lvl = 35,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushGirlTanya[] = {
    {
        .iv = 50,
        .lvl = 38,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 50,
        .lvl = 38,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltShea[] = {
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltHugh[] = {
    {
        .iv = 100,
        .lvl = 37,
        .species = SPECIES_MACHOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 100,
        .lvl = 37,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperBryce[] = {
    {
        .lvl = 36,
        .species = SPECIES_NIDORINO,
    },
    {
        .lvl = 36,
        .species = SPECIES_RATICATE,
    },
    {
        .lvl = 36,
        .species = SPECIES_SANDSLASH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerClaire[] = {
    {
        .lvl = 35,
        .species = SPECIES_MEOWTH,
    },
    {
        .lvl = 35,
        .species = SPECIES_MEOWTH,
    },
    {
        .lvl = 35,
        .species = SPECIES_PIKACHU,
    },
    {
        .lvl = 35,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushKinMikKia[] = {
    {
        .iv = 50,
        .lvl = 39,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 50,
        .lvl = 39,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerAromaLadyViolet[] = {
    {
        .lvl = 36,
        .species = SPECIES_BULBASAUR,
    },
    {
        .lvl = 36,
        .species = SPECIES_IVYSAUR,
    },
    {
        .lvl = 36,
        .species = SPECIES_IVYSAUR,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTuberAlexis[] = {
    {
        .lvl = 34,
        .species = SPECIES_STARYU,
    },
    {
        .lvl = 34,
        .species = SPECIES_STARYU,
    },
    {
        .lvl = 34,
        .species = SPECIES_KRABBY,
    },
    {
        .lvl = 34,
        .species = SPECIES_KRABBY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTwinsJoyMeg[] = {
    {
        .lvl = 37,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .lvl = 37,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleTisha[] = {
    {
        .lvl = 38,
        .species = SPECIES_KINGLER,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPainterCelina[] = {
    {
        .iv = 50,
        .lvl = 50,
        .species = SPECIES_SMEARGLE,
        .moves = {MOVE_FLY, MOVE_DIG, MOVE_DIVE, MOVE_BOUNCE},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPainterRayna[] = {
    {
        .iv = 50,
        .lvl = 50,
        .species = SPECIES_SMEARGLE,
        .moves = {MOVE_CROSSCHOP, MOVE_MEGAHORN, MOVE_DOUBLEEDGE, MOVE_SELFDESTRUCT},
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerLadyGillian[] = {
    {
        .lvl = 47,
        .species = SPECIES_MAREEP,
        .heldItem = ITEM_STARDUST,
    },
    {
        .lvl = 48,
        .species = SPECIES_MAREEP,
        .heldItem = ITEM_STARDUST,
    },
    {
        .lvl = 49,
        .species = SPECIES_FLAAFFY,
        .heldItem = ITEM_NUGGET,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterDestin[] = {
    {
        .lvl = 48,
        .species = SPECIES_RATICATE,
    },
    {
        .lvl = 48,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleToby[] = {
    {
        .lvl = 48,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .lvl = 48,
        .species = SPECIES_TENTACOOL,
    },
    {
        .lvl = 48,
        .species = SPECIES_TENTACRUEL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt49[] = {
    {
        .lvl = 48,
        .species = SPECIES_RATTATA,
    },
    {
        .lvl = 48,
        .species = SPECIES_GRIMER,
    },
    {
        .lvl = 48,
        .species = SPECIES_MUK,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerTeamRocketGrunt50[] = {
    {
        .lvl = 49,
        .species = SPECIES_KOFFING,
        .moves = {MOVE_MEMENTO, MOVE_HAZE, MOVE_SMOKESCREEN, MOVE_SLUDGE},
    },
    {
        .lvl = 49,
        .species = SPECIES_WEEZING,
        .moves = {MOVE_HAZE, MOVE_SMOKESCREEN, MOVE_SLUDGE, MOVE_SELFDESTRUCT},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTeamRocketGrunt51[] = {
    {
        .iv = 50,
        .lvl = 48,
        .species = SPECIES_EKANS,
    },
    {
        .iv = 50,
        .lvl = 48,
        .species = SPECIES_GLOOM,
    },
    {
        .iv = 50,
        .lvl = 48,
        .species = SPECIES_GLOOM,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperMilo[] = {
    {
        .lvl = 47,
        .species = SPECIES_PIDGEY,
    },
    {
        .lvl = 49,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperChaz[] = {
    {
        .lvl = 47,
        .species = SPECIES_SPEAROW,
    },
    {
        .lvl = 49,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperHarold[] = {
    {
        .lvl = 47,
        .species = SPECIES_HOOTHOOT,
    },
    {
        .lvl = 49,
        .species = SPECIES_NOCTOWL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanTylor[] = {
    {
        .lvl = 49,
        .species = SPECIES_QWILFISH,
    },
    {
        .lvl = 49,
        .species = SPECIES_QWILFISH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleMymo[] = {
    {
        .lvl = 49,
        .species = SPECIES_KINGLER,
    },
    {
        .lvl = 49,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleNicole[] = {
    {
        .lvl = 50,
        .species = SPECIES_MARILL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSisAndBroAvaGeb[] = {
    {
        .lvl = 50,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .lvl = 50,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerAromaLadyRose[] = {
    {
        .lvl = 49,
        .species = SPECIES_SUNKERN,
    },
    {
        .lvl = 49,
        .species = SPECIES_SUNFLORA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleSamir[] = {
    {
        .lvl = 50,
        .species = SPECIES_GYARADOS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleDenise[] = {
    {
        .lvl = 49,
        .species = SPECIES_CHINCHOU,
    },
    {
        .lvl = 49,
        .species = SPECIES_LANTURN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTwinsMiuMia[] = {
    {
        .lvl = 50,
        .species = SPECIES_PIKACHU,
    },
    {
        .lvl = 50,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerEarl[] = {
    {
        .lvl = 49,
        .species = SPECIES_ONIX,
    },
    {
        .lvl = 49,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRuinManiacFoster[] = {
    {
        .lvl = 50,
        .species = SPECIES_GOLEM,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRuinManiacLarry[] = {
    {
        .lvl = 49,
        .species = SPECIES_MACHOKE,
    },
    {
        .lvl = 49,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerDaryl[] = {
    {
        .lvl = 50,
        .species = SPECIES_SUDOWOODO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokemaniacHector[] = {
    {
        .iv = 30,
        .lvl = 49,
        .species = SPECIES_RHYHORN,
    },
    {
        .iv = 30,
        .lvl = 49,
        .species = SPECIES_KANGASKHAN,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPsychicDario[] = {
    {
        .iv = 100,
        .lvl = 52,
        .species = SPECIES_GIRAFARIG,
        .moves = {MOVE_CRUNCH, MOVE_PSYBEAM, MOVE_ODORSLEUTH, MOVE_AGILITY},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPsychicRodette[] = {
    {
        .iv = 100,
        .lvl = 48,
        .species = SPECIES_NATU,
        .moves = {MOVE_NIGHTSHADE, MOVE_CONFUSERAY, MOVE_WISH, MOVE_FUTURESIGHT},
    },
    {
        .iv = 100,
        .lvl = 48,
        .species = SPECIES_DROWZEE,
        .moves = {MOVE_PSYCHIC, MOVE_DISABLE, MOVE_PSYCHUP, MOVE_FUTURESIGHT},
    },
    {
        .iv = 100,
        .lvl = 50,
        .species = SPECIES_HYPNO,
        .moves = {MOVE_PSYCHIC, MOVE_HYPNOSIS, MOVE_PSYCHUP, MOVE_FUTURESIGHT},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerAromaLadyMiah[] = {
    {
        .lvl = 50,
        .species = SPECIES_BELLOSSOM,
    },
    {
        .lvl = 50,
        .species = SPECIES_BELLOSSOM,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungCoupleEveJon[] = {
    {
        .lvl = 50,
        .species = SPECIES_GOLDUCK,
    },
    {
        .lvl = 50,
        .species = SPECIES_PSYDUCK,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerJugglerMason[] = {
    {
        .lvl = 47,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SWIFT, MOVE_LIGHTSCREEN, MOVE_SPARK, MOVE_SONICBOOM},
    },
    {
        .lvl = 47,
        .species = SPECIES_PINECO,
        .moves = {MOVE_SPIKES, MOVE_BIDE, MOVE_RAPIDSPIN, MOVE_TAKEDOWN},
    },
    {
        .lvl = 47,
        .species = SPECIES_VOLTORB,
        .moves = {MOVE_SWIFT, MOVE_LIGHTSCREEN, MOVE_SPARK, MOVE_SONICBOOM},
    },
    {
        .lvl = 47,
        .species = SPECIES_PINECO,
        .moves = {MOVE_SPIKES, MOVE_BIDE, MOVE_RAPIDSPIN, MOVE_EXPLOSION},
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushGirlCyndy[] = {
    {
        .iv = 50,
        .lvl = 48,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 50,
        .lvl = 48,
        .species = SPECIES_HITMONTOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 50,
        .lvl = 48,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushGirlJocelyn[] = {
    {
        .iv = 50,
        .lvl = 38,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 50,
        .lvl = 38,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTamerEvan[] = {
    {
        .iv = 40,
        .lvl = 48,
        .species = SPECIES_SANDSLASH,
    },
    {
        .iv = 40,
        .lvl = 48,
        .species = SPECIES_LICKITUNG,
    },
    {
        .iv = 40,
        .lvl = 49,
        .species = SPECIES_URSARING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokemaniacMark2[] = {
    {
        .iv = 90,
        .lvl = 33,
        .species = SPECIES_RHYHORN,
    },
    {
        .iv = 90,
        .lvl = 33,
        .species = SPECIES_LICKITUNG,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPkmnRangerLogan[] = {
    {
        .iv = 100,
        .lvl = 37,
        .species = SPECIES_EXEGGCUTE,
        .moves = {MOVE_SLEEPPOWDER, MOVE_POISONPOWDER, MOVE_STUNSPORE, MOVE_CONFUSION},
    },
    {
        .iv = 100,
        .lvl = 40,
        .species = SPECIES_EXEGGUTOR,
        .moves = {MOVE_EGGBOMB, MOVE_STOMP, MOVE_CONFUSION, MOVE_HYPNOSIS},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPkmnRangerJackson[] = {
    {
        .iv = 100,
        .lvl = 49,
        .species = SPECIES_TANGELA,
        .moves = {MOVE_SLAM, MOVE_MEGADRAIN, MOVE_BIND, MOVE_INGRAIN},
    },
    {
        .iv = 100,
        .lvl = 49,
        .species = SPECIES_EXEGGCUTE,
        .moves = {MOVE_CONFUSION, MOVE_POISONPOWDER, MOVE_BARRAGE, MOVE_REFLECT},
    },
    {
        .iv = 100,
        .lvl = 49,
        .species = SPECIES_EXEGGUTOR,
        .moves = {MOVE_CONFUSION, MOVE_EGGBOMB, MOVE_SLEEPPOWDER, MOVE_STOMP},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPkmnRangerBeth[] = {
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_BELLSPROUT,
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_GLOOM,
    },
    {
        .iv = 100,
        .lvl = 38,
        .species = SPECIES_GLOOM,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPkmnRangerKatelyn[] = {
    {
        .iv = 100,
        .lvl = 52,
        .species = SPECIES_CHANSEY,
        .moves = {MOVE_EGGBOMB, MOVE_DEFENSECURL, MOVE_MINIMIZE, MOVE_SOFTBOILED},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerLeroy[] = {
    {
        .iv = 100,
        .lvl = 47,
        .species = SPECIES_RHYDON,
        .moves = {MOVE_TAKEDOWN, MOVE_HORNDRILL, MOVE_ROCKBLAST, MOVE_SCARYFACE},
    },
    {
        .iv = 100,
        .lvl = 48,
        .species = SPECIES_SLOWBRO,
        .moves = {MOVE_PSYCHIC, MOVE_HEADBUTT, MOVE_AMNESIA, MOVE_DISABLE},
    },
    {
        .iv = 100,
        .lvl = 47,
        .species = SPECIES_KANGASKHAN,
        .moves = {MOVE_DIZZYPUNCH, MOVE_BITE, MOVE_ENDURE, MOVE_REVERSAL},
    },
    {
        .iv = 100,
        .lvl = 48,
        .species = SPECIES_MACHOKE,
        .moves = {MOVE_CROSSCHOP, MOVE_VITALTHROW, MOVE_REVENGE, MOVE_SEISMICTOSS},
    },
    {
        .iv = 100,
        .lvl = 50,
        .species = SPECIES_URSARING,
        .moves = {MOVE_SLASH, MOVE_FEINTATTACK, MOVE_SNORE, MOVE_REST},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerMichelle[] = {
    {
        .iv = 100,
        .lvl = 47,
        .species = SPECIES_PERSIAN,
        .moves = {MOVE_SLASH, MOVE_SCREECH, MOVE_FEINTATTACK, MOVE_BITE},
    },
    {
        .iv = 100,
        .lvl = 47,
        .species = SPECIES_DEWGONG,
        .moves = {MOVE_ICEBEAM, MOVE_TAKEDOWN, MOVE_ICYWIND, MOVE_GROWL},
    },
    {
        .iv = 100,
        .lvl = 48,
        .species = SPECIES_NINETALES,
        .moves = {MOVE_FLAMETHROWER, MOVE_CONFUSERAY, MOVE_WILLOWISP, MOVE_GRUDGE},
    },
    {
        .iv = 100,
        .lvl = 48,
        .species = SPECIES_RAPIDASH,
        .moves = {MOVE_BOUNCE, MOVE_AGILITY, MOVE_FIRESPIN, MOVE_TAKEDOWN},
    },
    {
        .iv = 100,
        .lvl = 50,
        .species = SPECIES_GIRAFARIG,
        .moves = {MOVE_CRUNCH, MOVE_PSYBEAM, MOVE_STOMP, MOVE_ODORSLEUTH},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCoolCoupleLexNya[] = {
    {
        .iv = 100,
        .lvl = 52,
        .species = SPECIES_MILTANK,
        .moves = {MOVE_BODYSLAM, MOVE_MILKDRINK, MOVE_GROWL, MOVE_DEFENSECURL},
    },
    {
        .iv = 100,
        .lvl = 52,
        .species = SPECIES_TAUROS,
        .moves = {MOVE_THRASH, MOVE_HORNATTACK, MOVE_PURSUIT, MOVE_SWAGGER},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRuinManiacBrandon[] = {
    {
        .lvl = 50,
        .species = SPECIES_ONIX,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerRuinManiacBenjamin[] = {
    {
        .lvl = 48,
        .species = SPECIES_GEODUDE,
        .moves = {MOVE_EARTHQUAKE, MOVE_ROCKBLAST, MOVE_ROLLOUT, MOVE_SELFDESTRUCT},
    },
    {
        .lvl = 48,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_EARTHQUAKE, MOVE_ROCKBLAST, MOVE_ROCKTHROW, MOVE_SELFDESTRUCT},
    },
    {
        .lvl = 48,
        .species = SPECIES_GRAVELER,
        .moves = {MOVE_EARTHQUAKE, MOVE_ROCKBLAST, MOVE_ROCKTHROW, MOVE_SELFDESTRUCT},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPainterEdna[] = {
    {
        .iv = 50,
        .lvl = 50,
        .species = SPECIES_SMEARGLE,
        .moves = {MOVE_FAKEOUT, MOVE_EXTREMESPEED, MOVE_PROTECT, MOVE_QUICKATTACK},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGentlemanClifford[] = {
    {
        .lvl = 49,
        .species = SPECIES_MAROWAK,
    },
    {
        .lvl = 49,
        .species = SPECIES_GOLDUCK,
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerLadySelphy[] = {
    {
        .lvl = 49,
        .species = SPECIES_PERSIAN,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {0, 0, 0, 0, 0, 0},
        .evSpread = {0, 0, 0, 0, 0, 0},
        .heldItem = ITEM_NUGGET,
        .moves = {MOVE_PAYDAY, MOVE_BITE, MOVE_TAUNT, MOVE_TORMENT},
        .teraType = TYPE_NORMAL,
    },
    {
        .lvl = 49,
        .species = SPECIES_PERSIAN,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {0, 0, 0, 0, 0, 0},
        .evSpread = {0, 0, 0, 0, 0, 0},
        .heldItem = ITEM_NUGGET,
        .moves = {MOVE_PAYDAY, MOVE_SCRATCH, MOVE_TORMENT, MOVE_TAUNT},
        .teraType = TYPE_NORMAL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRuinManiacLawson[] = {
    {
        .lvl = 47,
        .species = SPECIES_ONIX,
    },
    {
        .lvl = 48,
        .species = SPECIES_GRAVELER,
    },
    {
        .lvl = 49,
        .species = SPECIES_MAROWAK,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPsychicLaura[] = {
    {
        .iv = 100,
        .lvl = 48,
        .species = SPECIES_NATU,
    },
    {
        .iv = 100,
        .lvl = 48,
        .species = SPECIES_NATU,
    },
    {
        .iv = 100,
        .lvl = 49,
        .species = SPECIES_XATU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPkmnBreederBethany[] = {
    {
        .iv = 30,
        .lvl = 50,
        .species = SPECIES_CHANSEY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPkmnBreederAllison[] = {
    {
        .iv = 30,
        .lvl = 48,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = 30,
        .lvl = 48,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = 30,
        .lvl = 48,
        .species = SPECIES_CLEFABLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherGarret[] = {
    {
        .lvl = 49,
        .species = SPECIES_HERACROSS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherJonah[] = {
    {
        .lvl = 45,
        .species = SPECIES_YANMA,
    },
    {
        .lvl = 45,
        .species = SPECIES_BEEDRILL,
    },
    {
        .lvl = 46,
        .species = SPECIES_YANMA,
    },
    {
        .lvl = 47,
        .species = SPECIES_BEEDRILL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherVance[] = {
    {
        .lvl = 48,
        .species = SPECIES_VENONAT,
    },
    {
        .lvl = 48,
        .species = SPECIES_VENOMOTH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterNash[] = {
    {
        .lvl = 47,
        .species = SPECIES_WEEPINBELL,
    },
    {
        .lvl = 47,
        .species = SPECIES_WEEPINBELL,
    },
    {
        .lvl = 49,
        .species = SPECIES_VICTREEBEL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterCordell[] = {
    {
        .lvl = 48,
        .species = SPECIES_FARFETCHD,
    },
    {
        .lvl = 48,
        .species = SPECIES_FARFETCHD,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassDalia[] = {
    {
        .lvl = 46,
        .species = SPECIES_HOPPIP,
    },
    {
        .lvl = 47,
        .species = SPECIES_HOPPIP,
    },
    {
        .lvl = 47,
        .species = SPECIES_SKIPLOOM,
    },
    {
        .lvl = 48,
        .species = SPECIES_SKIPLOOM,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassJoana[] = {
    {
        .lvl = 49,
        .species = SPECIES_SNUBBULL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperRiley[] = {
    {
        .lvl = 49,
        .species = SPECIES_PINSIR,
    },
    {
        .lvl = 50,
        .species = SPECIES_HERACROSS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerMarcy[] = {
    {
        .lvl = 48,
        .species = SPECIES_PARAS,
    },
    {
        .lvl = 48,
        .species = SPECIES_PARAS,
    },
    {
        .lvl = 49,
        .species = SPECIES_PARASECT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRuinManiacLayton[] = {
    {
        .lvl = 48,
        .species = SPECIES_SANDSLASH,
    },
    {
        .lvl = 48,
        .species = SPECIES_ONIX,
    },
    {
        .lvl = 48,
        .species = SPECIES_SANDSLASH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerKelsey2[] = {
    {
        .iv = 20,
        .lvl = 21,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .iv = 20,
        .lvl = 21,
        .species = SPECIES_NIDORAN_F,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerKelsey3[] = {
    {
        .iv = 60,
        .lvl = 29,
        .species = SPECIES_NIDORINO,
    },
    {
        .iv = 60,
        .lvl = 29,
        .species = SPECIES_NIDORINA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerKelsey4[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_NIDORINO,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_NIDORINA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperRicky2[] = {
    {
        .iv = 20,
        .lvl = 22,
        .species = SPECIES_SQUIRTLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperRicky3[] = {
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperRicky4[] = {
    {
        .iv = 120,
        .lvl = 55,
        .species = SPECIES_WARTORTLE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperJeff2[] = {
    {
        .iv = 20,
        .lvl = 21,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = 20,
        .lvl = 21,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperJeff3[] = {
    {
        .iv = 60,
        .lvl = 29,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 60,
        .lvl = 29,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperJeff4[] = {
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerIsabelle2[] = {
    {
        .iv = 20,
        .lvl = 21,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 20,
        .lvl = 21,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 20,
        .lvl = 18,
        .species = SPECIES_PIDGEY,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerIsabelle3[] = {
    {
        .iv = 60,
        .lvl = 29,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 60,
        .lvl = 29,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 60,
        .lvl = 26,
        .species = SPECIES_PIDGEOTTO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerIsabelle4[] = {
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 80,
        .lvl = 50,
        .species = SPECIES_PIDGEOT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterYasu2[] = {
    {
        .iv = 40,
        .lvl = 22,
        .species = SPECIES_RATTATA,
    },
    {
        .iv = 40,
        .lvl = 22,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 40,
        .lvl = 22,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterYasu3[] = {
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_RATICATE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerEngineerBernie2[] = {
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_MAGNETON,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_MAGNETON,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_MAGNETON,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGamerDarian2[] = {
    {
        .iv = 60,
        .lvl = 29,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = 60,
        .lvl = 29,
        .species = SPECIES_VULPIX,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperChris2[] = {
    {
        .iv = 40,
        .lvl = 24,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = 40,
        .lvl = 24,
        .species = SPECIES_CHARMANDER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperChris3[] = {
    {
        .iv = 60,
        .lvl = 29,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = 60,
        .lvl = 29,
        .species = SPECIES_CHARMELEON,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCamperChris4[] = {
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_ARCANINE,
    },
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_CHARMELEON,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerAlicia2[] = {
    {
        .iv = 40,
        .lvl = 25,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerAlicia3[] = {
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_PERSIAN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerAlicia4[] = {
    {
        .iv = 120,
        .lvl = 55,
        .species = SPECIES_PERSIAN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerJeremy2[] = {
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_MACHOKE,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_ONIX,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokemaniacMark3[] = {
    {
        .iv = 150,
        .lvl = 54,
        .species = SPECIES_RHYDON,
    },
    {
        .iv = 150,
        .lvl = 54,
        .species = SPECIES_LICKITUNG,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokemaniacHerman2[] = {
    {
        .iv = 90,
        .lvl = 29,
        .species = SPECIES_MAROWAK,
    },
    {
        .iv = 90,
        .lvl = 29,
        .species = SPECIES_SLOWBRO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokemaniacHerman3[] = {
    {
        .iv = 150,
        .lvl = 54,
        .species = SPECIES_MAROWAK,
    },
    {
        .iv = 150,
        .lvl = 54,
        .species = SPECIES_SLOWBRO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerTrent2[] = {
    {
        .iv = 60,
        .lvl = 31,
        .species = SPECIES_ONIX,
    },
    {
        .iv = 60,
        .lvl = 31,
        .species = SPECIES_GRAVELER,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassMegan2[] = {
    {
        .iv = 40,
        .lvl = 22,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 40,
        .lvl = 22,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 40,
        .lvl = 23,
        .species = SPECIES_NIDORAN_M,
    },
    {
        .iv = 40,
        .lvl = 21,
        .species = SPECIES_MEOWTH,
    },
    {
        .iv = 40,
        .lvl = 22,
        .species = SPECIES_PIKACHU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerLassMegan3[] = {
    {
        .iv = 80,
        .lvl = 46,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_NIDORINO,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_PERSIAN,
    },
    {
        .iv = 80,
        .lvl = 48,
        .species = SPECIES_RAICHU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSuperNerdGlenn2[] = {
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_MUK,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_MUK,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_MUK,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerGamerRich2[] = {
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_GROWLITHE,
    },
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_VULPIX,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBikerJaren2[] = {
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_MUK,
    },
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_MUK,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanElliot2[] = {
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_CLOYSTER,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_SEADRA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRockerLuca2[] = {
    {
        .iv = 60,
        .lvl = 33,
        .species = SPECIES_ELECTRODE,
    },
    {
        .iv = 60,
        .lvl = 33,
        .species = SPECIES_ELECTRODE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBeautySheila2[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_PERSIAN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperRobert2[] = {
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperRobert3[] = {
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerSusie2[] = {
    {
        .iv = 60,
        .lvl = 27,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 60,
        .lvl = 27,
        .species = SPECIES_MEOWTH,
    },
    {
        .iv = 60,
        .lvl = 27,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 60,
        .lvl = 27,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = 60,
        .lvl = 27,
        .species = SPECIES_MEOWTH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerSusie3[] = {
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_PERSIAN,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_PERSIAN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerSusie4[] = {
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_PERSIAN,
    },
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_RAICHU,
    },
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_PERSIAN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBikerLukas2[] = {
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_KOFFING,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_KOFFING,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_MUK,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_WEEZING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperBenny2[] = {
    {
        .iv = 60,
        .lvl = 32,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 60,
        .lvl = 32,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperBenny3[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperMarlon2[] = {
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_DODUO,
    },
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperMarlon3[] = {
    {
        .iv = 80,
        .lvl = 48,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 80,
        .lvl = 48,
        .species = SPECIES_DODRIO,
    },
    {
        .iv = 80,
        .lvl = 48,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBeautyGrace2[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_PIDGEOT,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_WIGGLYTUFF,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperChester2[] = {
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_DODRIO,
    },
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_DODRIO,
    },
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_DODUO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperChester3[] = {
    {
        .iv = 80,
        .lvl = 48,
        .species = SPECIES_DODRIO,
    },
    {
        .iv = 80,
        .lvl = 48,
        .species = SPECIES_DODRIO,
    },
    {
        .iv = 80,
        .lvl = 48,
        .species = SPECIES_DODRIO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerBecky2[] = {
    {
        .iv = 60,
        .lvl = 32,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = 60,
        .lvl = 32,
        .species = SPECIES_RAICHU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerBecky3[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_RAICHU,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerBecky4[] = {
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_RAICHU,
    },
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_RAICHU,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushKinRonMya2[] = {
    {
        .iv = 110,
        .lvl = 33,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 110,
        .lvl = 33,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushKinRonMya3[] = {
    {
        .iv = 130,
        .lvl = 51,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 130,
        .lvl = 51,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushKinRonMya4[] = {
    {
        .iv = 170,
        .lvl = 56,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 170,
        .lvl = 56,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBikerRuben2[] = {
    {
        .iv = 80,
        .lvl = 48,
        .species = SPECIES_WEEZING,
    },
    {
        .iv = 80,
        .lvl = 48,
        .species = SPECIES_WEEZING,
    },
    {
        .iv = 80,
        .lvl = 48,
        .species = SPECIES_WEEZING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCueBallCamron2[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_PRIMEAPE,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBikerJaxon2[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_WEEZING,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_MUK,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCueBallIsaiah2[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_MACHOKE,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_MACHAMP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCueBallCorey2[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_PRIMEAPE,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_MACHAMP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperJacob2[] = {
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_SPEAROW,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 60,
        .lvl = 28,
        .species = SPECIES_SPEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperJacob3[] = {
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleAlice2[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleDarrin2[] = {
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_SEADRA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerMissy2[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPicnickerMissy3[] = {
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_SEAKING,
    },
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerFishermanWade2[] = {
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_MAGIKARP,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_MAGIKARP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleJack2[] = {
    {
        .iv = 80,
        .lvl = 50,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSisAndBroLilIan2[] = {
    {
        .iv = 80,
        .lvl = 50,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = 80,
        .lvl = 50,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSisAndBroLilIan3[] = {
    {
        .iv = 120,
        .lvl = 55,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = 120,
        .lvl = 55,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleFinn2[] = {
    {
        .iv = 80,
        .lvl = 50,
        .species = SPECIES_STARMIE,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushGirlSharon2[] = {
    {
        .iv = 130,
        .lvl = 50,
        .species = SPECIES_MANKEY,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 130,
        .lvl = 50,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushGirlSharon3[] = {
    {
        .iv = 170,
        .lvl = 55,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 170,
        .lvl = 55,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushGirlTanya2[] = {
    {
        .iv = 130,
        .lvl = 50,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 130,
        .lvl = 50,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushGirlTanya3[] = {
    {
        .iv = 170,
        .lvl = 55,
        .species = SPECIES_HITMONLEE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 170,
        .lvl = 55,
        .species = SPECIES_HITMONCHAN,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltShea2[] = {
    {
        .iv = 180,
        .lvl = 50,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 180,
        .lvl = 50,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltShea3[] = {
    {
        .iv = 220,
        .lvl = 55,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 220,
        .lvl = 55,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltHugh2[] = {
    {
        .iv = 180,
        .lvl = 50,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 180,
        .lvl = 50,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerBlackBeltHugh3[] = {
    {
        .iv = 220,
        .lvl = 55,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 220,
        .lvl = 55,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushKinMikKia2[] = {
    {
        .iv = 130,
        .lvl = 51,
        .species = SPECIES_MACHOKE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 130,
        .lvl = 51,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushKinMikKia3[] = {
    {
        .iv = 170,
        .lvl = 56,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 170,
        .lvl = 56,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTuberAmira2[] = {
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = 80,
        .lvl = 47,
        .species = SPECIES_POLIWHIRL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTwinsJoyMeg2[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_CLEFAIRY,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPainterRayna2[] = {
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_SMEARGLE,
        .moves = {MOVE_CROSSCHOP, MOVE_MEGAHORN, MOVE_DOUBLEEDGE, MOVE_SELFDESTRUCT},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungsterDestin2[] = {
    {
        .iv = 120,
        .lvl = 53,
        .species = SPECIES_RATICATE,
    },
    {
        .iv = 120,
        .lvl = 53,
        .species = SPECIES_PIDGEOT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPkmnBreederAlize2[] = {
    {
        .iv = 150,
        .lvl = 53,
        .species = SPECIES_PIKACHU,
    },
    {
        .iv = 150,
        .lvl = 53,
        .species = SPECIES_CLEFAIRY,
    },
    {
        .iv = 150,
        .lvl = 53,
        .species = SPECIES_MARILL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungCoupleGiaJes2[] = {
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_NIDORINA,
    },
    {
        .iv = 60,
        .lvl = 30,
        .species = SPECIES_NIDORINO,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerYoungCoupleGiaJes3[] = {
    {
        .iv = 120,
        .lvl = 55,
        .species = SPECIES_NIDOKING,
    },
    {
        .iv = 120,
        .lvl = 55,
        .species = SPECIES_NIDOQUEEN,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperMilo2[] = {
    {
        .iv = 120,
        .lvl = 53,
        .species = SPECIES_PIDGEOTTO,
    },
    {
        .iv = 120,
        .lvl = 55,
        .species = SPECIES_PIDGEOT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperChaz2[] = {
    {
        .iv = 120,
        .lvl = 53,
        .species = SPECIES_FEAROW,
    },
    {
        .iv = 120,
        .lvl = 55,
        .species = SPECIES_FEAROW,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBirdKeeperHarold2[] = {
    {
        .iv = 120,
        .lvl = 53,
        .species = SPECIES_NOCTOWL,
    },
    {
        .iv = 120,
        .lvl = 55,
        .species = SPECIES_NOCTOWL,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleNicole2[] = {
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_MARILL,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPsychicJaclyn2[] = {
    {
        .iv = 220,
        .lvl = 52,
        .species = SPECIES_NATU,
        .moves = {MOVE_PSYCHIC, MOVE_CONFUSERAY, MOVE_FUTURESIGHT, MOVE_WISH},
    },
    {
        .iv = 220,
        .lvl = 52,
        .species = SPECIES_SLOWBRO,
        .moves = {MOVE_PSYCHIC, MOVE_HEADBUTT, MOVE_AMNESIA, MOVE_YAWN},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_KADABRA,
        .moves = {MOVE_PSYCHIC, MOVE_FUTURESIGHT, MOVE_RECOVER, MOVE_REFLECT},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleSamir2[] = {
    {
        .iv = 120,
        .lvl = 55,
        .species = SPECIES_GYARADOS,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerHikerEarl2[] = {
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_ONIX,
    },
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_MACHAMP,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerRuinManiacLarry2[] = {
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_MACHOKE,
    },
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_MACHOKE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerPokemaniacHector2[] = {
    {
        .iv = 150,
        .lvl = 55,
        .species = SPECIES_RHYDON,
    },
    {
        .iv = 150,
        .lvl = 55,
        .species = SPECIES_KANGASKHAN,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPsychicDario2[] = {
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_GIRAFARIG,
        .moves = {MOVE_CRUNCH, MOVE_PSYBEAM, MOVE_ODORSLEUTH, MOVE_AGILITY},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPsychicRodette2[] = {
    {
        .iv = 220,
        .lvl = 53,
        .species = SPECIES_NATU,
        .moves = {MOVE_PSYCHIC, MOVE_CONFUSERAY, MOVE_WISH, MOVE_FUTURESIGHT},
    },
    {
        .iv = 220,
        .lvl = 53,
        .species = SPECIES_HYPNO,
        .moves = {MOVE_PSYCHIC, MOVE_DISABLE, MOVE_PSYCHUP, MOVE_FUTURESIGHT},
    },
    {
        .iv = 220,
        .lvl = 53,
        .species = SPECIES_HYPNO,
        .moves = {MOVE_PSYCHIC, MOVE_HYPNOSIS, MOVE_PSYCHUP, MOVE_FUTURESIGHT},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerJugglerMason2[] = {
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_ELECTRODE,
    },
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_PINECO,
    },
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_ELECTRODE,
    },
    {
        .iv = 120,
        .lvl = 52,
        .species = SPECIES_PINECO,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPkmnRangerNicolas2[] = {
    {
        .iv = 220,
        .lvl = 55,
        .species = SPECIES_VICTREEBEL,
        .moves = {MOVE_RAZORLEAF, MOVE_ACID, MOVE_STUNSPORE, MOVE_WRAP},
    },
    {
        .iv = 220,
        .lvl = 55,
        .species = SPECIES_VICTREEBEL,
        .moves = {MOVE_RAZORLEAF, MOVE_ACID, MOVE_SLEEPPOWDER, MOVE_SLAM},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPkmnRangerMadeline2[] = {
    {
        .iv = 220,
        .lvl = 55,
        .species = SPECIES_VILEPLUME,
        .moves = {MOVE_PETALDANCE, MOVE_MOONLIGHT, MOVE_ACID, MOVE_SLEEPPOWDER},
    },
    {
        .iv = 220,
        .lvl = 55,
        .species = SPECIES_VILEPLUME,
        .moves = {MOVE_PETALDANCE, MOVE_MOONLIGHT, MOVE_ACID, MOVE_STUNSPORE},
    },
};

static const struct TrainerMonItemDefaultMoves sParty_TrainerCrushGirlCyndy2[] = {
    {
        .iv = 170,
        .lvl = 54,
        .species = SPECIES_PRIMEAPE,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 170,
        .lvl = 54,
        .species = SPECIES_HITMONTOP,
        .heldItem = ITEM_BLACK_BELT,
    },
    {
        .iv = 170,
        .lvl = 54,
        .species = SPECIES_MACHAMP,
        .heldItem = ITEM_BLACK_BELT,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerTamerEvan2[] = {
    {
        .iv = 160,
        .lvl = 52,
        .species = SPECIES_SANDSLASH,
    },
    {
        .iv = 160,
        .lvl = 52,
        .species = SPECIES_LICKITUNG,
    },
    {
        .iv = 160,
        .lvl = 55,
        .species = SPECIES_URSARING,
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPkmnRangerJackson2[] = {
    {
        .iv = 220,
        .lvl = 53,
        .species = SPECIES_TANGELA,
        .moves = {MOVE_SLAM, MOVE_MEGADRAIN, MOVE_BIND, MOVE_INGRAIN},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_EXEGGUTOR,
        .moves = {MOVE_CONFUSION, MOVE_EGGBOMB, MOVE_STUNSPORE, MOVE_REFLECT},
    },
    {
        .iv = 220,
        .lvl = 55,
        .species = SPECIES_EXEGGUTOR,
        .moves = {MOVE_CONFUSION, MOVE_EGGBOMB, MOVE_SLEEPPOWDER, MOVE_STOMP},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerPkmnRangerKatelyn2[] = {
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_CHANSEY,
        .moves = {MOVE_EGGBOMB, MOVE_DEFENSECURL, MOVE_MINIMIZE, MOVE_SOFTBOILED},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerLeroy2[] = {
    {
        .iv = 220,
        .lvl = 52,
        .species = SPECIES_RHYDON,
        .moves = {MOVE_EARTHQUAKE, MOVE_HORNDRILL, MOVE_ROCKBLAST, MOVE_SCARYFACE},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_SLOWBRO,
        .moves = {MOVE_PSYCHIC, MOVE_HEADBUTT, MOVE_AMNESIA, MOVE_DISABLE},
    },
    {
        .iv = 220,
        .lvl = 52,
        .species = SPECIES_KANGASKHAN,
        .moves = {MOVE_DIZZYPUNCH, MOVE_BITE, MOVE_ENDURE, MOVE_REVERSAL},
    },
    {
        .iv = 220,
        .lvl = 52,
        .species = SPECIES_MACHAMP,
        .moves = {MOVE_CROSSCHOP, MOVE_VITALTHROW, MOVE_REVENGE, MOVE_SEISMICTOSS},
    },
    {
        .iv = 220,
        .lvl = 55,
        .species = SPECIES_URSARING,
        .moves = {MOVE_SLASH, MOVE_FEINTATTACK, MOVE_SNORE, MOVE_REST},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCooltrainerMichelle2[] = {
    {
        .iv = 220,
        .lvl = 53,
        .species = SPECIES_PERSIAN,
        .moves = {MOVE_SLASH, MOVE_SCREECH, MOVE_FEINTATTACK, MOVE_BITE},
    },
    {
        .iv = 220,
        .lvl = 53,
        .species = SPECIES_DEWGONG,
        .moves = {MOVE_ICEBEAM, MOVE_TAKEDOWN, MOVE_ICYWIND, MOVE_SHEERCOLD},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_NINETALES,
        .moves = {MOVE_FLAMETHROWER, MOVE_CONFUSERAY, MOVE_WILLOWISP, MOVE_GRUDGE},
    },
    {
        .iv = 220,
        .lvl = 54,
        .species = SPECIES_RAPIDASH,
        .moves = {MOVE_BOUNCE, MOVE_AGILITY, MOVE_FIRESPIN, MOVE_TAKEDOWN},
    },
    {
        .iv = 220,
        .lvl = 56,
        .species = SPECIES_GIRAFARIG,
        .moves = {MOVE_CRUNCH, MOVE_PSYBEAM, MOVE_STOMP, MOVE_ODORSLEUTH},
    },
};

static const struct TrainerMonNoItemCustomMoves sParty_TrainerCoolCoupleLexNya2[] = {
    {
        .iv = 220,
        .lvl = 57,
        .species = SPECIES_MILTANK,
        .moves = {MOVE_BODYSLAM, MOVE_MILKDRINK, MOVE_GROWL, MOVE_DEFENSECURL},
    },
    {
        .iv = 220,
        .lvl = 57,
        .species = SPECIES_TAUROS,
        .moves = {MOVE_TAKEDOWN, MOVE_SCARYFACE, MOVE_PURSUIT, MOVE_SWAGGER},
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherColton2[] = {
    {
        .iv = 20,
        .lvl = 19,
        .species = SPECIES_METAPOD,
    },
    {
        .iv = 20,
        .lvl = 19,
        .species = SPECIES_WEEDLE,
    },
    {
        .iv = 20,
        .lvl = 19,
        .species = SPECIES_METAPOD,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherColton3[] = {
    {
        .iv = 60,
        .lvl = 27,
        .species = SPECIES_BUTTERFREE,
    },
    {
        .iv = 60,
        .lvl = 27,
        .species = SPECIES_KAKUNA,
    },
    {
        .iv = 60,
        .lvl = 27,
        .species = SPECIES_BUTTERFREE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerBugCatcherColton4[] = {
    {
        .iv = 120,
        .lvl = 51,
        .species = SPECIES_BUTTERFREE,
    },
    {
        .iv = 120,
        .lvl = 54,
        .species = SPECIES_BEEDRILL,
    },
    {
        .iv = 120,
        .lvl = 51,
        .species = SPECIES_BUTTERFREE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleMatthew2[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_POLIWRATH,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerMaleTony2[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_SEADRA,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_SEADRA,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerSwimmerFemaleMelissa2[] = {
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_POLIWHIRL,
    },
    {
        .iv = 80,
        .lvl = 49,
        .species = SPECIES_SEAKING,
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerEliteFourLorelei2[] = {
    {
        .lvl = 64,
        .species = SPECIES_DEWGONG,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_ICEBEAM, MOVE_SURF, MOVE_SIGNALBEAM, MOVE_DOUBLETEAM},
        .teraType = TYPE_ICE,
    },
    {
        .lvl = 63,
        .species = SPECIES_CLOYSTER,
        .ability = Ability_RandomAll,
        .nature = NATURE_JOLLY,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_ICEBEAM, MOVE_SURF, MOVE_SUPERSONIC, MOVE_RAINDANCE},
        .teraType = TYPE_WATER,
    },
    {
        .lvl = 63,
        .species = SPECIES_PILOSWINE,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_BLIZZARD, MOVE_EARTHQUAKE, MOVE_DOUBLEEDGE, MOVE_ROCKSLIDE},
        .teraType = TYPE_ICE,
    },
    {
        .lvl = 66,
        .species = SPECIES_JYNX,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_ICEBEAM, MOVE_PSYCHIC, MOVE_LOVELYKISS, MOVE_ATTRACT},
        .teraType = TYPE_ICE,
    },
    {
        .lvl = 66,
        .species = SPECIES_LAPRAS,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31, 31, 31, 31, 31, 31},
        .evSpread = {85, 85, 85, 85, 85, 85},
        .heldItem = ITEM_CHERI_BERRY,
        .moves = {MOVE_ICEBEAM, MOVE_SURF, MOVE_PSYCHIC, MOVE_THUNDER},
        .teraType = TYPE_WATER,
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerEliteFourBruno2[] = {
    {
        .lvl = 65,
        .species = SPECIES_STEELIX,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_EARTHQUAKE, MOVE_ROCKTOMB, MOVE_IRONTAIL, MOVE_ROAR},
        .teraType = TYPE_STEEL,
    },
    {
        .lvl = 65,
        .species = SPECIES_HITMONCHAN,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SKYUPPERCUT, MOVE_MACHPUNCH, MOVE_ROCKTOMB, MOVE_COUNTER},
        .teraType = TYPE_FIGHTING,
    },
    {
        .lvl = 65,
        .species = SPECIES_HITMONLEE,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_MEGAKICK, MOVE_FORESIGHT, MOVE_BRICKBREAK, MOVE_FACADE},
        .teraType = TYPE_FIGHTING,
    },
    {
        .lvl = 66,
        .species = SPECIES_STEELIX,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_DOUBLEEDGE, MOVE_EARTHQUAKE, MOVE_IRONTAIL, MOVE_SANDTOMB},
        .teraType = TYPE_STEEL,
    },
    {
        .lvl = 68,
        .species = SPECIES_MACHAMP_GIGA,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_CROSSCHOP, MOVE_BULKUP, MOVE_SCARYFACE, MOVE_ROCKTOMB},
        .teraType = TYPE_FIGHTING,
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerEliteFourAgatha2[] = {
    {
        .lvl = 66,
        .species = SPECIES_GENGAR,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SHADOWBALL, MOVE_PSYCHIC, MOVE_CONFUSERAY, MOVE_HYPNOSIS},
        .teraType = TYPE_GHOST,
    },
    {
        .lvl = 66,
        .species = SPECIES_CROBAT,
        .ability = Ability_RandomAll,
        .nature = NATURE_JOLLY,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SLUDGEBOMB, MOVE_AIRCUTTER, MOVE_SHADOWBALL, MOVE_CONFUSERAY},
        .teraType = TYPE_POISON,
    },
    {
        .lvl = 65,
        .species = SPECIES_MISDREAVUS,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SHADOWBALL, MOVE_PSYCHIC, MOVE_THUNDERBOLT, MOVE_ATTRACT},
        .teraType = TYPE_GHOST,
    },
    {
        .lvl = 68,
        .species = SPECIES_ARBOK,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_SLUDGEBOMB, MOVE_EARTHQUAKE, MOVE_GIGADRAIN, MOVE_DOUBLETEAM},
        .teraType = TYPE_POISON,
    },
    {
        .lvl = 70,
        .species = SPECIES_GENGAR,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_CHESTO_BERRY,
        .moves = {MOVE_SHADOWBALL, MOVE_PSYCHIC, MOVE_THUNDERBOLT, MOVE_SLUDGEBOMB},
        .teraType = TYPE_GHOST,
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerEliteFourLance2[] = {
    {
        .lvl = 68,
        .species = SPECIES_GYARADOS,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPERBEAM, MOVE_DRAGONDANCE, MOVE_EARTHQUAKE, MOVE_THUNDERWAVE},
        .teraType = TYPE_WATER,
    },
    {
        .lvl = 66,
        .species = SPECIES_DRAGONITE,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPERBEAM, MOVE_EARTHQUAKE, MOVE_DRAGONCLAW, MOVE_FLAMETHROWER},
        .teraType = TYPE_DRAGON,
    },
    {
        .lvl = 66,
        .species = SPECIES_KINGDRA,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPERBEAM, MOVE_DRAGONDANCE, MOVE_SURF, MOVE_ICEBEAM},
        .teraType = TYPE_WATER,
    },
    {
        .lvl = 70,
        .species = SPECIES_AERODACTYL,
        .ability = Ability_RandomAll,
        .nature = NATURE_JOLLY,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYPERBEAM, MOVE_ANCIENTPOWER, MOVE_AERIALACE, MOVE_EARTHQUAKE},
        .teraType = TYPE_ROCK,
    },
    {
        .lvl = 72,
        .species = SPECIES_DRAGONITE,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_PERSIM_BERRY,
        .moves = {MOVE_HYPERBEAM, MOVE_OUTRAGE, MOVE_THUNDERBOLT, MOVE_ICEBEAM},
        .teraType = TYPE_DRAGON,
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerChampionRematchSquirtle[] = {
    {
        .lvl = 72,
        .species = SPECIES_HERACROSS,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_MEGAHORN, MOVE_EARTHQUAKE, MOVE_COUNTER, MOVE_ROCKTOMB},
        .teraType = TYPE_BUG,
    },
    {
        .lvl = 73,
        .species = SPECIES_ALAKAZAM,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_PSYCHIC, MOVE_FUTURESIGHT, MOVE_RECOVER, MOVE_REFLECT},
        .teraType = TYPE_PSYCHIC,
    },
    {
        .lvl = 72,
        .species = SPECIES_TYRANITAR,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_CRUNCH, MOVE_EARTHQUAKE, MOVE_THUNDERBOLT, MOVE_AERIALACE},
        .teraType = TYPE_ROCK,
    },
    {
        .lvl = 73,
        .species = SPECIES_ARCANINE_H,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_EXTREMESPEED, MOVE_FLAMETHROWER, MOVE_ROAR, MOVE_BITE},
        .teraType = TYPE_FIRE,
    },
    {
        .lvl = 73,
        .species = SPECIES_EXEGGUTOR,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_GIGADRAIN, MOVE_EGGBOMB, MOVE_SLEEPPOWDER, MOVE_LIGHTSCREEN},
        .teraType = TYPE_GRASS,
    },
    {
        .lvl = 75,
        .species = SPECIES_BLASTOISE_GIGA,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_HYDROPUMP, MOVE_RAINDANCE, MOVE_SKULLBASH, MOVE_BITE},
        .teraType = TYPE_WATER,
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerChampionRematchBulbasaur[] = {
    {
        .lvl = 72,
        .species = SPECIES_HERACROSS,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_MEGAHORN, MOVE_EARTHQUAKE, MOVE_COUNTER, MOVE_ROCKTOMB},
        .teraType = TYPE_BUG,
    },
    {
        .lvl = 73,
        .species = SPECIES_ALAKAZAM,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOWBALL, MOVE_CALMMIND, MOVE_REFLECT},
        .teraType = TYPE_PSYCHIC,
    },
    {
        .lvl = 72,
        .species = SPECIES_TYRANITAR,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_CRUNCH, MOVE_EARTHQUAKE, MOVE_THUNDERBOLT, MOVE_AERIALACE},
        .teraType = TYPE_ROCK,
    },
    {
        .lvl = 73,
        .species = SPECIES_GYARADOS,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYDROPUMP, MOVE_DRAGONDANCE, MOVE_EARTHQUAKE, MOVE_HYPERBEAM},
        .teraType = TYPE_WATER,
    },
    {
        .lvl = 73,
        .species = SPECIES_ARCANINE_H,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_EXTREMESPEED, MOVE_FLAMETHROWER, MOVE_ROAR, MOVE_BITE},
        .teraType = TYPE_FIRE,
    },
    {
        .lvl = 75,
        .species = SPECIES_VENUSAUR_GIGA,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_SOLARBEAM, MOVE_SYNTHESIS, MOVE_SUNNYDAY, MOVE_GROWTH},
        .teraType = TYPE_GRASS,
    },
};

static const struct TrainerMonItemCustomMoves sParty_TrainerChampionRematchCharmander[] = {
    {
        .lvl = 72,
        .species = SPECIES_HERACROSS,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_MEGAHORN, MOVE_EARTHQUAKE, MOVE_COUNTER, MOVE_ROCKTOMB},
        .teraType = TYPE_BUG,
    },
    {
        .lvl = 73,
        .species = SPECIES_ALAKAZAM,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_PSYCHIC, MOVE_SHADOWBALL, MOVE_CALMMIND, MOVE_REFLECT},
        .teraType = TYPE_PSYCHIC,
    },
    {
        .lvl = 72,
        .species = SPECIES_TYRANITAR,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_CRUNCH, MOVE_EARTHQUAKE, MOVE_THUNDERBOLT, MOVE_AERIALACE},
        .teraType = TYPE_ROCK,
    },
    {
        .lvl = 73,
        .species = SPECIES_EXEGGUTOR,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_GIGADRAIN, MOVE_PSYCHIC, MOVE_SLEEPPOWDER, MOVE_LIGHTSCREEN},
        .teraType = TYPE_GRASS,
    },
    {
        .lvl = 73,
        .species = SPECIES_GYARADOS,
        .ability = Ability_RandomAll,
        .nature = NATURE_ADAMANT,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_NONE,
        .moves = {MOVE_HYDROPUMP, MOVE_DRAGONDANCE, MOVE_EARTHQUAKE, MOVE_HYPERBEAM},
        .teraType = TYPE_WATER,
    },
    {
        .lvl = 75,
        .species = SPECIES_CHARIZARD,
        .ability = Ability_RandomAll,
        .nature = NATURE_MODEST,
        .ivSpread = {31,31,31,31,31,31},
        .evSpread = {85,85,85,85,85,85},
        .heldItem = ITEM_SITRUS_BERRY,
        .moves = {MOVE_FIREBLAST, MOVE_DRAGONCLAW, MOVE_AERIALACE, MOVE_EARTHQUAKE},
        .teraType = TYPE_FIRE,
    },
};

static const struct TrainerMonNoItemDefaultMoves sParty_TrainerCueBallPaxton[] = {
    {
        .lvl = 39,
        .species = SPECIES_WEEZING,
    },
    {
        .lvl = 39,
        .species = SPECIES_MUK,
    },
};
#endif