/* Custom File. Give credits if used!
   ~ 1RWT16KU1D
*/

// For Terastallization

#include "../pokemon.h"
#include "../global.h"

// Battle Scripts
extern u8 BattleScript_Terastallize[];

// Custom Functions
extern bool8 IsTerastallized(u8 bank);
extern u8 GetTeraType(u8 bank);
extern void ChangeTeraTypeInOW(void);
extern u8* DoTerastallize(u8 bank);
extern void GetTeraTypeInOW(void);
extern bool8 CanTerastallize(u8 bank);
bool8 ShouldAIDelayTerastallization(u8 bankAtk, u8 bankDef, u16 move, bool8 optimizeAndLookAtTeraPotential, bool8 runDamageCalcs);
bool8 CalculateMoveKnocksOutXHitsFresh(u16 move, u8 bankAtk, u8 bankDef, u8 numHits);
bool8 TerastalEnabled(u8 bank);
u8 GetRandomTeraType(void);
#ifdef SHOW_TERA_TYPE_ICON_ON_SUMMARY_SCREEN
void TeraIconSummaryScreen(void);
#endif

// Visuals
extern void FadeBankPaletteForTera(u8 bank, u16 paletteOffset);
extern const u16 gTeraBlendColors[];

/* NEXT TO BE IMPLEMENTED LATER*/
// extern void AddTeraCrownSprite(u8 bank, u8 teraType, u8 x, u8 y);

// Sprite Data (TO BE IMPLEMENTED LATER)

extern const u8 sText_Normal[];
extern const u8 sText_Fighting[];
extern const u8 sText_Flying[];
extern const u8 sText_Poison[];
extern const u8 sText_Ground[];
extern const u8 sText_Rock[];
extern const u8 sText_Bug[];
extern const u8 sText_Ghost[];
extern const u8 sText_Steel[];
extern const u8 sText_Fire[];
extern const u8 sText_Water[];
extern const u8 sText_Grass[];
extern const u8 sText_Electric[];
extern const u8 sText_Psychic[];
extern const u8 sText_Ice[];
extern const u8 sText_Dragon[];
extern const u8 sText_Dark[];
extern const u8 sText_Fairy[];
extern const u8 sText_Stellar[];
