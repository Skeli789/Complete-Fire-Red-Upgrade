.thumb
.global AbilityDamageListTable

.include "..\\defines"

.equ ITEM_TABLES_TERMIN, 0xFEFE

.global NaturalGiftTable
.global FlingTable
.global ConsumableItemEffectTable

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

NaturalGiftTable:
.hword ITEM_CHERIBERRY
.byte TYPE_FIRE, 80

.hword ITEM_CHESTOBERRY
.byte TYPE_WATER, 80

.hword ITEM_PECHABERRY
.byte TYPE_ELECTRIC, 80

.hword ITEM_RAWSTBERRY
.byte TYPE_GRASS, 80

.hword ITEM_ASPEARBERRY
.byte TYPE_ICE, 80

.hword ITEM_LEPPABERRY
.byte TYPE_FIGHTING, 80

.hword ITEM_ORANBERRY
.byte TYPE_POISON, 80

.hword ITEM_PERSIMBERRY
.byte TYPE_GROUND, 80

.hword ITEM_LUMBERRY
.byte TYPE_FLYING, 80

.hword ITEM_SITRUSBERRY
.byte TYPE_PSYCHIC, 80

.hword ITEM_FIGYBERRY
.byte TYPE_BUG, 80

.hword ITEM_WIKIBERRY
.byte TYPE_ROCK, 80

.hword ITEM_MAGOBERRY
.byte TYPE_GHOST, 80

.hword ITEM_AGUAVBERRY
.byte TYPE_DRAGON, 80

.hword ITEM_IAPAPABERRY
.byte TYPE_DARK, 80

.hword ITEM_RAZZBERRY
.byte TYPE_STEEL, 80

.hword ITEM_BLUKBERRY
.byte TYPE_FIRE, 90

.hword ITEM_NANABBERRY
.byte TYPE_WATER, 90

.hword ITEM_WEPEARBERRY
.byte TYPE_ELECTRIC, 90

.hword ITEM_PINAPBERRY
.byte TYPE_GRASS, 90

.hword ITEM_POMEGBERRY
.byte TYPE_ICE, 90

.hword ITEM_KELPSYBERRY
.byte TYPE_FIGHTING, 90

.hword ITEM_QUALOTBERRY
.byte TYPE_POISON, 90

.hword ITEM_HONDEWBERRY
.byte TYPE_GROUND, 90

.hword ITEM_GREPABERRY
.byte TYPE_FLYING, 90

.hword ITEM_TAMATOBERRY
.byte TYPE_PSYCHIC, 90

.hword ITEM_CORNNBERRY
.byte TYPE_BUG, 90

.hword ITEM_MAGOSTBERRY
.byte TYPE_ROCK, 90

.hword ITEM_RABUTABERRY
.byte TYPE_GHOST, 90

.hword ITEM_NOMELBERRY
.byte TYPE_DRAGON, 90

.hword ITEM_SPELONBERRY
.byte TYPE_DARK, 90

.hword ITEM_PAMTREBERRY
.byte TYPE_STEEL, 90

.hword ITEM_WATMELBERRY
.byte TYPE_FIRE, 100

.hword ITEM_DURINBERRY
.byte TYPE_WATER, 100

.hword ITEM_BELUEBERRY
.byte TYPE_ELECTRIC, 100

.hword ITEM_OCCABERRY
.byte TYPE_FIRE, 80

.hword ITEM_PASSHOBERRY
.byte TYPE_WATER, 80

.hword ITEM_WACANBERRY
.byte TYPE_ELECTRIC, 80

.hword ITEM_RINDOBERRY
.byte TYPE_GRASS, 80

.hword ITEM_YACHEBERRY
.byte TYPE_ICE, 80

.hword ITEM_CHOPLEBERRY
.byte TYPE_FIGHTING, 80

.hword ITEM_KEBIABERRY
.byte TYPE_POISON, 80

.hword ITEM_SHUCABERRY
.byte TYPE_GROUND, 80

.hword ITEM_COBABERRY
.byte TYPE_FLYING, 80

.hword ITEM_PAYAPABERRY
.byte TYPE_PSYCHIC, 80

.hword ITEM_TANGABERRY
.byte TYPE_BUG, 80

.hword ITEM_CHARTIBERRY
.byte TYPE_ROCK, 80

.hword ITEM_KASIBBERRY
.byte TYPE_GHOST, 80

.hword ITEM_HABANBERRY
.byte TYPE_DRAGON, 80

.hword ITEM_COLBURBERRY
.byte TYPE_DARK, 80

.hword ITEM_BABIRIBERRY
.byte TYPE_STEEL, 80

