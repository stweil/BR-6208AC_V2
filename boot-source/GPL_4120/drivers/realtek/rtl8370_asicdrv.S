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
	.globl	rtl8370_setAsicRegBit
	.ent	rtl8370_setAsicRegBit
	.type	rtl8370_setAsicRegBit, @function
rtl8370_setAsicRegBit:
$LFB2:
	.file 1 "rtl8370_asicdrv.c"
	.loc 1 161 0
	.frame	$sp,48,$31		# vars= 8, regs= 4/0, args= 16, gp= 8
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-48
$LCFI0:
	sw	$31,44($sp)
$LCFI1:
	sw	$18,40($sp)
$LCFI2:
	sw	$17,36($sp)
$LCFI3:
	sw	$16,32($sp)
$LCFI4:
	.cprestore	16
	.loc 1 161 0
	move	$16,$5
	.loc 1 215 0
	sltu	$2,$16,16
	.loc 1 218 0
	lw	$25,%call16(smi_read)($28)
	addiu	$5,$sp,24
	.loc 1 161 0
	move	$17,$6
	move	$18,$4
	.loc 1 216 0
	.loc 1 215 0
	beq	$2,$0,$L1
	li	$3,1			# 0x1

	.loc 1 218 0
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 220 0
	.loc 1 219 0
	bne	$2,$0,$L1
	li	$3,14			# 0xe

	.loc 1 228 0
	li	$2,1			# 0x1
	sll	$2,$2,$16
	.loc 1 226 0
	move	$3,$2
	.loc 1 228 0
	nor	$4,$0,$2
	.loc 1 225 0
	beq	$17,$0,$L4
	lw	$2,24($sp)

	.loc 1 226 0
	b	$L7
	or	$2,$2,$3

$L4:
	.loc 1 228 0
	and	$2,$2,$4
$L7:
	sw	$2,24($sp)
	.loc 1 230 0
	lw	$25,%call16(smi_write)($28)
	lw	$5,24($sp)
	jalr	$25
	move	$4,$18

	lw	$28,16($sp)
	.loc 1 232 0
	li	$3,14			# 0xe
	movz	$3,$0,$2
$L1:
	.loc 1 241 0
	lw	$31,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
$LFE2:
	.end	rtl8370_setAsicRegBit
	.align	2
	.globl	rtl8370_getAsicRegBit
	.ent	rtl8370_getAsicRegBit
	.type	rtl8370_getAsicRegBit, @function
rtl8370_getAsicRegBit:
$LFB3:
	.loc 1 244 0
	.frame	$sp,48,$31		# vars= 8, regs= 3/0, args= 16, gp= 8
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-48
$LCFI5:
	sw	$31,40($sp)
$LCFI6:
	sw	$17,36($sp)
$LCFI7:
	sw	$16,32($sp)
$LCFI8:
	.cprestore	16
	.loc 1 280 0
	lw	$25,%call16(smi_read)($28)
	.loc 1 244 0
	move	$16,$5
	.loc 1 280 0
	addiu	$5,$sp,24
	.loc 1 244 0
	.loc 1 280 0
	jalr	$25
	move	$17,$6

	.loc 1 288 0
	li	$3,1			# 0x1
	.loc 1 280 0
	lw	$28,16($sp)
	.loc 1 288 0
	sll	$3,$3,$16
	.loc 1 282 0
	.loc 1 281 0
	bne	$2,$0,$L8
	li	$4,14			# 0xe

	.loc 1 288 0
	lw	$2,24($sp)
	.loc 1 291 0
	move	$4,$0
	.loc 1 288 0
	and	$2,$2,$3
	srl	$2,$2,$16
	sw	$2,0($17)
$L8:
	.loc 1 292 0
	lw	$31,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$4
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
$LFE3:
	.end	rtl8370_getAsicRegBit
	.align	2
	.globl	rtl8370_setAsicRegBits
	.ent	rtl8370_setAsicRegBits
	.type	rtl8370_setAsicRegBits, @function
