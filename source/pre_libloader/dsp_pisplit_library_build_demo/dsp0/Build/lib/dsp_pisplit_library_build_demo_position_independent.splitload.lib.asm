
Build/lib/dsp_pisplit_library_build_demo_position_independent.splitload.lib:     file format elf32-xtensa-le


Disassembly of section .text:

00000000 <_init-0x54>:
   0:	000000ac 00000128 00000670 00000494     ....(...p.......
  10:	00000000 00000474 00000000 00000674     ....t.......t...
  20:	000004b0 0000068c 000004fc 00000550     ............P...
  30:	0000059c 000005f0 000000c8 000000dc     ................
  40:	0000011c 000004a0 0000027c 00000478     ........|...x...
  50:	00000070                                p...

00000054 <_init>:
  54:	008136        	entry	a1, 64
  57:	ffea81        	l32r	a8, 0 (ac <frame_dummy>)
  5a:	f03d           	nop.n
  5c:	0008e0        	callx8	a8
  5f:	ffe981        	l32r	a8, 4 (128 <__do_global_ctors_aux>)
  62:	f03d           	nop.n
  64:	0008e0        	callx8	a8
  67:	f01d           	retw.n

00000069 <_init+0x15>:
  69:	00000000 36000000                                .......

00000070 <__do_global_dtors_aux>:
  70:	004136        	entry	a1, 32
  73:	ffe531        	l32r	a3, 8 (670 <completed$3521_6_3>)
  76:	000382        	l8ui	a8, a3, 0
  79:	d8ec           	bnez.n	a8, aa <__do_global_dtors_aux+0x3a>
  7b:	ffe421        	l32r	a2, c (494 <p$3519_6_1>)
  7e:	02b8           	l32i.n	a11, a2, 0
  80:	0ba8           	l32i.n	a10, a11, 0
  82:	ca8c           	beqz.n	a10, 92 <__do_global_dtors_aux+0x22>

00000084 <__do_global_dtors_aux+0x14>:
  84:	9b4b           	addi.n	a9, a11, 4
  86:	0299           	s32i.n	a9, a2, 0
  88:	000ae0        	callx8	a10
  8b:	02b8           	l32i.n	a11, a2, 0
  8d:	0ba8           	l32i.n	a10, a11, 0
  8f:	ff1a56        	bnez	a10, 84 <__do_global_dtors_aux+0x14>

00000092 <__do_global_dtors_aux+0x22>:
  92:	ffdfa1        	l32r	a10, 10 (0 <_text_start>)
  95:	0b0c           	movi.n	a11, 0
  97:	081ab7        	beq	a10, a11, a3 <__do_global_dtors_aux+0x33>
  9a:	ffdea1        	l32r	a10, 14 (474 <__EH_FRAME_BEGIN___44>)
  9d:	ffdc81        	l32r	a8, 10 (0 <_text_start>)
  a0:	0008e0        	callx8	a8

000000a3 <__do_global_dtors_aux+0x33>:
  a3:	1c0c           	movi.n	a12, 1
  a5:	0043c2        	s8i	a12, a3, 0
  a8:	f01d           	retw.n

000000aa <__do_global_dtors_aux+0x3a>:
  aa:	f01d           	retw.n

000000ac <frame_dummy>:
  ac:	004136        	entry	a1, 32
  af:	ffda81        	l32r	a8, 18 (0 <_text_start>)
  b2:	090c           	movi.n	a9, 0
  b4:	0b1897        	beq	a8, a9, c3 <frame_dummy+0x17>
  b7:	ffd7a1        	l32r	a10, 14 (474 <__EH_FRAME_BEGIN___44>)
  ba:	ffd8b1        	l32r	a11, 1c (674 <object$3538_8_1>)
  bd:	ffd681        	l32r	a8, 18 (0 <_text_start>)
  c0:	0008e0        	callx8	a8

000000c3 <frame_dummy+0x17>:
  c3:	f01d           	retw.n

000000c5 <frame_dummy+0x19>:
  c5:	000000                                        ...

000000c8 <dsp0_demo_lib_entry_1>:
  c8:	004136        	entry	a1, 32
  cb:	ffd5a1        	l32r	a10, 20 (4b0 <export_parameter_array+0x10>)
  ce:	ffd581        	l32r	a8, 24 (68c <printf_ptr>)
  d1:	0888           	l32i.n	a8, a8, 0
  d3:	0008e0        	callx8	a8
  d6:	ffd421        	l32r	a2, 28 (4fc <export_parameter_array+0x5c>)
  d9:	f01d           	retw.n

000000db <dsp0_demo_lib_entry_1+0x13>:
	...

000000dc <dsp0_demo_lib_entry_2>:
  dc:	004136        	entry	a1, 32
  df:	ffd3a1        	l32r	a10, 2c (550 <export_parameter_array+0xb0>)
  e2:	ffd081        	l32r	a8, 24 (68c <printf_ptr>)
  e5:	0888           	l32i.n	a8, a8, 0
  e7:	0008e0        	callx8	a8
  ea:	ffd121        	l32r	a2, 30 (59c <export_parameter_array+0xfc>)
  ed:	f01d           	retw.n

000000ef <dsp0_demo_lib_entry_2+0x13>:
	...

000000f0 <_start>:
  f0:	006136        	entry	a1, 48
  f3:	0129           	s32i.n	a2, a1, 0
  f5:	ffcba1        	l32r	a10, 24 (68c <printf_ptr>)
  f8:	0198           	l32i.n	a9, a1, 0
  fa:	0998           	l32i.n	a9, a9, 0
  fc:	0a99           	s32i.n	a9, a10, 0
  fe:	ffcda1        	l32r	a10, 34 (5f0 <export_parameter_array+0x150>)
 101:	ffcdb1        	l32r	a11, 38 (c8 <dsp0_demo_lib_entry_1>)
 104:	ffcec1        	l32r	a12, 3c (dc <dsp0_demo_lib_entry_2>)
 107:	ffced1        	l32r	a13, 40 (11c <dsp0_demo_lib_entry_3>)
 10a:	ffc681        	l32r	a8, 24 (68c <printf_ptr>)
 10d:	0888           	l32i.n	a8, a8, 0
 10f:	0008e0        	callx8	a8
 112:	ffcc21        	l32r	a2, 44 (4a0 <export_parameter_array>)
 115:	f01d           	retw.n

00000117 <_start+0x27>:
	...

00000118 <_start+0x28>:
 118:	00000414                                ....

0000011c <dsp0_demo_lib_entry_3>:
 11c:	004136        	entry	a1, 32
 11f:	fffe21        	l32r	a2, 118 (414 <_DYNAMIC+0x2b4>)
 122:	f01d           	retw.n

00000124 <dsp0_demo_lib_entry_3+0x8>:
 124:	00000000                                ....

00000128 <__do_global_ctors_aux>:
 128:	004136        	entry	a1, 32
 12b:	ffc781        	l32r	a8, 48 (27c <_DYNAMIC+0x11c>)
 12e:	7f2882        	l32i	a8, a8, 0x1fc
 131:	ffc621        	l32r	a2, 4c (478 <__CTOR_LIST___47>)
 134:	0c0826        	beqi	a8, -1, 144 <__do_global_ctors_aux+0x1c>
 137:	02a8           	l32i.n	a10, a2, 0

00000139 <__do_global_ctors_aux+0x11>:
 139:	000ae0        	callx8	a10
 13c:	fcc222        	addi	a2, a2, -4
 13f:	02a8           	l32i.n	a10, a2, 0
 141:	f40a66        	bnei	a10, -1, 139 <__do_global_ctors_aux+0x11>

00000144 <__do_global_ctors_aux+0x1c>:
 144:	f01d           	retw.n

00000146 <__do_global_ctors_aux+0x1e>:
	...

00000148 <_fini>:
 148:	008136        	entry	a1, 64
 14b:	ffc181        	l32r	a8, 50 (70 <__do_global_dtors_aux>)
 14e:	f03d           	nop.n
 150:	0008e0        	callx8	a8

00000153 <_fini+0xb>:
 153:	f01d           	retw.n
