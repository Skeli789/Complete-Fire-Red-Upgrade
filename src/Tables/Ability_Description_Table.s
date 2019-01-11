/*.thumb
.text
.align 2
.global AbilityDescriptionTable

.loadtable "..\\charmap.tbl"

AbilityDescriptionTable:
.word 0x824f3c4		@ABILITY_NONE
.word 0x824f3d8		@ABILITY_STENCH
.word 0x824f3f2		@ABILITY_DRIZZLE
.word 0x824f40a		@ABILITY_SPEEDBOOST
.word 0x824f422		@ABILITY_BATTLEARMOR
.word 0x824f438		@ABILITY_STURDY
.word 0x824f452		@ABILITY_DAMP
.word 0x824f46d		@ABILITY_LIMBER
.word 0x824f481		@ABILITY_SANDVEIL
.word 0x824f49d		@ABILITY_STATIC
.word 0x824f4b3		@ABILITY_VOLTABSORB
.word 0x824f4ce		@ABILITY_WATERABSORB
.word 0x824f4e5		@ABILITY_OBLIVIOUS
.word 0x824f4fa		@ABILITY_CLOUDNINE
.word 0x824f513		@ABILITY_COMPOUNDEYES
.word 0x824f524		@ABILITY_INSOMNIA
.word 0x824f534		@ABILITY_COLORCHANGE
.word 0x824f550		@ABILITY_IMMUNITY
.word 0x824f564		@ABILITY_FLASHFIRE
.word 0x824f57e		@ABILITY_SHIELDDUST
.word 0x824f596		@ABILITY_OWNTEMPO
.word 0x824f5aa		@ABILITY_SUCTIONCUPS
.word 0x824f5c3		@ABILITY_INTIMIDATE
.word 0x824f5dc		@ABILITY_SHADOWTAG
.word 0x824f5f7		@ABILITY_ROUGHSKIN
.word 0x824f607		@ABILITY_WONDERGUARD
.word 0x824f61f		@ABILITY_LEVITATE
.word 0x824f63a		@ABILITY_EFFECTSPORE
.word 0x824f654		@ABILITY_SYNCHRONIZE
.word 0x824f66f		@ABILITY_CLEARBODY
.word 0x824f68b		@ABILITY_NATURALCURE
.word 0x824f6a5		@ABILITY_LIGHTNINGROD
.word 0x824f6bd		@ABILITY_SERENEGRACE
.word 0x824f6d5		@ABILITY_SWIFTSWIM
.word 0x824f6eb		@ABILITY_CHLOROPHYLL
.word 0x824f705		@ABILITY_ILLUMINATE
.word 0x824f71f		@ABILITY_TRACE
.word 0x824f737		@ABILITY_HUGEPOWER
.word 0x824f746		@ABILITY_POISONPOINT
.word 0x824f75e		@ABILITY_INNERFOCUS
.word 0x824f772		@ABILITY_MAGMAARMOR
.word 0x824f785		@ABILITY_WATERVEIL
.word 0x824f795		@ABILITY_MAGNETPULL
.word 0x824f7af		@ABILITY_SOUNDPROOF
.word 0x824f7c9		@ABILITY_RAINDISH
.word 0x824f7e5		@ABILITY_SANDSTREAM
.word 0x824f7fa		@ABILITY_PRESSURE
.word 0x824f811		@ABILITY_THICKFAT
.word 0x824f82b		@ABILITY_EARLYBIRD
.word 0x824f847		@ABILITY_FLAMEBODY
.word 0x824f861		@ABILITY_RUNAWAY
.word 0x824f878		@ABILITY_KEENEYE
.word 0x824f893		@ABILITY_HYPERCUTTER
.word 0x824f8ae		@ABILITY_PICKUP
.word 0x824f8c1		@ABILITY_TRUANT
.word 0x824f8dd		@ABILITY_HUSTLE
.word 0x824f8f8		@ABILITY_CUTECHARM
.word 0x824f90f		@ABILITY_PLUS
.word 0x824f925		@ABILITY_MINUS
.word 0x824f93a		@ABILITY_FORECAST
.word 0x824f954		@ABILITY_STICKYHOLD
.word 0x824f969		@ABILITY_SHEDSKIN
.word 0x824f985		@ABILITY_GUTS
.word 0x824f99e		@ABILITY_MARVELSCALE
.word 0x824f9b8		@ABILITY_LIQUIDOOZE
.word 0x824f9d0		@ABILITY_OVERGROW
.word 0x824f9ec		@ABILITY_BLAZE
.word 0x824fa07		@ABILITY_TORRENT
.word 0x824fa23		@ABILITY_SWARM
.word 0x824fa3d		@ABILITY_ROCKHEAD
.word 0x824fa55		@ABILITY_DROUGHT
.word 0x824fa71		@ABILITY_ARENATRAP
.word 0x824fa83		@ABILITY_VITALSPIRIT
.word 0x824fa93		@ABILITY_WHITESMOKE
.word 0x824faaf		@ABILITY_PUREPOWER
.word 0x824fabe		@ABILITY_SHELLARMOR
.word 0x824fad4		@ABILITY_CACOPHONY
.word 0x824faee		@ABILITY_AIRLOCK

.word DESC_SWEETVEIL
.word DESC_SKILLLINK
.word DESC_MOTORDRIVE
.word DESC_MULTISCALE
.word DESC_TECHNICIAN
.word DESC_SCRAPPY
.word DESC_SUPERLUCK
.word DESC_SNIPER
.word DESC_REGENERATOR
.word DESC_PRANKSTER
.word DESC_ADAPTABILITY
.word DESC_BIGPECKS
.word DESC_MAGICBOUNCE
.word DESC_RECKLESS
.word DESC_SHEERFORCE
.word DESC_IRONFIST
.word DESC_RIVALRY
.word DESC_SANDFORCE
.word DESC_SOLARPOWER
.word DESC_HEATPROOF
.word DESC_DRYSKIN
.word DESC_TINTEDLENS
.word DESC_SOLIDROCK
.word DESC_FILTER
.word DESC_INFILTRATOR
.word DESC_DOWNLOAD
.word DESC_POISONHEAL
.word DESC_ICEBODY
.word DESC_MOODY
.word DESC_HYDRATION
.word DESC_HEALER
.word DESC_SNOWCLOAK
.word DESC_TANGLEDFEET
.word DESC_SNOWWARNING
.word DESC_QUICKFEET
.word DESC_SAPSIPPER
.word DESC_OVERCOAT
.word DESC_MAGICGUARD
.word DESC_BULLETPROOF
.word DESC_GALEWINGS
.word DESC_MOXIE
.word DESC_AFTERMATH
.word DESC_CURSEDBODY
.word DESC_GOOEY
.word DESC_MUMMY
.word DESC_IRONBARBS
.word DESC_SANDRUSH
.word DESC_ANALYTIC
.word DESC_NOGUARD
.word DESC_MEGALAUNCHER
.word DESC_TOUGHCLAWS
.word DESC_STRONGJAW
.word DESC_VICTORYSTAR
.word DESC_STORMDRAIN
.word DESC_DARKAURA
.word DESC_FAIRYAURA
.word DESC_AURABREAK
.word DESC_REFRIGERATE
.word DESC_PIXILATE
.word DESC_AERILATE
.word DESC_NORMALIZE
.word DESC_UNBURDEN
.word DESC_SIMPLE
.word DESC_UNAWARE
.word DESC_DEFIANT
.word DESC_COMPETITIVE
.word DESC_DEFEATIST
.word DESC_SLOWSTART
.word DESC_TOXICBOOST
.word DESC_FLAREBOOST
.word DESC_FURCOAT
.word DESC_WONDERSKIN
.word DESC_PROTEAN
.word DESC_PARENTALBOND
.word DESC_MOLDBREAKER
.word DESC_TURBOBLAZE
.word DESC_TERAVOLT
.word DESC_ZENMODE
.word DESC_BATTLEBOND
.word DESC_BEASTBOOST
.word DESC_CORROSION
.word DESC_DISGUISE
.word DESC_EMERGENCYEXIT
.word DESC_FLUFFY
.word DESC_FULLMETALBODY
.word DESC_BROKEN_1
.word DESC_BROKEN_2
.word DESC_POWERCONSTRUCT
.word DESC_PRISMARMOR
.word DESC_RKS_SYSTEM
.word DESC_SCHOOLING
.word DESC_SHADOWSHIELD
.word DESC_SHIELDSDOWN
.word DESC_SLUSHRUSH
.word DESC_SOULHEART
.word DESC_STAMINA
.word DESC_STEELWORKER
.word DESC_TANGLINGHAIR
.word DESC_TRIAGE
.word DESC_WATERBUBBLE
.word DESC_WATERCOMPACTION
.word DESC_STALL
.word DESC_MULTITYPE
.word DESC_ELECTRICSURGE
.word DESC_GRASSYSURGE
.word DESC_MISTYSURGE
.word DESC_PSYCHICSURGE
.word DESC_SURGESURFER
.word DESC_GRASSPELT
.word DESC_ANGERPOINT
.word DESC_ANTICIPATION
.word DESC_FOREWARN
.word DESC_FRISK
.word DESC_CONTRARY
.word DESC_UNNERVE
.word DESC_WEAKARMOR
.word DESC_HEAVYMETAL
.word DESC_LIGHTMETAL
.word DESC_STEADFAST
.word DESC_IMPOSTER
.word DESC_JUSTIFIED
.word DESC_RATTLED
.word DESC_MERCILESS
.word DESC_AROMAVEIL
.word DESC_FLOWERVEIL
.word DESC_LEAFGUARD
.word DESC_KLUTZ
.word DESC_FLOWERGIFT
.word DESC_BADDREAMS
.word DESC_PICKPOCKET
.word DESC_PORTALPOWER
.word DESC_POISONTOUCH
.word DESC_MAGICIAN
.word DESC_STANCECHANGE
.word DESC_PRIMORDIALSEA
.word DESC_DESOLATELAND
.word DESC_DELTASTREAM
.word DESC_WIMPOUT
.word DESC_BERSERK
.word DESC_LONGREACH
.word DESC_LIQUIDVOICE
.word DESC_QUEENLYMAJESTY
.word DESC_INNARDSOUT
.word DESC_DAZZLING
.word DESC_GLUTTONY
.word DESC_HONEYGATHER
.word DESC_FRIENDGUARD
.word DESC_HARVEST
.word DESC_TELEPATHY
.word DESC_ILLUSION
.word DESC_CHEEKPOUCH
.word DESC_SYMBIOSIS
.word DESC_STAKEOUT
.word DESC_COMATOSE
.word DESC_DANCER
.word DESC_BATTERY
.word DESC_RECEIVER
.word DESC_POWEROFALCHEMY
.word DESC_NEUROFORCE
.word DESC_GALVANIZE

.align 2
DESC_SWEETVEIL: .string "Keeps user and allies awake.$"

DESC_SKILLLINK: .string "Multi-hit moves hit five times.$"

DESC_MOTORDRIVE: .string "Turns electricity into Speed.$"

DESC_MULTISCALE: .string "Reduces damage at full HP.$"

DESC_TECHNICIAN: .string "Boosts weaker moves.$"

DESC_SCRAPPY: .string "Moves hit Ghost-type Pokémon.$"

DESC_SUPERLUCK: .string "Critical hits land more often.$"

DESC_SNIPER: .string "Boosts critical hits.$"

DESC_REGENERATOR: .string "Heals upon switching out.$"

DESC_PRANKSTER: .string "Status moves hit first.$"

DESC_ADAPTABILITY: .string "Boosts same type attacks.$"

DESC_BIGPECKS: .string "Prevents Defense reduction.$"

DESC_MAGICBOUNCE: .string "Reflects status moves.$"

DESC_RECKLESS: .string "Boosts moves with recoil.$"

DESC_SHEERFORCE: .string "Trades added effects for power.$"

DESC_IRONFIST: .string "Boosts punching moves.$"

DESC_RIVALRY: .string "Boosts moves against rivals.$"

DESC_SANDFORCE: .string "Boosts moves in a sandstorm.$"

DESC_SOLARPOWER: .string "Trades HP for power in sunshine.$"

DESC_HEATPROOF: .string "Heat protection.$"

DESC_DRYSKIN: .string "Prefers the rain to heat.$"

DESC_TINTEDLENS: .string "Boosts not very effective moves.$"

DESC_SOLIDROCK: .string "Weakens Super Effective moves.$"

DESC_FILTER: .string "Weakens Super Effective moves.$"

DESC_INFILTRATOR: .string "Passes through barriers.$"

DESC_DOWNLOAD: .string "Adjusts power from foe's stats.$"

DESC_POISONHEAL: .string "Restores HP if poisoned.$"

DESC_ICEBODY: .string "Slight HP recovery in hail.$"

DESC_MOODY: .string "Raises one stat and lowers another.$"

DESC_HYDRATION: .string "Heals status problems in rain.$"

DESC_HEALER: .string "May heal ally's status problems.$"

DESC_SNOWCLOAK: .string "Ups evasion in hail.$"

DESC_TANGLEDFEET: .string "Ups evasion if confused.$"

DESC_SNOWWARNING: .string "Summons a hailstorm in battle.$"

DESC_QUICKFEET: .string "Ups Speed if suffering.$"

DESC_SAPSIPPER: .string "Boosts Attack if hit by Grass.$"

DESC_OVERCOAT: .string "Immune to weather and powder.$"

DESC_MAGICGUARD: .string "Only damaged by attacks.$"

DESC_BULLETPROOF: .string "Avoids ball and bomb moves.$"

DESC_GALEWINGS: .string "Flying moves hit first.$"

DESC_MOXIE: .string "Knockouts boost Attack.$"

DESC_AFTERMATH: .string "Fainting damages the foe.$"

DESC_CURSEDBODY: .string "May disable a foe's move.$"

DESC_GOOEY: .string "Lowers the foe's Speed if hit.$"

DESC_MUMMY: .string "Spreads with contact.$"

DESC_IRONBARBS: .string "Hurts to touch.$"

DESC_SANDRUSH: .string "Raises Speed in a sandstorm.$"

DESC_ANALYTIC: .string "Moving last boosts power.$"

DESC_NOGUARD: .string "Ensures that all moves land.$"

DESC_MEGALAUNCHER: .string "Boosts aura and pulse moves.$"

DESC_TOUGHCLAWS: .string "Boosts moves that make contact.$"

DESC_STRONGJAW: .string "Boosts biting moves.$"

DESC_VICTORYSTAR: .string "Raises accuracy.$"

DESC_STORMDRAIN: .string "Draws in Water-type moves.$"

DESC_DARKAURA: .string "Powers up Dark-type moves.$"

DESC_FAIRYAURA: .string "Powers up Fairy-type moves.$"

DESC_AURABREAK: .string "Reverses «Aura» abilities.$"

DESC_REFRIGERATE: .string "Normal- moves become Ice-type.$"

DESC_PIXILATE: .string "Normal- moves become Fairy-type.$"

DESC_AERILATE: .string "Normal- moves become Flying-type.$"

DESC_NORMALIZE: .string "All moves become Normal-type.$"

DESC_UNBURDEN: .string "Doubles speed if item is consumed.$"

DESC_SIMPLE: .string "Doubles stat changes.$"

DESC_UNAWARE: .string "Ignores foe's stat changes.$"

DESC_DEFIANT: .string "Attack rises when stats fall.$"

DESC_COMPETITIVE: .string "Sp. Atk rises when stats fall.$"

DESC_DEFEATIST: .string "Stats fall when HP goes below half.$"

DESC_SLOWSTART: .string "Temporarily halves Atk and Spd.\n$"

DESC_TOXICBOOST: .string "Attack rises when poisoned.$"

DESC_FLAREBOOST: .string "Sp. Atk rises when burned.$"

DESC_FURCOAT: .string "Halves damage from physical moves.$"

DESC_WONDERSKIN: .string "Ups status move evasion.$"

DESC_PROTEAN: .string "Changes type to match move.$"

DESC_PARENTALBOND: .string "Parent and child attack together.$"

DESC_MOLDBREAKER: .string "Abilities don't affect moves.$"

DESC_TURBOBLAZE: .string "Abilities don't affect moves.$"

DESC_TERAVOLT: .string "Abilities don't affect moves.$"

DESC_ZENMODE: .string "Changes the Pokemon into Zen Form$"

DESC_BATTLEBOND: .string "KO a foe to tranform.$"

DESC_BEASTBOOST: .string "KO a foe to up the highest stat.$"

DESC_CORROSION: .string "Poison Steel and Poison-types.$"

DESC_DISGUISE: .string "Takes one hit with no damage.$"

DESC_EMERGENCYEXIT: .string "Switch out below half health.$"

DESC_FLUFFY: .string "Halves contact damage.$"

DESC_FULLMETALBODY: .string "Prevents stat reduction.$"

DESC_BROKEN_1: .string "No special ability.$"

DESC_BROKEN_2: .string "No special ability.$"

DESC_POWERCONSTRUCT: .string "Transform below half health.$"

DESC_PRISMARMOR: .string "Weakens Super Effective moves.$"

DESC_RKS_SYSTEM: .string "Changes type to match Memory.$"

DESC_SCHOOLING: .string "Schools up with high HP.$"

DESC_SHADOWSHIELD: .string "Reduces damage at full HP.$"

DESC_SHIELDSDOWN: .string "Lowers shields below half health.$"

DESC_SLUSHRUSH: .string "Boosts speed in Hail.$"

DESC_SOULHEART: .string "Knockouts boost Sp. Atk.$"

DESC_STAMINA: .string "Taking a hit raises Defense.$"

DESC_STEELWORKER: .string "Powers up Steel-type moves.$"

DESC_TANGLINGHAIR: .string "Lowers the foe's Speed if hit.$"

DESC_TRIAGE: .string "Gives priority to healing moves.$"

DESC_WATERBUBBLE: .string "Ups water attacks. Prevents burn.$"

DESC_WATERCOMPACTION: .string "Water-type hits raise Defense.$"

DESC_STALL: .string "Move after even slower foes.$"

DESC_MULTITYPE: .string "Changes type to match held plate.$"

DESC_ELECTRICSURGE: .string "Creates an Electric Terrain.$"

DESC_GRASSYSURGE: .string "Creates a Grassy Terrain.$"

DESC_MISTYSURGE: .string "Creates a Misty Terrain.$"

DESC_PSYCHICSURGE: .string "Creates a Psychic Terrain.$"

DESC_SURGESURFER: .string "Boosts speed on Electric Terrain.$"

DESC_GRASSPELT: .string "Ups Defense on Grassy Terrain.$"

DESC_ANGERPOINT: .string "Taking a critical-hit maxes Atk.$"

DESC_ANTICIPATION: .string "Senses Super-Effective moves.$"

DESC_FOREWARN: .string "Warns of the foe's strongest move.$"

DESC_FRISK: .string "Reveals the foe's held item.$"

DESC_CONTRARY: .string "Reverses stats changes.$"

DESC_UNNERVE: .string "Stops the foe from eating berries.$"

DESC_WEAKARMOR: .string "Contact hits raise its stats.$"

DESC_HEAVYMETAL: .string "Doubles weight.$"

DESC_LIGHTMETAL: .string "Halves weight.$"

DESC_STEADFAST: .string "Flinching raises Speed.$"

DESC_IMPOSTER: .string "Transforms upon entering battle.$"

DESC_JUSTIFIED: .string "Dark-type hits raise Attack.$"

DESC_RATTLED: .string "Ghost, Bug, Dark hits raise Spd.$"

DESC_MERCILESS: .string "Lands crit-hits on poisoned foes.$"

DESC_AROMAVEIL: .string "Protects against mental problems.$"

DESC_FLOWERVEIL: .string "Protects Grass-types from status.$"

DESC_LEAFGUARD: .string "Protects from status when sunny.$"

DESC_KLUTZ: .string "Can't use items.$"

DESC_FLOWERGIFT: .string "Powers up team when sunny.$"

DESC_BADDREAMS: .string "Hurts sleeping foes.$"

DESC_PICKPOCKET: .string "Steals foe's item when hit.$"

DESC_PORTALPOWER: .string "Weakens non-contact moves.$"

DESC_POISONTOUCH: .string "Can poison foe on contact.$"

DESC_MAGICIAN: .string "Can steal foe's item after attack.$"

DESC_STANCECHANGE: .string "Switches form when attacking.$"

DESC_PRIMORDIALSEA: .string "Summons a heavy rainstorm.$"

DESC_DESOLATELAND: .string "Summons extremely harsh sunlight.$"

DESC_DELTASTREAM: .string "Summons a mysterious air current.$"

DESC_WIMPOUT: .string "Switch out below half health.$"

DESC_BERSERK: .string "Ups Sp. Atk when HP halves.$"

DESC_LONGREACH: .string "Never makes contact.$"

DESC_LIQUIDVOICE: .string "Sound moves become water-type.$"

DESC_QUEENLYMAJESTY: .string "Blocks priority moves.$"

DESC_INNARDSOUT: .string "Fainting damages the foe.$"

DESC_DAZZLING: .string "Blocks priority moves.$"

DESC_GLUTTONY: .string "Eats berries early.$"

DESC_HONEYGATHER: .string "Picks up Honey after battle.$"

DESC_FRIENDGUARD: .string "Raises allies' Defense.$"

DESC_HARVEST: .string "Can regain consumed berries.$"

DESC_TELEPATHY: .string "Protects from allies' moves.$"

DESC_ILLUSION: .string "Enters battle disguised.$"

DESC_CHEEKPOUCH: .string "Eating berries restores health.$"

DESC_SYMBIOSIS: .string "Gives item to ally after use.$"

DESC_STAKEOUT: .string "Doubles damage on switch ins.$"

DESC_COMATOSE: .string "Acts as if asleep.$"

DESC_DANCER: .string "Copies dance moves.$"

DESC_BATTERY: .string "Powers up allies.$"

DESC_RECEIVER: .string "Takes over a KOed ally's ability.$"

DESC_POWEROFALCHEMY: .string "Takes over a KOed ally's ability.$"

DESC_NEUROFORCE: .string "Powers up Super-Effective hits.$"

DESC_GALVANIZE: .string "Normal moves become Electric-type.$"

*/