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
	.globl	rtl8370_setAsic1xPBEnConfig
	.ent	rtl8370_setAsic1xPBEnConfig
	.type	rtl8370_setAsic1xPBEnConfig, @function
rtl8370_setAsic1xPBEnConfig:
$LFB2:
	.file 1 "rtl8370_asicdrv_dot1x.c"
	.loc 1 33 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 34 0
	sltu	$2,$4,16
	.loc 1 33 0
	move	$6,$5
	.loc 1 37 0
	sltu	$7,$5,2
	.loc 1 35 0
	.loc 1 34 0
	beq	$2,$0,$L1
	li	$3,3			# 0x3

	.loc 1 40 0
	move	$5,$4
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)
	li	$4,2688			# 0xa80
	.loc 1 38 0
	.loc 1 37 0
	beq	$7,$0,$L1
	li	$3,21			# 0x15

	.loc 1 40 0
	jr	$25
	nop

$L1:
	.loc 1 41 0
	j	$31
	move	$2,$3

	.set	macro
	.set	reorder
$LFE2:
	.end	rtl8370_setAsic1xPBEnConfig
	.align	2
	.globl	rtl8370_getAsic1xPBEnConfig
	.ent	rtl8370_getAsic1xPBEnConfig
	.type	rtl8370_getAsic1xPBEnConfig, @function
rtl8370_getAsic1xPBEnConfig:
$LFB3:
	.loc 1 54 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 54 0
	move	$2,$4
	.loc 1 55 0
	sltu	$3,$2,16
	.loc 1 58 0
	move	$6,$5
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
	li	$4,2688			# 0xa80
	.loc 1 55 0
	beq	$3,$0,$L6
	move	$5,$2

	.loc 1 58 0
	jr	$25
	nop

$L6:
	.loc 1 59 0
	j	$31
	li	$2,3			# 0x3

	.set	macro
	.set	reorder
$LFE3:
	.end	rtl8370_getAsic1xPBEnConfig
	.align	2
	.globl	rtl8370_setAsic1xPBAuthConfig
	.ent	rtl8370_setAsic1xPBAuthConfig
	.type	rtl8370_setAsic1xPBAuthConfig, @function
rtl8370_setAsic1xPBAuthConfig:
$LFB4:
	.loc 1 72 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 73 0
	sltu	$2,$4,16
	.loc 1 72 0
	move	$6,$5
	.loc 1 75 0
	sltu	$7,$5,2
	.loc 1 74 0
	.loc 1 73 0
	beq	$2,$0,$L7
	li	$3,3			# 0x3

	.loc 1 78 0
	move	$5,$4
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)
	li	$4,2690			# 0xa82
	.loc 1 76 0
	.loc 1 75 0
	beq	$7,$0,$L7
	li	$3,21			# 0x15

	.loc 1 78 0
	jr	$25
	nop

$L7:
	.loc 1 79 0
	j	$31
	move	$2,$3

	.set	macro
	.set	reorder
$LFE4:
	.end	rtl8370_setAsic1xPBAuthConfig
	.align	2
	.globl	rtl8370_getAsic1xPBAuthConfig
	.ent	rtl8370_getAsic1xPBAuthConfig
	.type	rtl8370_getAsic1xPBAuthConfig, @function
rtl8370_getAsic1xPBAuthConfig:
$LFB5:
	.loc 1 93 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 93 0
	move	$2,$4
	.loc 1 94 0
	sltu	$3,$2,16
	.loc 1 97 0
	move	$6,$5
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
	li	$4,2690			# 0xa82
	.loc 1 94 0
	beq	$3,$0,$L12
	move	$5,$2

	.loc 1 97 0
	jr	$25
	nop

$L12:
	.loc 1 98 0
	j	$31
	li	$2,3			# 0x3

	.set	macro
	.set	reorder
$LFE5:
	.end	rtl8370_getAsic1xPBAuthConfig
	.align	2
	.globl	rtl8370_setAsic1xPBOpdirConfig
	.ent	rtl8370_setAsic1xPBOpdirConfig
	.type	rtl8370_setAsic1xPBOpdirConfig, @function
