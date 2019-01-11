/*.thumb
.text
.align 2
.global AttackDescriptionTable

.loadtable "..\table.ini"

AttackDescriptionTable:
.word 0x8482834		@MOVE_NONE
.word 0x8482874		@MOVE_POUND
.word 0x84828bd		@MOVE_KARATECHOP
.word 0x84828ff		@MOVE_DOUBLESLAP
.word 0x8482946		@MOVE_COMETPUNCH
.word 0x8482985		@MOVE_MEGAPUNCH
.word 0x84829c9		@MOVE_PAYDAY
.word 0x8482a11		@MOVE_FIREPUNCH
.word 0x8482a53		@MOVE_ICEPUNCH
.word 0x8482aa0		@MOVE_THUNDERPUNCH
.word 0x8482acd		@MOVE_SCRATCH
.word 0x8482b00		@MOVE_VICEGRIP
.word 0x8482b46		@MOVE_GUILLOTINE
.word 0x8482b94		@MOVE_RAZORWIND
.word 0x8482bd5		@MOVE_SWORDSDANCE
.word 0x8482c16		@MOVE_CUT
.word 0x8482c4f		@MOVE_GUST
.word 0x8482c89		@MOVE_WINGATTACK
.word 0x8482cd3		@MOVE_WHIRLWIND
.word 0x8482d1d		@MOVE_FLY
.word 0x8482d66		@MOVE_BIND
.word 0x8482d96		@MOVE_SLAM
.word 0x8482dc8		@MOVE_VINEWHIP
.word 0x8482e08		@MOVE_STOMP
.word 0x8482e45		@MOVE_DOUBLEKICK
.word 0x8482e83		@MOVE_MEGAKICK
.word 0x8482ecc		@MOVE_JUMPKICK
.word 0x8482f0a		@MOVE_ROLLINGKICK
.word 0x8482f4c		@MOVE_SANDATTACK
.word 0x8482f8f		@MOVE_HEADBUTT
.word 0x8482fd0		@MOVE_HORNATTACK
.word 0x8483014		@MOVE_FURYATTACK
.word 0x848305c		@MOVE_HORNDRILL
.word 0x84830a2		@MOVE_TACKLE
.word 0x84830eb		@MOVE_BODYSLAM
.word 0x8483130		@MOVE_WRAP
.word 0x8483178		@MOVE_TAKEDOWN
.word 0x84831bf		@MOVE_THRASH
.word 0x8483203		@MOVE_DOUBLEEDGE
.word 0x8483249		@MOVE_TAILWHIP
.word 0x848328b		@MOVE_POISONSTING
.word 0x84832d2		@MOVE_TWINEEDLE
.word 0x8483314		@MOVE_PINMISSILE
.word 0x8483358		@MOVE_LEER
.word 0x8483397		@MOVE_BITE
.word 0x84833dc		@MOVE_GROWL
.word 0x8483426		@MOVE_ROAR
.word 0x848346c		@MOVE_SING
.word 0x84834a8		@MOVE_SUPERSONIC
.word 0x84834ed		@MOVE_SONICBOOM
.word 0x8483534		@MOVE_DISABLE
.word 0x8483580		@MOVE_ACID
.word 0x84835c2		@MOVE_EMBER
.word 0x8483606		@MOVE_FLAMETHROWER
.word 0x848364b		@MOVE_MIST
.word 0x8483693		@MOVE_WATERGUN
.word 0x84836d6		@MOVE_HYDROPUMP
.word 0x848371f		@MOVE_SURF
.word 0x8483760		@MOVE_ICEBEAM
.word 0x84837a1		@MOVE_BLIZZARD
.word 0x84837e3		@MOVE_PSYBEAM
.word 0x848382a		@MOVE_BUBBLEBEAM
.word 0x848386d		@MOVE_AURORABEAM
.word 0x84838b3		@MOVE_HYPERBEAM
.word 0x84838ea		@MOVE_PECK
.word 0x8483927		@MOVE_DRILLPECK
.word 0x848396e		@MOVE_SUBMISSION
.word 0x84839ae		@MOVE_LOWKICK
.word 0x84839f8		@MOVE_COUNTER
.word 0x8483a3a		@MOVE_SEISMICTOSS
.word 0x8483a82		@MOVE_STRENGTH
.word 0x8483ac5		@MOVE_ABSORB
.word 0x8483b0c		@MOVE_MEGADRAIN
.word 0x8483b57		@MOVE_LEECHSEED
.word 0x8483b94		@MOVE_GROWTH
.word 0x8483bda		@MOVE_RAZORLEAF
.word 0x8483c22		@MOVE_SOLARBEAM
.word 0x8483c5d		@MOVE_POISONPOWDER
.word 0x8483c9b		@MOVE_STUNSPORE
.word 0x8483cdb		@MOVE_SLEEPPOWDER
.word 0x8483d24		@MOVE_PETALDANCE
.word 0x8483d6b		@MOVE_STRINGSHOT
.word 0x8483db0		@MOVE_DRAGONRAGE
.word 0x8483dfe		@MOVE_FIRESPIN
.word 0x8483e3e		@MOVE_THUNDERSHOCK
.word 0x8483e80		@MOVE_THUNDERBOLT
.word 0x8483ec2		@MOVE_THUNDERWAVE
.word 0x8483f03		@MOVE_THUNDER
.word 0x8483f44		@MOVE_ROCKTHROW
.word 0x8483f89		@MOVE_EARTHQUAKE
.word 0x8483fc7		@MOVE_FISSURE
.word 0x848400f		@MOVE_DIG
.word 0x8484058		@MOVE_TOXIC
.word 0x8484098		@MOVE_CONFUSION
.word 0x84840df		@MOVE_PSYCHIC
.word 0x8484123		@MOVE_HYPNOSIS
.word 0x8484165		@MOVE_MEDITATE
.word 0x84841a8		@MOVE_AGILITY
.word 0x84841e9		@MOVE_QUICKATTACK
.word 0x848422e		@MOVE_RAGE
.word 0x8484278		@MOVE_TELEPORT
.word 0x84842c0		@MOVE_NIGHTSHADE
.word 0x848430a		@MOVE_MIMIC
.word 0x8484353		@MOVE_SCREECH
.word 0x8484398		@MOVE_DOUBLETEAM
.word 0x84843e5		@MOVE_RECOVER
.word 0x848442e		@MOVE_HARDEN
.word 0x8484476		@MOVE_MINIMIZE
.word 0x84844b5		@MOVE_SMOKESCREEN
.word 0x84844f3		@MOVE_CONFUSERAY
.word 0x848453c		@MOVE_WITHDRAW
.word 0x8484580		@MOVE_DEFENSECURL
.word 0x84845c5		@MOVE_BARRIER
.word 0x8484606		@MOVE_LIGHTSCREEN
.word 0x8484647		@MOVE_HAZE
.word 0x8484689		@MOVE_REFLECT
.word 0x84846d3		@MOVE_FOCUSENERGY
.word 0x8484715		@MOVE_BIDE
.word 0x848475e		@MOVE_METRONOME
.word 0x84847a2		@MOVE_MIRRORMOVE
.word 0x84847e8		@MOVE_SELFDESTRUCT
.word 0x848482d		@MOVE_EGGBOMB
.word 0x8484871		@MOVE_LICK
.word 0x84848b5		@MOVE_SMOG
.word 0x84848f2		@MOVE_SLUDGE
.word 0x848493b		@MOVE_BONECLUB
.word 0x8484986		@MOVE_FIREBLAST
.word 0x84849ca		@MOVE_WATERFALL
.word 0x8484a15		@MOVE_CLAMP
.word 0x8484a57		@MOVE_SWIFT
.word 0x8484aa2		@MOVE_SKULLBASH
.word 0x8484ae1		@MOVE_SPIKECANNON
.word 0x8484b27		@MOVE_CONSTRICT
.word 0x8484b5b		@MOVE_AMNESIA
.word 0x8484ba1		@MOVE_KINESIS
.word 0x8484beb		@MOVE_SOFTBOILED
.word 0x8484c27		@MOVE_HIGHJUMPKICK
.word 0x8484c75		@MOVE_GLARE
.word 0x8484cbb		@MOVE_DREAMEATER
.word 0x8484d01		@MOVE_POISONGAS
.word 0x8484d42		@MOVE_BARRAGE
.word 0x8484d85		@MOVE_LEECHLIFE
.word 0x8484dcd		@MOVE_LOVELYKISS
.word 0x8484e18		@MOVE_SKYATTACK
.word 0x8484e60		@MOVE_TRANSFORM
.word 0x8484ea4		@MOVE_BUBBLE
.word 0x8484ee5		@MOVE_DIZZYPUNCH
.word 0x8484f20		@MOVE_SPORE
.word 0x8484f6a		@MOVE_FLASH
.word 0x8484fb5		@MOVE_PSYWAVE
.word 0x8484ff8		@MOVE_SPLASH
.word 0x848503f		@MOVE_ACIDARMOR
.word 0x848508b		@MOVE_CRABHAMMER
.word 0x84850d4		@MOVE_EXPLOSION
.word 0x8485114		@MOVE_FURYSWIPES
.word 0x848515e		@MOVE_BONEMERANG
.word 0x84851ad		@MOVE_REST
.word 0x84851ef		@MOVE_ROCKSLIDE
.word 0x8485231		@MOVE_HYPERFANG
.word 0x8485277		@MOVE_SHARPEN
.word 0x84852b8		@MOVE_CONVERSION
.word 0x8485301		@MOVE_TRIATTACK
.word 0x848533c		@MOVE_SUPERFANG
.word 0x8485382		@MOVE_SLASH
.word 0x84853be		@MOVE_SUBSTITUTE
.word 0x8485405		@MOVE_STRUGGLE
.word 0x8485446		@MOVE_SKETCH
.word 0x848548b		@MOVE_TRIPLEKICK
.word 0x84854d6		@MOVE_THIEF
.word 0x848551d		@MOVE_SPIDERWEB
.word 0x8485560		@MOVE_MINDREADER
.word 0x84855aa		@MOVE_NIGHTMARE
.word 0x84855e9		@MOVE_FLAMEWHEEL
.word 0x848562f		@MOVE_SNORE
.word 0x8485679		@MOVE_CURSE
.word 0x84856c1		@MOVE_FLAIL
.word 0x848570c		@MOVE_CONVERSION2
.word 0x8485752		@MOVE_AEROBLAST
.word 0x8485798		@MOVE_COTTONSPORE
.word 0x84857df		@MOVE_REVERSAL
.word 0x848581e		@MOVE_SPITE
.word 0x8485857		@MOVE_POWDERSNOW
.word 0x84858a1		@MOVE_PROTECT
.word 0x84858e2		@MOVE_MACHPUNCH
.word 0x8485923		@MOVE_SCARYFACE
.word 0x848596b		@MOVE_FEINTATTACK
.word 0x84859ae		@MOVE_SWEETKISS
.word 0x84859f2		@MOVE_BELLYDRUM
.word 0x8485a30		@MOVE_SLUDGEBOMB
.word 0x8485a7a		@MOVE_MUDSLAP
.word 0x8485ac1		@MOVE_OCTAZOOKA
.word 0x8485b0c		@MOVE_SPIKES
.word 0x8485b55		@MOVE_ZAPCANNON
.word 0x8485b9c		@MOVE_FORESIGHT
.word 0x8485bde		@MOVE_DESTINYBOND
.word 0x8485c24		@MOVE_PERISHSONG
.word 0x8485c66		@MOVE_ICYWIND
.word 0x8485cb0		@MOVE_DETECT
.word 0x8485cf0		@MOVE_BONERUSH
.word 0x8485d30		@MOVE_LOCKON
.word 0x8485d77		@MOVE_OUTRAGE
.word 0x8485dc1		@MOVE_SANDSTORM
.word 0x8485e09		@MOVE_GIGADRAIN
.word 0x8485e55		@MOVE_ENDURE
.word 0x8485ea0		@MOVE_CHARM
.word 0x8485ee1		@MOVE_ROLLOUT
.word 0x8485f24		@MOVE_FALSESWIPE
.word 0x8485f6c		@MOVE_SWAGGER
.word 0x8485fb6		@MOVE_MILKDRINK
.word 0x8485ff5		@MOVE_SPARK
.word 0x848602b		@MOVE_FURYCUTTER
.word 0x8486075		@MOVE_STEELWING
.word 0x84860b7		@MOVE_MEANLOOK
.word 0x8486105		@MOVE_ATTRACT
.word 0x8486145		@MOVE_SLEEPTALK
.word 0x8486187		@MOVE_HEALBELL
.word 0x84861d1		@MOVE_RETURN
.word 0x848621b		@MOVE_PRESENT
.word 0x8486265		@MOVE_FRUSTRATION
.word 0x84862ab		@MOVE_SAFEGUARD
.word 0x84862f3		@MOVE_PAINSPLIT
.word 0x8486330		@MOVE_SACREDFIRE
.word 0x8486378		@MOVE_MAGNITUDE
.word 0x84863c7		@MOVE_DYNAMICPUNCH
.word 0x848640b		@MOVE_MEGAHORN
.word 0x8486455		@MOVE_DRAGONBREATH
.word 0x848649f		@MOVE_BATONPASS
.word 0x84864e8		@MOVE_ENCORE
.word 0x8486532		@MOVE_PURSUIT
.word 0x8486579		@MOVE_RAPIDSPIN
.word 0x84865bf		@MOVE_SWEETSCENT
.word 0x8486606		@MOVE_IRONTAIL
.word 0x8486651		@MOVE_METALCLAW
.word 0x8486693		@MOVE_VITALTHROW
.word 0x84866de		@MOVE_MORNINGSUN
.word 0x8486729		@MOVE_SYNTHESIS
.word 0x8486774		@MOVE_MOONLIGHT
.word 0x84867b7		@MOVE_HIDDENPOWER
.word 0x84867fb		@MOVE_CROSSCHOP
.word 0x848683a		@MOVE_TWISTER
.word 0x848687c		@MOVE_RAINDANCE
.word 0x84868b8		@MOVE_SUNNYDAY
.word 0x84868fe		@MOVE_CRUNCH
.word 0x8486941		@MOVE_MIRRORCOAT
.word 0x848698a		@MOVE_PSYCHUP
.word 0x84869cf		@MOVE_EXTREMESPEED
.word 0x8486a1b		@MOVE_ANCIENTPOWER
.word 0x8486a62		@MOVE_SHADOWBALL
.word 0x8486aa6		@MOVE_FUTURESIGHT
.word 0x8486aef		@MOVE_ROCKSMASH
.word 0x8486b36		@MOVE_WHIRLPOOL
.word 0x8486b7e		@MOVE_BEATUP
.word 0x8486bc7		@MOVE_FAKEOUT
.word 0x8486c10		@MOVE_UPROAR
.word 0x8486c55		@MOVE_STOCKPILE
.word 0x8486c95		@MOVE_SPITUP
.word 0x8486cd4		@MOVE_SWALLOW
.word 0x8486d1c		@MOVE_HEATWAVE
.word 0x8486d64		@MOVE_HAIL
.word 0x8486db1		@MOVE_TORMENT
.word 0x8486df2		@MOVE_FLATTER
.word 0x8486e37		@MOVE_WILLOWISP
.word 0x8486e79		@MOVE_MEMENTO
.word 0x8486ebe		@MOVE_FACADE
.word 0x8486f04		@MOVE_FOCUSPUNCH
.word 0x8486f50		@MOVE_SMELLINGSALTS
.word 0x8486f96		@MOVE_FOLLOWME
.word 0x8486fd4		@MOVE_NATUREPOWER
.word 0x8487014		@MOVE_CHARGE
.word 0x848705c		@MOVE_TAUNT
.word 0x848709b		@MOVE_HELPINGHAND
.word 0x84870dd		@MOVE_TRICK
.word 0x848711e		@MOVE_ROLEPLAY
.word 0x8487163		@MOVE_WISH
.word 0x84871a1		@MOVE_ASSIST
.word 0x84871e9		@MOVE_INGRAIN
.word 0x8487234		@MOVE_SUPERPOWER
.word 0x8487276		@MOVE_MAGICCOAT
.word 0x84872aa		@MOVE_RECYCLE
.word 0x84872f2		@MOVE_REVENGE
.word 0x8487338		@MOVE_BRICKBREAK
.word 0x8487378		@MOVE_YAWN
.word 0x84873be		@MOVE_KNOCKOFF
.word 0x84873fb		@MOVE_ENDEAVOR
.word 0x848743c		@MOVE_ERUPTION
.word 0x848747f		@MOVE_SKILLSWAP
.word 0x84874c1		@MOVE_IMPRISON
.word 0x848750d		@MOVE_REFRESH
.word 0x8487558		@MOVE_GRUDGE
.word 0x848759a		@MOVE_SNATCH
.word 0x84875e5		@MOVE_SECRETPOWER
.word 0x8487628		@MOVE_DIVE
.word 0x848766b		@MOVE_ARMTHRUST
.word 0x84876a7		@MOVE_CAMOUFLAGE
.word 0x84876e6		@MOVE_TAILGLOW
.word 0x848772d		@MOVE_LUSTERPURGE
.word 0x8487771		@MOVE_MISTBALL
.word 0x84877bb		@MOVE_FEATHERDANCE
.word 0x84877f3		@MOVE_TEETERDANCE
.word 0x848783a		@MOVE_BLAZEKICK
.word 0x848787a		@MOVE_MUDSPORT
.word 0x84878bc		@MOVE_ICEBALL
.word 0x84878f5		@MOVE_NEEDLEARM
.word 0x8487932		@MOVE_SLACKOFF
.word 0x8487976		@MOVE_HYPERVOICE
.word 0x84879bc		@MOVE_POISONFANG
.word 0x8487a07		@MOVE_CRUSHCLAW
.word 0x8487a53		@MOVE_BLASTBURN
.word 0x8487a9e		@MOVE_HYDROCANNON
.word 0x8487aeb		@MOVE_METEORMASH
.word 0x8487b2f		@MOVE_ASTONISH
.word 0x8487b71		@MOVE_WEATHERBALL
.word 0x8487bbf		@MOVE_AROMATHERAPY
.word 0x8487bff		@MOVE_FAKETEARS
.word 0x8487c46		@MOVE_AIRCUTTER
.word 0x8487c8b		@MOVE_OVERHEAT
.word 0x8487cd2		@MOVE_ODORSLEUTH
.word 0x8487d1d		@MOVE_ROCKTOMB
.word 0x8487d68		@MOVE_SILVERWIND
.word 0x8487db0		@MOVE_METALSOUND
.word 0x8487def		@MOVE_GRASSWHISTLE
.word 0x8487e30		@MOVE_TICKLE
.word 0x8487e72		@MOVE_COSMICPOWER
.word 0x8487eb3		@MOVE_WATERSPOUT
.word 0x8487ef6		@MOVE_SIGNALBEAM
.word 0x8487f35		@MOVE_SHADOWPUNCH
.word 0x8487f76		@MOVE_EXTRASENSORY
.word 0x8487fb4		@MOVE_SKYUPPERCUT
.word 0x8487ff9		@MOVE_SANDTOMB
.word 0x8488041		@MOVE_SHEERCOLD
.word 0x848808a		@MOVE_MUDDYWATER
.word 0x84880d0		@MOVE_BULLETSEED
.word 0x8488111		@MOVE_AERIALACE
.word 0x8488153		@MOVE_ICICLESPEAR
.word 0x848819a		@MOVE_IRONDEFENSE
.word 0x84881e1		@MOVE_BLOCK
.word 0x848821f		@MOVE_HOWL
.word 0x8488266		@MOVE_DRAGONCLAW
.word 0x84882b4		@MOVE_FRENZYPLANT
.word 0x84882fb		@MOVE_BULKUP
.word 0x8488341		@MOVE_BOUNCE
.word 0x8488383		@MOVE_MUDSHOT
.word 0x84883c9		@MOVE_POISONTAIL
.word 0x8488408		@MOVE_COVET
.word 0x848844b		@MOVE_VOLTTACKLE
.word 0x848848a		@MOVE_MAGICALLEAF
.word 0x84884c5		@MOVE_WATERSPORT
.word 0x8488507		@MOVE_CALMMIND
.word 0x848854f		@MOVE_LEAFBLADE
.word 0x8488597		@MOVE_DRAGONDANCE
.word 0x84885d3		@MOVE_ROCKBLAST
.word 0x8488614		@MOVE_SHOCKWAVE
.word 0x848865a		@MOVE_WATERPULSE
.word 0x84886a1		@MOVE_DOOMDESIRE
.word 0x8e8d6c9		@MOVE_PSYCHOBOOST

.word DESC_LEECHFANG
.word DESC_AIRSLASH
.word DESC_AQUAJET
.word DESC_AQUATAIL
.word DESC_AURASPHERE
.word DESC_AVALANCHE
.word DESC_BRAVEBIRD
.word DESC_BUGBUZZ
.word DESC_BULLETPUNCH
.word DESC_CHARGEBEAM
.word DESC_CLOSECOMBAT
.word DESC_CROSSPOISON
.word DESC_DARKPULSE
.word DESC_DISCHARGE
.word DESC_DRACOMETEOR
.word DESC_DRAGONPULSE
.word DESC_DRAINPUNCH
.word DESC_EARTHPOWER
.word DESC_ENERGYBALL
.word DESC_FOCUSBLAST
.word DESC_FORCEPALM
.word DESC_GIGAIMPACT
.word DESC_GRASSKNOT
.word DESC_GUNKSHOT
.word DESC_HAMMERARM
.word DESC_HEADSMASH
.word DESC_ICESHARD
.word DESC_IRONHEAD
.word DESC_LAVAPLUME
.word DESC_MAGNETBOMB
.word DESC_MUDBOMB
.word DESC_NASTYPLOT
.word DESC_NIGHTSLASH
.word DESC_OMINOUSWIND
.word DESC_POWERGEM
.word DESC_POWERWHIP
.word DESC_PSYCHOCUT
.word DESC_ROCKCLIMB
.word DESC_ROCKPOLISH
.word DESC_ROCKWRECKER
.word DESC_ROOST
.word DESC_SEEDBOMB
.word DESC_SHADOWCLAW
.word DESC_SHADOWSNEAK
.word DESC_VACUUMWAVE
.word DESC_XSCISSOR
.word DESC_ZENHEADBUTT
.word DESC_SWITCHEROO
.word DESC_DRILLRUN
.word DESC_BULLDOZE
.word DESC_ELECTROWEB
.word DESC_FLAMECHARGE
.word DESC_FROSTBREATH
.word DESC_HEARTSTAMP
.word DESC_HONECLAWS
.word DESC_HORNLEECH
.word DESC_COIL
.word DESC_HURRICANE
.word DESC_ICICLECRASH
.word DESC_WORKUP
.word DESC_QUIVERDANCE
.word DESC_LEAFTORNADO
.word DESC_LOWSWEEP
.word DESC_SNARL
.word DESC_STRUGGLEBUG
.word DESC_STEAMROLLER
.word DESC_STORMTHROW
.word DESC_VENOSHOCK
.word DESC_WILDCHARGE
.word DESC_POISONJAB
.word DESC_ACIDSPRAY
.word DESC_FIERYDANCE
.word DESC_CLEARSMOG
.word DESC_LEAFSTORM
.word DESC_STONEEDGE
.word DESC_PAYBACK
.word DESC_FIREFANG
.word DESC_ICEFANG
.word DESC_THUNDERFANG
.word DESC_FLAREBLITZ
.word DESC_INFERNO
.word DESC_HEX
.word DESC_DUALCHOP
.word DESC_DOUBLEHIT
.word DESC_COTTONGUARD
.word DESC_ACROBATICS
.word DESC_FLAMEBURST
.word DESC_UTURN
.word DESC_VOLTSWITCH
.word DESC_DRAGONTAIL
.word DESC_CIRCLETHROW
.word DESC_MIRRORSHOT
.word DESC_WOODHAMMER
.word DESC_HEALPULSE
.word DESC_FLASHCANNON
.word DESC_BUGBITE
.word DESC_PLUCK
.word DESC_SHELLSMASH
.word DESC_RAGEPOWDER
.word DESC_WAKEUPSLAP
.word DESC_RAZORSHELL
.word DESC_COPYCAT
.word DESC_MIRACLEEYE
.word DESC_SUCKERPUNCH
.word DESC_SCALD
.word DESC_BRINE
.word DESC_FAIRYWIND
.word DESC_BABYDOLLEYES
.word DESC_PLAYROUGH
.word DESC_MOONBLAST
.word DESC_DRAININGKISS
.word DESC_DAZZLINGGLEAM
.word DESC_DISARMINGVOICE
.word DESC_STEALTHROCK
.word DESC_TOXICSPIKES
.word DESC_HEALINGWISH
.word DESC_LUNARDANCE
.word DESC_TAILWIND
.word DESC_BOOMBURST
.word DESC_INCINERATE
.word DESC_WORRYSEED
.word DESC_GASTROACID
.word DESC_GEOMANCY
.word DESC_FLOWERSHIELD
.word DESC_ROTOTILLER
.word DESC_SHIFTGEAR
.word DESC_STICKYWEB
.word DESC_STOREDPOWER
.word DESC_ACUPRESSURE
.word DESC_PUNISHMENT
.word DESC_ASSURANCE
.word DESC_DRAGONRUSH
.word DESC_DARKVOID
.word DESC_FEINT
.word DESC_PHANTOMFORCE
.word DESC_PETALBLIZZARD
.word DESC_PLAYNICE
.word DESC_ROAROFTIME
.word DESC_SPACIALREND
.word DESC_MAGMASTORM
.word DESC_FINALGAMBIT
.word DESC_SHADOWFORCE
.word DESC_NUZZLE
.word DESC_RETALIATE
.word DESC_STEELYHIT
.word DESC_JUDGMENT
.word DESC_FREEZEDRY
.word DESC_PSYSHOCK
.word DESC_ROUND
.word DESC_TAILSLAP
.word DESC_GEARGRIND
.word DESC_NATURALGIFT
.word DESC_CHIPAWAY
.word DESC_SMACKDOWN
.word DESC_HYPERSPACEHOLE
.word DESC_GYROBALL
.word DESC_HYPERSPACEFURY
.word DESC_ATTACKORDER
.word DESC_DEFENDORDER
.word DESC_HEALORDER
.word DESC_CAPTIVATE
.word DESC_DEFOG
.word DESC_ORIGINPULSE
.word DESC_PRECIPICEBLADES
.word DESC_GUARDSWAP
.word DESC_STEAMERUPTION
.word DESC_METALBURST
.word DESC_MEFIRST
.word DESC_POWERSWAP
.word DESC_POWERTRICK
.word DESC_PSYCHOSHIFT
.word DESC_WRINGOUT
.word DESC_KINGSSHIELD
.word DESC_PSYSTRIKE
.word DESC_AUTOTOMIZE
.word DESC_DIAMONDSTORM
.word DESC_SLUDGEWAVE
.word DESC_HEAVYSLAM
.word DESC_SYNCHRONOISE
.word DESC_ELECTROBALL
.word DESC_FOULPLAY
.word DESC_ENTRAINMENT
.word DESC_NIGHTDAZE
.word DESC_ECHOEDVOICE
.word DESC_LANDSWRATH
.word DESC_OBLIVIONWING
.word DESC_HEARTSWAP
.word DESC_CRUSHGRIP
.word DESC_SACREDSWORD
.word DESC_HEATCRASH
.word DESC_HEADCHARGE
.word DESC_TECHNOBLAST
.word DESC_RELICSONG
.word DESC_SECRETSWORD
.word DESC_GLACIATE
.word DESC_BOLTSTRIKE
.word DESC_BLUEFLARE
.word DESC_FREEZESHOCK
.word DESC_ICEBURN
.word DESC_VCREATE
.word DESC_FUSIONFLARE
.word DESC_FUSIONBOLT
.word DESC_FELLSTINGER
.word DESC_NOBLEROAR
.word DESC_DRAGONASCENT
.word DESC_PARTINGSHOT
.word DESC_TOPSYTURVY
.word DESC_COREENFORCER
.word DESC_CHATTER
.word DESC_CONFIDE
.word DESC_WATERSHURIKEN
.word DESC_MYSTICALFIRE
.word DESC_SPIKYSHIELD
.word DESC_EERIEIMPULSE
.word DESC_HOLDBACK
.word DESC_INFESTATION
.word DESC_POWERUPPUNCH
.word DESC_THOUSANDARROWS
.word DESC_THOUSANDWAVES
.word DESC_PARABOLICCHARGE
.word DESC_SEARINGSHOT
.word DESC_SEEDFLARE
.word DESC_ACCELEROCK
.word DESC_ANCHORSHOT
.word DESC_AURORAVEIL
.word DESC_BANEFULBUNKER
.word DESC_BEAKBLAST
.word DESC_BRUTALSWING
.word DESC_BURNUP
.word DESC_CLANGINGSCALES
.word DESC_DARKESTLARIAT
.word DESC_DRAGONHAMMER
.word DESC_FIRELASH
.word DESC_FIRSTIMPRESSION
.word DESC_FLEURCANNON
.word DESC_FLORALHEALING
.word DESC_GEARUP
.word DESC_HIGHHORSEPOWER
.word DESC_ICEHAMMER
.word DESC_LASERFOCUS
.word DESC_LEAFAGE
.word DESC_LIQUIDATION
.word DESC_LUNGE
.word DESC_MOONGEISTBEAM
.word DESC_MULTIATTACK
.word DESC_NATURESMADNESS
.word DESC_POLLENPUFF
.word DESC_POWERTRIP
.word DESC_PRISMATICLASER
.word DESC_PSYCHICFANGS
.word DESC_PURIFY
.word DESC_REVELATIONDANCE
.word DESC_SHADOWBONE
.word DESC_SHELLTRAP
.word DESC_SHOREUP
.word DESC_SMARTSTRIKE
.word DESC_SOLARBLADE
.word DESC_SPARKLINGARIA
.word DESC_SPECTRALTHIEF
.word DESC_SPEEDSWAP
.word DESC_SPIRITSHACKLE
.word DESC_SPOTLIGHT
.word DESC_STOMPINGTANTRUM
.word DESC_STRENGTHSAP
.word DESC_SUNSTEELSTRIKE
.word DESC_TEARFULLOOK
.word DESC_THROATCHOP
.word DESC_TOXICTHREAD
.word DESC_TROPKICK
.word DESC_ZINGZAP
.word DESC_ELECTRICTERRAIN
.word DESC_GRASSYTERRAIN
.word DESC_MISTYTERRAIN
.word DESC_PSYCHICTERRAIN
.word DESC_GRASSPLEDGE
.word DESC_FIREPLEDGE
.word DESC_WATERPLEDGE
.word DESC_TRICKROOM
.word DESC_MAGICROOM
.word DESC_WONDERROOM
.word DESC_CRAFTYSHIELD
.word DESC_MATBLOCK
.word DESC_QUICKGUARD
.word DESC_WIDEGUARD
.word DESC_AQUARING
.word DESC_AROMATICMIST
.word DESC_BELCH
.word DESC_BESTOW
.word DESC_FLING
.word DESC_FLYINGPRESS
.word DESC_GRAVITY
.word DESC_GUARDSPLIT
.word DESC_HEALBLOCK
.word DESC_IONDELUGE
.word DESC_LUCKYCHANT
.word DESC_MAGNETRISE
.word DESC_MAGNETICFLUX
.word DESC_POWDER
.word DESC_POWERSPLIT
.word DESC_REFLECTTYPE
.word DESC_SIMPLEBEAM
.word DESC_SOAK
.word DESC_TELEKINESIS
.word DESC_TRICKORTREAT
.word DESC_VENOMDRENCH
.word DESC_FORESTSCURSE
.word DESC_MINDBLOWN
.word DESC_PHOTONGEYSER
.word DESC_PLASMAFISTS
.word DESC_ELECTRIFY
.word DESC_EMBARGO
.word DESC_FAIRYLOCK
.word DESC_LIGHTOFRUIN
.word DESC_TRUMPCARD
.word DESC_AFTERYOU
.word DESC_INSTRUCT
.word DESC_QUASH
.word DESC_HAPPYHOUR
.word DESC_DOUBLEIRONBASH
.word DESC_LASTRESORT
.word DESC_SKYDROP
.word DESC_CELEBRATE
.word DESC_HOLDHANDS
.word DESC_BLANK	@MOVE_BREAKNECK_BLITZ_P
.word DESC_BLANK	@MOVE_BREAKNECK_BLITZ_S
.word DESC_BLANK	@MOVE_ALL-OUT_PUMMELING_P
.word DESC_BLANK	@MOVE_ALL-OUT_PUMMELING_S
.word DESC_BLANK	@MOVE_SUPERSONIC_SKYSTRIKE_P
.word DESC_BLANK	@MOVE_SUPERSONIC_SKYSTRIKE_S
.word DESC_BLANK	@MOVE_ACID_DOWNPOUR_P
.word DESC_BLANK	@MOVE_ACID_DOWNPOUR_S
.word DESC_BLANK	@MOVE_TECTONIC_RAGE_P
.word DESC_BLANK	@MOVE_TECTONIC_RAGE_S
.word DESC_BLANK	@MOVE_CONTINENTAL_CRUSH_P
.word DESC_BLANK	@MOVE_CONTINENTAL_CRUSH_S
.word DESC_BLANK	@MOVE_SAVAGE_SPIN-OUT_P
.word DESC_BLANK	@MOVE_SAVAGE_SPIN-OUT_S
.word DESC_BLANK	@MOVE_NEVER-ENDING_NIGHTMARE_P
.word DESC_BLANK	@MOVE_NEVER-ENDING_NIGHTMARE_S
.word DESC_BLANK	@MOVE_CORKSCREW_CRASH_P
.word DESC_BLANK	@MOVE_CORKSCREW_CRASH_S
.word DESC_BLANK	@MOVE_INFERNO_OVERDRIVE_P
.word DESC_BLANK	@MOVE_INFERNO_OVERDRIVE_S
.word DESC_BLANK	@MOVE_HYDRO_VORTEX_P
.word DESC_BLANK	@MOVE_HYDRO_VORTEX_S
.word DESC_BLANK	@MOVE_BLOOM_DOOM_P
.word DESC_BLANK	@MOVE_BLOOM_DOOM_S
.word DESC_BLANK	@MOVE_GIGAVOLT_HAVOC_P
.word DESC_BLANK	@MOVE_GIGAVOLT_HAVOC_S
.word DESC_BLANK	@MOVE_SHATTERED_PSYCHE_P
.word DESC_BLANK	@MOVE_SHATTERED_PSYCHE_S
.word DESC_BLANK	@MOVE_SUBZERO_SLAMMER_P
.word DESC_BLANK	@MOVE_SUBZERO_SLAMMER_S
.word DESC_BLANK	@MOVE_DEVASTATING_DRAKE_P
.word DESC_BLANK	@MOVE_DEVASTATING_DRAKE_S
.word DESC_BLANK	@MOVE_BLACK_HOLE_ECLIPSE_P
.word DESC_BLANK	@MOVE_BLACK_HOLE_ECLIPSE_S
.word DESC_BLANK	@MOVE_TWINKLE_TACKLE_P
.word DESC_BLANK	@MOVE_TWINKLE_TACKLE_S
.word DESC_BLANK	@MOVE_CATASTROPIKA
.word DESC_BLANK	@MOVE_10000000_VOLT_THUNDERBOLT
.word DESC_BLANK	@MOVE_STOKED_SPARKSURFER
.word DESC_BLANK	@MOVE_EXTREME_EVOBOOST
.word DESC_BLANK	@MOVE_PULVERIZING_PANCAKE
.word DESC_BLANK	@MOVE_GENESIS_SUPERNOVA
.word DESC_BLANK	@MOVE_SINISTER_ARROW_RAID
.word DESC_BLANK	@MOVE_MALICIOUS_MOONSAULT
.word DESC_BLANK	@MOVE_OCEANIC_OPERETTA
.word DESC_BLANK	@MOVE_SPLINTERED_STORMSHARDS
.word DESC_BLANK	@MOVE_LETS_SNUGGLE_FOREVER
.word DESC_BLANK	@MOVE_CLANGOROUS_SOULBLAZE
.word DESC_BLANK	@MOVE_GUARDIAN_OF_ALOLA
.word DESC_BLANK	@MOVE_SEARING_SUNRAZE_SMASH
.word DESC_BLANK	@MOVE_MENACING_MOONRAZE_MAELSTROM
.word DESC_BLANK	@MOVE_LIGHT_THAT_BURNS_THE_SKY
.word DESC_BLANK	@MOVE_SOUL-STEALING_7-STAR_STRIKE

.align 2
DESC_LEECHFANG: .string "An attack that\nabsorbs half the\ndamage it inflicted\nto restore HP.$"

DESC_AIRSLASH: .string "The foe is sliced\nby a blade of air.\nIt may also make\nthe foe flinch.$"

DESC_AQUAJET: .string "An almost invisibly\nfast attack that\nis certain to strike\nfirst.$"

DESC_AQUATAIL: .string "The foe is attacked\nwith a swinging tail\nas vicious as a\nwave in a storm.$"

DESC_AURASPHERE: .string "A blast of aura\npower is let loose\nat the foe, it is\nsure to hit.$"

DESC_AVALANCHE: .string "An attack move that\ngains in intensity\nif the target has\nhurt the user.$"

DESC_BRAVEBIRD: .string "The user tucks in\nits wings and\ncharges. It also\nhurts the user.         $"

DESC_BUGBUZZ: .string "The user creates a\ndamaging sound\nwave that may lower\nthe foe's Sp. Def.$"

DESC_BULLETPUNCH: .string "The user strikes\nwith a tough punch\nthat is certain to\nstrike first.$"

DESC_CHARGEBEAM: .string "The user fires a\nload of electricity\nthat may raise the\nuser's Sp. Atk.  $"

DESC_CLOSECOMBAT: .string "The foe is fought\nup close. It also\ncuts the user's\nDefensive stats.$"

DESC_CROSSPOISON: .string "An attack with a\nhigh critical-hit\nratio. It may also\npoison the foe.$"

DESC_DARKPULSE: .string "The user releases\na horrible aura that\nmay also make the\nfoe flinch.$"

DESC_DISCHARGE: .string "Electricty that can\nparalyze, is loosed\nto strike all\nPokemon in battle.          $"

DESC_DRACOMETEOR: .string "An intense attack\nthat also sharply\nreduces the user's\nSp. Atk stat.$"

DESC_DRAGONPULSE: .string "The foe is attacked\nwith a shock wave\ngenerated by the\nuser's gaping mouth.$"

DESC_DRAINPUNCH: .string "A harsh punch that\nabsorbs half the\ndamage it inflicted\nto restore HP.$"

DESC_EARTHPOWER: .string "The ground under the\nfoe erupts. It may \nalso lower the foe's\nSp. Def.          $"

DESC_ENERGYBALL: .string "The foe is blasted\nwith natural energy.\nIt may also lower\nthe foe's Sp. Def.$"

DESC_FOCUSBLAST: .string "The user unleashes\nits power. It may\nalso lower the\nfoe's Sp. Def.$"

DESC_FORCEPALM: .string "The foe is attacked\nwith a shock wave\nthat may also leave\nit paralyzed.$"

DESC_GIGAIMPACT: .string "A severely damaging\nattack that makes\nthe user rest on\nthe next turn.$"

DESC_GRASSKNOT: .string "Grass snares and\ntrips the foe. It\ndoes more damage\nto heavier foes.$"

DESC_GUNKSHOT: .string "Filthy garbage is\nshot at the foe.\nIt may also poison\nthe target.$"

DESC_HAMMERARM: .string "The user swings\nits heavy fist.\nIt also lowers the\nuser's Speed.$"

DESC_HEADSMASH: .string "A reckless, life-\nrisking tackle that\nalso hurts the user\nterribly.$"

DESC_ICESHARD: .string "The user hurls\nchunks of ice\nthat are certain to\nstrike first.$"

DESC_IRONHEAD: .string "The foe is slammed\nwith its hard head.\nIt may also make\nthe foe flinch.$"

DESC_LAVAPLUME: .string "Scarlet flames wash\nover all Pokémon in\nbattle. It may also\ninflict burns.$"

DESC_MAGNETBOMB: .string "A steel bomb is\nstuck to the foe.\nIt is sure to hit.$"

DESC_MUDBOMB: .string "The foe is struck\nwith a mud ball,\nthat may also lower\nits Accuracy.$"

DESC_NASTYPLOT: .string "The user thinks\nbad thoughts. It\nsharply raises the\nSp. Atk stat.$"

DESC_NIGHTSLASH: .string "The user slashes\nthe foe when it\ncan. It has a high\ncritical-hit ratio.$"

DESC_OMINOUSWIND: .string "Repulsive wind is\nused to attack. It\nmay also raise all\nthe user's stats.$"

DESC_POWERGEM: .string "The user attacks\nwith a ray of light\nthat sparkles like\ngemstones.$"

DESC_POWERWHIP: .string "The user violently\nwhirls its vines or\ntentacles to lash\nthe foe.$"

DESC_PSYCHOCUT: .string "The user tears the\nfoe with psychic\nblades. It has a\nhigh crit ratio.        $"

DESC_ROCKCLIMB: .string "A charging attack\nthat may confuse\nthe foe. Can also be\nused to scale walls.$"

DESC_ROCKPOLISH: .string "The user polishes\nits body to sharply\nboost its Speed.$"

DESC_ROCKWRECKER: .string "The foe is hit with\na huge boulder.\nThe user can't move\non the next turn.$"

DESC_ROOST: .string "A self-healing move\nthat restores HP by\nup to half of the\nuser's maximum HP.$"

DESC_SEEDBOMB: .string "The user shoots a\nbarrage of hard\nseeds at the foe.$"

DESC_SHADOWCLAW: .string "Claws made of\nshadows rake the\nfoe. It has a high\ncritical-hit ratio.$"

DESC_SHADOWSNEAK: .string "The user extends\nits shadow to\nattack. It will\nstrike first.         $"

DESC_VACUUMWAVE: .string "The user sends a\nwave of vacuum at\nthe foe. It will\nstrike first.         $"

DESC_XSCISSOR: .string "The user slashes\nthe foe with crossed\nclaws as if they\nwere scissors.$"

DESC_ZENHEADBUTT: .string "The user focuses\nand rams the foe.\nIt may also make\nthe foe flinch.$"

DESC_SWITCHEROO: .string "The user trades held\nitems with the foe\nfaster than the eye\ncan follow.$"

DESC_DRILLRUN: .string "The user rotates\nits body like a\ndrill. It has a high\ncritical-hit ratio.$"

DESC_BULLDOZE: .string "The user stomps,\ndamaging all Pokémon\nin battle. It lowers\nopposing Speed.     $"

DESC_ELECTROWEB: .string "The user traps the\nfoe in an electric\nnet. It also lowers\nthe foe's Speed.$"

DESC_FLAMECHARGE: .string "The user cloaks\nitself in flames and\nattacks. It also ups\nthe user's Speed.   $"

DESC_FROSTBREATH: .string "The user blows\ncold air at the foe.\nIt always results in\na critical hit.$"

DESC_HEARTSTAMP: .string "The foe is hit\nafter a cute act.\nIt may also make\nthe foe flinch.$"

DESC_HONECLAWS: .string "The user sharpens\nits claws to raise\nits Attack and\nAccuracy.$"

DESC_HORNLEECH: .string "A harsh attack that\nabsorbs half the\ndamage it inflicted\nto restore HP.$"

DESC_COIL: .string "The user coils up\nto raise its Attack\nDefense and\nAccuracy.$"

DESC_HURRICANE: .string "The foe is caught\nin a fierce wind.\nIt may also confuse\nthe foe.$"

DESC_ICICLECRASH: .string "Icicles are dropped\non the foe. It may\nalso make the foe\nflinch.$"

DESC_WORKUP: .string "The user rouses\nitself to raise\nits Attack and\nSp. Atk stats.$"

DESC_QUIVERDANCE: .string "The user dances\nto raise its Sp. Atk\nSp. Def. and\nSpeed stats.$"

DESC_LEAFTORNADO: .string "Sharp leaves circle\nthe foe. It may also\nlower the foe's\nAccuracy.$"

DESC_LOWSWEEP: .string "The user attacks\nthe foe's legs,\nlowering its\nSpeed stat.$"

DESC_SNARL: .string "The user barks\nmenacingly. It\nalso lowers the\nfoe's Sp. Atk.$"

DESC_STRUGGLEBUG: .string "While resisting, the\nuser attacks the\nfoe, lowerings its\nSp. Atk stat.$"

DESC_STEAMROLLER: .string "The user crushes\nthe foe with its\nbody. It may also\nmake the foe flinch.$"

DESC_STORMTHROW: .string "The user strikes\nthe foe fiercely.\nIt always results in\na critical hit.$"

DESC_VENOSHOCK: .string "The foe is soaked\nby a special acid.\nIts power doubles if\nthe foe is poisoned.$"

DESC_WILDCHARGE: .string "The user throws an\nelectrified tackle.\nIt hurts the user\na little.$"

DESC_POISONJAB: .string "The foe is stabbed\nby an arm steeped\nin poison. It may\nalso poison the foe.$"

DESC_ACIDSPRAY: .string "The user spits fluid\nthat melts the foe.\nIt harshly lowers\nthe foe's Sp. Def.$"

DESC_FIERYDANCE: .string "Cloaked in flames,\nthe user dances.\nIt may also raise\nthe user's Sp. Atk.$"

DESC_CLEARSMOG: .string "Eliminates all stat\nchanges among all\nPokémon engaged in\nbattle.$"

DESC_LEAFSTORM: .string "An intense attack\nthat also sharply\nreduces the user's\nSp. Atk stat.$"

DESC_STONEEDGE: .string "The foe is stabbed\nby sharpened stones.\nIt has a high\ncritical-hit ratio.$"

DESC_PAYBACK: .string "An attack move that\ngains in intensity\nif the target has\nhurt the user.$"

DESC_FIREFANG: .string "The user bites the\nfoe. It may also\nmake the foe flinch,\nor leave it burned.$"

DESC_ICEFANG: .string "The user bites the\nfoe. It may also\nmake the foe flinch,\nor leave it frozen.$"

DESC_THUNDERFANG: .string "The user bites the\nfoe. It may also \nmake the foe flinch, \nor paralyze it.     $"

DESC_FLAREBLITZ: .string "A fiery tackle that\nmay leave the foe\nburned. It also hurts\nthe user a little.$"

DESC_INFERNO: .string "The foe is engulfed\nby an intense fire\nthat is sure to\nleave a burn.$"

DESC_HEX: .string "This attack does\ndouble damage to a\ntarget affected by\na status problem.$"

DESC_DUALCHOP: .string "The user hits\nthe foe with two\nbrutal strikes\nin one turn.$"

DESC_DOUBLEHIT: .string "The foe is struck\nwith a long tail,\nvines, etc. Twice\nin one turn.$"

DESC_COTTONGUARD: .string "The user protects\nitself with soft\ncotton. Sharply\nraising its Defense.$"

DESC_ACROBATICS: .string "A nimble attack\nthat does double\ndamage if the user\nhas no held item.$"

DESC_FLAMEBURST: .string "The user attacks\nthe foe with a\nbursting flame.$"

DESC_UTURN: .string "After attacking,\nthe user switches\nplaces with a\nparty Pokémon.$"

DESC_VOLTSWITCH: .string "After attacking,\nthe user switches\nplaces with a\nparty Pokémon.$"

DESC_DRAGONTAIL: .string "The foe is made to\nswitch out with an\nally. In the wild,\nthe battle ends.$"

DESC_CIRCLETHROW: .string "The foe is made to\nswitch out with an\nally. In the wild,\nthe battle ends.$"

DESC_MIRRORSHOT: .string "The user looses\na flash of energy\nto damage and\nlower Accuracy.$"

DESC_WOODHAMMER: .string "The user slams the\nfoe with its hard\nbody, also hurting\nitself a little.$"

DESC_HEALPULSE: .string "The user emits a\nhealing pulse that\nrestores a target's\nHP by up to half.$"

DESC_FLASHCANNON: .string "Light energy is\nshot at the foe.\nIt may also lower\nthe foe's Sp. Def.$"

DESC_BUGBITE: .string "The user bites\nthe foe, eating\nits berry if it is\nholding one.$"

DESC_PLUCK: .string "The user pecks\nthe foe, plucking\nits berry if it is\nholding one.$"

DESC_SHELLSMASH: .string "Raises the user's\noffensive stats,\nat the cost of its\ndefensive stats.$"

DESC_RAGEPOWDER: .string "The user scatters\na cloud of powder,\nmaking foes attack\nonly the user.$"

DESC_WAKEUPSLAP: .string "Does double damage\nto a sleeping foe.\nIt also wakes the\nfoe up, however.$"

DESC_RAZORSHELL: .string "The foe is cut\nby sharp shells.\nIt may also lower\nthe foe's Defense.$"

DESC_COPYCAT: .string "The user copies the\nmove last used by\nthe foe for the\nrest of the battle.$"

DESC_MIRACLEEYE: .string "Completely negates\nthe foe's efforts to\nheighten its ability\nto evade.$"

DESC_SUCKERPUNCH: .string "This move allows\nthe user to strike\nfirst, if the foe is\nreadying an attack.$"

DESC_SCALD: .string "Boiling water is\nshot at the foe.\nIt may also cause\na burn.$"

DESC_BRINE: .string "If the foe's HP is\ndown to half, this\nattack does double\ndamage.$"

DESC_FAIRYWIND: .string "The user stirs up\na fairy wind and\nstrikes the foe\nwith it.$"

DESC_BABYDOLLEYES: .string "A cute stare that\nlowers the foe's\nAttack stat. It is\nsure to go first.$"

DESC_PLAYROUGH: .string "The foe is roughed\nup by the user.\nThis may lower the\nfoe's Attack.$"

DESC_MOONBLAST: .string "The foe is attacked\nwith the power of\nthe moon, this may\nlower its Sp. Atk.$"

DESC_DRAININGKISS: .string "A sweet kiss that\nabsorbs half the\ndamage it inflicted\nto restore HP.$"

DESC_DAZZLINGGLEAM: .string "The user damages\nopposing Pokémon\nby emitting a\npowerful flash.$"

DESC_DISARMINGVOICE: .string "The user lets out\na charming cry\nthat cannot be\nevaded.$"

DESC_STEALTHROCK: .string "Levitating rocks\nare laid around the\nfoe's party to hurt\nfoes switching in.$"

DESC_TOXICSPIKES: .string "Poisoned spikes\nare laid around the\nfoe's party to hurt\nfoes switching in.$"

DESC_HEALINGWISH: .string "The user faints,\nbut will fully\nheal the Pokémon\ntaking its place.$"

DESC_LUNARDANCE: .string "The user faints,\nbut will fully\nheal the Pokémon\ntaking its place.$"

DESC_TAILWIND: .string "Whips up a tailwind\nthat ups the Speed\nof all party Pokémon\nfor four turns.$"

DESC_BOOMBURST: .string "The user emits a\nterrible soundwave\nthat hits everything\naround it.$"

DESC_INCINERATE: .string "The user burns\nthe foe completely,\nrendering a held\nBerry useless.$"

DESC_WORRYSEED: .string "A worrying seed is\nplanted on the foe,\nmaking its ability\nInsomnia.$"

DESC_GASTROACID: .string "Stomach acid is\nhurled onto the\nfoe, suppressing\nits ability.$"

DESC_GEOMANCY: .string "The user's Sp. Atk\nSp. Def. and\nSpeed stats rise\non the 2nd turn.$"

DESC_FLOWERSHIELD: .string "The Defense stat\nof all Grass-type\nPokémon is raised\nby a strange power.$"

DESC_ROTOTILLER: .string "The Attack and\nSp. Atk stats of\nall Grass-type\nPokémon are raised.$"

DESC_SHIFTGEAR: .string "Rotating its gears\nthe user raises its\nAttack and Sp. Atk\nstats.$"

DESC_STICKYWEB: .string "A sticky net is\nwoven around the\nfoe's party to slow\nfoes switching in.$"

DESC_STOREDPOWER: .string "The user attacks\nwith a stored power\nthat grows with\neach raised stat.$"

DESC_ACUPRESSURE: .string "The user sharply\nraises one of its\nstats by pressing\nits stress points.$"

DESC_PUNISHMENT: .string "The more the foe\nhas raised its\nstats, the greater\nthe move's power.$"

DESC_ASSURANCE: .string "Does double damage\nif the foe has\nalready taken\ndamage this turn.$"

DESC_DRAGONRUSH: .string "A tackle exhibiting\ndevastating menace.\nThis may also make\nthe target flinch.$"

DESC_DARKVOID: .string "All foes are taken\ninto a world of\ntotal darkness that\nmakes them sleep.$"

DESC_FEINT: .string "An attack that\nbreaks through\nprotecting foes.$"

DESC_PHANTOMFORCE: .string "The user vanishes,\nthen strikes on the\nnext turn. This\nbypasses Protect.$"

DESC_PETALBLIZZARD: .string "A violent petal\nblizzard stirs up,\nhitting everything\naround it.$"

DESC_PLAYNICE: .string "The target loses\nits will to fight.\nThis lowers its\nAttack.$"

DESC_ROAROFTIME: .string "An attack which\ndistorts time. The\nuser must rest on\nthe next turn.$"

DESC_SPACIALREND: .string "An attack that\ntears the space\naround it. Critical\nhits land more\neasily.$"

DESC_MAGMASTORM: .string "A maelstrom of fire\nrages that traps\nthe target for four\nto five turns.$"

DESC_FINALGAMBIT: .string "The user risks all,\ndealing damage\nequal to its HP\nand then fainting\nfrom exhaustion.$"

DESC_SHADOWFORCE: .string "The user vanishes,\nthen strikes on the\nnext turn. This\nbypasses Protect.$"

DESC_NUZZLE: .string "The target is\nattacked with\nelectrified cheeks\nthat leave it\nparalyzed.$"

DESC_RETALIATE: .string "If an ally fainted\nin the previous\nturn, this attack's\ndamage increases.$"

DESC_STEELYHIT: .string "A physical attack\nin which the user\ncharges, full body,\ninto the foe.$"

DESC_JUDGMENT: .string "An attack which\nchanges type,\ndepending on which\nkind of Plate the\nuser is holding.$"

DESC_FREEZEDRY: .string "An attack which may\nleave the target\nfrozen. It is also\nsuper effective on\nWater types.$"

DESC_PSYSHOCK: .string "An odd psychic wave\nattacks the target.\nThis attack does\nphysical damage.$"

DESC_ROUND: .string "The user attacks\nthe target with a\nsong.$"

DESC_TAILSLAP: .string "The use attacks by\nhitting the target\nwith its hard tail.\nIt hits two to five\ntimes in a row.$"

DESC_GEARGRIND: .string "The user attacks by\nthrowing two steel\ngears at it target.\nThis attack hits\ntwice in a row.$"

DESC_NATURALGIFT: .string "The type and base\npower of this move\nis determined by\nthe user's Berry.$"

DESC_CHIPAWAY: .string "An attack which is\nnot affected by the\ntarget's stat\nchanges.$"

DESC_SMACKDOWN: .string "A projectile is\nthrown at the\nopponent. A flying\nPokemon will fall\nto the ground.$"

DESC_HYPERSPACEHOLE: .string "Using a hyperspace\nhole, the user\nattacks the target.\nThis also hits\nthrough protect.$"

DESC_GYROBALL: .string "A fast spin. The\nslower the user,\nthe greater the\ndamage.$"

DESC_HYPERSPACEFURY: .string "The target is hit\nwith many arms. \nThis hits through \nprotect and lowers \nthe user's Defense.$"

DESC_ATTACKORDER: .string "The user calls out\nits underlings to\npummel the foe. \nIt has a high \ncritical-hit ratio.$"

DESC_DEFENDORDER: .string "The user calls out \nits underlings to \nshield its body, \nraising its Defense \nand Sp. Def stats.$"

DESC_HEALORDER: .string "The user calls out \nits underlings to \nheal it. The user \nregains up to half \nof its max HP.$"

DESC_CAPTIVATE: .string "If the foe is the\nopposite gender of\nthe user, the foe's\nSpecial Attack is\nharshly lowered.$"

DESC_DEFOG: .string "A strong wind blows \naway the target's \nobstacles. It also \nlowers the target's\nevasiveness.$"

DESC_ORIGINPULSE: .string "The user attacks \nopposing Pokémon \nwith countless \nbeams of light that\nglow a deep blue.$"

DESC_PRECIPICEBLADES: .string "The user attacks \nopposing Pokémon by manifesting the \npower of the land\nin blades of stone.$"

DESC_GUARDSWAP: .string "The user uses its\npsychic power to\nswitch changes to\nits Defense and Sp.\nDef with the foe.$"

DESC_STEAMERUPTION: .string "The user immerses\nthe target in\nsuperheated steam.\nThis may also burn\nthe target.$"

DESC_METALBURST: .string "The user retaliates\nwith much greater\nforce against the\nopponent that last\ndid damage to it.$"

DESC_MEFIRST: .string "The user tries to \ncut ahead of the\ntarget to steal and\nuse its intended\nmove with more power.$"

DESC_POWERSWAP: .string "The user uses its\npsychic power to\nswitch changes to\nits Attack and Sp.\nAttack with the foe.$"

DESC_POWERTRICK: .string "The user employs\nits psychic power\nto switch its\nAttack stat with\nits Defense stat.$"

DESC_PSYCHOSHIFT: .string "Using its psychic\npower of suggestion,\nthe user transfers\nits status problems\nto the target.$"

DESC_WRINGOUT: .string "The user powerfully\nwrings the target.\nThe more HP the\ntarget has, the\ngreater the power.$"

DESC_KINGSSHIELD: .string "The user protects\nitself from damage,\nand harshly lowers\nthe foe's Attack if\nit makes contact.$"

DESC_PSYSTRIKE: .string "An odd psychic wave\nattacks the target.\nThis attack does\nphysical damage.$"

DESC_AUTOTOMIZE: .string "The user sheds part\nof its body to make\nitself lighter and\nsharply raise its\nSpeed stat.$"

DESC_DIAMONDSTORM: .string "The user whips up a\nstorm of diamonds\nto damage the foes.\nThis may also raise\nthe user's Defens  $"

DESC_SLUDGEWAVE: .string "The area around the\nuser is swamped by\na giant sludge wave.\nIt may also poison\nthose hit.$"

DESC_HEAVYSLAM: .string "The user slams into\nthe target. The\ngreater the weight\ndifference, the\ngreater the damage.$"

DESC_SYNCHRONOISE: .string "Using an odd shock\nwave, the user\ndamages same-type\nPokemon in the\nsurrounding area.$"

DESC_ELECTROBALL: .string "The user hurls an\nelectric orb at\nthe target. Damage\nscales depending on\nuser/target's speed.$"

DESC_FOULPLAY: .string "The user turns the\ntarget's power\nagainst it. Power\nincreases with the\nfoe's Attack stat.$"

DESC_ENTRAINMENT: .string "The user dances\nwith an odd rhythm,\nmaking the target's\nAbility the same as\nthe user's Ability.$"

DESC_NIGHTDAZE: .string "The user releases a\npitch-black blast\nat its target. May\nalso lower target's\naccuracy.$"

DESC_ECHOEDVOICE: .string "The user attacks\nthe target with a \nharsh echo.\nIf used every turn,\npower increases.$"

DESC_LANDSWRATH: .string "The user gathers\nenergy from the \nland and damages \nthe opposing Pokemon\nwith its power.$"

DESC_OBLIVIONWING: .string "The user absorbs its\ntarget's HP. The\nuser's HP is restored\nby over half the\ndamage.$"

DESC_HEARTSWAP: .string "The user employs its\npsychic power to\nswitch stat changes\nwith the target.$"

DESC_CRUSHGRIP: .string "The foe's crushed\nwith great force. The\nmore HP the foe has,\nthe stronger the\npower.$"

DESC_SACREDSWORD: .string "The user attacks by\nslicing with a long\nhorn. The target's\nstat changes are\nignored.$"

DESC_HEATCRASH: .string "The user fiery slams\nthe foe. The more the\nuser outweighs the\nfoe, the greater the\ndamage.$"

DESC_HEADCHARGE: .string "The user charges its\nhead into its foe,\nusing its guard hair.\nIt slightly damages\nthe user.$"

DESC_TECHNOBLAST: .string "The user fires a beam\nof light at its foe.\nThe type changes\ndepending on the\nDrive held.$"

DESC_RELICSONG: .string "The user sings an\nancient song and\nattacks the hearts of\nthe listeners. It\nmay induce sleep.$"

DESC_SECRETSWORD: .string "The user cuts with\nits long horn. The\nodd power in the\nhorn does physical\ndamage to the foe.$"

DESC_GLACIATE: .string "The user attacks by\nblowing freezing cold\nair at opposing\nPokémon. This lowers\ntheir Speed.$"

DESC_BOLTSTRIKE: .string "The user surrounds\nitself with abundant\nelectricity that can\nparalyze and charges\nits target.$"

DESC_BLUEFLARE: .string "The user attacks by\nengulfing the foe in\na severe, beautiful,\nblue flame. It may\nburn the foe.$"

DESC_FREEZESHOCK: .string "A two-turn attack\nthat hits the foe\nwith electrically\ncharged ice. It may\nparalyze the foe.$"

DESC_ICEBURN: .string "A two-turn attack\nthat surrounds the\nfoe with ultracold,\nfreezing wind. It\nmay burn the foe.$"

DESC_VCREATE: .string "The user hurls the\nhot flame on its\nforehead. It lowers\nthe user's Def, Sp.\nDef, and Speed.$"

DESC_FUSIONFLARE: .string "The user brings down\na huge flame. It does\nmore damage when\ninfluenced by an\nenormous bolt.$"

DESC_FUSIONBOLT: .string "The user throws down\na giant bolt. It does\nmore damage when\ninfluenced by an\nenormous flame.$"

DESC_FELLSTINGER: .string "When the user knocks\nout a target with\nthis move, the user's\nAttack stat rises\ndrastically.$"

DESC_NOBLEROAR: .string "Letting out a noble\nroar, the user\nintimidates the\ntarget and lowers its\nAtk and Sp. Atk stats.$"

DESC_DRAGONASCENT: .string "The user attacks by\ndropping off from\nthe sky at high speed.\nIt lowers its own Def\nand Sp. Def.$"

DESC_PARTINGSHOT: .string "With a parting threat,\nthe user lowers the\nfoe's Attack and Sp.\nAtk stats then\nswitches out.$"

DESC_TOPSYTURVY: .string "All stat changes\naffecting the target\nturn topsy-turvy and\nbecome the opposite\nof them.$"

DESC_COREENFORCER: .string "If the foe the user\nhit have already used\ntheir moves, it\neliminates the effect\nof its Ability.$"

DESC_CHATTER: .string "The user attacks the\ntarget with sound\nwaves of deafening\nchatter. This\nconfuses the target.$"

DESC_CONFIDE: .string "The user tells the\nfoe a secret, and it\nloses concentration.\nIt lowers the foe's\nSp.Atk stat.$"

DESC_WATERSHURIKEN: .string "The user hits the foe\nwith throwing stars\ntwo to five times in\na row. This always\ngoes first.$"

DESC_MYSTICALFIRE: .string "The user attacks by\nbreathing a special,\nhot fire. This also\nlowers the target's\nSp. Atk stat.$"

DESC_SPIKYSHIELD: .string "Aside from protecting\nthe user from attacks,\nthis also damages\nattackers who make\ncontact.$"

DESC_EERIEIMPULSE: .string "The user's body\ngenerates an impulse.\nExposing the foe to\nit harshly lowers\nits Sp. Atk stat.$"

DESC_HOLDBACK: .string "The user holds back\nwhen it attacks, and\nthe target is left\nwith at least 1 HP.$"

DESC_INFESTATION: .string "The foe is infested\nand attacked for four\nto five turns. The\ntarget can't flee\nduring this.$"

DESC_POWERUPPUNCH: .string "Striking opponents\nover and over makes\nthe user's fists\nharder. Thus,\nincreases Atk stat.$"

DESC_THOUSANDARROWS: .string "This move also hits\nopposing Pokémon in\nthe air. They are\nknocked down to the\nground.$"

DESC_THOUSANDWAVES: .string "The user attacks with\na wave that crawls\nalong the ground.\nThose hit can't flee\nfrom battle.$"

DESC_PARABOLICCHARGE: .string "The user attacks\neverything around it.\nThe user's HP is\nrestored by half the\ndamage.$"

DESC_SEARINGSHOT: .string "The user torches\neverything around it\nin an inferno of\nscarlet flames. It\nmay burn the foes.$"

DESC_SEEDFLARE: .string "The user emits a\nshock wave from its\nbody. This may also\nharshly lower the\ntarget's Sp. Def.$"

DESC_ACCELEROCK: .string "The user smashes into\nthe target at high\nspeed. This move\nalways goes first.$"

DESC_ANCHORSHOT: .string "The user entangles\nthe target with its\nanchor chain while \nattacking. The target\ncannot flee.$"

DESC_AURORAVEIL: .string "This move reduces\ndamage from physical\nand special moves for\nfive turns during a\nhailstorm.$"

DESC_BANEFULBUNKER: .string "Aside from protecting\nthe user from attacks,\nthis also poisons\nattackers who make\ncontact.$"

DESC_BEAKBLAST: .string "The user heats up its\nbeak, and attacks the\ntarget. Making contact\nwhile heating up\ncauses a burn.$"

DESC_BRUTALSWING: .string "The user swings its\nbody around violently\nto inflict damage on\neverything in its\nvicinity.$"

DESC_BURNUP: .string "The user burns\nitself out. After\nusing this move,\nthe user will no\nlonger be Fire type.$"

DESC_CLANGINGSCALES: .string "The user rubs its\nscales and makes a\nhuge noise to\nattack. The user's\nDef stat goes down.$"

DESC_DARKESTLARIAT: .string "The user swings both\narms and hits the\ntarget. The target's\nstat changes are\nignored.$"

DESC_DRAGONHAMMER: .string "The user uses its\nbody like a hammer\nto attack the target\nand inflict damage.$"

DESC_FIRELASH: .string "The user strikes the\ntarget with a burning\nlash. This also\nlowers the target's\nDefense stat.$"

DESC_FIRSTIMPRESSION: .string "Although this move\nhas great power, it\nonly works the first\nturn the user is in\nbattle.$"

DESC_FLEURCANNON: .string "The user unleashes a\nstrong beam. The\nattack's recoil\nharshly lowers the\nuser's Sp. Atk stat.$"

DESC_FLORALHEALING: .string "The user restores up\nto half the max HP\nof the target. It\nrestores more HP\nwith grass terrain.$"

DESC_GEARUP: .string "The user engages its\ngears to raise the\nAtk and Sp. Atk of\nallies with Plus or\nMinus Ability.$"

DESC_HIGHHORSEPOWER: .string "The user fiercely\nattacks the target\nusing its entire\nbody.$"

DESC_ICEHAMMER: .string "The user swings\nits heavy fist.\nIt also lowers the\nuser's Speed.$"

DESC_LASERFOCUS: .string "The user concentrates \nintensely. The\nattack on the next\nturn always results\nin a critical hit.$"

DESC_LEAFAGE: .string "The user attacks by\npelting the target\nwith leaves.$"

DESC_LIQUIDATION: .string "The user slams into\nthe foe using a\nfull-force water\nblast. This may lower\nthe foe's Def stat.$"

DESC_LUNGE: .string "The user makes a\nlunge at the foe,\nattacking with full\nforce. This lowers\nthe foe's Atk stat.$"

DESC_MOONGEISTBEAM: .string "The user emits a\nsinister ray. This\ncan be used on the\ntarget regardless\nof its Ability.$"

DESC_MULTIATTACK: .string "Cloaking itself in\nhigh energy, the user\nslams into the foe.\nIts type depends on\nthe memory.$"

DESC_NATURESMADNESS: .string "The user hits the\ntarget with the\nforce of nature. It\nhalves the target's\nHP.$"

DESC_POLLENPUFF: .string "The user attacks the\nfoe with exploding\npollen puff. If\ntarget is an ally, it\nheals instead.$"

DESC_POWERTRIP: .string "The user boasts its\nstrength. The more\nthe user's stats are\nraised, the greater\nthe power.$"

DESC_PRISMATICLASER: .string "The user shoots\npowerful lasers using\nprism power. The\nuser can't move on\nthe next turn.$"

DESC_PSYCHICFANGS: .string "The user bites the\ntarget with psychic\ncapabilities. This\ndestroys Light\nScreen and Reflect.$"

DESC_PURIFY: .string "The user heals the\ntarget's status\ncondition. If the move\nsucceeds, it also\nheals user's own HP.$"

DESC_REVELATIONDANCE: .string "The user attacks the\ntarget by dancing\nvery hard. The user's\ntype determines this\nmove's type.$"

DESC_SHADOWBONE: .string "The user attacks by\nbeating the foe with\na bone that has a\nspirit. It may lower\nthe foe's Def.$"

DESC_SHELLTRAP: .string "The user sets a\nshell trap. If the\nuser is hit by a\nphysical move, the\ntrap will explode.$"

DESC_SHOREUP: .string "The user regains up\nto half of its max\nHP. It restores\nmore HP in a\nsandstorm.$"

DESC_SMARTSTRIKE: .string "The user stabs the\ntarget with a sharp\nhorn. This attack\nnever misses.$"

DESC_SOLARBLADE: .string "In this two-turn move,\nthe user gathers\nlight and fills a\nblade with light's\nenergy to attack.$"

DESC_SPARKLINGARIA: .string "The user bursts into\nsong, emitting\nbubbles. Any burnt\nPokemon will be\nhealed.$"

DESC_SPECTRALTHIEF: .string "The user hides in\nthe target's shadow,\nsteals the target's\nstat boosts,and\nthen attacks.$"

DESC_SPEEDSWAP: .string "The user exchanges\nSpeed stats with\nthe target.$"

DESC_SPIRITSHACKLE: .string "The user attacks\nwhile stitching the\ntarget's shadow to\nthe ground to\nprevent escape.$"

DESC_SPOTLIGHT: .string "The user shines a\nspotlight on the\ntarget so that only\nthe target will be\nattacked.$"

DESC_STOMPINGTANTRUM: .string "If the user's\nprevious moves has\nfailed, the power\nof this move\ndoubles.$"

DESC_STRENGTHSAP: .string "The user restores HP\nby the same amount\nas the target's\nAtk stat. It lowers\nthe target's Atk.$"

DESC_SUNSTEELSTRIKE: .string "The user slams the\ntarget with a meteor\nforce. This move\ndisregards the\ntarget's Ability.$"

DESC_TEARFULLOOK: .string "The user gets teary\neyed to look pitiful.\nThis lowers the\ntarget's Atk and\nSp. Atk stats.$"

DESC_THROATCHOP: .string "The user attacks the\nfoe's throat to\nprevent it from\nusing moves that emit\nsound for two turns.$"

DESC_TOXICTHREAD: .string "The user shoots\npoisonous threads to\npoison the target\nand lower the\ntarget's Speed stat.$"

DESC_TROPKICK: .string "The user lands an\nintense kick of\ntropical origins on\nthe target. It lowers\nthe target's Atk.$"

DESC_ZINGZAP: .string "A strong electric\nblast crashes down\non the target. This\nmay make the target\nflinch.$"

DESC_ELECTRICTERRAIN: .string "The user electrifies\nthe ground for five\nturns, powering up\nElectric moves and\neluding sleep.$"

DESC_GRASSYTERRAIN: .string "The user turns the\nground to grass for\nfive turns, powering\nup Grass moves and\nrestoring HP.$"

DESC_MISTYTERRAIN: .string "This protects Pokémon\nfrom status\nconditions and halves\ndamage from Dragon\nmoves for 5 turns.$"

DESC_PSYCHICTERRAIN: .string "This protects Pokémon\nfrom priority moves\nand powers up\nPsychic moves for\nfive turns.$"

DESC_GRASSPLEDGE: .string "When used with its\nwater equivalent,\nits power increases\nand a vast swamp\nappears.$"

DESC_FIREPLEDGE: .string "When used with its\ngrass equivalent,\nits power increases\nand a vast sea of\nfire appears.$"

DESC_WATERPLEDGE: .string "When combined with\nits fire equivalent,\nits power increases\nand a rainbow\nappears.$"

DESC_TRICKROOM: .string "The user creates a\nbizarre area in\nwhich slower Pokémon\nget to move first\nfor five turns.$"

DESC_MAGICROOM: .string "The user creates a\nbizarre area in\nwhich Pokémon's held\nitems lose effect\nfor five turns.$"

DESC_WONDERROOM: .string "The user creates a\nbizarre area where\nPokémon's Def and\nSp. Def stats are\nswapped for 5 turns.$"

DESC_CRAFTYSHIELD: .string "The user protects\nitself and its\nallies from status\nmoves with a\nmysterious power.$"

DESC_MATBLOCK: .string "Using a pulled-up\nmat as a shield,\nthe user protects\nitself and its allies\nfrom damaging moves.$"

DESC_QUICKGUARD: .string "The user protects\nitself and its\nallies from priority\nmoves. It may fail if\nused in succession.$"

DESC_WIDEGUARD: .string "The user's side is\nprotected from\nwide-ranging attack.\nIt may fail if used\nin succession.$"

DESC_AQUARING: .string "The user envelops\nitself in a veil\nmade of water. It\nregains some HP\nevery turn.$"

DESC_AROMATICMIST: .string "The user raises the\nSp. Def stat of an\nally Pokémon by\nusing a mysterious\naroma.$"

DESC_BELCH: .string "The user lets out a\ndamaging belch at\nthe target. The user\nmust eat a held\nBerry to use this.$"

DESC_BESTOW: .string "The user passes its\nheld item to the\ntarget when the\ntarget isn't\nholding an item.$"

DESC_FLING: .string "The user flings its\nheld item at the\ntarget to attack.\nIts power and effect\ndepend on the item.$"

DESC_FLYINGPRESS: .string "The user dives down\nonto the target from\nthe sky. This move\nis both Fighting\nand Flying type.$"

DESC_GRAVITY: .string "Gravity's intensified\nfor 5 turns, making\nmoves involving\nflying unusable and\nvoiding Levitate.$"

DESC_GUARDSPLIT: .string "The user employs its\npsychic power to\naverage its Def and\nSp. Def stats with\nthe target.$"

DESC_HEALBLOCK: .string "The user prevents\nthe opposing team\nfrom using any HP\nrecovery for five\nturns.$"

DESC_IONDELUGE: .string "The user disperses\nelectrically charged\nparticles, changing\nNormal- to Electric-\ntype moves.$"

DESC_LUCKYCHANT: .string "An incantation is\nchanted to the sky,\npreventing enemy\nfrom landing critical\nhits for 5 turns.$"

DESC_MAGNETRISE: .string "The user levitates\nusing electrically\ngenerated magnetism\nfor five turns.$"

DESC_MAGNETICFLUX: .string "The user manipulates\nmagnets to up Def\nand Sp. Def stats of\nallies with Plus or\nMinus Ability.$"

DESC_POWDER: .string "The user covers the\ntarget in a powder\nthat explodes if it\nuses a Fire-type\nmove.$"

DESC_POWERSPLIT: .string "The user employs its\npsychic power to\naverage its Atk and\nSp. Atk stats with\nthe target.$"

DESC_REFLECTTYPE: .string "The user reflects\nthe target's type,\nmaking it the same\ntype as the target.$"

DESC_SIMPLEBEAM: .string "The user's\nmysterious psychic\nwave changes the\ntarget's Ability to\nSimple.$"

DESC_SOAK: .string "The user shoots a\ntorrent of water at\nthe target and\nchanges the target's\ntype to Water.$"

DESC_TELEKINESIS: .string "The user makes the\ntarget float with\npsychic power. The\ntarget is easier to\nhit for 3 turns.$"

DESC_TRICKORTREAT: .string "Enemy is brought\ntrick-or-treating.\nThis adds Ghost type\nto the target's type.$"

DESC_VENOMDRENCH: .string "Foes are drenched in\nodd poisonous liquid.\nThis lowers the foes'\nAtk, Sp. Atk, and Spd\nstats.$"

DESC_FORESTSCURSE: .string "The user puts a\nforest curse on the\ntarget. The target\nis now Grass type\nas well.$"

DESC_MINDBLOWN: .string "The user attacks\neverything around it\nby causing its own\nhead to explode. This\ndamages self.$"

DESC_PHOTONGEYSER: .string "The user attacks a\nfoe with pillar of\nlight. It inflicts\nthe higher attack\nstat of the user.$"

DESC_PLASMAFISTS: .string "The user attacks\nwith electrically\ncharged fists. This\nchanges Normal- to\nElectric-type moves.$"

DESC_ELECTRIFY: .string "If the target is\nelectrified before\nits move in the turn,\nthe target's move\nbecomes Electric.$"

DESC_EMBARGO: .string " $"

DESC_FAIRYLOCK: .string "By locking down the\nbattlefield, the\nuser keeps all\nPokémon from fleeing\nduring the next turn.$"

DESC_LIGHTOFRUIN: .string "The user fires a\npowerful beam of\nlight from the\nEternal Flower. This\ndamages the user.$"

DESC_TRUMPCARD: .string "The fewer PP this\nmove has, the\ngreater its power.$"

DESC_AFTERYOU: .string "The user helps the\ntarget and makes it\nuse its move right\nafter the user.$"

DESC_INSTRUCT: .string "The user instructs\nthe target to use\nthe target's last\nmove again.$"

DESC_QUASH: .string "The user suppresses\nthe target and makes\nits move go last.$"

DESC_HAPPYHOUR: .string "Using Happy Hour\ndoubles the amount\nof prize money\nreceived after\nbattle.$"

DESC_DOUBLEIRONBASH: .string " $"

DESC_LASTRESORT: .string " $"

DESC_SKYDROP: .string " $"

DESC_CELEBRATE: .string " $"

DESC_HOLDHANDS: .string " $"

DESC_BLANK: .byte 0xFF
*/