.hword ITEM_CHILANBERRY
.byte TYPE_NORMAL, 80

.hword ITEM_LIECHIBERRY
.byte TYPE_GRASS, 100

.hword ITEM_GANLONBERRY
.byte TYPE_ICE, 100

.hword ITEM_SALACBERRY
.byte TYPE_FIGHTING, 100

.hword ITEM_PETAYABERRY
.byte TYPE_POISON, 100

.hword ITEM_APICOTBERRY
.byte TYPE_GROUND, 100

.hword ITEM_LANSATBERRY
.byte TYPE_FLYING, 100

.hword ITEM_STARFBERRY
.byte TYPE_PSYCHIC, 100

.hword ITEM_ENIGMABERRY
.byte TYPE_BUG, 100

.hword ITEM_MICLEBERRY
.byte TYPE_ROCK, 100

.hword ITEM_CUSTAPBERRY
.byte TYPE_GHOST, 100

.hword ITEM_JABOCABERRY
.byte TYPE_DRAGON, 100

.hword ITEM_ROWAPBERRY
.byte TYPE_DARK, 100

.hword ITEM_ROSELIBERRY
.byte TYPE_FAIRY, 100

.hword ITEM_KEEBERRY
.byte TYPE_FAIRY, 100

.hword ITEM_MARANGABERRY
.byte TYPE_DARK, 100

.hword ITEM_TABLES_TERMIN
.hword ITEM_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

FlingTable:
.hword ITEM_CHOICEBAND
.byte 10, 0

.hword ITEM_BRIGHTPOWDER
.byte 10, 0

.hword ITEM_FOCUSBAND
.byte 10, 0

.hword ITEM_LEFTOVERS
.byte 10, 0

.hword ITEM_MENTALHERB
.byte 10, 0

.hword ITEM_METALPOWDER
.byte 10, 0

.hword ITEM_SILKSCARF
.byte 10, 0

.hword ITEM_SILVERPOWDER
.byte 10, 0

.hword ITEM_SOFTSAND
.byte 10, 0

.hword ITEM_SOOTHEBELL
.byte 10, 0

.hword ITEM_WHITEHERB
.byte 10, 0xFF

.hword ITEM_ANTIDOTE
.byte 30, 0

.hword ITEM_PARLYZHEAL
.byte 30, 0

.hword ITEM_AWAKENING
.byte 30, 0

.hword ITEM_BURNHEAL
.byte 30, 0

.hword ITEM_ICEHEAL
.byte 30, 0

.hword ITEM_FULLHEAL
.byte 30, 0

.hword ITEM_FULLRESTORE
.byte 30, 0

.hword ITEM_LAVACOOKIE
.byte 30, 0

.hword ITEM_POTION
.byte 30, 0

.hword ITEM_SUPERPOTION
.byte 30, 0

.hword ITEM_HYPERPOTION
.byte 30, 0

.hword ITEM_MAXPOTION
.byte 30, 0

.hword ITEM_HEALPOWDER
.byte 30, 0

.hword ITEM_ENERGYPOWDER
.byte 30, 0

.hword ITEM_ENERGYROOT
.byte 30, 0

.hword ITEM_REVIVALHERB
.byte 30, 0

.hword ITEM_REVIVE
.byte 30, 0

.hword ITEM_MAXREVIVE
.byte 30, 0

.hword ITEM_SODAPOP
.byte 30, 0

.hword ITEM_LEMONADE
.byte 30, 0

.hword ITEM_SODAPOP
.byte 30, 0

.hword ITEM_MOOMOOMILK
.byte 30, 0

.hword ITEM_BERRYJUICE
.byte 30, 0

.hword ITEM_ETHER
.byte 30, 0

.hword ITEM_MAXETHER
.byte 30, 0

.hword ITEM_ELIXIR
.byte 30, 0

.hword ITEM_MAXELIXIR
.byte 30, 0

.hword ITEM_REPEL
.byte 30, 0

.hword ITEM_SUPERREPEL
.byte 30, 0

.hword ITEM_MAXREPEL
.byte 30, 0

.hword ITEM_PROTEIN
.byte 30, 0

.hword ITEM_ZINC
.byte 30, 0

.hword ITEM_CALCIUM
.byte 30, 0

.hword ITEM_CARBOS
.byte 30, 0

.hword ITEM_HPUP
.byte 30, 0

.hword ITEM_PPUP
.byte 30, 0

.hword ITEM_PPMAX
.byte 30, 0

.hword ITEM_REDSHARD
.byte 30, 0

.hword ITEM_BLUESHARD
.byte 30, 0

.hword ITEM_YELLOWSHARD
.byte 30, 0

.hword ITEM_GREENSHARD
.byte 30, 0

