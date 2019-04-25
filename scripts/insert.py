#!/usr/bin/env python3

import os
import subprocess
import sys
import shutil
import binascii
import textwrap
import sys

OFFSET_TO_PUT = 0xc12ff0

from datetime import datetime

PathVar = os.environ.get('Path')
Paths = PathVar.split(';')
PATH = ""
for candidatePath in Paths:
	if "devkitARM" in candidatePath:
		PATH = candidatePath
		break
if PATH == "":
	PATH = 'C://devkitPro//devkitARM//bin'
	if os.path.isdir(PATH) == False:
		print('Devkit not found.')
		sys.exit(1)
		
ROM_NAME = "test.gba"

PREFIX = 'arm-none-eabi-'
OBJCOPY = os.path.join(PATH, PREFIX + 'objcopy')
OBJDUMP = os.path.join(PATH, PREFIX + 'objdump')
NM = os.path.join(PATH, PREFIX + 'nm')
AS = os.path.join(PATH, PREFIX + 'as')
CC = os.path.join(PATH, PREFIX + 'gcc')
CXX = os.path.join(PATH, PREFIX + 'g++')

def ExtractPointer(listy):
	pointer = 0
	for a in range(len(listy)):
		pointer += (int(listy[a])) << (8 * a)
	return pointer

def get_text_section():
		try:
			# Dump sections
			out = subprocess.check_output([OBJDUMP, '-t', 'build/linked.o'])
			lines = out.decode().split('\n')
			
			# Find text section
			text = filter(lambda x: x.strip().endswith('.text'), lines)
			section = (list(text))[0]
			
			# Get the offset
			offset = int(section.split(' ')[0], 16)
			
			return offset
			
		except:
			print("Error: The insertion process could not be completed.\n" +
				  "The linker symbol file was not found.")
			sys.exit(1)

def symbols(subtract=0):
		out = subprocess.check_output([NM, 'build/linked.o'])
		lines = out.decode().split('\n')
		
		name = ''
		
		ret = {}
		for line in lines:
				parts = line.strip().split()
				
				if (len(parts) < 3):
						continue
						
				if (parts[1].lower() not in {'t','d'}):
						continue
						
				offset = int(parts[0], 16)
				ret[parts[2]] = offset - subtract
				
		return ret
					
def hook(rom, space, hook_at, register=0):
		# Align 2
		if hook_at & 1:
			hook_at -= 1
			
		rom.seek(hook_at)
		
		register &= 7
		
		if hook_at % 4:
			data = bytes([0x01, 0x48 | register, 0x00 | (register << 3), 0x47, 0x0, 0x0])
		else:
			data = bytes([0x00, 0x48 | register, 0x00 | (register << 3), 0x47])
			
		space += 0x08000001
		data += (space.to_bytes(4, 'little'))
		rom.write(bytes(data))

def funcwrap(rom, space, hook_at, nparams, isreturning):
		# Align 2
		if hook_at & 1:
				hook_at -= 1
			
		rom.seek(hook_at)
		nparams=nparams-1

		if nparams<4:
				data = bytes([0x10, 0xB5, 0x3, 0x4C, 0x0, 0xF0, 0x3, 0xF8, 0x10, 0xBC , (isreturning+1), 0xBC , (isreturning<<3), 0x47, 0x20, 0x47])
		else:
				k=nparams-3
				data = bytes([0x10, 0xB5, 0x82, 0xB0])
				for i in range(k+2):
						data += bytes([ i+2, 0x9C , i, 0x94])
				data += bytes([0x0, 0x9C , (nparams-1), 0x94, 0x1, 0x9C , nparams, 0x94, 0x2, 0xB0 , (k+8), 0x4C,
							   0x0, 0xF0 , ((k<<1)+13), 0xF8, 0x82, 0xB0 , nparams, 0x9C, 0x1, 0x94 , (nparams-1), 0x9C , 0x0, 0x94])
				for i in reversed(range(k+2)):
						data += bytes([ i, 0x9C , i+2, 0x94])
				data += bytes([0x2, 0xB0 , 0x10, 0xBC, (isreturning+1), 0xBC , (isreturning<<3), 0x47, 0x20, 0x47])
		
		space += 0x08000001
		data += (space.to_bytes(4, 'little'))
		rom.write(bytes(data))

