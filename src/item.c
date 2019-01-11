#include "defines.h"
#include "helper_functions.h"

u8* ItemId_GetName(u16 itemId) {
	u8* name = gItems[SanitizeItemId(itemId)].name;
	
	if (name[3] == 0x8) //Expanded Item Names
		name = T1_READ_PTR(name);
    return name;
}