rtl8370_setAsic1xPBOpdirConfig:
$LFB6:
	.loc 1 112 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 113 0
	sltu	$2,$4,16
	.loc 1 112 0
	move	$6,$5
	.loc 1 115 0
	sltu	$7,$5,2
	.loc 1 114 0
	.loc 1 113 0
	beq	$2,$0,$L13
	li	$3,3			# 0x3

	.loc 1 118 0
	move	$5,$4
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)
	li	$4,2691			# 0xa83
	.loc 1 116 0
	.loc 1 115 0
	beq	$7,$0,$L13
	li	$3,21			# 0x15

	.loc 1 118 0
	jr	$25
	nop

$L13:
	.loc 1 119 0
	j	$31
	move	$2,$3

	.set	macro
	.set	reorder
$LFE6:
	.end	rtl8370_setAsic1xPBOpdirConfig
	.align	2
	.globl	rtl8370_getAsic1xPBOpdirConfig
	.ent	rtl8370_getAsic1xPBOpdirConfig
	.type	rtl8370_getAsic1xPBOpdirConfig, @function
rtl8370_getAsic1xPBOpdirConfig:
$LFB7:
	.loc 1 133 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 133 0
	move	$2,$4
	.loc 1 134 0
	sltu	$3,$2,16
	.loc 1 137 0
	move	$6,$5
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
	li	$4,2691			# 0xa83
	.loc 1 134 0
	beq	$3,$0,$L18
	move	$5,$2

	.loc 1 137 0
	jr	$25
	nop

$L18:
	.loc 1 138 0
	j	$31
	li	$2,3			# 0x3

	.set	macro
	.set	reorder
$LFE7:
	.end	rtl8370_getAsic1xPBOpdirConfig
	.align	2
	.globl	rtl8370_setAsic1xMBEnConfig
	.ent	rtl8370_setAsic1xMBEnConfig
	.type	rtl8370_setAsic1xMBEnConfig, @function
rtl8370_setAsic1xMBEnConfig:
$LFB8:
	.loc 1 152 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 153 0
	sltu	$2,$4,16
	.loc 1 152 0
	move	$6,$5
	.loc 1 155 0
	sltu	$7,$5,2
	.loc 1 154 0
	.loc 1 153 0
	beq	$2,$0,$L19
	li	$3,3			# 0x3

	.loc 1 158 0
	move	$5,$4
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)
	li	$4,2689			# 0xa81
	.loc 1 156 0
	.loc 1 155 0
	beq	$7,$0,$L19
	li	$3,21			# 0x15

	.loc 1 158 0
	jr	$25
	nop

$L19:
	.loc 1 159 0
	j	$31
	move	$2,$3

	.set	macro
	.set	reorder
$LFE8:
	.end	rtl8370_setAsic1xMBEnConfig
	.align	2
	.globl	rtl8370_getAsic1xMBEnConfig
	.ent	rtl8370_getAsic1xMBEnConfig
	.type	rtl8370_getAsic1xMBEnConfig, @function
rtl8370_getAsic1xMBEnConfig:
$LFB9:
	.loc 1 173 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 173 0
	move	$2,$4
	.loc 1 174 0
	sltu	$3,$2,16
	.loc 1 177 0
	move	$6,$5
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
	li	$4,2689			# 0xa81
	.loc 1 174 0
	beq	$3,$0,$L24
	move	$5,$2

	.loc 1 177 0
	jr	$25
	nop

$L24:
	.loc 1 178 0
	j	$31
	li	$2,3			# 0x3

	.set	macro
	.set	reorder
$LFE9:
	.end	rtl8370_getAsic1xMBEnConfig
	.align	2
	.globl	rtl8370_setAsic1xMBOpdirConfig
	.ent	rtl8370_setAsic1xMBOpdirConfig
	.type	rtl8370_setAsic1xMBOpdirConfig, @function
rtl8370_setAsic1xMBOpdirConfig:
$LFB10:
	.loc 1 189 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 189 0
	move	$2,$4
	.loc 1 190 0
	sltu	$2,$2,2
	.loc 1 193 0
	move	$6,$4
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)
	li	$4,2694			# 0xa86
	.loc 1 190 0
	beq	$2,$0,$L27
	li	$5,5			# 0x5

	.loc 1 193 0
	jr	$25
	nop

