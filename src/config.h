#pragma once

/**
 * \file config.h
 * \brief Handles all customizable options.
 */

/*===== Debugging Options =====*/
//#define DEBUG_QUICK_BATTLES //Battles will end after choosing the first attack
//#define DEBUG_MEGA //Mega Evolution can always be used
//#define DEBUG_HMS //HMs can always be used from the party screen, Surf, Waterfall, and Rock Climb can always be used
//#define DEBUG_OBEDIENCE //Traded Pokemon never have obedience issues
//#define DEBUG_DYNAMAX //Dynamax can be used in Dynamax battles without a Dynamax Band
//#define DEBUG_AI_CHOICES //Removes the frame-based randomness of the AI's decisions

/*===== General Vars =====*/
#define VAR_TERRAIN 0x5000 //Set to a terrain type for a battle to begin with the given terrain
#define VAR_TOTEM 0x5001 //to var + 3 (0x5004)
#define VAR_BACKSPRITE_SWITCH 0x5006 //This var can be set to a number to change the Player's backsprite
//#define VAR_BATTLE_BG 0x5007 //Set this var to a custom background id
#define VAR_SWARM_INDEX 0x5008 //Set by the engine
#define VAR_SWARM_DAILY_EVENT 0x5009 //Set by the engine. Used to tell the game if a swarm has already been generated for the day. (Uses + 1 var also)
#define VAR_DEFAULT_WALKING_SCRIPT 0x500B  //Walking scripts from JPAN's engine. His engine used 0x407E.
#define VAR_DEXNAV 0x500C //Set by the engine indicating which Pokemon has been registered to find in the OW
#define VAR_STATUS_INDUCER 0x500D //Causes wild Pokemon to be spawned with the given status condition. Status + 0x100 clears var after battle.
#define VAR_RAID_PARTNER_RANDOM_NUM 0x500E //A random number set daily to help give more random partners in raids

#define VAR_SECOND_OPPONENT 0x5010 //Set this to the var used to store the Trainer Id of the second opponent in Multi Battles (can be any free var)
#define VAR_PARTNER 0x5011 //Set this to the var used to store the Trainer Id of your partner in Multi Battles (can be any free var)
#define VAR_PARTNER_BACKSPRITE 0x5012 //Set this to the var used to store the backsprite number of your partner in Multi Battles (can be any free var)

