#pragma once

#include "../global.h"

/**
 * \file link.h
 * \brief Contains functions that help transfer data in link battles.
 */

//Exported Functions
void EmitDataTransfer(u8 bufferId, void* dst, u16 size, void* data);

//Functions Hooked In
void HandleDataTransfer(void);
