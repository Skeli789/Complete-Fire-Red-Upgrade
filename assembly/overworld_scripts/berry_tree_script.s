.thumb
.align 2

.include "../xse_commands.s"
.include "../xse_defines.s"
.include "../asm_defines.s"

//Every Berry Tree must start with different script like this
//to goto "EventScript_Plant_Berry_Tree_Start" at the end
.global EventScript_Plant_Berry_Tree
EventScript_Plant_Berry_Tree:
    setvar 0x8000 0x5
    setvar 0x8001 0x1
    setvar LASTRESULT 0x1
    goto EventScript_Plant_Berry_Tree_Start

EventScript_Plant_Berry_Tree_Start:
    lock
    copyvar 0x8002 LASTRESULT
    copyvar 0x8011 0x8002
    callasm GetTreeData
    compare 0x8005 0
    if equal _goto EventScript_Plant_Berry_Tree_NonePlanted
    compare 0x8005 1
    if equal _goto EventScript_Plant_Berry_Tree_Fresh
    compare 0x8005 2
    if equal _goto EventScript_Plant_Berry_Tree_Sprouting
    compare 0x8005 3
    if equal _goto EventScript_Plant_Berry_Tree_Growing
    compare 0x8005 4
    if equal _goto EventScript_Plant_Berry_Tree_Blooming
    callasm CalculateYield
    buffernumber 0x0 LASTRESULT
    bufferitem 0x1 0x8008
    additem 0x8008 0x800D
    msgbox gText_Berry_Received MSG_NORMAL
    setvar 0x8005 0x0
    setvar 0x8006 0x0
    setvar 0x8007 0x0
    setvar 0x8008 0x0
    setvar 0x8009 0x0
    setvar 0x800A 0x0
    setvar 0x800B 0x0
    callasm SetTreeData
    msgbox gText_Berry_Returned MSG_NORMAL
    release
    end

EventScript_Plant_Berry_Tree_NonePlanted:
    msgbox gText_None_Planted MSG_YESNO
    compare LASTRESULT 0x0
    if equal _goto EventScript_Plant_Berry_Tree_End
    setvar 0x8000 0x4
    special 0xB1
    compare 0x800E 0x0
    if equal _goto EventScript_Plant_Berry_Tree_End
    compare 0x800E 0x84
    if lessorequal _goto EventScript_Plant_Berry_Tree_LaughAtPlayer
    compare 0x800E 0xB0
    if greaterorequal _goto EventScript_Plant_Berry_Tree_LaughAtPlayer
    copyvar 0x8008 0x800E
    bufferitem 0x0 0x8008
    msgbox gText_Added_Berry MSG_NORMAL
    setvar 0x8005 0x1
    setvar 0x8006 0x0
    setvar 0x8007 0x0
    copyvar 0x8002 0x8011
    callasm SetPlantingTime
    callasm SetTreeData
    removeitem 0x8008 0x1
    release
    end


EventScript_Plant_Berry_Tree_Fresh:
    bufferitem 0x0 0x8008
    msgbox gText_New_Plant MSG_NORMAL
    compare 0x8007 0x0
    if lessorequal _goto EventScript_Plant_Berry_Tree_WaterPlant
    msgbox gText_Watered_Already MSG_NORMAL
    release
    end


EventScript_Plant_Berry_Tree_Sprouting:
    bufferitem 0x0 0x8008
    msgbox gText_Berry_Sprouted MSG_NORMAL
    compare 0x8007 0x0
    if lessorequal _goto EventScript_Plant_Berry_Tree_WaterPlant
    msgbox gText_Watered_Already MSG_NORMAL
    release
    end


EventScript_Plant_Berry_Tree_Growing:
    bufferitem 0x0 0x8008
    msgbox gText_Berry_Is_Tall MSG_NORMAL
    compare 0x8007 0x0
    if lessorequal _goto EventScript_Plant_Berry_Tree_WaterPlant
    msgbox gText_Watered_Already MSG_NORMAL
    release
    end


EventScript_Plant_Berry_Tree_Blooming:
    bufferitem 0x0 0x8008
    msgbox gText_Berry_Bloom MSG_NORMAL
    compare 0x8007 0x0
    if lessorequal _goto EventScript_Plant_Berry_Tree_WaterPlant
    msgbox gText_Watered_Already MSG_NORMAL
    release
    end

EventScript_Plant_Berry_Tree_End:
    release
    end

EventScript_Plant_Berry_Tree_LaughAtPlayer:
    msgbox gText_Berry_WrongItem MSG_NORMAL
    release
    end

EventScript_Plant_Berry_Tree_WaterPlant:
    checkitem ITEM_POKE_BALL 0x1 //Watering item ID
    compare LASTRESULT TRUE
    if lessthan _goto EventScript_Plant_Berry_Tree_End
    removeitem ITEM_POKE_BALL 0x1 //Watering item ID
    msgbox gText_Berry_Want_Water MSG_YESNO
    compare LASTRESULT 0x0
    if equal _goto EventScript_Plant_Berry_Tree_End
    msgbox gText_Berry_Finished_Watering MSG_NORMAL
    addvar 0x8006 0x1
    addvar 0x8007 0x1
    copyvar LASTRESULT 0x8005
    callasm SetTreeData
    release
    end

//Insert this script in every level you have berry trees
//level script type must be "MAP_SCRIPT_ON_RESUME"
.global LevelScript_BerryTrees_OnMap
LevelScript_BerryTrees_OnMap:
    setvar 0x4011 0x1 // ID of first berry tree on map
    setvar 0x8003 0x2 // ID of last berry tree on map
    setvar 0x8004 0x10 // Start of the berry tree OWs event numbers in A-Map
    goto LevelScript_BerryTrees_OnMap_Main

LevelScript_BerryTrees_OnMap_Main:
    copyvar 0x8002 0x4011
    callasm GetTreeData
    callasm SetTreeData
    compare 0x8005 0x0
    if equal _goto LevelScript_BerryTrees_OnMap_Nothing
    compare 0x8005 0x1
    if equal _goto LevelScript_BerryTrees_OnMap_Sprout
    compare 0x8005 0x2
    if equal _goto LevelScript_BerryTrees_OnMap_Growing
    compare 0x8005 0x3
    if equal _goto LevelScript_BerryTrees_OnMap_Flowering
    spritebehave 0x8004 0x4D // Makes the OW hold the VS seeker animation
    goto LevelScript_BerryTrees_OnMap_Next

LevelScript_BerryTrees_OnMap_Next:
    comparevars 0x8003 0x4011 // Checks if the current berry tree ID is the last one on the map
    if equal _goto LevelScript_BerryTrees_OnMap_Main_Ending
    addvar 0x4011 0x1
    addvar 0x8004 0x1
    goto LevelScript_BerryTrees_OnMap_Main // Loops back to Main

LevelScript_BerryTrees_OnMap_Nothing:
    movesprite2 0x8004 0xFF 0xFF // Moves the OW far offscreen if there is no growth
    goto LevelScript_BerryTrees_OnMap_Next

LevelScript_BerryTrees_OnMap_Sprout:
    spritebehave 0x8004 0x0 // Makes the OW face down
    goto LevelScript_BerryTrees_OnMap_Next

LevelScript_BerryTrees_OnMap_Growing:
    spritebehave 0x8004 0x7 // Makes the OW face up
    goto LevelScript_BerryTrees_OnMap_Next

LevelScript_BerryTrees_OnMap_Flowering:
    spritebehave 0x8004 0x9 // Makes the OW face left
    goto LevelScript_BerryTrees_OnMap_Next

LevelScript_BerryTrees_OnMap_Main_Ending:
    end