/*===== General Flags =====*/
#define FLAG_INVERSE 0x900 //Set this flag for all battles to become Inverse Battles
#define FLAG_SKY_BATTLE 0x901 //Set this flag to indicate the Sky Battle rules should be followed in the next battle.
#define FLAG_NO_CATCHING 0x902 //The player cannot catch Pokemon in battles when this flag is set
#define FLAG_NO_RUNNING 0x903 //The player cannot run in battles when this flag is set
#define FLAG_NO_CATCHING_AND_RUNNING 0x904 //The player cannot run or catch Pokemon in battles when this flag is set
#define FLAG_CATCH_TRAINERS_POKEMON 0x905 //Setting this flag will allow you to capture trainers' pokemon. Comment this line to remove this feature
#define FLAG_EXP_SHARE 0x906 //Used to indicate if the Gen 6+ Exp Share is enabled
#define FLAG_DOUBLE_BATTLE 0x907 //Setting this flag will automatically trigger a double battle if possible
#define FLAG_TAG_BATTLE 0x908 //This flag is set by the engine when trainerbattle 0x10 is used to activate a tag battle
#define FLAG_TWO_OPPONENTS 0x909 //This flag is set by the engine when trainerbattle 0x11 is used to activate a dual opponent battle
#define FLAG_ACTIVATE_TUTORIAL 0x90A //Setting this flag will allow you to activate Oak's tutorial in any kind of battle (including doubles). Comment this line to remove this feature
#define FLAG_WILD_CUSTOM_MOVES 0x90B //Setting this flag lets you assign wild Pokemon custom moves
#define FLAG_SMART_WILD 0x90C //Set this flag is you don't want wild Pokemon to act randomly (useful for one time smarter wild battles). This is cleared at the end of the battle.
#define FLAG_SCALE_WILD_POKEMON_LEVELS 0x90D //If this flag is set, all random wild Pokemon (other than scripted ones) will have their level scaled to the lowest level in the player's party
#define FLAG_SCALE_TRAINER_LEVELS 0x90E //If this flag is set, all Trainer Pokemon levels will be set to the highest in your party
#define FLAG_HIDDEN_ABILITY 0x90F //If this flag is set, generated wild Pokemon will have their hidden abilities
#define FLAG_GIGANTAMAXABLE 0x928 //If this flag is set, generated wild and gift Pokemon will have their Gigantamax bit set
#define FLAG_DOUBLE_WILD_BATTLE 0x910 //If this flag is set, a wild battles will be against two Pokemon
#define FLAG_NO_RANDOM_WILD_ENCOUNTERS 0x911 //If this is set, wild Pokemon won't appear when walking/surfing in grass, caves, water, etc.
#define FLAG_REMOVE_EVO_ITEM 0x912  //Flag to toggle item removal after leveling up (set by the engine)
#define FLAG_SHINY_CREATION 0x913 //Flag to cause next battle to be against a shiny Pokemon
#define FLAG_AUTO_RUN 0x914 //Flag to enable auto-run (toggled by L-button). If this feature is used, L=A will not work. Comment out this line for it to work again.
#define FLAG_RUNNING_ENABLED 0x82F //The player can only run if this flag is set. If commented, the player will start the game with running shoes.
#define FLAG_BIKE_TURBO_BOOST 0x91F //The bike moves extra fast if this flag is set or if the player holds B.
#define FLAG_SURF_TURBO_BOOST 0x929 //The Surf blob moves extra fast if set
#define FLAG_DISABLE_BAG 0x915 //The bag can't be used in-battle if this flag is set
#define FLAG_MOVE_RELEARNER_IGNORE_LEVEL 0x916 //When set, the move relearner shows all level-up moves up to MAX_LEVEL
#define FLAG_EGG_MOVE_RELEARNER 0x917 //When set, the move learner loads egg moves instead
#define FLAG_DYNAMAX_BATTLE 0x918 //When set, Pokemon can Dynamax in battle
#define FLAG_RAID_BATTLE 0x919 //When set, wild Pokemon will appear Dynamaxed and be fought Raid Battle style.
#define FLAG_RAID_BATTLE_NO_FORCE_END 0x91A //When set, if FLAG_RAID_BATTLE is set, then raid battles won't be force ended.
#define FLAG_FAST_BATTLE_MESSAGES 0x925 //When set, battle messages have no wait time once they've completed printing
#define FLAG_FOLLOWER_IGNORE_ACTIVE_SCRIPT 0x926 //If set, the follower will reappear and move even when a script is in progress
#define FLAG_KEEP_CONSUMABLE_ITEMS 0x927 //If set, consumable items are returned after battles
#define FLAG_UNLOCKED_DEXNAV_HELD_ITEMS 0x92A //If set, possible held items will be displayed for wild Pokemon on the DexNav UI
#define FLAG_DAILY_EVENTS_START 0xE00 //To flag + 0xFF, resets every new day.

/*===== Start Menu/Poketools Flags =====*/
#define FLAG_SYS_BAG_HIDE 0x91B		//Toggle bag off
#define FLAG_SYS_PLAYER_HIDE 0x91C	//Toggle [player] off
#define FLAG_SYS_SAVE_HIDE 0x91D	//Toggle save off
#define FLAG_SYS_DEXNAV 0x91E		//Flag to turn dexnav on/off in start menu (if undefined, this will eliminate dexnav from the start menu)

/*===== Pedometer Flags as in JPAN Engine =====*/
#define FLAG_LONG_PEDOMETER 0x920		//32 bit / 4 bytes / Max 4 294 967 295 steps
#define FLAG_MED_PEDOMETER 0x921		//16 bit / 2 bytes / Max 65 535 steps
#define FLAG_SMALL_PEDOMETER_1 0x922	//8 bit  / 1 byte  / Max 255 steps
#define FLAG_SMALL_PEDOMETER_2 0x923	//8 bit  / 1 byte  / Max 255 steps

/*===== Battle Tower Options =====*/
#define FLAG_BATTLE_FACILITY 0x930
#define VAR_BATTLE_FACILITY_POKE_NUM 0x5015 //Var
#define VAR_BATTLE_FACILITY_POKE_LEVEL 0x5016 //Var
#define VAR_BATTLE_FACILITY_BATTLE_TYPE 0x5017 //Var
#define VAR_BATTLE_FACILITY_TIER 0x5018 //Var
#define VAR_BATTLE_FACILITY_TRAINER1_NAME 0x5019 //Empty var. Will be set to 0xFFFF after every battle.
#define VAR_BATTLE_FACILITY_TRAINER2_NAME 0x501A //Empty var. Will be set to 0xFFFF after every battle.
#define VAR_BATTLE_FACILITY_SONG_OVERRIDE 0x501B //Set this var to the song id to be played during Link Battles and in the Battle Tower.

