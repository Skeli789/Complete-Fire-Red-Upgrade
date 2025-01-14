.thumb
.text
.align 2
.global BattleScriptCommandsTable
/*
battle_script_command_table.s
	table of battle script commands, including new ones
*/

.include "../asm_defines.s"
.equ NULL, 0x00000000

.global gBattleScriptingCommandsTable
.global gBattleScriptingCommandsTable2

gBattleScriptingCommandsTable:
.word atk00_attackcanceler
.word atk01_accuracycheck
.word atk02_attackstring
.word atk03_ppreduce
.word atk04_critcalc
.word atk05_damagecalc
.word atk06_typecalc
.word atk07_adjustnormaldamage
.word atk08_adjustnormaldamage2
.word atk09_attackanimation
.word 0x801f589					@waitanimation
.word atk0B_healthbarupdate		@graphicalhpupdate
.word atk0C_datahpupdate
.word atk0D_critmessage
.word atk0E_effectivenesssound
.word atk0F_resultmessage
.word 0x801fd51					@printstring
.word 0x801fd91					@printstring2
.word atk12_waitmessage
.word atk13_printfromtable
.word 0x801fe85					@printfromtable2
.word atk15_seteffectwithchance	@seteffectwithchancetarget
.word 0x8021211					@seteffecttarget
.word 0x8021221					@seteffectuser
.word 0x8021231					@clearstatus
.word atk19_tryfaintmon			@faintpokemon
.word 0x80215a1					@dofaintanimation
.word atk1B_cleareffectsonfaint
.word 0x8021641					@jumpifstatus
.word atk1D_jumpifstatus2		@jumpifsecondarystatus
.word atk1E_jumpifability		@jumpifability
.word atk1F_jumpifsideaffecting	@jumpifhalverset
.word atk20_jumpifstat
.word 0x8021989					@jumpifspecialstatusflag
.word atk22_jumpiftype
.word atk23_getexp				@battleendingfunction
.word 0x80223f9					@ifwildbattleend
.word 0x80225d9					@movevaluescleanup
.word 0x80225f1					@storeloopingcounter
.word atk27_decrementmultihit
.word 0x8022651					@goto
.word 0x8022671					@jumpifbyte
.word 0x8022711					@jumpifhalfword
.word 0x80227b9					@jumpifword
.word 0x802286d					@jumpifarrayequal
.word 0x80228f5					@jumpifarraynotequal
.word 0x8022979					@setbyte
.word 0x80229a1					@addbyte
.word 0x80229cd					@subtractbyte
.word 0x80229f9					@copyarray
.word 0x8022a4d					@copyarraywithindex
.word 0x8022ab9					@orbyte
.word 0x8022ae5					@orhalfword
.word 0x8022b1d					@orword
.word 0x8022b61					@bicbyte
.word 0x8022b8d					@bichalfword
.word 0x8022bc5					@bicword
.word 0x8022c09					@pause
.word 0x8022c49					@waitstateatk
.word 0x8022c69					@healthbarupdate
.word 0x8022cc1					@return
.word 0x8022ccd					@end
.word 0x8022ced					@end2
.word 0x8022d05					@end3
.word atk40_jumpifaffectedbyprotect
.word 0x8022d41					@call
.word atk42_jumpiftype2
.word 0x8022dcd					@jumpifabilitypresent
.word 0x8022e19					@endselectionscript
.word atk45_playanimation
.word atk46_playanimation2
.word atk47_setgraphicalstatchangevalues
.word atk48_playstatchangeanimation
.word atk49_moveend				@cmd49
.word atk4A_typecalc2			@damagecalc2
.word 0x8023e89					@returnatktoball
.word 0x8023ed9					@getswitchedmondata
.word atk4D_switchindataupdate	@switchindataupdate
.word 0x80240d5					@switchinanim
.word atk4F_jumpifcantswitch	@jumpifcannotswitch
.word 0x80243ed					@openpartyscreen
.word atk51_switchhandleorder
.word atk52_switchineffects
.word atk53_trainerslidein
.word 0x802511d					@playse
.word 0x8025159					@fanfare
.word 0x8025195					@pokemonfaintcry
.word 0x80251c5					@flee
.word 0x80251fd					@returntoball
.word 0x8025231					@handlelearnnewmove
.word 0x802537d					@yesnoboxlearnmove
.word 0x80256e1					@yesnoboxstoplearningmove
.word atk5C_hitanimation		@flash
.word 0x80258ad					@getmoneyreward
.word 0x8025a71					@atk5e
.word 0x8025b25					@atk5f
.word 0x8025b75					@incrementgamestat
.word atk61_drawpartystatussummary
.word 0x8025c6d					@atk62
.word atk63_jumptocalledmove	@jumptoattack
.word atk64_statusanimation
.word 0x8025d91					@status2animation
.word atk66_chosenstatusanimation
.word 0x8025ecd					@yesnobox
.word 0x8025fa9					@cancelallactions
.word atk69_adjustsetdamage
.word atk6A_removeitem
.word atk6B_atknameinbuff1
.word 0x8026209					@drawlvlupbox
.word 0x80267d9					@resetsentmonsvalue
.word 0x80267f1					@setatktoplayer0
.word 0x8026811					@makevisible
.word atk70_recordlastability
.word 0x80268a1					@buffermovetolearn
.word 0x80268b9					@jumpifplayerran
.word 0x80268fd					@hpthresholds
.word 0x80269a9					@hpthresholds2
.word 0x8026a59					@useitemonopponent
.word 0x8026ab1					@atk76
.word atk77_setprotect
.word atk78_faintifabilitynotdamp
.word 0x80271c1					@setuserhptozero
.word atk7A_jumpifnexttargetvalid	@jumpwhiletargetvalid
.word atk7B_tryhealhalfhealth		@setdamageasrestorehalfmaxhp
.word atk7C_trymirrormove			@jumptolastusedattack
.word atk7D_setrain
.word atk7E_setreflect
.word atk7F_setseeded			@setleechseed
.word 0x8027689					@manipulatedamage
.word atk81_trysetrest			@setrest
.word atk82_jumpifnotfirstturn
.word 0x8027821					@nop3
.word atk84_jumpifcantmakeasleep	@jumpifcannotsleep
.word 0x8027949					@stockpile
.word atk86_stockpiletobasedamage
.word atk87_stockpiletohpheal	@stockpiletohprecovery
.word atk88_negativedamage
.word atk89_statbuffchange
.word atk8A_normalisebuffs
.word 0x8028121					@setbide
.word 0x8028191					@confuseifrepeatingattackends
.word atk8D_setmultihitcounter	@setloopcounter
.word 0x802821d					@initmultihitstring
.word atk8F_forcerandomswitch
.word atk90_tryconversiontypechange	@changetypestoenemyattacktype
.word atk91_givepaydaymoney
.word atk92_setlightscreen
.word atk93_tryKO
.word atk94_damagetohalftargethp	@gethalfcurrentenemyhp
.word atk95_setsandstorm
.word atk96_weatherdamage
.word atk97_tryinfatuating		@tryinfatuatebank
.word 0x8029049					@refreshhpbar
.word atk99_setmist				@setmisteffect
.word 0x8029279					@setincreasedcriticalchance
.word atk9B_transformdataexecution
.word 0x8029475					@setsubstituteeffect
.word atk9D_mimicattackcopy		@copyattack
.word atk9E_metronome			@metronomeeffect
.word 0x80297fd					@nightshadedamageeffect
.word atkA0_psywavedamageeffect
.word atkA1_counterdamagecalculator
.word atkA2_mirrorcoatdamagecalculator
.word atkA3_disablelastusedattack
.word atkA4_trysetencore		@setencore
.word atkA5_painsplitdmgcalc	@painsplitdamagecalculator
.word atkA6_settypetorandomresistance
.word 0x8029fad					@setalwayshitflag
.word atkA8_copymovepermanently
.word atkA9_trychoosesleeptalkmove
.word atkAA_setdestinybond		@destinybondeffect
.word 0x802a459					@trysetdestinybondtohappen
.word 0x802a471					@remaininghptopower
.word atkAD_tryspiteppreduce	@reducepprandom
.word atkAE_healpartystatus		@healpartystatus
.word atkAF_cursetarget
.word atkB0_trysetspikes
.word atkB1_setforesight
.word atkB2_trysetperishsong	@setperishsong
.word atkB3_rolloutdamagecalculation
.word atkB4_jumpifconfusedandstatmaxed
.word atkB5_furycuttercalc		@furycutterdamagecalculation
.word 0x802ae25					@happinesstodamagecalculation
.word atkB7_presentdamagecalculation
.word atkB8_setsafeguard
.word atkB9_magnitudedamagecalculation
.word atkBA_jumpifnopursuitswitchdmg
.word atkBB_setsunny
.word atkBC_maxattackhalvehp
.word atkBD_copyfoestats
.word atkBE_rapidspinfree		@breakfree
.word 0x802b541					@setcurled
.word atkC0_recoverbasedonsunlight
.word 0x802b679					@hiddenpowerdamagecalculation
.word atkC2_selectfirstvalidtarget	@selectnexttarget
.word atkC3_trysetfutureattack		@setfutureattack
.word 0x802B911					@beatupcalculation
.word atkC5_setsemiinvulnerablebit
.word atkC6_clearsemiinvulnerablebit
.word 0x802bc35					@setminimize
.word atkC8_sethail
.word atkC9_jumpifattackandspecialattackcannotfall
.word atkCA_setforcedtarget
.word 0x802bdb5					@setcharge
.word atkCC_callterrainattack
.word atkCD_cureifburnedparalysedorpoisoned
.word atkCE_settorment
.word atkCF_jumpifnodamage
.word atkD0_settaunt
.word atkD1_trysethelpinghand	@sethelpinghand
.word atkD2_tryswapitems		@itemswap
.word atkD3_trycopyability		@copyability
.word atkD4_trywish
.word 0x802c4dd					@setroots
.word 0x802c535					@doubledamagedealtifdamaged
.word atkD7_setyawn
.word atkD8_setdamagetohealthdifference
.word 0x802c671					@scaledamagebyhealthratio
.word atkDA_tryswapabilities	@abilityswap
.word atkDB_tryimprision		@imprisoneffect
.word 0x802c875					@setgrudge
.word 0x802c8cd					@weightdamagecalculation
.word atkDE_asistattackselect
.word 0x802cae5					@setmagiccoat
.word 0x802cb69					@setstealstatchange
.word atkE1_trygetintimidatetarget
.word atkE2_switchoutabilities
.word 0x802cd3d					@jumpiffainted
.word atkE4_getsecretpowereffect	@secretpower
.word atkE5_pickupitemcalculation
.word 0x802cf31					@castformtransform
.word atkE7_trycastformdatachange	@castformswitch
.word atkE8_settypebasedhalvers
.word 0x802d091					@seteffectbyweather
.word atkEA_tryrecycleitem
.word atkEB_settypetoterrain
.word 0x802d2a1					@pursuitwhenswitched
.word 0x802d365					@snatchmove
.word atkEE_removelightscreenreflect	@removereflectlightscreen
.word atkEF_handleballthrow		@pokemoncatchfunction
.word atkF0_givecaughtmon		@catchpoke
.word atkF1_trysetcaughtmondexflags
.word 0x802d9d9					@displaydexinfo
.word 0x802dd39					@capturenicknamepoke
.word 0x802dfbd					@subattackerhpbydmg
.word 0x802dfed					@removeattackerstatus1
.word 0x802e015					@finishaction
.word 0x802e021					@finishturn
.word atkF8_callasm
.word atkF9_sethalfword
.word atkFA_setword
.word atkFB_setspecialstatusbit
.word atkFC_clearspecialstatusbit
.word atkFD_jumpifabilitypresenttargetfield
.word atkFE_prefaintmoveendeffects
.word atkFF_callsecondarytable

