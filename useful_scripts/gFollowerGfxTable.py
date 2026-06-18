import re

# Your input species map — paste it below
input_species_block = """
#define SPECIES_CHESPIN 0x2F6
#define SPECIES_QUILLADIN 0x2F7
#define SPECIES_CHESNAUGHT 0x2F8
#define SPECIES_FENNEKIN 0x2F9
#define SPECIES_BRAIXEN 0x2FA
#define SPECIES_DELPHOX 0x2FB
#define SPECIES_FROAKIE 0x2FC
#define SPECIES_FROGADIER 0x2FD
#define SPECIES_GRENINJA 0x2FE
#define SPECIES_BUNNELBY 0x2FF
#define SPECIES_DIGGERSBY 0x300
#define SPECIES_FLETCHLING 0x301
#define SPECIES_FLETCHINDER 0x302
#define SPECIES_TALONFLAME 0x303
#define SPECIES_SCATTERBUG 0x304
#define SPECIES_SPEWPA 0x305
#define SPECIES_VIVILLON 0x306
#define SPECIES_LITLEO 0x307
#define SPECIES_PYROAR 0x308
#define SPECIES_FLABEBE 0x309
#define SPECIES_FLOETTE 0x30A
#define SPECIES_FLORGES 0x30B
#define SPECIES_SKIDDO 0x30C
#define SPECIES_GOGOAT 0x30D
#define SPECIES_PANCHAM 0x30E
#define SPECIES_PANGORO 0x30F
#define SPECIES_FURFROU 0x310
#define SPECIES_ESPURR 0x311
#define SPECIES_MEOWSTIC 0x312
#define SPECIES_HONEDGE 0x313
#define SPECIES_DOUBLADE 0x314
#define SPECIES_AEGISLASH 0x315
#define SPECIES_SPRITZEE 0x316
#define SPECIES_AROMATISSE 0x317
#define SPECIES_SWIRLIX 0x318
#define SPECIES_SLURPUFF 0x319
#define SPECIES_INKAY 0x31A
#define SPECIES_MALAMAR 0x31B
#define SPECIES_BINACLE 0x31C
#define SPECIES_BARBARACLE 0x31D
#define SPECIES_SKRELP 0x31E
#define SPECIES_DRAGALGE 0x31F
#define SPECIES_CLAUNCHER 0x320
#define SPECIES_CLAWITZER 0x321
#define SPECIES_HELIOPTILE 0x322
#define SPECIES_HELIOLISK 0x323
#define SPECIES_TYRUNT 0x324
#define SPECIES_TYRANTRUM 0x325
#define SPECIES_AMAURA 0x326
#define SPECIES_AURORUS 0x327
#define SPECIES_SYLVEON 0x328
#define SPECIES_HAWLUCHA 0x329
#define SPECIES_DEDENNE 0x32A
#define SPECIES_CARBINK 0x32B
#define SPECIES_GOOMY 0x32C
#define SPECIES_SLIGGOO 0x32D
#define SPECIES_GOODRA 0x32E
#define SPECIES_KLEFKI 0x32F
#define SPECIES_PHANTUMP 0x330
#define SPECIES_TREVENANT 0x331
#define SPECIES_PUMPKABOO 0x332
#define SPECIES_GOURGEIST 0x333
#define SPECIES_BERGMITE 0x334
#define SPECIES_AVALUGG 0x335
#define SPECIES_NOIBAT 0x336
#define SPECIES_NOIVERN 0x337
#define SPECIES_XERNEAS 0x338
#define SPECIES_YVELTAL 0x339
#define SPECIES_ZYGARDE 0x33A
#define SPECIES_DIANCIE 0x33B
#define SPECIES_HOOPA 0x33C
#define SPECIES_HOOPA_UNBOUND 0x33D
#define SPECIES_VOLCANION 0x33E
#define SPECIES_PYROAR_FEMALE 0x33F
#define SPECIES_MEOWSTIC_FEMALE 0x340
#define SPECIES_ZYGARDE_10 0x345
#define SPECIES_ZYGARDE_COMPLETE 0x346

"""

# Set your starting index for the array
start_index = 0


# Regex pattern to extract species name
pattern = re.compile(r"#define SPECIES_([A-Z0-9_]+)")

# Clean and capitalize species names (handle underscores properly)
def format_species_name(raw):
    return ''.join(part.capitalize() for part in raw.lower().split('_'))

lines = input_species_block.strip().splitlines()

for i, line in enumerate(lines):
    match = pattern.search(line)
    if match:
        species_name = format_species_name(match.group(1))
        index = start_index + i
        print(f"\t&gFollowerMonGfxTable3[{index}], // {species_name}")
