#!/usr/bin/env python3

from glob import glob
from pathlib import Path
import os, itertools, hashlib, subprocess, sys
from datetime import datetime
from string import StringFileConverter

if sys.platform.startswith('win'):
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
	GR = ("deps/grit.exe")
	ARP = ('deps/armips.exe')
	OBJCOPY = (PATH + PREFIX + 'objcopy')
else:
	PREFIX = 'arm-none-eabi-'
	AS = (PREFIX + 'as')
	CC = (PREFIX + 'gcc')
	LD = (PREFIX + 'ld')
	GR = ("grit")
	ARP = ('armips')
	OBJCOPY = (PREFIX + 'objcopy')

SRC = './src'
GRAPHICS = './graphics'
ASSEMBLY = './assembly'
STRINGS = './strings'
BUILD = './build'
ASFLAGS = ['-mthumb', '-I', ASSEMBLY]
LDFLAGS = ['BPRE.ld', '-T', 'linker.ld']
CFLAGS = ['-mthumb', '-mno-thumb-interwork', '-mcpu=arm7tdmi', '-mtune=arm7tdmi',
'-mno-long-calls', '-march=armv4t', '-Wall', '-Wextra','-Os', '-fira-loop-pressure', '-fipa-pta']

PrintedCompilingImages = False #Used to tell the script whether or not the string "Compiling Images" has been printed

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
	
	if not os.path.isfile(filename):
		return [newfilename, False]
	
	fileExists = os.path.isfile(newfilename)
	
	if fileExists and os.path.getmtime(newfilename) > os.path.getmtime(filename): #If the object file was created after the file was last modified
		return [newfilename, False]
	
	return [newfilename, True]

def make_output_img_file(filename):
	'''Return "IMG" + hash of filename to use as object filename'''
	m = hashlib.md5()
	m.update(filename.encode())
	newfilename = os.path.join(BUILD, 'IMG_' + m.hexdigest() + '.o')
	
	if not os.path.isfile(filename):
		return [newfilename, False]
	
	fileExists = os.path.isfile(newfilename)
	
	if fileExists and os.path.getmtime(newfilename) > os.path.getmtime(filename): #If the object file was created after the file was last modified
		return [newfilename, False]
	
	return [newfilename, True]

def process_assembly(in_file):
	'''Assemble'''
	out_file_list = make_output_file(in_file)
	out_file = out_file_list[0]
	if out_file_list[1] is False:
		return out_file #No point in recompiling file
	
	try:
		print ('Assembling %s' % in_file)
		cmd = [AS] + ASFLAGS + ['-c', in_file, '-o', out_file]
		run_command(cmd)
		
	except FileNotFoundError:
		print('Error! The assembler could not be located.\nAre you sure you set up your path to devkitPro/devkitARM/bin correctly?')
		sys.exit(1)
		
	return out_file
	
def process_c(in_file):
	'''Compile C'''
	out_file_list = make_output_file(in_file)
	out_file = out_file_list[0]
	if out_file_list[1] is False:
		return out_file #No point in recompiling file
	
	try:
		print ('Compiling %s' % in_file)
		cmd = [CC] + CFLAGS + ['-c', in_file, '-o', out_file]
		run_command(cmd)

	except FileNotFoundError:
		print('Error! The C compiler could not be located.\nAre you sure you set up your path to devkitPro/devkitARM/bin correctly?')
		sys.exit(1)
	
	return out_file

def process_string(filename):
	'''Build Strings'''
	out_file = filename.split(".string")[0] + '.s'
	object_file = make_output_file(out_file)[0]

	fileExists = os.path.isfile(object_file)

	if fileExists and os.path.getmtime(object_file) > os.path.getmtime(filename): #If the .o file was created after the image was last modified
		return make_output_file(out_file)[0]

	print ('Building Strings %s' % filename)
	StringFileConverter(filename)

	out_file_list = make_output_file(out_file)
	new_out_file = out_file_list[0]
	if out_file_list[1] == False:
		os.remove(out_file)
		return new_out_file #No point in recompiling file

	cmd = [AS] + ASFLAGS + ['-c', out_file, '-o', new_out_file]
	run_command(cmd)
	os.remove(out_file)
	return new_out_file

