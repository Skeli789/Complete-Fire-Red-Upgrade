# Complete Fire Red Upgrade

## What is this:
A complete upgrade for Fire Red, including an upgraded Battle Engine.

**Before doing anything it is HIGHLY recommended the documentation be read thoroughly.**

## Features:
* Expanded PC Boxes (up to 24!)
* Battle Engine Upgraded to Gen 7
  * All Moves, Abilities, Items, Item Effects through Gen 7
  * A complete set of move animations
  * Vastly improved AI system with decision-making skills for all new battle effects
  * Z Moves
  * Mega Evolution / Primal Reversion / Ultra Burst
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
  * Shiny Charm + Oval Charm
  * Level Scaling
  * Updated Exp. Share
  * Inverse Battles, Sky Battles
  * Overworld trainer facing
  * Move types light up on move menu based on effectiveness
* Battle Frontier/Facilities
* Expanded Move Reminder (up to 50 moves)
* Improved Tm/Hm Expansion, Reusable TMs
* Updated Pickup mechanics
* Swarms
* Day/Night/Seasons
* Roaming Pokémon
* JPANs engine features ported and redesigned/improved in many ways
  * Character Customization
  * Lots of new scripting specials
  * Whiteout hack / text updates
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
* Expanded Pokemon in the Hall of Fame
* Triple Layer Blocks
* Expanded Text Buffers
* Footstep noise in grass & on sand
* Omnidirectional Jumping
* Item Image on obtain
* Move Items on Party Screen
* New Field Moves like Rock Climb & Defog
* Expanded Coins, Safari Balls/Steps
* Fairy Type
* Oval Charm
* And More!

**NOTE** Personalized options are available in *src/config.h*. The options can be customized by commenting and uncommenting lines.

**Note** Pokemon Expansion can be found [here](https://github.com/Skeli789/Dynamic-Pokemon-Expansion)

## Installation Instructions:
### Windows:
1. Download and install Devkitpro. You can follow steps 4-6 from [this tutorial](https://www.pokecommunity.com/showpost.php?p=8825585&postcount=96). You may need to restart your computer for the changes to take effect.

2. Download the latest version of Python 3.7 (3.7.6). **DO NOT DOWNLOAD PYTHON 3.8!**
After downloading and before proceeding to install, make sure that the **add to path** checkbox is ticked, otherwise you'll have to add the Python path in the environment variables manually. If you have installed multiple Python versions, please make sure that Python 3.6+ can be explicitly called with command `python3`.

3. Download (or clone) the master folder from this github page.
(Click 'Clone or Download', then 'Download Zip')

4. Get your ROM, rename it to **BPRE0.gba** and place it the main (master) folder.

5. To decide the offsets where you want to insert the code:
In 'scripts/make.py' change `OFFSET_TO_PUT=YYY` to the location you want to insert the data. Don't worry about changing *insert.py* because *make.py* automatically updates *insert.py* and *linker.ld*.

6. Run **cmd.exe** from the **main folder**. You can do this by typing `cmd` and hitting enter in the
url address or selecting *Open command window here* from right clicking on empty space while holding the shift key. If the command line contains */scripts* when showing the current directory, you are in the wrong place.

7. In the command prompt window, type `python scripts//make.py` (or `python3 scripts//make.py` if you've installed multiple python versions).

7.5. If you get an error with a mention of a file called **libwinpthread-1.dll** (or something similar), search Google for a 64-bit version of this file and add it to the root.

A new gba file will appear named as **test.gba** and an **offsets.ini** file.
Those are your resultant files.

### UNIX-like OS (Linux, MacOS, ...):
1. Install devkitPro with instructions [here](https://devkitpro.org/wiki/Getting_Started).

2. Export `${DEVKITARM}/bin/` to your `PATH` variable.

3. Make sure you've installed python 3.6+ and it can be called directly via either `python` or `python3`.

4. Clone the repo and go inside:   
```bash
git clone https://github.com/Skeli789/Complete-Fire-Red-Upgrade
cd Complete-Fire-Red-Upgrade
```

5. Get your ROM into the current directory and rename it to **BPRE0.gba**.

6. Configure the offset you want to insert the code:
In *scripts/make.py* change `OFFSET_TO_PUT=YYY` to the location you want to insert the data.

7. Run `python scripts/make.py` (or `python3 scripts/make.py` if you’ve installed multiple python versions).

A new gba file will appear named as **test.gba** and an **offsets.ini** file.
Those are your resultant files.

## Notes

Anytime you make changes, the compiler will only compile the files you have changed.

### Specific to Windows
Any changes made to header files will require you to type ``python scripts//clean.py build``
in cmd and then rerun the build scripts. For more command line options, see "Engine
Scripts" in the documentation.

### Specific to UNIX-like OS (linux, macOS, ...):
Any changes made to header files will require you to clean everything in *build/* and then rerun the build scripts.
