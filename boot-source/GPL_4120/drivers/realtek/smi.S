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
	.globl	_rtl865x_setGpioDataBit
	.ent	_rtl865x_setGpioDataBit
	.type	_rtl865x_setGpioDataBit, @function
_rtl865x_setGpioDataBit:
$LFB43:
	.file 1 "smi.c"
	.loc 1 41 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	.loc 1 43 0
	li	$2,-1342177280			# 0xffffffffb0000000
	ori	$7,$2,0x620
	.loc 1 45 0
	li	$2,1			# 0x1
	sll	$2,$2,$4
	.loc 1 41 0
	andi	$5,$5,0xffff
	.loc 1 43 0
	move	$3,$2
	.loc 1 45 0
	.loc 1 42 0
	beq	$5,$0,$L2
	nor	$4,$0,$2

	.loc 1 43 0
	lw	$2,0($7)
	or	$2,$2,$3
	sw	$2,0($7)
	j	$31
	nop

$L2:
	.loc 1 45 0
	lw	$2,0($7)
	and	$2,$2,$4
	sw	$2,0($7)
	j	$31
	nop

	.set	macro
	.set	reorder
$LFE43:
	.end	_rtl865x_setGpioDataBit
	.align	2
	.globl	_rtl865x_getGpioDataBit
	.ent	_rtl865x_getGpioDataBit
	.type	_rtl865x_getGpioDataBit, @function
_rtl865x_getGpioDataBit:
$LFB44:
	.loc 1 49 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	.loc 1 50 0
	li	$2,-1342177280			# 0xffffffffb0000000
	ori	$2,$2,0x620
	lw	$3,0($2)
	li	$2,1			# 0x1
	sll	$2,$2,$4
	and	$3,$3,$2
	.loc 1 51 0
	sltu	$3,$0,$3
	.loc 1 54 0
	j	$31
	sw	$3,0($5)

	.set	macro
	.set	reorder
$LFE44:
	.end	_rtl865x_getGpioDataBit
	.align	2
	.globl	_smi_start
	.ent	_smi_start
	.type	_smi_start, @function
_smi_start:
$LFB45:
	.loc 1 57 0
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 16, gp= 8
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-40
$LCFI0:
	sw	$31,32($sp)
$LCFI1:
	sw	$17,28($sp)
$LCFI2:
	sw	$16,24($sp)
$LCFI3:
	.cprestore	16
	.loc 1 58 0
	li	$5,-1342177280			# 0xffffffffb0000000
	ori	$5,$5,0x624
	lw	$2,0($5)
	li	$3,4194304			# 0x400000
	.loc 1 62 0
	lw	$16,%got(smi_SCK)($28)
	.loc 1 58 0
	or	$2,$2,$3
	sw	$2,0($5)
	.loc 1 59 0
	lw	$3,0($5)
	.loc 1 62 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	.loc 1 59 0
	li	$2,8388608			# 0x800000
	.loc 1 62 0
	lw	$4,0($16)
	.loc 1 59 0
	or	$3,$3,$2
	sw	$3,0($5)
	.loc 1 63 0
	lw	$17,%got(smi_SDA)($28)
	.loc 1 62 0
	jalr	$25
	move	$5,$0

	lw	$28,16($sp)
	.loc 1 63 0
	lw	$4,0($17)
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	jalr	$25
	li	$5,1			# 0x1

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB2:
	.loc 1 64 0
	addiu	$2,$2,-1
$L44:
	bgez	$2,$L44
	addiu	$2,$2,-1

$LBE2:
	.loc 1 67 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($16)
	jalr	$25
	li	$5,1			# 0x1

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB3:
	.loc 1 68 0
	addiu	$2,$2,-1
$L45:
	bgez	$2,$L45
	addiu	$2,$2,-1

$LBE3:
	.loc 1 69 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($16)
	jalr	$25
	move	$5,$0

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB4:
	.loc 1 70 0
	addiu	$2,$2,-1
$L46:
	bgez	$2,$L46
	addiu	$2,$2,-1

$LBE4:
	.loc 1 73 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($16)
	jalr	$25
	li	$5,1			# 0x1

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB5:
	.loc 1 74 0
	addiu	$2,$2,-1
$L47:
	bgez	$2,$L47
	addiu	$2,$2,-1

$LBE5:
	.loc 1 75 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($17)
	jalr	$25
	move	$5,$0

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB6:
	.loc 1 76 0
	addiu	$2,$2,-1
$L48:
	bgez	$2,$L48
	addiu	$2,$2,-1

$LBE6:
	.loc 1 77 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($16)
	jalr	$25
	move	$5,$0

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB7:
	.loc 1 78 0
	addiu	$2,$2,-1
$L49:
	bgez	$2,$L49
	addiu	$2,$2,-1

$LBE7:
	.loc 1 79 0
	lw	$4,0($17)
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$31,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	li	$5,1			# 0x1
	jr	$25
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
$LFE45:
	.end	_smi_start
	.align	2
	.globl	_smi_writeBit
	.ent	_smi_writeBit
	.type	_smi_writeBit, @function
_smi_writeBit:
$LFB46:
	.loc 1 82 0
	.frame	$sp,40,$31		# vars= 0, regs= 4/0, args= 16, gp= 8
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-40
$LCFI4:
	sw	$31,36($sp)
$LCFI5:
	sw	$18,32($sp)
$LCFI6:
	sw	$17,28($sp)
$LCFI7:
	sw	$16,24($sp)
$LCFI8:
	.cprestore	16
	.loc 1 82 0
	.loc 1 83 0
	beq	$5,$0,$L70
	andi	$18,$4,0xffff

$L68:
	li	$2,2999			# 0xbb7
$LBB8:
	.loc 1 84 0
	addiu	$2,$2,-1
$L78:
	bgez	$2,$L78
	addiu	$2,$2,-1

$LBE8:
	.loc 1 87 0
	addiu	$16,$5,-1
	sra	$2,$18,$16
	andi	$2,$2,0x1
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	beq	$2,$0,$L58
	lw	$4,%got(smi_SDA)($28)

	.loc 1 88 0
	lw	$4,0($4)
	b	$L77
	li	$5,1			# 0x1