enum //These vars need to be one after the other (hence the enum)
{
	VAR_FACILITY_TRAINER_ID = 0x501C, 			//An index in the gTowerTrainers table, not Trainer ID
	VAR_FACILITY_TRAINER_ID_2,	//0x501D		//Index of the second trainer for Multi Battlers in the gTowerTrainers table, the var should be 1 after the first one
	VAR_FACILITY_TRAINER_ID_PARTNER, //0x501E	//If your partner is randomized, its Id would be found in this var
};

/*===== Character Customization Vars =====*/
#define VAR_PLAYER_WALKRUN 0x501F			//Change walking, running player sprite. 0x4054 in JPAN engine.
#define VAR_PLAYER_BIKING 0x5020			//Change biking player sprite. 0x4055 in JPAN engine.
#define VAR_PLAYER_SURFING 0x5021			//Change player surfing sprite. 0x4056 in JPAN engine.
#define VAR_PLAYER_HM_USE 0x5022			//Change field move usage sprite. 0x4057 in JPAN engine.
#define VAR_PLAYER_FISHING 0x5023			//Change fishing sprite. 0x4058 in JPAN engine.
#define VAR_PLAYER_VS_SEEKER 0x503D         //Change vs seeker usage sprite. NOT in JPAN's engine
#define VAR_PLAYER_VS_SEEKER_ON_BIKE 0x5024	//Change vs seeker on bike sprite. 0x4059 in JPAN engine.
#define VAR_PLAYER_UNDERWATER 0x5025		//Change underwater sprite.
#define VAR_TRAINERCARD_MALE 0x5026			//Change trainer card image (male). 0x4060 in JPAN engine.
#define VAR_TRAINERCARD_FEMALE 0x5027		//Change trainer card image (female). 0x4061 in JPAN engine.

#define VAR_RUNTIME_CHANGEABLE 0x5028	//'Secret Base' variables, save 15 consecutive variables for this. 0x4080 in JPAN engine.

/*===== Healing Place as in JPAN's Engine =====*/
#define VAR_HEALINGMAP 0x5037 //0x405A in JPAN's Engine
#define VAR_HEALING_XPOS 0x5038 //0x405B in JPAN's Engine
#define VAR_HEALING_YPOS 0x5039 //0x405C in JPAN's Engine

/*===== TM/HM/Tutor Options (Vastly improved from other versions!) =====*/
//#define EXPANDED_TMSHMS  //Comment this out if you want to keep 50 tms/8 hms
//#define EXPANDED_MOVE_TUTORS //Comment this out if you want to keep 16 move tutors and the ultimate elemental moves being exclusive to Kantonian starters
#define NUM_TMS 50	//keep this defined even if EXPANDED_TMSHMS is not!!
#define NUM_HMS 8	//keep this defined even if EXPANDED_TMSHMS is not!!
#define NUM_MOVE_TUTORS 16 //keep this defined even if EXPANDED_MOVE_TUTORS is not!! If using DPE, set to 128.
#define LAST_TOTAL_TUTOR_NUM 24 //Should be equal to (NUM_MOVE_TUTORS - 1) + 9. Must be set to an actual integer or the compilation will not work.
//#define TMS_BEFORE_HMS  //Uncomment this if you want the HMs to appear after the TMs in your bag
//#define DELETABLE_HMS //Uncomment this if you want HMs to be deletable without the Move Deleter
//#define REUSABLE_TMS	//if defined, don't forget to give all TMs a Mystery byte of 1!

/*===== Time of Day Options =====*/
#define TIME_MORNING_START 4		//4:00 AM / 4:00
#define TIME_DAY_START 8			//8:00 AM / 8:00
#define TIME_EVENING_START 17		//5:00 PM / 17:00
#define TIME_NIGHT_START 20			//8:00 PM / 20:00

/*===== General Number Options =====*/
#define KANTO_DEX_COUNT 151
#define NATIONAL_DEX_COUNT 386

#define MAX_LEVEL 100 //Also change this in the file "asm_defines.asm" found in the root
#define NUM_TRAINER_CLASSES 107 //Vanilla FR has 107
#define EVOS_PER_MON 5 //The number of maximum evolutions per Pokemon. Vanilla FR has 5. DPE has 16!!!
#define EV_CAP 252 //Also change this in the file "asm_defines.s" found in the root
#define POWER_ITEM_EV_YIELD 8 //Set to 4 for older mechanic

