#include "defines.h"
#include "../include/bike.h"
#include "../include/field_player_avatar.h"
#include "../include/fieldmap.h"
#include "../include/field_message_box.h"
#include "../include/field_weather.h"
#include "../include/gpu_regs.h"
#include "../include/item_icon.h"
#include "../include/item_menu.h"
#include "../include/list_menu.h"
#include "../include/map_name_popup.h"
#include "../include/menu.h"
#include "../include/m4a.h"
#include "../include/naming_screen.h"
#include "../include/overworld.h"
#include "../include/pokemon_summary_screen.h"
#include "../include/pokemon_storage_system.h"
#include "../include/region_map.h"
#include "../include/script.h"
#include "../include/script_menu.h"
#include "../include/sound.h"
#include "../include/sprite.h"
#include "../include/string_util.h"
#include "../include/text.h"
#include "../include/wild_encounter.h"
#include "../include/window.h"
#include "../include/event_data.h"
#include "../include/constants/abilities.h"
#include "../include/constants/items.h"
#include "../include/constants/moves.h"
#include "../include/constants/pokedex.h"
#include "../include/constants/pokemon.h"
#include "../include/constants/songs.h"

#include "../include/new/battle_strings.h"
#include "../include/new/build_pokemon.h"
#include "../include/new/catching.h"
#include "../include/new/damage_calc.h"
#include "../include/new/dns.h"
#include "../include/new/util.h"
#include "../include/new/item.h"
#include "../include/new/learn_move.h"
#include "../include/new_menu_helpers.h"
#include "../include/new/multi.h"
#include "../include/new/overworld.h"
#include "../include/new/pokemon_storage_system.h"
#include "../include/new/ram_locs_battle.h"
#include "../include/new/read_keys.h"
#include "../include/new/roamer.h"
#include "../include/new/text.h"
#include "../include/new/scrolling_multichoice.h"
#include "../include/new/Vanilla_functions_battle.h"
#include "../include/new/wild_encounter.h"
#include "../include/random.h"

#define LOW_HAPPINESS 70
#define HIGH_HAPPINESS 200
#define LOW_HP_PERCENT 20
#define ITEM_FIND_CHANCE 3  // 3% chance

extern const u8 sText_HappyJumping[];
extern const u8 sText_LooksLonely[];
extern const u8 sText_TiredButReady[];
extern const u8 sText_StatusAsleep[];
extern const u8 sText_StatusParalyzed[];
extern const u8 sText_StatusPoisoned[];
extern const u8 sText_StatusBurned[];
extern const u8 sText_StatusFrozen[];

extern const u8 sText_TypeFire[];
extern const u8 sText_TypeWater[];
extern const u8 sText_TypeElectric[];
extern const u8 sText_TypeGrass[];
extern const u8 sText_TypeIce[];
extern const u8 sText_TypeFighting[];
extern const u8 sText_TypeFlying[];
extern const u8 sText_TypePoison[];
extern const u8 sText_TypeGround[];
extern const u8 sText_TypeRock[];
extern const u8 sText_TypeBug[];
extern const u8 sText_TypeGhost[];
extern const u8 sText_TypeSteel[];
extern const u8 sText_TypePsychic[];
extern const u8 sText_TypeDragon[];
extern const u8 sText_TypeDark[];
extern const u8 sText_TypeFairy[];
extern const u8 sText_TypeNormal[];

extern const u8 sText_FoundItem[];
extern const u8 sText_IdleLookingAround[];
extern const u8 sText_IdleBored[];
extern const u8 sText_IdleStretching[];
extern const u8 sText_IdleTailWag[];
extern const u8 sText_RecallsMetLocation[];
extern const u8 sText_NatureHardy[];
extern const u8 sText_NatureLonely[];
extern const u8 sText_NatureBrave[];
extern const u8 sText_NatureAdamant[];
extern const u8 sText_NatureNaughty[];

extern const u8 sText_NatureBold[];
extern const u8 sText_NatureDocile[];
extern const u8 sText_NatureRelaxed[];
extern const u8 sText_NatureImpish[];
extern const u8 sText_NatureLax[];

extern const u8 sText_NatureTimid[];
extern const u8 sText_NatureHasty[];
extern const u8 sText_NatureSerious[];
extern const u8 sText_NatureJolly[];
extern const u8 sText_NatureNaive[];

extern const u8 sText_NatureModest[];
extern const u8 sText_NatureMild[];
extern const u8 sText_NatureQuiet[];
extern const u8 sText_NatureBashful[];
extern const u8 sText_NatureRash[];

