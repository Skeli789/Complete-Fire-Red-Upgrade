#ifndef GUARD_TRAINERS_H
#define GUARD_TRAINERS_H

// Special Trainer Ids.
#define TRAINER_RECORD_MIXING_FRIEND        300
#define TRAINER_RECORD_MIXING_APPRENTICE    400
#define TRAINER_EREADER                     500
#define TRAINER_FRONTIER_BRAIN              1022
#define TRAINER_PLAYER                      1023
#define TRAINER_SECRET_BASE                 1024
#define TRAINER_LINK_OPPONENT               2048
#define TRAINER_OPPONENT_C00                3072
#define TRAINER_STEVEN_PARTNER              3075
#define TRAINER_CUSTOM_PARTNER              3076

#define TRAINER_CLASS_LEADER				0x54
#define TRAINER_CLASS_ROCKET				0x55
#define TRAINER_CLASS_ELITE_4				0x57
#define TRAINER_CLASS_CHAMPION				0x5A

#ifndef UNBOUND //CHANGE THESE

#define TRAINER_ENCOUNTER_MUSIC_MALE         0 // standard male encounter music
#define TRAINER_ENCOUNTER_MUSIC_FEMALE       1 // standard female encounter music
#define TRAINER_ENCOUNTER_MUSIC_GIRL         2 // used for male Tubers and Young Couples too
#define TRAINER_ENCOUNTER_MUSIC_SUSPICIOUS   3
#define TRAINER_ENCOUNTER_MUSIC_INTENSE      4
#define TRAINER_ENCOUNTER_MUSIC_COOL         5
#define TRAINER_ENCOUNTER_MUSIC_AQUA         6
#define TRAINER_ENCOUNTER_MUSIC_MAGMA        7
#define TRAINER_ENCOUNTER_MUSIC_SWIMMER      8
#define TRAINER_ENCOUNTER_MUSIC_TWINS        9 // used for other trainer classes too
#define TRAINER_ENCOUNTER_MUSIC_ELITE_FOUR  10
#define TRAINER_ENCOUNTER_MUSIC_HIKER       11 // used for other trainer classes too
#define TRAINER_ENCOUNTER_MUSIC_INTERVIEWER 12
#define TRAINER_ENCOUNTER_MUSIC_RICH        13 // Rich Boys and Gentlemen

#else //For Unbound

#define TRAINER_ENCOUNTER_MALE         			0 
#define TRAINER_ENCOUNTER_FEMALE       			1
#define TRAINER_ENCOUNTER_LADY        	 		2
#define TRAINER_ENCOUNTER_SUSPICIOUS  			3
#define TRAINER_ENCOUNTER_INTENSE      			4
#define TRAINER_ENCOUNTER_ACE_TRAINER       	5
#define TRAINER_ENCOUNTER_HIKER         		6
#define TRAINER_ENCOUNTER_MYSTERIOUS_TRAINER    7
#define TRAINER_ENCOUNTER_SAILOR      			8	
#define TRAINER_ENCOUNTER_GAMBLER        		9
#define TRAINER_ENCOUNTER_ARTIST  				10
#define TRAINER_ENCOUNTER_SHADOW       			11
#define TRAINER_ENCOUNTER_LOR 					12
#define TRAINER_ENCOUNTER_ELITE_4        		13
#define TRAINER_ENCOUNTER_CYNTHIA				14

#define TRAINER_CLASS_LOR_LEADER 				0x2
#define TRAINER_CLASS_LOR_ADMIN 				0x2E
#define TRAINER_CLASS_LOR 						0x2F
#define TRAINER_CLASS_SHADOW_ADMIN 				0x30
#define TRAINER_CLASS_BOSS 						0x53
#define TRAINER_CLASS_SHADOW					0x55

#endif

#define F_TRAINER_FEMALE (1 << 7)

// All trainer parties specify the IV, level, and species for each Pokémon in the
// party. Some trainer parties also specify held items and custom moves for each
// Pokémon.
#define F_TRAINER_PARTY_CUSTOM_MOVESET (1 << 0)
#define F_TRAINER_PARTY_HELD_ITEM      (1 << 1)

#endif  // GUARD_TRAINERS_H
