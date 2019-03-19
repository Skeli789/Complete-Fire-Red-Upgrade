	.text
	.align 1
	
	.force_thumb
	.syntax unified
	.global __gnu_thumb1_case_uqi
	.thumb_func
	.type __gnu_thumb1_case_uqi,function
__gnu_thumb1_case_uqi:
	push	{r1}
	mov	r1, lr
	lsrs	r1, r1, #1
	lsls	r1, r1, #1
	ldrb	r1, [r1, r0]
	lsls	r1, r1, #1
	add	lr, lr, r1
	pop	{r1}
	bx	lr
	.size __gnu_thumb1_case_uqi, . - __gnu_thumb1_case_uqi
	
	.force_thumb
	.syntax unified
	.global __gnu_thumb1_case_uhi
	.thumb_func
	.type __gnu_thumb1_case_uhi,function
__gnu_thumb1_case_uhi:
	push	{r0, r1}
	mov	r1, lr
	lsrs	r1, r1, #1
	lsls	r0, r0, #1
	lsls	r1, r1, #1
	ldrh	r1, [r1, r0]
	lsls	r1, r1, #1
	add	lr, lr, r1
	pop	{r0, r1}
	bx	lr
	.size __gnu_thumb1_case_uhi, . - __gnu_thumb1_case_uhi

    .force_thumb
    .syntax unified
    .global __gnu_thumb1_case_sqi
    .thumb_func
    .type __gnu_thumb1_case_sqi,function
__gnu_thumb1_case_sqi:
     push    {r1}
     mov r1, lr
     lsrs    r1, r1, #1
     lsls    r1, r1, #1
     ldrsb   r1, [r1, r0]
     lsls    r1, r1, #1
     add lr, r1
     pop {r1}
     bx  lr
    .size __gnu_thumb1_case_sqi, . - __gnu_thumb1_case_sqi
	
	.force_thumb
	.syntax unified
    .global __gnu_thumb1_case_shi
    .thumb_func
    .type __gnu_thumb1_case_shi,function
__gnu_thumb1_case_shi:
	push    {r0, r1}
	mov     r1, lr
	lsrs    r1, r1, #1
	lsls    r0, r0, #1
	lsls    r1, r1, #1
	ldrsh   r1, [r1, r0]
	lsls    r1, r1, #1
	add     lr, lr, r1
	pop     {r0, r1}
	bx      lr
    .size __gnu_thumb1_case_shi, . - __gnu_thumb1_case_shi

	.force_thumb
	.syntax unified
    .global __gnu_thumb1_case_si
    .thumb_func
    .type __gnu_thumb1_case_shi,function
__gnu_thumb1_case_si:
        push    {r0, r1}
        mov     r1, lr
        adds.n  r1, r1, #2      /* Align to word.  */
        lsrs    r1, r1, #2
        lsls    r0, r0, #2
        lsls    r1, r1, #2
        ldr     r0, [r1, r0]
        adds    r0, r0, r1
        mov     lr, r0
        pop     {r0, r1}
        mov     pc, lr          /* We know we were called from thumb code.  */
    .size __gnu_thumb1_case_si, . - __gnu_thumb1_case_si

    .force_thumb
    .syntax unified
    .global __aeabi_uidiv
    .thumb_func
    .type __aeabi_uidiv,function
__aeabi_uidiv:
	ldr r3, =(__udivsi3)
	bx r3
	.size __aeabi_uidiv, . - __aeabi_uidiv
	
    .force_thumb
    .syntax unified
    .global __aeabi_idiv
    .thumb_func
    .type __aeabi_idiv,function
__aeabi_idiv:
	ldr r3, =(__divsi3)
	bx r3
	.size __aeabi_idiv, . - __aeabi_idiv

    .force_thumb
    .syntax unified
    .global __aeabi_idivmod
    .thumb_func
    .type __aeabi_idivmod,function
__aeabi_idivmod:
	push {lr}
	bl ActualModulationInt
.byte 0x01, 0x1C @;mov r1, r0 - Return to r1 for some reason
	pop {pc}
	
ActualModulationInt:
	ldr r3, =(__modsi3)
	bx r3
	.size __aeabi_idivmod, . - __aeabi_idivmod

    .force_thumb
    .syntax unified
    .global __aeabi_uidivmod
    .thumb_func
    .type __aeabi_uidivmod,function
__aeabi_uidivmod:
	push {lr}
	bl ActualModulationUInt
.byte 0x01, 0x1C @;mov r1, r0 - Return to r1 for some reason
	pop {pc}
	
ActualModulationUInt:
	ldr r3, =(__umodsi3)
	bx r3
	.size __aeabi_uidivmod, . - __aeabi_uidivmod

    .force_thumb
    .syntax unified
    .global memcpy
    .thumb_func
    .type memcpy,function
memcpy:
	ldr r3, =(memcpy_)
	bx r3
	.size memcpy, . - memcpy
	
    .force_thumb
    .syntax unified
    .global memset
    .thumb_func
    .type memset,function
memset:
	ldr r3, =(memset_)
	bx r3
	.size memset, . - memset
	
    .force_thumb
    .syntax unified
    .global malloc
    .thumb_func
    .type malloc,function
malloc:
	ldr r3, =(malloc_)
	bx r3
	.size malloc, . - malloc
	
    .force_thumb
    .syntax unified
    .global free
    .thumb_func
    .type free,function
free:
	ldr r1, =(free_)
	bx r1
	.size free, . - free

    .force_thumb
    .syntax unified
    .global read_word
    .thumb_func
    .type read_word,function
read_word:
	movs r2, r0
	ldrb r0, [r2, #3]
	lsls r0, r0, #0x18
	ldrb r1, [r2, #2]
	lsls r1, r1, #0x10
	orrs r0, r1
	ldrb r1, [r2, #1]
	lsls r1, r1, #8
	orrs r0, r1
	ldrb r1, [r2]
	orrs r0, r1
	bx lr
	
    .force_thumb
    .syntax unified
    .global rand
    .thumb_func
    .type rand,function
rand:
	ldr r2, =(0x3005000)	@rand_seed
	ldr r1, [r2]
	ldr r0, =(0x41C64E6D)
	muls r0, r1
	ldr r1, =(0x6073)
	adds r0, r0, r1
	str r0, [r2]
	lsrs r0, r0, #0x10
	bx lr