#define DUSK_BALL_MULTIPLIER 30 //Change this line to 35 to make the catch rate for the Dusk Ball 3.5x like before Gen 7
#define STANDARD_IV 10 //Change this to be the number of IVs Pokemon owned by random trainers should have (all stats will have this number)
#define SWARM_CHANCE 50 //Change this to the percentage that swarming Pokemon will appear if they can be found on the current route.
#define WILD_DOUBLE_RANDOM_CHANCE 50 //Change this to the percentage that a wild double battle will be initiated if the player is in special grass.
#define CREATE_WITH_X_PERFECT_IVS 3 //Change this to the number of set 31 IVs you'd like Pokemon defined in gSetPerfectXIvList to have.
#define CREATE_ROAMER_WITH_X_PERFECT_IVS 3 //Change this to the number of set 31 IVs you'd like roaming pokemon to have.
#define EGG_HATCH_LEVEL 1 //The level eggs hatch at
#define AI_TRY_TO_KILL_RATE 50 //In battles against a trainer with AI flags of 1, the AI will try to use a move to knock out the opponents XX percent of the time
#define BIKE_ENCOUNTER_PERCENT 60 //Biking through grass or in caves only has this percentage of the encounter rate (vanilla FR is 80)

/*===== Metatile Behaviours =====*/
#define MB_OMNIDIRECTIONAL_JUMP 0x7F //The tile behaviour byte that can be jumped over in all directions
#define MB_LOCKED_DOOR 0xA4 //The tile behaviour byte for doors that are locked and cannot be opened
#define MB_ROCK_CLIMB_WALL 0xA5 //The tile behaviour byte that can be climbed up using Rock Climb
#define MB_HEADBUTT_TREE 0xA6 //The tile behaviour byte for starting a wild encounter by headbutting a tree

#define MAP_PLAYER_HOME ((4 << 8) | 0) //The map bank and map number of the player's home

/*===== Expanded Coins Options =====*/
#define MAX_COINS_DIGITS 9	//Max number of digits for game corner coins (eg. 4 = 9999, 9 = 999,999,999). maximum of 9

/*===== Badge Obedience Options =====*/
#define BASE_OBEDIENCE_LEVEL	10
#define BADGE_1_OBEDIENCE_LEVEL	10
#define BADGE_2_OBEDIENCE_LEVEL 30
#define BADGE_3_OBEDIENCE_LEVEL 30
#define BADGE_4_OBEDIENCE_LEVEL 50
#define BADGE_5_OBEDIENCE_LEVEL 50
#define BADGE_6_OBEDIENCE_LEVEL 70
#define BADGE_7_OBEDIENCE_LEVEL 70

/*===== OW Palette Options =====*/
#define TREE_DISGUISE_PAL_ID 0x1105 //The NPC palette id of the tree disguise movement permission.
#define ROCK_DISGUISE_PAL_ID 0x1104 //The NPC palette id of the mountain disguise movement permission.
#define WEIRD_DISGUISE_PAL_ID 0x1103 //The NPC palette id of the weird disguise movement permission.

/*===== Pre-Battle Mugshot Options ====*/
#define FR_PRE_BATTLE_MUGSHOT_STYLE //The FR Elite Four and Champion use their GF defined mugshots
#define VAR_PRE_BATTLE_MUGSHOT_STYLE 0x503A //Used for changing between big, two bars, etc.
#define VAR_PRE_BATTLE_MUGSHOT_SPRITE 0x503B //Used for determining if the Vs. sprite should show
#define VAR_MUGSHOT_PLAYER_PAL 0x503C //Used to change the player's tilemap palette in the Two Bar mugshot style
#define FLAG_LOAD_MUGSHOT_SPRITE_FROM_TABLE 0x924 //Load custom mugshot for pre-battle transition

/*=====Safari Zone Options=====*/
#define EXPAND_SAFARI_BALLS	//Hold up to 0xFFFF safari balls
#define SAFARI_ZONE_MAX_STEPS 600	//Number of safari zone steps the player has
#define SAFARI_ZONE_BALL_START 30	//Number of safari balls the player starts with
#define MAX_SAFARI_BALLS 30		//Total safari balls player can get (maximum is 0xFFFF or 0xFF if EXPAND_SAFARI_BALLS is defined or not)

