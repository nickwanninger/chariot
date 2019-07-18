global _start
global start



;; The basic page table is setup as follows:
;; pml4[0] -> pdpt[0] -> pd
;; which with 2mb pages, maps the first GB of ram
extern pml4 ;; level 4 page table
extern pdpt ;; level 3 page dir
extern pd ;; level 2 page dir
          ;; where is level 1? idk lol



extern p4_table
extern p3_table
extern p2_table
extern p1_table

extern boot_stack_end ;; static memory from the binary where the stack begins
extern kmain ;; c entry point

;; TODO(put multiboot header here for true mb support)

multiboot_hdr:


section .boot
MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
FLAGS        equ 0x0            ; multiboot flags
CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum
																; (magic number + checksum + flags should equal 0)
align 4                         ; the code must be 4 byte aligned
		dd MAGIC_NUMBER             ; write the magic number to the machine code,
		dd FLAGS                    ; the flags,
		dd CHECKSUM                 ; and the checksum



[bits 16]
start:
_start:
	cli ; disable interrupts


	mov eax, gdtr32
	lgdt [eax] ; load GDT register with start address of Global Descriptor Table
	mov eax, cr0
	or al, 1       ; set PE (Protection Enable) bit in CR0 (Control Register 0)
	mov cr0, eax

	;; Perform a long-jump to the 32 bit protected mode
	jmp 0x8:gdt1_loaded

;; the 32bit protected mode gdt was loaded, now we need to go about setting up
;; paging for the 64bit long mode
[bits 32]
gdt1_loaded:
	mov eax, 0x10
	mov ds, ax
	mov ss, ax

	;; setup a starter stack
	mov esp, boot_stack_end - 1
	mov ebp, esp

	;; setup and initialize basic paging
	call paging_longmode_setup

	;; now our long mode GDT
	mov eax, gdtr64
	lgdt [eax]

	jmp 0x8:gdt2_loaded




.LOOP:
	out 0xff, ax
	push    20                                      ; 0067 _ 6A, 14
	call    fib32                                     ; 0069 _ E8, FFFFFFFC(rel)
	add     esp, 4                                  ; 006E _ 83. C4, 04
	out 0xee, ax

	jmp .LOOP



fib32:
	push    edi
	push    esi
	mov     ecx, dword [esp + 12]
	cmp     ecx, 2
	jge     .LBB0_2
	mov     eax, ecx
	pop     esi
	pop     edi
	ret
.LBB0_2:
	xor     esi, esi
	mov     eax, 1
	mov     edx, 2
.LBB0_3:
	mov     edi, eax
	mov     eax, esi
	inc     edx
	add     eax, edi
	cmp     edx, ecx
	mov     esi, edi
	jbe     .LBB0_3
	pop     esi
	pop     edi
	ret
; fib End of function





bits 64
gdt2_loaded:
	;; now that we are here, the second gdt is loaded and we are in 64bit long mode
	;; and paging is enabled.
	mov eax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov fs, ax
	mov gs, ax

	;; Reset the stack to the initial state
	mov rsp, boot_stack_end - 1
	mov rbp, rsp
	;; and call the c code in boot.c
	call kmain
	;; Ideally, we would not get here, but if we do we simply hlt spin

	;; just move a special value into eax, so we can see in the state dumps
	;; that we got here.
	mov rax, qword 0xfcfc

.spin:
	hlt
	jmp .spin


bits 32
paging_longmode_setup:

	;; recursively map p4 to itself (osdev told me to)
	mov eax, p4_table
	or eax, 0b11 ; present + writable
	mov [p4_table + 511 * 8], eax

	;; p4_table[0] -> p3_table
	mov eax, p3_table
	or eax, 0x3
	mov [p4_table], eax

	;; p3_table[0] -> p2_table
	mov eax, p2_table
	or eax, 0x3
	mov [p3_table], eax

	;; p2_table[0] -> p1_table
	mov eax, p1_table
	or eax, 0x3
	mov [p2_table], eax

	;; ident map the first 512 pages
	mov ecx, 512
	mov edx, p1_table
	mov eax, 0x3
	;; starting at 0x00

.write_pde:

	mov [edx], eax
	add eax, 4096
	add edx, 0x8 ;; shift the dst by 8 bytes (size of addr)
	loop .write_pde

	;; put pml4 address in cr3
	mov eax, p4_table
	mov cr3, eax

	;; enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax


	;; enable lme bit in MSR
	mov ecx, 0xC0000080          ; Set the C-register to 0xC0000080, which is the EFER MSR.
	rdmsr                        ; Read from the model-specific register.
	or eax, 1 << 8               ; Set the LM-bit which is the 9th bit (bit 8).
	wrmsr                        ; Write to the model-specific register.

	;; paging enable
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax                 ; Set control register 0 to the A-register.


	;; make sure we are in "normal cache mode"
	mov ebx, ~(3 << 29)
	and eax, ebx

	ret


align 8
gdt32:
	dq 0x0000000000000000 ;; null
	dq 0x00cf9a000000ffff ;; code
	dq 0x00cf92000000ffff ;; data

align 8
gdt64:
	dq 0x0000000000000000 ;; null
	dq 0x00af9a000000ffff ;; code (note lme bit)
	dq 0x00af92000000ffff ;; data (most entries don't matter)


align 8
gdtr32:
	dw 23
	dd gdt32


align 8
global gdtr64
gdtr64:
	dw 23
	dq gdt64



print_regs:
	out 0x3f, eax
	ret
set_start:
	rdtsc
	out 0xfd, eax
	ret
print_time:
	rdtsc
	out 0xfe, eax
	ret