$L27:
	.loc 1 194 0
	j	$31
	li	$2,21			# 0x15

	.set	macro
	.set	reorder
$LFE10:
	.end	rtl8370_setAsic1xMBOpdirConfig
	.align	2
	.globl	rtl8370_getAsic1xMBOpdirConfig
	.ent	rtl8370_getAsic1xMBOpdirConfig
	.type	rtl8370_getAsic1xMBOpdirConfig, @function
rtl8370_getAsic1xMBOpdirConfig:
$LFB11:
	.loc 1 206 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 207 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
	move	$6,$4
	li	$5,5			# 0x5
	jr	$25
	li	$4,2694			# 0xa86

	.set	macro
	.set	reorder
$LFE11:
	.end	rtl8370_getAsic1xMBOpdirConfig
	.align	2
	.globl	rtl8370_setAsic1xProcConfig
	.ent	rtl8370_setAsic1xProcConfig
	.type	rtl8370_setAsic1xProcConfig, @function
rtl8370_setAsic1xProcConfig:
$LFB12:
	.loc 1 222 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 223 0
	sltu	$2,$4,16
	.loc 1 222 0
	move	$6,$5
	.loc 1 226 0
	sltu	$7,$5,3
	.loc 1 224 0
	.loc 1 223 0
	beq	$2,$0,$L29
	li	$3,3			# 0x3

	.loc 1 229 0
	andi	$2,$4,0x7
	sll	$2,$2,1
	li	$5,3			# 0x3
	srl	$4,$4,3
	sll	$5,$5,$2
	.loc 1 227 0
	li	$2,1114112			# 0x110000
	.loc 1 229 0
	lw	$25,%call16(rtl8370_setAsicRegBits)($28)
	addiu	$4,$4,2692
	.loc 1 227 0
	.loc 1 226 0
	beq	$7,$0,$L29
	ori	$3,$2,0x6

	.loc 1 229 0
	jr	$25
	nop

$L29:
	.loc 1 230 0
	j	$31
	move	$2,$3

	.set	macro
	.set	reorder
$LFE12:
	.end	rtl8370_setAsic1xProcConfig
	.align	2
	.globl	rtl8370_getAsic1xProcConfig
	.ent	rtl8370_getAsic1xProcConfig
	.type	rtl8370_getAsic1xProcConfig, @function
rtl8370_getAsic1xProcConfig:
$LFB13:
	.loc 1 243 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 243 0
	move	$2,$4
	.loc 1 247 0
	andi	$3,$4,0x7
	sll	$3,$3,1
	srl	$4,$4,3
	.loc 1 243 0
	move	$6,$5
	.loc 1 244 0
	sltu	$2,$2,16
	.loc 1 247 0
	li	$5,3			# 0x3
	lw	$25,%call16(rtl8370_getAsicRegBits)($28)
	addiu	$4,$4,2692
	.loc 1 244 0
	beq	$2,$0,$L34
	sll	$5,$5,$3

	.loc 1 247 0
	jr	$25
	nop

$L34:
	.loc 1 248 0
	j	$31
	li	$2,3			# 0x3

	.set	macro
	.set	reorder
$LFE13:
	.end	rtl8370_getAsic1xProcConfig
	.align	2
	.globl	rtl8370_setAsic1xGuestVidx
	.ent	rtl8370_setAsic1xGuestVidx
	.type	rtl8370_setAsic1xGuestVidx, @function
rtl8370_setAsic1xGuestVidx:
$LFB14:
	.loc 1 260 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 260 0
	move	$2,$4
	.loc 1 261 0
	sltu	$2,$2,32
	.loc 1 264 0
	move	$6,$4
	lw	$25,%call16(rtl8370_setAsicRegBits)($28)
	li	$4,2694			# 0xa86
	.loc 1 261 0
	beq	$2,$0,$L37
	li	$5,31			# 0x1f

	.loc 1 264 0
	jr	$25
	nop

$L37:
	.loc 1 265 0
	li	$2,1114112			# 0x110000
	j	$31
	ori	$2,$2,0x7

	.set	macro
	.set	reorder
