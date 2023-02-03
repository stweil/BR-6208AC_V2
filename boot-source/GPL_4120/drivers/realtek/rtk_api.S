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
	.globl	filter_templateField
	.rdata
	.align	2
	.type	filter_templateField, @object
	.size	filter_templateField, 140
filter_templateField:
	.word	2
	.word	1
	.word	0
	.word	5
	.word	4
	.word	3
	.word	6
	.word	10
	.word	9
	.word	12
	.word	11
	.word	14
	.word	13
	.word	8
	.word	29
	.word	28
	.word	26
	.word	25
	.word	24
	.word	23
	.word	22
	.word	37
	.word	36
	.word	34
	.word	33
	.word	32
	.word	31
	.word	30
	.word	15
	.word	16
	.word	17
	.word	20
	.word	21
	.word	38
	.word	7
	.text
	.align	2
	.globl	rtk_port_rgmiiDelayExt0_set
	.ent	rtk_port_rgmiiDelayExt0_set
	.type	rtk_port_rgmiiDelayExt0_set, @function
rtk_port_rgmiiDelayExt0_set:
$LFB4:
	.file 1 "rtk_api.c"
	.loc 1 4449 0
	.frame	$sp,48,$31		# vars= 8, regs= 3/0, args= 16, gp= 8
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-48
$LCFI0:
	sw	$31,40($sp)
$LCFI1:
	sw	$17,36($sp)
$LCFI2:
	sw	$16,32($sp)
$LCFI3:
	.cprestore	16
	.loc 1 4453 0
	sltu	$2,$4,2
	.loc 1 4449 0
	move	$17,$4
	.loc 1 4454 0
	li	$6,1			# 0x1
	.loc 1 4449 0
	move	$16,$5
	.loc 1 4453 0
	beq	$2,$0,$L1
	sltu	$3,$5,8

	.loc 1 4456 0
	lw	$25,%call16(rtl8370_getAsicReg)($28)
	li	$4,4870			# 0x1306
	.loc 1 4453 0
	beq	$3,$0,$L1
	addiu	$5,$sp,24

	.loc 1 4456 0
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 4459 0
	sll	$3,$17,3
	andi	$3,$3,0x8
	andi	$7,$16,0x7
	.loc 1 4461 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,4870			# 0x1306
	.loc 1 4457 0
	.loc 1 4456 0
	bne	$2,$0,$L1
	move	$6,$2

	.loc 1 4459 0
	lw	$5,24($sp)
	andi	$5,$5,0xfff0
	or	$5,$5,$3
	or	$5,$5,$7
	.loc 1 4461 0
	jalr	$25
	sw	$5,24($sp)

	lw	$28,16($sp)
	move	$6,$2
$L1:
	.loc 1 4465 0
	lw	$31,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$6
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
$LFE4:
	.end	rtk_port_rgmiiDelayExt0_set
	.align	2
	.globl	rtk_port_rgmiiDelayExt1_set
	.ent	rtk_port_rgmiiDelayExt1_set
	.type	rtk_port_rgmiiDelayExt1_set, @function
rtk_port_rgmiiDelayExt1_set:
$LFB5:
	.loc 1 4525 0
	.frame	$sp,48,$31		# vars= 8, regs= 3/0, args= 16, gp= 8
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-48
$LCFI4:
	sw	$31,40($sp)
$LCFI5:
	sw	$17,36($sp)
$LCFI6:
	sw	$16,32($sp)
$LCFI7:
	.cprestore	16
	.loc 1 4529 0
	sltu	$2,$4,2
	.loc 1 4525 0
	move	$17,$4
	.loc 1 4530 0
	li	$6,1			# 0x1
	.loc 1 4525 0
	move	$16,$5
	.loc 1 4529 0
	beq	$2,$0,$L6
	sltu	$3,$5,8

	.loc 1 4532 0
	lw	$25,%call16(rtl8370_getAsicReg)($28)
	li	$4,4871			# 0x1307
	.loc 1 4529 0
	beq	$3,$0,$L6
	addiu	$5,$sp,24

	.loc 1 4532 0
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 4535 0
	sll	$3,$17,3
	andi	$3,$3,0x8
	andi	$7,$16,0x7
	.loc 1 4537 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,4871			# 0x1307
	.loc 1 4533 0
	.loc 1 4532 0
	bne	$2,$0,$L6
	move	$6,$2

	.loc 1 4535 0
	lw	$5,24($sp)
	andi	$5,$5,0xfff0
	or	$5,$5,$3
	or	$5,$5,$7
	.loc 1 4537 0
	jalr	$25
	sw	$5,24($sp)

	lw	$28,16($sp)
	move	$6,$2
$L6:
	.loc 1 4541 0
	lw	$31,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$6
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
$LFE5:
	.end	rtk_port_rgmiiDelayExt1_set
	.align	2
	.globl	rtk_vlan_init
	.ent	rtk_vlan_init
	.type	rtk_vlan_init, @function
rtk_vlan_init:
$LFB6:
	.loc 1 5175 0
	.frame	$sp,88,$31		# vars= 48, regs= 4/0, args= 16, gp= 8
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-88
$LCFI8:
	sw	$31,84($sp)
$LCFI9:
	sw	$18,80($sp)
$LCFI10:
	sw	$17,76($sp)
$LCFI11:
	sw	$16,72($sp)
$LCFI12:
	.cprestore	16
	.loc 1 5183 0
	move	$16,$0
	.loc 1 5194 0
	lw	$25,%call16(rtl8370_setAsicVlanMemberConfig)($28)
$L33:
	addiu	$17,$sp,48
	move	$4,$16
	.loc 1 5185 0
	sh	$0,48($sp)
	.loc 1 5186 0
	sh	$0,50($sp)
	.loc 1 5187 0
	sh	$0,52($sp)
	.loc 1 5188 0
	sh	$0,56($sp)
	.loc 1 5189 0
	sh	$0,54($sp)
	.loc 1 5190 0
	sh	$0,60($sp)
	.loc 1 5191 0
	sh	$0,58($sp)
	.loc 1 5192 0
	sh	$0,62($sp)
	.loc 1 5193 0
	sh	$0,64($sp)
	.loc 1 5194 0
	jalr	$25
	move	$5,$17

	.loc 1 5183 0
	addiu	$16,$16,1
	.loc 1 5194 0
	lw	$28,16($sp)
	.loc 1 5183 0
	.loc 1 5194 0
	bne	$2,$0,$L32
	sltu	$3,$16,32

	.loc 1 5183 0
	bne	$3,$0,$L33
	lw	$25,%call16(rtl8370_setAsicVlanMemberConfig)($28)

	.loc 1 5200 0
	lw	$25,%call16(memset)($28)
	addiu	$4,$sp,24
	move	$5,$0
	jalr	$25
	li	$6,20			# 0x14

	lw	$28,16($sp)
	.loc 1 5202 0
	li	$16,1023
	.loc 1 5201 0
	li	$18,1
	.loc 1 5205 0
	lw	$25,%call16(rtl8370_setAsicVlan4kEntry)($28)
	.loc 1 5201 0
	sh	$18,24($sp)
	.loc 1 5202 0
	sh	$16,28($sp)
	.loc 1 5203 0
	sh	$16,30($sp)
	.loc 1 5204 0
	sh	$0,32($sp)
	.loc 1 5205 0
	jalr	$25
	addiu	$4,$sp,24

	.loc 1 5206 0
	move	$3,$2
	.loc 1 5205 0
	bne	$2,$0,$L11
	lw	$28,16($sp)

	.loc 1 5210 0
	lw	$25,%call16(memset)($28)
	move	$4,$17
	move	$5,$0
	jalr	$25
	li	$6,18			# 0x12

	lw	$28,16($sp)
	.loc 1 5211 0
	sh	$18,48($sp)
	.loc 1 5212 0
	sh	$16,52($sp)
	.loc 1 5214 0
	lw	$25,%call16(rtl8370_setAsicVlanMemberConfig)($28)
	.loc 1 5213 0
	sh	$0,54($sp)
	.loc 1 5214 0
	move	$5,$17
	jalr	$25
	move	$4,$0

	.loc 1 5215 0
	move	$3,$2
	.loc 1 5214 0
	bne	$2,$0,$L11
	lw	$28,16($sp)

	.loc 1 5218 0
	move	$16,$0
	.loc 1 5220 0
	lw	$25,%call16(rtl8370_setAsicVlanPortBasedVID)($28)
$L34:
	move	$4,$16
	move	$5,$0
	jalr	$25
	move	$6,$0

	lw	$28,16($sp)
	.loc 1 5222 0
	move	$4,$16
	move	$5,$0
	lw	$25,%call16(rtl8370_setAsicVlanEgressTagMode)($28)
	.loc 1 5218 0
	.loc 1 5220 0
	bne	$2,$0,$L32
	addiu	$16,$16,1

	.loc 1 5222 0
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 5218 0
	.loc 1 5222 0
	bne	$2,$0,$L32
	sltu	$3,$16,10

	.loc 1 5220 0
	.loc 1 5218 0
	bne	$3,$0,$L34
	lw	$25,%call16(rtl8370_setAsicVlanPortBasedVID)($28)

	.loc 1 5227 0
	lw	$25,%call16(rtl8370_setAsicVlanFilter)($28)
	jalr	$25
	li	$4,1			# 0x1

	.loc 1 5228 0
	move	$3,$2
	.loc 1 5227 0
	bne	$2,$0,$L11
	lw	$28,16($sp)

	.loc 1 5230 0
	b	$L25
	move	$3,$0

$L32:
	move	$3,$2
