#pragma once

#define DEBUG_MEGA
#define DEBUG_UNBOUND_MUSIC

#define UNBOUND //Don't uncomment this line; it's for compiling specific aspects of Pokemon Unbound

#define TERRAIN_VAR 0x50CA
#define TOTEM_VAR 0x50CB //UPDATE HOW THIS WORKS
#define NPC_FOLLOWING_VAR 0x5000 //This var should should be set to the NPC Id of the NPC following the player. If no NPC is following the player, it should be set to 0.
#define OW_SPRITE_SWITCH_VAR 0x4054
#define BACKSPRITE_SWITCH_VAR 0x4062 //This var can be set to a number to change the Player's backsprite
#define BATTLE_BG_VAR 0x4039 //Set this var to a custom background id
#define SWARM_SPECIES_VAR 0x5030
#define SWARM_MAP_NAME_VAR 0x5031

#define INVERSE_FLAG 0x23D
#define SKY_BATTLE_FLAG 0x9AF
#define NO_CATCHING_FLAG 0x9F8
#define NO_RUNNING_FLAG 0x9F7
#define NO_CATCHING_AND_RUNNING_FLAG 0x8E2
#define CATCH_TRAINERS_POKEMON_FLAG 0x0 //If CATCH_TRAINERS_POKEMON is uncommented, then setting this flag will allow you to capture trainers' pokemon
#define EXP_SHARE_FLAG 0x9F2
#define DOUBLE_BATTLE_FLAG 0x0 //ACTIVATE_DOUBLE_BATTLE_FROM_FLAG is enabled, setting this flag will automatically trigger a double battle if possible
#define TAG_BATTLE_FLAG 0x9F4 //This flag is set by the engine when trainerbattle 0x10 is used to activate a tag battle
#define TWO_OPPONENT_FLAG 0x9F3 //This flag is set by the engine when trainerbattle 0x11 is used to activate a dual opponent battle
#define ACTIVATE_TUTORIAL_FLAG 0x9F5 //Setting this flag will allow you to activate Oak's tutorial in any kind of battle (including doubles)
#define WILD_CUSTOM_MOVES_FLAG 0x26B
#define SMART_WILD_FLAG 0x9F6 //Set this flag is you don't want wild Pokemon to act randomly (useful for one time smarter wild battles). This is cleared at the end of the battle.
#define SCALE_TRAINER_LEVELS_FLAG 0x0 //If this flag is set, all Trainer Pokemon levels will be set to the highest in your party
#define HIDDEN_ABILITY_FLAG 0x943 //If this flag is set, generated wild Pokemon will have their hidden abilities
#define DOUBLE_WILD_BATTLE_FLAG 0x9F9 //If this flag is set, a wild battles will be against two Pokemon

#define SECOND_OPPONENT_VAR 0x50BE //Set this to the var used to store the Trainer Id of the second opponent in Multi Battles (can be any free var)
#define PARTNER_VAR 0x50BF //Set this to the var used to store the Trainer Id of your partner in Multi Battles (can be any free var)
#define PARTNER_BACKSPRITE_VAR 0x50C0 //Set this to the var used to store the backsprite number of your partner in Multi Battles (can be any free var)

#define BATTLE_TOWER_FLAG 0x9F0
#define BATTLE_TOWER_POKE_NUM 0x50C1 //Var
#define BATTLE_TOWER_POKE_LEVEL 0x50C2 //Var
#define BATTLE_TOWER_BATTLE_TYPE 0x50C3 //Var
#define BATTLE_TOWER_TIER 0x50C4 //Var
#define BATTLE_TOWER_TID 0x399 //Trainer Index
#define BATTLE_TOWER_TRAINER_NAME 0x50C6 //Empty var. Will be set to 0xFFFF after every battle.
#define BATTLE_TOWER_SONG_OVERRIDE 0x50CF //Set this var to the song id to be played during Link Battles and in the Battle Tower.

enum 
{
	TOWER_TRAINER_ID_VAR = 0x50C7, 			//An index in the gTowerTrainers table, not Trainer ID
	TOWER_TRAINER_ID_2_VAR,	//0x50C8		//Index of the second trainer for Multi Battlers in the gTowerTrainers table, the var should be 1 after the first one
	TOWER_TRAINER_ID_PARTNER_VAR, //0x50C9	//If your partner is randomized, its Id would be found in this var
};

#define TIME_MORNING_START 4		//4:00 AM -  4:00
#define TIME_DAY_START 8			//8:00 AM -  8:00
#define TIME_AFTERNOON_START 17		//5:00 PM - 17:00
#define TIME_NIGHT_START 20			//8:00 PM - 20:00

#define KANTO_DEX_COUNT 151
#define NATIONAL_DEX_COUNT 809
#define MAX_NUM_POKEMON PKMN_MELMETAL + 1

#define MAX_LEVEL 100
#define NUM_TRAINER_CLASSES 107
#define EVOS_PER_MON 16