$LFE14:
	.end	rtl8370_setAsic1xGuestVidx
	.align	2
	.globl	rtl8370_getAsic1xGuestVidx
	.ent	rtl8370_getAsic1xGuestVidx
	.type	rtl8370_getAsic1xGuestVidx, @function
rtl8370_getAsic1xGuestVidx:
$LFB15:
	.loc 1 277 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 278 0
	lw	$25,%call16(rtl8370_getAsicRegBits)($28)
	move	$6,$4
	li	$5,31			# 0x1f
	jr	$25
	li	$4,2694			# 0xa86

	.set	macro
	.set	reorder
$LFE15:
	.end	rtl8370_getAsic1xGuestVidx
	.align	2
	.globl	rtl8370_setAsic1xGVOpdir
	.ent	rtl8370_setAsic1xGVOpdir
	.type	rtl8370_setAsic1xGVOpdir, @function
rtl8370_setAsic1xGVOpdir:
$LFB16:
	.loc 1 291 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 291 0
	move	$2,$4
	.loc 1 292 0
	sltu	$2,$2,2
	.loc 1 295 0
	move	$6,$4
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)
	li	$4,2694			# 0xa86
	.loc 1 292 0
	beq	$2,$0,$L41
	li	$5,6			# 0x6

	.loc 1 295 0
	jr	$25
	nop

$L41:
	.loc 1 296 0
	li	$2,1114112			# 0x110000
	j	$31
	ori	$2,$2,0x8

	.set	macro
	.set	reorder
$LFE16:
	.end	rtl8370_setAsic1xGVOpdir
	.align	2
	.globl	rtl8370_getAsic1xGVOpdir
	.ent	rtl8370_getAsic1xGVOpdir
	.type	rtl8370_getAsic1xGVOpdir, @function
rtl8370_getAsic1xGVOpdir:
$LFB17:
	.loc 1 308 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 309 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
	move	$6,$4
	li	$5,6			# 0x6
	jr	$25
	li	$4,2694			# 0xa86

	.set	macro
	.set	reorder
$LFE17:
	.end	rtl8370_getAsic1xGVOpdir
	.align	2
	.globl	rtl8370_setAsic1xTrapPriority
	.ent	rtl8370_setAsic1xTrapPriority
	.type	rtl8370_setAsic1xTrapPriority, @function
rtl8370_setAsic1xTrapPriority:
$LFB18:
	.loc 1 322 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 322 0
	move	$2,$4
	.loc 1 323 0
	sltu	$2,$2,8
	.loc 1 326 0
	move	$6,$4
	lw	$25,%call16(rtl8370_setAsicRegBits)($28)
	li	$4,2177			# 0x881
	.loc 1 323 0
	beq	$2,$0,$L45
	li	$5,7			# 0x7

	.loc 1 326 0
	jr	$25
	nop

$L45:
	.loc 1 327 0
	li	$2,851968			# 0xd0000
	j	$31
	ori	$2,$2,0x2

	.set	macro
	.set	reorder
$LFE18:
	.end	rtl8370_setAsic1xTrapPriority
	.align	2
	.globl	rtl8370_getAsic1xTrapPriority
	.ent	rtl8370_getAsic1xTrapPriority
	.type	rtl8370_getAsic1xTrapPriority, @function
rtl8370_getAsic1xTrapPriority:
$LFB19:
	.loc 1 338 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 339 0
	lw	$25,%call16(rtl8370_getAsicRegBits)($28)
	move	$6,$4
	li	$5,7			# 0x7
	jr	$25
	li	$4,2177			# 0x881

	.set	macro
	.set	reorder
$LFE19:
	.end	rtl8370_getAsic1xTrapPriority
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
	.align	2
$LEFDE2:
$LSFDE4:
	.4byte	$LEFDE4-$LASFDE4
$LASFDE4:
	.4byte	$Lframe0
	.4byte	$LFB4
	.4byte	$LFE4-$LFB4
	.align	2
$LEFDE4:
$LSFDE6:
	.4byte	$LEFDE6-$LASFDE6
$LASFDE6:
	.4byte	$Lframe0
	.4byte	$LFB5
	.4byte	$LFE5-$LFB5
	.align	2
$LEFDE6:
$LSFDE8:
	.4byte	$LEFDE8-$LASFDE8