rtl8370_setAsicRegBits:
$LFB4:
	.loc 1 308 0
	.frame	$sp,48,$31		# vars= 8, regs= 4/0, args= 16, gp= 8
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-48
$LCFI9:
	sw	$31,44($sp)
$LCFI10:
	sw	$18,40($sp)
$LCFI11:
	sw	$17,36($sp)
$LCFI12:
	sw	$16,32($sp)
$LCFI13:
	.cprestore	16
	.loc 1 386 0
	li	$2,65535			# 0xffff
	sltu	$2,$2,$5
	.loc 1 308 0
	move	$18,$4
	move	$17,$5
	.loc 1 387 0
	.loc 1 386 0
	bne	$2,$0,$L10
	li	$4,1			# 0x1

	.loc 1 394 0
	andi	$2,$5,0x1
	.loc 1 389 0
	.loc 1 394 0
	bne	$2,$0,$L20
	move	$3,$0

	li	$5,1			# 0x1
$L15:
	.loc 1 392 0
	addiu	$3,$3,1
	.loc 1 394 0
	sll	$2,$5,$3
	and	$4,$17,$2
	.loc 1 393 0
	sltu	$2,$3,16
	beq	$2,$0,$L21
	nop

	.loc 1 394 0
	beq	$4,$0,$L15
	nop

$L20:
	.loc 1 396 0
	sll	$16,$6,$3
	.loc 1 398 0
	li	$2,65535			# 0xffff
	sltu	$2,$2,$16
	.loc 1 399 0
	.loc 1 398 0
	bne	$2,$0,$L10
	li	$4,1			# 0x1

	.loc 1 401 0
	lw	$25,%call16(smi_read)($28)
	move	$4,$18
	jalr	$25
	addiu	$5,$sp,24

	lw	$28,16($sp)
	.loc 1 403 0
	.loc 1 402 0
	bne	$2,$0,$L10
	li	$4,14			# 0xe

	.loc 1 409 0
	lw	$5,24($sp)
	nor	$2,$0,$17
	.loc 1 410 0
	and	$3,$16,$17
	.loc 1 409 0
	and	$5,$5,$2
	.loc 1 412 0
	lw	$25,%call16(smi_write)($28)
	.loc 1 410 0
	or	$5,$5,$3
	.loc 1 412 0
	move	$4,$18
	.loc 1 410 0
	.loc 1 412 0
	jalr	$25
	sw	$5,24($sp)

	lw	$28,16($sp)
	.loc 1 414 0
	.loc 1 413 0
	bne	$2,$0,$L10
	li	$4,14			# 0xe

	.loc 1 420 0
	b	$L18
	move	$4,$0

$L21:
	li	$4,1			# 0x1
$L18:
$L10:
	.loc 1 421 0
	lw	$31,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$4
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
$LFE4:
	.end	rtl8370_setAsicRegBits
	.align	2
	.globl	rtl8370_getAsicRegBits
	.ent	rtl8370_getAsicRegBits
	.type	rtl8370_getAsicRegBits, @function
rtl8370_getAsicRegBits:
$LFB5:
	.loc 1 424 0
	.frame	$sp,48,$31		# vars= 8, regs= 4/0, args= 16, gp= 8
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-48
$LCFI14:
	sw	$31,44($sp)
$LCFI15:
	sw	$18,40($sp)
$LCFI16:
	sw	$17,36($sp)
$LCFI17:
	sw	$16,32($sp)
$LCFI18:
	.cprestore	16
	.loc 1 479 0
	li	$2,65535			# 0xffff
	sltu	$2,$2,$5
	.loc 1 424 0
	move	$17,$5
	move	$18,$6
	.loc 1 480 0
	.loc 1 479 0
	bne	$2,$0,$L22
	li	$3,1			# 0x1

	.loc 1 487 0
	andi	$2,$5,0x1
	.loc 1 482 0
	.loc 1 487 0
	bne	$2,$0,$L30
	move	$16,$0

	li	$5,1			# 0x1
