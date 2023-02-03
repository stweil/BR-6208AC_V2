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
	.globl	_rtl8370_VlanMCStUser2Smi
	.ent	_rtl8370_VlanMCStUser2Smi
	.type	_rtl8370_VlanMCStUser2Smi, @function
_rtl8370_VlanMCStUser2Smi:
$LFB2:
	.file 1 "rtl8370_asicdrv_vlan.c"
	.loc 1 31 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	.loc 1 33 0
	lhu	$3,8($4)
	lhu	$2,2($5)
	.loc 1 32 0
	lhu	$7,4($4)
	.loc 1 33 0
	sll	$3,$3,12
	andi	$2,$2,0xfff
	or	$2,$2,$3
	sh	$2,2($5)
	.loc 1 34 0
	lhu	$3,6($4)
	li	$6,-4096			# 0xfffffffffffff000
	and	$2,$2,$6
	andi	$3,$3,0xfff
	or	$2,$2,$3
	sh	$2,2($5)
	.loc 1 35 0
	lhu	$6,0($4)
	lhu	$3,6($5)
	.loc 1 36 0
	lhu	$2,4($5)
	.loc 1 35 0
	andi	$6,$6,0x1fff
	andi	$3,$3,0xe000
	or	$3,$3,$6
	sh	$3,6($5)
	.loc 1 36 0
	lbu	$3,12($4)
	andi	$2,$2,0xc03f
	.loc 1 32 0
	sh	$7,0($5)
	.loc 1 36 0
	sll	$3,$3,6
	or	$2,$2,$3
	sh	$2,4($5)
	.loc 1 37 0
	lhu	$3,10($4)
	andi	$2,$2,0xffdf
	andi	$3,$3,0x1
	sll	$3,$3,5
	or	$2,$2,$3
	sh	$2,4($5)
	.loc 1 38 0
	lhu	$3,16($4)
	andi	$2,$2,0xffe3
	andi	$3,$3,0x7
	sll	$3,$3,2
	or	$2,$2,$3
	sh	$2,4($5)
	.loc 1 39 0
	lhu	$3,14($4)
	andi	$2,$2,0xfffd
	andi	$3,$3,0x1
	sll	$3,$3,1
	or	$2,$2,$3
	sh	$2,4($5)
	.loc 1 40 0
	lhu	$3,2($4)
	andi	$2,$2,0xfffe
	andi	$3,$3,0x1
	or	$2,$2,$3
	j	$31
	sh	$2,4($5)

	.set	macro
	.set	reorder
$LFE2:
	.end	_rtl8370_VlanMCStUser2Smi
	.align	2
	.globl	_rtl8370_VlanMCStSmi2User
	.ent	_rtl8370_VlanMCStSmi2User
	.type	_rtl8370_VlanMCStSmi2User, @function
_rtl8370_VlanMCStSmi2User:
$LFB3:
	.loc 1 44 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	.loc 1 49 0
	lhu	$6,4($4)
	.loc 1 46 0
	lhu	$7,2($4)
	.loc 1 48 0
	lhu	$9,6($4)
	.loc 1 49 0
	andi	$2,$6,0xffff
	.loc 1 45 0
	lhu	$11,0($4)
	.loc 1 52 0
	srl	$10,$2,1
	.loc 1 46 0
	andi	$3,$7,0xffff
	.loc 1 49 0
	srl	$4,$2,6
	.loc 1 50 0
	srl	$8,$2,5
	.loc 1 51 0
	srl	$2,$2,2
	.loc 1 46 0
	srl	$3,$3,12
	.loc 1 47 0
	andi	$7,$7,0xfff
	.loc 1 48 0
	andi	$9,$9,0x1fff
	.loc 1 49 0
	andi	$4,$4,0xff
	.loc 1 50 0
	andi	$8,$8,0x1
	.loc 1 51 0
	andi	$2,$2,0x7
	.loc 1 52 0
	andi	$10,$10,0x1
	.loc 1 53 0
	andi	$6,$6,0x1
	sh	$6,2($5)
	.loc 1 45 0
	sh	$11,4($5)
	.loc 1 46 0
	sh	$3,8($5)
	.loc 1 47 0
	sh	$7,6($5)
	.loc 1 48 0
	sh	$9,0($5)
	.loc 1 49 0
	sh	$4,12($5)
	.loc 1 50 0
	sh	$8,10($5)
	.loc 1 51 0
	sh	$2,16($5)
	.loc 1 52 0
	.loc 1 53 0
	j	$31
	sh	$10,14($5)

	.set	macro
	.set	reorder
$LFE3:
	.end	_rtl8370_VlanMCStSmi2User
	.align	2
	.globl	_rtl8370_Vlan4kStUser2Smi
	.ent	_rtl8370_Vlan4kStUser2Smi
	.type	_rtl8370_Vlan4kStUser2Smi, @function
_rtl8370_Vlan4kStUser2Smi:
$LFB4:
	.loc 1 57 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	.loc 1 58 0
	lhu	$3,4($4)
	.loc 1 59 0
	lhu	$2,2($5)
	.loc 1 58 0
	sh	$3,0($5)
	.loc 1 59 0
	lhu	$6,8($4)
	andi	$2,$2,0xf000
	.loc 1 61 0
	lhu	$3,4($5)
	.loc 1 59 0
	andi	$6,$6,0xfff
	or	$2,$2,$6
	sh	$2,2($5)
	.loc 1 60 0
	lhu	$6,10($4)
	andi	$2,$2,0xfff
	.loc 1 61 0
	andi	$3,$3,0xfffe
	.loc 1 60 0
	sll	$6,$6,12
	or	$2,$2,$6
	sh	$2,2($5)
	.loc 1 61 0
	lhu	$2,2($4)
	.loc 1 67 0
	lhu	$6,6($5)
	.loc 1 61 0
	andi	$2,$2,0x1
	or	$3,$3,$2
	sh	$3,4($5)
	.loc 1 62 0
	lhu	$2,16($4)
	andi	$3,$3,0xfffd
	.loc 1 67 0
	andi	$6,$6,0xc000
	.loc 1 62 0
	andi	$2,$2,0x1
	sll	$2,$2,1
	or	$3,$3,$2
	sh	$3,4($5)
	.loc 1 63 0
	lhu	$2,18($4)
	andi	$3,$3,0xffe3
	andi	$2,$2,0x7
	sll	$2,$2,2
	or	$3,$3,$2
	sh	$3,4($5)
	.loc 1 64 0
	lhu	$2,12($4)
	andi	$3,$3,0xffdf
	andi	$2,$2,0x1
	sll	$2,$2,5
	or	$3,$3,$2
	sh	$3,4($5)
	.loc 1 65 0
	lbu	$2,14($4)
	andi	$3,$3,0xc03f
	sll	$2,$2,6
	or	$3,$3,$2
	sh	$3,4($5)
	.loc 1 66 0
	lhu	$2,6($4)
	andi	$3,$3,0x3fff
	andi	$2,$2,0x3
	sll	$2,$2,14
	or	$3,$3,$2
	sh	$3,4($5)
	.loc 1 67 0
	lhu	$2,6($4)
	srl	$2,$2,2
	or	$6,$6,$2
	j	$31
	sh	$6,6($5)

	.set	macro
	.set	reorder
$LFE4:
	.end	_rtl8370_Vlan4kStUser2Smi
	.align	2
	.globl	_rtl8370_Vlan4kStSmi2User
	.ent	_rtl8370_Vlan4kStSmi2User
	.type	_rtl8370_Vlan4kStSmi2User, @function
_rtl8370_Vlan4kStSmi2User:
$LFB5:
	.loc 1 71 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	
	.loc 1 72 0
	lhu	$2,0($4)
	sh	$2,4($5)
	.loc 1 73 0
	lhu	$2,2($4)
	andi	$2,$2,0xfff
	sh	$2,8($5)
	.loc 1 74 0
	lhu	$2,2($4)
	srl	$2,$2,12
	sh	$2,10($5)
	.loc 1 75 0
	lhu	$2,4($4)
	andi	$2,$2,0x1
	sh	$2,2($5)
	.loc 1 76 0
	lhu	$2,4($4)
	srl	$2,$2,1
	andi	$2,$2,0x1
	sh	$2,16($5)
	.loc 1 77 0
	lhu	$2,4($4)
	srl	$2,$2,2
	andi	$2,$2,0x7
	sh	$2,18($5)
	.loc 1 78 0
	lhu	$2,4($4)
	srl	$2,$2,5
	andi	$2,$2,0x1
	sh	$2,12($5)
	.loc 1 79 0
	lhu	$2,4($4)
	srl	$2,$2,6
	andi	$2,$2,0xff
	sh	$2,14($5)
	.loc 1 80 0
	lhu	$2,6($4)
	lhu	$3,4($4)
	andi	$2,$2,0x3fff
	srl	$3,$3,14
	sll	$2,$2,2
	or	$3,$3,$2
	j	$31
	sh	$3,6($5)

	.set	macro
	.set	reorder