def repoint(rom, space, repoint_at, slidefactor=0):
		rom.seek(repoint_at)
		
		space += (0x08000000+slidefactor)
		data = (space.to_bytes(4, 'little'))
		rom.write(bytes(data))

ignored_offsets = [0x3986C0, 0x3986EC, 0xDABDF0] #These offsets contain the word 0x8900000 - the attack data from
												 #Mr. DS's rombase. In order to maintain as much compatability as
												 #possible, the data at these offsets is never modified.
	
def real_repoint(rom, offset_tuples):
	pointer_list = []
	pointer_dict = {}
	for tuple in offset_tuples: #Format is (Double Pointer, New Pointer, Symbol)
		offset = tuple[0]
		rom.seek(offset)
		pointer = ExtractPointer(rom.read(4))
		pointer_list.append(pointer)
		pointer_dict[pointer] = (tuple[1] + 0x08000000, tuple[2])

	offset = 0
	offset_list = []
	
	while (offset < 0xFFFFFD):
		if offset in ignored_offsets:
			offset += 4
			continue
		
		rom.seek(offset)
		word = ExtractPointer(rom.read(4))
		rom.seek(offset)
		
		for pointer in pointer_list:
			if word == pointer:
				offset_list.append((offset, pointer_dict[pointer][1]))
				rom.write(bytes(pointer_dict[pointer][0].to_bytes(4, 'little')))
				break
		
		offset += 4
	
	return offset_list

def bytereplace(rom, offset, data):

		ar=offset
		words=data.split()
		for i in range(0,len(words)):
				rom.seek(ar)
				intbyte=int(words[i],16)
				rom.write(bytes(intbyte.to_bytes(1, 'big')))
				ar += 1

