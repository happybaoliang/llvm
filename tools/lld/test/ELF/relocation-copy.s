// REQUIRES: x86
// RUN: llvm-mc -filetype=obj -triple=x86_64-pc-linux %s -o %t.o
// RUN: llvm-mc -filetype=obj -triple=x86_64-pc-linux %p/Inputs/relocation-copy.s -o %t2.o
// RUN: ld.lld -shared %t2.o -o %t.so
// RUN: ld.lld %t.o %t.so -o %t3
// RUN: llvm-readobj -s -r --expand-relocs %t3 | FileCheck %s
// RUN: llvm-objdump -d %t3 | FileCheck -check-prefix=CODE %s

.text
.global _start
_start:
movl $5, x
movl $7, y
movl $9, z
movl $x, %edx
movl $y, %edx
movl $z, %edx

// CHECK:      Name: .bss
// CHECK-NEXT:  Type: SHT_NOBITS (0x8)
// CHECK-NEXT:  Flags [ (0x3)
// CHECK-NEXT:   SHF_ALLOC (0x2)
// CHECK-NEXT:   SHF_WRITE (0x1)
// CHECK-NEXT:  ]
// CHECK-NEXT:  Address: 0x13000
// CHECK-NEXT:  Offset:
// CHECK-NEXT:  Size: 24
// CHECK-NEXT:  Link: 0
// CHECK-NEXT:  Info: 0
// CHECK-NEXT:  AddressAlignment: 16
// CHECK-NEXT:  EntrySize: 0

// CHECK:      Relocations [
// CHECK-NEXT:   Section ({{.*}}) .rela.dyn {
// CHECK-NEXT:     Relocation {
// CHECK-NEXT:       Offset:
// CHECK-NEXT:       Type: R_X86_64_COPY
// CHECK-NEXT:       Symbol: x
// CHECK-NEXT:       Addend: 0x0
// CHECK-NEXT:     }
// CHECK-NEXT:     Relocation {
// CHECK-NEXT:       Offset:
// CHECK-NEXT:       Type: R_X86_64_COPY
// CHECK-NEXT:       Symbol: y
// CHECK-NEXT:       Addend: 0x0
// CHECK-NEXT:     }
// CHECK-NEXT:     Relocation {
// CHECK-NEXT:       Offset:
// CHECK-NEXT:       Type: R_X86_64_COPY
// CHECK-NEXT:       Symbol: z
// CHECK-NEXT:       Addend: 0x0
// CHECK-NEXT:     }
// CHECK-NEXT:   }
// CHECK-NEXT: ]

// 77824 = 0x13000
// 16 is alignment here
// 77840 = 0x13000 + 16
// 77844 = 0x13000 + 16 + 4
// CODE: Disassembly of section .text:
// CODE-NEXT: _start:
// CODE-NEXT: 11000: c7 04 25 00 30 01 00 05 00 00 00 movl $5, 77824
// CODE-NEXT: 1100b: c7 04 25 10 30 01 00 07 00 00 00 movl $7, 77840
// CODE-NEXT: 11016: c7 04 25 14 30 01 00 09 00 00 00 movl $9, 77844
// CODE-NEXT: 11021: ba 00 30 01 00                   movl $77824, %edx
// CODE-NEXT: 11026: ba 10 30 01 00                   movl $77840, %edx
// CODE-NEXT: 1102b: ba 14 30 01 00                   movl $77844, %edx