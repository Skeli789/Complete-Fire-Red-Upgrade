#include "../config.h"
#include "../../include/global.h"
#include "trainer_details.h"
#include "../../include/new/frontier.h"
#include "../../include/new/text.h"
#include "../../include/constants/trainer_classes.h"
#include "../../include/constants/songs.h"

#include "frontier_trainer_names.h"
#include "frontier_spreads.h"

/*struct BattleTowerTrainer
{
    u16 owNum;
	u8 trainerClass;
	u8 trainerSprite;
	u8 gender;
	u8* preBattleText;
	u8* playerWinText;
	u8* playerLoseText;
};*/

extern const u8 sFrontierText_Youngster_PreBattle_1[];
extern const u8 sFrontierText_Youngster_PlayerWin_1[];
extern const u8 sFrontierText_Youngster_PlayerLose_1[];
extern const u8 sFrontierText_Youngster_PreBattle_2[];
extern const u8 sFrontierText_Youngster_PlayerWin_2[];
extern const u8 sFrontierText_Youngster_PlayerLose_2[];
extern const u8 sFrontierText_Youngster_PreBattle_3[];
extern const u8 sFrontierText_Youngster_PlayerWin_3[];
extern const u8 sFrontierText_Youngster_PlayerLose_3[];

#ifdef UNBOUND
extern const u8 sFrontierText_Bugcatcher_PreBattle_1[];
extern const u8 sFrontierText_Bugcatcher_PlayerWin_1[];
extern const u8 sFrontierText_Bugcatcher_PlayerLose_1[];
extern const u8 sFrontierText_Bugcatcher_PreBattle_2[];
extern const u8 sFrontierText_Bugcatcher_PlayerWin_2[];
extern const u8 sFrontierText_Bugcatcher_PlayerLose_2[];
extern const u8 sFrontierText_Bugcatcher_PreBattle_3[];
extern const u8 sFrontierText_Bugcatcher_PlayerWin_3[];
extern const u8 sFrontierText_Bugcatcher_PlayerLose_3[];
extern const u8 sFrontierText_Lass_PreBattle_1[];
extern const u8 sFrontierText_Lass_PlayerWin_1[];
extern const u8 sFrontierText_Lass_PlayerLose_1[];
extern const u8 sFrontierText_Lass_PreBattle_2[];
extern const u8 sFrontierText_Lass_PlayerWin_2[];
extern const u8 sFrontierText_Lass_PlayerLose_2[];
extern const u8 sFrontierText_Lass_PreBattle_3[];
extern const u8 sFrontierText_Lass_PlayerWin_3[];
extern const u8 sFrontierText_Lass_PlayerLose_3[];
extern const u8 sFrontierText_Sailor_PreBattle_1[];
extern const u8 sFrontierText_Sailor_PlayerWin_1[];
extern const u8 sFrontierText_Sailor_PlayerLose_1[];
extern const u8 sFrontierText_Sailor_PreBattle_2[];
extern const u8 sFrontierText_Sailor_PlayerWin_2[];
extern const u8 sFrontierText_Sailor_PlayerLose_2[];
extern const u8 sFrontierText_Sailor_PreBattle_3[];
extern const u8 sFrontierText_Sailor_PlayerWin_3[];
extern const u8 sFrontierText_Sailor_PlayerLose_3[];
extern const u8 sFrontierText_Camper_PreBattle_1[];
extern const u8 sFrontierText_Camper_PlayerWin_1[];
extern const u8 sFrontierText_Camper_PlayerLose_1[];
extern const u8 sFrontierText_Camper_PreBattle_2[];
extern const u8 sFrontierText_Camper_PlayerWin_2[];
extern const u8 sFrontierText_Camper_PlayerLose_2[];
extern const u8 sFrontierText_Camper_PreBattle_3[];
extern const u8 sFrontierText_Camper_PlayerWin_3[];
extern const u8 sFrontierText_Camper_PlayerLose_3[];
extern const u8 sFrontierText_Picnicker_PreBattle_1[];
extern const u8 sFrontierText_Picnicker_PlayerWin_1[];
extern const u8 sFrontierText_Picnicker_PlayerLose_1[];
extern const u8 sFrontierText_Picnicker_PreBattle_2[];
extern const u8 sFrontierText_Picnicker_PlayerWin_2[];
extern const u8 sFrontierText_Picnicker_PlayerLose_2[];
extern const u8 sFrontierText_Picnicker_PreBattle_3[];
extern const u8 sFrontierText_Picnicker_PlayerWin_3[];
extern const u8 sFrontierText_Picnicker_PlayerLose_3[];
extern const u8 sFrontierText_Pokemaniac_PreBattle_1[];
extern const u8 sFrontierText_Pokemaniac_PlayerWin_1[];
extern const u8 sFrontierText_Pokemaniac_PlayerLose_1[];
extern const u8 sFrontierText_Pokemaniac_PreBattle_2[];
extern const u8 sFrontierText_Pokemaniac_PlayerWin_2[];
extern const u8 sFrontierText_Pokemaniac_PlayerLose_2[];
extern const u8 sFrontierText_Pokemaniac_PreBattle_3[];
extern const u8 sFrontierText_Pokemaniac_PlayerWin_3[];
extern const u8 sFrontierText_Pokemaniac_PlayerLose_3[];
extern const u8 sFrontierText_Supernerd_PreBattle_1[];
extern const u8 sFrontierText_Supernerd_PlayerWin_1[];
extern const u8 sFrontierText_Supernerd_PlayerLose_1[];
extern const u8 sFrontierText_Supernerd_PreBattle_2[];
extern const u8 sFrontierText_Supernerd_PlayerWin_2[];
extern const u8 sFrontierText_Supernerd_PlayerLose_2[];
extern const u8 sFrontierText_Supernerd_PreBattle_3[];
extern const u8 sFrontierText_Supernerd_PlayerWin_3[];
extern const u8 sFrontierText_Supernerd_PlayerLose_3[];
extern const u8 sFrontierText_Hiker_PreBattle_1[];
extern const u8 sFrontierText_Hiker_PlayerWin_1[];
extern const u8 sFrontierText_Hiker_PlayerLose_1[];
extern const u8 sFrontierText_Hiker_PreBattle_2[];
extern const u8 sFrontierText_Hiker_PlayerWin_2[];
extern const u8 sFrontierText_Hiker_PlayerLose_2[];
extern const u8 sFrontierText_Hiker_PreBattle_3[];
extern const u8 sFrontierText_Hiker_PlayerWin_3[];
extern const u8 sFrontierText_Hiker_PlayerLose_3[];
extern const u8 sFrontierText_Biker_PreBattle_1[];
extern const u8 sFrontierText_Biker_PlayerWin_1[];
extern const u8 sFrontierText_Biker_PlayerLose_1[];
extern const u8 sFrontierText_Biker_PreBattle_2[];
extern const u8 sFrontierText_Biker_PlayerWin_2[];
extern const u8 sFrontierText_Biker_PlayerLose_2[];
extern const u8 sFrontierText_Biker_PreBattle_3[];
extern const u8 sFrontierText_Biker_PlayerWin_3[];
extern const u8 sFrontierText_Biker_PlayerLose_3[];
extern const u8 sFrontierText_Cyclist_PreBattle_1[];
extern const u8 sFrontierText_Cyclist_PlayerWin_1[];
extern const u8 sFrontierText_Cyclist_PlayerLose_1[];
extern const u8 sFrontierText_Cyclist_PreBattle_2[];
extern const u8 sFrontierText_Cyclist_PlayerWin_2[];
extern const u8 sFrontierText_Cyclist_PlayerLose_2[];
extern const u8 sFrontierText_Cyclist_PreBattle_3[];
extern const u8 sFrontierText_Cyclist_PlayerWin_3[];
extern const u8 sFrontierText_Cyclist_PlayerLose_3[];
extern const u8 sFrontierText_Worker_PreBattle_1[];
extern const u8 sFrontierText_Worker_PlayerWin_1[];
extern const u8 sFrontierText_Worker_PlayerLose_1[];
extern const u8 sFrontierText_Worker_PreBattle_2[];
extern const u8 sFrontierText_Worker_PlayerWin_2[];
extern const u8 sFrontierText_Worker_PlayerLose_2[];
extern const u8 sFrontierText_Worker_PreBattle_3[];
extern const u8 sFrontierText_Worker_PlayerWin_3[];
extern const u8 sFrontierText_Worker_PlayerLose_3[];
extern const u8 sFrontierText_Fisherman_PreBattle_1[];
extern const u8 sFrontierText_Fisherman_PlayerWin_1[];
extern const u8 sFrontierText_Fisherman_PlayerLose_1[];
extern const u8 sFrontierText_Fisherman_PreBattle_2[];
extern const u8 sFrontierText_Fisherman_PlayerWin_2[];
extern const u8 sFrontierText_Fisherman_PlayerLose_2[];
extern const u8 sFrontierText_Fisherman_PreBattle_3[];
extern const u8 sFrontierText_Fisherman_PlayerWin_3[];
extern const u8 sFrontierText_Fisherman_PlayerLose_3[];
extern const u8 sFrontierText_Roughneck_PreBattle_1[];
extern const u8 sFrontierText_Roughneck_PlayerWin_1[];
extern const u8 sFrontierText_Roughneck_PlayerLose_1[];
extern const u8 sFrontierText_Roughneck_PreBattle_2[];
extern const u8 sFrontierText_Roughneck_PlayerWin_2[];
extern const u8 sFrontierText_Roughneck_PlayerLose_2[];
extern const u8 sFrontierText_Roughneck_PreBattle_3[];
extern const u8 sFrontierText_Roughneck_PlayerWin_3[];
extern const u8 sFrontierText_Roughneck_PlayerLose_3[];
extern const u8 sFrontierText_Beauty_PreBattle_1[];
extern const u8 sFrontierText_Beauty_PlayerWin_1[];
extern const u8 sFrontierText_Beauty_PlayerLose_1[];
extern const u8 sFrontierText_Beauty_PreBattle_2[];
extern const u8 sFrontierText_Beauty_PlayerWin_2[];
extern const u8 sFrontierText_Beauty_PlayerLose_2[];
extern const u8 sFrontierText_Beauty_PreBattle_3[];
extern const u8 sFrontierText_Beauty_PlayerWin_3[];
extern const u8 sFrontierText_Beauty_PlayerLose_3[];
extern const u8 sFrontierText_Psychic_PreBattle_1[];
extern const u8 sFrontierText_Psychic_PlayerWin_1[];
extern const u8 sFrontierText_Psychic_PlayerLose_1[];
extern const u8 sFrontierText_Psychic_PreBattle_2[];
extern const u8 sFrontierText_Psychic_PlayerWin_2[];
extern const u8 sFrontierText_Psychic_PlayerLose_2[];
extern const u8 sFrontierText_Psychic_PreBattle_3[];
extern const u8 sFrontierText_Psychic_PlayerWin_3[];
extern const u8 sFrontierText_Psychic_PlayerLose_3[];
extern const u8 sFrontierText_Psychic_PreBattle_4[];
extern const u8 sFrontierText_Psychic_PlayerWin_4[];
extern const u8 sFrontierText_Psychic_PlayerLose_4[];
extern const u8 sFrontierText_Psychic_PreBattle_5[];
extern const u8 sFrontierText_Psychic_PlayerWin_5[];
extern const u8 sFrontierText_Psychic_PlayerLose_5[];
extern const u8 sFrontierText_Psychic_PreBattle_6[];
extern const u8 sFrontierText_Psychic_PlayerWin_6[];
extern const u8 sFrontierText_Psychic_PlayerLose_6[];
extern const u8 sFrontierText_Guitarist_PreBattle_1[];
extern const u8 sFrontierText_Guitarist_PlayerWin_1[];
extern const u8 sFrontierText_Guitarist_PlayerLose_1[];
extern const u8 sFrontierText_Guitarist_PreBattle_2[];
extern const u8 sFrontierText_Guitarist_PlayerWin_2[];
extern const u8 sFrontierText_Guitarist_PlayerLose_2[];
extern const u8 sFrontierText_Guitarist_PreBattle_3[];
extern const u8 sFrontierText_Guitarist_PlayerWin_3[];
extern const u8 sFrontierText_Guitarist_PlayerLose_3[];
extern const u8 sFrontierText_Tamer_PreBattle_1[];
extern const u8 sFrontierText_Tamer_PlayerWin_1[];
extern const u8 sFrontierText_Tamer_PlayerLose_1[];
extern const u8 sFrontierText_Tamer_PreBattle_2[];
extern const u8 sFrontierText_Tamer_PlayerWin_2[];
extern const u8 sFrontierText_Tamer_PlayerLose_2[];
extern const u8 sFrontierText_Tamer_PreBattle_3[];
extern const u8 sFrontierText_Tamer_PlayerWin_3[];
extern const u8 sFrontierText_Tamer_PlayerLose_3[];
extern const u8 sFrontierText_Birdkeeper_PreBattle_1[];
extern const u8 sFrontierText_Birdkeeper_PlayerWin_1[];
extern const u8 sFrontierText_Birdkeeper_PlayerLose_1[];
extern const u8 sFrontierText_Birdkeeper_PreBattle_2[];
extern const u8 sFrontierText_Birdkeeper_PlayerWin_2[];
extern const u8 sFrontierText_Birdkeeper_PlayerLose_2[];
extern const u8 sFrontierText_Birdkeeper_PreBattle_3[];
extern const u8 sFrontierText_Birdkeeper_PlayerWin_3[];
extern const u8 sFrontierText_Birdkeeper_PlayerLose_3[];
extern const u8 sFrontierText_Blackbelt_PreBattle_1[];
extern const u8 sFrontierText_Blackbelt_PlayerWin_1[];
extern const u8 sFrontierText_Blackbelt_PlayerLose_1[];
extern const u8 sFrontierText_Blackbelt_PreBattle_2[];
extern const u8 sFrontierText_Blackbelt_PlayerWin_2[];
extern const u8 sFrontierText_Blackbelt_PlayerLose_2[];
extern const u8 sFrontierText_Blackbelt_PreBattle_3[];
extern const u8 sFrontierText_Blackbelt_PlayerWin_3[];
extern const u8 sFrontierText_Blackbelt_PlayerLose_3[];
extern const u8 sFrontierText_Scientist_PreBattle_1[];
extern const u8 sFrontierText_Scientist_PlayerWin_1[];
extern const u8 sFrontierText_Scientist_PlayerLose_1[];
extern const u8 sFrontierText_Scientist_PreBattle_2[];
extern const u8 sFrontierText_Scientist_PlayerWin_2[];
extern const u8 sFrontierText_Scientist_PlayerLose_2[];
extern const u8 sFrontierText_Scientist_PreBattle_3[];
extern const u8 sFrontierText_Scientist_PlayerWin_3[];
extern const u8 sFrontierText_Scientist_PlayerLose_3[];
extern const u8 sFrontierText_Scientist_PreBattle_4[];
extern const u8 sFrontierText_Scientist_PlayerWin_4[];
extern const u8 sFrontierText_Scientist_PlayerLose_4[];
extern const u8 sFrontierText_Acetrainer_PreBattle_1[];
extern const u8 sFrontierText_Acetrainer_PlayerWin_1[];
extern const u8 sFrontierText_Acetrainer_PlayerLose_1[];
extern const u8 sFrontierText_Acetrainer_PreBattle_2[];
extern const u8 sFrontierText_Acetrainer_PlayerWin_2[];
extern const u8 sFrontierText_Acetrainer_PlayerLose_2[];
extern const u8 sFrontierText_Acetrainer_PreBattle_3[];
extern const u8 sFrontierText_Acetrainer_PlayerWin_3[];
extern const u8 sFrontierText_Acetrainer_PlayerLose_3[];
extern const u8 sFrontierText_Acetrainer_PreBattle_4[];
extern const u8 sFrontierText_Acetrainer_PlayerWin_4[];
extern const u8 sFrontierText_Acetrainer_PlayerLose_4[];
extern const u8 sFrontierText_Acetrainer_PreBattle_5[];
extern const u8 sFrontierText_Acetrainer_PlayerWin_5[];
extern const u8 sFrontierText_Acetrainer_PlayerLose_5[];
extern const u8 sFrontierText_Acetrainer_PreBattle_6[];
extern const u8 sFrontierText_Acetrainer_PlayerWin_6[];
extern const u8 sFrontierText_Acetrainer_PlayerLose_6[];
extern const u8 sFrontierText_Gentleman_PreBattle_1[];
extern const u8 sFrontierText_Gentleman_PlayerWin_1[];
extern const u8 sFrontierText_Gentleman_PlayerLose_1[];
extern const u8 sFrontierText_Gentleman_PreBattle_2[];
extern const u8 sFrontierText_Gentleman_PlayerWin_2[];
extern const u8 sFrontierText_Gentleman_PlayerLose_2[];
extern const u8 sFrontierText_Gentleman_PreBattle_3[];
extern const u8 sFrontierText_Gentleman_PlayerWin_3[];
extern const u8 sFrontierText_Gentleman_PlayerLose_3[];
extern const u8 sFrontierText_Tuber_PreBattle_1[];
extern const u8 sFrontierText_Tuber_PlayerWin_1[];
extern const u8 sFrontierText_Tuber_PlayerLose_1[];
extern const u8 sFrontierText_Tuber_PreBattle_2[];
extern const u8 sFrontierText_Tuber_PlayerWin_2[];
extern const u8 sFrontierText_Tuber_PlayerLose_2[];
extern const u8 sFrontierText_Tuber_PreBattle_3[];
extern const u8 sFrontierText_Tuber_PlayerWin_3[];
extern const u8 sFrontierText_Tuber_PlayerLose_3[];
extern const u8 sFrontierText_Tuber_PreBattle_4[];
extern const u8 sFrontierText_Tuber_PlayerWin_4[];
extern const u8 sFrontierText_Tuber_PlayerLose_4[];
extern const u8 sFrontierText_Tuber_PreBattle_5[];
extern const u8 sFrontierText_Tuber_PlayerWin_5[];
extern const u8 sFrontierText_Tuber_PlayerLose_5[];
extern const u8 sFrontierText_Tuber_PreBattle_6[];
extern const u8 sFrontierText_Tuber_PlayerWin_6[];
extern const u8 sFrontierText_Tuber_PlayerLose_6[];
extern const u8 sFrontierText_Pkmnbreeder_PreBattle_1[];
extern const u8 sFrontierText_Pkmnbreeder_PlayerWin_1[];
extern const u8 sFrontierText_Pkmnbreeder_PlayerLose_1[];
extern const u8 sFrontierText_Pkmnbreeder_PreBattle_2[];
extern const u8 sFrontierText_Pkmnbreeder_PlayerWin_2[];
extern const u8 sFrontierText_Pkmnbreeder_PlayerLose_2[];
extern const u8 sFrontierText_Pkmnbreeder_PreBattle_3[];
extern const u8 sFrontierText_Pkmnbreeder_PlayerWin_3[];
extern const u8 sFrontierText_Pkmnbreeder_PlayerLose_3[];
extern const u8 sFrontierText_Pkmnbreeder_PreBattle_4[];
extern const u8 sFrontierText_Pkmnbreeder_PlayerWin_4[];
extern const u8 sFrontierText_Pkmnbreeder_PlayerLose_4[];
extern const u8 sFrontierText_Pkmnbreeder_PreBattle_5[];
extern const u8 sFrontierText_Pkmnbreeder_PlayerWin_5[];
extern const u8 sFrontierText_Pkmnbreeder_PlayerLose_5[];
extern const u8 sFrontierText_Pkmnbreeder_PreBattle_6[];
extern const u8 sFrontierText_Pkmnbreeder_PlayerWin_6[];
extern const u8 sFrontierText_Pkmnbreeder_PlayerLose_6[];
extern const u8 sFrontierText_Pkmnranger_PreBattle_1[];
extern const u8 sFrontierText_Pkmnranger_PlayerWin_1[];
extern const u8 sFrontierText_Pkmnranger_PlayerLose_1[];
extern const u8 sFrontierText_Pkmnranger_PreBattle_2[];
extern const u8 sFrontierText_Pkmnranger_PlayerWin_2[];
extern const u8 sFrontierText_Pkmnranger_PlayerLose_2[];
extern const u8 sFrontierText_Pkmnranger_PreBattle_3[];
extern const u8 sFrontierText_Pkmnranger_PlayerWin_3[];
extern const u8 sFrontierText_Pkmnranger_PlayerLose_3[];
extern const u8 sFrontierText_Pkmnranger_PreBattle_4[];
extern const u8 sFrontierText_Pkmnranger_PlayerWin_4[];
extern const u8 sFrontierText_Pkmnranger_PlayerLose_4[];
extern const u8 sFrontierText_Pkmnranger_PreBattle_5[];
extern const u8 sFrontierText_Pkmnranger_PlayerWin_5[];
extern const u8 sFrontierText_Pkmnranger_PlayerLose_5[];
extern const u8 sFrontierText_Pkmnranger_PreBattle_6[];
extern const u8 sFrontierText_Pkmnranger_PlayerWin_6[];
extern const u8 sFrontierText_Pkmnranger_PlayerLose_6[];
extern const u8 sFrontierText_Aromalady_PreBattle_1[];
extern const u8 sFrontierText_Aromalady_PlayerWin_1[];
extern const u8 sFrontierText_Aromalady_PlayerLose_1[];
extern const u8 sFrontierText_Aromalady_PreBattle_2[];
extern const u8 sFrontierText_Aromalady_PlayerWin_2[];
extern const u8 sFrontierText_Aromalady_PlayerLose_2[];
extern const u8 sFrontierText_Aromalady_PreBattle_3[];
extern const u8 sFrontierText_Aromalady_PlayerWin_3[];
extern const u8 sFrontierText_Aromalady_PlayerLose_3[];
extern const u8 sFrontierText_Ruinmaniac_PreBattle_1[];
extern const u8 sFrontierText_Ruinmaniac_PlayerWin_1[];
extern const u8 sFrontierText_Ruinmaniac_PlayerLose_1[];
extern const u8 sFrontierText_Ruinmaniac_PreBattle_2[];
extern const u8 sFrontierText_Ruinmaniac_PlayerWin_2[];
extern const u8 sFrontierText_Ruinmaniac_PlayerLose_2[];
extern const u8 sFrontierText_Ruinmaniac_PreBattle_3[];
extern const u8 sFrontierText_Ruinmaniac_PlayerWin_3[];
extern const u8 sFrontierText_Ruinmaniac_PlayerLose_3[];
extern const u8 sFrontierText_Richboy_PreBattle_1[];
extern const u8 sFrontierText_Richboy_PlayerWin_1[];
extern const u8 sFrontierText_Richboy_PlayerLose_1[];
extern const u8 sFrontierText_Richboy_PreBattle_2[];
extern const u8 sFrontierText_Richboy_PlayerWin_2[];
extern const u8 sFrontierText_Richboy_PlayerLose_2[];
extern const u8 sFrontierText_Richboy_PreBattle_3[];
extern const u8 sFrontierText_Richboy_PlayerWin_3[];
extern const u8 sFrontierText_Richboy_PlayerLose_3[];
extern const u8 sFrontierText_Richgirl_PreBattle_1[];
extern const u8 sFrontierText_Richgirl_PlayerWin_1[];
extern const u8 sFrontierText_Richgirl_PlayerLose_1[];
extern const u8 sFrontierText_Richgirl_PreBattle_2[];
extern const u8 sFrontierText_Richgirl_PlayerWin_2[];
extern const u8 sFrontierText_Richgirl_PlayerLose_2[];
extern const u8 sFrontierText_Richgirl_PreBattle_3[];
extern const u8 sFrontierText_Richgirl_PlayerWin_3[];
extern const u8 sFrontierText_Richgirl_PlayerLose_3[];
extern const u8 sFrontierText_Painter_PreBattle_1[];
extern const u8 sFrontierText_Painter_PlayerWin_1[];
extern const u8 sFrontierText_Painter_PlayerLose_1[];
extern const u8 sFrontierText_Painter_PreBattle_2[];
extern const u8 sFrontierText_Painter_PlayerWin_2[];
extern const u8 sFrontierText_Painter_PlayerLose_2[];
extern const u8 sFrontierText_Painter_PreBattle_3[];
extern const u8 sFrontierText_Painter_PlayerWin_3[];
extern const u8 sFrontierText_Painter_PlayerLose_3[];
extern const u8 sFrontierText_Waiter_PreBattle_1[];
extern const u8 sFrontierText_Waiter_PlayerWin_1[];
extern const u8 sFrontierText_Waiter_PlayerLose_1[];
extern const u8 sFrontierText_Waiter_PreBattle_2[];
extern const u8 sFrontierText_Waiter_PlayerWin_2[];
extern const u8 sFrontierText_Waiter_PlayerLose_2[];
extern const u8 sFrontierText_Waiter_PreBattle_3[];
extern const u8 sFrontierText_Waiter_PlayerWin_3[];
extern const u8 sFrontierText_Waiter_PlayerLose_3[];
extern const u8 sFrontierText_Waitress_PreBattle_1[];
extern const u8 sFrontierText_Waitress_PlayerWin_1[];
extern const u8 sFrontierText_Waitress_PlayerLose_1[];
extern const u8 sFrontierText_Waitress_PreBattle_2[];
extern const u8 sFrontierText_Waitress_PlayerWin_2[];
extern const u8 sFrontierText_Waitress_PlayerLose_2[];
extern const u8 sFrontierText_Waitress_PreBattle_3[];
extern const u8 sFrontierText_Waitress_PlayerWin_3[];
extern const u8 sFrontierText_Waitress_PlayerLose_3[];
extern const u8 sFrontierText_Idol_PreBattle_1[];
extern const u8 sFrontierText_Idol_PlayerWin_1[];
extern const u8 sFrontierText_Idol_PlayerLose_1[];
extern const u8 sFrontierText_Idol_PreBattle_2[];
extern const u8 sFrontierText_Idol_PlayerWin_2[];
extern const u8 sFrontierText_Idol_PlayerLose_2[];
extern const u8 sFrontierText_Idol_PreBattle_3[];
extern const u8 sFrontierText_Idol_PlayerWin_3[];
extern const u8 sFrontierText_Idol_PlayerLose_3[];
extern const u8 sFrontierText_Pokekid_PreBattle_1[];
extern const u8 sFrontierText_Pokekid_PlayerWin_1[];
extern const u8 sFrontierText_Pokekid_PlayerLose_1[];
extern const u8 sFrontierText_Pokekid_PreBattle_2[];
extern const u8 sFrontierText_Pokekid_PlayerWin_2[];
extern const u8 sFrontierText_Pokekid_PlayerLose_2[];
extern const u8 sFrontierText_Pokekid_PreBattle_3[];
extern const u8 sFrontierText_Pokekid_PlayerWin_3[];
extern const u8 sFrontierText_Pokekid_PlayerLose_3[];
extern const u8 sFrontierText_Jogger_PreBattle_1[];
extern const u8 sFrontierText_Jogger_PlayerWin_1[];
extern const u8 sFrontierText_Jogger_PlayerLose_1[];
extern const u8 sFrontierText_Jogger_PreBattle_2[];
extern const u8 sFrontierText_Jogger_PlayerWin_2[];
extern const u8 sFrontierText_Jogger_PlayerLose_2[];
extern const u8 sFrontierText_Jogger_PreBattle_3[];
extern const u8 sFrontierText_Jogger_PlayerWin_3[];
extern const u8 sFrontierText_Jogger_PlayerLose_3[];
extern const u8 sFrontierText_Rancher_PreBattle_1[];
extern const u8 sFrontierText_Rancher_PlayerWin_1[];
extern const u8 sFrontierText_Rancher_PlayerLose_1[];
extern const u8 sFrontierText_Rancher_PreBattle_2[];
extern const u8 sFrontierText_Rancher_PlayerWin_2[];
extern const u8 sFrontierText_Rancher_PlayerLose_2[];
extern const u8 sFrontierText_Rancher_PreBattle_3[];
extern const u8 sFrontierText_Rancher_PlayerWin_3[];
extern const u8 sFrontierText_Rancher_PlayerLose_3[];
extern const u8 sFrontierText_Rancher_PreBattle_4[];
extern const u8 sFrontierText_Rancher_PlayerWin_4[];
extern const u8 sFrontierText_Rancher_PlayerLose_4[];
extern const u8 sFrontierText_Rancher_PreBattle_5[];
extern const u8 sFrontierText_Rancher_PlayerWin_5[];
extern const u8 sFrontierText_Rancher_PlayerLose_5[];
extern const u8 sFrontierText_Rancher_PreBattle_6[];
extern const u8 sFrontierText_Rancher_PlayerWin_6[];
extern const u8 sFrontierText_Rancher_PlayerLose_6[];

