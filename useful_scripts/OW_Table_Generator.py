Input = open('Species.txt', 'r')
Output = open('OW_Entries_Table.txt', 'w')
PalTag = 0x11FF

def Adjust(PalTag):
    PalTag = PalTag.upper()
    PalTag = PalTag.replace('0X', '')
    return PalTag

for Mon in Input:
    Mon = Mon.replace('\n', '')
    Output.write(f'MON_OW_OBJECT_GRAPHICS({Adjust(hex(PalTag))}, {Mon}),\n')
    PalTag += 1

Input.close()
Output.close()