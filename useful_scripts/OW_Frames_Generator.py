Input = open('Species.txt', 'r')
Output = open('OW_Frames_Table.txt', 'w')
MonID = 0

for Mon in Input:
    Mon = Mon.replace('\n', '')
    Output.write(f'MON_OW_TEMPLATE_32x32_FRAMES({MonID:04}, {Mon});\n')
    MonID += 1

Input.close()
Output.close()