extern const u8 sTrainerName_Palmer[];
extern const u8 sTrainerName_Skeli[];
extern const u8 sTrainerName_Golche[];
extern const u8 sTrainerName_Gail[];
extern const u8 sTrainerName_Lixdel[];
extern const u8 sTrainerName_Cynthia[];
extern const u8 sTrainerName_Red[];
extern const u8 sTrainerName_BigMo[];
extern const u8 sTrainerName_Galavan[];
extern const u8 sTrainerName_Candice[];
extern const u8 sTrainerName_Giovanni[];

extern const u8 sFrontierText_Skeli_PreBattle[];
extern const u8 sFrontierText_Skeli_PlayerWin[];
extern const u8 sFrontierText_Skeli_PlayerLose[];

extern const u8 sFrontierText_Golche_PreBattle[];
extern const u8 sFrontierText_Golche_PlayerWin[];
extern const u8 sFrontierText_Golche_PlayerLose[];

extern const u8 sFrontierText_Gail_PreBattle[];
extern const u8 sFrontierText_Gail_PlayerWin[];
extern const u8 sFrontierText_Gail_PlayerLose[];

extern const u8 sFrontierText_Lixdel_PreBattle[];
extern const u8 sFrontierText_Lixdel_PlayerWin[];
extern const u8 sFrontierText_Lixdel_PlayerLose[];