$L27:
	.loc 1 485 0
	addiu	$16,$16,1
	.loc 1 487 0
	sll	$2,$5,$16
	and	$3,$17,$2
	.loc 1 486 0
	sltu	$2,$16,16
	beq	$2,$0,$L31
	nop

	.loc 1 487 0
	beq	$3,$0,$L27
	nop

$L30:
	.loc 1 490 0
	lw	$25,%call16(smi_read)($28)
	jalr	$25
	addiu	$5,$sp,24

	lw	$28,16($sp)
	.loc 1 492 0
	.loc 1 491 0
	bne	$2,$0,$L22
	li	$3,14			# 0xe

	.loc 1 494 0
	b	$L28
	lw	$2,24($sp)

$L31:
	.loc 1 487 0
	b	$L22
	li	$3,1			# 0x1

$L28:
	.loc 1 501 0
	move	$3,$0
	.loc 1 494 0
	and	$2,$2,$17
	srl	$2,$2,$16
	sw	$2,0($18)
$L22:
	.loc 1 502 0
	lw	$31,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
$LFE5:
	.end	rtl8370_getAsicRegBits
	.align	2
	.globl	rtl8370_setAsicReg
	.ent	rtl8370_setAsicReg
	.type	rtl8370_setAsicReg, @function
rtl8370_setAsicReg:
$LFB6:
	.loc 1 515 0
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 16, gp= 8
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-32
$LCFI19:
	sw	$31,24($sp)
$LCFI20:
	.cprestore	16
	.loc 1 550 0
	lw	$25,%call16(smi_write)($28)
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 560 0
	li	$3,14			# 0xe
	lw	$31,24($sp)
	movz	$3,$0,$2
	move	$2,$3
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
$LFE6:
	.end	rtl8370_setAsicReg
	.align	2
	.globl	rtl8370_getAsicReg
	.ent	rtl8370_getAsicReg
	.type	rtl8370_getAsicReg, @function
rtl8370_getAsicReg:
$LFB7:
	.loc 1 574 0
	.frame	$sp,40,$31		# vars= 8, regs= 2/0, args= 16, gp= 8
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-40
$LCFI21:
	sw	$31,36($sp)
$LCFI22:
	sw	$16,32($sp)
$LCFI23:
	.cprestore	16
	.loc 1 604 0
	lw	$25,%call16(smi_read)($28)
	.loc 1 574 0
	move	$16,$5
	.loc 1 604 0
	jalr	$25
	addiu	$5,$sp,24

	lw	$28,16($sp)
	.loc 1 606 0
	.loc 1 605 0
	bne	$2,$0,$L34
	li	$3,14			# 0xe

	.loc 1 608 0
	lw	$2,24($sp)
	.loc 1 615 0
	move	$3,$0
	.loc 1 608 0
	sw	$2,0($16)
$L34:
	.loc 1 616 0
	lw	$31,36($sp)
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
$LFE7:
	.end	rtl8370_getAsicReg
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
	.byte	0x4
	.4byte	$LCFI0-$LFB2
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.4byte	$LCFI4-$LCFI0
	.byte	0x11
	.uleb128 0x10
	.sleb128 -4
	.byte	0x11
	.uleb128 0x11
	.sleb128 -3
	.byte	0x11
	.uleb128 0x12
	.sleb128 -2
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -1
	.align	2
$LEFDE0:
$LSFDE2:
	.4byte	$LEFDE2-$LASFDE2
$LASFDE2:
	.4byte	$Lframe0
	.4byte	$LFB3
	.4byte	$LFE3-$LFB3
	.byte	0x4
	.4byte	$LCFI5-$LFB3
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.4byte	$LCFI8-$LCFI5
	.byte	0x11
	.uleb128 0x10
	.sleb128 -4
	.byte	0x11
	.uleb128 0x11
	.sleb128 -3
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -2
	.align	2
$LEFDE2:
$LSFDE4:
	.4byte	$LEFDE4-$LASFDE4
