#!/usr/bin/env python3

import os
import sys
import shutil
import hashlib
import glob, os.path

ROM_NAME = "test.gba"
SRC = './src'
GRAPHICS = './graphics'
ASSEMBLY = './assembly'
STRINGS = './strings'
BUILD = './build'

def PutFileNameInRightFormat(filename):
	filename = filename.split('/')
	newFileName = ""
	
	if filename[0].upper() == "SRC":
		newFileName = SRC
	elif filename[0].upper() == "ASSEMBLY":
		newFileName = ASSEMBLY
	elif filename[0].upper() == "GRAPHICS":
		newFileName = GRAPHICS
	elif filename[0].upper() == "STRINGS":
		newFileName = STRINGS
		
	for i in range(1, len(filename)):
		newFileName += "\\" + filename[i]

	return newFileName

dir_path = os.path.dirname(os.path.realpath(__file__))

try:
	os.remove(ROM_NAME)
except:
	pass

if len(sys.argv) > 1:
	#Try removing specific file only.
	if len(sys.argv) > 2 and sys.argv[1].upper() == 'FILE':
		try:
			filename = PutFileNameInRightFormat(sys.argv[2])
			print(filename)
			m = hashlib.md5()
			m.update(filename.encode())
			newfilename = os.path.join(BUILD, m.hexdigest() + '.o')
	
			try:
				os.remove(newfilename)
			except FileNotFoundError:
				os.remove(BUILD + "\\IMG_" + newfilename.split('\\')[1])
			print('"Build for ' + sys.argv[2] + '" removed successfully!')
			sys.exit(1)
		except:
			print('Error: Could not remove build for file "' + sys.argv[2] + '".')
			sys.exit(1)
	
	#Don't remove generated repoints if the user only wants to remove the build.
	elif sys.argv[1].upper() != 'BUILD' and sys.argv[1].upper() != 'GRAPHICS':
		try:
			os.remove('generatedrepoints')
		except:
			pass
else:
	try:
		os.remove('generatedrepoints')
	except:
		pass

try:
	os.remove('offsets.ini')
except:
	pass

if (len(sys.argv) > 1) and sys.argv[1].upper() == 'ALL':
	try:
		shutil.rmtree('build/')
	except:
		pass	

	os.chdir("graphics")
	for root, dirs, files in os.walk(".", topdown = False):
		for file in files:
			if file.endswith('.h'):
				os.remove(os.path.join(root, file))

elif (len(sys.argv) > 1) and sys.argv[1].upper() == 'GRAPHICS':
	os.chdir("graphics")
	for root, dirs, files in os.walk(".", topdown = False):
		for file in files:
			if file.endswith('.h'):
				os.remove(os.path.join(root, file))
	
	os.chdir(dir_path.split('\\scripts')[0])
	os.chdir("build")
	for root, dirs, files in os.walk(".", topdown = False):
		for file in files:
			if file.startswith('IMG_'): #Only remove image files
				os.remove(os.path.join(root, file))	
			
else:
	os.chdir("build")
	for root, dirs, files in os.walk(".", topdown = False):
		for file in files:
			if not file.startswith('IMG_') and not file.startswith('SND_') and not file.startswith('MUS_'): #Don't remove image or audio files
				os.remove(os.path.join(root, file))	

print("Directory cleaned!")