$LASFDE8:
	.4byte	$Lframe0
	.4byte	$LFB6
	.4byte	$LFE6-$LFB6
	.align	2
$LEFDE8:
$LSFDE10:
	.4byte	$LEFDE10-$LASFDE10
$LASFDE10:
	.4byte	$Lframe0
	.4byte	$LFB7
	.4byte	$LFE7-$LFB7
	.align	2
$LEFDE10:
$LSFDE12:
	.4byte	$LEFDE12-$LASFDE12
$LASFDE12:
	.4byte	$Lframe0
	.4byte	$LFB8
	.4byte	$LFE8-$LFB8
	.align	2
$LEFDE12:
$LSFDE14:
	.4byte	$LEFDE14-$LASFDE14
$LASFDE14:
	.4byte	$Lframe0
	.4byte	$LFB9
	.4byte	$LFE9-$LFB9
	.align	2
$LEFDE14:
$LSFDE16:
	.4byte	$LEFDE16-$LASFDE16
$LASFDE16:
	.4byte	$Lframe0
	.4byte	$LFB10
	.4byte	$LFE10-$LFB10
	.align	2
$LEFDE16:
$LSFDE18:
	.4byte	$LEFDE18-$LASFDE18
$LASFDE18:
	.4byte	$Lframe0
	.4byte	$LFB11
	.4byte	$LFE11-$LFB11
	.align	2
$LEFDE18:
$LSFDE20:
	.4byte	$LEFDE20-$LASFDE20
$LASFDE20:
	.4byte	$Lframe0
	.4byte	$LFB12
	.4byte	$LFE12-$LFB12
	.align	2
$LEFDE20:
$LSFDE22:
	.4byte	$LEFDE22-$LASFDE22
$LASFDE22:
	.4byte	$Lframe0
	.4byte	$LFB13
	.4byte	$LFE13-$LFB13
	.align	2
$LEFDE22:
$LSFDE24:
	.4byte	$LEFDE24-$LASFDE24
$LASFDE24:
	.4byte	$Lframe0
	.4byte	$LFB14
	.4byte	$LFE14-$LFB14
	.align	2
$LEFDE24:
$LSFDE26:
	.4byte	$LEFDE26-$LASFDE26
$LASFDE26:
	.4byte	$Lframe0
	.4byte	$LFB15
	.4byte	$LFE15-$LFB15
	.align	2
$LEFDE26:
$LSFDE28:
	.4byte	$LEFDE28-$LASFDE28
$LASFDE28:
	.4byte	$Lframe0
	.4byte	$LFB16
	.4byte	$LFE16-$LFB16
	.align	2
$LEFDE28:
$LSFDE30:
	.4byte	$LEFDE30-$LASFDE30
$LASFDE30:
	.4byte	$Lframe0
	.4byte	$LFB17
	.4byte	$LFE17-$LFB17
	.align	2
$LEFDE30:
$LSFDE32:
	.4byte	$LEFDE32-$LASFDE32
$LASFDE32:
	.4byte	$Lframe0
	.4byte	$LFB18
	.4byte	$LFE18-$LFB18
	.align	2
$LEFDE32:
$LSFDE34:
	.4byte	$LEFDE34-$LASFDE34
$LASFDE34:
	.4byte	$Lframe0
	.4byte	$LFB19
	.4byte	$LFE19-$LFB19
	.align	2
$LEFDE34:
	.align	0
	.file 2 "/home/IOData/BR6456IN/srcbase/boot/GPL_4100/include/realtek/rtk_types.h"
	.text
