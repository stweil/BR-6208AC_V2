	.section .mdebug.abi32
	.previous
	.abicalls
	.section	.debug_abbrev,"",@progbits
$Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
$Ldebug_info0:
	.section	.debug_line,"",@progbits
$Ldebug_line0:
	.text
$Ltext0:
	.align	2
	.globl	rtl8370_setAsicLedGroupMode
	.ent	rtl8370_setAsicLedGroupMode
	.type	rtl8370_setAsicLedGroupMode, @function
rtl8370_setAsicLedGroupMode:
$LFB2:
	.file 1 "rtl8370_asicdrv_led.c"
	.loc 1 204 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 204 0
	move	$2,$4
	.loc 1 212 0
	sll	$5,$4,12
	.loc 1 209 0
	sltu	$2,$2,4
	.loc 1 214 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	ori	$5,$5,0x4000
	.loc 1 209 0
	beq	$2,$0,$L3
	li	$4,6915			# 0x1b03

	.loc 1 214 0
	jr	$25
	nop

$L3:
	.loc 1 217 0
	j	$31
	li	$2,1			# 0x1

	.set	macro
	.set	reorder
$LFE2:
	.end	rtl8370_setAsicLedGroupMode
	.align	2
	.globl	rtl8370_setAsicLedGroupEnable
	.ent	rtl8370_setAsicLedGroupEnable
	.type	rtl8370_setAsicLedGroupEnable, @function
rtl8370_setAsicLedGroupEnable:
$LFB3:
	.loc 1 587 0
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 16, gp= 8
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-32
$LCFI0:
	sw	$31,24($sp)
$LCFI1:
	.cprestore	16
	.loc 1 592 0
	sltu	$2,$4,3
	.loc 1 587 0
	move	$6,$5
	.loc 1 595 0
	sltu	$7,$5,256
	.loc 1 593 0
	.loc 1 592 0
	beq	$2,$0,$L4
	li	$3,1			# 0x1

	.loc 1 600 0
	andi	$5,$4,0x1
	sll	$5,$5,3
	.loc 1 598 0
	srl	$4,$4,1
	.loc 1 600 0
	li	$2,255			# 0xff
	.loc 1 602 0
	lw	$25,%call16(rtl8370_setAsicRegBits)($28)
	sll	$5,$2,$5
	.loc 1 595 0
	beq	$7,$0,$L4
	addiu	$4,$4,6948

	.loc 1 602 0
	jalr	$25
	nop

	lw	$28,16($sp)
	move	$3,$2