starttime = datetime.now()
shutil.copyfile("Pokemon Unbound.gba", ROM_NAME)
with open(ROM_NAME, 'rb+') as rom:
		print("Inserting code.")
		table = symbols(get_text_section())
		rom.seek(OFFSET_TO_PUT)
		with open('build/output.bin', 'rb') as binary:
				rom.write(binary.read())
				binary.close()
		
		# Adjust symbol table
		for entry in table:
				table[entry] += OFFSET_TO_PUT

		# Insert byte changes
		with open('bytereplacement', 'r') as replacelist:
				for line in replacelist:
						if line.strip().startswith('#') or line.strip() == '' : continue
						offset = int(line[:8],16) - 0x08000000
						bytereplace(rom, offset, line[9:].strip())
		
		# Do Special Inserts
		with open('special_inserts.asm', 'r') as file:
			loadOffsets = False
			offsetList = []
			for line in file:
				if line.strip().startswith('.org '):
					offsetList.append(int(line.split('.org ')[1].split(',')[0], 16))
					
			offsetList.sort()
			
			try:
				with open('build/special_inserts.bin', 'rb') as binFile:
					for offset in offsetList:
						originalOffset = offset
						dataList = ""
						
						if offsetList.index(offset) == len(offsetList) - 1:
							while True:
								try:
									binFile.seek(offset)
									dataList += hex(binFile.read(1)[0]) + " "
								except IndexError:
									break
									
								offset += 1
						else:
							binFile.seek(offset)
							word = ExtractPointer(binFile.read(4))
							
							while (word != 0xFFFFFFFF):
								binFile.seek(offset)
								dataList += hex(binFile.read(1)[0]) + " "
								offset += 1
								
								if offset in offsetList: #Overlapping data
									break
									
								word = ExtractPointer(binFile.read(4))
							
						bytereplace(rom, originalOffset, dataList.strip())
							
			except FileNotFoundError:
				pass
						
		# Read hooks from a file
		with open('hooks', 'r') as hooklist:
				for line in hooklist:
						if line.strip().startswith('#') or line.strip() == '': continue
						
						symbol, address, register = line.split()
						offset = int(address, 16) - 0x08000000
						try:
								code = table[symbol]
						except KeyError:
								print('Symbol missing:', symbol)
								continue

						hook(rom, code, offset, int(register))
		
		# Read repoints from a file 
		with open('repoints', 'r') as repointlist:
				for line in repointlist:
						if line.strip().startswith('#') or line.strip() == '': continue
						if len(line.split()) is 2:
								symbol, address = line.split()
								offset = int(address, 16) - 0x08000000
								try:
										code = table[symbol]
								except KeyError:
										print('Symbol missing:', symbol)
										continue
								repoint(rom, code, offset)
						
						if len(line.split()) is 3:
								symbol, address, slide = line.split()
								offset = int(address, 16) - 0x08000000
								try:
										code = table[symbol]
								except KeyError:
										print('Symbol missing:', symbol)
										continue
								repoint(rom, code, offset, int(slide))

		symbols_repointed = set()

		try:
			with open('generatedrepoints', 'r') as repointlist:
				for line in repointlist:
					if line.strip().startswith('#') or line.strip() == '': continue
					
					symbol, address = line.split()
					offset = int(address)
					try:
						code = table[symbol]
					except KeyError:
						print('Symbol missing:', symbol)
						continue
					symbols_repointed.add(symbol)
					repoint(rom, code, offset)
				
		except FileNotFoundError:
			with open('generatedrepoints', 'w') as repointlist:
				repointlist.write('##This is a generated file at runtime. Do not modify it!\n')
		
		offsets_to_repoint_together = []
		
		with open('repointall', 'r') as repointlist:
			for line in repointlist:
				if line.strip().startswith('#') or line.strip() == '': continue
				
				symbol, address = line.split()
				offset = int(address, 16) - 0x08000000
				
				if symbol in symbols_repointed: continue
				
				try:
					code = table[symbol]
				except KeyError:
					print('Symbol missing:', symbol)
					continue
				offsets_to_repoint_together.append((offset, code, symbol))
		
			if offsets_to_repoint_together != []:
				offsets = real_repoint(rom, offsets_to_repoint_together) #Format is [(offset, symbol), ...]
				
				output = open('generatedrepoints', 'a')
				for tuple in offsets:
					output.write(tuple[1] + ' ' + str(tuple[0]) + '\n')
					#output.close() #Purposely left open so the user can't modify it

		# Read routine repoints from a file 
		with open('routinepointers', 'r') as pointerlist:
				for line in pointerlist:
						if line.strip().startswith('#') or line.strip() == '': continue
						
						symbol, address = line.split()
						offset = int(address, 16) - 0x08000000
						try:
								code = table[symbol]
						except KeyError:
								print('Symbol missing:', symbol)
								continue

						repoint(rom, code, offset, 1)

		# Read routine rewrite wrapper from a file
		with open('functionrewrites', 'r') as frwlist:
				for line in frwlist:
						if line.strip().startswith('#') or line.strip() == '': continue
						
						symbol, address, nparam, isreturning = line.split()
						offset = int(address, 16) - 0x08000000
						try:
								code = table[symbol]
						except KeyError:
								print('Symbol missing:', symbol)
								continue

						funcwrap(rom, code, offset, int(nparam), int(isreturning))
						
		width = max(map(len, table.keys())) + 1
		try:
			offset_file = open("offsets.ini", 'r+')
		except FileNotFoundError:
			offset_file = open("offsets.ini", 'w')
		offset_file.truncate()
		for key in sorted(table.keys()):
					fstr = ('{:' + str(width) + '} {:08X}')
					offset_file.write(fstr.format(key + ':', table[key] + 0x08000000) + '\n')
		offset_file.close()
		print('Inserted in ' + str(datetime.now() - starttime) + '.')