/*=====Randomizer Options=====*/
//#define FLAG_POKEMON_RANDOMIZER 0x940 //Setting randomizes Pokemon species that are created.
//#define FLAG_POKEMON_LEARNSET_RANDOMIZER 0x941 //Setting randomizes Pokemon learnsets.
//#define FLAG_ABILITY_RANDOMIZER 0x942 //Setting randomizes Pokemon abilities.
#define NUM_SPECIES_RANDOMIZER NUM_SPECIES_GEN_7 //The final number of Pokemon (+ 1) of species that can appear in the randomizer. Change to NUM_SPECIES for Gen 8

/*===== Pre-existing Offsets =====*/
//#define EXISTING_FOSSIL_IMAGE_TABLE_ADDRESS 0x81a4600 //Uncomment this if you've already inserted a fossil image table
//#define EXISTING_OW_TABLE_ADDRESS 0x81a2000 //Uncomment if you don't want new overworld NPC tables to be generated. DON'T TOUCH IF YOU DON'T KNOW EXACTLY WHAT THIS IS FOR!

/*===== Other General Options =====*/
#define TIME_ENABLED //Comment this line to disable time based features. All time dependent features will default in Daytime.
#define DNS_IN_BATTLE //Comment this line to disable the Day/Night system from working in battle
#define OVERWRITE_RIVAL //Comment this line to disable to loading of your rival's name into trainer classes 0x51, 0x59, and 0x5A
#define TRAINER_CLASS_POKE_BALLS //Comment this line to disable creating Trainer's pokemon with Poke Balls specific to their trainer class
//#define TRAINERS_WITH_EVS //Uncomment this line to enable the Trainers with EVs hack, (only avaible with custom moveset and held item)
//#define CONTINUE_LOST_BATTLES //Uncommenting this line will allow using trainerbattle 0x9 to continue lost battles if var 0x8000 is set to 0xFEFE
#define DISPLAY_REAL_MOVE_TYPE_ON_MENU //If this line remains uncommented, the "true" move type will be displayed (such as Hidden Power, Weather Ball in Weather, etc.)
#define DISPLAY_REAL_ACCURACY_ON_MENU //If this line remains uncommented, the "true" move accuracy will be displayed (Eg. The move Psychic on a Pokemon with Compound Eyes will have its accuracy appear as 130)
#define DISPLAY_REAL_POWER_ON_MENU //If this line remains uncommented, the "true" move power will be displayed (Eg. Moves like Fury Cutter will show their correct power)
#define DISPLAY_EFFECTIVENESS_ON_MENU //If this line remains uncommented, move types will have their effectiveness indicated on the move menu by highlighting the move type
#define OVERWRITE_BG_FOR_LEADER_CHAMPION //If this remains uncommented, special Battle Background palettes will be loaded in for battles against Gym Leaders and the Champion
#define BRIDGE_FIX //If this remains uncommented, the water battle background will only be loaded if the player's surfing sprite is shown
#define MEGA_EVOLUTION_FEATURE //Comment this line to remove Mega Evolutions
#define DYNAMAX_FEATURE //Comment this line to remove Dynamax and Raid Battles
#define TUTORIAL_BATTLES //Comment this line to remove Oak's Tutorial
#define TANOBY_RUINS_ENABLED //The Tanoby Ruins will be the only location Unown can be encountered
#define ALTERING_CAVE_ENABLED //The Altering Cave from FR will work as intended provided the proper var is set
#define SWEET_SCENT_ONLY_IN_CLEAR_WEATHER //Sweet Scent won't attract any wild Pokemon if the weather is not clear
#define SWEET_SCENT_WILD_DOUBLE_BATTLES //Sweet Scent can be used (with respect to WILD_DOUBLE_RANDOM_CHANCE) to encounter two wild Pokemon at once
#define OBEDIENCE_BY_BADGE_AMOUNT //Determines obedience based on the number of badges the Player has, rather than which badges the player has
#define SAVE_BLOCK_EXPANSION //Commenting this requires you to also manually remove Save Expansion Hooks found in hooks. It will also break several features. DO NOT COMMENT OUT!
#define SELECT_FROM_PC //Comment this out to remove select-from-pc hack
//#define SET_HEALING_PLACE_HACK  //Uncomment this if you want custom map/bank whiteout respawn locations
//#define FOSSIL_IMAGE_HACK   //Uncommenting includes JPANs fossil image hack (see EXISTING_FOSSIL_IMAGE_TABLE_ADDRESS)
#define EVO_HOLD_ITEM_REMOVAL //Comment this out if you want leveling up/hold item evolution (eg. sneasel) to remove the item (like normal)
#define EXPAND_MOVESETS //Comment this out if you're using the Dynamic Pokemon Expansion repo to expand the movesets
//#define FATHER_PASSES_TMS //Uncomment this out if you want TMs the father knows to be passed through breeding
//#define INHERIT_MASTER_CHERISH_BALL  //Uncomment this if you want Master and Cherish balls to be inherited by daycare offspring
//#define GIVEPOKEMON_CUSTOM_HACK //Alows custom Pokemon to be given by setting the second last byte of the givepokemon scripting command
//#define GIVEPOKEMON_BALL_HACK //Allows Pokemon to be given with a custom ball by setting the last byte of the givepokemon scripting command
#define FRLG_ROAMING //When a roaming Pokemon is created, it will either be a Entei, Raikou, or Suicune, depending on the player's starter choice
#define CAN_RUN_IN_BUILDINGS //Comment this line out to prevent the player from running indoors.
//#define NO_POISON_IN_OW //Uncommenting this line will stop Pokemon from taking Poison damage in the overworld.
#define POISON_1_HP_SURVIVAL //Comment this line to allow Pokemon to faint from Poison in the overworld.
#define BW_REPEL_SYSTEM  //Keep this uncommented if you want the game to ask the user to re-use another repel when it runs out
#define AUTO_NAMING_SCREEN_SWAP  //Comment out if you don't want naming screens to auto-swap to lower-case after first upper-case letter
#define MULTIPLE_PREMIER_BALLS_AT_ONCE //Comment out if you don't want the player received a Premier ball for every 10 Poke Balls purchased (only 1 no matter how many over 10 balls the player buys)
#define NON_TRAINER_SPOTTING //Comment out if you don't want regular NPCs to be able to spot the player in the oveworld (vanilla only trainers can or the game will crash)
//#define BIKE_ON_ANY_NON_INSIDE_MAP //Uncomment if you still want to be able to bike on any non-inside map
//#define GEN_4_PLAYER_RUNNING_FIX //Uncomment to increase the lag between frames as the player OW runs, to simulate a more accurate Gen 4 running effect
//#define SHRINK_PLAYER_THROUGH_DOOR //The player sprite will shrink slightly when going through a doorway
#define EXPAND_MOVE_REMINDER_DESCRIPTION //Keep this defined to have 5 lines for the move description as opposed to 4 in the move reminder menu
//#define ITEM_PICTURE_ACQUIRE //If uncommented, item images will be shown when the player finds or obtains an item. Breaks FR Game Corner prize room
//#define ITEM_DESCRIPTION_ACQUIRE //If uncommented, item descriptions will be shown the first time an item is added to the Bag. ITEM_PICTURE_ACQUIRE must be defined to work
#define EXPANDED_TEXT_BUFFERS //Expands the number of scripting string buffers available. Comment out to keep original buffer text
#define FOOTSTEP_NOISES //Sounds will be played when any object moves through grass or across sand
#define CAN_ONLY_USE_OMNIDRECTIONAL_JUMP_ON_HEIGHT_2 //The omnidirectional jump tiles can only be used if the player is standing on a tile of height 2 (movement permission 0xC in AdvanceMap)
#define HOOPA_CHANGE_IN_PC //Hoopa-Unbound will revert to confined form when placed in or withdrawn from a box
#define SHAYMIN_CHANGE_IN_PC //Shaymin-Sky will revert to sky from when placed in or withdrawn from a box
#define HIGH_PITCH_MEGA_PRIMAL_CRY //Mega Pokemon cries will be higher pitched. This is useful if you're using the base form cries for Mega Evolutions
//#define CUSTOM_MULTICHOICE //Adds JPAN's custom multichoice menus (see documentation)
//#define SCROLLING_MULTICHOICE //Enables scrolling multichoice menus by using special 0x158.
//#define REPLACE_SOME_VANILLA_SPECIALS //Replaces the sp07C, sp07D, sp09E, sp156, sp18B, & the Coins scripting commands with modified versions. Breaks vanilla FR compatability.
#define REPLACE_ASH_WEATHER_WITH_WHITE_SANDSTORM //Replaces the falling ash weather effect with a white version of the sandstorm weather effect
//#define ONLY_CHECK_ITEM_FOR_HM_USAGE //Allows HMs to be used if the HM is the Bag, and as long as there is a Pokemon in the party that can learn the HM
//#define FADE_NPCS_IN_FOG //Blends the NPC palettes in foggy weather to create the illusion that they're under the fog. Breaks FR Pokemon Tower healing zone
#define FRIENDSHIP_HEART_ON_SUMMARY_SCREEN //Creates a heart sprite on the summary screen if the current Pokemon has max Friendship (position requires adjusting)
//#define INSTANT_TEXT // Enable Instant Text. Some Hack Authors might want this disabled because of the effects instant text has on pacing
#define AUTOSCROLL_TEXT_BY_HOLDING_R //Text will scroll automatically by holding R.
#define DEXNAV_POKEMON_MOVE_IN_CAVES_WATER //DexNav phenomenon move around a couple times on water and in caves before the battle starts (like in ORAS)
#define SYNCHRONIZE_GIFT_POKEMON //Gift Pokemon can have their natures manipulated with synchronize

