#include "defines.h"
#include "helper_functions.h"
/*
Simplified DexNav System
	credits to FBI: https://github.com/EternalCode/Dexnav
*/

/*
TO DO:
	oe_state -> gFieldEffectArguments


*/


/*

#define TOOL_COUNT 2
#define priv0 tasks[taskId].data[0]

struct TextColor MenuTextBlack = {0, 2, 3};
struct Textbox tbox = {0, 2, 2, 10, TOOL_COUNT << 1, 0xF, 0x130};

#define CPUFSCPY 0
#define CPUFSSET 1
#define CPUModeFS(size, mode) ((size >> 2) | (mode << 24))
//extern void CpuFastSet(void* src, void* dst, u32 mode);

extern u8 gText_DexNavText[];
extern u8 gText_PokeTools[];


u16 rand_range(u16 min, u16 max)
{
    if (min == max) return min;
    return (rand() % (max - min)) + min;
}




void vblank_cb_spq() {
	gpu_sprites_upload();
	copy_queue_process();
	gpu_pal_upload();
}


void c2_pokenav() {
	tilemaps_sync();
	copy_queue_process();
	gpu_pal_upload();
}


void handlers_clear() {
	vblank_handler_set(0);
	hblank_handler_set(0);
	set_callback1(0);
	set_callback2(0);
}

void c2_dexnav_gui()
{
    obj_sync_superstate();
    objc_exec();
    process_palfade();
    task_exec();
    tilemaps_sync();
    // merge textbox and text tile maps
    remoboxes_upload_tilesets();
}


void reset_pal_settings() {
	pal_fade_control_and_dead_struct_reset();
	palette_bg_faded_fill_black();
	gpu_pal_allocator_reset();
	*gpu_pal_tag_search_lower_boundary = 0;
}


void reset_bg_settings() {
	overworld_free_bgmaps();
	gpu_tile_bg_drop_all_sets(0);
	bgid_mod_x_offset(0, 0, 0);
    bgid_mod_y_offset(0, 0, 0);
	bgid_mod_x_offset(1, 0, 0);
    bgid_mod_y_offset(1, 0, 0);
	bgid_mod_x_offset(2, 0, 0);
    bgid_mod_y_offset(2, 0, 0);
	bgid_mod_x_offset(3, 0, 0);
    bgid_mod_y_offset(3, 0, 0);
}

void reset_boxes() {
	remo_reset_acknowledgement_flags();
	rboxes_free();
}


void setup()
{
    // callbacks
    handlers_clear();
    // BGs
    reset_bg_settings();
    // pals
    reset_pal_settings();
    // objs
    obj_and_aux_reset_all();
    gpu_tile_obj_tags_reset();
    // VRAM clear
    u32 set = 0;
    CpuFastSet((void*)&set, (void*)ADDR_VRAM, CPUModeFS(0x10000, CPUFSSET));
    // tasks
    malloc_init((void*)0x2000000, 0x1C000);
    tasks_init();
}



// overworld cave animation
void OeiCaveEffect() {
    struct SpritePalette caveSmoke = {(const u8*)&caveSmokePal, 0x1005};
    GpuPalObjAllocTagAndApply(&caveSmoke);
    GpuPalApply((u8 *)&caveSmokePal, 29 * 16, 32);
    LogCoordsCameraRelative(&gFieldEffectArguments.effect_pos.x, &gFieldEffectArguments.effect_pos.y, 8, 8);
    u8 objId = TemplateInstanciateReverseSearch(&objt_cave, gFieldEffectArguments.effect_pos.x, gFieldEffectArguments.effect_pos.y, 0xFF);
    if (objId != 64) {
        objects[objId].bitfield2 |= 2;
        objects[objId].priv[0] = 22;
    }

}


u8 SelectHookRestore(void)
{
    script_env_init_script((void*) 0x81A77A0); // script that lets you know you can register item via select
    return 0;
}



u8 CheckRegisteredSelect(void) {
    u16 var_val = var_load(DEXNAV_VAR);
    if (var_val) {
        if (task_is_running(dexnav_hud_manage)) {
            // don't redo the search...
            return 0;
        }
        init_dexnav_hud(var_val & 0x7FFF, var_val >> 15);
        return 0;
    }
    return 1;
}



// ===== POKETOOLS ===== //
void ToolSelection(u8 taskId) {
    
    switch (priv0) {
        case 0:
            if (ScriptEnv2IsEnabled())
                return;
            priv0++;
			break;
        case 1:
            script_env_enable();
            u8 boxId = rboxId_init(&tbox);
            rboxId_clear_pixels(boxId, 0x11);
            outline_box(boxId, 1, 0x214, 0xE);
            rboxId_print(boxId, 1, 8, 2, &MenuTextBlack, 0, menu_text);
            choice_setup_simple(boxId, 2, 0, 1, 16, TOOL_COUNT, 0);
            rboxId_update(boxId, 3);
            rboxId_tilemap_update(boxId);
            tasks[taskId].data[1] = boxId;
            priv0++;
            break;
        case 2:
        {
            s8 choice = rbox_choice_update();
            if (choice == 0)
			{
                //pokedex
                StartMenuPokedexFunc();
                DestroyTask(taskId);
            } 
			else if (choice == 1)
			{
                // dexnav
                exec_dexnav();
                DestroyTask(taskId);
            } 
			else if (choice == -1)
			{
                // b pressed, exit
                rboxId_clean(tasks[taskId].data[1], 1);
                rboxId_free(tasks[taskId].data[1]);
                script_env_disable();
                DestroyTask(taskId);
            }
            return;
        }
    };

}


void CloseStartMenu() {
	CloseSafariStepsBox();	// void safari_stepscount_close(void) 0806EF18
	CloseStartMenuDescriptionBox();	// void sm_close_description(void) 080F7998
	HideStartMenu();		// void sm_close_menu(void) 0806FEA0
}


u8 PokeToolsFunc(void) {
    CloseStartMenu();
    CreateTask(ToolSelection, 0);
    return 1;
}


*/