.hword ITEM_XACCURACY
.byte 30, 0

.hword ITEM_XDEFEND
.byte 30, 0

.hword ITEM_XSPEED
.byte 30, 0

.hword ITEM_XATTACK
.byte 30, 0

.hword ITEM_XSPECIAL
.byte 30, 0

.hword ITEM_YELLOWFLUTE
.byte 30, 0

.hword ITEM_BLACKFLUTE
.byte 30, 0

.hword ITEM_WHITEFLUTE
.byte 30, 0

.hword ITEM_REDFLUTE
.byte 30, 0

.hword ITEM_BLUEFLUTE
.byte 30, 0

.hword ITEM_AMULETCOIN
.byte 30, 0

.hword ITEM_BIGMUSHROOM
.byte 30, 0

.hword ITEM_BIGPEARL
.byte 30, 0

.hword ITEM_BLACKBELT
.byte 30, 0

.hword ITEM_CHARCOAL
.byte 30, 0

.hword ITEM_CLEANSETAG
.byte 30, 0

.hword ITEM_DEEPSEASCALE
.byte 30, 0

.hword ITEM_DRAGONSCALE
.byte 30, 0

.hword ITEM_ESCAPEROPE
.byte 30, 0

.hword ITEM_EVERSTONE
.byte 30, 0

.hword ITEM_EXPSHARE
.byte 30, 0

.hword ITEM_FIRESTONE
.byte 30, 0

.hword ITEM_FLUFFYTAIL
.byte 30, 0

.hword ITEM_HEARTSCALE
.byte 30, 0

.hword ITEM_KINGSROCK
.byte 30, 8

.hword ITEM_LEAFSTONE
.byte 30, 0

.hword ITEM_LIGHTBALL
.byte 30, 5

.hword ITEM_LUCKYEGG
.byte 30, 0

.hword ITEM_MAGNET
.byte 30, 0

.hword ITEM_METALCOAT
.byte 30, 0

.hword ITEM_MIRACLESEED
.byte 30, 0

.hword ITEM_MOONSTONE
.byte 30, 0

.hword ITEM_MYSTICWATER
.byte 30, 0

.hword ITEM_NEVERMELTICE
.byte 30, 0

.hword ITEM_NUGGET
.byte 30, 0

.hword ITEM_PEARL
.byte 30, 0

.hword ITEM_POKEDOLL
.byte 30, 0

.hword ITEM_SACREDASH
.byte 30, 0

.hword ITEM_SCOPELENS
.byte 30, 0

.hword ITEM_SHELLBELL
.byte 30, 0

.hword ITEM_SHOALSALT
.byte 30, 0

.hword ITEM_SHOALSHELL
.byte 30, 0

.hword ITEM_SMOKEBALL
.byte 30, 0

.hword ITEM_SOULDEW
.byte 30, 0

.hword ITEM_SPELLTAG
.byte 30, 0

.hword ITEM_STARDUST
.byte 30, 0

.hword ITEM_STARPIECE
.byte 30, 0

.hword ITEM_SUNSTONE
.byte 30, 0

.hword ITEM_THUNDERSTONE
.byte 30, 0

.hword ITEM_TINYMUSHROOM
.byte 30, 0

.hword ITEM_TWISTEDSPOON
.byte 30, 0

.hword ITEM_UPGRADE
.byte 30, 0

.hword ITEM_WATERSTONE
.byte 30, 0

.hword ITEM_LUCKYPUNCH
.byte 40, 0

.hword ITEM_SHARPBEAK
.byte 50, 0

.hword ITEM_MACHOBRACE
.byte 60, 0

.hword ITEM_STICK
.byte 60, 0

.hword ITEM_DRAGONFANG
.byte 70, 0

.hword ITEM_POISONBARB
.byte 70, 2

.hword ITEM_QUICKCLAW
.byte 80, 0

.hword ITEM_DEEPSEATOOTH
.byte 90, 0

.hword ITEM_THICKCLUB
.byte 90, 0

.hword ITEM_DOMEFOSSIL
.byte 100, 0

.hword ITEM_HELIXFOSSIL
.byte 100, 0

.hword ITEM_OLDAMBER
.byte 100, 0

.hword ITEM_ROOTFOSSIL
.byte 100, 0

.hword ITEM_CLAWFOSSIL
.byte 100, 0

.hword ITEM_TABLES_TERMIN
.hword ITEM_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@TODO: Make look nice
ConsumableItemEffectTable:
.byte 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x17, 0x1C, 85, 94, 95, 96, 102, 103, 105, 106, 109, 113, 116, 120, 121, 122, 123, 125, 126, 127, 128, 0xFF
