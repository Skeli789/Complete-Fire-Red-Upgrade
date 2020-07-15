#include "defines.h"
#include "defines_battle.h"

#include "../include/new/util.h"
#include "../include/new/link.h"
/*
link.c
	handles data transfer
*/

void EmitDataTransfer(u8 bufferId, void* dst, u16 size, void* data)
{
	s32 i;

	gBattleBuffersTransferData[0] = CONTROLLER_DATATRANSFER;
	gBattleBuffersTransferData[1] = (u32)(dst);
	gBattleBuffersTransferData[2] = ((u32)(dst) & 0x0000FF00) >> 8;
	gBattleBuffersTransferData[3] = ((u32)(dst) & 0x00FF0000) >> 16;
	gBattleBuffersTransferData[4] = ((u32)(dst) & 0xFF000000) >> 24;
	gBattleBuffersTransferData[5] = size;
	gBattleBuffersTransferData[6] = (size & 0xFF00) >> 8;
	for (i = 0; i < size; i++)
		gBattleBuffersTransferData[7 + i] = *(u8*)(data++);
	PrepareBufferDataTransfer(bufferId, gBattleBuffersTransferData, size + 7);
}

void HandleDataTransfer(void)
{
	u32 dstArg = gBattleBufferA[gActiveBattler][1]
			  | (gBattleBufferA[gActiveBattler][2] << 8)
			  | (gBattleBufferA[gActiveBattler][3] << 16)
			  | (gBattleBufferA[gActiveBattler][4] << 24);

	u16 sizeArg = gBattleBufferA[gActiveBattler][5] | (gBattleBufferA[gActiveBattler][6] << 8);

	u8* src = &gBattleBufferA[gActiveBattler][7];
	u8* dst = (u8*)(dstArg);
	u32 size = sizeArg;

	while (TRUE)
	{
		if (size <= 0x1000)
		{
			Memcpy(dst, src, size);
			break;
		}
		Memcpy(dst, src, 0x1000);
		src += 0x1000;
		dst += 0x1000;
		size -= 0x1000;
	}

	if (SIDE(gActiveBattler) == B_SIDE_OPPONENT)
	{
		if (gBattleTypeFlags & BATTLE_TYPE_LINK)
			LinkOpponentBufferExecCompleted();
		else
			OpponentBufferExecCompleted();
	}
	else
		PlayerBufferExecCompleted();
}