def process_image(in_file):
	'''Compile Image'''
	if '.bmp' in in_file:
		out_file = in_file.split('.bmp')[0] + '.s'
	else:
		out_file = in_file.split('.png')[0] + '.s'
	if sys.platform.startswith('win'):
		namelist = in_file.split("\\") #Get path of grit flags
		namelist.pop(len(namelist) - 1)
		flags = "".join(str(i) + "\\" for i in namelist)
	else:
		namelist = in_file.split("/") #Get path of grit flags
		namelist.pop(len(namelist) - 1)
		flags = "".join(str(i) + "//" for i in namelist)
	flags += "gritflags.txt"
	
	try:
		with open(flags, 'r') as file:
			for line in file:
				cmd = [GR, in_file] + line.split() + ['-o', out_file]
				break #only needs the first line
	except FileNotFoundError:
		print("Error: No gritflags.txt found in directory with " + in_file)
		sys.exit(1)
	
	out_file_list = make_output_img_file(out_file)
	new_out_file = out_file_list[0]
	try:
		if os.path.getmtime(new_out_file) > os.path.getmtime(in_file): #If the .o file was created after the image was last modified
			return new_out_file
		else:
			run_command(cmd)
	
	except FileNotFoundError:
		run_command(cmd) #No .o file has been created

	global PrintedCompilingImages
	if (PrintedCompilingImages is False):
		print ('Compiling Images')
		PrintedCompilingImages = True
	
	out_file_list = make_output_img_file(out_file)
	new_out_file = out_file_list[0]
	if out_file_list[1] == False:
		os.remove(out_file)
		return new_out_file #No point in recompiling file

	cmd = [AS] + ASFLAGS + ['-c', out_file, '-o', new_out_file]
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
	if globstr == '**/*.png' or globstr == '**/*.bmp': #Search the graphics location
		return run_glob_graphics(globstr, fn)
	elif globstr == '**/*.s':
		return run_glob_assembly(globstr, fn)
	elif globstr == '**/*.string':
		return run_glob_strings(globstr, fn)
	
	if sys.version_info > (3, 4):
		try:
			files = glob(os.path.join(SRC, globstr), recursive = True)
			return map(fn, files)
		except TypeError:
			print("Error compiling. Please make sure Python has been updated to the latest version.")
			sys.exit(1)
	else:
		files = Path(SRC).glob(globstr)
		return map(fn, map(str, files))

def run_glob_assembly(globstr, fn):
	'''Glob recursively and run the processor function on each file in result'''
	if sys.version_info > (3, 4):
		files = glob(os.path.join(ASSEMBLY, globstr), recursive = True)
		return map(fn, files)
	else:
		files = Path(ASSEMBLY).glob(globstr)
		return map(fn, map(str, files))

def run_glob_strings(globstr, fn):
	'''Glob recursively and run the processor function on each file in result'''
	if sys.version_info > (3, 4):
		files = glob(os.path.join(STRINGS, globstr), recursive = True)
		return map(fn, files)
	else:
		files = Path(STRINGS).glob(globstr)
		return map(fn, map(str, files))

def run_glob_graphics(globstr, fn):
	'''Glob recursively and run the processor function on each file in result'''
	if sys.version_info > (3, 4):
		files = glob(os.path.join(GRAPHICS, globstr), recursive = True)
		return map(fn, files)
	else:
		files = Path(GRAPHICS).glob(globstr)
		return map(fn, map(str, files))
		
def main():
	starttime = datetime.now()
	globs = {
			'**/*.s': process_assembly,
			'**/*.c': process_c,
			'**/*.string': process_string,
			'**/*.png': process_image,
			'**/*.bmp': process_image
	}
		
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
	
	#Build special_inserts.asm
	if not os.path.isfile('build/special_inserts.bin') or os.path.getmtime('build/special_inserts.bin') < os.path.getmtime('special_inserts.asm'): #If the binary file was created after the file was last modified):
		cmd = cmd = [AS] + ASFLAGS + ['-c', 'special_inserts.asm', '-o', 'build/special_inserts.o']
		run_command(cmd)
		
		cmd = [OBJCOPY, '-O', 'binary', 'build/special_inserts.o', 'build/special_inserts.bin']
		run_command(cmd)
		
		print ('Assembling special_inserts.asm')
	
	print('Built in ' + str(datetime.now() - starttime) + '.')
	
if __name__ == '__main__':
	main()
