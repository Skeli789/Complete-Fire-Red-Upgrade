# Complete Fire Red Upgrade

## What is this:
A complete upgrade for Fire Red, including an upgraded Battle Engine.

## Features:
* Battle Engine Upgraded to Gen 7
  * All Moves, Abilities, Items, Weather effects, Terrain effects, mechanics, effects through Gen 7
  * Vastly improved AI system with decision-making skills for all new battle effects
  * Ability pop-ups
  * Hidden Abilities
  * Dissapearing HP-bars during Attack Animations
  * Trainer Sliding / In-battle messages
  * Expanded Poké Balls
  * Battle Terrain
  * Totem Pokémon
  * Trainers with EVs
  * Class-based Music
  * Class-based Poké Balls
  * Dynamic Trainer Backsprites
  * Pre-battle mugshots
  * Wild double battles
  * Multi Battles (with a partner)
  * New Evolution Methods
  * Expanded Learnsets
  * Badge-based obedience
  * Shiny Charm
  * Level Scaling
  * Updated Exp. Share
  * Inverse Battles, Sky Battles
  * Z Moves
  * Overworld trainer facing
* Mega Evolution/Primal Reversion/Ultra Burst
* Battle Frontier
* Expanded Move Reminder (up to 50 moves)
* Improved Tm/Hm Expansion, Reusable TMs
* Updated Pickup mechanics
* Swarms
* Day/Night/Seasons
* Roaming Pokémon
* JPANs engine features ported and redesigned/improved in many ways
  * Character Customization
  * Lots of new scripting specials
  * Whiteout hack
  * And more!
* Save-block expansion
* DexNav and secondary PokéTools Start Menu
* Dynamic overworld palettes
* Updated Daycare system
* Expanded Text Names (Abilities, etc)
* Select Pokemon from PC directly
* Scrolling summary screen
* Base stats on pokedex summary screen
* Updated repel system
* Configurable Start Menu
* Follow Me
* Secondary Options Menu

**NOTE** Personalized options are available in src/config.h. The options can be customized
by commenting and uncommenting lines. **Before doing anything it is HIGHLY
recommended the documention be read.** It may be long, but it is imperative that at 
least the configuration section be read before building the engine.

## Installation Instructions:
```
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

5. Get your ROM, rename it BPRE0.gba and 
place it the main (master) folder.

6. To decide the offsets where you want to insert the code:

a) In the 'make.py' file in the folder 'scripts' change OFFSET_TO_PUT=YYY to the location 
   you want to insert the data (let it be X). Don't worry about changing 'insert.py' also.
   'make.py' automatically updates the 'insert.py' file and linker file.
 
7. Run cmd.exe in the main folder. You can do this by typing 'cmd' and hitting enter in the 
url address or selecting 'run command prompt from here' from right clciking on empty space 
while holding the shift key.

8. In command prompt window, type 'python scripts//make.py'
  
A new gba file will appear named as test.gba and an offsets.ini file.
That is your resultant file.
```

## Notes

Anytime you make changes, the compiler will only compile the files you have changed.
Any changes made to header files will require you to type 'python scripts//clean.py'
in cmd and then rerun the build scripts. For more command line options, see "Engine
Scripts" in the documentation.