/*===== Misc Battle Effect Options =====*/
//#define OLD_BURN_DAMAGE //Uncomment this line if you want burn damage to do 1/8 of max health instead of 1/16
//#define OLD_PARALYSIS_SPD_DROP //Uncomment this line if you want paralysis to lower Spd down to 1/4 instead of 1/2
//#define OLD_CONFUSION_CHANCE //Uncomment this line if you want the chance that confusion will stop your attack to 50% instead of 33%
//#define INFINITE_WEATHER //Uncomment this line if you want weather abilities to last for infinite turns
//#define INFINITE_TERRAIN //Uncomment this line if you want terrain abilities to last for infinite turns
//#define NO_SHEER_COLD_NERF //Uncomment this line to remove all Gen 7 Sheer Cold nerfs
//#define OLD_MOVE_SPLIT //Uncomment this line to use the Physical/Special split based on move types. Status moves are still set with the split byte.
//#define OLD_CONFUSION_HEAL_BERRIES //Uncomment this line for berries like Figy and Wiki Berry to restore only 1/8 max HP when HP is below 1/2
//#define GEN_7_CONFUSION_HEAL_BERRIES //Uncomment this line for berries like Figy and Wiki Berry to restore 1/2 max HP (Gen 8 is 1/3) when HP is below 1/4
//#define PLA_HELD_ORIGIN_ORBS //Dialga and Palkia change into their Origin forms when they hold their respective orbs