$L58:
	.loc 1 90 0
	lw	$4,0($4)
	move	$5,$0
$L77:
	jalr	$25
	nop

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB9:
	.loc 1 91 0
	addiu	$2,$2,-1
$L79:
	bgez	$2,$L79
	addiu	$2,$2,-1

$LBE9:
	.loc 1 94 0
	lw	$17,%got(smi_SCK)($28)
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	li	$5,1			# 0x1
	jalr	$25
	lw	$4,0($17)

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB10:
	.loc 1 95 0
	addiu	$2,$2,-1
$L80:
	bgez	$2,$L80
	addiu	$2,$2,-1

$LBE10:
	.loc 1 96 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($17)
	jalr	$25
	move	$5,$0

	lw	$28,16($sp)
	.loc 1 83 0
	bne	$16,$0,$L68
	move	$5,$16

$L70:
	lw	$31,36($sp)
	lw	$18,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
$LFE46:
	.end	_smi_writeBit
	.align	2
	.globl	_smi_readBit
	.ent	_smi_readBit
	.type	_smi_readBit, @function
_smi_readBit:
$LFB47:
	.loc 1 100 0
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
	.loc 1 102 0
	li	$6,-1342177280			# 0xffffffffb0000000
	ori	$6,$6,0x624
	lw	$3,0($6)
	li	$2,-4259840			# 0xffffffffffbf0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	sw	$3,0($6)
	.loc 1 100 0
	move	$17,$5
	move	$16,$4
	.loc 1 103 0
	beq	$4,$0,$L95
	sw	$0,0($5)

$L93:
	li	$2,2999			# 0xbb7
$LBB11:
	.loc 1 104 0
	addiu	$2,$2,-1
$L100:
	bgez	$2,$L100
	addiu	$2,$2,-1

$LBE11:
	.loc 1 106 0
	lw	$18,%got(smi_SCK)($28)
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	li	$5,1			# 0x1
	jalr	$25
	lw	$4,0($18)

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB12:
	.loc 1 107 0
	addiu	$2,$2,-1
$L101:
	bgez	$2,$L101
	addiu	$2,$2,-1

$LBE12:
	.loc 1 108 0
	lw	$2,%got(smi_SDA)($28)
	lw	$25,%call16(_rtl865x_getGpioDataBit)($28)
	addiu	$5,$sp,24
	jalr	$25
	lw	$4,0($2)

	lw	$28,16($sp)
	.loc 1 109 0
	lw	$4,0($18)
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	jalr	$25
	move	$5,$0

	.loc 1 111 0
	lw	$2,24($sp)
	lw	$3,0($17)
	addiu	$4,$16,-1
	sll	$2,$2,$4
	or	$3,$3,$2
	.loc 1 109 0
	lw	$28,16($sp)
	.loc 1 111 0
	sw	$3,0($17)
	.loc 1 103 0
	bne	$4,$0,$L93
	move	$16,$4

$L95:
	.loc 1 113 0
	li	$2,-1342177280			# 0xffffffffb0000000
	ori	$2,$2,0x624
	lw	$3,0($2)
	lw	$31,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	li	$4,4194304			# 0x400000
	or	$3,$3,$4
	addiu	$sp,$sp,48
	sw	$3,0($2)
	j	$31
	nop

	.set	macro
	.set	reorder
$LFE47:
	.end	_smi_readBit
	.align	2
	.globl	_smi_stop
	.ent	_smi_stop
	.type	_smi_stop, @function
_smi_stop:
$LFB48:
	.loc 1 116 0
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 16, gp= 8
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-40
$LCFI14:
	sw	$31,32($sp)
$LCFI15:
	sw	$17,28($sp)
$LCFI16:
	sw	$16,24($sp)
$LCFI17:
	.cprestore	16
	li	$2,2999			# 0xbb7
$LBB13:
	.loc 1 117 0
	addiu	$2,$2,-1
$L145:
	bgez	$2,$L145
	addiu	$2,$2,-1

$LBE13:
	.loc 1 118 0
	lw	$17,%got(smi_SDA)($28)
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	move	$5,$0
	.loc 1 119 0
	lw	$16,%got(smi_SCK)($28)
	.loc 1 118 0
	jalr	$25
	lw	$4,0($17)

	lw	$28,16($sp)
	.loc 1 119 0
	lw	$4,0($16)
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	jalr	$25
	li	$5,1			# 0x1

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB14:
	.loc 1 120 0
	addiu	$2,$2,-1
$L146:
	bgez	$2,$L146
	addiu	$2,$2,-1

$LBE14:
	.loc 1 121 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($17)
	jalr	$25
	li	$5,1			# 0x1

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB15:
	.loc 1 122 0
	addiu	$2,$2,-1
$L147:
	bgez	$2,$L147
	addiu	$2,$2,-1

$LBE15:
	.loc 1 123 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($16)
	jalr	$25
	li	$5,1			# 0x1

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB16:
	.loc 1 124 0
	addiu	$2,$2,-1
$L148:
	bgez	$2,$L148
	addiu	$2,$2,-1

$LBE16:
	.loc 1 125 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($16)
	jalr	$25
	move	$5,$0

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB17:
	.loc 1 126 0
	addiu	$2,$2,-1
$L149:
	bgez	$2,$L149
	addiu	$2,$2,-1

$LBE17:
	.loc 1 127 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($16)
	jalr	$25
	li	$5,1			# 0x1

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB18:
	.loc 1 130 0
	addiu	$2,$2,-1
$L150:
	bgez	$2,$L150
	addiu	$2,$2,-1

$LBE18:
	.loc 1 131 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($16)
	jalr	$25
	move	$5,$0

	lw	$28,16($sp)
	li	$2,2999			# 0xbb7
$LBB19:
	.loc 1 132 0
	addiu	$2,$2,-1
$L151:
	bgez	$2,$L151
	addiu	$2,$2,-1