$LFE5:
	.end	_rtl8370_Vlan4kStSmi2User
	.align	2
	.globl	rtl8370_setAsicVlanMemberConfig
	.ent	rtl8370_setAsicVlanMemberConfig
	.type	rtl8370_setAsicVlanMemberConfig, @function
rtl8370_setAsicVlanMemberConfig:
$LFB6:
	.loc 1 103 0
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
	.loc 1 112 0
	sltu	$2,$4,32
	.loc 1 103 0
	move	$18,$4
	.loc 1 112 0
	bne	$2,$0,$L6
	move	$16,$5

	.loc 1 113 0
	li	$2,65536			# 0x10000
	b	$L5
	ori	$3,$2,0x5

$L6:
	.loc 1 116 0
	.loc 1 115 0
	beq	$5,$0,$L5
	li	$3,1			# 0x1

	.loc 1 118 0
	lhu	$2,0($5)
	sltu	$2,$2,8192
	.loc 1 119 0
	.loc 1 118 0
	beq	$2,$0,$L5
	li	$3,65536			# 0x10000

	.loc 1 121 0
	lhu	$2,2($5)
	sltu	$2,$2,2
	.loc 1 122 0
	.loc 1 121 0
	beq	$2,$0,$L5
	li	$3,1			# 0x1

	.loc 1 124 0
	lhu	$2,6($5)
	sltu	$2,$2,4096
	.loc 1 125 0
	.loc 1 124 0
	bne	$2,$0,$L10
	li	$2,393216			# 0x60000

	.loc 1 125 0
	b	$L5
	ori	$3,$2,0x7

$L10:
	.loc 1 127 0
	lhu	$2,8($5)
	sltu	$2,$2,16
	.loc 1 128 0
	.loc 1 127 0
	beq	$2,$0,$L5
	li	$3,196608			# 0x30000

	.loc 1 130 0
	lhu	$2,10($5)
	sltu	$2,$2,2
	.loc 1 131 0
	.loc 1 130 0
	beq	$2,$0,$L5
	li	$3,1			# 0x1

	.loc 1 133 0
	lhu	$2,12($5)
	sltu	$2,$2,64
	.loc 1 134 0
	.loc 1 133 0
	bne	$2,$0,$L13
	li	$2,458752			# 0x70000

	.loc 1 134 0
	b	$L5
	ori	$3,$2,0xe

$L13:
	.loc 1 136 0
	lhu	$2,14($5)
	sltu	$2,$2,2
	.loc 1 137 0
	.loc 1 136 0
	beq	$2,$0,$L5
	li	$3,1			# 0x1

	.loc 1 139 0
	lhu	$2,16($5)
	sltu	$2,$2,8
	.loc 1 142 0
	.loc 1 139 0
	bne	$2,$0,$L15
	lw	$25,%call16(memset)($28)

	.loc 1 140 0
	li	$2,65536			# 0x10000
	b	$L5
	ori	$3,$2,0x1

$L23:
	.loc 1 153 0
	b	$L5
	move	$3,$2

$L15:
	.loc 1 142 0
	addiu	$4,$sp,24
	move	$5,$0
	jalr	$25
	li	$6,8			# 0x8

	lw	$28,16($sp)
	.loc 1 143 0
	move	$4,$16
	addiu	$5,$sp,24
	lw	$25,%call16(_rtl8370_VlanMCStUser2Smi)($28)
	.loc 1 144 0
	move	$17,$5
	.loc 1 146 0
	.loc 1 143 0
	jalr	$25
	move	$16,$0

	lw	$28,16($sp)
	.loc 1 151 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
$L24:
	lhu	$5,0($17)
	.loc 1 148 0
	sll	$4,$18,2
	addu	$4,$4,$16
	.loc 1 151 0
	jalr	$25
	addiu	$4,$4,1832

	.loc 1 146 0
	addiu	$16,$16,1
	.loc 1 151 0
	lw	$28,16($sp)
	.loc 1 146 0
	sltu	$3,$16,4
	.loc 1 155 0
	.loc 1 152 0
	bne	$2,$0,$L23
	addiu	$17,$17,2

	.loc 1 151 0
	.loc 1 146 0
	bne	$3,$0,$L24
	lw	$25,%call16(rtl8370_setAsicReg)($28)

$L5:
	.loc 1 159 0
	lw	$31,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
$LFE6:
	.end	rtl8370_setAsicVlanMemberConfig
	.align	2
	.globl	rtl8370_getAsicVlanMemberConfig
	.ent	rtl8370_getAsicVlanMemberConfig
	.type	rtl8370_getAsicVlanMemberConfig, @function
rtl8370_getAsicVlanMemberConfig:
$LFB7:
	.loc 1 174 0
	.frame	$sp,64,$31		# vars= 16, regs= 5/0, args= 16, gp= 8
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-64
$LCFI5:
	sw	$31,56($sp)
$LCFI6:
	sw	$19,52($sp)
$LCFI7:
	sw	$18,48($sp)
$LCFI8:
	sw	$17,44($sp)
$LCFI9:
	sw	$16,40($sp)
$LCFI10:
	.cprestore	16
	.loc 1 182 0
	sltu	$2,$4,32
	.loc 1 174 0
	move	$18,$4
	.loc 1 182 0
	bne	$2,$0,$L26
	move	$19,$5

	.loc 1 183 0
	li	$2,65536			# 0x10000
	b	$L25
	ori	$2,$2,0x5

$L26:
	.loc 1 186 0
	.loc 1 185 0
	beq	$5,$0,$L25
	li	$2,1			# 0x1

	.loc 1 188 0
	lw	$25,%call16(memset)($28)
	addiu	$4,$sp,24
	move	$5,$0
	jalr	$25
	li	$6,8			# 0x8

	lw	$28,16($sp)
	.loc 1 189 0
	addiu	$17,$sp,24
	.loc 1 191 0
	move	$16,$0
$L32:
	.loc 1 195 0
	lw	$25,%call16(rtl8370_getAsicReg)($28)
	.loc 1 193 0
	sll	$4,$18,2
	addu	$4,$4,$16
	.loc 1 195 0
	addiu	$4,$4,1832
	jalr	$25
	addiu	$5,$sp,32

	.loc 1 191 0
	addiu	$16,$16,1
	.loc 1 195 0
	lw	$28,16($sp)
	.loc 1 191 0
	.loc 1 196 0
	bne	$2,$0,$L25
	sltu	$3,$16,4

	.loc 1 199 0
	lw	$2,32($sp)
	sh	$2,0($17)
	.loc 1 200 0
	.loc 1 191 0
	bne	$3,$0,$L32
	addiu	$17,$17,2

	.loc 1 203 0
	lw	$25,%call16(_rtl8370_VlanMCStSmi2User)($28)
	move	$5,$19
	jalr	$25
	addiu	$4,$sp,24

	lw	$28,16($sp)
	.loc 1 204 0
	move	$2,$0
$L25:
	.loc 1 205 0
	lw	$31,56($sp)
	lw	$19,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	j	$31
	addiu	$sp,$sp,64

	.set	macro
	.set	reorder
$LFE7:
	.end	rtl8370_getAsicVlanMemberConfig
	.align	2
	.globl	rtl8370_setAsicVlan4kEntry
	.ent	rtl8370_setAsicVlan4kEntry
	.type	rtl8370_setAsicVlan4kEntry, @function
rtl8370_setAsicVlan4kEntry:
$LFB8:
	.loc 1 224 0
	.frame	$sp,48,$31		# vars= 8, regs= 4/0, args= 16, gp= 8
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-48
$LCFI11:
	sw	$31,44($sp)
$LCFI12:
	sw	$18,40($sp)
$LCFI13:
	sw	$17,36($sp)
$LCFI14:
	sw	$16,32($sp)
$LCFI15:
	.cprestore	16
	.loc 1 224 0
	move	$17,$4
	.loc 1 232 0
	.loc 1 231 0
	beq	$4,$0,$L36
	li	$3,1			# 0x1

	.loc 1 234 0
	lhu	$2,0($4)
	sltu	$2,$2,4096
	.loc 1 235 0
	.loc 1 234 0
	beq	$2,$0,$L36
	li	$3,65536			# 0x10000

	.loc 1 237 0
	lhu	$2,2($4)
	sltu	$2,$2,2
	.loc 1 238 0
	.loc 1 237 0
	beq	$2,$0,$L36
	li	$3,1			# 0x1

	.loc 1 240 0
	lhu	$2,8($4)
	sltu	$2,$2,4096
	.loc 1 241 0
	.loc 1 240 0
	bne	$2,$0,$L40
	li	$2,393216			# 0x60000

	.loc 1 241 0
	b	$L36
	ori	$3,$2,0x7