$LASFDE4:
	.4byte	$Lframe0
	.4byte	$LFB4
	.4byte	$LFE4-$LFB4
	.byte	0x4
	.4byte	$LCFI9-$LFB4
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.4byte	$LCFI13-$LCFI9
	.byte	0x11
	.uleb128 0x10
	.sleb128 -4
	.byte	0x11
	.uleb128 0x11
	.sleb128 -3
	.byte	0x11
	.uleb128 0x12
	.sleb128 -2
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -1
	.align	2
$LEFDE4:
$LSFDE6:
	.4byte	$LEFDE6-$LASFDE6
$LASFDE6:
	.4byte	$Lframe0
	.4byte	$LFB5
	.4byte	$LFE5-$LFB5
	.byte	0x4
	.4byte	$LCFI14-$LFB5
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.4byte	$LCFI18-$LCFI14
	.byte	0x11
	.uleb128 0x10
	.sleb128 -4
	.byte	0x11
	.uleb128 0x11
	.sleb128 -3
	.byte	0x11
	.uleb128 0x12
	.sleb128 -2
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -1
	.align	2
$LEFDE6:
$LSFDE8:
	.4byte	$LEFDE8-$LASFDE8
$LASFDE8:
	.4byte	$Lframe0
	.4byte	$LFB6
	.4byte	$LFE6-$LFB6
	.byte	0x4
	.4byte	$LCFI19-$LFB6
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	$LCFI20-$LCFI19
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -2
	.align	2
$LEFDE8:
$LSFDE10:
	.4byte	$LEFDE10-$LASFDE10
$LASFDE10:
	.4byte	$Lframe0
	.4byte	$LFB7
	.4byte	$LFE7-$LFB7
	.byte	0x4
	.4byte	$LCFI21-$LFB7
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	$LCFI23-$LCFI21
	.byte	0x11
	.uleb128 0x10
	.sleb128 -2
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -1
	.align	2
$LEFDE10:
	.align	0
	.file 2 "/home/IOData/BR6456IN/srcbase/boot/GPL_4100/include/realtek/rtk_types.h"
	.text