$LBE19:
	.loc 1 133 0
	lw	$4,0($16)
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	jalr	$25
	li	$5,1			# 0x1

	.loc 1 135 0
	li	$4,-1342177280			# 0xffffffffb0000000
	ori	$4,$4,0x624
	lw	$3,0($4)
	li	$2,-4259840			# 0xffffffffffbf0000
	ori	$2,$2,0xffff
	and	$3,$3,$2
	sw	$3,0($4)
	.loc 1 136 0
	lw	$3,0($4)
	.loc 1 133 0
	lw	$28,16($sp)
	.loc 1 136 0
	li	$2,-8454144			# 0xffffffffff7f0000
	lw	$31,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	ori	$2,$2,0xffff
	and	$3,$3,$2
	addiu	$sp,$sp,40
	sw	$3,0($4)
	j	$31
	nop

	.set	macro
	.set	reorder
$LFE48:
	.end	_smi_stop
	.align	2
	.globl	smi_reset
	.ent	smi_reset
	.type	smi_reset, @function
smi_reset:
$LFB49:
	.loc 1 141 0
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 16, gp= 8
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-40
$LCFI18:
	sw	$31,32($sp)
$LCFI19:
	sw	$17,28($sp)
$LCFI20:
	sw	$16,24($sp)
$LCFI21:
	.cprestore	16
	.loc 1 144 0
	li	$3,-1342177280			# 0xffffffffb0000000
	ori	$3,$3,0x624
	.loc 1 145 0
	lw	$17,%got(smi_RST)($28)
	.loc 1 144 0
	lw	$2,0($3)
	.loc 1 145 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($17)
	.loc 1 144 0
	ori	$2,$2,0x1
	sw	$2,0($3)
	.loc 1 145 0
	jalr	$25
	li	$5,1			# 0x1

	li	$2,19988480			# 0x1310000
	lw	$28,16($sp)
	ori	$2,$2,0x2cff
	addiu	$16,$2,1
$LBB20:
	.loc 1 146 0
	addiu	$16,$16,-1
$L171:
	bne	$16,$0,$L171
	addiu	$16,$16,-1

$LBE20:
	.loc 1 147 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($17)
	move	$5,$0
	jalr	$25
	addiu	$16,$2,1

	lw	$28,16($sp)
$LBB21:
	.loc 1 148 0
	addiu	$16,$16,-1
$L172:
	bne	$16,$0,$L172
	addiu	$16,$16,-1

$LBE21:
	.loc 1 149 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	lw	$4,0($17)
	li	$2,19988480			# 0x1310000
	ori	$2,$2,0x2cff
	li	$5,1			# 0x1
	jalr	$25
	addiu	$16,$2,1

	lw	$28,16($sp)
	move	$2,$16
$L164:
$LBB22:
	.loc 1 150 0
	addiu	$2,$2,-1
	.loc 1 152 0
	bne	$2,$0,$L164
	lw	$31,32($sp)

$LBE22:
	lw	$17,28($sp)
	lw	$16,24($sp)
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
$LFE49:
	.end	smi_reset
	.align	2
	.globl	smi_init
	.ent	smi_init
	.type	smi_init, @function
smi_init:
$LFB50:
	.loc 1 155 0
	.frame	$sp,32,$31		# vars= 0, regs= 2/0, args= 16, gp= 8
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-32
$LCFI22:
	sw	$31,28($sp)
$LCFI23:
	sw	$16,24($sp)