$L40:
	.loc 1 243 0
	lhu	$2,10($4)
	sltu	$2,$2,16
	.loc 1 244 0
	.loc 1 243 0
	beq	$2,$0,$L36
	li	$3,196608			# 0x30000

	.loc 1 246 0
	lhu	$2,12($4)
	sltu	$2,$2,2
	.loc 1 247 0
	.loc 1 246 0
	beq	$2,$0,$L36
	li	$3,1			# 0x1

	.loc 1 249 0
	lhu	$2,14($4)
	sltu	$2,$2,64
	.loc 1 250 0
	.loc 1 249 0
	bne	$2,$0,$L43
	li	$2,458752			# 0x70000

	.loc 1 250 0
	b	$L36
	ori	$3,$2,0xe

$L43:
	.loc 1 252 0
	lhu	$2,16($4)
	sltu	$2,$2,2
	.loc 1 253 0
	.loc 1 252 0
	beq	$2,$0,$L36
	li	$3,1			# 0x1

	.loc 1 255 0
	lhu	$2,18($4)
	sltu	$2,$2,8
	.loc 1 258 0
	.loc 1 255 0
	bne	$2,$0,$L45
	lw	$25,%call16(memset)($28)

	.loc 1 256 0
	li	$2,65536			# 0x10000
	b	$L36
	ori	$3,$2,0x1

$L45:
	.loc 1 258 0
	addiu	$4,$sp,24
	move	$5,$0
	jalr	$25
	li	$6,8			# 0x8

	lw	$28,16($sp)
	.loc 1 259 0
	addiu	$5,$sp,24
	move	$4,$17
	lw	$25,%call16(_rtl8370_Vlan4kStUser2Smi)($28)
	.loc 1 262 0
	move	$18,$5
	.loc 1 263 0
	.loc 1 259 0
	jalr	$25
	move	$16,$0

	lw	$28,16($sp)
	.loc 1 266 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
$L56:
	lhu	$5,0($18)
	addiu	$4,$16,1283
	.loc 1 263 0
	.loc 1 266 0
	jalr	$25
	addiu	$16,$16,1

	.loc 1 270 0
	addiu	$18,$18,2
	.loc 1 266 0
	lw	$28,16($sp)
	.loc 1 263 0
	.loc 1 267 0
	bne	$2,$0,$L55
	sltu	$3,$16,4

	.loc 1 266 0
	.loc 1 263 0
	bne	$3,$0,$L56
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 275 0
	lhu	$5,0($17)
	jalr	$25
	li	$4,1281			# 0x501

	.loc 1 277 0
	move	$3,$2
	.loc 1 275 0
	.loc 1 276 0
	bne	$2,$0,$L36
	lw	$28,16($sp)

	.loc 1 280 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,1280			# 0x500
	jalr	$25
	li	$5,19			# 0x13

	.loc 1 282 0
	move	$3,$2
	.loc 1 280 0
	.loc 1 281 0
	bne	$2,$0,$L36
	lw	$28,16($sp)

	.loc 1 287 0
	b	$L52
	move	$3,$0

$L55:
	move	$3,$2
$L52:
$L36:
	.loc 1 288 0
	lw	$31,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
$LFE8:
	.end	rtl8370_setAsicVlan4kEntry
	.align	2
	.globl	rtl8370_getAsicVlan4kEntry
	.ent	rtl8370_getAsicVlan4kEntry
	.type	rtl8370_getAsicVlan4kEntry, @function
rtl8370_getAsicVlan4kEntry:
$LFB9:
	.loc 1 302 0
	.frame	$sp,56,$31		# vars= 16, regs= 4/0, args= 16, gp= 8
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-56
$LCFI16:
	sw	$31,52($sp)
$LCFI17:
	sw	$18,48($sp)
$LCFI18:
	sw	$17,44($sp)
$LCFI19:
	sw	$16,40($sp)
$LCFI20:
	.cprestore	16
	.loc 1 302 0
	move	$18,$4
	.loc 1 310 0
	.loc 1 309 0
	beq	$4,$0,$L57
	li	$3,1			# 0x1

	.loc 1 313 0
	lhu	$5,0($4)
	.loc 1 314 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,1281			# 0x501
	.loc 1 313 0
	.loc 1 314 0
	jalr	$25
	sw	$5,32($sp)

	.loc 1 316 0
	move	$3,$2
	.loc 1 314 0
	.loc 1 315 0
	bne	$2,$0,$L57
	lw	$28,16($sp)

	.loc 1 319 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,1280			# 0x500
	jalr	$25
	li	$5,3			# 0x3

	.loc 1 321 0
	move	$3,$2
	.loc 1 319 0
	.loc 1 320 0
	bne	$2,$0,$L57
	lw	$28,16($sp)

	.loc 1 326 0
	b	$L60
	addiu	$17,$sp,24

$L68:
	.loc 1 331 0
	b	$L57
	move	$3,$2

$L60:
	.loc 1 327 0
	move	$16,$0
$L65:
	.loc 1 329 0
	lw	$25,%call16(rtl8370_getAsicReg)($28)
	addiu	$4,$16,1283
	jalr	$25
	addiu	$5,$sp,32

	.loc 1 327 0
	addiu	$16,$16,1
	.loc 1 329 0
	lw	$28,16($sp)
	.loc 1 327 0
	.loc 1 330 0
	bne	$2,$0,$L68
	sltu	$3,$16,4

	.loc 1 333 0
	lw	$2,32($sp)
	sh	$2,0($17)
	.loc 1 334 0
	.loc 1 327 0
	bne	$3,$0,$L65
	addiu	$17,$17,2

	.loc 1 337 0
	lw	$25,%call16(_rtl8370_Vlan4kStSmi2User)($28)
	move	$5,$18
	jalr	$25
	addiu	$4,$sp,24

	lw	$28,16($sp)
	.loc 1 343 0
	move	$3,$0