$Letext0:
	.section	.debug_info
	.4byte	0x35d
	.2byte	0x2
	.4byte	$Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	$Ldebug_line0
	.4byte	$Letext0
	.4byte	$Ltext0
	.4byte	$LASF24
	.byte	0x1
	.4byte	$LASF25
	.4byte	$LASF26
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
	.4byte	0x107
	.byte	0x1
	.4byte	$LASF13
	.byte	0x1
	.byte	0xa1
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB2
	.4byte	$LFE2
	.4byte	$LSFDE0
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.ascii	"reg\000"
	.byte	0x1
	.byte	0xa0
	.4byte	0x33
	.byte	0x1
	.byte	0x62
	.uleb128 0x6
	.ascii	"bit\000"
	.byte	0x1
	.byte	0xa0
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x7
	.4byte	$LASF10
	.byte	0x1
	.byte	0xa0
	.4byte	0x33
	.byte	0x1
	.byte	0x61
	.uleb128 0x8
	.4byte	$LASF11
	.byte	0x1
	.byte	0xd4
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x9
	.4byte	$LASF12
	.byte	0x1
	.byte	0xd5
	.4byte	0x7a
	.uleb128 0xa
	.4byte	0xf7
	.byte	0x1
	.4byte	$LASF14
	.byte	0x1
	.byte	0xda
	.4byte	0x50
	.byte	0x1
	.uleb128 0xb
	.byte	0x0
	.uleb128 0xc
	.byte	0x1
	.4byte	$LASF16
	.byte	0x1
	.byte	0xe6
	.4byte	0x50
	.byte	0x1
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.4byte	0x178
	.byte	0x1
	.4byte	$LASF15
	.byte	0x1
	.byte	0xf4
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB3
	.4byte	$LFE3
	.4byte	$LSFDE2
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.ascii	"reg\000"
	.byte	0x1
	.byte	0xf3
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x6
	.ascii	"bit\000"
	.byte	0x1
	.byte	0xf3
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x7
	.4byte	$LASF10
	.byte	0x1
	.byte	0xf3
	.4byte	0x178
	.byte	0x1
	.byte	0x61
	.uleb128 0xd
	.4byte	$LASF11
	.byte	0x1
	.2byte	0x115
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0xe
	.4byte	$LASF12
	.byte	0x1
	.2byte	0x116
	.4byte	0x7a
	.uleb128 0xc
	.byte	0x1
	.4byte	$LASF14
	.byte	0x1
	.byte	0xda
	.4byte	0x50
	.byte	0x1
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.byte	0x4
	.4byte	0x33
	.uleb128 0x10
	.4byte	0x222
	.byte	0x1
	.4byte	$LASF17
	.byte	0x1
	.2byte	0x134
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB4
	.4byte	$LFE4
	.4byte	$LSFDE4
	.byte	0x1
	.byte	0x6d
	.uleb128 0x11
	.ascii	"reg\000"
	.byte	0x1
	.2byte	0x133
	.4byte	0x33
	.byte	0x1
	.byte	0x62
	.uleb128 0x12
	.4byte	$LASF18
	.byte	0x1
	.2byte	0x133
	.4byte	0x33
	.byte	0x1
	.byte	0x61
	.uleb128 0x12
	.4byte	$LASF10
	.byte	0x1
	.2byte	0x133
	.4byte	0x33
	.byte	0x1
	.byte	0x56
	.uleb128 0xd
	.4byte	$LASF11
	.byte	0x1
	.2byte	0x17d
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0xe
	.4byte	$LASF12
	.byte	0x1
	.2byte	0x17e
	.4byte	0x7a
	.uleb128 0xd
	.4byte	$LASF19
	.byte	0x1
	.2byte	0x17f
	.4byte	0x33
	.byte	0x1
	.byte	0x53
	.uleb128 0xd
	.4byte	$LASF20
	.byte	0x1
	.2byte	0x180
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0xa
	.4byte	0x212
	.byte	0x1
	.4byte	$LASF14
	.byte	0x1
	.byte	0xda
	.4byte	0x50
	.byte	0x1
	.uleb128 0xb
	.byte	0x0
	.uleb128 0xc
	.byte	0x1
	.4byte	$LASF16
	.byte	0x1
	.byte	0xe6
	.4byte	0x50
	.byte	0x1
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.4byte	0x2a5
	.byte	0x1
	.4byte	$LASF21
	.byte	0x1
	.2byte	0x1a8
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB5
	.4byte	$LFE5
	.4byte	$LSFDE6
	.byte	0x1
	.byte	0x6d
	.uleb128 0x11
	.ascii	"reg\000"
	.byte	0x1
	.2byte	0x1a7
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x12
	.4byte	$LASF18
	.byte	0x1
	.2byte	0x1a7
	.4byte	0x33
	.byte	0x1
	.byte	0x61
	.uleb128 0x12
	.4byte	$LASF10
	.byte	0x1
	.2byte	0x1a7
	.4byte	0x178
	.byte	0x1
	.byte	0x62
	.uleb128 0xd
	.4byte	$LASF11
	.byte	0x1
	.2byte	0x1db
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0xe
	.4byte	$LASF12
	.byte	0x1
	.2byte	0x1dc
	.4byte	0x7a
	.uleb128 0xd
	.4byte	$LASF19
	.byte	0x1
	.2byte	0x1dd
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0xc
	.byte	0x1
	.4byte	$LASF14
	.byte	0x1
	.byte	0xda
	.4byte	0x50
	.byte	0x1
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.4byte	0x2fd
	.byte	0x1
	.4byte	$LASF22
	.byte	0x1
	.2byte	0x203
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB6
	.4byte	$LFE6
	.4byte	$LSFDE8
	.byte	0x1
	.byte	0x6d
	.uleb128 0x11
	.ascii	"reg\000"
	.byte	0x1
	.2byte	0x202
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x12
	.4byte	$LASF10
	.byte	0x1
	.2byte	0x202
	.4byte	0x33
	.byte	0x1
	.byte	0x55
	.uleb128 0xe
	.4byte	$LASF12
	.byte	0x1
	.2byte	0x224
	.4byte	0x7a
	.uleb128 0xc
	.byte	0x1
	.4byte	$LASF16
	.byte	0x1
	.byte	0xe6
	.4byte	0x50
	.byte	0x1
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.byte	0x1
	.4byte	$LASF23
	.byte	0x1
	.2byte	0x23e
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB7
	.4byte	$LFE7
	.4byte	$LSFDE10
	.byte	0x1
	.byte	0x6d
	.uleb128 0x11
	.ascii	"reg\000"
	.byte	0x1
	.2byte	0x23d
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x12
	.4byte	$LASF10
	.byte	0x1
	.2byte	0x23d
	.4byte	0x178
	.byte	0x1
	.byte	0x60
	.uleb128 0xd
	.4byte	$LASF11
	.byte	0x1
	.2byte	0x259
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0xe
	.4byte	$LASF12
	.byte	0x1
	.2byte	0x25a
	.4byte	0x7a
	.uleb128 0xc
	.byte	0x1
	.4byte	$LASF14
	.byte	0x1
	.byte	0xda
	.4byte	0x50
	.byte	0x1
	.uleb128 0xb
	.byte	0x0
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
	.uleb128 0x8
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
	.uleb128 0x8
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
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
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
	.uleb128 0xa
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
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0x18
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
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
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xe
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
	.uleb128 0xf
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
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
	.uleb128 0x11
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
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
	.uleb128 0x12
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
	.uleb128 0x13
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
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.4byte	0xa6
	.2byte	0x2
	.4byte	$Ldebug_info0
	.4byte	0x361
	.4byte	0x85
	.ascii	"rtl8370_setAsicRegBit\000"
	.4byte	0x107
	.ascii	"rtl8370_getAsicRegBit\000"
	.4byte	0x17e
	.ascii	"rtl8370_setAsicRegBits\000"
	.4byte	0x222
	.ascii	"rtl8370_getAsicRegBits\000"
	.4byte	0x2a5
	.ascii	"rtl8370_setAsicReg\000"
	.4byte	0x2fd
	.ascii	"rtl8370_getAsicReg\000"
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
$LASF10:
	.ascii	"value\000"