#define DUSK_BALL_MULTIPLIER 30 //Change this line to 35 to make the catch rate for the Dusk Ball 3.5x like before Gen 7
#define STANDARD_IV 10 //Change this to be the number of IVs Pokemon owned by random trainers should have (all stats will have this number)
#define SWARM_CHANCE 50

#ifndef UNBOUND 
//Change These
#define BASE_OBEDIENCE_LEVEL	10
#define BADGE_1_OBEDIENCE_LEVEL	10
#define BADGE_2_OBEDIENCE_LEVEL 30
#define BADGE_3_OBEDIENCE_LEVEL 30
#define BADGE_4_OBEDIENCE_LEVEL 50
#define BADGE_5_OBEDIENCE_LEVEL 50
#define BADGE_6_OBEDIENCE_LEVEL 70
#define BADGE_7_OBEDIENCE_LEVEL 70

#else //For Pokemon Unbound
#define BASE_OBEDIENCE_LEVEL	20
#define BADGE_1_OBEDIENCE_LEVEL	30
#define BADGE_2_OBEDIENCE_LEVEL 40
#define BADGE_3_OBEDIENCE_LEVEL 50
#define BADGE_4_OBEDIENCE_LEVEL 60
#define BADGE_5_OBEDIENCE_LEVEL 70
#define BADGE_6_OBEDIENCE_LEVEL 80
#define BADGE_7_OBEDIENCE_LEVEL 90
#endif

#define SEEN_DEX_FLAGS 0x203C700 //gSaveBlock2Ptr->pokedex.seen
#define CAUGHT_DEX_FLAGS 0x203C765 //gSaveBlock2Ptr->pokedex.owned

#define INVERSE_BATTLES //Comment this line to disable the possibility of having Inverse Battles
#define TIME_ENABLED //Comment this line to disable time based features. All time dependent features will default in Daytime.
#define OVERWRITE_RIVAL //Comment this line to disable to loading of your rival's name into trainer classes 0x51, 0x59, and 0x5A
#define TRAINER_CLASS_POKE_BALLS //Comment this line to disable creating Trainer's pokemon with Poke Balls specific to their trainer class
#define TRAINERS_WITH_EVS //Comment this line to disbale the Trainers with EVs hack, (only avaible with custom moveset)
#define CONTINUE_LOST_BATTLES //Commenting this line will remove the feature of using trainerbattle 0x9 to continue lost battles if var 0x8000 is set to 0xFEFE
#define CLASS_BASED_BALLS_MULTI_FIX //If this line is uncommented, In Multi Battles, to avoid graphical bugs, the 2nd opponent and your partner will use the same Poke Ball as the 1st opponent. If you don't mind the bugs, comment out this line.
/*Needs Fixing if Undefined*/ #define REALLY_SMART_AI //Comment this line if you don't want the AI knowing everything about your pokemon (such as which moves it has, its ability, item, etc.) without having seen it first. This does not allow the AI to know what move you'll use before you use it.
#define DISPLAY_REAL_MOVE_TYPE_ON_MENU //If this line remains uncommented, the "true" move type will be displayed (such as Hidden Power, Weather Ball in Weather, etc.)
#define DISPLAY_REAL_ACCURACY_ON_MENU //If this line remains uncommented, the "true" move accuracy will be displayed (Eg. The move Psychic on a Pokemon with Compound Eyes will have its accuracy appear as 130)
#define DISPLAY_REAL_POWER_ON_MENU //If this line remains uncommented, the "true" move power will be displayed (Eg. Moves like Fury Cutter will show their correct power)
#define CUSTOM_BATTLE_BACKGROUNDS //If this line remains uncommented, setting BATTLE_BG_VAR will allow custom backgrounds to be loaded in battles
//#define OVERWRITE_BG_FOR_LEADER_CHAMPION //If this remains uncommented, special Battle Background palettes will be loaded in for battles against Gym Leaders and the Champion
#define BRIDGE_FIX //If this remains uncommented, the water battle background will only be loaded if the player's surfing sprite is shown
#define MEGA_EVOLUTION_FEATURE //Comment this line to remove Mega Evolutions
#define TUTORIAL_BATTLES //Comment this line to remove Oak's Tutorial
#define TANOBY_RUINS_ENABLED
#define ALTERING_CAVE_ENABLED
#define SWEET_SCENT_ONLY_IN_CLEAR_WEATHER
#define OBEDIENCE_BY_BADGE_AMOUNT //Determines obedience based on the number of badges the Player has, rather than which badges the player has.

/* Misc Effect Options */
//#define OLD_BURN_DAMAGE //Uncomment this line if you want burn damage to do 1/8 of max health instead of 1/16
//#define OLD_PARALYSIS_SPD_DROP //Uncomment this line if you want paralysis to lower Spd down to 1/4 instead of 1/2
//#define OLD_CONFUSION_CHANCE //Uncomment this line if you want the chance that confusion will stop your attack to 50% instead of 33%
//#define INFINITE_WEATHER //Uncomment this line if you want weather abilities to last for infinite turns
//#define INFINITE_TERRAIN //Uncomment this line if you want terrain abilities to last for infinite turns
//#define NO_SHEER_COLD_NERF //Uncomment this line to remove all Gen 7 Sheer Cold nerfs
//#define OLD_MOVE_SPLIT //Uncomment this line to use the Physical/Special split based on move types. Status moves are still set with the split byte.