$LCFI24:
	.cprestore	16
	.loc 1 157 0
	li	$4,-1342177280			# 0xffffffffb0000000
	ori	$5,$4,0x60
	lw	$2,0($5)
	.loc 1 158 0
	li	$3,-129			# 0xffffffffffffff7f
	.loc 1 160 0
	ori	$4,$4,0x624
	.loc 1 157 0
	ori	$2,$2,0x100
	sw	$2,0($5)
	.loc 1 158 0
	lw	$2,0($5)
	.loc 1 163 0
	lw	$16,%got(smi_SCK)($28)
	.loc 1 168 0
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	.loc 1 158 0
	and	$2,$2,$3
	sw	$2,0($5)
	.loc 1 160 0
	lw	$2,0($4)
	li	$3,8388608			# 0x800000
	.loc 1 168 0
	li	$5,1			# 0x1
	.loc 1 160 0
	or	$2,$2,$3
	sw	$2,0($4)
	.loc 1 161 0
	lw	$2,0($4)
	li	$3,4194304			# 0x400000
	or	$2,$2,$3
	sw	$2,0($4)
	.loc 1 164 0
	lw	$2,%got(smi_SDA)($28)
	li	$3,22			# 0x16
	.loc 1 168 0
	li	$4,22			# 0x16
	.loc 1 164 0
	sw	$3,0($2)
	.loc 1 163 0
	li	$2,23			# 0x17
	sw	$2,0($16)
	.loc 1 165 0
	lw	$2,%got(smi_RST)($28)
	.loc 1 168 0
	jalr	$25
	sw	$0,0($2)

	lw	$28,16($sp)
	.loc 1 169 0
	lw	$4,0($16)
	lw	$25,%call16(_rtl865x_setGpioDataBit)($28)
	jalr	$25
	li	$5,1			# 0x1

	lw	$28,16($sp)
	.loc 1 171 0
	lw	$31,28($sp)
	lw	$16,24($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
$LFE50:
	.end	smi_init
	.align	2
	.globl	smi_read
	.ent	smi_read
	.type	smi_read, @function
smi_read:
$LFB51:
	.loc 1 173 0
	.frame	$sp,56,$31		# vars= 8, regs= 5/0, args= 16, gp= 8
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-56
$LCFI25:
	sw	$31,48($sp)
$LCFI26:
	sw	$19,44($sp)
$LCFI27:
	sw	$18,40($sp)
$LCFI28:
	sw	$17,36($sp)
$LCFI29:
	sw	$16,32($sp)
$LCFI30:
	.cprestore	16
	.loc 1 200 0
	lw	$25,%call16(_smi_start)($28)
	.loc 1 190 0
	sw	$0,28($sp)
	.loc 1 173 0
	move	$19,$4
	.loc 1 200 0
	jalr	$25
	move	$18,$5

	lw	$28,16($sp)
	.loc 1 202 0
	li	$4,11			# 0xb
	li	$5,4			# 0x4
	lw	$25,%call16(_smi_writeBit)($28)
	.loc 1 192 0
	move	$17,$0
	.loc 1 208 0
	.loc 1 202 0
	jalr	$25
	move	$16,$0

	lw	$28,16($sp)
	.loc 1 204 0
	li	$4,4			# 0x4
	lw	$25,%call16(_smi_writeBit)($28)
	jalr	$25
	li	$5,3			# 0x3

	lw	$28,16($sp)
	.loc 1 206 0
	li	$4,1			# 0x1
	lw	$25,%call16(_smi_writeBit)($28)
	jalr	$25
	li	$5,1			# 0x1

	lw	$28,16($sp)
	.loc 1 211 0
	lw	$25,%call16(_smi_readBit)($28)
$L187:
	.loc 1 210 0
	addiu	$2,$16,1
	.loc 1 211 0
	li	$4,1			# 0x1
	addiu	$5,$sp,24
	.loc 1 210 0
	.loc 1 211 0
	jalr	$25
	andi	$16,$2,0x00ff

	.loc 1 212 0
	lw	$2,24($sp)
	.loc 1 211 0
	lw	$28,16($sp)
	.loc 1 212 0
	beq	$2,$0,$L178
	sltu	$3,$16,5

	.loc 1 211 0
	.loc 1 212 0
	bne	$3,$0,$L187
	lw	$25,%call16(_smi_readBit)($28)

	.loc 1 215 0
	li	$17,-1			# 0xffffffffffffffff
$L178:
	.loc 1 217 0
	lw	$25,%call16(_smi_writeBit)($28)
	andi	$4,$19,0x00ff
	jalr	$25
	li	$5,8			# 0x8

	lw	$28,16($sp)
	.loc 1 219 0
	move	$16,$0
	.loc 1 222 0
	lw	$25,%call16(_smi_readBit)($28)
$L188:
	.loc 1 221 0
	addiu	$2,$16,1
	.loc 1 222 0
	li	$4,1			# 0x1
	addiu	$5,$sp,24
	.loc 1 221 0
	.loc 1 222 0
	jalr	$25
	andi	$16,$2,0x00ff

	.loc 1 223 0
	lw	$2,24($sp)
	.loc 1 222 0
	lw	$28,16($sp)
	.loc 1 223 0
	beq	$2,$0,$L182
	sltu	$3,$16,5

	.loc 1 222 0
	.loc 1 223 0
	bne	$3,$0,$L188
	lw	$25,%call16(_smi_readBit)($28)

	.loc 1 226 0
	li	$17,-1			# 0xffffffffffffffff
$L182:
	.loc 1 228 0
	lw	$25,%call16(_smi_writeBit)($28)
	srl	$4,$19,8
	andi	$4,$4,0xffff
	jalr	$25
	li	$5,8			# 0x8

	lw	$28,16($sp)
	.loc 1 230 0
	move	$16,$0
	.loc 1 233 0
	lw	$25,%call16(_smi_readBit)($28)
$L189:
	.loc 1 232 0
	addiu	$2,$16,1
	.loc 1 233 0
	li	$4,1			# 0x1
	addiu	$5,$sp,24
	.loc 1 232 0
	.loc 1 233 0
	jalr	$25
	andi	$16,$2,0x00ff

	.loc 1 234 0
	lw	$2,24($sp)
	.loc 1 233 0
	lw	$28,16($sp)
	.loc 1 234 0
	beq	$2,$0,$L186
	sltu	$3,$16,5

	.loc 1 233 0
	.loc 1 234 0
	bne	$3,$0,$L189
	lw	$25,%call16(_smi_readBit)($28)

	.loc 1 236 0
	li	$17,-1			# 0xffffffffffffffff
$L186:
	.loc 1 238 0
	lw	$25,%call16(_smi_readBit)($28)
	addiu	$5,$sp,28
	jalr	$25
	li	$4,8			# 0x8

	lw	$28,16($sp)
	.loc 1 239 0
	lbu	$2,28($sp)
	.loc 1 241 0
	move	$4,$0
	lw	$25,%call16(_smi_writeBit)($28)
	.loc 1 239 0
	sw	$2,0($18)
	.loc 1 241 0
	jalr	$25
	li	$5,1			# 0x1

	lw	$28,16($sp)
	.loc 1 243 0
	addiu	$5,$sp,28
	lw	$25,%call16(_smi_readBit)($28)
	jalr	$25
	li	$4,8			# 0x8

	lw	$28,16($sp)
	.loc 1 245 0
	li	$4,1			# 0x1
	lw	$25,%call16(_smi_writeBit)($28)
	jalr	$25
	li	$5,1			# 0x1

	lw	$28,16($sp)
	.loc 1 246 0
	lw	$2,28($sp)
	lw	$3,0($18)
	.loc 1 248 0
	lw	$25,%call16(_smi_stop)($28)
	.loc 1 246 0
	sll	$2,$2,8
	or	$3,$3,$2
	.loc 1 248 0
	jalr	$25
	sw	$3,0($18)

	.loc 1 254 0
	move	$2,$17
	.loc 1 248 0
	lw	$28,16($sp)
	.loc 1 254 0
	lw	$31,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
$LFE51:
	.end	smi_read
	.align	2
	.globl	smi_write
	.ent	smi_write
	.type	smi_write, @function
smi_write:
$LFB52:
	.loc 1 256 0
	.frame	$sp,56,$31		# vars= 8, regs= 5/0, args= 16, gp= 8
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-56
$LCFI31:
	sw	$31,48($sp)
$LCFI32:
	sw	$19,44($sp)
$LCFI33:
	sw	$18,40($sp)
$LCFI34:
	sw	$17,36($sp)
$LCFI35:
	sw	$16,32($sp)
$LCFI36:
	.cprestore	16
	.loc 1 284 0
	lw	$25,%call16(_smi_start)($28)
	.loc 1 256 0
	move	$18,$4
	.loc 1 284 0
	jalr	$25
	move	$19,$5

	lw	$28,16($sp)
	.loc 1 286 0
	li	$4,11			# 0xb
	li	$5,4			# 0x4
	lw	$25,%call16(_smi_writeBit)($28)
	.loc 1 279 0
	move	$17,$0
	.loc 1 292 0
	.loc 1 286 0
	jalr	$25
	move	$16,$0

	lw	$28,16($sp)
	.loc 1 288 0
	li	$4,4			# 0x4
	lw	$25,%call16(_smi_writeBit)($28)
	jalr	$25
	li	$5,3			# 0x3

	lw	$28,16($sp)
	.loc 1 290 0
	move	$4,$0
	lw	$25,%call16(_smi_writeBit)($28)
	jalr	$25
	li	$5,1			# 0x1

	lw	$28,16($sp)
	.loc 1 295 0
	lw	$25,%call16(_smi_readBit)($28)
$L211:
	.loc 1 294 0
	addiu	$2,$16,1
	.loc 1 295 0
	li	$4,1			# 0x1
	addiu	$5,$sp,24
	.loc 1 294 0
	.loc 1 295 0
	jalr	$25
	sll	$16,$2,24

	.loc 1 296 0
	lw	$2,24($sp)
	.loc 1 294 0
	sra	$16,$16,24
	.loc 1 295 0
	lw	$28,16($sp)
	.loc 1 296 0
	beq	$2,$0,$L194
	slt	$3,$16,5

	.loc 1 295 0
	.loc 1 296 0
	bne	$3,$0,$L211
	lw	$25,%call16(_smi_readBit)($28)

	.loc 1 299 0
	b	$L190
	li	$2,-1			# 0xffffffffffffffff

$L194:
	.loc 1 301 0
	lw	$25,%call16(_smi_writeBit)($28)
	andi	$4,$18,0x00ff
	jalr	$25
	li	$5,8			# 0x8

	lw	$28,16($sp)
	.loc 1 303 0
	move	$16,$0
	.loc 1 306 0
	lw	$25,%call16(_smi_readBit)($28)
$L212:
	.loc 1 305 0
	addiu	$2,$16,1
	.loc 1 306 0
	li	$4,1			# 0x1
	addiu	$5,$sp,24
	.loc 1 305 0
	.loc 1 306 0
	jalr	$25
	sll	$16,$2,24

	.loc 1 307 0
	lw	$2,24($sp)
	.loc 1 305 0
	sra	$16,$16,24
	.loc 1 306 0
	lw	$28,16($sp)
	.loc 1 307 0
	beq	$2,$0,$L198
	slt	$3,$16,5

	.loc 1 306 0
	.loc 1 307 0
	bne	$3,$0,$L212
	lw	$25,%call16(_smi_readBit)($28)

	.loc 1 309 0
	li	$17,-1			# 0xffffffffffffffff
$L198:
	.loc 1 311 0
	lw	$25,%call16(_smi_writeBit)($28)
	srl	$4,$18,8
	andi	$4,$4,0xffff
	jalr	$25
	li	$5,8			# 0x8

	lw	$28,16($sp)
	.loc 1 313 0
	move	$16,$0
	.loc 1 316 0
	lw	$25,%call16(_smi_readBit)($28)
$L213:
	.loc 1 315 0
	addiu	$2,$16,1
	.loc 1 316 0
	li	$4,1			# 0x1
	addiu	$5,$sp,24
	.loc 1 315 0
	.loc 1 316 0
	jalr	$25
	sll	$16,$2,24

	.loc 1 317 0
	lw	$2,24($sp)
	.loc 1 315 0
	sra	$16,$16,24
	.loc 1 316 0
	lw	$28,16($sp)
	.loc 1 317 0
	beq	$2,$0,$L202
	slt	$3,$16,5

	.loc 1 316 0
	.loc 1 317 0
	bne	$3,$0,$L213
	lw	$25,%call16(_smi_readBit)($28)

	.loc 1 319 0
	li	$17,-1			# 0xffffffffffffffff
$L202:
	.loc 1 321 0
	lw	$25,%call16(_smi_writeBit)($28)
	andi	$4,$19,0x00ff
	jalr	$25
	li	$5,8			# 0x8

	lw	$28,16($sp)
	.loc 1 323 0
	move	$16,$0
	.loc 1 326 0
	lw	$25,%call16(_smi_readBit)($28)
$L214:
	.loc 1 325 0
	addiu	$2,$16,1
	.loc 1 326 0
	li	$4,1			# 0x1
	addiu	$5,$sp,24
	.loc 1 325 0
	.loc 1 326 0
	jalr	$25
	sll	$16,$2,24

	.loc 1 327 0
	lw	$2,24($sp)
	.loc 1 325 0
	sra	$16,$16,24
	.loc 1 326 0
	lw	$28,16($sp)
	.loc 1 327 0
	beq	$2,$0,$L206
	slt	$3,$16,5

	.loc 1 326 0
	.loc 1 327 0
	bne	$3,$0,$L214
	lw	$25,%call16(_smi_readBit)($28)

	.loc 1 329 0
	li	$17,-1			# 0xffffffffffffffff
$L206:
	.loc 1 331 0
	lw	$25,%call16(_smi_writeBit)($28)
	srl	$4,$19,8
	andi	$4,$4,0xffff
	jalr	$25
	li	$5,8			# 0x8

	lw	$28,16($sp)
	.loc 1 333 0
	move	$16,$0
	.loc 1 336 0
	lw	$25,%call16(_smi_readBit)($28)
$L215:
	.loc 1 335 0
	addiu	$2,$16,1
	.loc 1 336 0
	li	$4,1			# 0x1
	addiu	$5,$sp,24
	.loc 1 335 0
	.loc 1 336 0
	jalr	$25
	sll	$16,$2,24

	.loc 1 337 0
	lw	$2,24($sp)
	.loc 1 335 0
	sra	$16,$16,24
	.loc 1 336 0
	lw	$28,16($sp)
	.loc 1 337 0
	beq	$2,$0,$L210
	slt	$3,$16,5

	.loc 1 336 0
	.loc 1 337 0
	bne	$3,$0,$L215
	lw	$25,%call16(_smi_readBit)($28)

	.loc 1 339 0
	li	$17,-1			# 0xffffffffffffffff
$L210:
	.loc 1 341 0
	lw	$25,%call16(_smi_stop)($28)
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 345 0
	move	$2,$17
$L190:
	.loc 1 347 0
	lw	$31,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
$LFE52:
	.end	smi_write

	.comm	smi_SCK,4,4

	.comm	smi_SDA,4,4

	.comm	smi_RST,4,4
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
	.4byte	$LFB43
	.4byte	$LFE43-$LFB43
	.align	2
$LEFDE0:
$LSFDE2:
	.4byte	$LEFDE2-$LASFDE2
$LASFDE2:
	.4byte	$Lframe0
	.4byte	$LFB44
	.4byte	$LFE44-$LFB44
	.align	2
$LEFDE2:
$LSFDE4:
	.4byte	$LEFDE4-$LASFDE4
$LASFDE4:
	.4byte	$Lframe0
	.4byte	$LFB45
	.4byte	$LFE45-$LFB45
	.byte	0x4
	.4byte	$LCFI0-$LFB45
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	$LCFI3-$LCFI0
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
$LEFDE4:
$LSFDE6:
	.4byte	$LEFDE6-$LASFDE6
$LASFDE6:
	.4byte	$Lframe0
	.4byte	$LFB46
	.4byte	$LFE46-$LFB46
	.byte	0x4
	.4byte	$LCFI4-$LFB46
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	$LCFI8-$LCFI4
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
	.4byte	$LFB47
	.4byte	$LFE47-$LFB47
	.byte	0x4
	.4byte	$LCFI9-$LFB47
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
$LEFDE8:
$LSFDE10:
	.4byte	$LEFDE10-$LASFDE10
$LASFDE10:
	.4byte	$Lframe0
	.4byte	$LFB48
	.4byte	$LFE48-$LFB48
	.byte	0x4
	.4byte	$LCFI14-$LFB48
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	$LCFI17-$LCFI14
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
$LEFDE10:
$LSFDE12:
	.4byte	$LEFDE12-$LASFDE12
$LASFDE12:
	.4byte	$Lframe0
	.4byte	$LFB49
	.4byte	$LFE49-$LFB49
	.byte	0x4
	.4byte	$LCFI18-$LFB49
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	$LCFI21-$LCFI18
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
$LEFDE12:
$LSFDE14:
	.4byte	$LEFDE14-$LASFDE14
$LASFDE14:
	.4byte	$Lframe0
	.4byte	$LFB50
	.4byte	$LFE50-$LFB50
	.byte	0x4
	.4byte	$LCFI22-$LFB50
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	$LCFI24-$LCFI22
	.byte	0x11
	.uleb128 0x10
	.sleb128 -2
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -1
	.align	2
$LEFDE14:
$LSFDE16:
	.4byte	$LEFDE16-$LASFDE16
$LASFDE16:
	.4byte	$Lframe0
	.4byte	$LFB51
	.4byte	$LFE51-$LFB51
	.byte	0x4
	.4byte	$LCFI25-$LFB51
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.4byte	$LCFI30-$LCFI25
	.byte	0x11
	.uleb128 0x10
	.sleb128 -6
	.byte	0x11
	.uleb128 0x11
	.sleb128 -5
	.byte	0x11
	.uleb128 0x12
	.sleb128 -4
	.byte	0x11
	.uleb128 0x13
	.sleb128 -3
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -2
	.align	2
$LEFDE16:
$LSFDE18:
	.4byte	$LEFDE18-$LASFDE18
$LASFDE18:
	.4byte	$Lframe0
	.4byte	$LFB52
	.4byte	$LFE52-$LFB52
	.byte	0x4
	.4byte	$LCFI31-$LFB52
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.4byte	$LCFI36-$LCFI31
	.byte	0x11
	.uleb128 0x10
	.sleb128 -6
	.byte	0x11
	.uleb128 0x11
	.sleb128 -5
	.byte	0x11
	.uleb128 0x12
	.sleb128 -4
	.byte	0x11
	.uleb128 0x13
	.sleb128 -3
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -2
	.align	2
$LEFDE18:
	.align	0
	.file 2 "/home/IOData/BR6456IN/srcbase/boot/GPL_4100/include/realtek/rtk_types.h"
	.text
$Letext0:
	.section	.debug_info
	.4byte	0x523
	.2byte	0x2
	.4byte	$Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	$Ldebug_line0
	.4byte	$Letext0
	.4byte	$Ltext0
	.4byte	$LASF35
	.byte	0x1
	.4byte	$LASF36
	.4byte	$LASF37
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
	.uleb128 0x3
	.4byte	$LASF5
	.byte	0x2
	.byte	0x12
	.4byte	0x62
	.uleb128 0x2
	.4byte	$LASF6
	.byte	0x2
	.byte	0x7
	.uleb128 0x2
	.4byte	$LASF7
	.byte	0x2
	.byte	0x5
	.uleb128 0x3
	.4byte	$LASF8
	.byte	0x2
	.byte	0x14
	.4byte	0x7b
	.uleb128 0x2
	.4byte	$LASF9
	.byte	0x1
	.byte	0x8
	.uleb128 0x3
	.4byte	$LASF10
	.byte	0x2
	.byte	0x15
	.4byte	0x8d
	.uleb128 0x2
	.4byte	$LASF11
	.byte	0x1
	.byte	0x6
	.uleb128 0x2
	.4byte	$LASF2
	.byte	0x4
	.byte	0x7
	.uleb128 0x2
	.4byte	$LASF12
	.byte	0x4
	.byte	0x7
	.uleb128 0x2
	.4byte	$LASF13
	.byte	0x4
	.byte	0x5
	.uleb128 0x2
	.4byte	$LASF14
	.byte	0x1
	.byte	0x6
	.uleb128 0x5
	.4byte	0xe6
	.byte	0x1
	.4byte	$LASF17
	.byte	0x1
	.byte	0x29
	.byte	0x1
	.4byte	$LFB43
	.4byte	$LFE43
	.4byte	$LSFDE0
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF15
	.byte	0x1
	.byte	0x29
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x6
	.4byte	$LASF16
	.byte	0x1
	.byte	0x29
	.4byte	0x57
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0x5
	.4byte	0x11a
	.byte	0x1
	.4byte	$LASF18
	.byte	0x1
	.byte	0x31
	.byte	0x1
	.4byte	$LFB44
	.4byte	$LFE44
	.4byte	$LSFDE2
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF15
	.byte	0x1
	.byte	0x31
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x7
	.ascii	"u\000"
	.byte	0x1
	.byte	0x31
	.4byte	0x11a
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0x8
	.byte	0x4
	.4byte	0x33
	.uleb128 0x5
	.4byte	0x1ce
	.byte	0x1
	.4byte	$LASF19
	.byte	0x1
	.byte	0x39
	.byte	0x1
	.4byte	$LFB45
	.4byte	$LFE45
	.4byte	$LSFDE4
	.byte	0x1
	.byte	0x6d
	.uleb128 0x9
	.4byte	0x154
	.4byte	$LBB2
	.4byte	$LBE2
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x40
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x9
	.4byte	0x16d
	.4byte	$LBB3
	.4byte	$LBE3
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x44
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x9
	.4byte	0x186
	.4byte	$LBB4
	.4byte	$LBE4
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x46
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x9
	.4byte	0x19f
	.4byte	$LBB5
	.4byte	$LBE5
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x4a
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x9
	.4byte	0x1b8
	.4byte	$LBB6
	.4byte	$LBE6
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x4c
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0xb
	.4byte	$LBB7
	.4byte	$LBE7
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x4e
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.4byte	0x24b
	.byte	0x1
	.4byte	$LASF20
	.byte	0x1
	.byte	0x52
	.byte	0x1
	.4byte	$LFB46
	.4byte	$LFE46
	.4byte	$LSFDE6
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF21
	.byte	0x1
	.byte	0x52
	.4byte	0x57
	.byte	0x1
	.byte	0x62
	.uleb128 0x6
	.4byte	$LASF22
	.byte	0x1
	.byte	0x52
	.4byte	0x33
	.byte	0x1
	.byte	0x55
	.uleb128 0x9
	.4byte	0x21c
	.4byte	$LBB8
	.4byte	$LBE8
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x54
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x9
	.4byte	0x235
	.4byte	$LBB9
	.4byte	$LBE9
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x5b
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0xb
	.4byte	$LBB10
	.4byte	$LBE10
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x5f
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.4byte	0x2bb
	.byte	0x1
	.4byte	$LASF23
	.byte	0x1
	.byte	0x64
	.byte	0x1
	.4byte	$LFB47
	.4byte	$LFE47
	.4byte	$LSFDE8
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF22
	.byte	0x1
	.byte	0x64
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x6
	.4byte	$LASF24
	.byte	0x1
	.byte	0x64
	.4byte	0x11a
	.byte	0x1
	.byte	0x61
	.uleb128 0xa
	.ascii	"u\000"
	.byte	0x1
	.byte	0x65
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x9
	.4byte	0x2a5
	.4byte	$LBB11
	.4byte	$LBE11
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x68
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0xb
	.4byte	$LBB12
	.4byte	$LBE12
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x6b
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.4byte	0x382
	.byte	0x1
	.4byte	$LASF25
	.byte	0x1
	.byte	0x74
	.byte	0x1
	.4byte	$LFB48
	.4byte	$LFE48
	.4byte	$LSFDE10
	.byte	0x1
	.byte	0x6d
	.uleb128 0x9
	.4byte	0x2ef
	.4byte	$LBB13
	.4byte	$LBE13
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x75
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x9
	.4byte	0x308
	.4byte	$LBB14
	.4byte	$LBE14
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x78
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x9
	.4byte	0x321
	.4byte	$LBB15
	.4byte	$LBE15
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x7a
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x9
	.4byte	0x33a
	.4byte	$LBB16
	.4byte	$LBE16
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x7c
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x9
	.4byte	0x353
	.4byte	$LBB17
	.4byte	$LBE17
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x7e
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x9
	.4byte	0x36c
	.4byte	$LBB18
	.4byte	$LBE18
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x82
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0xb
	.4byte	$LBB19
	.4byte	$LBE19
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x84
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.4byte	0x3fe
	.byte	0x1
	.4byte	$LASF26
	.byte	0x1
	.byte	0x8d
	.4byte	0x45
	.4byte	$LFB49
	.4byte	$LFE49
	.4byte	$LSFDE12
	.byte	0x1
	.byte	0x6d
	.uleb128 0xd
	.4byte	$LASF27
	.byte	0x1
	.byte	0x8e
	.4byte	0x33
	.uleb128 0xe
	.ascii	"res\000"
	.byte	0x1
	.byte	0x8f
	.4byte	0x45
	.uleb128 0x9
	.4byte	0x3cf
	.4byte	$LBB20
	.4byte	$LBE20
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x92
	.4byte	0x50
	.byte	0x1
	.byte	0x60
	.byte	0x0
	.uleb128 0x9
	.4byte	0x3e8
	.4byte	$LBB21
	.4byte	$LBE21
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x94
	.4byte	0x50
	.byte	0x1
	.byte	0x60
	.byte	0x0
	.uleb128 0xb
	.4byte	$LBB22
	.4byte	$LBE22
	.uleb128 0xa
	.ascii	"i\000"
	.byte	0x1
	.byte	0x96
	.4byte	0x50
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.byte	0x1
	.4byte	$LASF38
	.byte	0x1
	.byte	0x9b
	.4byte	0x45
	.4byte	$LFB50
	.4byte	$LFE50
	.4byte	$LSFDE14
	.byte	0x1
	.byte	0x6d
	.uleb128 0x10
	.4byte	0x488
	.byte	0x1
	.4byte	$LASF28
	.byte	0x1
	.byte	0xad
	.byte	0x1
	.4byte	0x45
	.4byte	$LFB51
	.4byte	$LFE51
	.4byte	$LSFDE16
	.byte	0x1
	.byte	0x6d
	.uleb128 0x6
	.4byte	$LASF29
	.byte	0x1
	.byte	0xad
	.4byte	0x33
	.byte	0x1
	.byte	0x63
	.uleb128 0x6
	.4byte	$LASF24
	.byte	0x1
	.byte	0xad
	.4byte	0x11a
	.byte	0x1
	.byte	0x62
	.uleb128 0x11
	.4byte	$LASF30
	.byte	0x1
	.byte	0xbe
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 28
	.uleb128 0xa
	.ascii	"ACK\000"
	.byte	0x1
	.byte	0xbe
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0xa
	.ascii	"con\000"
	.byte	0x1
	.byte	0xbf
	.4byte	0x70
	.byte	0x1
	.byte	0x60
	.uleb128 0xa
	.ascii	"ret\000"
	.byte	0x1
	.byte	0xc0
	.4byte	0x33
	.byte	0x1
	.byte	0x61
	.byte	0x0
	.uleb128 0x12
	.4byte	0x4f0
	.byte	0x1
	.4byte	$LASF31
	.byte	0x1
	.2byte	0x100
	.byte	0x1
	.4byte	0x45
	.4byte	$LFB52
	.4byte	$LFE52
	.4byte	$LSFDE18
	.byte	0x1
	.byte	0x6d
	.uleb128 0x13
	.4byte	$LASF29
	.byte	0x1
	.2byte	0x100
	.4byte	0x33
	.byte	0x1
	.byte	0x62
	.uleb128 0x13
	.4byte	$LASF24
	.byte	0x1
	.2byte	0x100
	.4byte	0x33
	.byte	0x1
	.byte	0x63
	.uleb128 0x14
	.ascii	"con\000"
	.byte	0x1
	.2byte	0x115
	.4byte	0x82
	.byte	0x1
	.byte	0x60
	.uleb128 0x14
	.ascii	"ACK\000"
	.byte	0x1
	.2byte	0x116
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x14
	.ascii	"ret\000"
	.byte	0x1
	.2byte	0x117
	.4byte	0x33
	.byte	0x1
	.byte	0x61
	.byte	0x0
	.uleb128 0x15
	.4byte	$LASF32
	.byte	0x1
	.byte	0x1e
	.4byte	0x33
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	smi_SCK
	.uleb128 0x15
	.4byte	$LASF33
	.byte	0x1
	.byte	0x1f
	.4byte	0x33
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	smi_SDA
	.uleb128 0x15
	.4byte	$LASF34
	.byte	0x1
	.byte	0x20
	.4byte	0x33
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	smi_RST
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
	.uleb128 0x8
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0x34
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
	.uleb128 0xb
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
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
	.uleb128 0xd
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
	.uleb128 0xe
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.uleb128 0x2e
	.byte	0x0
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
	.uleb128 0x11
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
	.uleb128 0x12
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
	.uleb128 0x13
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
	.uleb128 0x14
	.uleb128 0x34
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
	.uleb128 0x15
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
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.4byte	0xe0
	.2byte	0x2
	.4byte	$Ldebug_info0
	.4byte	0x527
	.4byte	0xb0
	.ascii	"_rtl865x_setGpioDataBit\000"
	.4byte	0xe6
	.ascii	"_rtl865x_getGpioDataBit\000"
	.4byte	0x120
	.ascii	"_smi_start\000"
	.4byte	0x1ce
	.ascii	"_smi_writeBit\000"
	.4byte	0x24b
	.ascii	"_smi_readBit\000"
	.4byte	0x2bb
	.ascii	"_smi_stop\000"
	.4byte	0x382
	.ascii	"smi_reset\000"
	.4byte	0x3fe
	.ascii	"smi_init\000"
	.4byte	0x418
	.ascii	"smi_read\000"
	.4byte	0x488
	.ascii	"smi_write\000"
	.4byte	0x4f0
	.ascii	"smi_SCK\000"
	.4byte	0x502
	.ascii	"smi_SDA\000"
	.4byte	0x514
	.ascii	"smi_RST\000"
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
$LASF25:
	.ascii	"_smi_stop\000"
$LASF21:
	.ascii	"signal\000"
$LASF17:
	.ascii	"_rtl865x_setGpioDataBit\000"
$LASF37:
	.ascii	"/home/IOData/BR6456IN/srcbase/boot/realtek_8370\000"
$LASF4:
	.ascii	"int32\000"
$LASF0:
	.ascii	"long long unsigned int\000"
$LASF33:
	.ascii	"smi_SDA\000"
$LASF16:
	.ascii	"value\000"
$LASF35:
	.ascii	"GNU C 3.4.2 -g\000"
$LASF20:
	.ascii	"_smi_writeBit\000"
$LASF27:
	.ascii	"gpioId\000"
$LASF31:
	.ascii	"smi_write\000"
$LASF19:
	.ascii	"_smi_start\000"
$LASF11:
	.ascii	"char\000"
$LASF15:
	.ascii	"gpio\000"
$LASF5:
	.ascii	"uint16\000"
$LASF10:
	.ascii	"int8\000"
$LASF22:
	.ascii	"bitLen\000"
$LASF28:
	.ascii	"smi_read\000"
$LASF1:
	.ascii	"long long int\000"
$LASF24:
	.ascii	"rData\000"
$LASF32:
	.ascii	"smi_SCK\000"
$LASF26:
	.ascii	"smi_reset\000"
$LASF3:
	.ascii	"uint32\000"
$LASF38:
	.ascii	"smi_init\000"
$LASF30:
	.ascii	"rawData\000"
$LASF7:
	.ascii	"short int\000"
$LASF13:
	.ascii	"long int\000"
$LASF2:
	.ascii	"unsigned int\000"
$LASF12:
	.ascii	"long unsigned int\000"
$LASF29:
	.ascii	"mAddrs\000"
$LASF9:
	.ascii	"unsigned char\000"
$LASF23:
	.ascii	"_smi_readBit\000"
$LASF36:
	.ascii	"smi.c\000"
$LASF14:
	.ascii	"signed char\000"
$LASF18:
	.ascii	"_rtl865x_getGpioDataBit\000"
$LASF6:
	.ascii	"short unsigned int\000"
$LASF34:
	.ascii	"smi_RST\000"
$LASF8:
	.ascii	"uint8\000"
	.ident	"GCC: (GNU) 3.4.2"
