#pragma once

#include "global.h"

struct ScriptContext;

typedef bool8 (*ScrCmdFunc)(struct ScriptContext *);
typedef u8 Script[];


struct ScriptContext
{
    u8 stackDepth;
    u8 mode;
    u8 comparisonResult;
    u8 (*nativePtr)(void);
	//ScrCmdFunc *scriptAsmPointer;
    const u8 *scriptPtr;
    const u8* stack[20];
    ScrCmdFunc* cmdTable;
    ScrCmdFunc* cmdTableEnd;
    u32 data[4];
	bool8 enabled;
	u8 field_75;
    u8 keypad_control_enabled;	// if true, can walk away to cancel script (eg. signpost)
    u8 keypad_override_direction;	// Locks player movement.
};

extern u8* sScriptStringVars[]; //Text buffers
#define gTextColourCurrent (*((u16*) 0x20370DA))
#define gTextColourBackup (*((u16*) 0x20370DC))

extern struct ScriptContext gScriptEnv1;	//03000EB0
extern struct ScriptContext gScriptEnv2;	//03000F28
extern const u8* gLoadPointer; //0x03000f14
extern const u8* gRAMScriptPtr; //0x20370A4
extern const u8 *const gStdScripts[];
extern const u8 *const gStdScriptsEnd[];

#define ScriptReadByte(ctx) (*(ctx->scriptPtr++))
u16 __attribute__((long_call)) ScriptReadHalfword(struct ScriptContext *ctx);
u32 __attribute__((long_call)) ScriptReadWord(struct ScriptContext *ctx);
void __attribute__((long_call)) MapHeaderRunScriptByTag(u8 tag);
u8* __attribute__((long_call)) MapHeaderCheckScriptTable(u8 tag);
void __attribute__((long_call)) ScriptContext1_SetupScript(const u8* ptr);
void __attribute__((long_call)) ScriptCall(struct ScriptContext *ctx, const u8 *ptr);
void __attribute__((long_call)) SetTextboxSignpostDesign(void);
void __attribute__((long_call)) EnableBothScriptContexts(void);
bool8 __attribute__((long_call)) ScriptContext2_IsEnabled(void);
void __attribute__((long_call)) ScriptContext2_Enable(void);
void __attribute__((long_call)) ScriptContext2_Disable(void);
void __attribute__((long_call)) ScriptContext1_Init(void);
bool8 __attribute__((long_call)) ScriptContext1_IsScriptSetUp(void);
void __attribute__((long_call)) ScriptContext1_Stop(void);
void __attribute__((long_call)) SetupNativeScript(struct ScriptContext *ctx, bool8 (*ptr)(void));

/*
void InitScriptContext(struct ScriptContext *ctx, void *cmdTable, void *cmdTableEnd);
u8 SetupBytecodeScript(struct ScriptContext *ctx, const u8 *ptr);
void StopScript(struct ScriptContext *ctx);
bool8 RunScriptCommand(struct ScriptContext *ctx);
u8 ScriptPush(struct ScriptContext *ctx, const u8 *ptr);
const u8 *ScriptPop(struct ScriptContext *ctx);
void ScriptJump(struct ScriptContext *ctx, const u8 *ptr);
void ScriptReturn(struct ScriptContext *ctx);
u16 ScriptReadHalfword(struct ScriptContext *ctx);
u32 ScriptReadWord(struct ScriptContext *ctx);
bool8 ScriptContext1_IsScriptSetUp(void);
bool8 ScriptContext2_RunScript(void);
//void ScriptContext1_SetupScript(const u8 *ptr);
void EnableBothScriptContexts(void);
void ScriptContext2_RunNewScript(const u8 *ptr);
u8 *mapheader_get_tagged_pointer(u8 tag);
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
*/