$Letext0:
	.section	.debug_info
	.4byte	0x43c
	.2byte	0x2
	.4byte	$Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	$Ldebug_line0
	.4byte	$Letext0
	.4byte	$Ltext0
	.4byte	$LASF34
	.byte	0x1
	.4byte	$LASF35
	.4byte	$LASF36
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
	.4byte	0xbf
	.byte	0x1
	.4byte	$LASF12
	.byte	0x1
	.byte	0x21
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB2
	.4byte	$LFE2
	.4byte	$LSFDE0
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF10
	.byte	0x1
	.byte	0x20
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x6
	.4byte	$LASF11
	.byte	0x1
	.byte	0x20
	.4byte	0x33
	.byte	0x1
	.byte	0x56
	.byte	0x0
	.uleb128 0x5
	.4byte	0xf9
	.byte	0x1
	.4byte	$LASF13
	.byte	0x1
	.byte	0x36
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB3
	.4byte	$LFE3
	.4byte	$LSFDE2
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF10
	.byte	0x1
	.byte	0x35
	.4byte	0x33
	.byte	0x1
	.byte	0x52
	.uleb128 0x6
	.4byte	$LASF11
	.byte	0x1
	.byte	0x35
	.4byte	0xf9
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0x7
	.byte	0x4
	.4byte	0x33
	.uleb128 0x5
	.4byte	0x139
	.byte	0x1
	.4byte	$LASF14
	.byte	0x1
	.byte	0x48
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB4
	.4byte	$LFE4
	.4byte	$LSFDE4
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF10
	.byte	0x1
	.byte	0x47
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x6
	.4byte	$LASF15
	.byte	0x1
	.byte	0x47
	.4byte	0x33
	.byte	0x1
	.byte	0x56
	.byte	0x0
	.uleb128 0x5
	.4byte	0x173
	.byte	0x1
	.4byte	$LASF16
	.byte	0x1
	.byte	0x5d
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB5
	.4byte	$LFE5
	.4byte	$LSFDE6
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF10
	.byte	0x1
	.byte	0x5c
	.4byte	0x33
	.byte	0x1
	.byte	0x52
	.uleb128 0x6
	.4byte	$LASF15
	.byte	0x1
	.byte	0x5c
	.4byte	0xf9
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0x5
	.4byte	0x1ad
	.byte	0x1
	.4byte	$LASF17
	.byte	0x1
	.byte	0x70
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB6
	.4byte	$LFE6
	.4byte	$LSFDE8
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF10
	.byte	0x1
	.byte	0x6f
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x6
	.4byte	$LASF18
	.byte	0x1
	.byte	0x6f
	.4byte	0x33
	.byte	0x1
	.byte	0x56
	.byte	0x0
	.uleb128 0x5
	.4byte	0x1e7
	.byte	0x1
	.4byte	$LASF19
	.byte	0x1
	.byte	0x85
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB7
	.4byte	$LFE7
	.4byte	$LSFDE10
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF10
	.byte	0x1
	.byte	0x84
	.4byte	0x33
	.byte	0x1
	.byte	0x52
	.uleb128 0x6
	.4byte	$LASF18
	.byte	0x1
	.byte	0x84
	.4byte	0xf9
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0x5
	.4byte	0x221
	.byte	0x1
	.4byte	$LASF20
	.byte	0x1
	.byte	0x98
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB8
	.4byte	$LFE8
	.4byte	$LSFDE12
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF10
	.byte	0x1
	.byte	0x97
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x6
	.4byte	$LASF11
	.byte	0x1
	.byte	0x97
	.4byte	0x33
	.byte	0x1
	.byte	0x56
	.byte	0x0
	.uleb128 0x5
	.4byte	0x25b
	.byte	0x1
	.4byte	$LASF21
	.byte	0x1
	.byte	0xad
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB9
	.4byte	$LFE9
	.4byte	$LSFDE14
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF10
	.byte	0x1
	.byte	0xac
	.4byte	0x33
	.byte	0x1
	.byte	0x52
	.uleb128 0x6
	.4byte	$LASF11
	.byte	0x1
	.byte	0xac
	.4byte	0xf9
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0x5
	.4byte	0x288
	.byte	0x1
	.4byte	$LASF22
	.byte	0x1
	.byte	0xbd
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB10
	.4byte	$LFE10
	.4byte	$LSFDE16
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF18
	.byte	0x1
	.byte	0xbc
	.4byte	0x33
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x5
	.4byte	0x2b5
	.byte	0x1
	.4byte	$LASF23
	.byte	0x1
	.byte	0xce
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB11
	.4byte	$LFE11
	.4byte	$LSFDE18
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF18
	.byte	0x1
	.byte	0xcd
	.4byte	0xf9
	.byte	0x1
	.byte	0x54
	.byte	0x0
	.uleb128 0x5
	.4byte	0x2ef
	.byte	0x1
	.4byte	$LASF24
	.byte	0x1
	.byte	0xde
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB12
	.4byte	$LFE12
	.4byte	$LSFDE20
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF10
	.byte	0x1
	.byte	0xdd
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x6
	.4byte	$LASF25
	.byte	0x1
	.byte	0xdd
	.4byte	0x33
	.byte	0x1
	.byte	0x56
	.byte	0x0
	.uleb128 0x5
	.4byte	0x329
	.byte	0x1
	.4byte	$LASF26
	.byte	0x1
	.byte	0xf3
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB13
	.4byte	$LFE13
	.4byte	$LSFDE22
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF10
	.byte	0x1
	.byte	0xf2
	.4byte	0x33
	.byte	0x1
	.byte	0x52
	.uleb128 0x6
	.4byte	$LASF25
	.byte	0x1
	.byte	0xf2
	.4byte	0xf9
	.byte	0x1
	.byte	0x56
	.byte	0x0
	.uleb128 0x8
	.4byte	0x358
	.byte	0x1
	.4byte	$LASF27
	.byte	0x1
	.2byte	0x104
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB14
	.4byte	$LFE14
	.4byte	$LSFDE24
	.byte	0x1
	.byte	0x6d
	.uleb128 0x9
	.4byte	$LASF28
	.byte	0x1
	.2byte	0x103
	.4byte	0x33
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x8
	.4byte	0x387
	.byte	0x1
	.4byte	$LASF29
	.byte	0x1
	.2byte	0x115
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB15
	.4byte	$LFE15
	.4byte	$LSFDE26
	.byte	0x1
	.byte	0x6d
	.uleb128 0x9
	.4byte	$LASF28
	.byte	0x1
	.2byte	0x114
	.4byte	0xf9
	.byte	0x1
	.byte	0x54
	.byte	0x0
	.uleb128 0x8
	.4byte	0x3b6
	.byte	0x1
	.4byte	$LASF30
	.byte	0x1
	.2byte	0x123
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB16
	.4byte	$LFE16
	.4byte	$LSFDE28
	.byte	0x1
	.byte	0x6d
	.uleb128 0x9
	.4byte	$LASF11
	.byte	0x1
	.2byte	0x122
	.4byte	0x33
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x8
	.4byte	0x3e5
	.byte	0x1
	.4byte	$LASF31
	.byte	0x1
	.2byte	0x134
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB17
	.4byte	$LFE17
	.4byte	$LSFDE30
	.byte	0x1
	.byte	0x6d
	.uleb128 0x9
	.4byte	$LASF11
	.byte	0x1
	.2byte	0x133
	.4byte	0xf9
	.byte	0x1
	.byte	0x54
	.byte	0x0
	.uleb128 0x8
	.4byte	0x414
	.byte	0x1
	.4byte	$LASF32
	.byte	0x1
	.2byte	0x142
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB18
	.4byte	$LFE18
	.4byte	$LSFDE32
	.byte	0x1
	.byte	0x6d
	.uleb128 0x9
	.4byte	$LASF33
	.byte	0x1
	.2byte	0x141
	.4byte	0x33
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0xa
	.byte	0x1
	.4byte	$LASF37
	.byte	0x1
	.2byte	0x152
	.byte	0x1
	.4byte	0x7a
	.4byte	$LFB19
	.4byte	$LFE19
	.4byte	$LSFDE34
	.byte	0x1
	.byte	0x6d
	.uleb128 0x9
	.4byte	$LASF33
	.byte	0x1
	.2byte	0x151
	.4byte	0xf9
	.byte	0x1
	.byte	0x54
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
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
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
	.4byte	0x25a
	.2byte	0x2
	.4byte	$Ldebug_info0
	.4byte	0x440
	.4byte	0x85
	.ascii	"rtl8370_setAsic1xPBEnConfig\000"
	.4byte	0xbf
	.ascii	"rtl8370_getAsic1xPBEnConfig\000"
	.4byte	0xff
	.ascii	"rtl8370_setAsic1xPBAuthConfig\000"
	.4byte	0x139
	.ascii	"rtl8370_getAsic1xPBAuthConfig\000"
	.4byte	0x173
	.ascii	"rtl8370_setAsic1xPBOpdirConfig\000"
	.4byte	0x1ad
	.ascii	"rtl8370_getAsic1xPBOpdirConfig\000"
	.4byte	0x1e7
	.ascii	"rtl8370_setAsic1xMBEnConfig\000"
	.4byte	0x221
	.ascii	"rtl8370_getAsic1xMBEnConfig\000"
	.4byte	0x25b
	.ascii	"rtl8370_setAsic1xMBOpdirConfig\000"
	.4byte	0x288
	.ascii	"rtl8370_getAsic1xMBOpdirConfig\000"
	.4byte	0x2b5
	.ascii	"rtl8370_setAsic1xProcConfig\000"
	.4byte	0x2ef
	.ascii	"rtl8370_getAsic1xProcConfig\000"
	.4byte	0x329
	.ascii	"rtl8370_setAsic1xGuestVidx\000"
	.4byte	0x358
	.ascii	"rtl8370_getAsic1xGuestVidx\000"
	.4byte	0x387
	.ascii	"rtl8370_setAsic1xGVOpdir\000"
	.4byte	0x3b6
	.ascii	"rtl8370_getAsic1xGVOpdir\000"
	.4byte	0x3e5
	.ascii	"rtl8370_setAsic1xTrapPriority\000"
	.4byte	0x414
	.ascii	"rtl8370_getAsic1xTrapPriority\000"
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
$LASF21:
	.ascii	"rtl8370_getAsic1xMBEnConfig\000"
