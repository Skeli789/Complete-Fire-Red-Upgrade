# Complete-Fire-Red-Upgrade
A complete upgrade for Fire Red, including an upgraded Battle Engine.

Personalized options are available in src/config.h. The options can be customized
by commenting and uncommenting lines.

Installation Instructions:

1. Download devkitpro. Follow the instructions.
(Note: you can only install devkitARM)
For Windows users, follows steps 4-6 from this tutorial:
https://www.pokecommunity.com/showpost.php?p=8825585&postcount=96

2. Download the latest version of python (3.7.2).
After downloading and before proceeding to install make sure that the 'add to path' 
checkbox is ticked, otherwise you'll have to add the python path in the environment 
variables manually.

**NOTE**: If a python version lower than 3.6 is installed, you'll need to uninstall it and manually
remove it from your path before installing the newer version of Python.

3. Download the master folder from this github page.
(click 'Clone or Download', then 'Download Zip')

4. Extract the zipped file in the root called "deps.rar" somewhere convenient and add that
folder to your path (same steps as shown in the tutorial found in step 1).

5. Get your ROM, rename it Pokemon Unbound.gba (this will be changed eventually) and 
place it the main (master) folder.

6. To decide the offsets where you want to insert the code:

a) In the 'insert.py' file in the folder 'scripts' change OFFSET_TO_PUT=YYY to the location 
   you want to insert the data (let it be X). Do this as well in the file 'make.py'.
  
b) In the 'linker.ld' file change ORIGIN = (0x08000000 +YYY) ORIGIN = (0x08000000 + X)

7. Run cmd.exe in the main folder. You can do this by typing 'cmd' and hitting enter in the 
url address or selecting 'run command prompt from here' from right clciking on empty space 
while holding the shift key.

8. In command prompt window, do one of the following:

a) Type 'python scripts//build.py', press Enter and then 'python scripts//insert.py' (here we 
   output a text file containing a list of all the offsets), press Enter.

b) Type 'python scripts//make.py'
  
A new gba file will appear named as test.gba and an offsets.ini file.
That is your resultant file.

Anytime you make changes, the compiler will only compile the files you have changed.
Any changes made to header files will require you to type 'python scripts//clean.py'
in cmd and then rerun the build scripts.