/*===== Ability Options =====*/
//#define OLD_GALE_WINGS //Uncomment this line if you want Gale Wings to activate regardless of the user's HP
//#define OLD_PRANKSTER //Uncomment this line if you want Prankster to be able to affect Dark-Types

/*===== Damage Calculation Options =====*/
//#define OLD_CRIT_DAMAGE //Uncomment this line if you want Critical hits to do 2x damage, and 3x with Sniper
//#define CRIT_CHANCE_GEN_6 //Uncomment this line if you want the Gen 6 crit chance
//#define CRIT_CHANCE_GEN_2_TO_5 //Uncomment this line if you want the Gens 2-5 crit chance
//Uncommenting both of the above two lines will default in the Gen 6 crit chance
//#define BADGE_BOOSTS //Uncomment this line to allow Badges to give stat boosts
//#define OLD_ATE_BOOST //Uncomment this line to make 'ate' abilities give a 1.3 boost instead of 1.2
//#define OLD_GEM_BOOST //Uncomment this line to make Gems give a 1.5 boost instead of 1.3
//#define OLD_TERRAIN_BOOST //Uncomment this line to make Terrains give a 1.5 boost instead of 1.3
//#define OLD_EXPLOSION_BOOST //Uncomment this line to make Exploding moves halve the target's defense
//#define OLD_HIDDEN_POWER_BP //Uncomment this line for Hidden Power to have its Base Power calculated from the Pokemon's IVs
//#define PORTAL_POWER //Uncomment this line to enable Hoopa-Unbound's special ability in Pokemon Unbound, Portal Power (reduces power of non-contact moves by 25%)
//#define OLD_SOUL_DEW_EFFECT //Uncomment this line if you want the Soul Dew to double Latios + Latias' Sp. Atk & Sp. Def
//#define OLD_PARENTAL_BOND_DAMAGE //Uncomment this line to make the second hit of Parental Bond do 50% of the original damage instead of 25%
#define GEN_6_POWER_NERFS //Comment out for moves that had powers nerfed in Gen 6 to retain their original base powers.
#define GEN_7_POWER_NERFS //Comment out for Sucker Punch to retain its original base power.
#define BUFFED_LEECH_LIFE //Comment out for Leech Life to retain its original base power.
#define DARK_VOID_ACC_NERF //Comment out for Dark Void to retain its original accuracy.

