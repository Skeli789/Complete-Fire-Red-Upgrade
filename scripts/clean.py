#!/usr/bin/env python3

import os
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

try:
	shutil.rmtree('build/')
except:
	pass
	
os.chdir("src\\Images")
for root, dirs, files in os.walk(".", topdown = False):
	for file in files:
		if file.endswith('.h'):
			os.remove(os.path.join(root, file))

print("Directory cleaned!")