#!/usr/bin/env python3

import shutil, sys, os

############
#Options go here.
############

ROM_NAME = "BPRE0.gba"  #the name of your rom
OFFSET_TO_PUT = 0x900000
SEARCH_FREE_SPACE = False #Set to True if you want the script to search for free space;Set to False if you don't want to search for free space as you for example update the engine

#############
#Options end here.
#############

###############
#Functions start here.
###############

def align_x100(offset):
	mod_x100 = offset % 0x100
	if mod_x100 != 0x0: #not aligned properly
		offset += (0x100 - mod_x100)
	return offset

def find_offset_to_put(rom, needed_bytes, start_loc):
	offset = start_loc
	rom.seek(0, 2)
	max_pos = rom.tell()
	found_bytes = 0
	while found_bytes < needed_bytes:
		if offset + found_bytes >= max_pos:
			print("End of file reached. Not enough free space.")
			return 0
		found_bytes += 1
		rom.seek(offset + found_bytes)
		if rom.read(1) != b'\xFF':
			offset = align_x100(offset + found_bytes)
			found_bytes = 0
	return offset
	
def file_change_line(file_path, line_to_change, replacement):
	file = open(file_path, 'r')
	copy = file.read()
	file.seek(0x0)
	line_no = 1
	for line in file:
		if (line_no == line_to_change):
			copy = copy.replace(line, replacement)
			break
		line_no += 1
	file.close()
	
	file = open(file_path, 'w')
	file.write(copy)
	file.close()

def edit_linker(offset):
	file_change_line("linker.ld", 4, "\t\trom     : ORIGIN = (0x08000000 + " + hex(offset) + "), LENGTH = 32M\n")
	
def edit_insert(offset):
	file_change_line("./scripts/insert.py", 6, "OFFSET_TO_PUT = " + hex(offset) + '\n')
	file_change_line("./scripts/insert.py", 7, 'SOURCE_ROM = "' + ROM_NAME + '"\n')
		
def clear_from_to(rom, from_, to_):
	rom.seek(from_)
	for i in range(0, to_ - from_):
		rom.write(b'\xFF')

##############
#Functions end here.
##############

try:
	with open(ROM_NAME, 'rb+') as rom:
		offset = OFFSET_TO_PUT
		if SEARCH_FREE_SPACE == True:
			offset = find_offset_to_put(rom, 0x50000, align_x100(offset))
		edit_linker(offset)
		edit_insert(offset)
		if shutil.which('python3') is not None:
			os.system("python3 scripts/build.py")
			os.system("python3 scripts/insert.py")
		else:
			os.system("python scripts/build.py")
			os.system("python scripts/insert.py")
		rom.close()
except:
	print('Error: Could not find source rom: "' + ROM_NAME + '".\nPlease make sure a rom with this name exists in the root.')