$L57:
	.loc 1 344 0
	lw	$31,52($sp)
	lw	$18,48($sp)
	lw	$17,44($sp)
	lw	$16,40($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
$LFE9:
	.end	rtl8370_getAsicVlan4kEntry
	.align	2
	.globl	rtl8370_setAsicVlanAccpetFrameType
	.ent	rtl8370_setAsicVlanAccpetFrameType
	.type	rtl8370_setAsicVlanAccpetFrameType, @function
rtl8370_setAsicVlanAccpetFrameType:
$LFB10:
	.loc 1 359 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 362 0
	sltu	$2,$4,16
	.loc 1 359 0
	move	$6,$5
	.loc 1 365 0
	sltu	$7,$5,3
	.loc 1 363 0
	.loc 1 362 0
	beq	$2,$0,$L69
	li	$3,3			# 0x3

	.loc 1 369 0
	andi	$2,$4,0x7
	sll	$2,$2,1
	li	$5,3			# 0x3
	.loc 1 368 0
	srl	$4,$4,3
	.loc 1 370 0
	sll	$5,$5,$2
	.loc 1 366 0
	li	$2,65536			# 0x10000
	.loc 1 370 0
	lw	$25,%call16(rtl8370_setAsicRegBits)($28)
	addiu	$4,$4,1962
	.loc 1 366 0
	.loc 1 365 0
	beq	$7,$0,$L69
	ori	$3,$2,0x3

	.loc 1 370 0
	jr	$25
	nop

$L69:
	.loc 1 371 0
	j	$31
	move	$2,$3

	.set	macro
	.set	reorder
$LFE10:
	.end	rtl8370_setAsicVlanAccpetFrameType
	.align	2
	.globl	rtl8370_setAsicVlanEgressTagMode
	.ent	rtl8370_setAsicVlanEgressTagMode
	.type	rtl8370_setAsicVlanEgressTagMode, @function
rtl8370_setAsicVlanEgressTagMode:
$LFB11:
	.loc 1 463 0
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 16, gp= 8
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-32
$LCFI21:
	sw	$31,24($sp)
$LCFI22:
	.cprestore	16
	.loc 1 466 0
	sltu	$2,$4,16
	.loc 1 469 0
	sltu	$7,$5,4
	.loc 1 467 0
	.loc 1 466 0
	beq	$2,$0,$L72
	li	$3,3			# 0x3

	.loc 1 472 0
	sll	$4,$4,5
	move	$6,$5
	lw	$25,%call16(rtl8370_setAsicRegBits)($28)
	addiu	$4,$4,14
	li	$5,48			# 0x30
	.loc 1 470 0
	.loc 1 469 0
	beq	$7,$0,$L72
	li	$3,1			# 0x1

	.loc 1 472 0
	jalr	$25
	nop

	lw	$28,16($sp)
	move	$3,$2
$L72:
	.loc 1 477 0
	lw	$31,24($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
$LFE11:
	.end	rtl8370_setAsicVlanEgressTagMode
	.align	2
	.globl	rtl8370_setAsicVlanPortBasedVID
	.ent	rtl8370_setAsicVlanPortBasedVID
	.type	rtl8370_setAsicVlanPortBasedVID, @function
rtl8370_setAsicVlanPortBasedVID:
$LFB12:
	.loc 1 525 0
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 16, gp= 8
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-40
$LCFI23:
	sw	$31,32($sp)
$LCFI24:
	sw	$17,28($sp)
$LCFI25:
	sw	$16,24($sp)
$LCFI26:
	.cprestore	16
	.loc 1 529 0
	sltu	$2,$4,16
	.loc 1 525 0
	move	$16,$4
	move	$8,$5
	.loc 1 532 0
	sltu	$4,$5,32
	.loc 1 525 0
	move	$17,$6
	.loc 1 530 0
	.loc 1 529 0
	beq	$2,$0,$L76
	li	$3,3			# 0x3

	.loc 1 533 0
	li	$2,65536			# 0x10000
	ori	$3,$2,0x5
	.loc 1 535 0
	.loc 1 532 0
	beq	$4,$0,$L76
	sltu	$7,$6,8

	.loc 1 539 0
	andi	$5,$16,0x1
	li	$2,31			# 0x1f
	sll	$5,$5,3
	.loc 1 538 0
	srl	$4,$16,1
	.loc 1 540 0
	sll	$5,$2,$5
	.loc 1 536 0
	li	$2,65536			# 0x10000
	.loc 1 540 0
	lw	$25,%call16(rtl8370_setAsicRegBits)($28)
	addiu	$4,$4,1792
	move	$6,$8
	.loc 1 536 0
	.loc 1 535 0
	beq	$7,$0,$L76
	ori	$3,$2,0x1

	.loc 1 540 0
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 545 0
	andi	$3,$16,0x3
	sll	$3,$3,2
	.loc 1 544 0
	srl	$4,$16,2
	.loc 1 545 0
	li	$5,7			# 0x7
	.loc 1 546 0
	sll	$5,$5,$3
	lw	$25,%call16(rtl8370_setAsicRegBits)($28)
	addiu	$4,$4,2129
	move	$6,$17
	.loc 1 542 0
	.loc 1 541 0
	bne	$2,$0,$L76
	move	$3,$2

	.loc 1 546 0
	jalr	$25
	nop

	lw	$28,16($sp)
	move	$3,$2
$L76:
	.loc 1 551 0
	lw	$31,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
$LFE12:
	.end	rtl8370_setAsicVlanPortBasedVID
	.align	2
	.globl	rtl8370_getAsicVlanPortBasedVID
	.ent	rtl8370_getAsicVlanPortBasedVID
	.type	rtl8370_getAsicVlanPortBasedVID, @function
rtl8370_getAsicVlanPortBasedVID:
$LFB13:
	.loc 1 570 0
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 16, gp= 8
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-40
$LCFI27:
	sw	$31,32($sp)
$LCFI28:
	sw	$17,28($sp)
$LCFI29:
	sw	$16,24($sp)
$LCFI30:
	.cprestore	16
	.loc 1 574 0
	sltu	$2,$4,16
	.loc 1 570 0
	move	$16,$4
	move	$7,$5
	move	$17,$6
	.loc 1 575 0
	.loc 1 574 0
	beq	$2,$0,$L82
	li	$3,3			# 0x3

	.loc 1 578 0
	.loc 1 577 0
	beq	$5,$0,$L82
	li	$3,1			# 0x1

	.loc 1 584 0
	andi	$5,$4,0x1
	sll	$5,$5,3
	.loc 1 583 0
	srl	$4,$4,1
	.loc 1 584 0
	li	$2,31			# 0x1f
	.loc 1 585 0
	lw	$25,%call16(rtl8370_getAsicRegBits)($28)
	sll	$5,$2,$5
	addiu	$4,$4,1792
	.loc 1 580 0
	beq	$17,$0,$L82
	move	$6,$7

	.loc 1 585 0
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 590 0
	andi	$3,$16,0x3
	sll	$3,$3,2
	.loc 1 589 0
	srl	$4,$16,2
	.loc 1 590 0
	li	$5,7			# 0x7
	.loc 1 591 0
	sll	$5,$5,$3
	lw	$25,%call16(rtl8370_getAsicRegBits)($28)
	addiu	$4,$4,2129
	move	$6,$17
	.loc 1 587 0
	.loc 1 586 0
	bne	$2,$0,$L82
	move	$3,$2

	.loc 1 591 0
	jalr	$25
	nop

	lw	$28,16($sp)
	move	$3,$2
$L82:
	.loc 1 596 0
	lw	$31,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
$LFE13:
	.end	rtl8370_getAsicVlanPortBasedVID
	.align	2
	.globl	rtl8370_getAsicVlanPortAndProtocolBased
	.ent	rtl8370_getAsicVlanPortAndProtocolBased
	.type	rtl8370_getAsicVlanPortAndProtocolBased, @function
rtl8370_getAsicVlanPortAndProtocolBased:
$LFB14:
	.loc 1 785 0
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
	.loc 1 790 0
	sltu	$2,$4,16
	.loc 1 785 0
	move	$16,$4
	move	$19,$6
	move	$18,$5
	.loc 1 793 0
	sltu	$4,$5,4
	.loc 1 791 0
	.loc 1 790 0
	beq	$2,$0,$L88
	li	$3,3			# 0x3

	.loc 1 794 0
	.loc 1 793 0
	beq	$4,$0,$L88
	li	$3,12			# 0xc

	.loc 1 800 0
	sll	$17,$5,3
	.loc 1 801 0
	li	$5,1			# 0x1
	.loc 1 802 0
	lw	$25,%call16(rtl8370_getAsicRegBits)($28)
	sll	$5,$5,$16
	addiu	$4,$17,1800
	addiu	$6,$sp,24
	.loc 1 797 0
	.loc 1 796 0
	beq	$19,$0,$L88
	li	$3,1			# 0x1

	.loc 1 802 0
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 811 0
	addiu	$6,$sp,24
	.loc 1 804 0
	move	$3,$2
	.loc 1 811 0
	.loc 1 803 0
	bne	$2,$0,$L88
	lw	$25,%call16(rtl8370_getAsicRegBits)($28)

	.loc 1 809 0
	li	$4,-1431699456			# 0xffffffffaaaa0000
	ori	$4,$4,0xaaab
	multu	$16,$4
	.loc 1 806 0
	lw	$2,24($sp)
	sw	$2,0($19)
	.loc 1 809 0
	mfhi	$4
	srl	$4,$4,1
	.loc 1 810 0
	sll	$2,$4,1
	addu	$2,$2,$4
	subu	$2,$16,$2
	sll	$5,$2,2
	addu	$5,$5,$2
	.loc 1 809 0
	addu	$4,$17,$4
	.loc 1 811 0
	li	$2,31			# 0x1f
	sll	$5,$2,$5
	jalr	$25
	addiu	$4,$4,1801

	lw	$28,16($sp)
	move	$7,$2
	.loc 1 819 0
	sll	$4,$18,2
	.loc 1 820 0
	andi	$3,$16,0x3
	.loc 1 819 0
	srl	$2,$16,2
	.loc 1 820 0
	sll	$3,$3,2
	.loc 1 819 0
	addu	$4,$4,$2
	.loc 1 820 0
	li	$5,7			# 0x7
	.loc 1 821 0
	sll	$5,$5,$3
	lw	$25,%call16(rtl8370_getAsicRegBits)($28)
	addiu	$4,$4,2133
	addiu	$6,$sp,24
	.loc 1 813 0
	.loc 1 812 0
	bne	$7,$0,$L88
	move	$3,$7

	.loc 1 815 0
	lw	$2,24($sp)
	.loc 1 821 0
	jalr	$25
	sw	$2,4($19)

	.loc 1 823 0
	move	$3,$2
	.loc 1 821 0
	.loc 1 822 0
	bne	$2,$0,$L88
	lw	$28,16($sp)

	.loc 1 825 0
	lw	$2,24($sp)
	.loc 1 826 0
	move	$3,$0
	.loc 1 825 0
	sw	$2,8($19)
$L88:
	.loc 1 827 0
	lw	$31,48($sp)
	lw	$19,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,56

	.set	macro
	.set	reorder
$LFE14:
	.end	rtl8370_getAsicVlanPortAndProtocolBased
	.align	2
	.globl	rtl8370_setAsicVlanFilter
	.ent	rtl8370_setAsicVlanFilter
	.type	rtl8370_setAsicVlanFilter, @function
rtl8370_setAsicVlanFilter:
$LFB15:
	.loc 1 840 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 840 0
	move	$2,$4
	.loc 1 841 0
	sltu	$2,$2,2
	.loc 1 844 0
	move	$6,$4
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)
	li	$4,1960			# 0x7a8
	.loc 1 841 0
	beq	$2,$0,$L97
	move	$5,$0

	.loc 1 844 0
	jr	$25
	nop

$L97:
	.loc 1 845 0
	j	$31
	li	$2,21			# 0x15

	.set	macro
	.set	reorder
$LFE15:
	.end	rtl8370_setAsicVlanFilter
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
	.byte	0x4
	.4byte	$LCFI0-$LFB6
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
$LEFDE8:
$LSFDE10:
	.4byte	$LEFDE10-$LASFDE10
$LASFDE10:
	.4byte	$Lframe0
	.4byte	$LFB7
	.4byte	$LFE7-$LFB7
	.byte	0x4
	.4byte	$LCFI5-$LFB7
	.byte	0xe
	.uleb128 0x40
	.byte	0x4
	.4byte	$LCFI10-$LCFI5
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
$LEFDE10:
$LSFDE12:
	.4byte	$LEFDE12-$LASFDE12
$LASFDE12:
	.4byte	$Lframe0
	.4byte	$LFB8
	.4byte	$LFE8-$LFB8
	.byte	0x4
	.4byte	$LCFI11-$LFB8
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.4byte	$LCFI15-$LCFI11
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
$LEFDE12:
$LSFDE14:
	.4byte	$LEFDE14-$LASFDE14
$LASFDE14:
	.4byte	$Lframe0
	.4byte	$LFB9
	.4byte	$LFE9-$LFB9
	.byte	0x4
	.4byte	$LCFI16-$LFB9
	.byte	0xe
	.uleb128 0x38
	.byte	0x4
	.4byte	$LCFI20-$LCFI16
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
	.byte	0x4
	.4byte	$LCFI21-$LFB11
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	$LCFI22-$LCFI21
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -2
	.align	2
$LEFDE18:
$LSFDE20:
	.4byte	$LEFDE20-$LASFDE20
$LASFDE20:
	.4byte	$Lframe0
	.4byte	$LFB12
	.4byte	$LFE12-$LFB12
	.byte	0x4
	.4byte	$LCFI23-$LFB12
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	$LCFI26-$LCFI23
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
$LEFDE20:
$LSFDE22:
	.4byte	$LEFDE22-$LASFDE22
$LASFDE22:
	.4byte	$Lframe0
	.4byte	$LFB13
	.4byte	$LFE13-$LFB13
	.byte	0x4
	.4byte	$LCFI27-$LFB13
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	$LCFI30-$LCFI27
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
$LEFDE22:
$LSFDE24:
	.4byte	$LEFDE24-$LASFDE24
$LASFDE24:
	.4byte	$Lframe0
	.4byte	$LFB14
	.4byte	$LFE14-$LFB14
	.byte	0x4
	.4byte	$LCFI31-$LFB14
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
$LEFDE24:
$LSFDE26:
	.4byte	$LEFDE26-$LASFDE26
$LASFDE26:
	.4byte	$Lframe0
	.4byte	$LFB15
	.4byte	$LFE15-$LFB15
	.align	2
$LEFDE26:
	.align	0
	.file 2 "/home/IOData/BR6456IN/srcbase/boot/GPL_4100/include/realtek/rtl8370_asicdrv_vlan.h"
	.file 3 "/home/IOData/BR6456IN/srcbase/boot/GPL_4100/include/realtek/rtk_types.h"
	.text
$Letext0:
	.section	.debug_info
	.4byte	0x951
	.2byte	0x2
	.4byte	$Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	$Ldebug_line0
	.4byte	$Letext0
	.4byte	$Ltext0
	.4byte	$LASF84
	.byte	0x1
	.4byte	$LASF85
	.4byte	$LASF86
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
	.byte	0x3
	.byte	0x10
	.4byte	0x3e
	.uleb128 0x2
	.4byte	$LASF2
	.byte	0x4
	.byte	0x7
	.uleb128 0x3
	.4byte	$LASF4
	.byte	0x3
	.byte	0x11
	.4byte	0x50
	.uleb128 0x4
	.ascii	"int\000"
	.byte	0x4
	.byte	0x5
	.uleb128 0x3
	.4byte	$LASF5
	.byte	0x3
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
	.uleb128 0x2
	.4byte	$LASF8
	.byte	0x1
	.byte	0x8
	.uleb128 0x2
	.4byte	$LASF9
	.byte	0x1
	.byte	0x6
	.uleb128 0x2
	.4byte	$LASF2
	.byte	0x4
	.byte	0x7
	.uleb128 0x3
	.4byte	$LASF10
	.byte	0x3
	.byte	0x4e
	.4byte	0x45
	.uleb128 0x5
	.4byte	0x158
	.4byte	$LASF21
	.byte	0x8
	.byte	0x2
	.byte	0x15
	.uleb128 0x6
	.ascii	"mbr\000"
	.byte	0x2
	.byte	0x17
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.byte	0x0
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.ascii	"fid\000"
	.byte	0x2
	.byte	0x19
	.4byte	0x57
	.byte	0x2
	.byte	0xc
	.byte	0x4
	.byte	0x2
	.byte	0x10
	.uleb128 0x2
	.uleb128 0x7
	.4byte	$LASF11
	.byte	0x2
	.byte	0x1a
	.4byte	0x57
	.byte	0x2
	.byte	0x4
	.byte	0x0
	.byte	0x2
	.byte	0x10
	.uleb128 0x2
	.uleb128 0x7
	.4byte	$LASF12
	.byte	0x2
	.byte	0x1c
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0xf
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF13
	.byte	0x2
	.byte	0x1d
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0xe
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF14
	.byte	0x2
	.byte	0x1e
	.4byte	0x57
	.byte	0x2
	.byte	0x3
	.byte	0xb
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF15
	.byte	0x2
	.byte	0x1f
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0xa
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF16
	.byte	0x2
	.byte	0x20
	.4byte	0x57
	.byte	0x2
	.byte	0x8
	.byte	0x2
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF17
	.byte	0x2
	.byte	0x21
	.4byte	0x57
	.byte	0x2
	.byte	0x2
	.byte	0x0
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF18
	.byte	0x2
	.byte	0x23
	.4byte	0x57
	.byte	0x2
	.byte	0xd
	.byte	0x3
	.byte	0x2
	.byte	0x10
	.uleb128 0x6
	.uleb128 0x7
	.4byte	$LASF19
	.byte	0x2
	.byte	0x24
	.4byte	0x57
	.byte	0x2
	.byte	0x3
	.byte	0x0
	.byte	0x2
	.byte	0x10
	.uleb128 0x6
	.byte	0x0
	.uleb128 0x3
	.4byte	$LASF20
	.byte	0x2
	.byte	0x36
	.4byte	0x90
	.uleb128 0x5
	.4byte	0x1ee
	.4byte	$LASF22
	.byte	0x12
	.byte	0x2
	.byte	0x39
	.uleb128 0x8
	.4byte	$LASF18
	.byte	0x2
	.byte	0x3a
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x8
	.4byte	$LASF12
	.byte	0x2
	.byte	0x3b
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x2
	.uleb128 0x9
	.ascii	"mbr\000"
	.byte	0x2
	.byte	0x3c
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x9
	.ascii	"fid\000"
	.byte	0x2
	.byte	0x3d
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x6
	.uleb128 0x8
	.4byte	$LASF11
	.byte	0x2
	.byte	0x3e
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x8
	.uleb128 0x8
	.4byte	$LASF15
	.byte	0x2
	.byte	0x3f
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0xa
	.uleb128 0x8
	.4byte	$LASF16
	.byte	0x2
	.byte	0x40
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0xc
	.uleb128 0x8
	.4byte	$LASF13
	.byte	0x2
	.byte	0x41
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0xe
	.uleb128 0x8
	.4byte	$LASF14
	.byte	0x2
	.byte	0x42
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x10
	.byte	0x0
	.uleb128 0x3
	.4byte	$LASF23
	.byte	0x2
	.byte	0x43
	.4byte	0x163
	.uleb128 0x5
	.4byte	0x2be
	.4byte	$LASF24
	.byte	0x8
	.byte	0x2
	.byte	0x46
	.uleb128 0x9
	.ascii	"mbr\000"
	.byte	0x2
	.byte	0x48
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.ascii	"fid\000"
	.byte	0x2
	.byte	0x4a
	.4byte	0x57
	.byte	0x2
	.byte	0xc
	.byte	0x4
	.byte	0x2
	.byte	0x10
	.uleb128 0x2
	.uleb128 0x7
	.4byte	$LASF11
	.byte	0x2
	.byte	0x4b
	.4byte	0x57
	.byte	0x2
	.byte	0x4
	.byte	0x0
	.byte	0x2
	.byte	0x10
	.uleb128 0x2
	.uleb128 0x7
	.4byte	$LASF12
	.byte	0x2
	.byte	0x4d
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0xf
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF13
	.byte	0x2
	.byte	0x4e
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0xe
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF14
	.byte	0x2
	.byte	0x4f
	.4byte	0x57
	.byte	0x2
	.byte	0x3
	.byte	0xb
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF15
	.byte	0x2
	.byte	0x50
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0xa
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF16
	.byte	0x2
	.byte	0x51
	.4byte	0x57
	.byte	0x2
	.byte	0x8
	.byte	0x2
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF25
	.byte	0x2
	.byte	0x52
	.4byte	0x57
	.byte	0x2
	.byte	0x2
	.byte	0x0
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF26
	.byte	0x2
	.byte	0x54
	.4byte	0x57
	.byte	0x2
	.byte	0xe
	.byte	0x2
	.byte	0x2
	.byte	0x10
	.uleb128 0x6
	.uleb128 0x7
	.4byte	$LASF17
	.byte	0x2
	.byte	0x55
	.4byte	0x57
	.byte	0x2
	.byte	0x2
	.byte	0x0
	.byte	0x2
	.byte	0x10
	.uleb128 0x6
	.byte	0x0
	.uleb128 0x3
	.4byte	$LASF27
	.byte	0x2
	.byte	0x66
	.4byte	0x1f9
	.uleb128 0x5
	.4byte	0x362
	.4byte	$LASF28
	.byte	0x14
	.byte	0x2
	.byte	0x68
	.uleb128 0x9
	.ascii	"vid\000"
	.byte	0x2
	.byte	0x6a
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x8
	.4byte	$LASF12
	.byte	0x2
	.byte	0x6b
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x2
	.uleb128 0x9
	.ascii	"mbr\000"
	.byte	0x2
	.byte	0x6c
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x8
	.4byte	$LASF29
	.byte	0x2
	.byte	0x6d
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x6
	.uleb128 0x9
	.ascii	"fid\000"
	.byte	0x2
	.byte	0x6e
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x8
	.uleb128 0x8
	.4byte	$LASF11
	.byte	0x2
	.byte	0x6f
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0xa
	.uleb128 0x8
	.4byte	$LASF15
	.byte	0x2
	.byte	0x70
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0xc
	.uleb128 0x8
	.4byte	$LASF16
	.byte	0x2
	.byte	0x71
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0xe
	.uleb128 0x8
	.4byte	$LASF13
	.byte	0x2
	.byte	0x72
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x10
	.uleb128 0x8
	.4byte	$LASF14
	.byte	0x2
	.byte	0x73
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x12
	.byte	0x0
	.uleb128 0x3
	.4byte	$LASF30
	.byte	0x2
	.byte	0x75
	.4byte	0x2c9
	.uleb128 0xa
	.4byte	0x38e
	.byte	0x4
	.byte	0x2
	.byte	0x78
	.uleb128 0xb
	.4byte	$LASF31
	.sleb128 0
	.uleb128 0xb
	.4byte	$LASF32
	.sleb128 1
	.uleb128 0xb
	.4byte	$LASF33
	.sleb128 2
	.uleb128 0xb
	.4byte	$LASF34
	.sleb128 3
	.byte	0x0
	.uleb128 0x3
	.4byte	$LASF35
	.byte	0x2
	.byte	0x7d
	.4byte	0x36d
	.uleb128 0xa
	.4byte	0x3c0
	.byte	0x4
	.byte	0x2
	.byte	0x80
	.uleb128 0xb
	.4byte	$LASF36
	.sleb128 0
	.uleb128 0xb
	.4byte	$LASF37
	.sleb128 1
	.uleb128 0xb
	.4byte	$LASF38
	.sleb128 2
	.uleb128 0xb
	.4byte	$LASF39
	.sleb128 3
	.uleb128 0xb
	.4byte	$LASF40
	.sleb128 4
	.byte	0x0
	.uleb128 0x3
	.4byte	$LASF41
	.byte	0x2
	.byte	0x86
	.4byte	0x399
	.uleb128 0xc
	.4byte	0x3fe
	.byte	0xc
	.byte	0x2
	.byte	0xa3
	.uleb128 0x8
	.4byte	$LASF42
	.byte	0x2
	.byte	0xa0
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x8
	.4byte	$LASF43
	.byte	0x2
	.byte	0xa1
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x8
	.4byte	$LASF44
	.byte	0x2
	.byte	0xa2
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x8
	.byte	0x0
	.uleb128 0x3
	.4byte	$LASF45
	.byte	0x2
	.byte	0xa3
	.4byte	0x3cb
	.uleb128 0xd
	.4byte	0x43f
	.byte	0x1
	.4byte	$LASF48
	.byte	0x1
	.byte	0x1f
	.byte	0x1
	.4byte	$LFB2
	.4byte	$LFE2
	.4byte	$LSFDE0
	.byte	0x1
	.byte	0x6d
	.uleb128 0xe
	.4byte	$LASF46
	.byte	0x1
	.byte	0x1e
	.4byte	0x43f
	.byte	0x1
	.byte	0x54
	.uleb128 0xe
	.4byte	$LASF47
	.byte	0x1
	.byte	0x1e
	.4byte	0x445
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0xf
	.byte	0x4
	.4byte	0x1ee
	.uleb128 0xf
	.byte	0x4
	.4byte	0x158
	.uleb128 0xd
	.4byte	0x481
	.byte	0x1
	.4byte	$LASF49
	.byte	0x1
	.byte	0x2c
	.byte	0x1
	.4byte	$LFB3
	.4byte	$LFE3
	.4byte	$LSFDE2
	.byte	0x1
	.byte	0x6d
	.uleb128 0xe
	.4byte	$LASF47
	.byte	0x1
	.byte	0x2b
	.4byte	0x445
	.byte	0x1
	.byte	0x54
	.uleb128 0xe
	.4byte	$LASF46
	.byte	0x1
	.byte	0x2b
	.4byte	0x43f
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0xd
	.4byte	0x4b7
	.byte	0x1
	.4byte	$LASF50
	.byte	0x1
	.byte	0x39
	.byte	0x1
	.4byte	$LFB4
	.4byte	$LFE4
	.4byte	$LSFDE4
	.byte	0x1
	.byte	0x6d
	.uleb128 0xe
	.4byte	$LASF51
	.byte	0x1
	.byte	0x38
	.4byte	0x4b7
	.byte	0x1
	.byte	0x54
	.uleb128 0xe
	.4byte	$LASF52
	.byte	0x1
	.byte	0x38
	.4byte	0x4bd
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0xf
	.byte	0x4
	.4byte	0x362
	.uleb128 0xf
	.byte	0x4
	.4byte	0x2be
	.uleb128 0xd
	.4byte	0x4f9
	.byte	0x1
	.4byte	$LASF53
	.byte	0x1
	.byte	0x47
	.byte	0x1
	.4byte	$LFB5
	.4byte	$LFE5
	.4byte	$LSFDE6
	.byte	0x1
	.byte	0x6d
	.uleb128 0xe
	.4byte	$LASF52
	.byte	0x1
	.byte	0x46
	.4byte	0x4bd
	.byte	0x1
	.byte	0x54
	.uleb128 0xe
	.4byte	$LASF51
	.byte	0x1
	.byte	0x46
	.4byte	0x4b7
	.byte	0x1
	.byte	0x55
	.byte	0x0
	.uleb128 0x10
	.4byte	0x58d
	.byte	0x1
	.4byte	$LASF54
	.byte	0x1
	.byte	0x67
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB6
	.4byte	$LFE6
	.4byte	$LSFDE8
	.byte	0x1
	.byte	0x6d
	.uleb128 0xe
	.4byte	$LASF55
	.byte	0x1
	.byte	0x66
	.4byte	0x33
	.byte	0x1
	.byte	0x62
	.uleb128 0xe
	.4byte	$LASF46
	.byte	0x1
	.byte	0x66
	.4byte	0x43f
	.byte	0x1
	.byte	0x60
	.uleb128 0x11
	.4byte	$LASF56
	.byte	0x1
	.byte	0x68
	.4byte	0x85
	.byte	0x1
	.byte	0x52
	.uleb128 0x12
	.4byte	$LASF57
	.byte	0x1
	.byte	0x69
	.4byte	0x33
	.uleb128 0x12
	.4byte	$LASF58
	.byte	0x1
	.byte	0x6a
	.4byte	0x33
	.uleb128 0x11
	.4byte	$LASF59
	.byte	0x1
	.byte	0x6b
	.4byte	0x58d
	.byte	0x1
	.byte	0x61
	.uleb128 0x11
	.4byte	$LASF60
	.byte	0x1
	.byte	0x6c
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x11
	.4byte	$LASF61
	.byte	0x1
	.byte	0x6d
	.4byte	0x158
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x13
	.byte	0x1
	.4byte	$LASF63
	.byte	0x1
	.byte	0x8e
	.4byte	0x50
	.byte	0x1
	.uleb128 0x14
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.byte	0x4
	.4byte	0x57
	.uleb128 0x10
	.4byte	0x62a
	.byte	0x1
	.4byte	$LASF62
	.byte	0x1
	.byte	0xae
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB7
	.4byte	$LFE7
	.4byte	$LSFDE10
	.byte	0x1
	.byte	0x6d
	.uleb128 0xe
	.4byte	$LASF55
	.byte	0x1
	.byte	0xad
	.4byte	0x33
	.byte	0x1
	.byte	0x62
	.uleb128 0xe
	.4byte	$LASF46
	.byte	0x1
	.byte	0xad
	.4byte	0x43f
	.byte	0x1
	.byte	0x63
	.uleb128 0x11
	.4byte	$LASF56
	.byte	0x1
	.byte	0xaf
	.4byte	0x85
	.byte	0x1
	.byte	0x52
	.uleb128 0x11
	.4byte	$LASF60
	.byte	0x1
	.byte	0xb0
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x12
	.4byte	$LASF57
	.byte	0x1
	.byte	0xb1
	.4byte	0x33
	.uleb128 0x11
	.4byte	$LASF58
	.byte	0x1
	.byte	0xb2
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 32
	.uleb128 0x11
	.4byte	$LASF59
	.byte	0x1
	.byte	0xb3
	.4byte	0x58d
	.byte	0x1
	.byte	0x61
	.uleb128 0x11
	.4byte	$LASF61
	.byte	0x1
	.byte	0xb4
	.4byte	0x158
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x13
	.byte	0x1
	.4byte	$LASF63
	.byte	0x1
	.byte	0x8e
	.4byte	0x50
	.byte	0x1
	.uleb128 0x14
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.4byte	0x6a6
	.byte	0x1
	.4byte	$LASF64
	.byte	0x1
	.byte	0xe0
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB8
	.4byte	$LFE8
	.4byte	$LSFDE12
	.byte	0x1
	.byte	0x6d
	.uleb128 0xe
	.4byte	$LASF65
	.byte	0x1
	.byte	0xdf
	.4byte	0x4b7
	.byte	0x1
	.byte	0x61
	.uleb128 0x11
	.4byte	$LASF66
	.byte	0x1
	.byte	0xe1
	.4byte	0x2be
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x11
	.4byte	$LASF60
	.byte	0x1
	.byte	0xe2
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x11
	.4byte	$LASF59
	.byte	0x1
	.byte	0xe3
	.4byte	0x58d
	.byte	0x1
	.byte	0x62
	.uleb128 0x11
	.4byte	$LASF56
	.byte	0x1
	.byte	0xe4
	.4byte	0x85
	.byte	0x1
	.byte	0x52
	.uleb128 0x12
	.4byte	$LASF58
	.byte	0x1
	.byte	0xe5
	.4byte	0x33
	.uleb128 0x13
	.byte	0x1
	.4byte	$LASF63
	.byte	0x1
	.byte	0x8e
	.4byte	0x50
	.byte	0x1
	.uleb128 0x14
	.byte	0x0
	.byte	0x0
	.uleb128 0x15
	.4byte	0x71d
	.byte	0x1
	.4byte	$LASF67
	.byte	0x1
	.2byte	0x12e
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB9
	.4byte	$LFE9
	.4byte	$LSFDE14
	.byte	0x1
	.byte	0x6d
	.uleb128 0x16
	.4byte	$LASF65
	.byte	0x1
	.2byte	0x12d
	.4byte	0x4b7
	.byte	0x1
	.byte	0x62
	.uleb128 0x17
	.4byte	$LASF66
	.byte	0x1
	.2byte	0x12f
	.4byte	0x2be
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x17
	.4byte	$LASF60
	.byte	0x1
	.2byte	0x130
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x17
	.4byte	$LASF59
	.byte	0x1
	.2byte	0x131
	.4byte	0x58d
	.byte	0x1
	.byte	0x61
	.uleb128 0x17
	.4byte	$LASF56
	.byte	0x1
	.2byte	0x132
	.4byte	0x85
	.byte	0x1
	.byte	0x52
	.uleb128 0x17
	.4byte	$LASF58
	.byte	0x1
	.2byte	0x133
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 32
	.byte	0x0
	.uleb128 0x15
	.4byte	0x772
	.byte	0x1
	.4byte	$LASF68
	.byte	0x1
	.2byte	0x167
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB10
	.4byte	$LFE10
	.4byte	$LSFDE16
	.byte	0x1
	.byte	0x6d
	.uleb128 0x16
	.4byte	$LASF69
	.byte	0x1
	.2byte	0x166
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x16
	.4byte	$LASF70
	.byte	0x1
	.2byte	0x166
	.4byte	0x38e
	.byte	0x1
	.byte	0x56
	.uleb128 0x18
	.4byte	$LASF57
	.byte	0x1
	.2byte	0x168
	.4byte	0x33
	.uleb128 0x18
	.4byte	$LASF71
	.byte	0x1
	.2byte	0x168
	.4byte	0x33
	.byte	0x0
	.uleb128 0x15
	.4byte	0x7bb
	.byte	0x1
	.4byte	$LASF72
	.byte	0x1
	.2byte	0x1cf
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB11
	.4byte	$LFE11
	.4byte	$LSFDE18
	.byte	0x1
	.byte	0x6d
	.uleb128 0x16
	.4byte	$LASF69
	.byte	0x1
	.2byte	0x1ce
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x16
	.4byte	$LASF73
	.byte	0x1
	.2byte	0x1ce
	.4byte	0x3c0
	.byte	0x1
	.byte	0x55
	.uleb128 0x18
	.4byte	$LASF56
	.byte	0x1
	.2byte	0x1d0
	.4byte	0x85
	.byte	0x0
	.uleb128 0x15
	.4byte	0x82c
	.byte	0x1
	.4byte	$LASF74
	.byte	0x1
	.2byte	0x20d
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB12
	.4byte	$LFE12
	.4byte	$LSFDE20
	.byte	0x1
	.byte	0x6d
	.uleb128 0x16
	.4byte	$LASF69
	.byte	0x1
	.2byte	0x20c
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x16
	.4byte	$LASF55
	.byte	0x1
	.2byte	0x20c
	.4byte	0x33
	.byte	0x1
	.byte	0x58
	.uleb128 0x19
	.ascii	"pri\000"
	.byte	0x1
	.2byte	0x20c
	.4byte	0x33
	.byte	0x1
	.byte	0x61
	.uleb128 0x18
	.4byte	$LASF57
	.byte	0x1
	.2byte	0x20e
	.4byte	0x33
	.uleb128 0x18
	.4byte	$LASF71
	.byte	0x1
	.2byte	0x20e
	.4byte	0x33
	.uleb128 0x17
	.4byte	$LASF56
	.byte	0x1
	.2byte	0x20f
	.4byte	0x85
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x15
	.4byte	0x89d
	.byte	0x1
	.4byte	$LASF75
	.byte	0x1
	.2byte	0x23a
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB13
	.4byte	$LFE13
	.4byte	$LSFDE22
	.byte	0x1
	.byte	0x6d
	.uleb128 0x16
	.4byte	$LASF69
	.byte	0x1
	.2byte	0x239
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x16
	.4byte	$LASF76
	.byte	0x1
	.2byte	0x239
	.4byte	0x89d
	.byte	0x1
	.byte	0x57
	.uleb128 0x16
	.4byte	$LASF77
	.byte	0x1
	.2byte	0x239
	.4byte	0x89d
	.byte	0x1
	.byte	0x61
	.uleb128 0x18
	.4byte	$LASF57
	.byte	0x1
	.2byte	0x23b
	.4byte	0x33
	.uleb128 0x18
	.4byte	$LASF71
	.byte	0x1
	.2byte	0x23b
	.4byte	0x33
	.uleb128 0x17
	.4byte	$LASF56
	.byte	0x1
	.2byte	0x23c
	.4byte	0x85
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0xf
	.byte	0x4
	.4byte	0x33
	.uleb128 0x15
	.4byte	0x923
	.byte	0x1
	.4byte	$LASF78
	.byte	0x1
	.2byte	0x311
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB14
	.4byte	$LFE14
	.4byte	$LSFDE24
	.byte	0x1
	.byte	0x6d
	.uleb128 0x16
	.4byte	$LASF69
	.byte	0x1
	.2byte	0x310
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x16
	.4byte	$LASF55
	.byte	0x1
	.2byte	0x310
	.4byte	0x33
	.byte	0x1
	.byte	0x62
	.uleb128 0x16
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x310
	.4byte	0x923
	.byte	0x1
	.byte	0x63
	.uleb128 0x18
	.4byte	$LASF80
	.byte	0x1
	.2byte	0x312
	.4byte	0x33
	.uleb128 0x18
	.4byte	$LASF71
	.byte	0x1
	.2byte	0x312
	.4byte	0x33
	.uleb128 0x17
	.4byte	$LASF81
	.byte	0x1
	.2byte	0x312
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x17
	.4byte	$LASF56
	.byte	0x1
	.2byte	0x313
	.4byte	0x85
	.byte	0x1
	.byte	0x57
	.byte	0x0
	.uleb128 0xf
	.byte	0x4
	.4byte	0x3fe
	.uleb128 0x1a
	.byte	0x1
	.4byte	$LASF82
	.byte	0x1
	.2byte	0x348
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB15
	.4byte	$LFE15
	.4byte	$LSFDE26
	.byte	0x1
	.byte	0x6d
	.uleb128 0x16
	.4byte	$LASF83
	.byte	0x1
	.2byte	0x347
	.4byte	0x33
	.byte	0x1
	.byte	0x52
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
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xd
	.uleb128 0xb
	.uleb128 0xc
	.uleb128 0xb
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xd
	.uleb128 0xb
	.uleb128 0xc
	.uleb128 0xb
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0x28
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xd
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
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
	.uleb128 0xe
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
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.uleb128 0x18
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x15
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
	.uleb128 0x16
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
	.uleb128 0x17
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
	.uleb128 0x18
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
	.uleb128 0x19
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
	.uleb128 0x1a
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
	.4byte	0x1ea
	.2byte	0x2
	.4byte	$Ldebug_info0
	.4byte	0x955
	.4byte	0x409
	.ascii	"_rtl8370_VlanMCStUser2Smi\000"
	.4byte	0x44b
	.ascii	"_rtl8370_VlanMCStSmi2User\000"
	.4byte	0x481
	.ascii	"_rtl8370_Vlan4kStUser2Smi\000"
	.4byte	0x4c3
	.ascii	"_rtl8370_Vlan4kStSmi2User\000"
	.4byte	0x4f9
	.ascii	"rtl8370_setAsicVlanMemberConfig\000"
	.4byte	0x593
	.ascii	"rtl8370_getAsicVlanMemberConfig\000"
	.4byte	0x62a
	.ascii	"rtl8370_setAsicVlan4kEntry\000"
	.4byte	0x6a6
	.ascii	"rtl8370_getAsicVlan4kEntry\000"
	.4byte	0x71d
	.ascii	"rtl8370_setAsicVlanAccpetFrameType\000"
	.4byte	0x772
	.ascii	"rtl8370_setAsicVlanEgressTagMode\000"
	.4byte	0x7bb
	.ascii	"rtl8370_setAsicVlanPortBasedVID\000"
	.4byte	0x82c
	.ascii	"rtl8370_getAsicVlanPortBasedVID\000"
	.4byte	0x8a3
	.ascii	"rtl8370_getAsicVlanPortAndProtocolBased\000"
	.4byte	0x929
	.ascii	"rtl8370_setAsicVlanFilter\000"
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
$LASF5:
	.ascii	"uint16\000"
$LASF8:
	.ascii	"unsigned char\000"
$LASF25:
	.ascii	"untag1\000"
$LASF26:
	.ascii	"untag2\000"
$LASF24:
	.ascii	"VLANTABLE\000"
$LASF18:
	.ascii	"evid\000"
$LASF72:
	.ascii	"rtl8370_setAsicVlanEgressTagMode\000"
$LASF10:
	.ascii	"ret_t\000"
$LASF68:
	.ascii	"rtl8370_setAsicVlanAccpetFrameType\000"
$LASF3:
	.ascii	"uint32\000"
$LASF56:
	.ascii	"retVal\000"
$LASF0:
	.ascii	"long long unsigned int\000"
$LASF76:
	.ascii	"ptr_index\000"
$LASF30:
	.ascii	"rtl8370_user_vlan4kentry\000"
$LASF1:
	.ascii	"long long int\000"
$LASF62:
	.ascii	"rtl8370_getAsicVlanMemberConfig\000"
$LASF11:
	.ascii	"msti\000"
$LASF78:
	.ascii	"rtl8370_getAsicVlanPortAndProtocolBased\000"
$LASF75:
	.ascii	"rtl8370_getAsicVlanPortBasedVID\000"
$LASF4:
	.ascii	"int32\000"
$LASF29:
	.ascii	"untag\000"
$LASF73:
	.ascii	"tag_mode\000"
$LASF16:
	.ascii	"meteridx\000"
$LASF40:
	.ascii	"EG_TAG_MODE_MAX_BOUND\000"
$LASF67:
	.ascii	"rtl8370_getAsicVlan4kEntry\000"
$LASF38:
	.ascii	"EG_TAG_MODE_PRI_TAG\000"
$LASF79:
	.ascii	"ptr_ppbcfg\000"
$LASF64:
	.ascii	"rtl8370_setAsicVlan4kEntry\000"
$LASF54:
	.ascii	"rtl8370_setAsicVlanMemberConfig\000"
$LASF80:
	.ascii	"reg_addr\000"
$LASF86:
	.ascii	"/home/IOData/BR6456IN/srcbase/boot/realtek_8370\000"
$LASF85:
	.ascii	"rtl8370_asicdrv_vlan.c\000"
$LASF66:
	.ascii	"vlan_4k_entry\000"
$LASF46:
	.ascii	"ptr_vlancfg\000"
$LASF43:
	.ascii	"vlan_idx\000"
$LASF20:
	.ascii	"rtl8370_vlanconfigsmi\000"
$LASF47:
	.ascii	"ptr_smi_vlancfg\000"
$LASF59:
	.ascii	"tableAddr\000"
$LASF2:
	.ascii	"unsigned int\000"
$LASF82:
	.ascii	"rtl8370_setAsicVlanFilter\000"
$LASF22:
	.ascii	"VLANCONFIGUSER\000"
$LASF12:
	.ascii	"lurep\000"
$LASF36:
	.ascii	"EG_TAG_MODE_ORI\000"
$LASF13:
	.ascii	"vbpen\000"
$LASF32:
	.ascii	"FRAME_TYPE_TAGGED_ONLY\000"
$LASF83:
	.ascii	"enabled\000"
$LASF6:
	.ascii	"short unsigned int\000"
$LASF69:
	.ascii	"port\000"
$LASF44:
	.ascii	"priority\000"
$LASF33:
	.ascii	"FRAME_TYPE_UNTAGGED_ONLY\000"
$LASF7:
	.ascii	"short int\000"
$LASF74:
	.ascii	"rtl8370_setAsicVlanPortBasedVID\000"
$LASF39:
	.ascii	"EG_TAG_MODE_REAL_KEEP\000"
$LASF23:
	.ascii	"rtl8370_vlanconfiguser\000"
$LASF31:
	.ascii	"FRAME_TYPE_BOTH\000"
$LASF60:
	.ascii	"page_idx\000"
$LASF61:
	.ascii	"smi_vlancfg\000"
$LASF37:
	.ascii	"EG_TAG_MODE_KEEP\000"
$LASF27:
	.ascii	"rtl8370_vlan4kentrysmi\000"
$LASF34:
	.ascii	"FRAME_TYPE_MAX_BOUND\000"
$LASF28:
	.ascii	"USER_VLANTABLE\000"
$LASF21:
	.ascii	"VLANCONFIGSMI\000"
$LASF50:
	.ascii	"_rtl8370_Vlan4kStUser2Smi\000"
$LASF35:
	.ascii	"rtl8370_accframetype\000"
$LASF77:
	.ascii	"ptr_pri\000"
$LASF51:
	.ascii	"ptr_user_vlan4kEntry\000"
$LASF81:
	.ascii	"bit_value\000"
$LASF41:
	.ascii	"rtl8370_egtagmode\000"
$LASF42:
	.ascii	"valid\000"
$LASF9:
	.ascii	"char\000"
$LASF58:
	.ascii	"regData\000"
$LASF48:
	.ascii	"_rtl8370_VlanMCStUser2Smi\000"
$LASF53:
	.ascii	"_rtl8370_Vlan4kStSmi2User\000"
$LASF57:
	.ascii	"regAddr\000"
$LASF63:
	.ascii	"memset\000"
$LASF49:
	.ascii	"_rtl8370_VlanMCStSmi2User\000"
$LASF84:
	.ascii	"GNU C 3.4.2 -g\000"
$LASF19:
	.ascii	"reserved2\000"
$LASF15:
	.ascii	"envlanpol\000"
$LASF45:
	.ascii	"rtl8370_protocolvlancfg\000"
$LASF71:
	.ascii	"bit_mask\000"
$LASF52:
	.ascii	"ptr_smi_vlan4kEntry\000"
$LASF55:
	.ascii	"index\000"
$LASF17:
	.ascii	"reserved1\000"
$LASF70:
	.ascii	"frame_type\000"
$LASF14:
	.ascii	"vbpri\000"
$LASF65:
	.ascii	"ptr_vlan4kEntry\000"
	.ident	"GCC: (GNU) 3.4.2"