gBattleScriptingCommandsTable2:
.word NULL
.word NULL
.word atkFF02_cureprimarystatus
.word atkFF03_jumpifpartnerattack
.word NULL
.word NULL
.word atkFF06_setterrain
.word atkFF07_jumpifhelditemeffect
.word atkFF08_counterclear
.word atkFF09_jumpifcounter
.word atkFF0A_setability
.word NULL
.word atkFF0C_jumpiftargetpartner
.word NULL
.word atkFF0E_setcounter
.word atkFF0F_jumpifgrounded
.word atkFF10_jumpifhelditem
.word 0x8029049					@refreshhpbar
.word atkFF12_jumpifhealthcomparestomax
.word atkFF13_setdamagetobankhealthfraction
.word atkFF14_jumpiftypepresent
.word atkFF15_jumpifstatcanbemodified
.word atkFF16_jumpifnoviablemonsleft
.word atkFF17_setsidestatus
.word atkFF18_clearsidestatus
.word atkFF19_formchange
.word atkFF1A_jumpifabilitypresentattackerfield
.word atkFF1B_tryactivateswitchinability
.word atkFF1C_handletrainerslidemsg
.word atkFF1D_trytrainerslidefirstdownmsg
.word atkFF1E_trainerslideout
.word atkFF1F_flowershieldlooper
.word atkFF20_jumpifprotectedbycraftyshield
.word atkFF21_tryspectralthiefsteal
.word atkFF22_jumpifspecies
.word atkFF23_faintpokemonaftermove
.word atkFF24_jumpifattackeralreadydiddamage
.word atkFF25_jumpifterrainandgrounded
.word atkFF26_attackstringnoprotean
.word atkFF27_tryactivateprotean
.word atkFF28_jumpifweight
.word atkFF29_trysetsleep
.word atkFF2A_trysetparalysis
.word atkFF2B_trysetburn
.word atkFF2C_trysetpoison
.word atkFF2D_addindicatorforattackerswitchineffects
.word atkFF2E_setmoveeffect2
.word atkFF2F_setmaxmoveeffect
.word atkFF30_jumpifdynamaxed
.word atkFF31_jumpifraidboss
.word atkFF32_recycleberry
.word atkFF33_SetEffectPrimaryScriptingBank
.word atkFF34_canconfuse
.word atkFF35_jumpifmaxchistrikecapped
.word atkFF36_trygetcottondowntarget
