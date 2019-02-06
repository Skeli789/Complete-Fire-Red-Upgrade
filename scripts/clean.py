#!/usr/bin/env python3

import os
import shutil

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

print("Directory cleaned!")