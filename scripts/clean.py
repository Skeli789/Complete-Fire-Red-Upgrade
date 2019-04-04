#!/usr/bin/env python3

import os
import sys
import shutil
import glob, os.path

ROM_NAME = "test.gba"

try:
	os.remove(ROM_NAME)
except:
	pass

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
				
else:
	os.chdir("build")
	for root, dirs, files in os.walk(".", topdown = False):
		for file in files:
			if not file.startswith('IMG_'): #Don't remove image file
				os.remove(os.path.join(root, file))	

print("Directory cleaned!")