extern const u8 sFrontierText_Cynthia_PreBattle[];
extern const u8 sFrontierText_Cynthia_PlayerWin[];
extern const u8 sFrontierText_Cynthia_PlayerLose[];

extern const u8 sFrontierText_Red_PreBattle[];
extern const u8 sFrontierText_Red_PlayerWin[];
extern const u8 sFrontierText_Red_PlayerLose[];

extern const u8 sFrontierText_BigMo_PreBattle[];
extern const u8 sFrontierText_BigMo_PlayerWin[];
extern const u8 sFrontierText_BigMo_PlayerLose[];

extern const u8 sFrontierText_Galavan_PreBattle[];
extern const u8 sFrontierText_Galavan_PlayerWin[];
extern const u8 sFrontierText_Galavan_PlayerLose[];

extern const u8 sFrontierText_Candice_PreBattle[];
extern const u8 sFrontierText_Candice_PlayerWin[];
extern const u8 sFrontierText_Candice_PlayerLose[];

extern const u8 sFrontierText_Giovanni_PreBattle[];
extern const u8 sFrontierText_Giovanni_PlayerWin[];
extern const u8 sFrontierText_Giovanni_PlayerLose[];

const struct BattleTowerTrainer gTowerTrainers[] = 
{
	{
		.owNum = 			OW_YOUNGSTER,
		.trainerClass = 	CLASS_YOUNGSTER,
		.trainerSprite = 	TRAINER_SPRITE_YOUNGSTER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Youngster_PreBattle_1,
		.playerWinText = 	sFrontierText_Youngster_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Youngster_PlayerLose_1,
	},
	{
		.owNum = 			OW_YOUNGSTER,
		.trainerClass = 	CLASS_YOUNGSTER,
		.trainerSprite = 	TRAINER_SPRITE_YOUNGSTER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Youngster_PreBattle_2,
		.playerWinText = 	sFrontierText_Youngster_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Youngster_PlayerLose_2,
	},
	{
		.owNum = 			OW_YOUNGSTER,
		.trainerClass = 	CLASS_YOUNGSTER,
		.trainerSprite = 	TRAINER_SPRITE_YOUNGSTER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Youngster_PreBattle_3,
		.playerWinText = 	sFrontierText_Youngster_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Youngster_PlayerLose_3,
	},
	{
		.owNum = 			OW_BUG_CATCHER,
		.trainerClass = 	CLASS_BUG_CATCHER,
		.trainerSprite = 	TRAINER_SPRITE_BUGCATCHER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Bugcatcher_PreBattle_1,
		.playerWinText = 	sFrontierText_Bugcatcher_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Bugcatcher_PlayerLose_1,
	},
	{
		.owNum = 			OW_BUG_CATCHER,
		.trainerClass = 	CLASS_BUG_CATCHER,
		.trainerSprite = 	TRAINER_SPRITE_BUGCATCHER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Bugcatcher_PreBattle_2,
		.playerWinText = 	sFrontierText_Bugcatcher_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Bugcatcher_PlayerLose_2,
	},
	{
		.owNum = 			OW_BUG_CATCHER,
		.trainerClass = 	CLASS_BUG_CATCHER,
		.trainerSprite = 	TRAINER_SPRITE_BUGCATCHER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Bugcatcher_PreBattle_3,
		.playerWinText = 	sFrontierText_Bugcatcher_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Bugcatcher_PlayerLose_3,
	},
	{
		.owNum = 			OW_LASS,
		.trainerClass = 	CLASS_LASS,
		.trainerSprite = 	TRAINER_SPRITE_LASS,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Lass_PreBattle_1,
		.playerWinText = 	sFrontierText_Lass_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Lass_PlayerLose_1,
	},
	{
		.owNum = 			OW_LASS,
		.trainerClass = 	CLASS_LASS,
		.trainerSprite = 	TRAINER_SPRITE_LASS,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Lass_PreBattle_2,
		.playerWinText = 	sFrontierText_Lass_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Lass_PlayerLose_2,
	},
	{
		.owNum = 			OW_LASS,
		.trainerClass = 	CLASS_LASS,
		.trainerSprite = 	TRAINER_SPRITE_LASS,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Lass_PreBattle_3,
		.playerWinText = 	sFrontierText_Lass_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Lass_PlayerLose_3,
	},
	{
		.owNum = 			OW_SAILOR,
		.trainerClass = 	CLASS_SAILOR,
		.trainerSprite = 	TRAINER_SPRITE_SAILOR,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Sailor_PreBattle_1,
		.playerWinText = 	sFrontierText_Sailor_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Sailor_PlayerLose_1,
	},
	{
		.owNum = 			OW_SAILOR,
		.trainerClass = 	CLASS_SAILOR,
		.trainerSprite = 	TRAINER_SPRITE_SAILOR,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Sailor_PreBattle_2,
		.playerWinText = 	sFrontierText_Sailor_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Sailor_PlayerLose_2,
	},
	{
		.owNum = 			OW_SAILOR,
		.trainerClass = 	CLASS_SAILOR,
		.trainerSprite = 	TRAINER_SPRITE_SAILOR,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Sailor_PreBattle_3,
		.playerWinText = 	sFrontierText_Sailor_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Sailor_PlayerLose_3,
	},
	{
		.owNum = 			OW_CAMPER,
		.trainerClass = 	CLASS_CAMPER,
		.trainerSprite = 	TRAINER_SPRITE_CAMPER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Camper_PreBattle_1,
		.playerWinText = 	sFrontierText_Camper_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Camper_PlayerLose_1,
	},
	{
		.owNum = 			OW_CAMPER,
		.trainerClass = 	CLASS_CAMPER,
		.trainerSprite = 	TRAINER_SPRITE_CAMPER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Camper_PreBattle_2,
		.playerWinText = 	sFrontierText_Camper_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Camper_PlayerLose_2,
	},
	{
		.owNum = 			OW_CAMPER,
		.trainerClass = 	CLASS_CAMPER,
		.trainerSprite = 	TRAINER_SPRITE_CAMPER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Camper_PreBattle_3,
		.playerWinText = 	sFrontierText_Camper_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Camper_PlayerLose_3,
	},
	{
		.owNum = 			OW_PICNICKER,
		.trainerClass = 	CLASS_PICNICKER,
		.trainerSprite = 	TRAINER_SPRITE_PICNICKER,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Picnicker_PreBattle_1,
		.playerWinText = 	sFrontierText_Picnicker_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Picnicker_PlayerLose_1,
	},
	{
		.owNum = 			OW_PICNICKER,
		.trainerClass = 	CLASS_PICNICKER,
		.trainerSprite = 	TRAINER_SPRITE_PICNICKER,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Picnicker_PreBattle_2,
		.playerWinText = 	sFrontierText_Picnicker_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Picnicker_PlayerLose_2,
	},
	{
		.owNum = 			OW_PICNICKER,
		.trainerClass = 	CLASS_PICNICKER,
		.trainerSprite = 	TRAINER_SPRITE_PICNICKER,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Picnicker_PreBattle_3,
		.playerWinText = 	sFrontierText_Picnicker_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Picnicker_PlayerLose_3,
	},
	{
		.owNum = 			OW_POKEMANIAC,
		.trainerClass = 	CLASS_POKEMANIAC,
		.trainerSprite = 	TRAINER_SPRITE_POKEMANIAC,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Pokemaniac_PreBattle_1,
		.playerWinText = 	sFrontierText_Pokemaniac_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Pokemaniac_PlayerLose_1,
	},
	{
		.owNum = 			OW_POKEMANIAC,
		.trainerClass = 	CLASS_POKEMANIAC,
		.trainerSprite = 	TRAINER_SPRITE_POKEMANIAC,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Pokemaniac_PreBattle_2,
		.playerWinText = 	sFrontierText_Pokemaniac_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Pokemaniac_PlayerLose_2,
	},
	{
		.owNum = 			OW_POKEMANIAC,
		.trainerClass = 	CLASS_POKEMANIAC,
		.trainerSprite = 	TRAINER_SPRITE_POKEMANIAC,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Pokemaniac_PreBattle_3,
		.playerWinText = 	sFrontierText_Pokemaniac_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Pokemaniac_PlayerLose_3,
	},
	{
		.owNum = 			OW_SUPER_NERD,
		.trainerClass = 	CLASS_SUPER_NERD,
		.trainerSprite = 	TRAINER_SPRITE_SUPERNERD,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Supernerd_PreBattle_1,
		.playerWinText = 	sFrontierText_Supernerd_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Supernerd_PlayerLose_1,
	},
	{
		.owNum = 			OW_SUPER_NERD,
		.trainerClass = 	CLASS_SUPER_NERD,
		.trainerSprite = 	TRAINER_SPRITE_SUPERNERD,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Supernerd_PreBattle_2,
		.playerWinText = 	sFrontierText_Supernerd_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Supernerd_PlayerLose_2,
	},
	{
		.owNum = 			OW_SUPER_NERD,
		.trainerClass = 	CLASS_SUPER_NERD,
		.trainerSprite = 	TRAINER_SPRITE_SUPERNERD,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Supernerd_PreBattle_3,
		.playerWinText = 	sFrontierText_Supernerd_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Supernerd_PlayerLose_3,
	},
	{
		.owNum = 			OW_HIKER,
		.trainerClass = 	CLASS_HIKER,
		.trainerSprite = 	TRAINER_SPRITE_HIKER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Hiker_PreBattle_1,
		.playerWinText = 	sFrontierText_Hiker_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Hiker_PlayerLose_1,
	},
	{
		.owNum = 			OW_HIKER,
		.trainerClass = 	CLASS_HIKER,
		.trainerSprite = 	TRAINER_SPRITE_HIKER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Hiker_PreBattle_2,
		.playerWinText = 	sFrontierText_Hiker_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Hiker_PlayerLose_2,
	},
	{
		.owNum = 			OW_HIKER,
		.trainerClass = 	CLASS_HIKER,
		.trainerSprite = 	TRAINER_SPRITE_HIKER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Hiker_PreBattle_3,
		.playerWinText = 	sFrontierText_Hiker_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Hiker_PlayerLose_3,
	},
	{
		.owNum = 			OW_BIKER,
		.trainerClass = 	CLASS_BIKER,
		.trainerSprite = 	TRAINER_SPRITE_BIKER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Biker_PreBattle_1,
		.playerWinText = 	sFrontierText_Biker_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Biker_PlayerLose_1,
	},
	{
		.owNum = 			OW_BIKER,
		.trainerClass = 	CLASS_BIKER,
		.trainerSprite = 	TRAINER_SPRITE_BIKER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Biker_PreBattle_2,
		.playerWinText = 	sFrontierText_Biker_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Biker_PlayerLose_2,
	},
	{
		.owNum = 			OW_BIKER,
		.trainerClass = 	CLASS_BIKER,
		.trainerSprite = 	TRAINER_SPRITE_BIKER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Biker_PreBattle_3,
		.playerWinText = 	sFrontierText_Biker_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Biker_PlayerLose_3,
	},
	{
		.owNum = 			OW_CYCLIST_M,
		.trainerClass = 	CLASS_CYCLIST,
		.trainerSprite = 	TRAINER_SPRITE_CYCLIST_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Cyclist_PreBattle_1,
		.playerWinText = 	sFrontierText_Cyclist_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Cyclist_PlayerLose_1,
	},
	{
		.owNum = 			OW_CYCLIST_F,
		.trainerClass = 	CLASS_CYCLIST,
		.trainerSprite = 	TRAINER_SPRITE_CYCLIST_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Cyclist_PreBattle_2,
		.playerWinText = 	sFrontierText_Cyclist_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Cyclist_PlayerLose_2,
	},
	{
		.owNum = 			OW_CYCLIST_M,
		.trainerClass = 	CLASS_CYCLIST,
		.trainerSprite = 	TRAINER_SPRITE_CYCLIST_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Cyclist_PreBattle_3,
		.playerWinText = 	sFrontierText_Cyclist_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Cyclist_PlayerLose_3,
	},
	{
		.owNum = 			OW_WORKER,
		.trainerClass = 	CLASS_WORKER,
		.trainerSprite = 	TRAINER_SPRITE_WORKER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Worker_PreBattle_1,
		.playerWinText = 	sFrontierText_Worker_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Worker_PlayerLose_1,
	},
	{
		.owNum = 			OW_WORKER,
		.trainerClass = 	CLASS_WORKER,
		.trainerSprite = 	TRAINER_SPRITE_WORKER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Worker_PreBattle_2,
		.playerWinText = 	sFrontierText_Worker_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Worker_PlayerLose_2,
	},
	{
		.owNum = 			OW_WORKER,
		.trainerClass = 	CLASS_WORKER,
		.trainerSprite = 	TRAINER_SPRITE_WORKER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Worker_PreBattle_3,
		.playerWinText = 	sFrontierText_Worker_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Worker_PlayerLose_3,
	},
	{
		.owNum = 			OW_FISHERMAN,
		.trainerClass = 	CLASS_FISHERMAN,
		.trainerSprite = 	TRAINER_SPRITE_FISHERMAN,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Fisherman_PreBattle_1,
		.playerWinText = 	sFrontierText_Fisherman_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Fisherman_PlayerLose_1,
	},
	{
		.owNum = 			OW_FISHERMAN,
		.trainerClass = 	CLASS_FISHERMAN,
		.trainerSprite = 	TRAINER_SPRITE_FISHERMAN,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Fisherman_PreBattle_2,
		.playerWinText = 	sFrontierText_Fisherman_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Fisherman_PlayerLose_2,
	},
	{
		.owNum = 			OW_FISHERMAN,
		.trainerClass = 	CLASS_FISHERMAN,
		.trainerSprite = 	TRAINER_SPRITE_FISHERMAN,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Fisherman_PreBattle_3,
		.playerWinText = 	sFrontierText_Fisherman_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Fisherman_PlayerLose_3,
	},
	{
		.owNum = 			OW_ROUGHNECK,
		.trainerClass = 	CLASS_ROUGHNECK,
		.trainerSprite = 	TRAINER_SPRITE_ROUGHNECK,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Roughneck_PreBattle_1,
		.playerWinText = 	sFrontierText_Roughneck_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Roughneck_PlayerLose_1,
	},
	{
		.owNum = 			OW_ROUGHNECK,
		.trainerClass = 	CLASS_ROUGHNECK,
		.trainerSprite = 	TRAINER_SPRITE_ROUGHNECK,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Roughneck_PreBattle_2,
		.playerWinText = 	sFrontierText_Roughneck_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Roughneck_PlayerLose_2,
	},
	{
		.owNum = 			OW_ROUGHNECK,
		.trainerClass = 	CLASS_ROUGHNECK,
		.trainerSprite = 	TRAINER_SPRITE_ROUGHNECK,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Roughneck_PreBattle_3,
		.playerWinText = 	sFrontierText_Roughneck_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Roughneck_PlayerLose_3,
	},
	{
		.owNum = 			OW_BEAUTY,
		.trainerClass = 	CLASS_BEAUTY,
		.trainerSprite = 	TRAINER_SPRITE_BEAUTY,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Beauty_PreBattle_1,
		.playerWinText = 	sFrontierText_Beauty_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Beauty_PlayerLose_1,
	},
	{
		.owNum = 			OW_BEAUTY,
		.trainerClass = 	CLASS_BEAUTY,
		.trainerSprite = 	TRAINER_SPRITE_BEAUTY,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Beauty_PreBattle_2,
		.playerWinText = 	sFrontierText_Beauty_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Beauty_PlayerLose_2,
	},
	{
		.owNum = 			OW_BEAUTY,
		.trainerClass = 	CLASS_BEAUTY,
		.trainerSprite = 	TRAINER_SPRITE_BEAUTY,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Beauty_PreBattle_3,
		.playerWinText = 	sFrontierText_Beauty_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Beauty_PlayerLose_3,
	},
	{
		.owNum = 			OW_PSYCHIC_M,
		.trainerClass = 	CLASS_PSYCHIC,
		.trainerSprite = 	TRAINER_SPRITE_PSYCHIC_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Psychic_PreBattle_1,
		.playerWinText = 	sFrontierText_Psychic_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Psychic_PlayerLose_1,
	},
	{
		.owNum = 			OW_PSYCHIC_M,
		.trainerClass = 	CLASS_PSYCHIC,
		.trainerSprite = 	TRAINER_SPRITE_PSYCHIC_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Psychic_PreBattle_2,
		.playerWinText = 	sFrontierText_Psychic_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Psychic_PlayerLose_2,
	},
	{
		.owNum = 			OW_PSYCHIC_M,
		.trainerClass = 	CLASS_PSYCHIC,
		.trainerSprite = 	TRAINER_SPRITE_PSYCHIC_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Psychic_PreBattle_3,
		.playerWinText = 	sFrontierText_Psychic_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Psychic_PlayerLose_3,
	},
	{
		.owNum = 			OW_PSYCHIC_F,
		.trainerClass = 	CLASS_PSYCHIC,
		.trainerSprite = 	TRAINER_SPRITE_PSYCHIC_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Psychic_PreBattle_4,
		.playerWinText = 	sFrontierText_Psychic_PlayerWin_4,
		.playerLoseText = 	sFrontierText_Psychic_PlayerLose_4,
	},
	{
		.owNum = 			OW_PSYCHIC_F,
		.trainerClass = 	CLASS_PSYCHIC,
		.trainerSprite = 	TRAINER_SPRITE_PSYCHIC_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Psychic_PreBattle_5,
		.playerWinText = 	sFrontierText_Psychic_PlayerWin_5,
		.playerLoseText = 	sFrontierText_Psychic_PlayerLose_5,
	},
	{
		.owNum = 			OW_PSYCHIC_F,
		.trainerClass = 	CLASS_PSYCHIC,
		.trainerSprite = 	TRAINER_SPRITE_PSYCHIC_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Psychic_PreBattle_6,
		.playerWinText = 	sFrontierText_Psychic_PlayerWin_6,
		.playerLoseText = 	sFrontierText_Psychic_PlayerLose_6,
	},
	{
		.owNum = 			OW_GUITARIST,
		.trainerClass = 	CLASS_GUITARIST,
		.trainerSprite = 	TRAINER_SPRITE_GUITARIST,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Guitarist_PreBattle_1,
		.playerWinText = 	sFrontierText_Guitarist_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Guitarist_PlayerLose_1,
	},
	{
		.owNum = 			OW_GUITARIST,
		.trainerClass = 	CLASS_GUITARIST,
		.trainerSprite = 	TRAINER_SPRITE_GUITARIST,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Guitarist_PreBattle_2,
		.playerWinText = 	sFrontierText_Guitarist_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Guitarist_PlayerLose_2,
	},
	{
		.owNum = 			OW_GUITARIST,
		.trainerClass = 	CLASS_GUITARIST,
		.trainerSprite = 	TRAINER_SPRITE_GUITARIST,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Guitarist_PreBattle_3,
		.playerWinText = 	sFrontierText_Guitarist_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Guitarist_PlayerLose_3,
	},
	{
		.owNum = 			OW_DRAGON_TAMER,
		.trainerClass = 	CLASS_DRAGON_TAMER,
		.trainerSprite = 	TRAINER_SPRITE_DRAGON_TAMER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Tamer_PreBattle_1,
		.playerWinText = 	sFrontierText_Tamer_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Tamer_PlayerLose_1,
	},
	{
		.owNum = 			OW_DRAGON_TAMER,
		.trainerClass = 	CLASS_DRAGON_TAMER,
		.trainerSprite = 	TRAINER_SPRITE_DRAGON_TAMER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Tamer_PreBattle_2,
		.playerWinText = 	sFrontierText_Tamer_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Tamer_PlayerLose_2,
	},
	{
		.owNum = 			OW_DRAGON_TAMER,
		.trainerClass = 	CLASS_DRAGON_TAMER,
		.trainerSprite = 	TRAINER_SPRITE_DRAGON_TAMER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Tamer_PreBattle_3,
		.playerWinText = 	sFrontierText_Tamer_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Tamer_PlayerLose_3,
	},
	{
		.owNum = 			OW_BIRD_KEEPER,
		.trainerClass = 	CLASS_BIRD_KEEPER,
		.trainerSprite = 	TRAINER_SPRITE_BIRDKEEPER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Birdkeeper_PreBattle_1,
		.playerWinText = 	sFrontierText_Birdkeeper_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Birdkeeper_PlayerLose_1,
	},
	{
		.owNum = 			OW_BIRD_KEEPER,
		.trainerClass = 	CLASS_BIRD_KEEPER,
		.trainerSprite = 	TRAINER_SPRITE_BIRDKEEPER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Birdkeeper_PreBattle_2,
		.playerWinText = 	sFrontierText_Birdkeeper_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Birdkeeper_PlayerLose_2,
	},
	{
		.owNum = 			OW_BIRD_KEEPER,
		.trainerClass = 	CLASS_BIRD_KEEPER,
		.trainerSprite = 	TRAINER_SPRITE_BIRDKEEPER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Birdkeeper_PreBattle_3,
		.playerWinText = 	sFrontierText_Birdkeeper_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Birdkeeper_PlayerLose_3,
	},
	{
		.owNum = 			OW_KARATE_GUY,
		.trainerClass = 	CLASS_BLACK_BELT,
		.trainerSprite = 	TRAINER_SPRITE_BLACKBELT,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Blackbelt_PreBattle_1,
		.playerWinText = 	sFrontierText_Blackbelt_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Blackbelt_PlayerLose_1,
	},
	{
		.owNum = 			OW_KARATE_GUY,
		.trainerClass = 	CLASS_BLACK_BELT,
		.trainerSprite = 	TRAINER_SPRITE_BLACKBELT,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Blackbelt_PreBattle_2,
		.playerWinText = 	sFrontierText_Blackbelt_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Blackbelt_PlayerLose_2,
	},
	{
		.owNum = 			OW_KARATE_GUY,
		.trainerClass = 	CLASS_BLACK_BELT,
		.trainerSprite = 	TRAINER_SPRITE_BLACKBELT,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Blackbelt_PreBattle_3,
		.playerWinText = 	sFrontierText_Blackbelt_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Blackbelt_PlayerLose_3,
	},
	{
		.owNum = 			OW_SCIENTIST_MALE,
		.trainerClass = 	CLASS_SCIENTIST,
		.trainerSprite = 	TRAINER_SPRITE_SCIENTIST_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Scientist_PreBattle_1,
		.playerWinText = 	sFrontierText_Scientist_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Scientist_PlayerLose_1,
	},
	{
		.owNum = 			OW_SCIENTIST_MALE,
		.trainerClass = 	CLASS_SCIENTIST,
		.trainerSprite = 	TRAINER_SPRITE_SCIENTIST_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Scientist_PreBattle_2,
		.playerWinText = 	sFrontierText_Scientist_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Scientist_PlayerLose_2,
	},
	{
		.owNum = 			OW_SCIENTIST_MALE,
		.trainerClass = 	CLASS_SCIENTIST,
		.trainerSprite = 	TRAINER_SPRITE_SCIENTIST_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Scientist_PreBattle_3,
		.playerWinText = 	sFrontierText_Scientist_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Scientist_PlayerLose_3,
	},
	{
		.owNum = 			OW_SCIENTIST_F,
		.trainerClass = 	CLASS_SCIENTIST,
		.trainerSprite = 	TRAINER_SPRITE_SCIENTIST_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Scientist_PreBattle_4,
		.playerWinText = 	sFrontierText_Scientist_PlayerWin_4,
		.playerLoseText = 	sFrontierText_Scientist_PlayerLose_4,
	},
	{
		.owNum = 			OW_ACE_TRAINER_M,
		.trainerClass = 	CLASS_ACE_TRAINER,
		.trainerSprite = 	TRAINER_SPRITE_ACETRAINER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Acetrainer_PreBattle_1,
		.playerWinText = 	sFrontierText_Acetrainer_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Acetrainer_PlayerLose_1,
	},
	{
		.owNum = 			OW_ACE_TRAINER_M,
		.trainerClass = 	CLASS_ACE_TRAINER,
		.trainerSprite = 	TRAINER_SPRITE_ACETRAINER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Acetrainer_PreBattle_2,
		.playerWinText = 	sFrontierText_Acetrainer_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Acetrainer_PlayerLose_2,
	},
	{
		.owNum = 			OW_ACE_TRAINER_M,
		.trainerClass = 	CLASS_ACE_TRAINER,
		.trainerSprite = 	TRAINER_SPRITE_ACETRAINER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Acetrainer_PreBattle_3,
		.playerWinText = 	sFrontierText_Acetrainer_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Acetrainer_PlayerLose_3,
	},
	{
		.owNum = 			OW_ACE_TRAINER_F,
		.trainerClass = 	CLASS_ACE_TRAINER,
		.trainerSprite = 	TRAINER_SPRITE_ACETRAINER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Acetrainer_PreBattle_4,
		.playerWinText = 	sFrontierText_Acetrainer_PlayerWin_4,
		.playerLoseText = 	sFrontierText_Acetrainer_PlayerLose_4,
	},
	{
		.owNum = 			OW_ACE_TRAINER_F,
		.trainerClass = 	CLASS_ACE_TRAINER,
		.trainerSprite = 	TRAINER_SPRITE_ACETRAINER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Acetrainer_PreBattle_5,
		.playerWinText = 	sFrontierText_Acetrainer_PlayerWin_5,
		.playerLoseText = 	sFrontierText_Acetrainer_PlayerLose_5,
	},
	{
		.owNum = 			OW_ACE_TRAINER_F,
		.trainerClass = 	CLASS_ACE_TRAINER,
		.trainerSprite = 	TRAINER_SPRITE_ACETRAINER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Acetrainer_PreBattle_6,
		.playerWinText = 	sFrontierText_Acetrainer_PlayerWin_6,
		.playerLoseText = 	sFrontierText_Acetrainer_PlayerLose_6,
	},
	{
		.owNum = 			OW_GENTLEMAN,
		.trainerClass = 	CLASS_GENTLEMAN,
		.trainerSprite = 	TRAINER_SPRITE_GENTLEMAN,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Gentleman_PreBattle_1,
		.playerWinText = 	sFrontierText_Gentleman_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Gentleman_PlayerLose_1,
	},
	{
		.owNum = 			OW_GENTLEMAN,
		.trainerClass = 	CLASS_GENTLEMAN,
		.trainerSprite = 	TRAINER_SPRITE_GENTLEMAN,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Gentleman_PreBattle_2,
		.playerWinText = 	sFrontierText_Gentleman_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Gentleman_PlayerLose_2,
	},
	{
		.owNum = 			OW_GENTLEMAN,
		.trainerClass = 	CLASS_GENTLEMAN,
		.trainerSprite = 	TRAINER_SPRITE_GENTLEMAN,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Gentleman_PreBattle_3,
		.playerWinText = 	sFrontierText_Gentleman_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Gentleman_PlayerLose_3,
	},
	{
		.owNum = 			OW_TUBER_M,
		.trainerClass = 	CLASS_TUBER,
		.trainerSprite = 	TRAINER_SPRITE_TUBER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Tuber_PreBattle_1,
		.playerWinText = 	sFrontierText_Tuber_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Tuber_PlayerLose_1,
	},
	{
		.owNum = 			OW_TUBER_M,
		.trainerClass = 	CLASS_TUBER,
		.trainerSprite = 	TRAINER_SPRITE_TUBER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Tuber_PreBattle_2,
		.playerWinText = 	sFrontierText_Tuber_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Tuber_PlayerLose_2,
	},
	{
		.owNum = 			OW_TUBER_M,
		.trainerClass = 	CLASS_TUBER,
		.trainerSprite = 	TRAINER_SPRITE_TUBER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Tuber_PreBattle_3,
		.playerWinText = 	sFrontierText_Tuber_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Tuber_PlayerLose_3,
	},
	{
		.owNum = 			OW_TUBER_F,
		.trainerClass = 	CLASS_TUBER,
		.trainerSprite = 	TRAINER_SPRITE_TUBER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Tuber_PreBattle_4,
		.playerWinText = 	sFrontierText_Tuber_PlayerWin_4,
		.playerLoseText = 	sFrontierText_Tuber_PlayerLose_4,
	},
	{
		.owNum = 			OW_TUBER_F,
		.trainerClass = 	CLASS_TUBER,
		.trainerSprite = 	TRAINER_SPRITE_TUBER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Tuber_PreBattle_5,
		.playerWinText = 	sFrontierText_Tuber_PlayerWin_5,
		.playerLoseText = 	sFrontierText_Tuber_PlayerLose_5,
	},
	{
		.owNum = 			OW_TUBER_F,
		.trainerClass = 	CLASS_TUBER,
		.trainerSprite = 	TRAINER_SPRITE_TUBER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Tuber_PreBattle_6,
		.playerWinText = 	sFrontierText_Tuber_PlayerWin_6,
		.playerLoseText = 	sFrontierText_Tuber_PlayerLose_6,
	},
	{
		.owNum = 			OW_BREEDER_M,
		.trainerClass = 	CLASS_PKMN_BREEDER,
		.trainerSprite = 	TRAINER_SPRITE_PKMN_BREEDER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Pkmnbreeder_PreBattle_1,
		.playerWinText = 	sFrontierText_Pkmnbreeder_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Pkmnbreeder_PlayerLose_1,
	},
	{
		.owNum = 			OW_BREEDER_M,
		.trainerClass = 	CLASS_PKMN_BREEDER,
		.trainerSprite = 	TRAINER_SPRITE_PKMN_BREEDER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Pkmnbreeder_PreBattle_2,
		.playerWinText = 	sFrontierText_Pkmnbreeder_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Pkmnbreeder_PlayerLose_2,
	},
	{
		.owNum = 			OW_BREEDER_M,
		.trainerClass = 	CLASS_PKMN_BREEDER,
		.trainerSprite = 	TRAINER_SPRITE_PKMN_BREEDER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Pkmnbreeder_PreBattle_3,
		.playerWinText = 	sFrontierText_Pkmnbreeder_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Pkmnbreeder_PlayerLose_3,
	},
	{
		.owNum = 			OW_BREEDER_F,
		.trainerClass = 	CLASS_PKMN_BREEDER,
		.trainerSprite = 	TRAINER_SPRITE_PKMN_BREEDER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Pkmnbreeder_PreBattle_4,
		.playerWinText = 	sFrontierText_Pkmnbreeder_PlayerWin_4,
		.playerLoseText = 	sFrontierText_Pkmnbreeder_PlayerLose_4,
	},
	{
		.owNum = 			OW_BREEDER_F,
		.trainerClass = 	CLASS_PKMN_BREEDER,
		.trainerSprite = 	TRAINER_SPRITE_PKMN_BREEDER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Pkmnbreeder_PreBattle_5,
		.playerWinText = 	sFrontierText_Pkmnbreeder_PlayerWin_5,
		.playerLoseText = 	sFrontierText_Pkmnbreeder_PlayerLose_5,
	},
	{
		.owNum = 			OW_BREEDER_F,
		.trainerClass = 	CLASS_PKMN_BREEDER,
		.trainerSprite = 	TRAINER_SPRITE_PKMN_BREEDER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Pkmnbreeder_PreBattle_6,
		.playerWinText = 	sFrontierText_Pkmnbreeder_PlayerWin_6,
		.playerLoseText = 	sFrontierText_Pkmnbreeder_PlayerLose_6,
	},
	{
		.owNum = 			OW_RANGER_M,
		.trainerClass = 	CLASS_PKMN_RANGER,
		.trainerSprite = 	TRAINER_SPRITE_PKMNRANGER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Pkmnranger_PreBattle_1,
		.playerWinText = 	sFrontierText_Pkmnranger_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Pkmnranger_PlayerLose_1,
	},
	{
		.owNum = 			OW_RANGER_M,
		.trainerClass = 	CLASS_PKMN_RANGER,
		.trainerSprite = 	TRAINER_SPRITE_PKMNRANGER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Pkmnranger_PreBattle_2,
		.playerWinText = 	sFrontierText_Pkmnranger_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Pkmnranger_PlayerLose_2,
	},
	{
		.owNum = 			OW_RANGER_M,
		.trainerClass = 	CLASS_PKMN_RANGER,
		.trainerSprite = 	TRAINER_SPRITE_PKMNRANGER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Pkmnranger_PreBattle_3,
		.playerWinText = 	sFrontierText_Pkmnranger_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Pkmnranger_PlayerLose_3,
	},
	{
		.owNum = 			OW_RANGER_F,
		.trainerClass = 	CLASS_PKMN_RANGER,
		.trainerSprite = 	TRAINER_SPRITE_PKMNRANGER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Pkmnranger_PreBattle_4,
		.playerWinText = 	sFrontierText_Pkmnranger_PlayerWin_4,
		.playerLoseText = 	sFrontierText_Pkmnranger_PlayerLose_4,
	},
	{
		.owNum = 			OW_RANGER_F,
		.trainerClass = 	CLASS_PKMN_RANGER,
		.trainerSprite = 	TRAINER_SPRITE_PKMNRANGER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Pkmnranger_PreBattle_5,
		.playerWinText = 	sFrontierText_Pkmnranger_PlayerWin_5,
		.playerLoseText = 	sFrontierText_Pkmnranger_PlayerLose_5,
	},
	{
		.owNum = 			OW_RANGER_F,
		.trainerClass = 	CLASS_PKMN_RANGER,
		.trainerSprite = 	TRAINER_SPRITE_PKMNRANGER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Pkmnranger_PreBattle_6,
		.playerWinText = 	sFrontierText_Pkmnranger_PlayerWin_6,
		.playerLoseText = 	sFrontierText_Pkmnranger_PlayerLose_6,
	},
	{
		.owNum = 			OW_AROMA_LADY,
		.trainerClass = 	CLASS_AROMA_LADY,
		.trainerSprite = 	TRAINER_SPRITE_AROMALADY,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Aromalady_PreBattle_1,
		.playerWinText = 	sFrontierText_Aromalady_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Aromalady_PlayerLose_1,
	},
	{
		.owNum = 			OW_AROMA_LADY,
		.trainerClass = 	CLASS_AROMA_LADY,
		.trainerSprite = 	TRAINER_SPRITE_AROMALADY,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Aromalady_PreBattle_2,
		.playerWinText = 	sFrontierText_Aromalady_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Aromalady_PlayerLose_2,
	},
	{
		.owNum = 			OW_AROMA_LADY,
		.trainerClass = 	CLASS_AROMA_LADY,
		.trainerSprite = 	TRAINER_SPRITE_AROMALADY,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Aromalady_PreBattle_3,
		.playerWinText = 	sFrontierText_Aromalady_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Aromalady_PlayerLose_3,
	},
	{
		.owNum = 			OW_RUIN_MANIAC,
		.trainerClass = 	CLASS_RUIN_MANIAC,
		.trainerSprite = 	TRAINER_SPRITE_RUINMANIAC,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Ruinmaniac_PreBattle_1,
		.playerWinText = 	sFrontierText_Ruinmaniac_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Ruinmaniac_PlayerLose_1,
	},
	{
		.owNum = 			OW_RUIN_MANIAC,
		.trainerClass = 	CLASS_RUIN_MANIAC,
		.trainerSprite = 	TRAINER_SPRITE_RUINMANIAC,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Ruinmaniac_PreBattle_2,
		.playerWinText = 	sFrontierText_Ruinmaniac_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Ruinmaniac_PlayerLose_2,
	},
	{
		.owNum = 			OW_RUIN_MANIAC,
		.trainerClass = 	CLASS_RUIN_MANIAC,
		.trainerSprite = 	TRAINER_SPRITE_RUINMANIAC,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Ruinmaniac_PreBattle_3,
		.playerWinText = 	sFrontierText_Ruinmaniac_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Ruinmaniac_PlayerLose_3,
	},
	{
		.owNum = 			OW_RICH_BOY,
		.trainerClass = 	CLASS_RICH_BOY,
		.trainerSprite = 	TRAINER_SPRITE_RICHBOY,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Richboy_PreBattle_1,
		.playerWinText = 	sFrontierText_Richboy_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Richboy_PlayerLose_1,
	},
	{
		.owNum = 			OW_RICH_BOY,
		.trainerClass = 	CLASS_RICH_BOY,
		.trainerSprite = 	TRAINER_SPRITE_RICHBOY,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Richboy_PreBattle_2,
		.playerWinText = 	sFrontierText_Richboy_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Richboy_PlayerLose_2,
	},
	{
		.owNum = 			OW_RICH_BOY,
		.trainerClass = 	CLASS_RICH_BOY,
		.trainerSprite = 	TRAINER_SPRITE_RICHBOY,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Richboy_PreBattle_3,
		.playerWinText = 	sFrontierText_Richboy_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Richboy_PlayerLose_3,
	},
	{
		.owNum = 			OW_RICH_GIRL,
		.trainerClass = 	CLASS_RICH_GIRL,
		.trainerSprite = 	TRAINER_SPRITE_RICHGIRL,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Richgirl_PreBattle_1,
		.playerWinText = 	sFrontierText_Richgirl_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Richgirl_PlayerLose_1,
	},
	{
		.owNum = 			OW_RICH_GIRL,
		.trainerClass = 	CLASS_RICH_GIRL,
		.trainerSprite = 	TRAINER_SPRITE_RICHGIRL,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Richgirl_PreBattle_2,
		.playerWinText = 	sFrontierText_Richgirl_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Richgirl_PlayerLose_2,
	},
	{
		.owNum = 			OW_RICH_GIRL,
		.trainerClass = 	CLASS_RICH_GIRL,
		.trainerSprite = 	TRAINER_SPRITE_RICHGIRL,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Richgirl_PreBattle_3,
		.playerWinText = 	sFrontierText_Richgirl_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Richgirl_PlayerLose_3,
	},
	{
		.owNum = 			OW_PAINTER,
		.trainerClass = 	CLASS_PAINTER,
		.trainerSprite = 	TRAINER_SPRITE_PAINTER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Painter_PreBattle_1,
		.playerWinText = 	sFrontierText_Painter_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Painter_PlayerLose_1,
	},
	{
		.owNum = 			OW_PAINTER,
		.trainerClass = 	CLASS_PAINTER,
		.trainerSprite = 	TRAINER_SPRITE_PAINTER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Painter_PreBattle_2,
		.playerWinText = 	sFrontierText_Painter_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Painter_PlayerLose_2,
	},
	{
		.owNum = 			OW_PAINTER,
		.trainerClass = 	CLASS_PAINTER,
		.trainerSprite = 	TRAINER_SPRITE_PAINTER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Painter_PreBattle_3,
		.playerWinText = 	sFrontierText_Painter_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Painter_PlayerLose_3,
	},
	{
		.owNum = 			OW_WAITER,
		.trainerClass = 	CLASS_WAITER,
		.trainerSprite = 	TRAINER_SPRITE_WAITER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Waiter_PreBattle_1,
		.playerWinText = 	sFrontierText_Waiter_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Waiter_PlayerLose_1,
	},
	{
		.owNum = 			OW_WAITER,
		.trainerClass = 	CLASS_WAITER,
		.trainerSprite = 	TRAINER_SPRITE_WAITER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Waiter_PreBattle_2,
		.playerWinText = 	sFrontierText_Waiter_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Waiter_PlayerLose_2,
	},
	{
		.owNum = 			OW_WAITER,
		.trainerClass = 	CLASS_WAITER,
		.trainerSprite = 	TRAINER_SPRITE_WAITER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Waiter_PreBattle_3,
		.playerWinText = 	sFrontierText_Waiter_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Waiter_PlayerLose_3,
	},
	{
		.owNum = 			OW_WAITRESS,
		.trainerClass = 	CLASS_WAITRESS,
		.trainerSprite = 	TRAINER_SPRITE_WAITRESS,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Waitress_PreBattle_1,
		.playerWinText = 	sFrontierText_Waitress_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Waitress_PlayerLose_1,
	},
	{
		.owNum = 			OW_WAITRESS,
		.trainerClass = 	CLASS_WAITRESS,
		.trainerSprite = 	TRAINER_SPRITE_WAITRESS,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Waitress_PreBattle_2,
		.playerWinText = 	sFrontierText_Waitress_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Waitress_PlayerLose_2,
	},
	{
		.owNum = 			OW_WAITRESS,
		.trainerClass = 	CLASS_WAITRESS,
		.trainerSprite = 	TRAINER_SPRITE_WAITRESS,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Waitress_PreBattle_3,
		.playerWinText = 	sFrontierText_Waitress_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Waitress_PlayerLose_3,
	},
	{
		.owNum = 			OW_IDOL,
		.trainerClass = 	CLASS_IDOL,
		.trainerSprite = 	TRAINER_SPRITE_IDOL,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Idol_PreBattle_1,
		.playerWinText = 	sFrontierText_Idol_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Idol_PlayerLose_1,
	},
	{
		.owNum = 			OW_IDOL,
		.trainerClass = 	CLASS_IDOL,
		.trainerSprite = 	TRAINER_SPRITE_IDOL,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Idol_PreBattle_2,
		.playerWinText = 	sFrontierText_Idol_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Idol_PlayerLose_2,
	},
	{
		.owNum = 			OW_IDOL,
		.trainerClass = 	CLASS_IDOL,
		.trainerSprite = 	TRAINER_SPRITE_IDOL,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Idol_PreBattle_3,
		.playerWinText = 	sFrontierText_Idol_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Idol_PlayerLose_3,
	},
	{
		.owNum = 			OW_POKEKID,
		.trainerClass = 	CLASS_POKE_KID,
		.trainerSprite = 	TRAINER_SPRITE_POKE_KID,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Pokekid_PreBattle_1,
		.playerWinText = 	sFrontierText_Pokekid_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Pokekid_PlayerLose_1,
	},
	{
		.owNum = 			OW_POKEKID,
		.trainerClass = 	CLASS_POKE_KID,
		.trainerSprite = 	TRAINER_SPRITE_POKE_KID,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Pokekid_PreBattle_2,
		.playerWinText = 	sFrontierText_Pokekid_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Pokekid_PlayerLose_2,
	},
	{
		.owNum = 			OW_POKEKID,
		.trainerClass = 	CLASS_POKE_KID,
		.trainerSprite = 	TRAINER_SPRITE_POKE_KID,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Pokekid_PreBattle_3,
		.playerWinText = 	sFrontierText_Pokekid_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Pokekid_PlayerLose_3,
	},
	{
		.owNum = 			OW_JOGGER,
		.trainerClass = 	CLASS_JOGGER,
		.trainerSprite = 	TRAINER_SPRITE_JOGGER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Jogger_PreBattle_1,
		.playerWinText = 	sFrontierText_Jogger_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Jogger_PlayerLose_1,
	},
	{
		.owNum = 			OW_JOGGER,
		.trainerClass = 	CLASS_JOGGER,
		.trainerSprite = 	TRAINER_SPRITE_JOGGER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Jogger_PreBattle_2,
		.playerWinText = 	sFrontierText_Jogger_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Jogger_PlayerLose_2,
	},
	{
		.owNum = 			OW_JOGGER,
		.trainerClass = 	CLASS_JOGGER,
		.trainerSprite = 	TRAINER_SPRITE_JOGGER,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Jogger_PreBattle_3,
		.playerWinText = 	sFrontierText_Jogger_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Jogger_PlayerLose_3,
	},
	{
		.owNum = 			OW_RANCHER_M,
		.trainerClass = 	CLASS_RANCHER,
		.trainerSprite = 	TRAINER_SPRITE_RANCHER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Rancher_PreBattle_1,
		.playerWinText = 	sFrontierText_Rancher_PlayerWin_1,
		.playerLoseText = 	sFrontierText_Rancher_PlayerLose_1,
	},
	{
		.owNum = 			OW_RANCHER_M,
		.trainerClass = 	CLASS_RANCHER,
		.trainerSprite = 	TRAINER_SPRITE_RANCHER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Rancher_PreBattle_2,
		.playerWinText = 	sFrontierText_Rancher_PlayerWin_2,
		.playerLoseText = 	sFrontierText_Rancher_PlayerLose_2,
	},
	{
		.owNum = 			OW_RANCHER_M,
		.trainerClass = 	CLASS_RANCHER,
		.trainerSprite = 	TRAINER_SPRITE_RANCHER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.preBattleText = 	sFrontierText_Rancher_PreBattle_3,
		.playerWinText = 	sFrontierText_Rancher_PlayerWin_3,
		.playerLoseText = 	sFrontierText_Rancher_PlayerLose_3,
	},
	{
		.owNum = 			OW_RANCHER_F,
		.trainerClass = 	CLASS_RANCHER,
		.trainerSprite = 	TRAINER_SPRITE_RANCHER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Rancher_PreBattle_4,
		.playerWinText = 	sFrontierText_Rancher_PlayerWin_4,
		.playerLoseText = 	sFrontierText_Rancher_PlayerLose_4,
	},
	{
		.owNum = 			OW_RANCHER_F,
		.trainerClass = 	CLASS_RANCHER,
		.trainerSprite = 	TRAINER_SPRITE_RANCHER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Rancher_PreBattle_5,
		.playerWinText = 	sFrontierText_Rancher_PlayerWin_5,
		.playerLoseText = 	sFrontierText_Rancher_PlayerLose_5,
	},
	{
		.owNum = 			OW_RANCHER_F,
		.trainerClass = 	CLASS_RANCHER,
		.trainerSprite = 	TRAINER_SPRITE_RANCHER_F,
		.gender = 			BATTLE_TOWER_FEMALE,
		.preBattleText = 	sFrontierText_Rancher_PreBattle_6,
		.playerWinText = 	sFrontierText_Rancher_PlayerWin_6,
		.playerLoseText = 	sFrontierText_Rancher_PlayerLose_6,
	},
};