/*===== Capturing Pokemon Options =====*/
//#define NO_HARDER_WILD_DOUBLES //In Gen 5, Pokemon encountered in double wild battles were harder to catch (based on how many species are owned). Uncomment this line to remove the catch rate decrement.
#define CRITICAL_CAPTURE //Comment this line to remove the Critical Capture feature
#define SWSH_CATCHING_DIFFICULTY_MODIFIER //Trying to catch a Pokemon that's higher level than your current Pokemon makes it harder to catch

/*===== Exp Gain Options =====*/
//#define TRAINER_EXP_BOOST // Uncomment this line to give the Exp boost for battling a Trainer's Pokemon (Pre Gen 7)
//#define OLD_EXP_SPLIT //Uncomment this line to split the Exp amongst all participating pokemon (Pre Gen 6)
//#define FLAT_EXP_FORMULA //Uncomment this line to use a Flat Exp calculation formula (Gens 2 - 4, 6)
#define GEN_7_BASE_EXP_YIELD //Base Exp Yield is read from gBaseExpBySpecies to use larger values that match Gen 7
#define CAPTURE_EXPERIENCE //Experience is awared upon capturing Pokemon.
#define EXP_AFFECTION_BOOST //Pokemon with friendship >= 220 gain boosted experience

/*===== Other Battle Options =====*/
//#define NO_GHOST_BATTLES //Uncomment this line to disable the Ghost battle feature from Pokemon Tower in Lavender town
//#define GEN4_PLUS_SELECTION_SCREEN //Uncommenting this line does not give you the Gen 4+ selection screen, it only adds features that supports it
//#define OBEDIENCE_CHECK_FOR_PLAYER_ORIGINAL_POKEMON //Uncommenting line line will open up the possibility that the Player's Pokemon can disobey them (not just traded mons)
//#define WILD_ALWAYS_SMART //Uncomment this line if you want all Wild Pokemon to act smartly
//#define HAIL_IN_BATTLE //Uncommenting this line enables the Hail weather effect in battle when the OW weather is set to WEATHER_STEADY_SNOW (0x7)
//#define FOG_IN_BATTLE //Uncommenting this line enables the Fog weather effect in battle. Don't uncomment this line without uncommenting one of the lines below!
//#define FOG_IN_BATTLE_1 //Uncommenting this line enables the Fog weather effect when the OW weather is set to WEATHER_FOG_1 (0x6)
//#define FOG_IN_BATTLE_2 //Uncommenting this line enables the Fog weather effect when the OW weather is set to WEATHER_FOG_2 (0x9)
//#define FOG_IN_BATTLE_3 //Uncommenting this line enables the Fog weather effect when the OW weather is set to WEATHER_FOG_3 (0xA)
#define HIDE_HEALTHBOXES_DURING_ANIMS //Commenting this line prevents the health boxes from being hidden during move animations and some special animations.
//#define DONT_HIDE_HEALTHBOXES_ATTACKER_STATUS_MOVES //Uncommenting this line doesn't hide the healthboxes when the attacker is using a status move that targets itself (Gen 4).
#define FASTER_HEALTHBOX_CHANGE //The healthbar increases/decreases at a somewhat constant rate for all Pokemon
//#define ENCOUNTER_MUSIC_BY_CLASS //Plays music when a trainer spots the player based on the trainer class rather than the value set in the trainer data.
#define OKAY_WITH_AI_SUICIDE //The AI is allowed to use self-destructing moves
//#define HEALTHBAR_TYPE_ICONS //Pokemon types will always be shown next to the healthbar
#define LAST_USED_BALL_TRIGGER //The player will be given an option to use their last used Poke Ball in wild battles by pressing L
//#define PICK_UP_KNOCKED_OFF_ITEMS //Items knocked off wild Pokemon will be picked up at the end of battle
//#define PICKUP_ITEMS_STRAIGHT_TO_BAG //Items obtained with Pickup or Honey Gather are placed right in the bag
//#define TAKE_WILD_MON_ITEM_ON_CAPTURE //Items held by Wild Pokemon when caught can be placed in the bag at the player's request
//#define PAYDAY_MONEY_CAP //Pay Day and G-Max Gold Rush should cap at $99999 per battle
//#define QUICK_CLAW_PROCING_ALWAYS_ALLOWS_FLEEING //Fleeing from wild battles will never fail the turn a Quick Claw activates
//#define FROSTBITE //Replaces the Freeze status condition with the Frostbite status condition from PLA

/* DexNav Options */
//See "include/new/dexnav_config.h"
