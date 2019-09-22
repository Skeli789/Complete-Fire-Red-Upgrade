#pragma once

#include "../global.h"

/**
 * \file training_sliding.h
 * \brief Functions relating to the enemy training sliding in- an off-screen mid-battle.
 *		  Also implements the feature where trainers can say messages mid-battle.
 */

//Exported Functions
bool8 ShouldDoTrainerSlide(u8 bank, u16 trainerId, u8 caseId);
void atk53_trainerslidein(void);
void atkFF1C_handletrainerslidemsg(void);
void atkFF1D_trytrainerslidefirstdownmsg(void);
void atkFF1E_trainerslideout(void);

//Functions Hooked In
void HandleIntroSlide(u8 terrain);
void CheckLastMonLowHPSlide(void);
