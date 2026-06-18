#include "defines.h"
#include "../include/berry.h"
#include "../include/item.h"
#include "../include/item_menu.h"
#include "../include/list_menu.h"
#include "../include/random.h"

#include "../include/constants/songs.h"
#include "../include/new/Vanilla_functions.h"

extern u32 __divsi3(u32 dividend, u32 divisor);

struct BerryTreeHack
{
    u8 stage:7;
    u8 berryId;
    u8 monthPlanted;
    u8 dayPlanted;
    u8 hourPlanted;
    u8 unused;
};

#define gBerryTrees ((struct BerryTreeHack *) 0x203C000)
#define BERRY_TREE_COUNT 128 // Adjustable
#define minutesPerStage 180  // 3 hoous by stage

void GetTreeData(void)
{
    // Check if baghack is active
    if (VarGet(Var800F) == 0x7E)
        return;

    u16 TreeID = VarGet(Var8002);
    struct BerryTreeHack *tree = &gBerryTrees[TreeID];

     // Decrypt the packed byte into separate values
    u8 packed = tree->stage;
    VarSet(Var8005, packed >> 5);          // Stage
    VarSet(Var8006, (packed >> 1) & 0xF);  // Berry ID
    VarSet(Var8007, packed & 0x1);         // Watered flag
    
    // Copy remaining bytes
    VarSet(Var8008, tree->berryId);
    VarSet(Var800A, tree->monthPlanted);
    VarSet(Var800B, tree->dayPlanted);
    VarSet(Var8009, tree->hourPlanted);

    // Skip if stage is 0
    if (VarGet(Var8005) == 0)
        return;

    u8 planted_month = VarGet(Var800A);
    u8 current_month = gClock.month;
    
    if (current_month != planted_month) {
        int month_diff = current_month - planted_month;
        if (month_diff != 1 && month_diff != -11) {
            // More than 1 month difference
            VarSet(Var8005, 5);
            VarSet(Var8007, 0); // Reset watering
            return;
        }
        
        // Exactly 1 month difference
        u8 current_day = gClock.day;
        u8 planted_day = VarGet(Var800B);
        int day_diff = current_day - planted_day + 30;
        
        if (day_diff >= 3) {
            VarSet(Var8005, 5);
            VarSet(Var8007, 0);
            return;
        }

    } else {
        // Same month
        u8 current_day = gClock.day;
        u8 planted_day = VarGet(Var800B);
        int day_diff = current_day - planted_day;
        
        if (day_diff >= 3) {
            VarSet(Var8005, 5);
            VarSet(Var8007, 0);
            return;
        }
    }

    // Check hours passed
    u8 current_hour = gClock.hour;
    u8 planted_hour = VarGet(Var8009);
    u8 current_day = gClock.day;
    u8 planted_day = VarGet(Var800B);
    int day_diff = current_day - planted_day;
    
    int hours_passed = current_hour - planted_hour + (day_diff * 24);
    
    // Calculate growth stage
    u8 stage = VarGet(Var8005);
    u8 time_per_stage = 12; // 12 hours between stages
    u8 new_stage = stage;
    
    for (int i = 0; i < 5; i++) {
        if (hours_passed < time_per_stage * (i + 1)) {
            new_stage = i + 1;
            break;
        }
    }
    
    if (new_stage > 5) new_stage = 5;
    
    if (new_stage != stage) {
        VarSet(Var8005, new_stage);
        if (new_stage == 5) {
            VarSet(Var8007, 0); // Reset watering when fully grown
        }
    }
}

void SetTreeData(void)
{
    if (Var800F != 0x7E)
        return;

    u16 TreeId = VarGet(Var8002);
    struct BerryTreeHack *tree = &gBerryTrees[TreeId];

    u8 packed = (VarGet(Var8005) << 5) | 
                (VarGet(Var8006) << 1) | 
                VarGet(Var8007);

    tree->stage = packed;
    tree->berryId = VarGet(Var8008);
    tree->monthPlanted  = VarGet(Var8009);
    tree->dayPlanted = VarGet(Var800A);
    tree->hourPlanted   = VarGet(Var800B);
}

void CalculateYield(void)
{
    u16 max = VarGet(Var8000);
    u16 min = VarGet(Var8001);
    u16 watered_count = VarGet(Var8007);

    if (watered_count > 0)
        watered_count--;

    u16 randVal = Random();
    u16 range = max - min;
    u16 bonus = range * watered_count;

    randVal = umodsi(randVal, 4) + bonus;
    u16 yield = __divsi3(randVal, 4) + min;
    
    VarSet(Var800D, yield);
}

void SetPlantingTime(void)
{
    Var8009 = gClock.hour;
    Var800A = gClock.month;
    Var800B = gClock.day;
}