/* Ability Options */
//#define OLD_GALE_WINGS //Uncomment this line if you want Gale Wings to activate regardless of the user's HP
//#define OLD_PRANKSTER //Uncomment this line if you want Prankster to be able to affect Dark-Types

/* Damage Calculation Options */
//#define OLD_CRIT_DAMAGE //Uncomment this line if you want Critical hits to do 2x damage, and 3x with Sniper
//#define CRIT_CHANCE_GEN_6 //Uncomment this line if you want the Gen 6 crit chance
//#define CRIT_CHANCE_GEN_2_TO_5 //Uncomment this line if you want the Gens 2-5 crit chance
//Uncommenting both of the above two lines will default in the Gen 6 crit chance
//#define BADGE_BOOSTS //Uncomment this line to allow Badges to give stat boosts
//#define OLD_ATE_BOOST //Uncomment this line to make 'ate' abilities give a 1.3 boost instead of 1.2 
//#define OLD_GEM_BOOST //Uncomment this line to make Gems give a 1.5 boost instead of 1.3
//#define OLD_EXPLOSION_BOOST //Uncomment this line to make Exploding moves halve the target's defense
//#define OLD_HIDDEN_POWER_BP //Uncomment this line for Hidden Power to have its Base Power calculated from the Pokemon's IVs
//#define PORTAL_POWER //Uncomment this line to enable Hoopa-Unbound's special ability in Pokemon Unbound, Portal Power (reduces power of non-contact moves by 25%)
//#define OLD_SOUL_DEW_EFFECT //Uncomment this line if you want the Soul Dew to double Latios + Latias' Sp. Atk & Sp. Def
//#define OLD_PARENTAL_BOND_DAMAGE //Uncomment this line to make the second hit of Parental Bond do 50% of the original damage instead of 25%

/* Capturing Pokemon Options */
//#define CATCH_TRAINERS_POKEMON //Uncomment this line to allow the possibility of capturing trainer's Pokemon
//#define NO_HARDER_WILD_DOUBLES //In Gen 5, Pokemon encountered in double wild battles were harder to catch (based on how many species are owned). Uncomment this line to remove the catch rate decrement.
#define CRITICAL_CAPTURE //Uncomment this line to enable the Critical Capture feature

/* Exp Gain Options */
//#define OLD_EXP_SHARE //Uncomment this line to make the Exp. Share work like it did before Gen 6
#define TRAINER_EXP_BOOST // Uncomment this line to give the Exp boost for battling a Trainer's Pokemon (Pre Gen 7)
//#define OLD_EXP_SPLIT //Uncomment this line to split the Exp amongst all participating pokemon (Pre Gen 6)
#define FLAT_EXP_FORMULA //Uncomment this line to use a Flat Exp calculation formula (Gens 2 - 4, 6)

/* Other Options */
//#define ACTIVATE_DOUBLE_BATTLE_FROM_FLAG //Uncommenting this line will allow you to set a flag to enable double battles if possible
//#define NO_GHOST_BATTLES //Uncomment this line to disable the Ghost battle feature from Pokemon Tower in Lavender town
#define GEN_4_PLAYER_RUNNING_FIX //Uncommmenting this line will increase the lag between frames as the player OW runs, to simulate a more accurate Gen 4 running effect
#define GEN4_PLUS_SELECTION_SCREEN //Uncommenting this line does not give you the Gen 4+ selection screen, it only adds features that supports it
//#define OBEDIENCE_CHECK_FOR_PLAYER_ORIGINAL_POKEMON //Uncommenting line line will open up the possibility that the Player's Pokemon can disobey them (not just traded mons)
//#define WILD_ALWAYS_SMART //Uncomment this line if you want all Wild Pokemon to act smartly
#define HAIL_IN_BATTLE //Uncommenting this line enables the Hail weather effect in battle when the OW weather is set to WEATHER_STEADY_SNOW (0x7)
#define FOG_IN_BATTLE //Uncommenting this line enables the Fog weather effect in battle. Don't uncomment this line without uncommenting one of the lines below!
#define FOG_IN_BATTLE_1 //Uncommenting this line enables the Fog weather effect when the OW weather is set to WEATHER_FOG_1 (0x6)
#define FOG_IN_BATTLE_2 //Uncommenting this line enables the Fog weather effect when the OW weather is set to WEATHER_FOG_2 (0x9)
#define FOG_IN_BATTLE_3 //Uncommenting this line enables the Fog weather effect when the OW weather is set to WEATHER_FOG_3 (0xA)