$L25:
$L11:
	.loc 1 5231 0
	lw	$31,84($sp)
	lw	$18,80($sp)
	lw	$17,76($sp)
	lw	$16,72($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,88

	.set	macro
	.set	reorder
$LFE6:
	.end	rtk_vlan_init
	.align	2
	.globl	rtk_vlan_set
	.ent	rtk_vlan_set
	.type	rtk_vlan_set, @function
rtk_vlan_set:
$LFB7:
	.loc 1 5259 0
	.frame	$sp,72,$31		# vars= 24, regs= 5/0, args= 16, gp= 8
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-72
$LCFI13:
	sw	$31,64($sp)
$LCFI14:
	sw	$19,60($sp)
$LCFI15:
	sw	$18,56($sp)
$LCFI16:
	sw	$17,52($sp)
$LCFI17:
	sw	$16,48($sp)
$LCFI18:
	.cprestore	16
	.loc 1 5264 0
	sltu	$2,$4,4096
	.loc 1 5259 0
	move	$17,$5
	move	$18,$4
	move	$19,$7
	.loc 1 5267 0
	sltu	$5,$5,1024
	.loc 1 5259 0
	move	$16,$6
	.loc 1 5265 0
	.loc 1 5264 0
	beq	$2,$0,$L35
	li	$3,65536			# 0x10000

	.loc 1 5268 0
	li	$2,65536			# 0x10000
	ori	$3,$2,0x6
	.loc 1 5270 0
	.loc 1 5267 0
	beq	$5,$0,$L35
	sltu	$4,$6,1024

	.loc 1 5271 0
	li	$2,65536			# 0x10000
	ori	$3,$2,0x6
	.loc 1 5273 0
	.loc 1 5270 0
	beq	$4,$0,$L35
	sltu	$7,$7,4096

	.loc 1 5274 0
	li	$2,393216			# 0x60000
	.loc 1 5277 0
	lw	$25,%call16(memset)($28)
	addiu	$4,$sp,24
	move	$5,$0
	li	$6,20			# 0x14
	.loc 1 5274 0
	.loc 1 5273 0
	beq	$7,$0,$L35
	ori	$3,$2,0x7

	.loc 1 5277 0
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 5278 0
	sh	$18,24($sp)
	.loc 1 5279 0
	sh	$17,28($sp)
	.loc 1 5282 0
	lw	$25,%call16(rtl8370_setAsicVlan4kEntry)($28)
	.loc 1 5280 0
	sh	$16,30($sp)
	.loc 1 5281 0
	sh	$19,32($sp)
	.loc 1 5282 0
	jalr	$25
	addiu	$4,$sp,24

	lw	$28,16($sp)
	move	$3,$2
$L35:
	.loc 1 5286 0
	lw	$31,64($sp)
	lw	$19,60($sp)
	lw	$18,56($sp)
	lw	$17,52($sp)
	lw	$16,48($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,72

	.set	macro
	.set	reorder
$LFE7:
	.end	rtk_vlan_set
	.align	2
	.globl	rtk_vlan_portPvid_set
	.ent	rtk_vlan_portPvid_set
	.type	rtk_vlan_portPvid_set, @function
rtk_vlan_portPvid_set:
$LFB8:
	.loc 1 5353 0
	.frame	$sp,144,$31		# vars= 80, regs= 9/0, args= 16, gp= 8
	.mask	0x80ff0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-144
$LCFI19:
	sw	$31,136($sp)
$LCFI20:
	sw	$23,132($sp)
$LCFI21:
	sw	$22,128($sp)
$LCFI22:
	sw	$21,124($sp)
$LCFI23:
	sw	$20,120($sp)
$LCFI24:
	sw	$19,116($sp)
$LCFI25:
	sw	$18,112($sp)
$LCFI26:
	sw	$17,108($sp)
$LCFI27:
	sw	$16,104($sp)
$LCFI28:
	.cprestore	16
	.loc 1 5365 0
	sltu	$2,$4,10
	.loc 1 5353 0
	move	$20,$4
	move	$19,$5
	move	$22,$6
	.loc 1 5366 0
	.loc 1 5365 0
	beq	$2,$0,$L41
	li	$3,3			# 0x3

	.loc 1 5369 0
	sltu	$2,$5,4096
	.loc 1 5370 0
	.loc 1 5369 0
	beq	$2,$0,$L41
	li	$3,65536			# 0x10000

	.loc 1 5373 0
	sltu	$2,$6,8
	.loc 1 5377 0
	.loc 1 5373 0
	bne	$2,$0,$L44
	li	$17,65535			# 0xffff

	.loc 1 5374 0
	li	$2,65536			# 0x10000
	b	$L41
	ori	$3,$2,0x1

$L44:
	.loc 1 5379 0
	li	$16,31			# 0x1f
	.loc 1 5381 0
	lw	$25,%call16(rtl8370_getAsicVlanMemberConfig)($28)
$L99:
	addiu	$21,$sp,48
	move	$4,$16
	jalr	$25
	move	$5,$21

	bne	$2,$0,$L96
	lw	$28,16($sp)

	.loc 1 5384 0
	lhu	$2,48($sp)
	.loc 1 5484 0
	.loc 1 5384 0
	beq	$19,$2,$L98
	lw	$25,%call16(rtl8370_setAsicVlanPortBasedVID)($28)

	.loc 1 5391 0
	bne	$2,$0,$L47
	lhu	$2,52($sp)

	.loc 1 5393 0
	movz	$17,$16,$2
$L47:
	.loc 1 5379 0
	addiu	$16,$16,-1
	.loc 1 5381 0
	.loc 1 5379 0
	bgez	$16,$L99
	lw	$25,%call16(rtl8370_getAsicVlanMemberConfig)($28)

	.loc 1 5405 0
	li	$2,65535			# 0xffff
	.loc 1 5408 0
	.loc 1 5405 0
	beq	$17,$2,$L54
	lw	$25,%call16(rtl8370_getAsicVlan4kEntry)($28)

	.loc 1 5407 0
	sh	$19,24($sp)
	.loc 1 5408 0
	jalr	$25
	addiu	$4,$sp,24

	.loc 1 5409 0
	move	$3,$2
	.loc 1 5408 0
	bne	$2,$0,$L41
	lw	$28,16($sp)

	.loc 1 5412 0
	lhu	$2,28($sp)
	.loc 1 5419 0
	lw	$25,%call16(rtl8370_setAsicVlanMemberConfig)($28)
	.loc 1 5411 0
	sh	$19,48($sp)
	.loc 1 5412 0
	sh	$2,52($sp)
	.loc 1 5413 0
	lhu	$2,32($sp)
	.loc 1 5419 0
	move	$5,$21
	move	$4,$17
	.loc 1 5413 0
	sh	$2,54($sp)
	.loc 1 5414 0
	lhu	$2,34($sp)
	sh	$2,56($sp)
	.loc 1 5415 0
	lhu	$2,38($sp)
	sh	$2,60($sp)
	.loc 1 5416 0
	lhu	$2,36($sp)
	sh	$2,58($sp)
	.loc 1 5417 0
	lhu	$2,26($sp)
	.loc 1 5419 0
	jalr	$25
	sh	$2,50($sp)

	.loc 1 5420 0
	move	$3,$2
	.loc 1 5419 0
	bne	$2,$0,$L41
	lw	$28,16($sp)

	.loc 1 5422 0
	lw	$25,%call16(rtl8370_setAsicVlanPortBasedVID)($28)
	move	$4,$20
	move	$5,$17
	jalr	$25
	move	$6,$22

	b	$L96
	lw	$28,16($sp)

$L54:
	.loc 1 5428 0
	lw	$25,%call16(rtl8370_getAsic1xGuestVidx)($28)
	jalr	$25
	addiu	$4,$sp,88

	.loc 1 5429 0
	move	$3,$2
	.loc 1 5428 0
	bne	$2,$0,$L41
	lw	$28,16($sp)

	.loc 1 5432 0
	b	$L58
	move	$16,$0

$L96:
	.loc 1 5485 0
	b	$L41
	move	$3,$2

$L58:
	.loc 1 5434 0
	move	$23,$0
$L103:
	.loc 1 5436 0
	move	$18,$0
	.loc 1 5438 0
	lw	$25,%call16(rtl8370_getAsicVlanPortBasedVID)($28)
$L101:
	move	$4,$18
	addiu	$5,$sp,92
	jalr	$25
	addiu	$6,$sp,96

	bne	$2,$0,$L96
	lw	$28,16($sp)

	.loc 1 5441 0
	lw	$2,92($sp)
	.loc 1 5447 0
	.loc 1 5441 0
	beq	$16,$2,$L61
	lw	$2,88($sp)

	.loc 1 5458 0
	.loc 1 5447 0
	bne	$16,$2,$L75
	move	$17,$0

	.loc 1 5449 0
	lw	$25,%call16(rtl8370_getAsic1xProcConfig)($28)
	move	$4,$18
	jalr	$25
	addiu	$5,$sp,100

	bne	$2,$0,$L96
	lw	$28,16($sp)

	.loc 1 5451 0
	lw	$3,100($sp)
	li	$2,2			# 0x2
	.loc 1 5458 0
	.loc 1 5451 0
	beq	$3,$2,$L61
	move	$17,$0

$L75:
	.loc 1 5460 0
	lw	$25,%call16(rtl8370_getAsicVlanPortAndProtocolBased)($28)
$L100:
	move	$5,$17
	move	$4,$20
	jalr	$25
	addiu	$6,$sp,72

	.loc 1 5458 0
	addiu	$17,$17,1
	.loc 1 5460 0
	lw	$28,16($sp)
	.loc 1 5458 0
	.loc 1 5460 0
	bne	$2,$0,$L96
	sltu	$4,$17,4

	.loc 1 5462 0
	lw	$3,72($sp)
	li	$2,1			# 0x1
	bne	$3,$2,$L72
	lw	$2,76($sp)

	bne	$2,$16,$L72
	nop

	.loc 1 5464 0
	.loc 1 5465 0
	b	$L71
	li	$23,1			# 0x1

$L72:
	.loc 1 5460 0
	.loc 1 5458 0
	bne	$4,$0,$L100
	lw	$25,%call16(rtl8370_getAsicVlanPortAndProtocolBased)($28)

$L71:
	.loc 1 5436 0
	addiu	$18,$18,1
	sltu	$2,$18,10
	.loc 1 5438 0
	.loc 1 5436 0
	bne	$2,$0,$L101
	lw	$25,%call16(rtl8370_getAsicVlanPortBasedVID)($28)

	.loc 1 5432 0
	.loc 1 5470 0
	bne	$23,$0,$L102
	addiu	$16,$16,1

	addiu	$16,$16,-1
	.loc 1 5473 0
	lw	$25,%call16(rtl8370_getAsicVlan4kEntry)($28)
	.loc 1 5472 0
	sh	$19,24($sp)
	.loc 1 5473 0
	jalr	$25
	addiu	$4,$sp,24

	.loc 1 5474 0
	move	$3,$2
	.loc 1 5473 0
	bne	$2,$0,$L41
	lw	$28,16($sp)

	.loc 1 5475 0
	lhu	$2,28($sp)
	.loc 1 5481 0
	lw	$25,%call16(rtl8370_setAsicVlanMemberConfig)($28)
	move	$5,$21
	.loc 1 5475 0
	sh	$2,52($sp)
	.loc 1 5476 0
	lhu	$2,32($sp)
	.loc 1 5481 0
	move	$4,$16
	.loc 1 5476 0
	sh	$2,54($sp)
	.loc 1 5477 0
	lhu	$2,34($sp)
	sh	$2,56($sp)
	.loc 1 5478 0
	lhu	$2,38($sp)
	sh	$2,60($sp)
	.loc 1 5479 0
	lhu	$2,36($sp)
	sh	$2,58($sp)
	.loc 1 5480 0
	lhu	$2,26($sp)
	.loc 1 5481 0
	jalr	$25
	sh	$2,50($sp)

	.loc 1 5482 0
	move	$3,$2
	.loc 1 5481 0
	bne	$2,$0,$L41
	lw	$28,16($sp)

	.loc 1 5484 0
	lw	$25,%call16(rtl8370_setAsicVlanPortBasedVID)($28)
$L98:
	move	$4,$20
	move	$5,$16
	jalr	$25
	move	$6,$22

	lw	$28,16($sp)
	.loc 1 5487 0
	.loc 1 5484 0
	beq	$2,$0,$L41
	move	$3,$0

	b	$L41
	move	$3,$2

$L61:
	.loc 1 5432 0
	addiu	$16,$16,1
$L102:
	slt	$2,$16,32
	.loc 1 5434 0
	.loc 1 5432 0
	bne	$2,$0,$L103
	move	$23,$0

	.loc 1 5491 0
	li	$3,-1			# 0xffffffffffffffff
$L41:
	.loc 1 5492 0
	lw	$31,136($sp)
	lw	$23,132($sp)
	lw	$22,128($sp)
	lw	$21,124($sp)
	lw	$20,120($sp)
	lw	$19,116($sp)
	lw	$18,112($sp)
	lw	$17,108($sp)
	lw	$16,104($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,144

	.set	macro
	.set	reorder
$LFE8:
	.end	rtk_vlan_portPvid_set
	.align	2
	.globl	rtl8370_setAsicLutLearnLimitNo
	.ent	rtl8370_setAsicLutLearnLimitNo
	.type	rtl8370_setAsicLutLearnLimitNo, @function
rtl8370_setAsicLutLearnLimitNo:
$LFB12:
	.loc 1 11439 0
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	.loc 1 11440 0
	sltu	$3,$4,16
	.loc 1 11443 0
	sltu	$6,$5,16384
	.loc 1 11441 0
	.loc 1 11440 0
	beq	$3,$0,$L107
	li	$2,3			# 0x3

	.loc 1 11444 0
	li	$2,393216			# 0x60000
	.loc 1 11446 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	addiu	$4,$4,2592
	.loc 1 11444 0
	.loc 1 11443 0
	beq	$6,$0,$L107
	ori	$2,$2,0x5

	.loc 1 11446 0
	jr	$25
	nop

$L107:
	.loc 1 11447 0
	j	$31
	nop

	.set	macro
	.set	reorder
$LFE12:
	.end	rtl8370_setAsicLutLearnLimitNo
	.rdata
	.align	2
$LC0:
	.half	6948
	.half	0
	.half	6949
	.half	0
	.half	6950
	.half	0
	.half	6951
	.half	0
	.half	4927
	.half	48
	.half	4926
	.half	14
	.half	8735
	.half	7
	.half	8734
	.half	72
	.half	8729
	.half	16402
	.half	8735
	.half	3
	.half	8705
	.half	13652
	.half	8706
	.half	25576
	.half	8707
	.half	-26174
	.half	8708
	.half	275
	.half	8709
	.half	12350
	.half	8717
	.half	519
	.half	8718
	.half	-7936
	.half	8735
	.half	7
	.half	8734
	.half	64
	.half	8728
	.half	0
	.half	8735
	.half	7
	.half	8734
	.half	44
	.half	8728
	.half	139
	.half	8735
	.half	5
	.half	8709
	.half	-10
	.half	8710
	.half	128
	.half	8735
	.half	5
	.half	8709
	.half	-32768
	.half	8710
	.half	640
	.half	8710
	.half	11255
	.half	8710
	.half	224
	.half	8710
	.half	-9
	.half	8710
	.half	-24448
	.half	8710
	.half	686
	.half	8710
	.half	-2558
	.half	8710
	.half	-32690
	.half	8710
	.half	513
	.half	8710
	.half	20482
	.half	8710
	.half	355
	.half	8710
	.half	513
	.half	8710
	.half	31200
	.half	8710
	.half	-29812
	.half	8710
	.half	-7797
	.half	8710
	.half	-29410
	.half	8710
	.half	481
	.half	8710
	.half	-29810
	.half	8710
	.half	7681
	.half	8710
	.half	-24576
	.half	8710
	.half	-6994
	.half	8710
	.half	-10049
	.half	8710
	.half	-29816
	.half	8710
	.half	-5120
	.half	8710
	.half	6569
	.half	8710
	.half	-29808
	.half	8710
	.half	-1554
	.half	8710
	.half	-10
	.half	8710
	.half	238
	.half	8710
	.half	-9
	.half	8710
	.half	-800
	.half	8710
	.half	-7872
	.half	8710
	.half	-7711
	.half	8710
	.half	16887
	.half	8710
	.half	12278
	.half	8710
	.half	10468
	.half	8710
	.half	-7872
	.half	8710
	.half	-6687
	.half	8710
	.half	16644
	.half	8710
	.half	-1798
	.half	8710
	.half	-4247
	.half	8710
	.half	-8053
	.half	8710
	.half	-31060
	.half	8710
	.half	8218
	.half	8710
	.half	-16512
	.half	8710
	.half	30672
	.half	8710
	.half	27650
	.half	8710
	.half	10616
	.half	8710
	.half	-7968
	.half	8710
	.half	-6943
	.half	8710
	.half	-7963
	.half	8710
	.half	22534
	.half	8710
	.half	26816
	.half	8710
	.half	-11822
	.half	8710
	.half	-6944
	.half	8710
	.half	-6939
	.half	8710
	.half	-7963
	.half	8710
	.half	-4202
	.half	8710
	.half	-260
	.half	8710
	.half	1061
	.half	8710
	.half	2055
	.half	8710
	.half	9792
	.half	8710
	.half	29223
	.half	8710
	.half	9854
	.half	8710
	.half	10244
	.half	8710
	.half	-18647
	.half	8710
	.half	9590
	.half	8710
	.half	10856
	.half	8710
	.half	-6869
	.half	8710
	.half	-21248
	.half	8710
	.half	11483
	.half	8710
	.half	-4051
	.half	8710
	.half	26555
	.half	8710
	.half	11899
	.half	8710
	.half	3887
	.half	8710
	.half	29541
	.half	8710
	.half	12716
	.half	8710
	.half	-13262
	.half	8710
	.half	8960
	.half	8710
	.half	13101
	.half	8710
	.half	5940
	.half	8710
	.half	32594
	.half	8710
	.half	13584
	.half	8710
	.half	54
	.half	8710
	.half	1536
	.half	8710
	.half	14092
	.half	8710
	.half	-16328
	.half	8710
	.half	32718
	.half	8710
	.half	15589
	.half	8710
	.half	-2243
	.half	8710
	.half	15780
	.half	8710
	.half	25904
	.half	8710
	.half	15975
	.half	8710
	.half	83
	.half	8710
	.half	27090
	.half	8710
	.half	3946
	.half	8710
	.half	300
	.half	8710
	.half	27691
	.half	8710
	.half	4974
	.half	8710
	.half	-7936
	.half	8710
	.half	28434
	.half	8710
	.half	-2191
	.half	8710
	.half	107
	.half	8710
	.half	29446
	.half	8710
	.half	-5260
	.half	8710
	.half	-27449
	.half	8710
	.half	30360
	.half	8710
	.half	2679
	.half	8710
	.half	20480
	.half	8710
	.half	30858
	.half	8710
	.half	5497
	.half	8710
	.half	32623
	.half	8710
	.half	31238
	.half	8710
	.half	-23040
	.half	8705
	.half	1793
	.half	8704
	.half	1029
	.half	8735
	.half	0
	.half	8704
	.half	4928
	.half	8735
	.half	0
	.half	4927
	.half	16
	.half	4926
	.half	4094
	.half	4611
	.half	-256
	.half	4608
	.half	32708
	.half	4637
	.half	32022
	.half	4638
	.half	1000
	.half	4639
	.half	590
	.half	4640
	.half	560
	.half	4641
	.half	580
	.half	4642
	.half	550
	.half	4643
	.half	590
	.half	4644
	.half	560
	.half	4645
	.half	580
	.half	4646
	.half	550
	.half	4647
	.half	192
	.half	4648
	.half	180
	.half	4655
	.half	192
	.half	4656
	.half	180
	.half	520
	.half	1000
	.half	521
	.half	1000
	.half	522
	.half	1000
	.half	523
	.half	1000
	.half	524
	.half	1000
	.half	525
	.half	1000
	.half	526
	.half	1000
	.half	527
	.half	1000
	.half	528
	.half	1000
	.half	529
	.half	1000
	.half	530
	.half	1000
	.half	531
	.half	1000
	.half	532
	.half	1000
	.half	533
	.half	1000
	.half	534
	.half	1000
	.half	535
	.half	1000
	.half	2304
	.half	0
	.half	2305
	.half	0
	.half	2306
	.half	0
	.half	2307
	.half	0
	.half	2149
	.half	12816
	.half	2171
	.half	0
	.half	2172
	.half	-256
	.half	2173
	.half	0
	.half	2174
	.half	0
	.half	2049
	.half	256
	.half	2050
	.half	256
	.half	2592
	.half	8256
	.half	2593
	.half	8256
	.half	2594
	.half	8256
	.half	2595
	.half	8256
	.half	2596
	.half	8256
	.half	2600
	.half	8256
	.half	2601
	.half	8256
	.half	8352
	.half	6464
	.half	8384
	.half	6464
	.half	8416
	.half	6464
	.half	4876
	.half	80
	.half	-1
	.half	-21555
	.align	2
$LC1:
	.half	6948
	.half	0
	.half	6949
	.half	0
	.half	6950
	.half	0
	.half	6951
	.half	0
	.half	4927
	.half	48
	.half	4926
	.half	14
	.half	8735
	.half	7
	.half	8734
	.half	72
	.half	8729
	.half	16402
	.half	8735
	.half	3
	.half	8705
	.half	13652
	.half	8706
	.half	25576
	.half	8707
	.half	-26174
	.half	8708
	.half	275
	.half	8709
	.half	12350
	.half	8717
	.half	519
	.half	8718
	.half	-7936
	.half	8735
	.half	7
	.half	8734
	.half	64
	.half	8728
	.half	0
	.half	8735
	.half	7
	.half	8734
	.half	44
	.half	8728
	.half	139
	.half	8735
	.half	5
	.half	8709
	.half	-10
	.half	8710
	.half	128
	.half	8735
	.half	5
	.half	8709
	.half	-32768
	.half	8710
	.half	640
	.half	8710
	.half	11255
	.half	8710
	.half	224
	.half	8710
	.half	-9
	.half	8710
	.half	-24448
	.half	8710
	.half	686
	.half	8710
	.half	-2558
	.half	8710
	.half	-32690
	.half	8710
	.half	513
	.half	8710
	.half	20482
	.half	8710
	.half	355
	.half	8710
	.half	513
	.half	8710
	.half	31200
	.half	8710
	.half	-29812
	.half	8710
	.half	-7797
	.half	8710
	.half	-29410
	.half	8710
	.half	481
	.half	8710
	.half	-29810
	.half	8710
	.half	7681
	.half	8710
	.half	-24576
	.half	8710
	.half	-6994
	.half	8710
	.half	-10049
	.half	8710
	.half	-29816
	.half	8710
	.half	-5120
	.half	8710
	.half	6569
	.half	8710
	.half	-29808
	.half	8710
	.half	-1554
	.half	8710
	.half	-10
	.half	8710
	.half	238
	.half	8710
	.half	-9
	.half	8710
	.half	-800
	.half	8710
	.half	-7872
	.half	8710
	.half	-7711
	.half	8710
	.half	16887
	.half	8710
	.half	12278
	.half	8710
	.half	10468
	.half	8710
	.half	-7872
	.half	8710
	.half	-6687
	.half	8710
	.half	16644
	.half	8710
	.half	-1798
	.half	8710
	.half	-4247
	.half	8710
	.half	-8053
	.half	8710
	.half	-31060
	.half	8710
	.half	8218
	.half	8710
	.half	-16512
	.half	8710
	.half	30672
	.half	8710
	.half	27650
	.half	8710
	.half	10616
	.half	8710
	.half	-7968
	.half	8710
	.half	-6943
	.half	8710
	.half	-7963
	.half	8710
	.half	22534
	.half	8710
	.half	26816
	.half	8710
	.half	-11822
	.half	8710
	.half	-6944
	.half	8710
	.half	-6939
	.half	8710
	.half	-7963
	.half	8710
	.half	-4202
	.half	8710
	.half	-260
	.half	8710
	.half	1061
	.half	8710
	.half	2055
	.half	8710
	.half	9792
	.half	8710
	.half	29223
	.half	8710
	.half	9854
	.half	8710
	.half	10244
	.half	8710
	.half	-18647
	.half	8710
	.half	9590
	.half	8710
	.half	10856
	.half	8710
	.half	-6869
	.half	8710
	.half	-21248
	.half	8710
	.half	11483
	.half	8710
	.half	-4051
	.half	8710
	.half	26555
	.half	8710
	.half	11899
	.half	8710
	.half	3887
	.half	8710
	.half	29541
	.half	8710
	.half	12716
	.half	8710
	.half	-13262
	.half	8710
	.half	8960
	.half	8710
	.half	13101
	.half	8710
	.half	5940
	.half	8710
	.half	32594
	.half	8710
	.half	13584
	.half	8710
	.half	54
	.half	8710
	.half	1536
	.half	8710
	.half	14092
	.half	8710
	.half	-16328
	.half	8710
	.half	32718
	.half	8710
	.half	15589
	.half	8710
	.half	-2243
	.half	8710
	.half	15780
	.half	8710
	.half	25904
	.half	8710
	.half	15975
	.half	8710
	.half	83
	.half	8710
	.half	27090
	.half	8710
	.half	3946
	.half	8710
	.half	300
	.half	8710
	.half	27691
	.half	8710
	.half	4974
	.half	8710
	.half	-7936
	.half	8710
	.half	28434
	.half	8710
	.half	-2191
	.half	8710
	.half	107
	.half	8710
	.half	29446
	.half	8710
	.half	-5260
	.half	8710
	.half	-27449
	.half	8710
	.half	30360
	.half	8710
	.half	2679
	.half	8710
	.half	20480
	.half	8710
	.half	30858
	.half	8710
	.half	5497
	.half	8710
	.half	32623
	.half	8710
	.half	31238
	.half	8710
	.half	-23040
	.half	8705
	.half	1793
	.half	8704
	.half	1029
	.half	8735
	.half	0
	.half	8704
	.half	4928
	.half	8735
	.half	0
	.half	4927
	.half	16
	.half	4926
	.half	4094
	.half	4611
	.half	-256
	.half	4608
	.half	32708
	.half	2304
	.half	0
	.half	2305
	.half	0
	.half	2306
	.half	0
	.half	2307
	.half	0
	.half	2149
	.half	12816
	.half	2171
	.half	0
	.half	2172
	.half	-256
	.half	2173
	.half	0
	.half	2174
	.half	0
	.half	2049
	.half	256
	.half	2050
	.half	256
	.half	2592
	.half	8256
	.half	2593
	.half	8256
	.half	2594
	.half	8256
	.half	2595
	.half	8256
	.half	2596
	.half	8256
	.half	2597
	.half	8256
	.half	2598
	.half	8256
	.half	2599
	.half	8256
	.half	2600
	.half	8256
	.half	2601
	.half	8256
	.half	4876
	.half	80
	.half	-1
	.half	-21555
	.text
	.align	2
	.ent	_rtk_switch_init2
	.type	_rtk_switch_init2, @function
_rtk_switch_init2:
$LFB11:
	.loc 1 11126 0
	.frame	$sp,1560,$31		# vars= 1520, regs= 3/0, args= 16, gp= 8
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-1560
$LCFI29:
	sw	$31,1552($sp)
$LCFI30:
	sw	$17,1548($sp)
$LCFI31:
	sw	$16,1544($sp)
$LCFI32:
	.cprestore	16
	.loc 1 11132 0
	lw	$2,%got($LC0)($28)
	addiu	$16,$sp,24
	move	$7,$16
	addiu	$6,$2,%lo($LC0)
	addiu	$8,$6,816
$L109:
	lwl	$2,3($6)
	lwl	$3,7($6)
	lwl	$4,11($6)
	lwl	$5,15($6)
	lwr	$2,0($6)
	lwr	$3,4($6)
	lwr	$4,8($6)
	lwr	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$8,$L109
	addiu	$7,$7,16

	.loc 1 11185 0
	lw	$2,%got($LC1)($28)
	addiu	$7,$sp,840
	addiu	$6,$2,%lo($LC1)
	addiu	$8,$6,688
$L110:
	lwl	$2,3($6)
	lwl	$3,7($6)
	lwl	$4,11($6)
	lwl	$5,15($6)
	lwr	$2,0($6)
	lwr	$3,4($6)
	lwr	$4,8($6)
	lwr	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$8,$L110
	addiu	$7,$7,16

	lwl	$2,3($6)
	lwl	$3,7($6)
	.loc 1 11231 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	.loc 1 11185 0
	lwr	$2,0($6)
	lwr	$3,4($6)
	.loc 1 11231 0
	li	$4,5058			# 0x13c2
	.loc 1 11185 0
	sw	$2,0($7)
	sw	$3,4($7)
	.loc 1 11231 0
	jalr	$25
	li	$5,585			# 0x249

	.loc 1 11232 0
	move	$3,$2
	.loc 1 11231 0
	bne	$2,$0,$L108
	lw	$28,16($sp)

	.loc 1 11234 0
	lw	$25,%call16(rtl8370_getAsicRegBits)($28)
	li	$4,4866			# 0x1302
	li	$5,7			# 0x7
	jalr	$25
	addiu	$6,$sp,1536

	.loc 1 11235 0
	move	$3,$2
	.loc 1 11234 0
	bne	$2,$0,$L108
	lw	$28,16($sp)

	.loc 1 11270 0
	lw	$3,1536($sp)
	li	$2,1			# 0x1
	.loc 1 11368 0
	.loc 1 11270 0
	beq	$3,$2,$L137
	li	$2,65535			# 0xffff

	beq	$3,$0,$L114
	nop

	li	$2,2			# 0x2
	.loc 1 11417 0
	.loc 1 11270 0
	beq	$3,$2,$L160
	li	$2,65535			# 0xffff

	.loc 1 11415 0
	.loc 1 11270 0
	b	$L108
	li	$3,-1			# 0xffffffffffffffff

$L114:
	.loc 1 11319 0
	lhu	$3,24($sp)
	beq	$3,$2,$L108
	move	$5,$16

	lhu	$3,26($sp)
	li	$2,43981			# 0xabcd
	beq	$3,$2,$L108
	move	$17,$0

$L136:
	.loc 1 11276 0
	addu	$5,$17,$5
	lhu	$4,0($5)
	li	$2,8192			# 0x2000
	andi	$3,$4,0xf000
	.loc 1 11316 0
	.loc 1 11276 0
	bne	$3,$2,$L117
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 11279 0
	li	$2,1			# 0x1
	sw	$2,1540($sp)
	.loc 1 11278 0
	move	$16,$0
	.loc 1 11283 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L225:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,1540

	.loc 1 11282 0
	addiu	$16,$16,1
	.loc 1 11283 0
	lw	$28,16($sp)
	.loc 1 11284 0
	.loc 1 11283 0
	bne	$2,$0,$L221
	sltu	$3,$16,5

	.loc 1 11284 0
	lw	$2,1540($sp)
	.loc 1 11286 0
	.loc 1 11284 0
	beq	$2,$0,$L224
	li	$2,5			# 0x5

	.loc 1 11283 0
	.loc 1 11284 0
	bne	$3,$0,$L225
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 11286 0
	li	$2,5			# 0x5
$L224:
	.loc 1 11289 0
	.loc 1 11286 0
	beq	$16,$2,$L219
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 11289 0
	li	$4,7939			# 0x1f03
	jalr	$25
	move	$5,$0

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11291 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7938			# 0x1f02
	jalr	$25
	li	$5,31			# 0x1f

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11293 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11297 0
	li	$2,1			# 0x1
	sw	$2,1540($sp)
	.loc 1 11296 0
	move	$16,$0
	.loc 1 11301 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L227:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,1540

	.loc 1 11300 0
	addiu	$16,$16,1
	.loc 1 11301 0
	lw	$28,16($sp)
	.loc 1 11302 0
	.loc 1 11301 0
	bne	$2,$0,$L221
	sltu	$3,$16,5

	.loc 1 11302 0
	lw	$2,1540($sp)
	.loc 1 11304 0
	.loc 1 11302 0
	beq	$2,$0,$L226
	li	$2,5			# 0x5

	.loc 1 11301 0
	.loc 1 11302 0
	bne	$3,$0,$L227
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 11304 0
	li	$2,5			# 0x5
$L226:
	.loc 1 11307 0
	.loc 1 11304 0
	beq	$16,$2,$L219
	addiu	$2,$sp,24

	.loc 1 11307 0
	addu	$16,$17,$2
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	lhu	$5,2($16)
	jalr	$25
	li	$4,7939			# 0x1f03

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11309 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	lhu	$5,0($16)
	jalr	$25
	li	$4,7938			# 0x1f02

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11311 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	beq	$2,$0,$L134
	lw	$28,16($sp)

	.loc 1 11408 0
	b	$L108
	move	$3,$2

$L117:
	.loc 1 11316 0
	jalr	$25
	lhu	$5,2($5)

	bne	$2,$0,$L222
	lw	$28,16($sp)

$L134:
	addiu	$17,$17,4
	.loc 1 11319 0
	addiu	$5,$sp,24
	addu	$4,$17,$5
	lhu	$3,0($4)
	li	$2,65535			# 0xffff
	beq	$3,$2,$L108
	li	$2,43981			# 0xabcd

	lhu	$3,2($4)
	bne	$3,$2,$L136
	lw	$31,1552($sp)

	.loc 1 11425 0
	.loc 1 11321 0
	b	$L236
	lw	$17,1548($sp)

$L137:
	.loc 1 11368 0
	lhu	$3,840($sp)
	beq	$3,$2,$L108
	move	$5,$16

	lhu	$3,842($sp)
	li	$2,43981			# 0xabcd
	beq	$3,$2,$L108
	move	$17,$0

$L159:
	.loc 1 11325 0
	addu	$5,$17,$5
	lhu	$4,816($5)
	li	$2,8192			# 0x2000
	andi	$3,$4,0xf000
	.loc 1 11365 0
	.loc 1 11325 0
	bne	$3,$2,$L140
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 11328 0
	li	$2,1			# 0x1
	sw	$2,1540($sp)
	.loc 1 11327 0
	move	$16,$0
	.loc 1 11332 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L229:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,1540

	.loc 1 11331 0
	addiu	$16,$16,1
	.loc 1 11332 0
	lw	$28,16($sp)
	.loc 1 11333 0
	.loc 1 11332 0
	bne	$2,$0,$L221
	sltu	$3,$16,5

	.loc 1 11333 0
	lw	$2,1540($sp)
	.loc 1 11335 0
	.loc 1 11333 0
	beq	$2,$0,$L228
	li	$2,5			# 0x5

	.loc 1 11332 0
	.loc 1 11333 0
	bne	$3,$0,$L229
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 11335 0
	li	$2,5			# 0x5
$L228:
	.loc 1 11338 0
	.loc 1 11335 0
	beq	$16,$2,$L219
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 11338 0
	li	$4,7939			# 0x1f03
	jalr	$25
	move	$5,$0

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11340 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7938			# 0x1f02
	jalr	$25
	li	$5,31			# 0x1f

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11342 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11346 0
	li	$2,1			# 0x1
	sw	$2,1540($sp)
	.loc 1 11345 0
	move	$16,$0
	.loc 1 11350 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L231:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,1540

	.loc 1 11349 0
	addiu	$16,$16,1
	.loc 1 11350 0
	lw	$28,16($sp)
	.loc 1 11351 0
	.loc 1 11350 0
	bne	$2,$0,$L221
	sltu	$3,$16,5

	.loc 1 11351 0
	lw	$2,1540($sp)
	.loc 1 11353 0
	.loc 1 11351 0
	beq	$2,$0,$L230
	li	$2,5			# 0x5

	.loc 1 11350 0
	.loc 1 11351 0
	bne	$3,$0,$L231
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 11353 0
	li	$2,5			# 0x5
$L230:
	.loc 1 11356 0
	.loc 1 11353 0
	beq	$16,$2,$L219
	addiu	$2,$sp,24

	.loc 1 11356 0
	addu	$16,$17,$2
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	lhu	$5,818($16)
	jalr	$25
	li	$4,7939			# 0x1f03

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11358 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	lhu	$5,816($16)
	jalr	$25
	li	$4,7938			# 0x1f02

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11360 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	beq	$2,$0,$L157
	lw	$28,16($sp)

	.loc 1 11408 0
	b	$L108
	move	$3,$2

$L140:
	.loc 1 11365 0
	jalr	$25
	lhu	$5,818($5)

	bne	$2,$0,$L222
	lw	$28,16($sp)

$L157:
	addiu	$17,$17,4
	.loc 1 11368 0
	addiu	$5,$sp,24
	addu	$4,$17,$5
	lhu	$3,816($4)
	li	$2,65535			# 0xffff
	beq	$3,$2,$L108
	li	$2,43981			# 0xabcd

	lhu	$3,818($4)
	bne	$3,$2,$L159
	lw	$31,1552($sp)

	.loc 1 11425 0
	.loc 1 11370 0
	b	$L236
	lw	$17,1548($sp)

$L160:
	.loc 1 11417 0
	lhu	$3,840($sp)
	beq	$3,$2,$L108
	move	$5,$16

	lhu	$3,842($sp)
	li	$2,43981			# 0xabcd
	beq	$3,$2,$L108
	move	$17,$0

$L182:
	.loc 1 11374 0
	addu	$5,$17,$5
	lhu	$4,816($5)
	li	$2,8192			# 0x2000
	andi	$3,$4,0xf000
	.loc 1 11414 0
	.loc 1 11374 0
	bne	$3,$2,$L163
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 11377 0
	li	$2,1			# 0x1
	sw	$2,1540($sp)
	.loc 1 11376 0
	move	$16,$0
	.loc 1 11381 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L233:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,1540

	.loc 1 11380 0
	addiu	$16,$16,1
	.loc 1 11381 0
	lw	$28,16($sp)
	.loc 1 11382 0
	.loc 1 11381 0
	bne	$2,$0,$L221
	sltu	$3,$16,5

	.loc 1 11382 0
	lw	$2,1540($sp)
	.loc 1 11384 0
	.loc 1 11382 0
	beq	$2,$0,$L232
	li	$2,5			# 0x5

	.loc 1 11381 0
	.loc 1 11382 0
	bne	$3,$0,$L233
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 11384 0
	li	$2,5			# 0x5
$L232:
	.loc 1 11387 0
	.loc 1 11384 0
	beq	$16,$2,$L219
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 11387 0
	li	$4,7939			# 0x1f03
	jalr	$25
	move	$5,$0

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11389 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7938			# 0x1f02
	jalr	$25
	li	$5,31			# 0x1f

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11391 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11395 0
	li	$2,1			# 0x1
	sw	$2,1540($sp)
	.loc 1 11394 0
	move	$16,$0
	.loc 1 11399 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L235:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,1540

	.loc 1 11398 0
	addiu	$16,$16,1
	.loc 1 11399 0
	lw	$28,16($sp)
	.loc 1 11400 0
	.loc 1 11399 0
	bne	$2,$0,$L221
	sltu	$3,$16,5

	.loc 1 11400 0
	lw	$2,1540($sp)
	.loc 1 11402 0
	.loc 1 11400 0
	beq	$2,$0,$L234
	li	$2,5			# 0x5

	.loc 1 11399 0
	.loc 1 11400 0
	bne	$3,$0,$L235
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 11402 0
	li	$2,5			# 0x5
$L234:
	.loc 1 11405 0
	.loc 1 11402 0
	beq	$16,$2,$L219
	addiu	$2,$sp,24

	.loc 1 11405 0
	addu	$16,$17,$2
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	lhu	$5,818($16)
	jalr	$25
	li	$4,7939			# 0x1f03

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11407 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	lhu	$5,816($16)
	jalr	$25
	li	$4,7938			# 0x1f02

	bne	$2,$0,$L221
	lw	$28,16($sp)

	.loc 1 11409 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	beq	$2,$0,$L180
	lw	$28,16($sp)

	.loc 1 11408 0
	b	$L108
	move	$3,$2

$L163:
	.loc 1 11414 0
	jalr	$25
	lhu	$5,818($5)

	bne	$2,$0,$L222
	lw	$28,16($sp)

$L180:
	addiu	$17,$17,4
	.loc 1 11417 0
	addiu	$5,$sp,24
	addu	$4,$17,$5
	lhu	$3,816($4)
	li	$2,65535			# 0xffff
	beq	$3,$2,$L108
	li	$2,43981			# 0xabcd

	lhu	$3,818($4)
	bne	$3,$2,$L182
	lw	$31,1552($sp)

	.loc 1 11425 0
	.loc 1 11419 0
	b	$L236
	lw	$17,1548($sp)

$L219:
	.loc 1 11403 0
	b	$L108
	li	$3,10			# 0xa

$L221:
	.loc 1 11408 0
	b	$L108
	move	$3,$2

$L222:
	.loc 1 11415 0
	li	$3,-1			# 0xffffffffffffffff
$L108:
	.loc 1 11425 0
	lw	$31,1552($sp)
	lw	$17,1548($sp)
$L236:
	lw	$16,1544($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,1560

	.set	macro
	.set	reorder
$LFE11:
	.end	_rtk_switch_init2
	.align	2
	.globl	rtl8370_setAsicPortForceLinkExt
	.ent	rtl8370_setAsicPortForceLinkExt
	.type	rtl8370_setAsicPortForceLinkExt, @function
rtl8370_setAsicPortForceLinkExt:
$LFB14:
	.loc 1 11497 0
	.frame	$sp,48,$31		# vars= 8, regs= 3/0, args= 16, gp= 8
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-48
$LCFI33:
	sw	$31,40($sp)
$LCFI34:
	sw	$17,36($sp)
$LCFI35:
	sw	$16,32($sp)
$LCFI36:
	.cprestore	16
	.loc 1 11497 0
	move	$17,$4
	.loc 1 11503 0
	sltu	$2,$17,2
	.loc 1 11497 0
	move	$16,$5
	.loc 1 11506 0
	lw	$25,%call16(memset)($28)
	li	$6,2			# 0x2
	addiu	$4,$sp,24
	move	$5,$0
	.loc 1 11504 0
	.loc 1 11503 0
	beq	$2,$0,$L237
	li	$3,3			# 0x3

	.loc 1 11506 0
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 11507 0
	move	$5,$16
	addiu	$4,$sp,24
	lw	$25,%call16(memcpy)($28)
	jalr	$25
	li	$6,2			# 0x2

	lw	$28,16($sp)
	.loc 1 11513 0
	lhu	$5,24($sp)
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	jalr	$25
	addiu	$4,$17,4880

	lw	$28,16($sp)
	move	$3,$2
$L237:
	.loc 1 11514 0
	lw	$31,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
$LFE14:
	.end	rtl8370_setAsicPortForceLinkExt
	.align	2
	.globl	rtl8370_getAsicPortForceLinkExt
	.ent	rtl8370_getAsicPortForceLinkExt
	.type	rtl8370_getAsicPortForceLinkExt, @function
rtl8370_getAsicPortForceLinkExt:
$LFB13:
	.loc 1 11460 0
	.frame	$sp,48,$31		# vars= 8, regs= 3/0, args= 16, gp= 8
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-48
$LCFI37:
	sw	$31,40($sp)
$LCFI38:
	sw	$17,36($sp)
$LCFI39:
	sw	$16,32($sp)
$LCFI40:
	.cprestore	16
	.loc 1 11460 0
	move	$16,$4
	.loc 1 11467 0
	sltu	$2,$16,2
	.loc 1 11460 0
	move	$17,$5
	.loc 1 11470 0
	lw	$25,%call16(memset)($28)
	li	$6,2			# 0x2
	addiu	$4,$sp,24
	move	$5,$0
	.loc 1 11468 0
	.loc 1 11467 0
	beq	$2,$0,$L239
	li	$3,3			# 0x3

	.loc 1 11470 0
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 11475 0
	addiu	$4,$16,4880
	lw	$25,%call16(rtl8370_getAsicReg)($28)
	jalr	$25
	addiu	$5,$sp,28

	lw	$28,16($sp)
	.loc 1 11481 0
	move	$4,$17
	li	$6,2			# 0x2
	lw	$25,%call16(memcpy)($28)
	.loc 1 11477 0
	move	$3,$2
	.loc 1 11481 0
	.loc 1 11476 0
	bne	$2,$0,$L239
	addiu	$5,$sp,24

	.loc 1 11479 0
	lw	$2,28($sp)
	.loc 1 11481 0
	jalr	$25
	sh	$2,24($sp)

	lw	$28,16($sp)
	.loc 1 11483 0
	move	$3,$0
$L239:
	.loc 1 11484 0
	lw	$31,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
$LFE13:
	.end	rtl8370_getAsicPortForceLinkExt
	.align	2
	.globl	rtl8370_setAsicPortExtMode
	.ent	rtl8370_setAsicPortExtMode
	.type	rtl8370_setAsicPortExtMode, @function
rtl8370_setAsicPortExtMode:
$LFB15:
	.loc 1 11535 0
	.frame	$sp,40,$31		# vars= 0, regs= 3/0, args= 16, gp= 8
	.mask	0x80030000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-40
$LCFI41:
	sw	$31,32($sp)
$LCFI42:
	sw	$17,28($sp)
$LCFI43:
	sw	$16,24($sp)
$LCFI44:
	.cprestore	16
	.loc 1 11538 0
	sltu	$2,$4,2
	.loc 1 11535 0
	move	$16,$4
	move	$17,$5
	.loc 1 11539 0
	.loc 1 11538 0
	beq	$2,$0,$L242
	li	$3,1			# 0x1

	.loc 1 11541 0
	sltu	$2,$5,8
	.loc 1 11544 0
	.loc 1 11541 0
	beq	$2,$0,$L242
	li	$2,7			# 0x7

	.loc 1 11544 0
	beq	$5,$2,$L246
	li	$2,1			# 0x1

	.loc 1 11551 0
	.loc 1 11544 0
	bne	$5,$2,$L245
	addiu	$2,$5,-4

$L246:
	.loc 1 11546 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,4867			# 0x1303
	jalr	$25
	li	$5,871			# 0x367

	.loc 1 11547 0
	move	$3,$2
	.loc 1 11546 0
	bne	$2,$0,$L242
	lw	$28,16($sp)

	.loc 1 11548 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,4868			# 0x1304
	jalr	$25
	li	$5,30583			# 0x7777

	.loc 1 11567 0
	b	$L262
	lw	$28,16($sp)

$L245:
	.loc 1 11551 0
	sltu	$2,$2,2
	.loc 1 11556 0
	.loc 1 11551 0
	beq	$2,$0,$L250
	li	$2,6			# 0x6

	.loc 1 11553 0
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)
	addiu	$5,$4,1
	andi	$5,$5,0x1
	b	$L261
	li	$4,1015			# 0x3f7

$L250:
	.loc 1 11556 0
	bne	$5,$2,$L253
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

	.loc 1 11558 0
	li	$4,4867			# 0x1303
	addiu	$5,$16,8
	jalr	$25
	li	$6,1			# 0x1

	.loc 1 11559 0
	move	$3,$2
	.loc 1 11558 0
	bne	$2,$0,$L242
	lw	$28,16($sp)

	.loc 1 11560 0
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)
	addiu	$4,$16,4870
	li	$5,6			# 0x6
$L261:
	b	$L259
	li	$6,1			# 0x1

$L253:
	.loc 1 11565 0
	addiu	$5,$4,1
	andi	$5,$5,0x1
	li	$4,1015			# 0x3f7
	jalr	$25
	move	$6,$0

	.loc 1 11566 0
	move	$3,$2
	.loc 1 11565 0
	bne	$2,$0,$L242
	lw	$28,16($sp)

	.loc 1 11567 0
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)
	addiu	$4,$16,4870
	li	$5,6			# 0x6
	move	$6,$0
$L259:
	jalr	$25
	nop

	lw	$28,16($sp)
$L262:
	.loc 1 11568 0
	.loc 1 11567 0
	bne	$2,$0,$L242
	move	$3,$2

	.loc 1 11571 0
	lw	$25,%call16(rtl8370_setAsicRegBits)($28)
	sll	$2,$16,2
	li	$5,7			# 0x7
	move	$6,$17
	.loc 1 11572 0
	lw	$31,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	.loc 1 11571 0
	sll	$5,$5,$2
	li	$4,4869			# 0x1305
	.loc 1 11572 0
	.loc 1 11571 0
	jr	$25
	addiu	$sp,$sp,40

$L242:
	.loc 1 11572 0
	lw	$31,32($sp)
	lw	$17,28($sp)
	lw	$16,24($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
$LFE15:
	.end	rtl8370_setAsicPortExtMode
	.align	2
	.globl	rtk_port_macForceLinkExt1_set
	.ent	rtk_port_macForceLinkExt1_set
	.type	rtk_port_macForceLinkExt1_set, @function
rtk_port_macForceLinkExt1_set:
$LFB3:
	.loc 1 4007 0
	.frame	$sp,40,$31		# vars= 8, regs= 2/0, args= 16, gp= 8
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-40
$LCFI45:
	sw	$31,36($sp)
$LCFI46:
	sw	$16,32($sp)
$LCFI47:
	.cprestore	16
	.loc 1 4011 0
	sltu	$2,$4,8
	.loc 1 4007 0
	move	$16,$5
	.loc 1 4012 0
	.loc 1 4011 0
	beq	$2,$0,$L263
	li	$3,1			# 0x1

	.loc 1 4014 0
	lw	$2,0($5)
	sltu	$2,$2,2
	beq	$2,$0,$L263
	nop

	lw	$2,4($5)
	sltu	$2,$2,3
	beq	$2,$0,$L263
	nop

	lw	$2,8($5)
	sltu	$2,$2,2
	beq	$2,$0,$L263
	nop

	lw	$2,12($5)
	sltu	$2,$2,2
	beq	$2,$0,$L263
	nop

	lw	$2,16($5)
	sltu	$2,$2,2
	beq	$2,$0,$L263
	nop

	lw	$2,20($5)
	sltu	$2,$2,2
	.loc 1 4036 0
	.loc 1 4014 0
	beq	$2,$0,$L270
	lw	$31,36($sp)

	lw	$2,24($5)
	sltu	$2,$2,2
	.loc 1 4018 0
	.loc 1 4014 0
	beq	$2,$0,$L270
	lw	$25,%call16(rtl8370_setAsicPortExtMode)($28)

	.loc 1 4018 0
	move	$5,$4
	jalr	$25
	li	$4,1			# 0x1

	.loc 1 4019 0
	move	$3,$2
	.loc 1 4018 0
	bne	$2,$0,$L263
	lw	$28,16($sp)

	.loc 1 4021 0
	lw	$25,%call16(rtl8370_getAsicPortForceLinkExt)($28)
	li	$4,1			# 0x1
	jalr	$25
	addiu	$5,$sp,24

	.loc 1 4022 0
	move	$3,$2
	.loc 1 4021 0
	bne	$2,$0,$L263
	lw	$28,16($sp)

	.loc 1 4024 0
	lw	$3,0($16)
	lhu	$2,24($sp)
	.loc 1 4025 0
	lw	$4,4($16)
	.loc 1 4026 0
	lw	$5,8($16)
	.loc 1 4024 0
	andi	$3,$3,0x1
	.loc 1 4027 0
	lw	$6,12($16)
	.loc 1 4024 0
	sll	$3,$3,12
	andi	$2,$2,0xeffc
	.loc 1 4025 0
	or	$2,$2,$3
	andi	$4,$4,0x3
	.loc 1 4028 0
	lw	$7,16($16)
	.loc 1 4025 0
	or	$2,$2,$4
	.loc 1 4026 0
	andi	$5,$5,0x1
	sll	$5,$5,2
	andi	$2,$2,0xffeb
	.loc 1 4027 0
	andi	$6,$6,0x1
	.loc 1 4029 0
	lw	$4,20($16)
	.loc 1 4027 0
	or	$2,$2,$5
	sll	$6,$6,4
	or	$2,$2,$6
	.loc 1 4028 0
	li	$3,-129			# 0xffffffffffffff7f
	andi	$7,$7,0x1
	.loc 1 4030 0
	lw	$5,24($16)
	.loc 1 4028 0
	and	$2,$2,$3
	sll	$7,$7,7
	or	$2,$2,$7
	.loc 1 4029 0
	li	$3,-65			# 0xffffffffffffffbf
	andi	$4,$4,0x1
	and	$2,$2,$3
	sll	$4,$4,6
	.loc 1 4030 0
	li	$3,-33			# 0xffffffffffffffdf
	.loc 1 4029 0
	or	$2,$2,$4
	.loc 1 4030 0
	andi	$5,$5,0x1
	and	$2,$2,$3
	sll	$5,$5,5
	.loc 1 4032 0
	lw	$25,%call16(rtl8370_setAsicPortForceLinkExt)($28)
	.loc 1 4030 0
	or	$2,$2,$5
	sh	$2,24($sp)
	.loc 1 4032 0
	li	$4,1			# 0x1
	jalr	$25
	addiu	$5,$sp,24

	lw	$28,16($sp)
	move	$3,$2
$L263:
	.loc 1 4036 0
	lw	$31,36($sp)
$L270:
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
$LFE3:
	.end	rtk_port_macForceLinkExt1_set
	.align	2
	.globl	rtk_port_macForceLinkExt0_set
	.ent	rtk_port_macForceLinkExt0_set
	.type	rtk_port_macForceLinkExt0_set, @function
rtk_port_macForceLinkExt0_set:
$LFB2:
	.loc 1 3905 0
	.frame	$sp,40,$31		# vars= 8, regs= 2/0, args= 16, gp= 8
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-40
$LCFI48:
	sw	$31,36($sp)
$LCFI49:
	sw	$16,32($sp)
$LCFI50:
	.cprestore	16
	.loc 1 3909 0
	sltu	$2,$4,8
	.loc 1 3905 0
	move	$16,$5
	.loc 1 3910 0
	.loc 1 3909 0
	beq	$2,$0,$L271
	li	$3,1			# 0x1

	.loc 1 3912 0
	lw	$2,0($5)
	sltu	$2,$2,2
	beq	$2,$0,$L271
	nop

	lw	$2,4($5)
	sltu	$2,$2,3
	beq	$2,$0,$L271
	nop

	lw	$2,8($5)
	sltu	$2,$2,2
	beq	$2,$0,$L271
	nop

	lw	$2,12($5)
	sltu	$2,$2,2
	beq	$2,$0,$L271
	nop

	lw	$2,16($5)
	sltu	$2,$2,2
	beq	$2,$0,$L271
	nop

	lw	$2,20($5)
	sltu	$2,$2,2
	.loc 1 3934 0
	.loc 1 3912 0
	beq	$2,$0,$L278
	lw	$31,36($sp)

	lw	$2,24($5)
	sltu	$2,$2,2
	.loc 1 3916 0
	.loc 1 3912 0
	beq	$2,$0,$L278
	lw	$25,%call16(rtl8370_setAsicPortExtMode)($28)

	.loc 1 3916 0
	move	$5,$4
	jalr	$25
	move	$4,$0

	.loc 1 3917 0
	move	$3,$2
	.loc 1 3916 0
	bne	$2,$0,$L271
	lw	$28,16($sp)

	.loc 1 3919 0
	lw	$25,%call16(rtl8370_getAsicPortForceLinkExt)($28)
	move	$4,$0
	jalr	$25
	addiu	$5,$sp,24

	.loc 1 3920 0
	move	$3,$2
	.loc 1 3919 0
	bne	$2,$0,$L271
	lw	$28,16($sp)

	.loc 1 3922 0
	lw	$3,0($16)
	lhu	$2,24($sp)
	.loc 1 3923 0
	lw	$4,4($16)
	.loc 1 3924 0
	lw	$5,8($16)
	.loc 1 3922 0
	andi	$3,$3,0x1
	.loc 1 3925 0
	lw	$6,12($16)
	.loc 1 3922 0
	sll	$3,$3,12
	andi	$2,$2,0xeffc
	.loc 1 3923 0
	or	$2,$2,$3
	andi	$4,$4,0x3
	.loc 1 3926 0
	lw	$7,16($16)
	.loc 1 3923 0
	or	$2,$2,$4
	.loc 1 3924 0
	andi	$5,$5,0x1
	sll	$5,$5,2
	andi	$2,$2,0xffeb
	.loc 1 3925 0
	andi	$6,$6,0x1
	.loc 1 3927 0
	lw	$4,20($16)
	.loc 1 3925 0
	or	$2,$2,$5
	sll	$6,$6,4
	or	$2,$2,$6
	.loc 1 3926 0
	li	$3,-129			# 0xffffffffffffff7f
	andi	$7,$7,0x1
	.loc 1 3928 0
	lw	$5,24($16)
	.loc 1 3926 0
	and	$2,$2,$3
	sll	$7,$7,7
	or	$2,$2,$7
	.loc 1 3927 0
	li	$3,-65			# 0xffffffffffffffbf
	andi	$4,$4,0x1
	and	$2,$2,$3
	sll	$4,$4,6
	.loc 1 3928 0
	li	$3,-33			# 0xffffffffffffffdf
	.loc 1 3927 0
	or	$2,$2,$4
	.loc 1 3928 0
	andi	$5,$5,0x1
	and	$2,$2,$3
	sll	$5,$5,5
	.loc 1 3930 0
	lw	$25,%call16(rtl8370_setAsicPortForceLinkExt)($28)
	.loc 1 3928 0
	or	$2,$2,$5
	sh	$2,24($sp)
	.loc 1 3930 0
	move	$4,$0
	jalr	$25
	addiu	$5,$sp,24

	lw	$28,16($sp)
	move	$3,$2
$L271:
	.loc 1 3934 0
	lw	$31,36($sp)
$L278:
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,40

	.set	macro
	.set	reorder
$LFE2:
	.end	rtk_port_macForceLinkExt0_set
	.align	2
	.globl	rtl8370_setAsicPHYReg
	.ent	rtl8370_setAsicPHYReg
	.type	rtl8370_setAsicPHYReg, @function
rtl8370_setAsicPHYReg:
$LFB16:
	.loc 1 11589 0
	.frame	$sp,48,$31		# vars= 8, regs= 4/0, args= 16, gp= 8
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-48
$LCFI51:
	sw	$31,44($sp)
$LCFI52:
	sw	$18,40($sp)
$LCFI53:
	sw	$17,36($sp)
$LCFI54:
	sw	$16,32($sp)
$LCFI55:
	.cprestore	16
	.loc 1 11594 0
	sltu	$2,$4,8
	.loc 1 11589 0
	move	$18,$4
	move	$17,$5
	.loc 1 11598 0
	sltu	$7,$5,32
	.loc 1 11589 0
	move	$16,$6
	.loc 1 11595 0
	.loc 1 11594 0
	beq	$2,$0,$L279
	li	$3,3			# 0x3

	.loc 1 11599 0
	li	$2,917504			# 0xe0000
	.loc 1 11607 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
	addiu	$6,$sp,24
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	.loc 1 11599 0
	.loc 1 11598 0
	beq	$7,$0,$L279
	ori	$3,$2,0x1

	.loc 1 11607 0
	jalr	$25
	nop

	.loc 1 11609 0
	move	$3,$2
	.loc 1 11607 0
	.loc 1 11608 0
	bne	$2,$0,$L279
	lw	$28,16($sp)

	.loc 1 11611 0
	lw	$2,24($sp)
	.loc 1 11615 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	move	$5,$16
	li	$4,7939			# 0x1f03
	.loc 1 11612 0
	.loc 1 11611 0
	bne	$2,$0,$L279
	li	$3,10			# 0xa

	.loc 1 11615 0
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 11620 0
	sll	$5,$18,5
	or	$5,$5,$17
	.loc 1 11622 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	ori	$5,$5,0x2000
	li	$4,7938			# 0x1f02
	.loc 1 11617 0
	.loc 1 11616 0
	bne	$2,$0,$L279
	move	$3,$2

	.loc 1 11622 0
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 11627 0
	li	$4,7936			# 0x1f00
	li	$5,3			# 0x3
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	.loc 1 11624 0
	.loc 1 11623 0
	bne	$2,$0,$L279
	move	$3,$2

	.loc 1 11627 0
	jalr	$25
	nop

	lw	$28,16($sp)
	move	$3,$2
$L279:
	.loc 1 11628 0
	lw	$31,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
$LFE16:
	.end	rtl8370_setAsicPHYReg
	.align	2
	.globl	rtl8370_getAsicPHYReg
	.ent	rtl8370_getAsicPHYReg
	.type	rtl8370_getAsicPHYReg, @function
rtl8370_getAsicPHYReg:
$LFB17:
	.loc 1 11645 0
	.frame	$sp,48,$31		# vars= 8, regs= 4/0, args= 16, gp= 8
	.mask	0x80070000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-48
$LCFI56:
	sw	$31,44($sp)
$LCFI57:
	sw	$18,40($sp)
$LCFI58:
	sw	$17,36($sp)
$LCFI59:
	sw	$16,32($sp)
$LCFI60:
	.cprestore	16
	.loc 1 11650 0
	sltu	$2,$4,8
	.loc 1 11645 0
	move	$17,$4
	move	$16,$5
	move	$18,$6
	.loc 1 11651 0
	.loc 1 11650 0
	beq	$2,$0,$L286
	li	$3,3			# 0x3

	.loc 1 11653 0
	sltu	$2,$5,32
	.loc 1 11657 0
	.loc 1 11653 0
	bne	$2,$0,$L288
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 11654 0
	li	$2,917504			# 0xe0000
	b	$L286
	ori	$3,$2,0x1

$L288:
	.loc 1 11657 0
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,24

	.loc 1 11659 0
	move	$3,$2
	.loc 1 11657 0
	.loc 1 11658 0
	bne	$2,$0,$L286
	lw	$28,16($sp)

	.loc 1 11661 0
	lw	$2,24($sp)
	.loc 1 11662 0
	.loc 1 11661 0
	bne	$2,$0,$L286
	li	$3,10			# 0xa

	.loc 1 11665 0
	sll	$5,$17,5
	.loc 1 11667 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	.loc 1 11665 0
	or	$5,$5,$16
	ori	$5,$5,0x2000
	sw	$5,28($sp)
	.loc 1 11667 0
	jalr	$25
	li	$4,7938			# 0x1f02

	.loc 1 11669 0
	move	$3,$2
	.loc 1 11667 0
	.loc 1 11668 0
	bne	$2,$0,$L286
	lw	$28,16($sp)

	.loc 1 11672 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,1			# 0x1

	.loc 1 11674 0
	move	$3,$2
	.loc 1 11672 0
	.loc 1 11673 0
	bne	$2,$0,$L286
	lw	$28,16($sp)

	.loc 1 11676 0
	li	$16,5			# 0x5
	.loc 1 11679 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L302:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,24

	.loc 1 11682 0
	addiu	$16,$16,-1
	.loc 1 11679 0
	.loc 1 11680 0
	beq	$2,$0,$L295
	lw	$28,16($sp)

	.loc 1 11679 0
	.loc 1 11683 0
	bne	$16,$0,$L302
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 11684 0
	b	$L286
	move	$3,$2

$L295:
	.loc 1 11693 0
	lw	$25,%call16(rtl8370_getAsicReg)($28)
	li	$4,7940			# 0x1f04
	jalr	$25
	addiu	$5,$sp,28

	.loc 1 11695 0
	move	$3,$2
	.loc 1 11693 0
	.loc 1 11694 0
	bne	$2,$0,$L286
	lw	$28,16($sp)

	.loc 1 11697 0
	lw	$2,28($sp)
	.loc 1 11699 0
	move	$3,$0
	.loc 1 11697 0
	sw	$2,0($18)
$L286:
	.loc 1 11700 0
	lw	$31,44($sp)
	lw	$18,40($sp)
	lw	$17,36($sp)
	lw	$16,32($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,48

	.set	macro
	.set	reorder
$LFE17:
	.end	rtl8370_getAsicPHYReg
	.align	2
	.globl	rtk_led_enable_set
	.ent	rtk_led_enable_set
	.type	rtk_led_enable_set, @function
rtk_led_enable_set:
$LFB18:
	.loc 1 11721 0
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 16, gp= 8
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-32
$LCFI61:
	sw	$31,24($sp)
$LCFI62:
	.cprestore	16
	.loc 1 11724 0
	sltu	$3,$4,3
	.loc 1 11727 0
	sltu	$6,$5,129
	.loc 1 11725 0
	.loc 1 11724 0
	beq	$3,$0,$L303
	li	$2,1			# 0x1

	.loc 1 11730 0
	.loc 1 11727 0
	beq	$6,$0,$L303
	lw	$25,%call16(rtl8370_setAsicLedGroupEnable)($28)

	.loc 1 11730 0
	jalr	$25
	nop

	lw	$28,16($sp)
$L303:
	.loc 1 11734 0
	lw	$31,24($sp)
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
$LFE18:
	.end	rtk_led_enable_set
	.align	2
	.globl	rtk_led_mode_set
	.ent	rtk_led_mode_set
	.type	rtk_led_mode_set, @function
rtk_led_mode_set:
$LFB19:
	.loc 1 11775 0
	.frame	$sp,32,$31		# vars= 0, regs= 1/0, args= 16, gp= 8
	.mask	0x80000000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-32
$LCFI63:
	sw	$31,24($sp)
$LCFI64:
	.cprestore	16
	.loc 1 11778 0
	sltu	$2,$4,4
	.loc 1 11781 0
	lw	$25,%call16(rtl8370_setAsicLedGroupMode)($28)
	.loc 1 11779 0
	.loc 1 11778 0
	beq	$2,$0,$L307
	li	$3,-1			# 0xffffffffffffffff

	.loc 1 11781 0
	jalr	$25
	nop

	lw	$28,16($sp)
	move	$3,$2
$L307:
	.loc 1 11785 0
	lw	$31,24($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,32

	.set	macro
	.set	reorder
$LFE19:
	.end	rtk_led_mode_set
	.align	2
	.globl	init_rgmii
	.ent	init_rgmii
	.type	init_rgmii, @function
init_rgmii:
$LFB20:
	.loc 1 11788 0
	.frame	$sp,72,$31		# vars= 40, regs= 2/0, args= 16, gp= 8
	.mask	0x80010000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-72
$LCFI65:
	sw	$31,68($sp)
$LCFI66:
	sw	$16,64($sp)
$LCFI67:
	.cprestore	16
	.loc 1 11805 0
	lw	$25,%call16(rtk_port_macForceLinkExt0_set)($28)
	.loc 1 11795 0
	li	$3,1			# 0x1
	.loc 1 11796 0
	li	$2,2			# 0x2
	.loc 1 11801 0
	sw	$3,44($sp)
	.loc 1 11795 0
	sw	$3,24($sp)
	.loc 1 11797 0
	sw	$3,32($sp)
	.loc 1 11798 0
	sw	$3,36($sp)
	.loc 1 11800 0
	sw	$3,48($sp)
	.loc 1 11796 0
	sw	$2,28($sp)
	.loc 1 11788 0
	move	$16,$4
	.loc 1 11799 0
	sw	$0,40($sp)
	.loc 1 11805 0
	li	$4,1			# 0x1
	jalr	$25
	addiu	$5,$sp,24

	lw	$28,16($sp)
	.loc 1 11806 0
	.loc 1 11805 0
	bne	$2,$0,$L310
	li	$3,-1			# 0xffffffffffffffff

	.loc 1 11810 0
	lw	$25,%call16(rtk_port_rgmiiDelayExt0_set)($28)
	li	$4,1			# 0x1
	jalr	$25
	li	$5,1			# 0x1

	.loc 1 11815 0
	bne	$16,$0,$L312
	lw	$28,16($sp)

	.loc 1 11817 0
	lw	$25,%call16(rtk_port_macForceLinkExt1_set)($28)
	li	$4,1			# 0x1
	jalr	$25
	addiu	$5,$sp,24

	lw	$28,16($sp)
	.loc 1 11818 0
	.loc 1 11817 0
	bne	$2,$0,$L310
	li	$3,-1			# 0xffffffffffffffff

	.loc 1 11822 0
	lw	$25,%call16(rtk_port_rgmiiDelayExt1_set)($28)
	li	$4,1			# 0x1
	jalr	$25
	li	$5,1			# 0x1

	lw	$28,16($sp)
$L312:
	.loc 1 11829 0
	lw	$25,%call16(rtk_led_enable_set)($28)
	move	$4,$0
	jalr	$25
	li	$5,31			# 0x1f

	lw	$28,16($sp)
	.loc 1 11830 0
	.loc 1 11829 0
	bne	$2,$0,$L310
	li	$3,-1			# 0xffffffffffffffff

	.loc 1 11832 0
	lw	$25,%call16(rtk_led_enable_set)($28)
	li	$4,1			# 0x1
	jalr	$25
	li	$5,31			# 0x1f

	lw	$28,16($sp)
	.loc 1 11833 0
	.loc 1 11832 0
	bne	$2,$0,$L310
	li	$3,-1			# 0xffffffffffffffff

	.loc 1 11835 0
	lw	$25,%call16(rtk_led_enable_set)($28)
	li	$4,2			# 0x2
	jalr	$25
	li	$5,31			# 0x1f

	lw	$28,16($sp)
	.loc 1 11836 0
	.loc 1 11835 0
	bne	$2,$0,$L310
	li	$3,-1			# 0xffffffffffffffff

	.loc 1 11838 0
	lw	$25,%call16(rtk_led_mode_set)($28)
	jalr	$25
	move	$4,$0

	lw	$28,16($sp)
	.loc 1 11839 0
	.loc 1 11838 0
	bne	$2,$0,$L310
	li	$3,-1			# 0xffffffffffffffff

	.loc 1 11843 0
	.loc 1 11842 0
	beq	$16,$0,$L310
	move	$3,$0

	.loc 1 11845 0
	lw	$25,%call16(rtk_vlan_init)($28)
	jalr	$25
	nop

	lw	$28,16($sp)
	.loc 1 11846 0
	.loc 1 11845 0
	bne	$2,$0,$L310
	li	$3,-1			# 0xffffffffffffffff

	.loc 1 11852 0
	lw	$25,%call16(rtk_vlan_set)($28)
	li	$4,1			# 0x1
	li	$5,526			# 0x20e
	li	$6,255			# 0xff
	jalr	$25
	li	$7,1			# 0x1

	lw	$28,16($sp)
	.loc 1 11856 0
	li	$7,2			# 0x2
	li	$4,2			# 0x2
	lw	$25,%call16(rtk_vlan_set)($28)
	li	$5,529			# 0x211
	jalr	$25
	li	$6,255			# 0xff

	lw	$28,16($sp)
	.loc 1 11858 0
	move	$4,$0
	li	$5,3			# 0x3
	lw	$25,%call16(rtk_vlan_portPvid_set)($28)
	jalr	$25
	move	$6,$0

	lw	$28,16($sp)
	.loc 1 11859 0
	li	$4,1			# 0x1
	li	$5,1			# 0x1
	lw	$25,%call16(rtk_vlan_portPvid_set)($28)
	jalr	$25
	move	$6,$0

	lw	$28,16($sp)
	.loc 1 11860 0
	li	$4,2			# 0x2
	li	$5,1			# 0x1
	lw	$25,%call16(rtk_vlan_portPvid_set)($28)
	jalr	$25
	move	$6,$0

	lw	$28,16($sp)
	.loc 1 11861 0
	li	$4,3			# 0x3
	li	$5,1			# 0x1
	lw	$25,%call16(rtk_vlan_portPvid_set)($28)
	jalr	$25
	move	$6,$0

	lw	$28,16($sp)
	.loc 1 11862 0
	li	$4,4			# 0x4
	li	$5,2			# 0x2
	lw	$25,%call16(rtk_vlan_portPvid_set)($28)
	jalr	$25
	move	$6,$0

	lw	$28,16($sp)
	.loc 1 11863 0
	li	$4,9			# 0x9
	li	$5,1			# 0x1
	lw	$25,%call16(rtk_vlan_portPvid_set)($28)
	jalr	$25
	move	$6,$0

	lw	$28,16($sp)
	.loc 1 11902 0
	move	$3,$0
$L310:
	.loc 1 11903 0
	lw	$31,68($sp)
	lw	$16,64($sp)
	move	$2,$3
	j	$31
	addiu	$sp,$sp,72

	.set	macro
	.set	reorder
$LFE20:
	.end	init_rgmii
	.rdata
	.align	2
	.type	__FUNCTION__.0, @object
	.size	__FUNCTION__.0, 16
__FUNCTION__.0:
	.ascii	"rtk_switch_init\000"
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
$LC3:
	.ascii	"regData1 = %04x regData2 = %04x, RTL8367R\n\000"
	.align	2
$LC2:
	.ascii	"regData1 = %04x regData2 = %04x, RTL8367M\n\000"
	.rdata
	.align	2
$LC4:
	.half	4927
	.half	48
	.half	4926
	.half	14
	.half	8735
	.half	0
	.half	8725
	.half	4102
	.half	8735
	.half	5
	.half	8704
	.half	198
	.half	8735
	.half	7
	.half	8734
	.half	72
	.half	8725
	.half	25618
	.half	8726
	.half	25618
	.half	8727
	.half	25618
	.half	8728
	.half	25618
	.half	8729
	.half	25618
	.half	8730
	.half	25618
	.half	8735
	.half	1
	.half	8716
	.half	-9232
	.half	8713
	.half	9590
	.half	8711
	.half	10366
	.half	8714
	.half	26853
	.half	8733
	.half	15780
	.half	8732
	.half	-6153
	.half	8724
	.half	32594
	.half	8728
	.half	32718
	.half	8712
	.half	1207
	.half	8710
	.half	16498
	.half	8720
	.half	-4002
	.half	8731
	.half	-19436
	.half	8735
	.half	3
	.half	8730
	.half	1702
	.half	8720
	.half	-4002
	.half	8723
	.half	1771
	.half	8722
	.half	-2862
	.half	8718
	.half	-7904
	.half	8704
	.half	31744
	.half	8706
	.half	24528
	.half	8717
	.half	519
	.half	8735
	.half	2
	.half	8709
	.half	2424
	.half	8706
	.half	-29695
	.half	8711
	.half	13856
	.half	8732
	.half	1
	.half	8707
	.half	1056
	.half	8708
	.half	-32568
	.half	4926
	.half	3806
	.half	8735
	.half	2
	.half	8716
	.half	115
	.half	8717
	.half	-5275
	.half	8718
	.half	20945
	.half	8719
	.half	24011
	.half	8720
	.half	12356
	.half	8721
	.half	6144
	.half	8722
	.half	32256
	.half	8723
	.half	0
	.half	4927
	.half	16
	.half	4926
	.half	4094
	.half	8319
	.half	2
	.half	8308
	.half	15650
	.half	8309
	.half	8192
	.half	8310
	.half	24640
	.half	8311
	.half	0
	.half	8312
	.half	3850
	.half	8313
	.half	20651
	.half	8314
	.half	0
	.half	8315
	.half	3855
	.half	8287
	.half	2
	.half	8276
	.half	-256
	.half	8277
	.half	10
	.half	8278
	.half	10
	.half	8279
	.half	5
	.half	8280
	.half	5
	.half	8281
	.half	0
	.half	8282
	.half	5
	.half	8283
	.half	5
	.half	8284
	.half	5
	.half	8351
	.half	2
	.half	8340
	.half	170
	.half	8341
	.half	170
	.half	8342
	.half	170
	.half	8343
	.half	170
	.half	8344
	.half	85
	.half	8345
	.half	170
	.half	8346
	.half	170
	.half	8347
	.half	170
	.half	4963
	.half	-31916
	.half	4720
	.half	13107
	.half	4721
	.half	13107
	.half	4722
	.half	13107
	.half	4912
	.half	219
	.half	4611
	.half	-256
	.half	4608
	.half	32708
	.half	4637
	.half	4102
	.half	4638
	.half	1000
	.half	4639
	.half	691
	.half	4640
	.half	655
	.half	4641
	.half	667
	.half	4642
	.half	631
	.half	4643
	.half	691
	.half	4644
	.half	655
	.half	4645
	.half	667
	.half	4646
	.half	631
	.half	4647
	.half	192
	.half	4648
	.half	180
	.half	4655
	.half	192
	.half	4656
	.half	180
	.half	4649
	.half	32
	.half	4650
	.half	12
	.half	4657
	.half	48
	.half	4658
	.half	36
	.half	537
	.half	50
	.half	512
	.half	1000
	.half	513
	.half	1000
	.half	514
	.half	1000
	.half	515
	.half	1000
	.half	516
	.half	1000
	.half	517
	.half	1000
	.half	518
	.half	1000
	.half	519
	.half	1000
	.half	536
	.half	50
	.half	520
	.half	667
	.half	521
	.half	667
	.half	522
	.half	667
	.half	523
	.half	667
	.half	524
	.half	667
	.half	525
	.half	667
	.half	526
	.half	667
	.half	527
	.half	667
	.half	528
	.half	667
	.half	529
	.half	667
	.half	530
	.half	667
	.half	531
	.half	667
	.half	532
	.half	667
	.half	533
	.half	667
	.half	534
	.half	667
	.half	535
	.half	667
	.half	2304
	.half	0
	.half	2305
	.half	0
	.half	2306
	.half	0
	.half	2307
	.half	0
	.half	2149
	.half	12816
	.half	2171
	.half	0
	.half	2172
	.half	-256
	.half	2173
	.half	0
	.half	2174
	.half	0
	.half	2049
	.half	256
	.half	2050
	.half	256
	.half	5888
	.half	332
	.half	769
	.half	255
	.half	4778
	.half	150
	.half	4927
	.half	48
	.half	4926
	.half	14
	.half	8735
	.half	5
	.half	8704
	.half	196
	.half	8735
	.half	0
	.half	8720
	.half	1519
	.half	8708
	.half	1505
	.half	8704
	.half	4928
	.half	4927
	.half	16
	.half	8352
	.half	6464
	.half	8384
	.half	6464
	.half	8416
	.half	6464
	.half	-1
	.half	-21555
	.align	2
$LC5:
	.half	4927
	.half	48
	.half	4926
	.half	14
	.half	8735
	.half	0
	.half	8725
	.half	4102
	.half	8735
	.half	5
	.half	8704
	.half	198
	.half	8735
	.half	7
	.half	8734
	.half	72
	.half	8725
	.half	25618
	.half	8726
	.half	25618
	.half	8727
	.half	25618
	.half	8728
	.half	25618
	.half	8729
	.half	25618
	.half	8730
	.half	25618
	.half	8735
	.half	1
	.half	8716
	.half	-9232
	.half	8713
	.half	9590
	.half	8711
	.half	10366
	.half	8714
	.half	26853
	.half	8733
	.half	15780
	.half	8732
	.half	-6153
	.half	8724
	.half	32594
	.half	8728
	.half	32718
	.half	8712
	.half	1207
	.half	8710
	.half	16498
	.half	8720
	.half	-4002
	.half	8731
	.half	-19436
	.half	8735
	.half	3
	.half	8730
	.half	1702
	.half	8720
	.half	-4002
	.half	8723
	.half	1771
	.half	8722
	.half	-2862
	.half	8718
	.half	-7904
	.half	8704
	.half	31744
	.half	8706
	.half	24528
	.half	8717
	.half	519
	.half	8735
	.half	2
	.half	8709
	.half	2424
	.half	8706
	.half	-29695
	.half	8711
	.half	13856
	.half	8732
	.half	1
	.half	8707
	.half	1056
	.half	8708
	.half	-32568
	.half	4926
	.half	3806
	.half	8735
	.half	2
	.half	8716
	.half	115
	.half	8717
	.half	-5275
	.half	8718
	.half	20945
	.half	8719
	.half	24011
	.half	8720
	.half	12356
	.half	8721
	.half	6144
	.half	8722
	.half	32256
	.half	8723
	.half	0
	.half	4927
	.half	16
	.half	4926
	.half	4094
	.half	8319
	.half	2
	.half	8308
	.half	15650
	.half	8309
	.half	8192
	.half	8310
	.half	24640
	.half	8311
	.half	0
	.half	8312
	.half	3850
	.half	8313
	.half	20651
	.half	8314
	.half	0
	.half	8315
	.half	3855
	.half	8287
	.half	2
	.half	8276
	.half	-256
	.half	8277
	.half	10
	.half	8278
	.half	10
	.half	8279
	.half	5
	.half	8280
	.half	5
	.half	8281
	.half	0
	.half	8282
	.half	5
	.half	8283
	.half	5
	.half	8284
	.half	5
	.half	8351
	.half	2
	.half	8340
	.half	170
	.half	8341
	.half	170
	.half	8342
	.half	170
	.half	8343
	.half	170
	.half	8344
	.half	85
	.half	8345
	.half	170
	.half	8346
	.half	170
	.half	8347
	.half	170
	.half	4963
	.half	-31916
	.half	4720
	.half	13107
	.half	4721
	.half	13107
	.half	4722
	.half	13107
	.half	4912
	.half	219
	.half	4611
	.half	-256
	.half	4608
	.half	32708
	.half	4637
	.half	6918
	.half	4638
	.half	2032
	.half	4639
	.half	1080
	.half	4640
	.half	1039
	.half	4641
	.half	1039
	.half	4642
	.half	1003
	.half	4643
	.half	1080
	.half	4644
	.half	1039
	.half	4645
	.half	1039
	.half	4646
	.half	1003
	.half	4647
	.half	324
	.half	4648
	.half	312
	.half	4655
	.half	324
	.half	4656
	.half	312
	.half	4649
	.half	32
	.half	4650
	.half	12
	.half	4657
	.half	48
	.half	4658
	.half	36
	.half	537
	.half	50
	.half	512
	.half	2000
	.half	513
	.half	2000
	.half	514
	.half	2000
	.half	515
	.half	2000
	.half	516
	.half	2000
	.half	517
	.half	2000
	.half	518
	.half	2000
	.half	519
	.half	2000
	.half	536
	.half	50
	.half	520
	.half	400
	.half	521
	.half	400
	.half	522
	.half	400
	.half	523
	.half	400
	.half	524
	.half	400
	.half	525
	.half	400
	.half	526
	.half	400
	.half	527
	.half	400
	.half	528
	.half	400
	.half	529
	.half	400
	.half	530
	.half	400
	.half	531
	.half	400
	.half	532
	.half	400
	.half	533
	.half	400
	.half	534
	.half	400
	.half	535
	.half	400
	.half	2304
	.half	0
	.half	2305
	.half	0
	.half	2306
	.half	0
	.half	2307
	.half	0
	.half	2149
	.half	12816
	.half	2171
	.half	0
	.half	2172
	.half	-256
	.half	2173
	.half	0
	.half	2174
	.half	0
	.half	2049
	.half	256
	.half	2050
	.half	256
	.half	5888
	.half	293
	.half	769
	.half	255
	.half	4778
	.half	150
	.half	4927
	.half	48
	.half	4926
	.half	14
	.half	8735
	.half	5
	.half	8704
	.half	196
	.half	8735
	.half	0
	.half	8720
	.half	1519
	.half	8708
	.half	1505
	.half	8704
	.half	4928
	.half	4927
	.half	16
	.half	-1
	.half	-21555
	.align	2
$LC6:
	.half	6948
	.half	0
	.half	6949
	.half	0
	.half	6950
	.half	0
	.half	6951
	.half	0
	.half	8319
	.half	2
	.half	8313
	.half	512
	.half	8319
	.half	0
	.half	4927
	.half	48
	.half	4926
	.half	14
	.half	8735
	.half	5
	.half	8705
	.half	1792
	.half	8709
	.half	-29822
	.half	8710
	.half	1483
	.half	8735
	.half	2
	.half	8708
	.half	-32574
	.half	8709
	.half	2360
	.half	8735
	.half	3
	.half	8722
	.half	-15150
	.half	8717
	.half	519
	.half	8735
	.half	1
	.half	8711
	.half	9854
	.half	8732
	.half	-6665
	.half	8731
	.half	1060
	.half	8735
	.half	7
	.half	8734
	.half	64
	.half	8728
	.half	0
	.half	8735
	.half	7
	.half	8734
	.half	44
	.half	8728
	.half	139
	.half	8735
	.half	5
	.half	8709
	.half	-10
	.half	8710
	.half	128
	.half	8709
	.half	-32768
	.half	8710
	.half	-1824
	.half	8710
	.half	-8192
	.half	8710
	.half	-7712
	.half	8710
	.half	428
	.half	8710
	.half	9224
	.half	8710
	.half	-8053
	.half	8710
	.half	-31497
	.half	8710
	.half	8420
	.half	8710
	.half	-29820
	.half	8710
	.half	-1019
	.half	8710
	.half	-1798
	.half	8710
	.half	-4247
	.half	8710
	.half	-8053
	.half	8710
	.half	-31060
	.half	8710
	.half	8218
	.half	8710
	.half	-16512
	.half	8710
	.half	22992
	.half	8710
	.half	9218
	.half	8710
	.half	-32707
	.half	8710
	.half	-7968
	.half	8710
	.half	-6943
	.half	8710
	.half	-7963
	.half	8710
	.half	22534
	.half	8710
	.half	26816
	.half	8710
	.half	-11822
	.half	8710
	.half	-6944
	.half	8710
	.half	-6939
	.half	8710
	.half	-7963
	.half	8710
	.half	-4202
	.half	8710
	.half	-260
	.half	8710
	.half	1531
	.half	8710
	.half	3067
	.half	8710
	.half	22783
	.half	8710
	.half	-25071
	.half	8710
	.half	1776
	.half	8710
	.half	3201
	.half	8710
	.half	-29984
	.half	8710
	.half	25
	.half	8710
	.half	7049
	.half	8710
	.half	-12309
	.half	8710
	.half	6635
	.half	8710
	.half	6576
	.half	8710
	.half	-4097
	.half	8710
	.half	3071
	.half	8710
	.half	1061
	.half	8710
	.half	2055
	.half	8710
	.half	9792
	.half	8710
	.half	29223
	.half	8710
	.half	9854
	.half	8710
	.half	10244
	.half	8710
	.half	-18647
	.half	8710
	.half	9590
	.half	8710
	.half	10856
	.half	8710
	.half	-6869
	.half	8710
	.half	-21248
	.half	8710
	.half	11483
	.half	8710
	.half	-4051
	.half	8710
	.half	26555
	.half	8710
	.half	11899
	.half	8710
	.half	3887
	.half	8710
	.half	29541
	.half	8710
	.half	12716
	.half	8710
	.half	-13262
	.half	8710
	.half	8960
	.half	8710
	.half	13101
	.half	8710
	.half	5940
	.half	8710
	.half	32594
	.half	8710
	.half	13584
	.half	8710
	.half	54
	.half	8710
	.half	1536
	.half	8710
	.half	14092
	.half	8710
	.half	-16328
	.half	8710
	.half	32718
	.half	8710
	.half	15589
	.half	8710
	.half	-2243
	.half	8710
	.half	15780
	.half	8710
	.half	25904
	.half	8710
	.half	15975
	.half	8710
	.half	83
	.half	8710
	.half	27090
	.half	8710
	.half	3946
	.half	8710
	.half	300
	.half	8710
	.half	27691
	.half	8710
	.half	4974
	.half	8710
	.half	-7936
	.half	8710
	.half	28434
	.half	8710
	.half	-2191
	.half	8710
	.half	107
	.half	8710
	.half	29446
	.half	8710
	.half	-5260
	.half	8710
	.half	-27449
	.half	8710
	.half	30360
	.half	8710
	.half	2679
	.half	8710
	.half	20480
	.half	8710
	.half	30858
	.half	8710
	.half	5497
	.half	8710
	.half	32623
	.half	8710
	.half	31238
	.half	8710
	.half	-23040
	.half	8709
	.half	-29808
	.half	8710
	.half	-32768
	.half	8709
	.half	-29806
	.half	8710
	.half	-32768
	.half	8709
	.half	-29804
	.half	8710
	.half	-32748
	.half	8712
	.half	-6
	.half	8706
	.half	15461
	.half	8709
	.half	-10
	.half	8710
	.half	247
	.half	8735
	.half	0
	.half	8735
	.half	7
	.half	8734
	.half	66
	.half	8728
	.half	0
	.half	8734
	.half	45
	.half	8728
	.half	-4080
	.half	8734
	.half	32
	.half	8725
	.half	0
	.half	8734
	.half	35
	.half	8726
	.half	-32768
	.half	8735
	.half	0
	.half	4927
	.half	16
	.half	4926
	.half	4094
	.half	4962
	.half	277
	.half	4963
	.half	2
	.half	4963
	.half	0
	.half	4870
	.half	12
	.half	4871
	.half	12
	.half	4867
	.half	103
	.half	4868
	.half	17476
	.half	4611
	.half	-256
	.half	4608
	.half	32708
	.half	4637
	.half	32022
	.half	4638
	.half	1000
	.half	4639
	.half	590
	.half	4640
	.half	560
	.half	4641
	.half	580
	.half	4642
	.half	550
	.half	4643
	.half	590
	.half	4644
	.half	560
	.half	4645
	.half	580
	.half	4646
	.half	550
	.half	4647
	.half	192
	.half	4648
	.half	180
	.half	4655
	.half	192
	.half	4656
	.half	180
	.half	520
	.half	1000
	.half	521
	.half	1000
	.half	522
	.half	1000
	.half	523
	.half	1000
	.half	524
	.half	1000
	.half	525
	.half	1000
	.half	526
	.half	1000
	.half	527
	.half	1000
	.half	528
	.half	1000
	.half	529
	.half	1000
	.half	530
	.half	1000
	.half	531
	.half	1000
	.half	532
	.half	1000
	.half	533
	.half	1000
	.half	534
	.half	1000
	.half	535
	.half	1000
	.half	2304
	.half	0
	.half	2305
	.half	0
	.half	2306
	.half	0
	.half	2307
	.half	0
	.half	2149
	.half	12816
	.half	2171
	.half	0
	.half	2172
	.half	-256
	.half	2173
	.half	0
	.half	2174
	.half	0
	.half	2049
	.half	256
	.half	2050
	.half	256
	.half	2592
	.half	8256
	.half	2593
	.half	8256
	.half	2594
	.half	8256
	.half	2595
	.half	8256
	.half	2596
	.half	8256
	.half	2600
	.half	8256
	.half	2601
	.half	8256
	.half	4927
	.half	48
	.half	4926
	.half	14
	.half	8735
	.half	0
	.half	8704
	.half	4928
	.half	8735
	.half	0
	.half	4927
	.half	16
	.half	4926
	.half	4094
	.half	8352
	.half	6464
	.half	8384
	.half	6464
	.half	8416
	.half	6464
	.half	4876
	.half	80
	.half	-1
	.half	-21555
	.align	2
$LC7:
	.half	6948
	.half	0
	.half	6949
	.half	0
	.half	6950
	.half	0
	.half	6951
	.half	0
	.half	8319
	.half	2
	.half	8313
	.half	512
	.half	8319
	.half	0
	.half	4927
	.half	48
	.half	4926
	.half	14
	.half	8735
	.half	5
	.half	8705
	.half	1792
	.half	8709
	.half	-29822
	.half	8710
	.half	1483
	.half	8735
	.half	2
	.half	8708
	.half	-32574
	.half	8709
	.half	2360
	.half	8735
	.half	3
	.half	8722
	.half	-15150
	.half	8717
	.half	519
	.half	8735
	.half	1
	.half	8711
	.half	9854
	.half	8732
	.half	-6665
	.half	8731
	.half	1060
	.half	8735
	.half	7
	.half	8734
	.half	64
	.half	8728
	.half	0
	.half	8735
	.half	7
	.half	8734
	.half	44
	.half	8728
	.half	139
	.half	8735
	.half	5
	.half	8709
	.half	-10
	.half	8710
	.half	128
	.half	8709
	.half	-32768
	.half	8710
	.half	-1824
	.half	8710
	.half	-8192
	.half	8710
	.half	-7712
	.half	8710
	.half	428
	.half	8710
	.half	9224
	.half	8710
	.half	-8053
	.half	8710
	.half	-31497
	.half	8710
	.half	8420
	.half	8710
	.half	-29820
	.half	8710
	.half	-1019
	.half	8710
	.half	-1798
	.half	8710
	.half	-4247
	.half	8710
	.half	-8053
	.half	8710
	.half	-31060
	.half	8710
	.half	8218
	.half	8710
	.half	-16512
	.half	8710
	.half	22992
	.half	8710
	.half	9218
	.half	8710
	.half	-32707
	.half	8710
	.half	-7968
	.half	8710
	.half	-6943
	.half	8710
	.half	-7963
	.half	8710
	.half	22534
	.half	8710
	.half	26816
	.half	8710
	.half	-11822
	.half	8710
	.half	-6944
	.half	8710
	.half	-6939
	.half	8710
	.half	-7963
	.half	8710
	.half	-4202
	.half	8710
	.half	-260
	.half	8710
	.half	1531
	.half	8710
	.half	3067
	.half	8710
	.half	22783
	.half	8710
	.half	-25071
	.half	8710
	.half	1776
	.half	8710
	.half	3201
	.half	8710
	.half	-29984
	.half	8710
	.half	25
	.half	8710
	.half	7049
	.half	8710
	.half	-12309
	.half	8710
	.half	6635
	.half	8710
	.half	6576
	.half	8710
	.half	-4097
	.half	8710
	.half	3071
	.half	8710
	.half	1061
	.half	8710
	.half	2055
	.half	8710
	.half	9792
	.half	8710
	.half	29223
	.half	8710
	.half	9854
	.half	8710
	.half	10244
	.half	8710
	.half	-18647
	.half	8710
	.half	9590
	.half	8710
	.half	10856
	.half	8710
	.half	-6869
	.half	8710
	.half	-21248
	.half	8710
	.half	11483
	.half	8710
	.half	-4051
	.half	8710
	.half	26555
	.half	8710
	.half	11899
	.half	8710
	.half	3887
	.half	8710
	.half	29541
	.half	8710
	.half	12716
	.half	8710
	.half	-13262
	.half	8710
	.half	8960
	.half	8710
	.half	13101
	.half	8710
	.half	5940
	.half	8710
	.half	32594
	.half	8710
	.half	13584
	.half	8710
	.half	54
	.half	8710
	.half	1536
	.half	8710
	.half	14092
	.half	8710
	.half	-16328
	.half	8710
	.half	32718
	.half	8710
	.half	15589
	.half	8710
	.half	-2243
	.half	8710
	.half	15780
	.half	8710
	.half	25904
	.half	8710
	.half	15975
	.half	8710
	.half	83
	.half	8710
	.half	27090
	.half	8710
	.half	3946
	.half	8710
	.half	300
	.half	8710
	.half	27691
	.half	8710
	.half	4974
	.half	8710
	.half	-7936
	.half	8710
	.half	28434
	.half	8710
	.half	-2191
	.half	8710
	.half	107
	.half	8710
	.half	29446
	.half	8710
	.half	-5260
	.half	8710
	.half	-27449
	.half	8710
	.half	30360
	.half	8710
	.half	2679
	.half	8710
	.half	20480
	.half	8710
	.half	30858
	.half	8710
	.half	5497
	.half	8710
	.half	32623
	.half	8710
	.half	31238
	.half	8710
	.half	-23040
	.half	8709
	.half	-29808
	.half	8710
	.half	-32768
	.half	8709
	.half	-29806
	.half	8710
	.half	-32768
	.half	8709
	.half	-29804
	.half	8710
	.half	-32748
	.half	8712
	.half	-6
	.half	8706
	.half	15461
	.half	8709
	.half	-10
	.half	8710
	.half	247
	.half	8735
	.half	0
	.half	8735
	.half	7
	.half	8734
	.half	66
	.half	8728
	.half	0
	.half	8734
	.half	45
	.half	8728
	.half	-4080
	.half	8734
	.half	32
	.half	8725
	.half	0
	.half	8734
	.half	35
	.half	8726
	.half	-32768
	.half	8735
	.half	0
	.half	4927
	.half	16
	.half	4926
	.half	4094
	.half	4962
	.half	277
	.half	4963
	.half	2
	.half	4963
	.half	0
	.half	4870
	.half	12
	.half	4871
	.half	12
	.half	4867
	.half	103
	.half	4868
	.half	17476
	.half	4611
	.half	-256
	.half	4608
	.half	32708
	.half	2304
	.half	0
	.half	2305
	.half	0
	.half	2306
	.half	0
	.half	2307
	.half	0
	.half	2149
	.half	12816
	.half	2171
	.half	0
	.half	2172
	.half	-256
	.half	2173
	.half	0
	.half	2174
	.half	0
	.half	2049
	.half	256
	.half	2050
	.half	256
	.half	2592
	.half	8256
	.half	2593
	.half	8256
	.half	2594
	.half	8256
	.half	2595
	.half	8256
	.half	2596
	.half	8256
	.half	2597
	.half	8256
	.half	2598
	.half	8256
	.half	2599
	.half	8256
	.half	2600
	.half	8256
	.half	2601
	.half	8256
	.half	4927
	.half	48
	.half	4926
	.half	14
	.half	8735
	.half	0
	.half	8704
	.half	4928
	.half	8735
	.half	0
	.half	4927
	.half	16
	.half	4926
	.half	4094
	.half	6915
	.half	2166
	.half	-1
	.half	-21555
	.section	.rodata.str1.4
	.align	2
$LC8:
	.ascii	"(%s %d) init_rgmii fail \n\000"
	.align	2
$LC9:
	.ascii	"(%s %d) init_rgmii done \n\000"
	.text
	.align	2
	.globl	rtk_switch_init
	.ent	rtk_switch_init
	.type	rtk_switch_init, @function
rtk_switch_init:
$LFB21:
	.loc 1 11923 0
	.frame	$sp,3024,$31		# vars= 2976, regs= 5/0, args= 16, gp= 8
	.mask	0x800f0000,-8
	.fmask	0x00000000,0
	.set	noreorder
	.cpload	$25
	.set	nomacro
	
	addiu	$sp,$sp,-3024
$LCFI68:
	sw	$31,3016($sp)
$LCFI69:
	sw	$19,3012($sp)
$LCFI70:
	sw	$18,3008($sp)
$LCFI71:
	sw	$17,3004($sp)
$LCFI72:
	sw	$16,3000($sp)
$LCFI73:
	.cprestore	16
	.loc 1 11929 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	.loc 1 11923 0
	move	$17,$4
	.loc 1 11929 0
	li	$5,585			# 0x249
	jalr	$25
	li	$4,5058			# 0x13c2

	move	$16,$2
	bne	$16,$0,$L320
	lw	$28,16($sp)

	.loc 1 11932 0
	lw	$25,%call16(rtl8370_getAsicRegBits)($28)
	li	$4,4865			# 0x1301
	li	$5,61440			# 0xf000
	jalr	$25
	addiu	$6,$sp,2976

	move	$16,$2
	bne	$16,$0,$L320
	lw	$28,16($sp)

	.loc 1 11935 0
	lw	$25,%call16(rtl8370_setAsicPHYReg)($28)
	move	$4,$0
	li	$5,31			# 0x1f
	jalr	$25
	li	$6,5			# 0x5

	move	$16,$2
	bne	$16,$0,$L320
	lw	$28,16($sp)

	.loc 1 11937 0
	lw	$25,%call16(rtl8370_setAsicPHYReg)($28)
	move	$4,$0
	li	$5,5			# 0x5
	jalr	$25
	li	$6,16382			# 0x3ffe

	move	$16,$2
	bne	$16,$0,$L320
	lw	$28,16($sp)

	.loc 1 11939 0
	lw	$25,%call16(rtl8370_getAsicPHYReg)($28)
	move	$4,$0
	li	$5,6			# 0x6
	jalr	$25
	addiu	$6,$sp,2980

	move	$16,$2
	bne	$16,$0,$L320
	lw	$28,16($sp)

	.loc 1 11943 0
	lw	$5,2976($sp)
	li	$2,1			# 0x1
	.loc 1 11948 0
	.loc 1 11943 0
	bne	$5,$2,$L635
	lw	$4,%got($LC3)($28)

	lw	$3,2980($sp)
	li	$2,38123			# 0x94eb
	.loc 1 11948 0
	.loc 1 11943 0
	bne	$3,$2,$L636
	lw	$25,%call16(printf)($28)

$LBB2:
	.loc 1 11944 0
	lw	$4,%got($LC2)($28)
	li	$5,1			# 0x1
	addiu	$4,$4,%lo($LC2)
	jalr	$25
	li	$6,38123			# 0x94eb

	lw	$28,16($sp)
$LBE2:
	.loc 1 11945 0
	b	$L327
	move	$19,$0

$L635:
$LBB3:
	.loc 1 11948 0
	lw	$25,%call16(printf)($28)
$L636:
	lw	$6,2980($sp)
	addiu	$4,$4,%lo($LC3)
	.loc 1 11951 0
	.loc 1 11948 0
	jalr	$25
	move	$19,$17

	.loc 1 11949 0
	li	$2,1			# 0x1
	.loc 1 11948 0
	lw	$28,16($sp)
	.loc 1 11949 0
	sw	$2,2976($sp)
	.loc 1 11950 0
	li	$2,38123			# 0x94eb
	sw	$2,2980($sp)
$L327:
$LBE3:
	.loc 1 11956 0
	lw	$3,2976($sp)
	.loc 1 11961 0
	.loc 1 11956 0
	bne	$3,$0,$L328
	li	$2,1			# 0x1

$LBB4:
$LBB5:
	.loc 1 10438 0
	lw	$2,%got($LC4)($28)
	addiu	$16,$sp,24
	move	$8,$16
	addiu	$6,$2,%lo($LC4)
	addiu	$7,$6,640
$L330:
	lwl	$2,3($6)
	lwl	$3,7($6)
	lwl	$4,11($6)
	lwl	$5,15($6)
	lwr	$2,0($6)
	lwr	$3,4($6)
	lwr	$4,8($6)
	lwr	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($8)
	sw	$3,4($8)
	sw	$4,8($8)
	sw	$5,12($8)
	bne	$6,$7,$L330
	addiu	$8,$8,16

	lwl	$3,3($6)
	.loc 1 10480 0
	lw	$2,%got($LC5)($28)
	addiu	$7,$sp,672
	.loc 1 10438 0
	lwr	$3,0($6)
	.loc 1 10480 0
	addiu	$6,$2,%lo($LC5)
	addiu	$9,$6,624
	.loc 1 10438 0
	sw	$3,0($8)
$L331:
	.loc 1 10480 0
	lwl	$2,3($6)
	lwl	$3,7($6)
	lwl	$4,11($6)
	lwl	$5,15($6)
	lwr	$2,0($6)
	lwr	$3,4($6)
	lwr	$4,8($6)
	lwr	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$9,$L331
	addiu	$7,$7,16

	lwl	$2,3($6)
	lwl	$3,7($6)
	.loc 1 10521 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	.loc 1 10480 0
	lwr	$2,0($6)
	lwr	$3,4($6)
	.loc 1 10521 0
	li	$4,5058			# 0x13c2
	.loc 1 10480 0
	sw	$2,0($7)
	sw	$3,4($7)
	.loc 1 10521 0
	jalr	$25
	li	$5,585			# 0x249

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10524 0
	lw	$25,%call16(rtl8370_getAsicRegBits)($28)
	li	$4,4866			# 0x1302
	li	$5,7			# 0x7
	jalr	$25
	addiu	$6,$sp,2984

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10528 0
	lw	$3,2984($sp)
	li	$2,1			# 0x1
	.loc 1 10527 0
	.loc 1 10528 0
	beq	$3,$2,$L623
	move	$17,$0

	.loc 1 10576 0
	.loc 1 10528 0
	beq	$3,$0,$L336
	li	$2,65535			# 0xffff

	li	$2,2			# 0x2
	beq	$3,$2,$L383
	li	$3,-1			# 0xffffffffffffffff

	b	$L320
	move	$2,$3

$L336:
	.loc 1 10576 0
	lhu	$3,24($sp)
	beq	$3,$2,$L623
	move	$5,$16

	lhu	$3,26($sp)
	li	$2,43981			# 0xabcd
	beq	$3,$2,$L623
	move	$18,$0

	.loc 1 10533 0
	addu	$5,$18,$5
$L641:
	lhu	$4,0($5)
	li	$2,8192			# 0x2000
	andi	$3,$4,0xf000
	.loc 1 10573 0
	.loc 1 10533 0
	bne	$3,$2,$L339
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 10536 0
	li	$2,1			# 0x1
	sw	$2,2988($sp)
	.loc 1 10535 0
	move	$16,$0
	.loc 1 10540 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L638:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,2988

	.loc 1 10539 0
	addiu	$16,$16,1
	.loc 1 10540 0
	lw	$28,16($sp)
	bne	$2,$0,$L618
	sltu	$3,$16,5

	lw	$2,2988($sp)
	.loc 1 10543 0
	.loc 1 10540 0
	beq	$2,$0,$L637
	li	$2,5			# 0x5

	bne	$3,$0,$L638
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 10543 0
	li	$2,5			# 0x5
$L637:
	.loc 1 10546 0
	.loc 1 10543 0
	beq	$16,$2,$L616
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 10546 0
	li	$4,7939			# 0x1f03
	jalr	$25
	move	$5,$0

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10548 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7938			# 0x1f02
	jalr	$25
	li	$5,31			# 0x1f

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10550 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10554 0
	li	$2,1			# 0x1
	sw	$2,2988($sp)
	.loc 1 10553 0
	move	$16,$0
	.loc 1 10558 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L640:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,2988

	.loc 1 10557 0
	addiu	$16,$16,1
	.loc 1 10558 0
	lw	$28,16($sp)
	bne	$2,$0,$L618
	sltu	$3,$16,5

	lw	$2,2988($sp)
	.loc 1 10561 0
	.loc 1 10558 0
	beq	$2,$0,$L639
	li	$2,5			# 0x5

	bne	$3,$0,$L640
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 10561 0
	li	$2,5			# 0x5
$L639:
	.loc 1 10564 0
	.loc 1 10561 0
	beq	$16,$2,$L616
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 10564 0
	addiu	$2,$sp,24
	addu	$16,$18,$2
	lhu	$5,2($16)
	jalr	$25
	li	$4,7939			# 0x1f03

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10566 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	lhu	$5,0($16)
	jalr	$25
	li	$4,7938			# 0x1f02

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10568 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	beq	$2,$0,$L356
	lw	$28,16($sp)

	.loc 1 11103 0
	b	$L461
	move	$3,$2

$L339:
	.loc 1 10573 0
	jalr	$25
	lhu	$5,2($5)

	bne	$2,$0,$L619
	lw	$28,16($sp)

$L356:
	addiu	$18,$18,4
	.loc 1 10576 0
	addiu	$5,$sp,24
	addu	$4,$18,$5
	lhu	$3,0($4)
	li	$2,65535			# 0xffff
	beq	$3,$2,$L623
	addiu	$17,$17,1

	lhu	$3,2($4)
	li	$2,43981			# 0xabcd
	.loc 1 10533 0
	.loc 1 10576 0
	bne	$3,$2,$L641
	addu	$5,$18,$5

	.loc 1 10624 0
	b	$L632
	sll	$2,$17,2

$L622:
	lhu	$3,650($5)
	.loc 1 10581 0
	.loc 1 10624 0
	beq	$3,$2,$L385
	andi	$4,$4,0xffff

	.loc 1 10581 0
	andi	$3,$4,0xf000
	li	$2,8192			# 0x2000
	.loc 1 10621 0
	.loc 1 10581 0
	bne	$3,$2,$L362
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 10584 0
	li	$2,1			# 0x1
	sw	$2,2988($sp)
	.loc 1 10583 0
	move	$16,$0
	.loc 1 10588 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L643:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,2988

	.loc 1 10587 0
	addiu	$16,$16,1
	.loc 1 10588 0
	lw	$28,16($sp)
	bne	$2,$0,$L618
	sltu	$3,$16,5

	lw	$2,2988($sp)
	.loc 1 10591 0
	.loc 1 10588 0
	beq	$2,$0,$L642
	li	$2,5			# 0x5

	bne	$3,$0,$L643
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 10591 0
	li	$2,5			# 0x5
$L642:
	.loc 1 10594 0
	.loc 1 10591 0
	beq	$16,$2,$L616
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 10594 0
	li	$4,7939			# 0x1f03
	jalr	$25
	move	$5,$0

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10596 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7938			# 0x1f02
	jalr	$25
	li	$5,31			# 0x1f

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10598 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10602 0
	li	$2,1			# 0x1
	sw	$2,2988($sp)
	.loc 1 10601 0
	move	$16,$0
	.loc 1 10606 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L645:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,2988

	.loc 1 10605 0
	addiu	$16,$16,1
	.loc 1 10606 0
	lw	$28,16($sp)
	bne	$2,$0,$L618
	sltu	$3,$16,5

	lw	$2,2988($sp)
	.loc 1 10609 0
	.loc 1 10606 0
	beq	$2,$0,$L644
	li	$2,5			# 0x5

	bne	$3,$0,$L645
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 10609 0
	li	$2,5			# 0x5
$L644:
	.loc 1 10612 0
	.loc 1 10609 0
	beq	$16,$2,$L616
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 10612 0
	sll	$2,$17,2
	addiu	$3,$sp,24
	addu	$16,$2,$3
	lhu	$5,650($16)
	jalr	$25
	li	$4,7939			# 0x1f03

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10614 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	lhu	$5,648($16)
	jalr	$25
	li	$4,7938			# 0x1f02

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10616 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	beq	$2,$0,$L379
	lw	$28,16($sp)

	.loc 1 11103 0
	b	$L461
	move	$3,$2

$L362:
	.loc 1 10621 0
	jalr	$25
	lhu	$5,650($5)

	bne	$2,$0,$L619
	lw	$28,16($sp)

$L379:
	.loc 1 10624 0
	addiu	$17,$17,1
$L623:
	sll	$2,$17,2
$L632:
	addiu	$3,$sp,24
	addu	$5,$2,$3
	lhu	$4,648($5)
	li	$2,65535			# 0xffff
	andi	$3,$4,0xffff
	bne	$3,$2,$L622
	li	$2,43981			# 0xabcd

	.loc 1 10679 0
	b	$L633
	lw	$25,%call16(rtl8370_setAsicReg)($28)

$L383:
	.loc 1 10676 0
	lhu	$3,672($sp)
	li	$2,65535			# 0xffff
	beq	$3,$2,$L385
	move	$5,$16

	lhu	$3,674($sp)
	li	$2,43981			# 0xabcd
	.loc 1 10679 0
	.loc 1 10676 0
	beq	$3,$2,$L633
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	move	$17,$0
	.loc 1 10633 0
	addu	$5,$17,$5
$L650:
	lhu	$4,648($5)
	li	$2,8192			# 0x2000
	andi	$3,$4,0xf000
	.loc 1 10673 0
	.loc 1 10633 0
	bne	$3,$2,$L386
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 10636 0
	li	$2,1			# 0x1
	sw	$2,2988($sp)
	.loc 1 10635 0
	move	$16,$0
	.loc 1 10640 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L647:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,2988

	.loc 1 10639 0
	addiu	$16,$16,1
	.loc 1 10640 0
	lw	$28,16($sp)
	bne	$2,$0,$L618
	sltu	$3,$16,5

	lw	$2,2988($sp)
	.loc 1 10643 0
	.loc 1 10640 0
	beq	$2,$0,$L646
	li	$2,5			# 0x5

	bne	$3,$0,$L647
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 10643 0
	li	$2,5			# 0x5
$L646:
	.loc 1 10646 0
	.loc 1 10643 0
	beq	$16,$2,$L616
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 10646 0
	li	$4,7939			# 0x1f03
	jalr	$25
	move	$5,$0

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10648 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7938			# 0x1f02
	jalr	$25
	li	$5,31			# 0x1f

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10650 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10654 0
	li	$2,1			# 0x1
	sw	$2,2988($sp)
	.loc 1 10653 0
	move	$16,$0
	.loc 1 10658 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L649:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,2988

	.loc 1 10657 0
	addiu	$16,$16,1
	.loc 1 10658 0
	lw	$28,16($sp)
	bne	$2,$0,$L618
	sltu	$3,$16,5

	lw	$2,2988($sp)
	.loc 1 10661 0
	.loc 1 10658 0
	beq	$2,$0,$L648
	li	$2,5			# 0x5

	bne	$3,$0,$L649
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 10661 0
	li	$2,5			# 0x5
$L648:
	.loc 1 10664 0
	.loc 1 10661 0
	beq	$16,$2,$L616
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 10664 0
	addiu	$2,$sp,24
	addu	$16,$17,$2
	lhu	$5,650($16)
	jalr	$25
	li	$4,7939			# 0x1f03

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10666 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	lhu	$5,648($16)
	jalr	$25
	li	$4,7938			# 0x1f02

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10668 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	beq	$2,$0,$L403
	lw	$28,16($sp)

	.loc 1 11103 0
	b	$L461
	move	$3,$2

$L386:
	.loc 1 10673 0
	jalr	$25
	lhu	$5,650($5)

	bne	$2,$0,$L619
	lw	$28,16($sp)

$L403:
	addiu	$17,$17,4
	.loc 1 10676 0
	addiu	$5,$sp,24
	addu	$4,$17,$5
	lhu	$3,648($4)
	li	$2,65535			# 0xffff
	beq	$3,$2,$L385
	li	$2,43981			# 0xabcd

	lhu	$3,650($4)
	.loc 1 10633 0
	.loc 1 10676 0
	bne	$3,$2,$L650
	addu	$5,$17,$5

$L385:
	.loc 1 10679 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
$L633:
	li	$4,5888			# 0x1700
	jalr	$25
	li	$5,309			# 0x135

	lw	$28,16($sp)
	bne	$2,$0,$L621
	li	$3,-1			# 0xffffffffffffffff

	.loc 1 10686 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,6926			# 0x1b0e
	jalr	$25
	li	$5,26231			# 0x6677

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10689 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,6927			# 0x1b0f
	jalr	$25
	li	$5,29702			# 0x7406

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10692 0
	lw	$25,%call16(rtl8370_getAsicReg)($28)
	li	$4,4876			# 0x130c
	jalr	$25
	addiu	$5,$sp,2984

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10695 0
	lw	$2,2984($sp)
	andi	$2,$2,0x1
	beq	$2,$0,$L411
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

	.loc 1 10697 0
	li	$4,6926			# 0x1b0e
	li	$5,14			# 0xe
	b	$L624
	move	$6,$0

$L411:
	.loc 1 10702 0
	li	$4,6926			# 0x1b0e
	li	$5,14			# 0xe
	li	$6,1			# 0x1
$L624:
	jalr	$25
	nop

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10706 0
	lw	$2,2984($sp)
	srl	$2,$2,4
	andi	$2,$2,0x1
	beq	$2,$0,$L415
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

	.loc 1 10708 0
	li	$4,6926			# 0x1b0e
	li	$5,8			# 0x8
	b	$L625
	move	$6,$0

$L415:
	.loc 1 10713 0
	li	$4,6926			# 0x1b0e
	li	$5,8			# 0x8
	li	$6,1			# 0x1
$L625:
	jalr	$25
	nop

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10717 0
	lw	$2,2984($sp)
	srl	$2,$2,5
	andi	$2,$2,0x1
	beq	$2,$0,$L419
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

	.loc 1 10719 0
	li	$4,6926			# 0x1b0e
	li	$5,10			# 0xa
	b	$L626
	move	$6,$0

$L419:
	.loc 1 10724 0
	li	$4,6926			# 0x1b0e
	li	$5,10			# 0xa
	li	$6,1			# 0x1
$L626:
	jalr	$25
	nop

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10728 0
	lw	$25,%call16(rtl8370_getAsicReg)($28)
	li	$4,4877			# 0x130d
	jalr	$25
	addiu	$5,$sp,2984

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10731 0
	lw	$2,2984($sp)
	srl	$2,$2,6
	andi	$2,$2,0x1
	beq	$2,$0,$L424
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

	.loc 1 10733 0
	li	$4,6927			# 0x1b0f
	move	$5,$0
	b	$L627
	li	$6,1			# 0x1

$L424:
	.loc 1 10738 0
	li	$4,6927			# 0x1b0f
	move	$5,$0
	move	$6,$0
$L627:
	jalr	$25
	nop

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10742 0
	lw	$2,2984($sp)
	srl	$2,$2,12
	andi	$2,$2,0x1
	beq	$2,$0,$L428
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

	.loc 1 10744 0
	li	$4,6926			# 0x1b0e
	li	$5,12			# 0xc
	b	$L628
	li	$6,1			# 0x1

$L428:
	.loc 1 10749 0
	li	$4,6926			# 0x1b0e
	li	$5,12			# 0xc
	move	$6,$0
$L628:
	jalr	$25
	nop

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10753 0
	lw	$25,%call16(rtl8370_getAsicReg)($28)
	li	$4,4918			# 0x1336
	jalr	$25
	addiu	$5,$sp,2984

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10756 0
	lw	$2,2984($sp)
	srl	$2,$2,4
	andi	$2,$2,0x1
	beq	$2,$0,$L433
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

	.loc 1 10758 0
	li	$4,6927			# 0x1b0f
	li	$5,4			# 0x4
	b	$L629
	li	$6,1			# 0x1

$L433:
	.loc 1 10763 0
	li	$4,6927			# 0x1b0f
	li	$5,4			# 0x4
	move	$6,$0
$L629:
	jalr	$25
	nop

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10767 0
	lw	$2,2984($sp)
	srl	$2,$2,5
	andi	$2,$2,0x1
	beq	$2,$0,$L437
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

	.loc 1 10769 0
	li	$4,6926			# 0x1b0e
	li	$5,6			# 0x6
	b	$L630
	li	$6,1			# 0x1

$L437:
	.loc 1 10774 0
	li	$4,6926			# 0x1b0e
	li	$5,6			# 0x6
	move	$6,$0
$L630:
	jalr	$25
	nop

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10778 0
	lw	$2,2984($sp)
	srl	$2,$2,6
	andi	$2,$2,0x1
	beq	$2,$0,$L441
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

	.loc 1 10780 0
	li	$4,6926			# 0x1b0e
	li	$5,4			# 0x4
	b	$L631
	li	$6,1			# 0x1

$L441:
	.loc 1 10785 0
	li	$4,6926			# 0x1b0e
	li	$5,4			# 0x4
	move	$6,$0
$L631:
	jalr	$25
	nop

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10789 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,6948			# 0x1b24
	jalr	$25
	move	$5,$0

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10792 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,6949			# 0x1b25
	jalr	$25
	move	$5,$0

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10795 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,6950			# 0x1b26
	jalr	$25
	move	$5,$0

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10798 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,6951			# 0x1b27
	jalr	$25
	move	$5,$0

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10802 0
	move	$17,$0
$L453:
	.loc 1 10804 0
	lw	$25,%call16(rtl8370_setAsicLutLearnLimitNo)($28)
	move	$4,$17
	jalr	$25
	li	$5,8256			# 0x2040

	.loc 1 10802 0
	addiu	$17,$17,1
	.loc 1 10804 0
	lw	$28,16($sp)
	.loc 1 10802 0
	.loc 1 10804 0
	bne	$2,$0,$L618
	sltu	$3,$17,11

	.loc 1 10802 0
	bne	$3,$0,$L453
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

	.loc 1 11976 0
	b	$L666
	li	$4,6912			# 0x1b00

$L328:
$LBE5:
$LBE4:
	.loc 1 11961 0
	bne	$3,$2,$L634
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

	.loc 1 11963 0
	lw	$3,2980($sp)
	li	$2,38123			# 0x94eb
	.loc 1 11968 0
	.loc 1 11963 0
	bne	$3,$2,$L456
	li	$2,8452			# 0x2104

$LBB6:
$LBB7:
	.loc 1 10820 0
	lw	$2,%got($LC6)($28)
	addiu	$7,$sp,1304
	addiu	$6,$2,%lo($LC6)
	addiu	$8,$6,896
$L458:
	lwl	$2,3($6)
	lwl	$3,7($6)
	lwl	$4,11($6)
	lwl	$5,15($6)
	lwr	$2,0($6)
	lwr	$3,4($6)
	lwr	$4,8($6)
	lwr	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$8,$L458
	addiu	$7,$7,16

	.loc 1 10877 0
	lw	$2,%got($LC7)($28)
	addiu	$7,$sp,2200
	addiu	$6,$2,%lo($LC7)
	addiu	$8,$6,768
$L459:
	lwl	$2,3($6)
	lwl	$3,7($6)
	lwl	$4,11($6)
	lwl	$5,15($6)
	lwr	$2,0($6)
	lwr	$3,4($6)
	lwr	$4,8($6)
	lwr	$5,12($6)
	addiu	$6,$6,16
	sw	$2,0($7)
	sw	$3,4($7)
	sw	$4,8($7)
	sw	$5,12($7)
	bne	$6,$8,$L459
	addiu	$7,$7,16

	lwl	$2,3($6)
	lwl	$3,7($6)
	.loc 1 10927 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	.loc 1 10877 0
	lwr	$2,0($6)
	lwr	$3,4($6)
	.loc 1 10927 0
	li	$4,5058			# 0x13c2
	.loc 1 10877 0
	sw	$2,0($7)
	sw	$3,4($7)
	.loc 1 10927 0
	jalr	$25
	li	$5,585			# 0x249

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10930 0
	lw	$25,%call16(rtl8370_getAsicRegBits)($28)
	li	$4,4866			# 0x1302
	li	$5,7			# 0x7
	jalr	$25
	addiu	$6,$sp,2992

	move	$3,$2
	bne	$2,$0,$L461
	lw	$28,16($sp)

	.loc 1 10966 0
	lw	$3,2992($sp)
	li	$2,1			# 0x1
	.loc 1 11064 0
	.loc 1 10966 0
	beq	$3,$2,$L487
	li	$2,65535			# 0xffff

	beq	$3,$0,$L464
	nop

	li	$2,2			# 0x2
	beq	$3,$2,$L510
	li	$3,-1			# 0xffffffffffffffff

	b	$L320
	move	$2,$3

$L464:
	.loc 1 11015 0
	lhu	$3,1304($sp)
	beq	$3,$2,$L454
	addiu	$5,$sp,24

	lhu	$3,1306($sp)
	li	$2,43981			# 0xabcd
	beq	$3,$2,$L454
	move	$17,$0

	.loc 1 10972 0
	addu	$5,$17,$5
$L655:
	lhu	$4,1280($5)
	li	$2,8192			# 0x2000
	andi	$3,$4,0xf000
	.loc 1 11012 0
	.loc 1 10972 0
	bne	$3,$2,$L467
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 10975 0
	li	$2,1			# 0x1
	sw	$2,2996($sp)
	.loc 1 10974 0
	move	$16,$0
	.loc 1 10979 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L652:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,2996

	.loc 1 10978 0
	addiu	$16,$16,1
	.loc 1 10979 0
	lw	$28,16($sp)
	bne	$2,$0,$L618
	sltu	$3,$16,5

	lw	$2,2996($sp)
	.loc 1 10982 0
	.loc 1 10979 0
	beq	$2,$0,$L651
	li	$2,5			# 0x5

	bne	$3,$0,$L652
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 10982 0
	li	$2,5			# 0x5
$L651:
	.loc 1 10985 0
	.loc 1 10982 0
	beq	$16,$2,$L616
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 10985 0
	li	$4,7939			# 0x1f03
	jalr	$25
	move	$5,$0

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10987 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7938			# 0x1f02
	jalr	$25
	li	$5,31			# 0x1f

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10989 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 10993 0
	li	$2,1			# 0x1
	sw	$2,2996($sp)
	.loc 1 10992 0
	move	$16,$0
	.loc 1 10997 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L654:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,2996

	.loc 1 10996 0
	addiu	$16,$16,1
	.loc 1 10997 0
	lw	$28,16($sp)
	bne	$2,$0,$L618
	sltu	$3,$16,5

	lw	$2,2996($sp)
	.loc 1 11000 0
	.loc 1 10997 0
	beq	$2,$0,$L653
	li	$2,5			# 0x5

	bne	$3,$0,$L654
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 11000 0
	li	$2,5			# 0x5
$L653:
	.loc 1 11003 0
	.loc 1 11000 0
	beq	$16,$2,$L616
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 11003 0
	addiu	$2,$sp,24
	addu	$16,$17,$2
	lhu	$5,1282($16)
	jalr	$25
	li	$4,7939			# 0x1f03

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 11005 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	lhu	$5,1280($16)
	jalr	$25
	li	$4,7938			# 0x1f02

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 11007 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	beq	$2,$0,$L484
	lw	$28,16($sp)

	.loc 1 11103 0
	b	$L461
	move	$3,$2

$L467:
	.loc 1 11012 0
	jalr	$25
	lhu	$5,1282($5)

	bne	$2,$0,$L619
	lw	$28,16($sp)

$L484:
	addiu	$17,$17,4
	.loc 1 11015 0
	addiu	$5,$sp,24
	addu	$4,$17,$5
	lhu	$3,1280($4)
	li	$2,65535			# 0xffff
	beq	$3,$2,$L454
	li	$2,43981			# 0xabcd

	lhu	$3,1282($4)
	.loc 1 10972 0
	.loc 1 11015 0
	bne	$3,$2,$L655
	addu	$5,$17,$5

	.loc 1 11976 0
	.loc 1 11017 0
	b	$L634
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

$L487:
	.loc 1 11064 0
	lhu	$3,2200($sp)
	beq	$3,$2,$L454
	addiu	$5,$sp,24

	lhu	$3,2202($sp)
	li	$2,43981			# 0xabcd
	beq	$3,$2,$L454
	move	$17,$0

	.loc 1 11021 0
	addu	$5,$17,$5
$L660:
	lhu	$4,2176($5)
	li	$2,8192			# 0x2000
	andi	$3,$4,0xf000
	.loc 1 11061 0
	.loc 1 11021 0
	bne	$3,$2,$L490
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 11024 0
	li	$2,1			# 0x1
	sw	$2,2996($sp)
	.loc 1 11023 0
	move	$16,$0
	.loc 1 11028 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L657:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,2996

	.loc 1 11027 0
	addiu	$16,$16,1
	.loc 1 11028 0
	lw	$28,16($sp)
	bne	$2,$0,$L618
	sltu	$3,$16,5

	lw	$2,2996($sp)
	.loc 1 11031 0
	.loc 1 11028 0
	beq	$2,$0,$L656
	li	$2,5			# 0x5

	bne	$3,$0,$L657
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 11031 0
	li	$2,5			# 0x5
$L656:
	.loc 1 11034 0
	.loc 1 11031 0
	beq	$16,$2,$L616
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 11034 0
	li	$4,7939			# 0x1f03
	jalr	$25
	move	$5,$0

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 11036 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7938			# 0x1f02
	jalr	$25
	li	$5,31			# 0x1f

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 11038 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 11042 0
	li	$2,1			# 0x1
	sw	$2,2996($sp)
	.loc 1 11041 0
	move	$16,$0
	.loc 1 11046 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L659:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,2996

	.loc 1 11045 0
	addiu	$16,$16,1
	.loc 1 11046 0
	lw	$28,16($sp)
	bne	$2,$0,$L618
	sltu	$3,$16,5

	lw	$2,2996($sp)
	.loc 1 11049 0
	.loc 1 11046 0
	beq	$2,$0,$L658
	li	$2,5			# 0x5

	bne	$3,$0,$L659
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 11049 0
	li	$2,5			# 0x5
$L658:
	.loc 1 11052 0
	.loc 1 11049 0
	beq	$16,$2,$L616
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 11052 0
	addiu	$2,$sp,24
	addu	$16,$17,$2
	lhu	$5,2178($16)
	jalr	$25
	li	$4,7939			# 0x1f03

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 11054 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	lhu	$5,2176($16)
	jalr	$25
	li	$4,7938			# 0x1f02

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 11056 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	beq	$2,$0,$L507
	lw	$28,16($sp)

	.loc 1 11103 0
	b	$L461
	move	$3,$2

$L490:
	.loc 1 11061 0
	jalr	$25
	lhu	$5,2178($5)

	bne	$2,$0,$L619
	lw	$28,16($sp)

$L507:
	addiu	$17,$17,4
	.loc 1 11064 0
	addiu	$5,$sp,24
	addu	$4,$17,$5
	lhu	$3,2176($4)
	li	$2,65535			# 0xffff
	beq	$3,$2,$L454
	li	$2,43981			# 0xabcd

	lhu	$3,2178($4)
	.loc 1 11021 0
	.loc 1 11064 0
	bne	$3,$2,$L660
	addu	$5,$17,$5

	.loc 1 11976 0
	.loc 1 11066 0
	b	$L634
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

$L510:
	.loc 1 11113 0
	lhu	$3,2200($sp)
	li	$2,65535			# 0xffff
	beq	$3,$2,$L454
	addiu	$5,$sp,24

	lhu	$3,2202($sp)
	li	$2,43981			# 0xabcd
	beq	$3,$2,$L454
	move	$17,$0

	.loc 1 11070 0
	addu	$5,$17,$5
$L665:
	lhu	$4,2176($5)
	li	$2,8192			# 0x2000
	andi	$3,$4,0xf000
	.loc 1 11110 0
	.loc 1 11070 0
	bne	$3,$2,$L513
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 11073 0
	li	$2,1			# 0x1
	sw	$2,2996($sp)
	.loc 1 11072 0
	move	$16,$0
	.loc 1 11077 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L662:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,2996

	.loc 1 11076 0
	addiu	$16,$16,1
	.loc 1 11077 0
	lw	$28,16($sp)
	bne	$2,$0,$L618
	sltu	$3,$16,5

	lw	$2,2996($sp)
	.loc 1 11080 0
	.loc 1 11077 0
	beq	$2,$0,$L661
	li	$2,5			# 0x5

	bne	$3,$0,$L662
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 11080 0
	li	$2,5			# 0x5
$L661:
	.loc 1 11083 0
	.loc 1 11080 0
	beq	$16,$2,$L616
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 11083 0
	li	$4,7939			# 0x1f03
	jalr	$25
	move	$5,$0

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 11085 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7938			# 0x1f02
	jalr	$25
	li	$5,31			# 0x1f

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 11087 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 11091 0
	li	$2,1			# 0x1
	sw	$2,2996($sp)
	.loc 1 11090 0
	move	$16,$0
	.loc 1 11095 0
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)
$L664:
	li	$4,7937			# 0x1f01
	li	$5,2			# 0x2
	jalr	$25
	addiu	$6,$sp,2996

	.loc 1 11094 0
	addiu	$16,$16,1
	.loc 1 11095 0
	lw	$28,16($sp)
	bne	$2,$0,$L618
	sltu	$3,$16,5

	lw	$2,2996($sp)
	.loc 1 11098 0
	.loc 1 11095 0
	beq	$2,$0,$L663
	li	$2,5			# 0x5

	bne	$3,$0,$L664
	lw	$25,%call16(rtl8370_getAsicRegBit)($28)

	.loc 1 11098 0
	li	$2,5			# 0x5
$L663:
	.loc 1 11101 0
	.loc 1 11098 0
	beq	$16,$2,$L616
	lw	$25,%call16(rtl8370_setAsicReg)($28)

	.loc 1 11101 0
	addiu	$2,$sp,24
	addu	$16,$17,$2
	lhu	$5,2178($16)
	jalr	$25
	li	$4,7939			# 0x1f03

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 11103 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	lhu	$5,2176($16)
	jalr	$25
	li	$4,7938			# 0x1f02

	bne	$2,$0,$L618
	lw	$28,16($sp)

	.loc 1 11105 0
	lw	$25,%call16(rtl8370_setAsicReg)($28)
	li	$4,7936			# 0x1f00
	jalr	$25
	li	$5,3			# 0x3

	beq	$2,$0,$L530
	lw	$28,16($sp)

	.loc 1 11103 0
	b	$L461
	move	$3,$2

$L513:
	.loc 1 11110 0
	jalr	$25
	lhu	$5,2178($5)

	bne	$2,$0,$L619
	lw	$28,16($sp)

$L530:
	addiu	$17,$17,4
	.loc 1 11113 0
	addiu	$5,$sp,24
	addu	$4,$17,$5
	lhu	$3,2176($4)
	li	$2,65535			# 0xffff
	beq	$3,$2,$L454
	li	$2,43981			# 0xabcd

	lhu	$3,2178($4)
	.loc 1 11070 0
	.loc 1 11113 0
	bne	$3,$2,$L665
	addu	$5,$17,$5

	.loc 1 11976 0
	.loc 1 11115 0
	b	$L634
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

$L616:
	.loc 1 11098 0
	b	$L621
	li	$3,10			# 0xa

$L618:
	.loc 1 11103 0
	b	$L461
	move	$3,$2

$L619:
	.loc 1 11110 0
	b	$L621
	li	$3,-1			# 0xffffffffffffffff

$L461:
$LBE7:
$LBE6:
	.loc 1 11976 0
	.loc 1 10814 0
	beq	$3,$0,$L634
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

$L621:
	.loc 1 11966 0
	b	$L320
	move	$2,$3

$L456:
	.loc 1 11976 0
	.loc 1 11968 0
	bne	$3,$2,$L634
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)

	.loc 1 11970 0
	lw	$25,%got(_rtk_switch_init2)($28)
	addiu	$25,$25,%lo(_rtk_switch_init2)
	jalr	$25
	nop

	move	$16,$2
	bne	$16,$0,$L320
	lw	$28,16($sp)

$L454:
	.loc 1 11976 0
	lw	$25,%call16(rtl8370_setAsicRegBit)($28)
$L634:
	li	$4,6912			# 0x1b00
$L666:
	li	$5,3			# 0x3
	jalr	$25
	move	$6,$0

	move	$16,$2
	bne	$16,$0,$L320
	lw	$28,16($sp)

	.loc 1 11979 0
	lw	$25,%call16(init_rgmii)($28)
	jalr	$25
	move	$4,$19

	lw	$28,16($sp)
	move	$16,$2
	lw	$25,%call16(printf)($28)
	beq	$2,$0,$L538
	lw	$5,%got(__FUNCTION__.0)($28)

$LBB8:
	.loc 1 11980 0
	lw	$4,%got($LC8)($28)
	addiu	$5,$5,%lo(__FUNCTION__.0)
	li	$6,11980			# 0x2ecc
	jalr	$25
	addiu	$4,$4,%lo($LC8)

	lw	$28,16($sp)
	.loc 1 11981 0
	b	$L320
	move	$2,$16

$L538:
$LBE8:
	.loc 1 11983 0
	lw	$4,%got($LC9)($28)
	addiu	$5,$5,%lo(__FUNCTION__.0)
	li	$6,11983			# 0x2ecf
	jalr	$25
	addiu	$4,$4,%lo($LC9)

	lw	$28,16($sp)
	.loc 1 11985 0
	move	$2,$0
$L320:
	.loc 1 11986 0
	lw	$31,3016($sp)
	lw	$19,3012($sp)
	lw	$18,3008($sp)
	lw	$17,3004($sp)
	lw	$16,3000($sp)
	j	$31
	addiu	$sp,$sp,3024

	.set	macro
	.set	reorder
$LFE21:
	.end	rtk_switch_init
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
	.4byte	$LFB4
	.4byte	$LFE4-$LFB4
	.byte	0x4
	.4byte	$LCFI0-$LFB4
	.byte	0xe
	.uleb128 0x30
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
$LEFDE0:
$LSFDE2:
	.4byte	$LEFDE2-$LASFDE2
$LASFDE2:
	.4byte	$Lframe0
	.4byte	$LFB5
	.4byte	$LFE5-$LFB5
	.byte	0x4
	.4byte	$LCFI4-$LFB5
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.4byte	$LCFI7-$LCFI4
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
	.4byte	$LFB6
	.4byte	$LFE6-$LFB6
	.byte	0x4
	.4byte	$LCFI8-$LFB6
	.byte	0xe
	.uleb128 0x58
	.byte	0x4
	.4byte	$LCFI12-$LCFI8
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
	.4byte	$LFB7
	.4byte	$LFE7-$LFB7
	.byte	0x4
	.4byte	$LCFI13-$LFB7
	.byte	0xe
	.uleb128 0x48
	.byte	0x4
	.4byte	$LCFI18-$LCFI13
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
$LEFDE6:
$LSFDE8:
	.4byte	$LEFDE8-$LASFDE8
$LASFDE8:
	.4byte	$Lframe0
	.4byte	$LFB8
	.4byte	$LFE8-$LFB8
	.byte	0x4
	.4byte	$LCFI19-$LFB8
	.byte	0xe
	.uleb128 0x90
	.byte	0x4
	.4byte	$LCFI28-$LCFI19
	.byte	0x11
	.uleb128 0x10
	.sleb128 -10
	.byte	0x11
	.uleb128 0x11
	.sleb128 -9
	.byte	0x11
	.uleb128 0x12
	.sleb128 -8
	.byte	0x11
	.uleb128 0x13
	.sleb128 -7
	.byte	0x11
	.uleb128 0x14
	.sleb128 -6
	.byte	0x11
	.uleb128 0x15
	.sleb128 -5
	.byte	0x11
	.uleb128 0x16
	.sleb128 -4
	.byte	0x11
	.uleb128 0x17
	.sleb128 -3
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -2
	.align	2
$LEFDE8:
$LSFDE10:
	.4byte	$LEFDE10-$LASFDE10
$LASFDE10:
	.4byte	$Lframe0
	.4byte	$LFB12
	.4byte	$LFE12-$LFB12
	.align	2
$LEFDE10:
$LSFDE12:
	.4byte	$LEFDE12-$LASFDE12
$LASFDE12:
	.4byte	$Lframe0
	.4byte	$LFB11
	.4byte	$LFE11-$LFB11
	.byte	0x4
	.4byte	$LCFI29-$LFB11
	.byte	0xe
	.uleb128 0x618
	.byte	0x4
	.4byte	$LCFI32-$LCFI29
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
	.4byte	$LFB14
	.4byte	$LFE14-$LFB14
	.byte	0x4
	.4byte	$LCFI33-$LFB14
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.4byte	$LCFI36-$LCFI33
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
$LEFDE14:
$LSFDE16:
	.4byte	$LEFDE16-$LASFDE16
$LASFDE16:
	.4byte	$Lframe0
	.4byte	$LFB13
	.4byte	$LFE13-$LFB13
	.byte	0x4
	.4byte	$LCFI37-$LFB13
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.4byte	$LCFI40-$LCFI37
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
$LEFDE16:
$LSFDE18:
	.4byte	$LEFDE18-$LASFDE18
$LASFDE18:
	.4byte	$Lframe0
	.4byte	$LFB15
	.4byte	$LFE15-$LFB15
	.byte	0x4
	.4byte	$LCFI41-$LFB15
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	$LCFI44-$LCFI41
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
$LEFDE18:
$LSFDE20:
	.4byte	$LEFDE20-$LASFDE20
$LASFDE20:
	.4byte	$Lframe0
	.4byte	$LFB3
	.4byte	$LFE3-$LFB3
	.byte	0x4
	.4byte	$LCFI45-$LFB3
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	$LCFI47-$LCFI45
	.byte	0x11
	.uleb128 0x10
	.sleb128 -2
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -1
	.align	2
$LEFDE20:
$LSFDE22:
	.4byte	$LEFDE22-$LASFDE22
$LASFDE22:
	.4byte	$Lframe0
	.4byte	$LFB2
	.4byte	$LFE2-$LFB2
	.byte	0x4
	.4byte	$LCFI48-$LFB2
	.byte	0xe
	.uleb128 0x28
	.byte	0x4
	.4byte	$LCFI50-$LCFI48
	.byte	0x11
	.uleb128 0x10
	.sleb128 -2
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -1
	.align	2
$LEFDE22:
$LSFDE24:
	.4byte	$LEFDE24-$LASFDE24
$LASFDE24:
	.4byte	$Lframe0
	.4byte	$LFB16
	.4byte	$LFE16-$LFB16
	.byte	0x4
	.4byte	$LCFI51-$LFB16
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.4byte	$LCFI55-$LCFI51
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
$LEFDE24:
$LSFDE26:
	.4byte	$LEFDE26-$LASFDE26
$LASFDE26:
	.4byte	$Lframe0
	.4byte	$LFB17
	.4byte	$LFE17-$LFB17
	.byte	0x4
	.4byte	$LCFI56-$LFB17
	.byte	0xe
	.uleb128 0x30
	.byte	0x4
	.4byte	$LCFI60-$LCFI56
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
$LEFDE26:
$LSFDE28:
	.4byte	$LEFDE28-$LASFDE28
$LASFDE28:
	.4byte	$Lframe0
	.4byte	$LFB18
	.4byte	$LFE18-$LFB18
	.byte	0x4
	.4byte	$LCFI61-$LFB18
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	$LCFI62-$LCFI61
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -2
	.align	2
$LEFDE28:
$LSFDE30:
	.4byte	$LEFDE30-$LASFDE30
$LASFDE30:
	.4byte	$Lframe0
	.4byte	$LFB19
	.4byte	$LFE19-$LFB19
	.byte	0x4
	.4byte	$LCFI63-$LFB19
	.byte	0xe
	.uleb128 0x20
	.byte	0x4
	.4byte	$LCFI64-$LCFI63
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -2
	.align	2
$LEFDE30:
$LSFDE32:
	.4byte	$LEFDE32-$LASFDE32
$LASFDE32:
	.4byte	$Lframe0
	.4byte	$LFB20
	.4byte	$LFE20-$LFB20
	.byte	0x4
	.4byte	$LCFI65-$LFB20
	.byte	0xe
	.uleb128 0x48
	.byte	0x4
	.4byte	$LCFI67-$LCFI65
	.byte	0x11
	.uleb128 0x10
	.sleb128 -2
	.byte	0x11
	.uleb128 0x1f
	.sleb128 -1
	.align	2
$LEFDE32:
$LSFDE34:
	.4byte	$LEFDE34-$LASFDE34
$LASFDE34:
	.4byte	$Lframe0
	.4byte	$LFB21
	.4byte	$LFE21-$LFB21
	.byte	0x4
	.4byte	$LCFI68-$LFB21
	.byte	0xe
	.uleb128 0xbd0
	.byte	0x4
	.4byte	$LCFI73-$LCFI68
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
$LEFDE34:
	.align	0
	.file 2 "/home/IOData/BR6456IN/srcbase/boot/GPL_4100/include/realtek/rtk_types.h"
	.file 3 "/home/IOData/BR6456IN/srcbase/boot/GPL_4100/include/realtek/rtk_api.h"
	.file 4 "/home/IOData/BR6456IN/srcbase/boot/GPL_4100/include/realtek/rtl8370_asicdrv_vlan.h"
	.file 5 "/home/IOData/BR6456IN/srcbase/boot/GPL_4100/include/realtek/rtl8370_asicdrv_port.h"
	.text
$Letext0:
	.section	.debug_info
	.4byte	0x100f
	.2byte	0x2
	.4byte	$Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	$Ldebug_line0
	.4byte	$Letext0
	.4byte	$Ltext0
	.4byte	$LASF139
	.byte	0x1
	.4byte	$LASF140
	.4byte	$LASF141
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
	.byte	0x2
	.byte	0x4d
	.4byte	0x45
	.uleb128 0x3
	.4byte	$LASF11
	.byte	0x2
	.byte	0x4e
	.4byte	0x45
	.uleb128 0x5
	.4byte	0x185
	.4byte	$LASF26
	.byte	0x2
	.byte	0x5
	.byte	0x38
	.uleb128 0x6
	.4byte	$LASF12
	.byte	0x5
	.byte	0x3a
	.4byte	0x57
	.byte	0x2
	.byte	0x2
	.byte	0xe
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.4byte	$LASF13
	.byte	0x5
	.byte	0x3b
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0xd
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.4byte	$LASF14
	.byte	0x5
	.byte	0x3c
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0xc
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.4byte	$LASF15
	.byte	0x5
	.byte	0x3d
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0xb
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.4byte	$LASF16
	.byte	0x5
	.byte	0x3e
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0xa
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.4byte	$LASF17
	.byte	0x5
	.byte	0x3f
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0x9
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.4byte	$LASF18
	.byte	0x5
	.byte	0x40
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0x8
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.4byte	$LASF19
	.byte	0x5
	.byte	0x41
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0x7
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.4byte	$LASF20
	.byte	0x5
	.byte	0x42
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0x6
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.4byte	$LASF21
	.byte	0x5
	.byte	0x43
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0x5
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.4byte	$LASF22
	.byte	0x5
	.byte	0x44
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0x4
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.4byte	$LASF23
	.byte	0x5
	.byte	0x45
	.4byte	0x57
	.byte	0x2
	.byte	0x1
	.byte	0x3
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x6
	.4byte	$LASF24
	.byte	0x5
	.byte	0x46
	.4byte	0x57
	.byte	0x2
	.byte	0x3
	.byte	0x0
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.byte	0x0
	.uleb128 0x3
	.4byte	$LASF25
	.byte	0x5
	.byte	0x57
	.4byte	0x9b
	.uleb128 0x5
	.4byte	0x21b
	.4byte	$LASF27
	.byte	0x12
	.byte	0x4
	.byte	0x39
	.uleb128 0x7
	.4byte	$LASF28
	.byte	0x4
	.byte	0x3a
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x7
	.4byte	$LASF29
	.byte	0x4
	.byte	0x3b
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x2
	.uleb128 0x8
	.ascii	"mbr\000"
	.byte	0x4
	.byte	0x3c
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x8
	.ascii	"fid\000"
	.byte	0x4
	.byte	0x3d
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x6
	.uleb128 0x7
	.4byte	$LASF30
	.byte	0x4
	.byte	0x3e
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x8
	.uleb128 0x7
	.4byte	$LASF31
	.byte	0x4
	.byte	0x3f
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0xa
	.uleb128 0x7
	.4byte	$LASF32
	.byte	0x4
	.byte	0x40
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0xc
	.uleb128 0x7
	.4byte	$LASF33
	.byte	0x4
	.byte	0x41
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0xe
	.uleb128 0x7
	.4byte	$LASF34
	.byte	0x4
	.byte	0x42
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x10
	.byte	0x0
	.uleb128 0x3
	.4byte	$LASF35
	.byte	0x4
	.byte	0x43
	.4byte	0x190
	.uleb128 0x5
	.4byte	0x2bf
	.4byte	$LASF36
	.byte	0x14
	.byte	0x4
	.byte	0x68
	.uleb128 0x8
	.ascii	"vid\000"
	.byte	0x4
	.byte	0x6a
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x7
	.4byte	$LASF29
	.byte	0x4
	.byte	0x6b
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x2
	.uleb128 0x8
	.ascii	"mbr\000"
	.byte	0x4
	.byte	0x6c
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF37
	.byte	0x4
	.byte	0x6d
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x6
	.uleb128 0x8
	.ascii	"fid\000"
	.byte	0x4
	.byte	0x6e
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x8
	.uleb128 0x7
	.4byte	$LASF30
	.byte	0x4
	.byte	0x6f
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0xa
	.uleb128 0x7
	.4byte	$LASF31
	.byte	0x4
	.byte	0x70
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0xc
	.uleb128 0x7
	.4byte	$LASF32
	.byte	0x4
	.byte	0x71
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0xe
	.uleb128 0x7
	.4byte	$LASF33
	.byte	0x4
	.byte	0x72
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x10
	.uleb128 0x7
	.4byte	$LASF34
	.byte	0x4
	.byte	0x73
	.4byte	0x57
	.byte	0x2
	.byte	0x10
	.uleb128 0x12
	.byte	0x0
	.uleb128 0x3
	.4byte	$LASF38
	.byte	0x4
	.byte	0x75
	.4byte	0x226
	.uleb128 0x9
	.4byte	0x2fd
	.byte	0xc
	.byte	0x4
	.byte	0xa3
	.uleb128 0x7
	.4byte	$LASF39
	.byte	0x4
	.byte	0xa0
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0x7
	.4byte	$LASF40
	.byte	0x4
	.byte	0xa1
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0x7
	.4byte	$LASF41
	.byte	0x4
	.byte	0xa2
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x8
	.byte	0x0
	.uleb128 0x3
	.4byte	$LASF42
	.byte	0x4
	.byte	0xa3
	.4byte	0x2ca
	.uleb128 0x3
	.4byte	$LASF43
	.byte	0x3
	.byte	0xbc
	.4byte	0x33
	.uleb128 0xa
	.4byte	0x356
	.4byte	$LASF55
	.byte	0x4
	.byte	0x3
	.byte	0xce
	.uleb128 0xb
	.4byte	$LASF44
	.sleb128 0
	.uleb128 0xb
	.4byte	$LASF45
	.sleb128 1
	.uleb128 0xb
	.4byte	$LASF46
	.sleb128 2
	.uleb128 0xb
	.4byte	$LASF47
	.sleb128 3
	.uleb128 0xb
	.4byte	$LASF48
	.sleb128 4
	.uleb128 0xb
	.4byte	$LASF49
	.sleb128 5
	.uleb128 0xb
	.4byte	$LASF50
	.sleb128 6
	.uleb128 0xb
	.4byte	$LASF51
	.sleb128 7
	.uleb128 0xb
	.4byte	$LASF52
	.sleb128 8
	.byte	0x0
	.uleb128 0x3
	.4byte	$LASF53
	.byte	0x3
	.byte	0xd8
	.4byte	0x313
	.uleb128 0x3
	.4byte	$LASF54
	.byte	0x3
	.byte	0xf1
	.4byte	0x33
	.uleb128 0xc
	.4byte	0x392
	.4byte	$LASF56
	.byte	0x4
	.byte	0x3
	.2byte	0x334
	.uleb128 0xb
	.4byte	$LASF57
	.sleb128 0
	.uleb128 0xb
	.4byte	$LASF58
	.sleb128 1
	.uleb128 0xb
	.4byte	$LASF59
	.sleb128 2
	.uleb128 0xb
	.4byte	$LASF60
	.sleb128 3
	.byte	0x0
	.uleb128 0xd
	.4byte	$LASF61
	.byte	0x3
	.2byte	0x339
	.4byte	0x36c
	.uleb128 0xc
	.4byte	0x3ca
	.4byte	$LASF62
	.byte	0x4
	.byte	0x3
	.2byte	0x33c
	.uleb128 0xb
	.4byte	$LASF63
	.sleb128 0
	.uleb128 0xb
	.4byte	$LASF64
	.sleb128 1
	.uleb128 0xb
	.4byte	$LASF65
	.sleb128 2
	.uleb128 0xb
	.4byte	$LASF66
	.sleb128 3
	.uleb128 0xb
	.4byte	$LASF67
	.sleb128 4
	.byte	0x0
	.uleb128 0xd
	.4byte	$LASF68
	.byte	0x3
	.2byte	0x342
	.4byte	0x39e
	.uleb128 0xd
	.4byte	$LASF69
	.byte	0x3
	.2byte	0x373
	.4byte	0x33
	.uleb128 0xe
	.4byte	0x477
	.4byte	$LASF70
	.byte	0x24
	.byte	0x3
	.2byte	0x384
	.uleb128 0xf
	.4byte	$LASF23
	.byte	0x3
	.2byte	0x385
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.uleb128 0xf
	.4byte	$LASF12
	.byte	0x3
	.2byte	0x386
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x4
	.uleb128 0xf
	.4byte	$LASF13
	.byte	0x3
	.2byte	0x387
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x8
	.uleb128 0xf
	.4byte	$LASF15
	.byte	0x3
	.2byte	0x388
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0xc
	.uleb128 0xf
	.4byte	$LASF18
	.byte	0x3
	.2byte	0x389
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x10
	.uleb128 0xf
	.4byte	$LASF17
	.byte	0x3
	.2byte	0x38a
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x14
	.uleb128 0xf
	.4byte	$LASF16
	.byte	0x3
	.2byte	0x38b
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x18
	.uleb128 0xf
	.4byte	$LASF21
	.byte	0x3
	.2byte	0x38c
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x1c
	.uleb128 0xf
	.4byte	$LASF22
	.byte	0x3
	.2byte	0x38d
	.4byte	0x33
	.byte	0x2
	.byte	0x10
	.uleb128 0x20
	.byte	0x0
	.uleb128 0xd
	.4byte	$LASF71
	.byte	0x3
	.2byte	0x38e
	.4byte	0x3e2
	.uleb128 0xe
	.4byte	0x4a0
	.4byte	$LASF72
	.byte	0x4
	.byte	0x3
	.2byte	0x3c0
	.uleb128 0xf
	.4byte	$LASF73
	.byte	0x3
	.2byte	0x3c1
	.4byte	0x4a0
	.byte	0x2
	.byte	0x10
	.uleb128 0x0
	.byte	0x0
	.uleb128 0x10
	.4byte	0x4b0
	.4byte	0x33
	.uleb128 0x11
	.4byte	0x7e
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
	.4byte	$LASF74
	.byte	0x3
	.2byte	0x3c2
	.4byte	0x483
	.uleb128 0xd
	.4byte	$LASF75
	.byte	0x3
	.2byte	0x3c4
	.4byte	0x33
	.uleb128 0xd
	.4byte	$LASF76
	.byte	0x3
	.2byte	0x518
	.4byte	0x33
	.uleb128 0x12
	.4byte	0x52e
	.byte	0x1
	.4byte	$LASF81
	.byte	0x1
	.2byte	0x1161
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB4
	.4byte	$LFE4
	.4byte	$LSFDE0
	.byte	0x1
	.byte	0x6d
	.uleb128 0x13
	.4byte	$LASF77
	.byte	0x1
	.2byte	0x1160
	.4byte	0x308
	.byte	0x1
	.byte	0x61
	.uleb128 0x13
	.4byte	$LASF78
	.byte	0x1
	.2byte	0x1160
	.4byte	0x308
	.byte	0x1
	.byte	0x60
	.uleb128 0x14
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x1162
	.4byte	0x85
	.byte	0x1
	.byte	0x52
	.uleb128 0x14
	.4byte	$LASF80
	.byte	0x1
	.2byte	0x1163
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.byte	0x0
	.uleb128 0x12
	.4byte	0x588
	.byte	0x1
	.4byte	$LASF82
	.byte	0x1
	.2byte	0x11ad
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB5
	.4byte	$LFE5
	.4byte	$LSFDE2
	.byte	0x1
	.byte	0x6d
	.uleb128 0x13
	.4byte	$LASF77
	.byte	0x1
	.2byte	0x11ac
	.4byte	0x308
	.byte	0x1
	.byte	0x61
	.uleb128 0x13
	.4byte	$LASF78
	.byte	0x1
	.2byte	0x11ac
	.4byte	0x308
	.byte	0x1
	.byte	0x60
	.uleb128 0x14
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x11ae
	.4byte	0x85
	.byte	0x1
	.byte	0x52
	.uleb128 0x14
	.4byte	$LASF80
	.byte	0x1
	.2byte	0x11af
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.byte	0x0
	.uleb128 0x12
	.4byte	0x5f1
	.byte	0x1
	.4byte	$LASF83
	.byte	0x1
	.2byte	0x1437
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB6
	.4byte	$LFE6
	.4byte	$LSFDE4
	.byte	0x1
	.byte	0x6d
	.uleb128 0x14
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x1438
	.4byte	0x85
	.byte	0x1
	.byte	0x52
	.uleb128 0x15
	.ascii	"i\000"
	.byte	0x1
	.2byte	0x1439
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x14
	.4byte	$LASF84
	.byte	0x1
	.2byte	0x143a
	.4byte	0x2bf
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x14
	.4byte	$LASF85
	.byte	0x1
	.2byte	0x143b
	.4byte	0x21b
	.byte	0x2
	.byte	0x91
	.sleb128 48
	.uleb128 0x16
	.byte	0x1
	.4byte	$LASF89
	.byte	0x1
	.2byte	0x1450
	.4byte	0x50
	.byte	0x1
	.uleb128 0x17
	.byte	0x0
	.byte	0x0
	.uleb128 0x12
	.4byte	0x675
	.byte	0x1
	.4byte	$LASF86
	.byte	0x1
	.2byte	0x148b
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB7
	.4byte	$LFE7
	.4byte	$LSFDE6
	.byte	0x1
	.byte	0x6d
	.uleb128 0x18
	.ascii	"vid\000"
	.byte	0x1
	.2byte	0x148a
	.4byte	0x4c8
	.byte	0x1
	.byte	0x62
	.uleb128 0x13
	.4byte	$LASF87
	.byte	0x1
	.2byte	0x148a
	.4byte	0x4b0
	.byte	0x1
	.byte	0x61
	.uleb128 0x13
	.4byte	$LASF88
	.byte	0x1
	.2byte	0x148a
	.4byte	0x4b0
	.byte	0x1
	.byte	0x60
	.uleb128 0x18
	.ascii	"fid\000"
	.byte	0x1
	.2byte	0x148a
	.4byte	0x361
	.byte	0x1
	.byte	0x63
	.uleb128 0x19
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x148c
	.4byte	0x85
	.uleb128 0x14
	.4byte	$LASF84
	.byte	0x1
	.2byte	0x148d
	.4byte	0x2bf
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x16
	.byte	0x1
	.4byte	$LASF89
	.byte	0x1
	.2byte	0x1450
	.4byte	0x50
	.byte	0x1
	.uleb128 0x17
	.byte	0x0
	.byte	0x0
	.uleb128 0x12
	.4byte	0x77c
	.byte	0x1
	.4byte	$LASF90
	.byte	0x1
	.2byte	0x14e9
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB8
	.4byte	$LFE8
	.4byte	$LSFDE8
	.byte	0x1
	.byte	0x6d
	.uleb128 0x13
	.4byte	$LASF91
	.byte	0x1
	.2byte	0x14e8
	.4byte	0x3d6
	.byte	0x1
	.byte	0x64
	.uleb128 0x13
	.4byte	$LASF92
	.byte	0x1
	.2byte	0x14e8
	.4byte	0x4c8
	.byte	0x1
	.byte	0x63
	.uleb128 0x13
	.4byte	$LASF41
	.byte	0x1
	.2byte	0x14e8
	.4byte	0x4bc
	.byte	0x1
	.byte	0x66
	.uleb128 0x14
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x14ea
	.4byte	0x85
	.byte	0x1
	.byte	0x52
	.uleb128 0x15
	.ascii	"i\000"
	.byte	0x1
	.2byte	0x14eb
	.4byte	0x45
	.byte	0x1
	.byte	0x60
	.uleb128 0x15
	.ascii	"j\000"
	.byte	0x1
	.2byte	0x14ec
	.4byte	0x33
	.byte	0x1
	.byte	0x62
	.uleb128 0x15
	.ascii	"k\000"
	.byte	0x1
	.2byte	0x14ed
	.4byte	0x33
	.byte	0x1
	.byte	0x61
	.uleb128 0x14
	.4byte	$LASF93
	.byte	0x1
	.2byte	0x14ee
	.4byte	0x33
	.byte	0x3
	.byte	0x91
	.sleb128 92
	.uleb128 0x14
	.4byte	$LASF94
	.byte	0x1
	.2byte	0x14ee
	.4byte	0x33
	.byte	0x1
	.byte	0x61
	.uleb128 0x14
	.4byte	$LASF95
	.byte	0x1
	.2byte	0x14ef
	.4byte	0x33
	.byte	0x3
	.byte	0x91
	.sleb128 88
	.uleb128 0x14
	.4byte	$LASF96
	.byte	0x1
	.2byte	0x14ef
	.4byte	0x33
	.byte	0x3
	.byte	0x91
	.sleb128 100
	.uleb128 0x14
	.4byte	$LASF97
	.byte	0x1
	.2byte	0x14f0
	.4byte	0x33
	.byte	0x1
	.byte	0x67
	.uleb128 0x15
	.ascii	"pri\000"
	.byte	0x1
	.2byte	0x14f0
	.4byte	0x33
	.byte	0x3
	.byte	0x91
	.sleb128 96
	.uleb128 0x14
	.4byte	$LASF84
	.byte	0x1
	.2byte	0x14f1
	.4byte	0x2bf
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x14
	.4byte	$LASF85
	.byte	0x1
	.2byte	0x14f2
	.4byte	0x21b
	.byte	0x2
	.byte	0x91
	.sleb128 48
	.uleb128 0x14
	.4byte	$LASF98
	.byte	0x1
	.2byte	0x14f3
	.4byte	0x2fd
	.byte	0x3
	.byte	0x91
	.sleb128 72
	.byte	0x0
	.uleb128 0x12
	.4byte	0x7b9
	.byte	0x1
	.4byte	$LASF99
	.byte	0x1
	.2byte	0x2caf
	.byte	0x1
	.4byte	0x90
	.4byte	$LFB12
	.4byte	$LFE12
	.4byte	$LSFDE10
	.byte	0x1
	.byte	0x6d
	.uleb128 0x13
	.4byte	$LASF91
	.byte	0x1
	.2byte	0x2cae
	.4byte	0x33
	.byte	0x1
	.byte	0x54
	.uleb128 0x13
	.4byte	$LASF100
	.byte	0x1
	.2byte	0x2cae
	.4byte	0x33
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x1a
	.4byte	0x840
	.4byte	$LASF135
	.byte	0x1
	.2byte	0x2b76
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB11
	.4byte	$LFE11
	.4byte	$LSFDE12
	.byte	0x1
	.byte	0x6d
	.uleb128 0x14
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x2b77
	.4byte	0x85
	.byte	0x1
	.byte	0x52
	.uleb128 0x19
	.4byte	$LASF93
	.byte	0x1
	.2byte	0x2b78
	.4byte	0x33
	.uleb128 0x14
	.4byte	$LASF80
	.byte	0x1
	.2byte	0x2b78
	.4byte	0x33
	.byte	0x3
	.byte	0x91
	.sleb128 1536
	.uleb128 0x14
	.4byte	$LASF101
	.byte	0x1
	.2byte	0x2b7a
	.4byte	0x33
	.byte	0x3
	.byte	0x91
	.sleb128 1540
	.uleb128 0x15
	.ascii	"cnt\000"
	.byte	0x1
	.2byte	0x2b7a
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x14
	.4byte	$LASF102
	.byte	0x1
	.2byte	0x2b7c
	.4byte	0x865
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x14
	.4byte	$LASF103
	.byte	0x1
	.2byte	0x2bb1
	.4byte	0x87a
	.byte	0x3
	.byte	0x91
	.sleb128 840
	.byte	0x0
	.uleb128 0x10
	.4byte	0x850
	.4byte	0x850
	.uleb128 0x11
	.4byte	0x7e
	.byte	0x1
	.byte	0x0
	.uleb128 0x1b
	.4byte	0x57
	.uleb128 0x10
	.4byte	0x865
	.4byte	0x840
	.uleb128 0x11
	.4byte	0x7e
	.byte	0xcb
	.byte	0x0
	.uleb128 0x1b
	.4byte	0x855
	.uleb128 0x10
	.4byte	0x87a
	.4byte	0x840
	.uleb128 0x11
	.4byte	0x7e
	.byte	0xad
	.byte	0x0
	.uleb128 0x1b
	.4byte	0x86a
	.uleb128 0x12
	.4byte	0x906
	.byte	0x1
	.4byte	$LASF104
	.byte	0x1
	.2byte	0x2ce9
	.byte	0x1
	.4byte	0x90
	.4byte	$LFB14
	.4byte	$LFE14
	.4byte	$LSFDE14
	.byte	0x1
	.byte	0x6d
	.uleb128 0x18
	.ascii	"id\000"
	.byte	0x1
	.2byte	0x2ce8
	.4byte	0x33
	.byte	0x1
	.byte	0x61
	.uleb128 0x13
	.4byte	$LASF105
	.byte	0x1
	.2byte	0x2ce8
	.4byte	0x906
	.byte	0x1
	.byte	0x60
	.uleb128 0x19
	.4byte	$LASF80
	.byte	0x1
	.2byte	0x2cea
	.4byte	0x33
	.uleb128 0x19
	.4byte	$LASF106
	.byte	0x1
	.2byte	0x2ceb
	.4byte	0x90c
	.uleb128 0x14
	.4byte	$LASF107
	.byte	0x1
	.2byte	0x2cec
	.4byte	0x185
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x1c
	.4byte	0x8f5
	.byte	0x1
	.4byte	$LASF89
	.byte	0x1
	.2byte	0x1450
	.4byte	0x50
	.byte	0x1
	.uleb128 0x17
	.byte	0x0
	.uleb128 0x16
	.byte	0x1
	.4byte	$LASF108
	.byte	0x1
	.2byte	0x2cd9
	.4byte	0x50
	.byte	0x1
	.uleb128 0x17
	.byte	0x0
	.byte	0x0
	.uleb128 0x1d
	.byte	0x4
	.4byte	0x185
	.uleb128 0x1d
	.byte	0x4
	.4byte	0x57
	.uleb128 0x12
	.4byte	0x9aa
	.byte	0x1
	.4byte	$LASF109
	.byte	0x1
	.2byte	0x2cc4
	.byte	0x1
	.4byte	0x90
	.4byte	$LFB13
	.4byte	$LFE13
	.4byte	$LSFDE16
	.byte	0x1
	.byte	0x6d
	.uleb128 0x18
	.ascii	"id\000"
	.byte	0x1
	.2byte	0x2cc3
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x13
	.4byte	$LASF105
	.byte	0x1
	.2byte	0x2cc3
	.4byte	0x906
	.byte	0x1
	.byte	0x61
	.uleb128 0x14
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x2cc5
	.4byte	0x90
	.byte	0x1
	.byte	0x52
	.uleb128 0x14
	.4byte	$LASF80
	.byte	0x1
	.2byte	0x2cc6
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 28
	.uleb128 0x19
	.4byte	$LASF106
	.byte	0x1
	.2byte	0x2cc7
	.4byte	0x90c
	.uleb128 0x14
	.4byte	$LASF107
	.byte	0x1
	.2byte	0x2cc8
	.4byte	0x185
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x1c
	.4byte	0x999
	.byte	0x1
	.4byte	$LASF89
	.byte	0x1
	.2byte	0x1450
	.4byte	0x50
	.byte	0x1
	.uleb128 0x17
	.byte	0x0
	.uleb128 0x16
	.byte	0x1
	.4byte	$LASF108
	.byte	0x1
	.2byte	0x2cd9
	.4byte	0x50
	.byte	0x1
	.uleb128 0x17
	.byte	0x0
	.byte	0x0
	.uleb128 0x12
	.4byte	0x9f4
	.byte	0x1
	.4byte	$LASF110
	.byte	0x1
	.2byte	0x2d0f
	.byte	0x1
	.4byte	0x90
	.4byte	$LFB15
	.4byte	$LFE15
	.4byte	$LSFDE18
	.byte	0x1
	.byte	0x6d
	.uleb128 0x18
	.ascii	"id\000"
	.byte	0x1
	.2byte	0x2d0e
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x13
	.4byte	$LASF111
	.byte	0x1
	.2byte	0x2d0e
	.4byte	0x33
	.byte	0x1
	.byte	0x61
	.uleb128 0x14
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x2d10
	.4byte	0x90
	.byte	0x1
	.byte	0x52
	.byte	0x0
	.uleb128 0x12
	.4byte	0xa4e
	.byte	0x1
	.4byte	$LASF112
	.byte	0x1
	.2byte	0xfa7
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB3
	.4byte	$LFE3
	.4byte	$LSFDE20
	.byte	0x1
	.byte	0x6d
	.uleb128 0x13
	.4byte	$LASF111
	.byte	0x1
	.2byte	0xfa6
	.4byte	0x356
	.byte	0x1
	.byte	0x54
	.uleb128 0x13
	.4byte	$LASF113
	.byte	0x1
	.2byte	0xfa6
	.4byte	0xa4e
	.byte	0x1
	.byte	0x60
	.uleb128 0x14
	.4byte	$LASF79
	.byte	0x1
	.2byte	0xfa8
	.4byte	0x85
	.byte	0x1
	.byte	0x52
	.uleb128 0x14
	.4byte	$LASF107
	.byte	0x1
	.2byte	0xfa9
	.4byte	0x185
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.byte	0x0
	.uleb128 0x1d
	.byte	0x4
	.4byte	0x477
	.uleb128 0x12
	.4byte	0xaae
	.byte	0x1
	.4byte	$LASF114
	.byte	0x1
	.2byte	0xf41
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB2
	.4byte	$LFE2
	.4byte	$LSFDE22
	.byte	0x1
	.byte	0x6d
	.uleb128 0x13
	.4byte	$LASF111
	.byte	0x1
	.2byte	0xf40
	.4byte	0x356
	.byte	0x1
	.byte	0x54
	.uleb128 0x13
	.4byte	$LASF113
	.byte	0x1
	.2byte	0xf40
	.4byte	0xa4e
	.byte	0x1
	.byte	0x60
	.uleb128 0x14
	.4byte	$LASF79
	.byte	0x1
	.2byte	0xf42
	.4byte	0x85
	.byte	0x1
	.byte	0x52
	.uleb128 0x14
	.4byte	$LASF107
	.byte	0x1
	.2byte	0xf43
	.4byte	0x185
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.byte	0x0
	.uleb128 0x12
	.4byte	0xb22
	.byte	0x1
	.4byte	$LASF115
	.byte	0x1
	.2byte	0x2d45
	.byte	0x1
	.4byte	0x90
	.4byte	$LFB16
	.4byte	$LFE16
	.4byte	$LSFDE24
	.byte	0x1
	.byte	0x6d
	.uleb128 0x13
	.4byte	$LASF116
	.byte	0x1
	.2byte	0x2d44
	.4byte	0x33
	.byte	0x1
	.byte	0x62
	.uleb128 0x13
	.4byte	$LASF117
	.byte	0x1
	.2byte	0x2d44
	.4byte	0x33
	.byte	0x1
	.byte	0x61
	.uleb128 0x13
	.4byte	$LASF118
	.byte	0x1
	.2byte	0x2d44
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x14
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x2d46
	.4byte	0x90
	.byte	0x1
	.byte	0x52
	.uleb128 0x19
	.4byte	$LASF80
	.byte	0x1
	.2byte	0x2d47
	.4byte	0x33
	.uleb128 0x14
	.4byte	$LASF101
	.byte	0x1
	.2byte	0x2d48
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.byte	0x0
	.uleb128 0x12
	.4byte	0xba7
	.byte	0x1
	.4byte	$LASF119
	.byte	0x1
	.2byte	0x2d7d
	.byte	0x1
	.4byte	0x90
	.4byte	$LFB17
	.4byte	$LFE17
	.4byte	$LSFDE26
	.byte	0x1
	.byte	0x6d
	.uleb128 0x13
	.4byte	$LASF116
	.byte	0x1
	.2byte	0x2d7c
	.4byte	0x33
	.byte	0x1
	.byte	0x61
	.uleb128 0x13
	.4byte	$LASF117
	.byte	0x1
	.2byte	0x2d7c
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.uleb128 0x13
	.4byte	$LASF118
	.byte	0x1
	.2byte	0x2d7c
	.4byte	0xba7
	.byte	0x1
	.byte	0x62
	.uleb128 0x14
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x2d7e
	.4byte	0x90
	.byte	0x1
	.byte	0x52
	.uleb128 0x14
	.4byte	$LASF80
	.byte	0x1
	.2byte	0x2d7f
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 28
	.uleb128 0x14
	.4byte	$LASF101
	.byte	0x1
	.2byte	0x2d80
	.4byte	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x14
	.4byte	$LASF120
	.byte	0x1
	.2byte	0x2d80
	.4byte	0x33
	.byte	0x1
	.byte	0x60
	.byte	0x0
	.uleb128 0x1d
	.byte	0x4
	.4byte	0x33
	.uleb128 0x12
	.4byte	0xbf6
	.byte	0x1
	.4byte	$LASF121
	.byte	0x1
	.2byte	0x2dc9
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB18
	.4byte	$LFE18
	.4byte	$LSFDE28
	.byte	0x1
	.byte	0x6d
	.uleb128 0x13
	.4byte	$LASF122
	.byte	0x1
	.2byte	0x2dc8
	.4byte	0x392
	.byte	0x1
	.byte	0x54
	.uleb128 0x13
	.4byte	$LASF123
	.byte	0x1
	.2byte	0x2dc8
	.4byte	0x4b0
	.byte	0x1
	.byte	0x52
	.uleb128 0x19
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x2dca
	.4byte	0x85
	.byte	0x0
	.uleb128 0x12
	.4byte	0xc31
	.byte	0x1
	.4byte	$LASF124
	.byte	0x1
	.2byte	0x2dff
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB19
	.4byte	$LFE19
	.4byte	$LSFDE30
	.byte	0x1
	.byte	0x6d
	.uleb128 0x13
	.4byte	$LASF111
	.byte	0x1
	.2byte	0x2dfe
	.4byte	0x3ca
	.byte	0x1
	.byte	0x54
	.uleb128 0x19
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x2e00
	.4byte	0x85
	.byte	0x0
	.uleb128 0x12
	.4byte	0xc9f
	.byte	0x1
	.4byte	$LASF125
	.byte	0x1
	.2byte	0x2e0c
	.byte	0x1
	.4byte	0x50
	.4byte	$LFB20
	.4byte	$LFE20
	.4byte	$LSFDE32
	.byte	0x1
	.byte	0x6d
	.uleb128 0x13
	.4byte	$LASF126
	.byte	0x1
	.2byte	0x2e0b
	.4byte	0x50
	.byte	0x1
	.byte	0x60
	.uleb128 0x14
	.4byte	$LASF127
	.byte	0x1
	.2byte	0x2e0d
	.4byte	0x477
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x19
	.4byte	$LASF128
	.byte	0x1
	.2byte	0x2e0e
	.4byte	0x4b0
	.uleb128 0x19
	.4byte	$LASF87
	.byte	0x1
	.2byte	0x2e0f
	.4byte	0x4b0
	.uleb128 0x19
	.4byte	$LASF88
	.byte	0x1
	.2byte	0x2e10
	.4byte	0x4b0
	.uleb128 0x19
	.4byte	$LASF111
	.byte	0x1
	.2byte	0x2e11
	.4byte	0x33
	.byte	0x0
	.uleb128 0x12
	.4byte	0xe9a
	.byte	0x1
	.4byte	$LASF129
	.byte	0x1
	.2byte	0x2e93
	.byte	0x1
	.4byte	0x85
	.4byte	$LFB21
	.4byte	$LFE21
	.4byte	$LSFDE34
	.byte	0x1
	.byte	0x6d
	.uleb128 0x13
	.4byte	$LASF130
	.byte	0x1
	.2byte	0x2e92
	.4byte	0x50
	.byte	0x1
	.byte	0x61
	.uleb128 0x14
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x2e95
	.4byte	0x85
	.byte	0x1
	.byte	0x60
	.uleb128 0x14
	.4byte	$LASF131
	.byte	0x1
	.2byte	0x2e96
	.4byte	0x33
	.byte	0x3
	.byte	0x91
	.sleb128 2976
	.uleb128 0x14
	.4byte	$LASF132
	.byte	0x1
	.2byte	0x2e96
	.4byte	0x33
	.byte	0x3
	.byte	0x91
	.sleb128 2980
	.uleb128 0x14
	.4byte	$LASF126
	.byte	0x1
	.2byte	0x2e97
	.4byte	0x50
	.byte	0x1
	.byte	0x63
	.uleb128 0x1e
	.4byte	$LASF133
	.4byte	0xeaf
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	__FUNCTION__.0
	.uleb128 0x1c
	.4byte	0xd2d
	.byte	0x1
	.4byte	$LASF134
	.byte	0x1
	.2byte	0x2ea8
	.4byte	0x50
	.byte	0x1
	.uleb128 0x17
	.byte	0x0
	.uleb128 0x1f
	.4byte	0xd4b
	.4byte	$LBB2
	.4byte	$LBE2
	.uleb128 0x16
	.byte	0x1
	.4byte	$LASF134
	.byte	0x1
	.2byte	0x2ea8
	.4byte	0x50
	.byte	0x1
	.uleb128 0x17
	.byte	0x0
	.byte	0x0
	.uleb128 0x1f
	.4byte	0xd69
	.4byte	$LBB3
	.4byte	$LBE3
	.uleb128 0x16
	.byte	0x1
	.4byte	$LASF134
	.byte	0x1
	.2byte	0x2ea8
	.4byte	0x50
	.byte	0x1
	.uleb128 0x17
	.byte	0x0
	.byte	0x0
	.uleb128 0x20
	.4byte	0xdf5
	.4byte	0xeb4
	.4byte	$LBB4
	.4byte	$LBE4
	.uleb128 0x21
	.4byte	0xec6
	.byte	0x1
	.byte	0x52
	.uleb128 0x21
	.4byte	0xed2
	.byte	0x1
	.byte	0x61
	.uleb128 0x21
	.4byte	0xede
	.byte	0x3
	.byte	0x91
	.sleb128 2984
	.uleb128 0x21
	.4byte	0xeea
	.byte	0x3
	.byte	0x91
	.sleb128 2988
	.uleb128 0x21
	.4byte	0xef6
	.byte	0x1
	.byte	0x60
	.uleb128 0x21
	.4byte	0xf02
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x21
	.4byte	0xf0e
	.byte	0x3
	.byte	0x91
	.sleb128 672
	.uleb128 0x22
	.4byte	$LBB5
	.4byte	$LBE5
	.uleb128 0x21
	.4byte	0xec6
	.byte	0x1
	.byte	0x52
	.uleb128 0x21
	.4byte	0xed2
	.byte	0x1
	.byte	0x61
	.uleb128 0x21
	.4byte	0xede
	.byte	0x3
	.byte	0x91
	.sleb128 2984
	.uleb128 0x21
	.4byte	0xeea
	.byte	0x3
	.byte	0x91
	.sleb128 2988
	.uleb128 0x21
	.4byte	0xef6
	.byte	0x1
	.byte	0x60
	.uleb128 0x21
	.4byte	0xf02
	.byte	0x2
	.byte	0x91
	.sleb128 24
	.uleb128 0x21
	.4byte	0xf0e
	.byte	0x3
	.byte	0x91
	.sleb128 672
	.byte	0x0
	.byte	0x0
	.uleb128 0x20
	.4byte	0xe7f
	.4byte	0xf45
	.4byte	$LBB6
	.4byte	$LBE6
	.uleb128 0x21
	.4byte	0xf57
	.byte	0x1
	.byte	0x52
	.uleb128 0x23
	.4byte	0xf63
	.uleb128 0x21
	.4byte	0xf6f
	.byte	0x3
	.byte	0x91
	.sleb128 2992
	.uleb128 0x21
	.4byte	0xf7b
	.byte	0x3
	.byte	0x91
	.sleb128 2996
	.uleb128 0x21
	.4byte	0xf87
	.byte	0x1
	.byte	0x60
	.uleb128 0x21
	.4byte	0xf93
	.byte	0x3
	.byte	0x91
	.sleb128 1304
	.uleb128 0x21
	.4byte	0xf9f
	.byte	0x3
	.byte	0x91
	.sleb128 2200
	.uleb128 0x22
	.4byte	$LBB7
	.4byte	$LBE7
	.uleb128 0x21
	.4byte	0xf57
	.byte	0x1
	.byte	0x52
	.uleb128 0x23
	.4byte	0xf63
	.uleb128 0x21
	.4byte	0xf6f
	.byte	0x3
	.byte	0x91
	.sleb128 2992
	.uleb128 0x21
	.4byte	0xf7b
	.byte	0x3
	.byte	0x91
	.sleb128 2996
	.uleb128 0x21
	.4byte	0xf87
	.byte	0x1
	.byte	0x60
	.uleb128 0x21
	.4byte	0xf93
	.byte	0x3
	.byte	0x91
	.sleb128 1304
	.uleb128 0x21
	.4byte	0xf9f
	.byte	0x3
	.byte	0x91
	.sleb128 2200
	.byte	0x0
	.byte	0x0
	.uleb128 0x22
	.4byte	$LBB8
	.4byte	$LBE8
	.uleb128 0x16
	.byte	0x1
	.4byte	$LASF134
	.byte	0x1
	.2byte	0x2ea8
	.4byte	0x50
	.byte	0x1
	.uleb128 0x17
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.4byte	0xeaa
	.4byte	0xeaa
	.uleb128 0x11
	.4byte	0x7e
	.byte	0xf
	.byte	0x0
	.uleb128 0x1b
	.4byte	0x77
	.uleb128 0x1b
	.4byte	0xe9a
	.uleb128 0x24
	.4byte	0xf1b
	.4byte	$LASF136
	.byte	0x1
	.2byte	0x28c2
	.byte	0x1
	.4byte	0x85
	.byte	0x1
	.uleb128 0x19
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x28c3
	.4byte	0x85
	.uleb128 0x19
	.4byte	$LASF93
	.byte	0x1
	.2byte	0x28c4
	.4byte	0x33
	.uleb128 0x19
	.4byte	$LASF80
	.byte	0x1
	.2byte	0x28c4
	.4byte	0x33
	.uleb128 0x19
	.4byte	$LASF101
	.byte	0x1
	.2byte	0x28c5
	.4byte	0x33
	.uleb128 0x25
	.ascii	"cnt\000"
	.byte	0x1
	.2byte	0x28c5
	.4byte	0x33
	.uleb128 0x19
	.4byte	$LASF102
	.byte	0x1
	.2byte	0x28c6
	.4byte	0xf2b
	.uleb128 0x19
	.4byte	$LASF103
	.byte	0x1
	.2byte	0x28f0
	.4byte	0xf40
	.byte	0x0
	.uleb128 0x10
	.4byte	0xf2b
	.4byte	0x840
	.uleb128 0x11
	.4byte	0x7e
	.byte	0xa0
	.byte	0x0
	.uleb128 0x1b
	.4byte	0xf1b
	.uleb128 0x10
	.4byte	0xf40
	.4byte	0x840
	.uleb128 0x11
	.4byte	0x7e
	.byte	0x9d
	.byte	0x0
	.uleb128 0x1b
	.4byte	0xf30
	.uleb128 0x24
	.4byte	0xfac
	.4byte	$LASF137
	.byte	0x1
	.2byte	0x2a3e
	.byte	0x1
	.4byte	0x85
	.byte	0x1
	.uleb128 0x19
	.4byte	$LASF79
	.byte	0x1
	.2byte	0x2a3f
	.4byte	0x85
	.uleb128 0x19
	.4byte	$LASF93
	.byte	0x1
	.2byte	0x2a40
	.4byte	0x33
	.uleb128 0x19
	.4byte	$LASF80
	.byte	0x1
	.2byte	0x2a40
	.4byte	0x33
	.uleb128 0x19
	.4byte	$LASF101
	.byte	0x1
	.2byte	0x2a42
	.4byte	0x33
	.uleb128 0x25
	.ascii	"cnt\000"
	.byte	0x1
	.2byte	0x2a42
	.4byte	0x33
	.uleb128 0x19
	.4byte	$LASF102
	.byte	0x1
	.2byte	0x2a44
	.4byte	0xfbc
	.uleb128 0x19
	.4byte	$LASF103
	.byte	0x1
	.2byte	0x2a7d
	.4byte	0xfd1
	.byte	0x0
	.uleb128 0x10
	.4byte	0xfbc
	.4byte	0x840
	.uleb128 0x11
	.4byte	0x7e
	.byte	0xdf
	.byte	0x0
	.uleb128 0x1b
	.4byte	0xfac
	.uleb128 0x10
	.4byte	0xfd1
	.4byte	0x840
	.uleb128 0x11
	.4byte	0x7e
	.byte	0xc1
	.byte	0x0
	.uleb128 0x1b
	.4byte	0xfc1
	.uleb128 0x10
	.4byte	0xfe6
	.4byte	0xfe6
	.uleb128 0x11
	.4byte	0x7e
	.byte	0x6
	.byte	0x0
	.uleb128 0x1b
	.4byte	0x33
	.uleb128 0x10
	.4byte	0xffb
	.4byte	0xfd6
	.uleb128 0x11
	.4byte	0x7e
	.byte	0x4
	.byte	0x0
	.uleb128 0x26
	.4byte	$LASF138
	.byte	0x1
	.byte	0x26
	.4byte	0x100d
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	filter_templateField
	.uleb128 0x1b
	.4byte	0xfeb
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
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
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
	.uleb128 0x9
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
	.uleb128 0xa
	.uleb128 0x4
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
	.uleb128 0x4
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
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
	.uleb128 0x16
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
	.uleb128 0xe
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
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x11
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
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
	.uleb128 0x15
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
	.uleb128 0x16
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
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0x17
	.uleb128 0x18
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x18
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
	.uleb128 0x19
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
	.uleb128 0x1a
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
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
	.uleb128 0x1b
	.uleb128 0x26
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1c
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
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.uleb128 0x1d
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1e
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x1f
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
	.uleb128 0x20
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x21
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x22
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0x0
	.byte	0x0
	.uleb128 0x23
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x24
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
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
	.uleb128 0x20
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x25
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
	.byte	0x0
	.byte	0x0
	.uleb128 0x26
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
	.4byte	0x1f5
	.2byte	0x2
	.4byte	$Ldebug_info0
	.4byte	0x1013
	.4byte	0x4d4
	.ascii	"rtk_port_rgmiiDelayExt0_set\000"
	.4byte	0x52e
	.ascii	"rtk_port_rgmiiDelayExt1_set\000"
	.4byte	0x588
	.ascii	"rtk_vlan_init\000"
	.4byte	0x5f1
	.ascii	"rtk_vlan_set\000"
	.4byte	0x675
	.ascii	"rtk_vlan_portPvid_set\000"
	.4byte	0x77c
	.ascii	"rtl8370_setAsicLutLearnLimitNo\000"
	.4byte	0x87f
	.ascii	"rtl8370_setAsicPortForceLinkExt\000"
	.4byte	0x912
	.ascii	"rtl8370_getAsicPortForceLinkExt\000"
	.4byte	0x9aa
	.ascii	"rtl8370_setAsicPortExtMode\000"
	.4byte	0x9f4
	.ascii	"rtk_port_macForceLinkExt1_set\000"
	.4byte	0xa54
	.ascii	"rtk_port_macForceLinkExt0_set\000"
	.4byte	0xaae
	.ascii	"rtl8370_setAsicPHYReg\000"
	.4byte	0xb22
	.ascii	"rtl8370_getAsicPHYReg\000"
	.4byte	0xbad
	.ascii	"rtk_led_enable_set\000"
	.4byte	0xbf6
	.ascii	"rtk_led_mode_set\000"
	.4byte	0xc31
	.ascii	"init_rgmii\000"
	.4byte	0xc9f
	.ascii	"rtk_switch_init\000"
	.4byte	0xffb
	.ascii	"filter_templateField\000"
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
$LASF139:
	.ascii	"GNU C 3.4.2 -g\000"
$LASF76:
	.ascii	"rtk_vlan_t\000"
$LASF122:
	.ascii	"group\000"
$LASF38:
	.ascii	"rtl8370_user_vlan4kentry\000"
$LASF3:
	.ascii	"uint32\000"
$LASF33:
	.ascii	"vbpen\000"
$LASF99:
	.ascii	"rtl8370_setAsicLutLearnLimitNo\000"
$LASF104:
	.ascii	"rtl8370_setAsicPortForceLinkExt\000"
$LASF22:
	.ascii	"lpi1000\000"
$LASF23:
	.ascii	"forcemode\000"
$LASF65:
	.ascii	"LED_MODE_2\000"
$LASF10:
	.ascii	"rtk_api_ret_t\000"
$LASF35:
	.ascii	"rtl8370_vlanconfiguser\000"
$LASF73:
	.ascii	"bits\000"
$LASF108:
	.ascii	"memcpy\000"
$LASF30:
	.ascii	"msti\000"
$LASF88:
	.ascii	"untagmsk\000"
$LASF27:
	.ascii	"VLANCONFIGUSER\000"
$LASF26:
	.ascii	"rtl8370_port_ability_s\000"
$LASF25:
	.ascii	"rtl8370_port_ability_t\000"
$LASF61:
	.ascii	"rtk_led_group_t\000"
$LASF6:
	.ascii	"short unsigned int\000"
$LASF128:
	.ascii	"ledmask\000"
$LASF85:
	.ascii	"vlanMC\000"
$LASF125:
	.ascii	"init_rgmii\000"
$LASF8:
	.ascii	"unsigned char\000"
$LASF107:
	.ascii	"ability\000"
$LASF94:
	.ascii	"empty_idx\000"
$LASF123:
	.ascii	"portmask\000"
$LASF129:
	.ascii	"rtk_switch_init\000"
$LASF113:
	.ascii	"pPortability\000"
$LASF112:
	.ascii	"rtk_port_macForceLinkExt1_set\000"
$LASF54:
	.ascii	"rtk_fid_t\000"
$LASF79:
	.ascii	"retVal\000"
$LASF9:
	.ascii	"char\000"
$LASF100:
	.ascii	"number\000"
$LASF71:
	.ascii	"rtk_port_mac_ability_t\000"
$LASF29:
	.ascii	"lurep\000"
$LASF133:
	.ascii	"__FUNCTION__\000"
$LASF34:
	.ascii	"vbpri\000"
$LASF21:
	.ascii	"lpi100\000"
$LASF140:
	.ascii	"rtk_api.c\000"
$LASF50:
	.ascii	"MODE_EXT_GMII\000"
$LASF17:
	.ascii	"txpause\000"
$LASF137:
	.ascii	"_rtk_switch_init1\000"
$LASF135:
	.ascii	"_rtk_switch_init2\000"
$LASF82:
	.ascii	"rtk_port_rgmiiDelayExt1_set\000"
$LASF119:
	.ascii	"rtl8370_getAsicPHYReg\000"
$LASF106:
	.ascii	"accessPtr\000"
$LASF134:
	.ascii	"printf\000"
$LASF69:
	.ascii	"rtk_port_t\000"
$LASF60:
	.ascii	"LED_GROUP_END\000"
$LASF12:
	.ascii	"speed\000"
$LASF40:
	.ascii	"vlan_idx\000"
$LASF132:
	.ascii	"regData2\000"
$LASF45:
	.ascii	"MODE_EXT_RGMII\000"
$LASF83:
	.ascii	"rtk_vlan_init\000"
$LASF81:
	.ascii	"rtk_port_rgmiiDelayExt0_set\000"
$LASF127:
	.ascii	"mac_cfg\000"
$LASF62:
	.ascii	"rtk_led_mode_e\000"
$LASF63:
	.ascii	"LED_MODE_0\000"
$LASF64:
	.ascii	"LED_MODE_1\000"
$LASF66:
	.ascii	"LED_MODE_3\000"
$LASF15:
	.ascii	"link\000"
$LASF70:
	.ascii	"rtk_port_mac_ability_s\000"
$LASF44:
	.ascii	"MODE_EXT_DISABLE\000"
$LASF57:
	.ascii	"LED_GROUP_0\000"
$LASF58:
	.ascii	"LED_GROUP_1\000"
$LASF84:
	.ascii	"vlan4K\000"
$LASF131:
	.ascii	"regData1\000"
$LASF36:
	.ascii	"USER_VLANTABLE\000"
$LASF130:
	.ascii	"init_vlan\000"
$LASF77:
	.ascii	"txDelay\000"
$LASF59:
	.ascii	"LED_GROUP_2\000"
$LASF72:
	.ascii	"rtk_portmask_s\000"
$LASF138:
	.ascii	"filter_templateField\000"
$LASF2:
	.ascii	"unsigned int\000"
$LASF116:
	.ascii	"phyNo\000"
$LASF115:
	.ascii	"rtl8370_setAsicPHYReg\000"
$LASF124:
	.ascii	"rtk_led_mode_set\000"
$LASF111:
	.ascii	"mode\000"
$LASF118:
	.ascii	"data\000"
$LASF110:
	.ascii	"rtl8370_setAsicPortExtMode\000"
$LASF91:
	.ascii	"port\000"
$LASF47:
	.ascii	"MODE_EXT_MII_PHY\000"
$LASF114:
	.ascii	"rtk_port_macForceLinkExt0_set\000"
$LASF52:
	.ascii	"MODE_EXT_END\000"
$LASF86:
	.ascii	"rtk_vlan_set\000"
$LASF42:
	.ascii	"rtl8370_protocolvlancfg\000"
$LASF55:
	.ascii	"rtk_mode_ext_e\000"
$LASF67:
	.ascii	"LED_MODE_END\000"
$LASF53:
	.ascii	"rtk_mode_ext_t\000"
$LASF31:
	.ascii	"envlanpol\000"
$LASF136:
	.ascii	"_rtk_switch_init0\000"
$LASF20:
	.ascii	"mstfault\000"
$LASF43:
	.ascii	"rtk_data_t\000"
$LASF78:
	.ascii	"rxDelay\000"
$LASF0:
	.ascii	"long long unsigned int\000"
$LASF126:
	.ascii	"vlan\000"
$LASF98:
	.ascii	"ppb_vlan_cfg\000"
$LASF32:
	.ascii	"meteridx\000"
$LASF19:
	.ascii	"mstmode\000"
$LASF101:
	.ascii	"busyFlag\000"
$LASF95:
	.ascii	"gvidx\000"
$LASF117:
	.ascii	"phyAddr\000"
$LASF49:
	.ascii	"MODE_EXT_TMII_PHY\000"
$LASF102:
	.ascii	"chipData0\000"
$LASF103:
	.ascii	"chipData1\000"
$LASF141:
	.ascii	"/home/IOData/BR6456IN/srcbase/boot/realtek_8370\000"
$LASF75:
	.ascii	"rtk_pri_t\000"
$LASF46:
	.ascii	"MODE_EXT_MII_MAC\000"
$LASF80:
	.ascii	"regData\000"
$LASF89:
	.ascii	"memset\000"
$LASF11:
	.ascii	"ret_t\000"
$LASF105:
	.ascii	"portability\000"
$LASF7:
	.ascii	"short int\000"
$LASF41:
	.ascii	"priority\000"
$LASF109:
	.ascii	"rtl8370_getAsicPortForceLinkExt\000"
$LASF93:
	.ascii	"index\000"
$LASF18:
	.ascii	"nway\000"
$LASF13:
	.ascii	"duplex\000"
$LASF97:
	.ascii	"bUsed\000"
$LASF39:
	.ascii	"valid\000"
$LASF121:
	.ascii	"rtk_led_enable_set\000"
$LASF48:
	.ascii	"MODE_EXT_TMII_MAC\000"
$LASF68:
	.ascii	"rtk_led_mode_t\000"
$LASF14:
	.ascii	"reserve1\000"
$LASF24:
	.ascii	"reserve3\000"
$LASF1:
	.ascii	"long long int\000"
$LASF28:
	.ascii	"evid\000"
$LASF92:
	.ascii	"pvid\000"
$LASF96:
	.ascii	"proc\000"
$LASF74:
	.ascii	"rtk_portmask_t\000"
$LASF37:
	.ascii	"untag\000"
$LASF4:
	.ascii	"int32\000"
$LASF16:
	.ascii	"rxpause\000"
$LASF51:
	.ascii	"MODE_EXT_RGMII_33V\000"
$LASF56:
	.ascii	"rtk_led_group_e\000"
$LASF87:
	.ascii	"mbrmsk\000"
$LASF120:
	.ascii	"checkCounter\000"
$LASF90:
	.ascii	"rtk_vlan_portPvid_set\000"
$LASF5:
	.ascii	"uint16\000"
	.ident	"GCC: (GNU) 3.4.2"