const struct SpecialBattleTowerTrainer gSpecialTowerTrainers[] =
{
	{
		.owNum = 			OW_SKELI,
		.trainerClass = 	CLASS_DEVELOPER,
		.trainerSprite = 	TRAINER_SPRITE_SKELI,
		.gender = 			BATTLE_TOWER_MALE,
		.name =				sTrainerName_Skeli,
		.preBattleText = 	sFrontierText_Skeli_PreBattle,
		.playerWinText = 	sFrontierText_Skeli_PlayerWin,
		.playerLoseText = 	sFrontierText_Skeli_PlayerLose,
		.regularSpreads =	gSpecialTowerSpread_Skeli,
		.middleCupSpreads = gMiddleCupTowerSpread_Skeli,
		.littleCupSpreads = gLittleCupTowerSpread_Skeli,
		.legendarySpreads = gLegendaryTowerSpread_Skeli,
		.regSpreadSize = 	ARRAY_COUNT(gSpecialTowerSpread_Skeli),
		.mcSpreadSize =		ARRAY_COUNT(gMiddleCupTowerSpread_Skeli),
		.lcSpreadSize =		ARRAY_COUNT(gLittleCupTowerSpread_Skeli),
		.legSpreadSize =	ARRAY_COUNT(gLegendaryTowerSpread_Skeli),
		.songId = 			BGM_BATTLE_LOR_BOSS,
		.isMonotype = 		FALSE,
	},
	{
		.owNum = 			OW_GOLCHE,
		.trainerClass = 	CLASS_DEVELOPER,
		.trainerSprite = 	TRAINER_SPRITE_GOLCHE,
		.gender = 			BATTLE_TOWER_MALE,
		.name =				sTrainerName_Golche,
		.preBattleText = 	sFrontierText_Golche_PreBattle,
		.playerWinText = 	sFrontierText_Golche_PlayerWin,
		.playerLoseText = 	sFrontierText_Golche_PlayerLose,
		.regularSpreads =	gSpecialTowerSpread_Golche,
		.middleCupSpreads = gMiddleCupTowerSpread_Golche,
		.littleCupSpreads = gLittleCupTowerSpread_Golche,
		//.legendarySpreads = gLegendaryTowerSpread_Golche,
		.regSpreadSize = 	ARRAY_COUNT(gSpecialTowerSpread_Golche),
		.mcSpreadSize =		ARRAY_COUNT(gMiddleCupTowerSpread_Golche),
		.lcSpreadSize =		ARRAY_COUNT(gLittleCupTowerSpread_Golche),
		//.legSpreadSize =	ARRAY_COUNT(gLegendaryTowerSpread_Golche),
		.songId = 			BGM_BATTLE_GALACTIC_BOSS,
		.isMonotype = 		FALSE,
	},
	{
		.owNum = 			OW_GAIL,
		.trainerClass = 	CLASS_PKMN_TRAINER_2,
		.trainerSprite = 	TRAINER_SPRITE_GAIL,
		.gender = 			BATTLE_TOWER_MALE,
		.name =				sTrainerName_Gail,
		.preBattleText = 	sFrontierText_Gail_PreBattle,
		.playerWinText = 	sFrontierText_Gail_PlayerWin,
		.playerLoseText = 	sFrontierText_Gail_PlayerLose,
		.regularSpreads =	gSpecialTowerSpread_Gail,
		.middleCupSpreads = gMiddleCupTowerSpread_Gail,
		.littleCupSpreads = gLittleCupTowerSpread_Gail,
		.legendarySpreads =	gLegendaryTowerSpread_Gail,
		.regSpreadSize = 	ARRAY_COUNT(gSpecialTowerSpread_Gail),
		.mcSpreadSize =		ARRAY_COUNT(gMiddleCupTowerSpread_Gail),
		.lcSpreadSize =		ARRAY_COUNT(gLittleCupTowerSpread_Gail),
		.legSpreadSize =	ARRAY_COUNT(gLegendaryTowerSpread_Gail),
		.songId = 			BGM_BATTLE_DIALGA_PALKIA,
		.isMonotype = 		FALSE,
	},
	{
		.owNum = 			OW_ACE_TRAINER_M,
		.trainerClass = 	CLASS_PKMN_TRAINER_2,
		.trainerSprite = 	TRAINER_SPRITE_ACETRAINER_M,
		.gender = 			BATTLE_TOWER_MALE,
		.name =				sTrainerName_Lixdel,
		.preBattleText = 	sFrontierText_Lixdel_PreBattle,
		.playerWinText = 	sFrontierText_Lixdel_PlayerWin,
		.playerLoseText = 	sFrontierText_Lixdel_PlayerLose,
		.regularSpreads =	gSpecialTowerSpread_Lixdel,
		.middleCupSpreads = gMiddleCupTowerSpread_Lixdel,
		.littleCupSpreads = gLittleCupTowerSpread_Lixdel,
		//.legendarySpreads = gLegendaryTowerSpread_Lixdel,
		.regSpreadSize = 	ARRAY_COUNT(gSpecialTowerSpread_Lixdel),
		.mcSpreadSize =		ARRAY_COUNT(gMiddleCupTowerSpread_Lixdel),
		.lcSpreadSize =		ARRAY_COUNT(gLittleCupTowerSpread_Lixdel),
		//.legSpreadSize =	ARRAY_COUNT(gLegendaryTowerSpread_Lixdel),
		.songId = 			BGM_BATTLE_CHAMPION,
		.isMonotype = 		FALSE,
	},
	{
		.owNum = 			OW_CYNTHIA,
		.trainerClass = 	CLASS_CHAMPION,
		.trainerSprite = 	TRAINER_SPRITE_CYNTHIA,
		.gender = 			BATTLE_TOWER_FEMALE,
		.name =				sTrainerName_Cynthia,
		.preBattleText = 	sFrontierText_Cynthia_PreBattle,
		.playerWinText = 	sFrontierText_Cynthia_PlayerWin,
		.playerLoseText = 	sFrontierText_Cynthia_PlayerLose,
		.regularSpreads =	gSpecialTowerSpread_Cynthia,
		.middleCupSpreads = gMiddleCupTowerSpread_Cynthia,
		.littleCupSpreads = gLittleCupTowerSpread_Cynthia,
		.legendarySpreads = gLegendaryTowerSpread_Cynthia,
		.regSpreadSize = 	ARRAY_COUNT(gSpecialTowerSpread_Cynthia),
		.mcSpreadSize =		ARRAY_COUNT(gMiddleCupTowerSpread_Cynthia),
		.lcSpreadSize =		ARRAY_COUNT(gLittleCupTowerSpread_Cynthia),
		.legSpreadSize = 	ARRAY_COUNT(gLegendaryTowerSpread_Cynthia),
		.songId = 			BGM_BATTLE_CYNTHIA,
		.isMonotype = 		FALSE,
	},
	{
		.owNum = 			OW_RED,
		.trainerClass = 	CLASS_PKMN_TRAINER_2,
		.trainerSprite = 	TRAINER_SPRITE_RED,
		.gender = 			BATTLE_TOWER_MALE,
		.name =				sTrainerName_Red,
		.preBattleText = 	sFrontierText_Red_PreBattle,
		.playerWinText = 	sFrontierText_Red_PlayerWin,
		.playerLoseText = 	sFrontierText_Red_PlayerLose,
		.regularSpreads =	gSpecialTowerSpread_Red,
		.middleCupSpreads = gMiddleCupTowerSpread_Red,
		.littleCupSpreads = gLittleCupTowerSpread_Red,
		.legendarySpreads = gLegendaryTowerSpread_Red,
		.regSpreadSize = 	ARRAY_COUNT(gSpecialTowerSpread_Red),
		.mcSpreadSize =		ARRAY_COUNT(gMiddleCupTowerSpread_Red),
		.lcSpreadSize =		ARRAY_COUNT(gLittleCupTowerSpread_Red),
		.legSpreadSize = 	ARRAY_COUNT(gLegendaryTowerSpread_Red),
		.songId = 			BGM_BATTLE_RED,
		.isMonotype = 		FALSE,
	},
	{
		.owNum = 			OW_BIG_MO,
		.trainerClass = 	CLASS_PKMN_TRAINER_2,
		.trainerSprite = 	TRAINER_SPRITE_BIG_MO,
		.gender = 			BATTLE_TOWER_MALE,
		.name =				sTrainerName_BigMo,
		.preBattleText = 	sFrontierText_BigMo_PreBattle,
		.playerWinText = 	sFrontierText_BigMo_PlayerWin,
		.playerLoseText = 	sFrontierText_BigMo_PlayerLose,
		.regularSpreads =	gSpecialTowerSpread_BigMo,
		.middleCupSpreads = gMiddleCupTowerSpread_BigMo,
		.littleCupSpreads = gLittleCupTowerSpread_BigMo,
		.legendarySpreads = gLegendaryTowerSpread_BigMo,
		.regSpreadSize = 	ARRAY_COUNT(gSpecialTowerSpread_BigMo),
		.mcSpreadSize =		ARRAY_COUNT(gMiddleCupTowerSpread_BigMo),
		.lcSpreadSize =		ARRAY_COUNT(gLittleCupTowerSpread_BigMo),
		.legSpreadSize =	ARRAY_COUNT(gLegendaryTowerSpread_BigMo),
		.songId = 			BGM_BATTLE_GYM_LEADER,
		.isMonotype = 		TRUE,
	},
	{
		.owNum = 			OW_GALAVAN,
		.trainerClass = 	CLASS_PKMN_TRAINER_2,
		.trainerSprite = 	TRAINER_SPRITE_GALAVAN,
		.gender = 			BATTLE_TOWER_MALE,
		.name =				sTrainerName_Galavan,
		.preBattleText = 	sFrontierText_Galavan_PreBattle,
		.playerWinText = 	sFrontierText_Galavan_PlayerWin,
		.playerLoseText = 	sFrontierText_Galavan_PlayerLose,
		.regularSpreads =	gSpecialTowerSpread_Galavan,
		.middleCupSpreads = gMiddleCupTowerSpread_Galavan,
		.littleCupSpreads = gLittleCupTowerSpread_Galavan,
		.legendarySpreads = gLegendaryTowerSpread_Galavan,
		.regSpreadSize = 	ARRAY_COUNT(gSpecialTowerSpread_Galavan),
		.mcSpreadSize =		ARRAY_COUNT(gMiddleCupTowerSpread_Galavan),
		.lcSpreadSize =		ARRAY_COUNT(gLittleCupTowerSpread_Galavan),
		.legSpreadSize =	ARRAY_COUNT(gLegendaryTowerSpread_Galavan),
		.songId = 			BGM_BATTLE_GYM_LEADER,
		.isMonotype = 		TRUE,
	},
	{
		.owNum = 			OW_CANDICE,
		.trainerClass = 	CLASS_LEADER,
		.trainerSprite = 	TRAINER_SPRITE_CANDICE,
		.gender = 			BATTLE_TOWER_FEMALE,
		.name =				sTrainerName_Candice,
		.preBattleText = 	sFrontierText_Candice_PreBattle,
		.playerWinText = 	sFrontierText_Candice_PlayerWin,
		.playerLoseText = 	sFrontierText_Candice_PlayerLose,
		.regularSpreads =	gSpecialTowerSpread_Candice,
		.middleCupSpreads = gMiddleCupTowerSpread_Candice,
		.littleCupSpreads = gLittleCupTowerSpread_Candice,
		.legendarySpreads = gLegendaryTowerSpread_Candice,
		.regSpreadSize = 	ARRAY_COUNT(gSpecialTowerSpread_Candice),
		.mcSpreadSize =		ARRAY_COUNT(gMiddleCupTowerSpread_Candice),
		.lcSpreadSize =		ARRAY_COUNT(gLittleCupTowerSpread_Candice),
		.legSpreadSize =	ARRAY_COUNT(gLegendaryTowerSpread_Candice),
		.songId = 			BGM_BATTLE_SINNOH_GYM_LEADER,
		.isMonotype = 		TRUE,
	},
	{
		.owNum = 			OW_GIOVANNI,
		.trainerClass = 	CLASS_BOSS,
		.trainerSprite = 	TRAINER_SPRITE_GIOVANNI,
		.gender = 			BATTLE_TOWER_MALE,
		.name =				sTrainerName_Giovanni,
		.preBattleText = 	sFrontierText_Giovanni_PreBattle,
		.playerWinText = 	sFrontierText_Giovanni_PlayerWin,
		.playerLoseText = 	sFrontierText_Giovanni_PlayerLose,
		.regularSpreads =	gSpecialTowerSpread_Giovanni,
		.middleCupSpreads = gMiddleCupTowerSpread_Giovanni,
		.littleCupSpreads = gLittleCupTowerSpread_Giovanni,
		.legendarySpreads = gLegendaryTowerSpread_Giovanni,
		.regSpreadSize = 	ARRAY_COUNT(gSpecialTowerSpread_Giovanni),
		.mcSpreadSize =		ARRAY_COUNT(gMiddleCupTowerSpread_Giovanni),
		.lcSpreadSize =		ARRAY_COUNT(gLittleCupTowerSpread_Giovanni),
		.legSpreadSize =	ARRAY_COUNT(gLegendaryTowerSpread_Giovanni),
		.songId = 			BGM_BATTLE_TEAM_ROCKET_GRUNT,
		.isMonotype = 		TRUE,
	},
};