$L4:
	.loc 1 606 0
	lw	$31,24($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
$LFE3:
	.end	rtl8370_setAsicLedGroupEnable
	.section	.debug_frame,"",@progbits
$Lframe0:
	.4byte	$LECIE0-$LSCIE0
$LSCIE0:
	.4byte	0xffffffff
	.byte	0x1
	.ascii	"\000"
	.uleb128 0x1
	.sleb128 4
	.byte	0x1f
	.byte	0xc
	.uleb128 0x1d
	.uleb128 0x0
	.align	2
$LECIE0:
$LSFDE0:
	.4byte	$LEFDE0-$LASFDE0
$LASFDE0:
	.4byte	$Lframe0
	.4byte	$LFB2
	.4byte	$LFE2-$LFB2
	.align	2
$LEFDE0:
$LSFDE2:
	.4byte	$LEFDE2-$LASFDE2
$LASFDE2:
	.4byte	$Lframe0
	.4byte	$LFB3
	.4byte	$LFE3-$LFB3
	.byte	0x4
	.4byte	$LCFI0-$LFB3
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	$LCFI1-$LCFI0
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -2
	.align	2
$LEFDE2:
	.align	0
	.file 2 "/home/IOData/BR6456IN/srcbase/boot/GPL_4100/include/realtek/rtk_types.h"
	.text
$Letext0:
	.section	.debug_info
	.4byte	0x122
	.2byte	0x2
	.4byte	$Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	$Ldebug_line0
	.4byte	$Letext0
	.4byte	$Ltext0
	.4byte	$LASF17
	.byte	0x1
	.4byte	$LASF18
	.4byte	$LASF19
	.uleb128 0x2
	.4byte	$LASF0
	.byte	0x8
	.byte	0x7
	.uleb128 0x2
	.4byte	$LASF1
	.byte	0x8
	.byte	0x5
	.uleb128 0x3
	.4byte	$LASF3
	.byte	0x2
	.byte	0x10
	.4byte	0x3e
	.uleb128 0x2
	.4byte	$LASF2
	.byte	0x4
	.byte	0x7
	.uleb128 0x3
	.4byte	$LASF4
	.byte	0x2
	.byte	0x11
	.4byte	0x50
	.uleb128 0x4
	.ascii	"int\000"
	.byte	0x4
	.byte	0x5
	.uleb128 0x2
	.4byte	$LASF5
	.byte	0x2
	.byte	0x7
	.uleb128 0x2
	.4byte	$LASF6
	.byte	0x2
	.byte	0x5
	.uleb128 0x2
	.4byte	$LASF7
	.byte	0x1
	.byte	0x8
	.uleb128 0x2
	.4byte	$LASF8
	.byte	0x1
	.byte	0x6
	.uleb128 0x2
	.4byte	$LASF2
	.byte	0x4
	.byte	0x7
	.uleb128 0x3
	.4byte	$LASF9
	.byte	0x2
	.byte	0x4e
	.4byte	0x45
	.uleb128 0x5
	.4byte	0xc8
	.byte	0x1
	.4byte	$LASF20
	.byte	0x1
	.byte	0xcc
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB2
	.4byte	$LFE2
	.4byte	$LSFDE0
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF12
	.byte	0x1
	.byte	0xcb
	.4byte	0x33
	.byte	0x1
	.byte	0x52
	.uleb128 0x7
	.4byte	$LASF10
	.byte	0x1
	.byte	0xcd
	.4byte	0x7a
	.uleb128 0x7
	.4byte	$LASF11
	.byte	0x1
	.byte	0xce
	.4byte	0x33
	.byte	0x0
	.uleb128 0x8
	.byte	0x1
	.4byte	$LASF21
	.byte	0x1
	.2byte	0x24b
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB3
	.4byte	$LFE3
	.4byte	$LSFDE2
	.byte	0x1
	.byte	0x6d
	.uleb128 0x9
	.4byte	$LASF13
	.byte	0x1
	.2byte	0x24a
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x9
	.4byte	$LASF14
	.byte	0x1
	.2byte	0x24a
	.4byte	0x33
	.byte	0x1
	.byte	0x56
	.uleb128 0xa
	.4byte	$LASF10
	.byte	0x1
	.2byte	0x24c
	.4byte	0x7a
	.uleb128 0xa
	.4byte	$LASF15
	.byte	0x1
	.2byte	0x24d
	.4byte	0x33
	.uleb128 0xa
	.4byte	$LASF16
	.byte	0x1
	.2byte	0x24e
	.4byte	0x33
	.byte	0x0
	.byte	0x0
	.section	.debug_abbrev
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x10
	.uleb128 0x6
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x2001
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x2001
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.4byte	0x50
	.2byte	0x2
	.4byte	$Ldebug_info0
	.4byte	0x126
	.4byte	0x85
	.ascii	"rtl8370_setAsicLedGroupMode\000"
	.4byte	0xc8
	.ascii	"rtl8370_setAsicLedGroupEnable\000"
	.4byte	0x0
	.section	.debug_aranges,"",@progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	$Ldebug_info0
	.byte	0x4
	.byte	0x0
	.2byte	0x0
	.2byte	0x0
	.4byte	$Ltext0
	.4byte	$Letext0-$Ltext0
	.4byte	0x0
	.4byte	0x0
	.section	.debug_str,"MS",@progbits,1
$LASF7:
	.ascii	"unsigned char\000"
$LASF9:
	.ascii	"ret_t\000"
$LASF3:
	.ascii	"uint32\000"
$LASF10:
	.ascii	"retVal\000"
$LASF0:
	.ascii	"long long unsigned int\000"
$LASF20:
	.ascii	"rtl8370_setAsicLedGroupMode\000"
$LASF1:
	.ascii	"long long int\000"
$LASF4:
	.ascii	"int32\000"
$LASF16:
	.ascii	"regDataMask\000"
$LASF13:
	.ascii	"group\000"
$LASF21:
	.ascii	"rtl8370_setAsicLedGroupEnable\000"
$LASF19:
	.ascii	"/home/IOData/BR6456IN/srcbase/boot/realtek_8370\000"
$LASF2:
	.ascii	"unsigned int\000"
$LASF5:
	.ascii	"short unsigned int\000"
$LASF18:
	.ascii	"rtl8370_asicdrv_led.c\000"
$LASF6:
	.ascii	"short int\000"
$LASF8:
	.ascii	"char\000"
$LASF11:
	.ascii	"regData\000"
$LASF15:
	.ascii	"regAddr\000"
$LASF17:
	.ascii	"GNU C 3.4.2 -g\000"
$LASF12:
	.ascii	"mode\000"
$LASF14:
	.ascii	"portmask\000"
	.ident	"GCC: (GNU) 3.4.2"