extern const u8 sText_NatureCalm[];
extern const u8 sText_NatureGentle[];
extern const u8 sText_NatureSassy[];
extern const u8 sText_NatureCareful[];
extern const u8 sText_NatureQuirky[];

extern const u8 sText_HoldingFood[];
extern const u8 sText_ChecksHeldItem[];
extern const u8 sText_LuxuryBallPride[];
extern const u8 sText_TryingToStandUpright[];
extern const u8 sText_StompingAround[];
extern const u8 sText_SkippingAlong[];
extern const u8 sText_SeemsDistracted[];
extern const u8 sText_FlexesMuscles[];

extern const u8 sText_EnjoyingSunlight[];
extern const u8 sText_NighttimeEnergy[];
extern const u8 sText_MorningGreeting[];

extern void ShowMysteryGiftMon(void);
extern struct Pokemon* GetFirstValidPartyMon(void);
extern u32 GetPlayerTrainerId(void);

void ShowFollowerMessage(const u8* message)
{
    StringExpandPlaceholders(gStringVar4, message);
    ShowFieldMessage(gStringVar4);
}

void ShowAnonymousFollowerMessage(void)
{
    struct Pokemon *mon = GetFirstValidPartyMon();
    if (mon == NULL)
        return;

    u8 happiness = GetMonData(mon, MON_DATA_FRIENDSHIP, NULL);
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u32 hp = GetMonData(mon, MON_DATA_HP, NULL);
    u32 maxHp = GetMonData(mon, MON_DATA_MAX_HP, NULL);
    u32 status = GetMonData(mon, MON_DATA_STATUS, NULL);
    u8 type1 = gBaseStats[species].type1;
    u8 metLocation = GetMonData(mon, MON_DATA_MET_LOCATION, NULL);
    u16 heldItem = GetMonData(mon, MON_DATA_HELD_ITEM, NULL);
    u8 ballType = GetMonData(mon, MON_DATA_POKEBALL, NULL);

    const u8 *text = NULL;

    if (((Random() % 100) < ITEM_FIND_CHANCE))
    {
        u8 commonItems[] = {ITEM_POTION, ITEM_POKE_BALL, ITEM_ANTIDOTE, ITEM_PARALYZE_HEAL, ITEM_AWAKENING, ITEM_BURN_HEAL, ITEM_ICE_HEAL};
        u8 item = commonItems[Random() % ARRAY_COUNT(commonItems)];
    
        AddBagItem(item, 1);
        text = sText_FoundItem; 
    }
    else if (((Random() % 100) < 10) && (happiness >= HIGH_HAPPINESS))
        text = sText_HappyJumping;
    else if (((Random() % 100) < 10) && (hp != 0 && ((hp * 100) / maxHp) <= LOW_HP_PERCENT))
        text = sText_TiredButReady;
    else if (((Random() % 100) < 10))
    {
        if (status & STATUS1_SLEEP)
            text = sText_StatusAsleep;
        else if (status & STATUS_PSN_ANY)
            text = sText_StatusPoisoned;
        else if (status & STATUS1_PARALYSIS)
            text = sText_StatusParalyzed;
        else if (status & STATUS1_BURN)
            text = sText_StatusBurned;
        else if (status & STATUS1_FREEZE)
            text = sText_StatusFrozen;
    }
    else if ((Random() % 100) < 10)
    {
        switch (type1) // type 1 is used only
        {
            case TYPE_FIRE:     text = sText_TypeFire; break;
            case TYPE_WATER:    text = sText_TypeWater; break;
            case TYPE_ELECTRIC: text = sText_TypeElectric; break;
            case TYPE_GRASS:    text = sText_TypeGrass; break;
            case TYPE_ICE:      text = sText_TypeIce; break;
            case TYPE_FIGHTING: text = sText_TypeFighting; break;
            case TYPE_FLYING:   text = sText_TypeFlying; break;
            case TYPE_POISON:   text = sText_TypePoison; break;
            case TYPE_GROUND:   text = sText_TypeGround; break;
            case TYPE_ROCK:     text = sText_TypeRock; break;
            case TYPE_BUG:      text = sText_TypeBug; break;
            case TYPE_GHOST:    text = sText_TypeGhost; break;
            case TYPE_STEEL:    text = sText_TypeSteel; break;
            case TYPE_PSYCHIC:  text = sText_TypePsychic; break;
            case TYPE_DRAGON:   text = sText_TypeDragon; break;
            case TYPE_DARK:     text = sText_TypeDark; break;
            case TYPE_FAIRY:    text = sText_TypeFairy; break;
            case TYPE_NORMAL:
            default:            text = sText_TypeNormal; break;
        }
    }

    else if (((Random() % 100) < 10) && (metLocation != 0 && (Random() % 100) < 100))
    {
        GetMapName(gStringVar2, metLocation, 0);
        text = sText_RecallsMetLocation;
    }
    else if (((Random() % 100) < 10)) // 10% chance nature-based
    {
        u8 nature = GetNature(mon);
        switch (nature)
        {
            case NATURE_HARDY:   text = sText_NatureHardy; break;
            case NATURE_LONELY:  text = sText_NatureLonely; break;
            case NATURE_BRAVE:   text = sText_NatureBrave; break;
            case NATURE_ADAMANT: text = sText_NatureAdamant; break;
            case NATURE_NAUGHTY: text = sText_NatureNaughty; break;

            case NATURE_BOLD:    text = sText_NatureBold; break;
            case NATURE_DOCILE:  text = sText_NatureDocile; break;
            case NATURE_RELAXED: text = sText_NatureRelaxed; break;
            case NATURE_IMPISH:  text = sText_NatureImpish; break;
            case NATURE_LAX:     text = sText_NatureLax; break;

            case NATURE_TIMID:   text = sText_NatureTimid; break;
            case NATURE_HASTY:   text = sText_NatureHasty; break;
            case NATURE_SERIOUS: text = sText_NatureSerious; break;
            case NATURE_JOLLY:   text = sText_NatureJolly; break;
            case NATURE_NAIVE:   text = sText_NatureNaive; break;

            case NATURE_MODEST:  text = sText_NatureModest; break;
            case NATURE_MILD:    text = sText_NatureMild; break;
            case NATURE_QUIET:   text = sText_NatureQuiet; break;
            case NATURE_BASHFUL: text = sText_NatureBashful; break;
            case NATURE_RASH:    text = sText_NatureRash; break;

            case NATURE_CALM:    text = sText_NatureCalm; break;
            case NATURE_GENTLE:  text = sText_NatureGentle; break;
            case NATURE_SASSY:   text = sText_NatureSassy; break;
            case NATURE_CAREFUL: text = sText_NatureCareful; break;
            case NATURE_QUIRKY:  text = sText_NatureQuirky; break;
        }
    }
    else if ((Random() % 100) < 10 && heldItem == ITEM_LEFTOVERS)
        text = sText_HoldingFood;
    else if ((Random() % 100) < 10 && heldItem != ITEM_NONE)
        text = sText_ChecksHeldItem;
    else if ((Random() % 100) < 10 && ballType == ITEM_LUXURY_BALL)
        text = sText_LuxuryBallPride;
    // Egg Group based behavior (2% chance)
    else if ((Random() % 100) < 10)
    {
        u8 eggGroup = gBaseStats[species].eggGroup1 & 0xFF;

        switch (eggGroup)
        {
            case EGG_GROUP_HUMAN_LIKE:
                text = sText_TryingToStandUpright;
                break;
            case EGG_GROUP_MONSTER:
                text = sText_StompingAround;
                break;
            case EGG_GROUP_FAIRY:
                text = sText_SkippingAlong;
                break;
            default:
                break; // nothing
        }
    }

    // OT vs Player (1% chance)
    else if ((Random() % 100) < 10)
    {
        u32 otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
        if (otId != GetPlayerTrainerId())
        {
            text = sText_SeemsDistracted;
        }
    }

    // High IV flex (2% chance)
    else if ((Random() % 100) < 10)
    {
        u8 atkIV = GetMonData(mon, MON_DATA_ATK_IV, NULL);
        u8 defIV = GetMonData(mon, MON_DATA_DEF_IV, NULL);

        if (atkIV >= 25 || defIV >= 25)
        {
            text = sText_FlexesMuscles;
        }
    }

    if (text == NULL)
    {
        u8 rand = Random() % 10;
        if (rand < 3)
            text = sText_IdleLookingAround;
        else if ((Random() % 100) < 10)
        {
            if (IsDayTime())
                text = sText_EnjoyingSunlight;
            else if (IsNightTime())
                text = sText_NighttimeEnergy;
            else if (IsMorning())
                text = sText_MorningGreeting;
        }
        else if (rand < 7)
            text = sText_IdleBored;
        else
            text = sText_IdleTailWag;
    }

    ShowMysteryGiftMon();
    ShowFieldMessage(text);
}