const struct FrontierBrain gFrontierBrains[] =
{
	[0] =
		{
			.trainerClass = 	CLASS_FRONTIER_BRAIN,
			.trainerSprite = 	TRAINER_SPRITE_PALMER,
			.gender = 			BATTLE_TOWER_MALE,
			.name =				sTrainerName_Palmer,
			.spreads =			gSpecialTowerSpread_Palmer1,
			.spreadSize = 		ARRAY_COUNT(gSpecialTowerSpread_Palmer1),
		},
	[1] =
		{
			.trainerClass = 	CLASS_FRONTIER_BRAIN,
			.trainerSprite = 	TRAINER_SPRITE_PALMER,
			.gender = 			BATTLE_TOWER_MALE,
			.name =				sTrainerName_Palmer,
			.spreads =			gSpecialTowerSpread_Palmer2,
			.spreadSize = 		ARRAY_COUNT(gSpecialTowerSpread_Palmer2),
		},
};

#else
	const struct BattleTowerTrainer gTowerTrainers[] =
	{
		{
			.owNum = 			OW_YOUNGSTER,
			.trainerClass = 	CLASS_YOUNGSTER,
			.trainerSprite = 	TRAINER_SPRITE_YOUNGSTER,
			.gender = 			BATTLE_TOWER_MALE,
			.preBattleText = 	sFrontierText_Youngster_PreBattle_1,
			.playerWinText = 	sFrontierText_Youngster_PlayerWin_1,
			.playerLoseText = 	sFrontierText_Youngster_PlayerLose_1,
		},
	};

	const struct SpecialBattleTowerTrainer gSpecialTowerTrainers[] =
	{
	};
	
	const struct FrontierBrain gFrontierBrains[] =
	{
	};
#endif

const u16 gNumTowerTrainers = ARRAY_COUNT(gTowerTrainers);
const u16 gNumSpecialTowerTrainers = ARRAY_COUNT(gSpecialTowerTrainers);