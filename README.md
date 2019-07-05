# Complete Fire Red Upgrade

## What is this:
A complete upgrade for Fire Red, including an upgraded Battle Engine.

**Before doing anything it is HIGHLY recommended the documentation be read thoroughly.**

## Features:
* Expanded Pokémon
* Expanded PC Boxes (up to 24!)
* Battle Engine Upgraded to Gen 7
  * All Moves, Abilities, Items, and Effects through Gen 7
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
  * Shiny Charm
  * Level Scaling
  * Updated Exp. Share
  * Inverse Battles, Sky Battles
  * Overworld trainer facing
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
* Secondary Options Menu
* Expanded Pokemon in the Hall of Fame
* Triple Layer Blocks
* Expanded Text Buffers
* Footstep noise in grass & on sand
* Omnidirectional Jumping
* Item Image on obtain

**Note:** Personalized options are available in 'src/config.h'. The options can be customized by commenting and uncommenting lines.

## Installation Instructions:
### Windows: 
1. Download and install devkitpro. You can follows steps 4-6 from [this tutorial](https://www.pokecommunity.com/showpost.php?p=8825585&postcount=96). 
(Note: you can only install devkitARM)

2. Download the latest version of python (3.7.3).
After downloading and before proceeding to install make sure that the **add to path** checkbox is ticked, otherwise you'll have to add the python path in the environment variables manually. If you have installed multiple python versions, please make sure that python 3.6+ can be explicitly called with command `python3`.

3. Download the master folder from this github page.
(Click 'Clone or Download', then 'Download Zip')

4. Extract the zipped file in the root called "deps.rar" somewhere convenient and add that folder to your path (same steps as shown in the tutorial found in step 1).

5. Get your ROM, rename it to **BPRE0.gba** and place it the main (master) folder.

6. To decide the offsets where you want to insert the code:
In 'scripts/make.py' change `OFFSET_TO_PUT=YYY` to the location you want to insert the data. Don't worry about changing 'insert.py' because 'make.py' automatically updates 'insert.py' and 'linker.ld'.
 
7. Run cmd.exe in the main folder. You can do this by typing `cmd` and hitting enter in the 
url address or selecting 'run command prompt from here' from right clicking on empty space while holding the shift key.

8. In command prompt window, type `python scripts//make.py` (or `python3 scripts//make.py` if you've installed multiple python versions). 
  
A new gba file will appear named as **test.gba** and an **offsets.ini** file.
That is your resultant file.

### UNIX-like OS (linux, macOS, ...): 
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
In ‘scripts/make.py’ change `OFFSET_TO_PUT=YYY` to the location you want to insert the data. 

7. Run `python scripts/make.py` (or `python3 scripts/make.py` if you’ve installed multiple python versions). 

A new gba file will appear named as **test.gba** and an **offsets.ini** file.
That is your resultant file.

## Notes

Anytime you make changes, the compiler will only compile the files you have changed.
Any changes made to header files will require you to clean everything in 'build/' and then rerun the build scripts. For more command line options, see "Engine Scripts" in the documentation. 

