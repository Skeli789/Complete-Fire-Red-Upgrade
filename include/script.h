#ifndef GUARD_SCRIPT_H
#define GUARD_SCRIPT_H

#include "global.h"

struct ScriptContext;

typedef bool8 (*ScrCmdFunc)(struct ScriptContext *);
typedef u8 Script[];

/*
struct ScriptContext
{
    u8 stackDepth;
    u8 mode;
    u8 comparisonResult;
    u8 (*nativePtr)(void);
    const u8 *scriptPtr;
    const u8 *stack[20];
    ScrCmdFunc *cmdTable;
    ScrCmdFunc *cmdTableEnd;
    u32 data[4];
};
*/


typedef u8 (*ScriptFunction)(void);
//typedef u8 (*ScriptCommand)(struct ScriptEnvironment* environment);

enum ScriptExecutionMode {
    /** The environment is not executing anything (wait state). */
    SCRIPT_EXECUTE_MODE_PAUSED,
    /** Normal script execution mode. */
    SCRIPT_EXECUTE_MODE_NORMAL,
    /** Executing pointer_asm function. */
    SCRIPT_EXECUTE_MODE_ASM,
};

struct ScriptContext {
    u8 depth;
    enum ScriptExecutionMode mode;
    u8 cmp_result;		//Function to run.
    u8 field_3;
    ScriptFunction pointer_asm;	//executed when mode = SCRIPT_EXECUTE_MODE_ASM
    u8* pointer_script;	//Executed when mode = SCRIPT_EXECUTE_MODE_NORMAL
    u8* stack[20];	//Call stack.
    ScrCmdFunc* cmd_table;	//pointer to script cmds
    ScrCmdFunc* cmd_table_max;	//Pointer to the last command in the table.
    u32 vars[4];	//Script banks.  Bank 0 is used for loading a pointer to a message to display.
    bool8 enabled;	//Usually set to false during cutscenes.
    u8 field_75;
    u8 keypad_control_enabled;	// if true, can walk away to cancel script (eg. signpost)
    u8 keypad_override_direction;	// Locks player movement.
};

typedef struct ScriptEnvironment gScriptEnv1;	//03000EB0
typedef struct ScriptEnvironment gScriptEnv2;	//03000F28


#define ScriptReadByte(ctx) (*(ctx->scriptPtr++))

void InitScriptContext(struct ScriptContext *ctx, void *cmdTable, void *cmdTableEnd);
u8 SetupBytecodeScript(struct ScriptContext *ctx, const u8 *ptr);
void SetupNativeScript(struct ScriptContext *ctx, bool8 (*ptr)(void));
void StopScript(struct ScriptContext *ctx);
bool8 RunScriptCommand(struct ScriptContext *ctx);
u8 ScriptPush(struct ScriptContext *ctx, const u8 *ptr);
const u8 *ScriptPop(struct ScriptContext *ctx);
void ScriptJump(struct ScriptContext *ctx, const u8 *ptr);
void ScriptCall(struct ScriptContext *ctx, const u8 *ptr);
void ScriptReturn(struct ScriptContext *ctx);
u16 ScriptReadHalfword(struct ScriptContext *ctx);
u32 ScriptReadWord(struct ScriptContext *ctx);
void ScriptContext2_Enable(void);
void ScriptContext2_Disable(void);
bool8 ScriptContext2_IsEnabled(void);
void ScriptContext1_Init(void);
bool8 ScriptContext1_IsScriptSetUp(void);
bool8 ScriptContext2_RunScript(void);
//void ScriptContext1_SetupScript(const u8 *ptr);
void ScriptContext1_Stop(void);
void EnableBothScriptContexts(void);
void ScriptContext2_RunNewScript(const u8 *ptr);
u8 *mapheader_get_tagged_pointer(u8 tag);
void mapheader_run_script_by_tag(u8 tag);
u8 *mapheader_get_first_match_from_tagged_ptr_list(u8 tag);
void mapheader_run_script_with_tag_x1(void);
void mapheader_run_script_with_tag_x3(void);
void mapheader_run_script_with_tag_x5(void);
void mapheader_run_script_with_tag_x7(void);
void mapheader_run_script_with_tag_x6(void);
bool8 mapheader_run_first_tag2_script_list_match(void);
void mapheader_run_first_tag4_script_list_match(void);
u32 CalculateRamScriptChecksum(void);
void ClearRamScript(void);
bool8 InitRamScript(u8 *script, u16 scriptSize, u8 mapGroup, u8 mapNum, u8 objectId);
u8 *GetRamScript(u8 objectId, u8 *script);
bool32 sub_80991F8(void);
u8 *sub_8099244(void);
void sub_80992A0(u8 *script, u16 scriptSize);

#endif // GUARD_SCRIPT_H
