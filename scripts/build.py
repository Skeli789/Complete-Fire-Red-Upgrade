#!/usr/bin/env python3

from glob import glob
from pathlib import Path
import os
import itertools
import hashlib
import subprocess
import sys

PathVar = os.environ.get('Path')
Paths = PathVar.split(';')
PATH = ""
for candidatePath in Paths:
    if "devkitARM" in candidatePath:
        PATH = candidatePath
        break
if PATH == "":
	print('DevKit does not exist in your Path variable.\nChecking default location.')
	PATH = 'C://devkitPro//devkitARM//bin'
	if os.path.isdir(PATH) == False:
		print("...\nDevkit not found.")
		sys.exit(1)
	else:
		print("Devkit found.")
		
PREFIX = '/arm-none-eabi-'
AS = (PATH + PREFIX + 'as')
CC = (PATH + PREFIX + 'gcc')
LD = (PATH + PREFIX + 'ld')
GR = (PATH + '/grit')
ARP = ('armips')
OBJCOPY = (PATH + PREFIX + 'objcopy')
SRC = './src'
BUILD = './build'
IMAGES = '\Images'
ASFLAGS = ['-mthumb', '-I', SRC]
LDFLAGS = ['BPRE.ld', '-T', 'linker.ld']
CFLAGS = ['-mthumb', '-mno-thumb-interwork', '-mcpu=arm7tdmi', '-mtune=arm7tdmi',
'-mno-long-calls', '-march=armv4t', '-Wall', '-Wextra','-Os', '-fira-loop-pressure', '-fipa-pta']
GRITFLAGS_UNCOMPRESSED = ['-gz!', '-gB', '4', '-ftc', '-fh', '-pe16', '-pzlz77!' '-pT4', '-gu8'] #Well...partially uncompressed
GRITFLAGS_COMPRESSED = ['-gzl', '-gB', '4', '-ftc', '-fh', '-pe16', '-pzlz77' '-pT4', '-gu8']

def run_command(cmd):
	try:
		subprocess.check_output(cmd)
	except subprocess.CalledProcessError as e:
		print(e.output.decode(), file = sys.stderr)
		sys.exit(1)

def make_output_file(filename):
	'''Return hash of filename to use as object filename'''
	m = hashlib.md5()
	m.update(filename.encode())
	newfilename = os.path.join(BUILD, m.hexdigest() + '.o')
	
	try:
		if os.path.getmtime(newfilename) > os.path.getmtime(filename): #If the object file was created after the file was last modified
			return [newfilename, False]
		else:
			return [newfilename, True]
	
	except FileNotFoundError: #If the object file doesn't exist, then obviously it needs to be made
		return [newfilename, True]

def process_assembly(in_file):
	'''Assemble'''
	out_file_list = make_output_file(in_file)
	out_file = out_file_list[0]
	if out_file_list[1] == False:
		return out_file #No point in recompiling file
	
	print ('Assembling %s' % in_file)
	cmd = [AS] + ASFLAGS + ['-c', in_file, '-o', out_file]
	run_command(cmd)
	return out_file
	
def process_c(in_file):
	'''Compile C'''
	out_file_list = make_output_file(in_file)
	out_file = out_file_list[0]
	if out_file_list[1] == False:
		return out_file #No point in recompiling file
	
	print ('Compiling %s' % in_file)
	cmd = [CC] + CFLAGS + ['-c', in_file, '-o', out_file]
	run_command(cmd)
	
	return out_file

def process_image(in_file):
	'''Compile Image'''
	if '.bmp' in in_file:
		out_file = in_file.split('.bmp')[0] + '.c'
		cmd = [GR, in_file] + GRITFLAGS_UNCOMPRESSED + ['-o', out_file]
	else:
		out_file = in_file.split('.png')[0] + '.c'
		cmd = [GR, in_file] + GRITFLAGS_COMPRESSED + ['-o', out_file]
	
	try:
		if os.path.getmtime(make_output_file(out_file)[0]) > os.path.getmtime(in_file): #If the .o file was created after the image was last modified
			return make_output_file(out_file)[0]
		else:
			run_command(cmd)
	
	except FileNotFoundError:
		run_command(cmd) #No .o file has been created

	out_file_list = make_output_file(out_file)
	new_out_file = out_file_list[0]
	if out_file_list[1] == False:
		os.remove(out_file)
		return new_out_file	#No point in recompiling file

	cmd = [CC] + CFLAGS + ['-c', out_file, '-o', new_out_file]
	run_command(cmd)
	os.remove(out_file)
	return new_out_file

def link(objects):
	'''Link objects into one binary'''
	linked = 'build/linked.o'
	cmd = [LD] + LDFLAGS + ['-o', linked] + list(objects)
	run_command(cmd)
	return linked
	
def objcopy(binary):
	cmd = [OBJCOPY, '-O', 'binary', binary, 'build/output.bin']
	run_command(cmd)
	
def run_glob(globstr, fn):
	'''Glob recursively and run the processor function on each file in result'''
	if sys.version_info > (3, 4):
		files = glob(os.path.join(SRC, globstr), recursive = True)
		return map(fn, files)
	else:
		files = Path(SRC).glob(globstr)
		return map(fn, map(str, files))

def main():
	globs = {
			'**/*.s': process_assembly,
			'**/*.c': process_c,
			'**/*.png': process_image,
			'**/*.bmp': process_image
	}
	
	print ('Compiling Images')
	
	# Create output directory
	try:
		os.makedirs(BUILD)
	except FileExistsError:
		pass
	
	# Gather source files and process them
	objects = itertools.starmap(run_glob, globs.items())
	
	# Link and extract raw binary
	linked = link(itertools.chain.from_iterable(objects))
	objcopy(linked)
	
if __name__ == '__main__':
	main()
