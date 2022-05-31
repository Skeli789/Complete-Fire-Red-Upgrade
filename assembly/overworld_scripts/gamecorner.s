.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

.global EventScript_StarterBuy
EventScript_StarterBuy:
	lock
	faceplayer
	checkflag 0x243
	if NOT_SET _goto EventScript_NeedCoinCase
	special 0x187
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_GCRelease
	showcoins 0x14 0x0
	msgbox gText_Exchange MSG_KEEPOPEN 
	closeonkeypress
	goto EventScript_SelectStarterBuy

EventScript_SelectStarterBuy:
	msgbox gText_WhatPrize MSG_KEEPOPEN
	setvar 0x8000 0x6
	setvar 0x8001 0x6
	setvar 0x8004 0x0
	special 0x158
	waitstate
	closeonkeypress
	compare LASTRESULT 0x18
	if greaterorequal _goto EventScript_GCBye
	copyvar 0x8005 LASTRESULT
	callasm SetStarterSpeciesToBuy
	copyvar 0x4001 0x8007
	bufferpokemon 0x0 0x8007
	msgbox gText_WantIt MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_GetStarterPokemon
	goto EventScript_SelectStarterBuy

EventScript_GetStarterPokemon:
	setvar 0x4002 0x7D0
	checkcoins LASTRESULT
	comparevars LASTRESULT 0x4002
	if 0x0 _goto EventScript_NeedMoreCoins
	removecoins 0x7D0
	updatecoins 0x14 0x0
	givepokemon 0x4001 0x28 0x0 0x0 0x0
	textcolor 0x3
	bufferpokemon 0x0 0x4001
	fanfare 0x13E
	msgbox gText_GetPokemon MSG_KEEPOPEN
	waitfanfare 
	closeonkeypress
	hidecoins 0x14 0x0
	release
	end
	
EventScript_NeedCoinCase:
	textcolor 0x3
	msgbox gText_NeedCoinCase MSG_KEEPOPEN
	closeonkeypress
	release
	end

EventScript_NeedMoreCoins:
	msgbox gText_NeedMoreCoins MSG_KEEPOPEN
	closeonkeypress
	hidecoins 0x14 0x0
	release
	end

EventScript_GCRelease:
	release
	end

EventScript_GCBye:
	hidecoins 0x14 0x0
	release
	end

.global EventScript_BuyCoins
EventScript_BuyCoins:
	lock
	checkflag 0x243
	if NOT_SET _goto EventScript_NeedCoinCase2
	setvar 0x4002 0x0
	setvar 0x4001 0x0
	showmoney 0x14 0x0
	showcoins 0x14 0x5
	msgbox gText_WelcomeRocketGamecorner MSG_KEEPOPEN
	setvar 0x8000 0x7
	setvar 0x8001 0x6
	setvar 0x8004 0x0
	special 0x158
	waitstate
	closeonkeypress
	compare LASTRESULT 0x0
	if 0x1 _goto EventScript_Buy50Coins
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_Buy500Coins
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_Buy5000Coins
	msgbox gText_ComePlaySoon MSG_KEEPOPEN
	closeonkeypress
	hidecoins 0x14 0x5
	hidemoney 0x14 0x0
	release
	end

EventScript_Buy50Coins:
	checkmoney 0xFA 0x0
	compare 0x800D 0x1
	if 0x0 _goto EventScript_NeedMoreCoinsBuy
	removemoney 0xFA 0x00
	addcoins 0x32 
	updatecoins 0x14 0x5
	updatemoney 0x14 0x0
	msgbox gText_HereCoins MSG_KEEPOPEN
	closeonkeypress
	hidecoins 0x14 0x5
	hidemoney 0x14 0x0
	release
	end

EventScript_Buy500Coins:
	checkmoney 0x9C4 0x0
	compare 0x800D 0x1
	if 0x0 _goto EventScript_NeedMoreCoinsBuy
	removemoney 0x9C4 0x00
	addcoins 0x1F4
	updatecoins 0x14 0x5
	updatemoney 0x14 0x0
	msgbox gText_HereCoins MSG_KEEPOPEN
	closeonkeypress
	hidecoins 0x14 0x5
	hidemoney 0x14 0x0
	release
	end

EventScript_Buy5000Coins:
	checkmoney 0x61A8 0x0
	compare 0x800D 0x1
	if 0x0 _goto EventScript_NeedMoreCoinsBuy
	removemoney 0x61A8 0x00
	addcoins 0x1388
	updatecoins 0x14 0x5
	updatemoney 0x14 0x0
	msgbox gText_HereCoins MSG_KEEPOPEN
	closeonkeypress
	hidecoins 0x14 0x5
	hidemoney 0x14 0x0
	release
	end


EventScript_NeedMoreCoinsBuy:
	msgbox gText_NeedMoreMoney MSG_KEEPOPEN
	closeonkeypress
	hidecoins 0x14 0x5
	hidemoney 0x14 0x0
	release
	end

EventScript_NeedCoinCase2:
	msgbox gText_NeedCoinCase2 MSG_KEEPOPEN
	closeonkeypress
	hidecoins 0x14 0x5
	hidemoney 0x14 0x0
	release
	end

.global EventScript_PseudoOthersBuy
EventScript_PseudoOthersBuy:
	lock
	faceplayer
	checkflag 0x243
	if NOT_SET _goto EventScript_NeedCoinCase
	special 0x187
	compare LASTRESULT 0x2
	if 0x1 _goto EventScript_GCRelease
	showcoins 0x14 0x0
	msgbox gText_Exchange MSG_KEEPOPEN 
	closeonkeypress
	goto EventScript_SelectPseudoOtherBuy

EventScript_SelectPseudoOtherBuy:
	msgbox gText_WhatPrize MSG_KEEPOPEN
	setvar 0x8000 0x8
	setvar 0x8001 0x6
	setvar 0x8004 0x0
	special 0x158
	waitstate
	closeonkeypress
	compare LASTRESULT 0x2B
	if greaterorequal _goto EventScript_GCBye
	copyvar 0x8005 LASTRESULT
	callasm SetPseudoOtherSpeciesToBuy
	copyvar 0x4001 0x8007
	bufferpokemon 0x0 0x8007
	msgbox gText_WantIt MSG_YESNO
	compare LASTRESULT 0x1
	if 0x1 _goto EventScript_GetPseudoOtherPokemon
	goto EventScript_SelectPseudoOtherBuy

EventScript_GetPseudoOtherPokemon:
	copyvar 0x4002 0x8008
	@@setvar 0x4002 0x
	checkcoins LASTRESULT
	comparevars LASTRESULT 0x4002
	if 0x0 _goto EventScript_NeedMoreCoins
	removecoins 0x4002
	updatecoins 0x14 0x0
	givepokemon 0x4001 0x28 0x0 0x0 0x0
	textcolor 0x3
	bufferpokemon 0x0 0x4001
	fanfare 0x13E
	msgbox gText_GetPokemon MSG_KEEPOPEN
	waitfanfare 
	closeonkeypress
	hidecoins 0x14 0x0
	release
	end