$LASF9:
	.ascii	"ret_t\000"
$LASF3:
	.ascii	"uint32\000"
$LASF0:
	.ascii	"long long unsigned int\000"
$LASF1:
	.ascii	"long long int\000"
$LASF11:
	.ascii	"enabled\000"
$LASF4:
	.ascii	"int32\000"
$LASF36:
	.ascii	"/home/IOData/BR6456IN/srcbase/boot/realtek_8370\000"
$LASF10:
	.ascii	"port\000"
$LASF2:
	.ascii	"unsigned int\000"
$LASF5:
	.ascii	"short unsigned int\000"
$LASF22:
	.ascii	"rtl8370_setAsic1xMBOpdirConfig\000"
$LASF29:
	.ascii	"rtl8370_getAsic1xGuestVidx\000"
$LASF15:
	.ascii	"auth\000"
$LASF33:
	.ascii	"priority\000"
$LASF6:
	.ascii	"short int\000"
$LASF19:
	.ascii	"rtl8370_getAsic1xPBOpdirConfig\000"
$LASF24:
	.ascii	"rtl8370_setAsic1xProcConfig\000"
$LASF20:
	.ascii	"rtl8370_setAsic1xMBEnConfig\000"
$LASF12:
	.ascii	"rtl8370_setAsic1xPBEnConfig\000"