$LASF9:
	.ascii	"ret_t\000"
$LASF3:
	.ascii	"uint32\000"
$LASF12:
	.ascii	"retVal\000"
$LASF0:
	.ascii	"long long unsigned int\000"
$LASF1:
	.ascii	"long long int\000"
$LASF14:
	.ascii	"smi_read\000"
$LASF25:
	.ascii	"rtl8370_asicdrv.c\000"
$LASF4:
	.ascii	"int32\000"
$LASF13:
	.ascii	"rtl8370_setAsicRegBit\000"
$LASF16:
	.ascii	"smi_write\000"
$LASF21:
	.ascii	"rtl8370_getAsicRegBits\000"
$LASF26:
	.ascii	"/home/IOData/BR6456IN/srcbase/boot/realtek_8370\000"
$LASF22:
	.ascii	"rtl8370_setAsicReg\000"
$LASF2:
	.ascii	"unsigned int\000"
$LASF18:
	.ascii	"bits\000"
$LASF5:
	.ascii	"short unsigned int\000"
$LASF6:
	.ascii	"short int\000"
$LASF15:
	.ascii	"rtl8370_getAsicRegBit\000"
$LASF23:
	.ascii	"rtl8370_getAsicReg\000"
$LASF8:
	.ascii	"char\000"
$LASF11:
	.ascii	"regData\000"
$LASF19:
	.ascii	"bitsShift\000"
$LASF20:
	.ascii	"valueShifted\000"
$LASF17:
	.ascii	"rtl8370_setAsicRegBits\000"
$LASF24:
	.ascii	"GNU C 3.4.2 -g\000"
	.ident	"GCC: (GNU) 3.4.2"