$LASF17:
	.ascii	"rtl8370_setAsic1xPBOpdirConfig\000"
$LASF30:
	.ascii	"rtl8370_setAsic1xGVOpdir\000"
$LASF18:
	.ascii	"opdir\000"
$LASF16:
	.ascii	"rtl8370_getAsic1xPBAuthConfig\000"
$LASF31:
	.ascii	"rtl8370_getAsic1xGVOpdir\000"
$LASF35:
	.ascii	"rtl8370_asicdrv_dot1x.c\000"
$LASF14:
	.ascii	"rtl8370_setAsic1xPBAuthConfig\000"
$LASF13:
	.ascii	"rtl8370_getAsic1xPBEnConfig\000"
$LASF8:
	.ascii	"char\000"
$LASF25:
	.ascii	"proc\000"
$LASF23:
	.ascii	"rtl8370_getAsic1xMBOpdirConfig\000"
$LASF26:
	.ascii	"rtl8370_getAsic1xProcConfig\000"
$LASF37:
	.ascii	"rtl8370_getAsic1xTrapPriority\000"
$LASF34:
	.ascii	"GNU C 3.4.2 -g\000"
$LASF27:
	.ascii	"rtl8370_setAsic1xGuestVidx\000"
$LASF28:
	.ascii	"index\000"
$LASF32:
	.ascii	"rtl8370_setAsic1xTrapPriority\000"
	.ident	"GCC: (GNU) 3.4.2"
