<!-- [![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QmHxyWUn) -->

# Page Table Output Explanation 

Below is the output we got upon running test.c:
```
print_pt starting
va: 0xDEF | pte: 0x7FF703E | pa: 0x7FF7DEF | perm: 0x3E
va: 0x1DEF | pte: 0x7FF803E | pa: 0x7FF8DEF | perm: 0x3E
va: 0x2DEF | pte: 0x7FF901E | pa: 0x7FF9DEF | perm: 0x1E
va: 0x3DEF | pte: 0x7FFA03E | pa: 0x7FFADEF | perm: 0x3E
va: 0x4DEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0x5DEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0x6DEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0x7DEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0x8DEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0x9DEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0xFFF6DEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0xFFF7DEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0xFFF8DEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0xFFF9DEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0xFFFADEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0xFFFBDEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0xFFFCDEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0xFFFDDEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0xFFFEDEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
va: 0xFFFFDEF | pte: 0xFFFFFFFF | pa: 0xFFFFFDEF | perm: 0xFFF
print_pt: OK
ugetpid_test starting
ugetpid_test: OK
print_kpt starting
Top 10 PDEs/PTEs at kernel level:
va: 0x80000def | pde: 0x40e | pa: 0xdef | perm: 0x40e
va: 0x80100def | pte: 0x10003e | pa: 0x100def | perm: 0x3e
va: 0x80200def | pte: 0x20003e | pa: 0x200def | perm: 0x3e
va: 0x80300def | pte: 0x30003e | pa: 0x300def | perm: 0x3e
va: 0x80400def | pte: 0x40003e | pa: 0x400def | perm: 0x3e
va: 0x80500def | pte: 0x50003e | pa: 0x500def | perm: 0x3e
va: 0x80600def | pte: 0x60003e | pa: 0x600def | perm: 0x3e
va: 0x80700def | pte: 0x70003e | pa: 0x700def | perm: 0x3e
va: 0x80800def | pte: 0x80003e | pa: 0x800def | perm: 0x3e
va: 0x80900def | pte: 0x90003e | pa: 0x900def | perm: 0x3e
Last 10 PDEs/PTEs at kernel level:
va: 0x87600def | pte: 0x760003e | pa: 0x7600def | perm: 0x3e
va: 0x87700def | pte: 0x770003e | pa: 0x7700def | perm: 0x3e
va: 0x87800def | pte: 0x780003e | pa: 0x7800def | perm: 0x3e
va: 0x87900def | pte: 0x790003e | pa: 0x7900def | perm: 0x3e
va: 0x87a00def | pte: 0x7a0003e | pa: 0x7a00def | perm: 0x3e
va: 0x87b00def | pte: 0x7b0003e | pa: 0x7b00def | perm: 0x3e
va: 0x87c00def | pte: 0x7c0003e | pa: 0x7c00def | perm: 0x3e
va: 0x87d00def | pte: 0x7d0003e | pa: 0x7d00def | perm: 0x3e
va: 0x87e00def | pte: 0x7e0003e | pa: 0x7e00def | perm: 0x3e
va: 0x87f00def | pte: 0x7f0003e | pa: 0x7f00def | perm: 0x3e
print_kpt: OK
superpg_test starting
pttest: superpg_test failed: pte different, pid=3
```

Now lets understand what this output means.

So basically the output is from four functions in test.c: `print_pt()`, `ugetpid_test()`, `print_kpt()`, and `superpg_test()`. 

## 1. User Page Table Test (`print_pt()`)

The function `print_pt()` in `test.c` prints the first and last 10 user PTEs:
- For each virtual address, it calls `pgpte()` which walks the page directory of the user process to get the raw PTE value.
- It prints:
    - `va`: Virtual address (28 bits used, 4KB pages)
    - `pte`: Raw PTE value (32 bits)
    - `pa`: Physical address (`PTE2PA(pte) = pte & ~0xFFF`)
    - `perm`: Permission bits (`PTE_FLAGS(pte) = pte & 0xFFF`)

So from the first 16 bits of VA (bits 27-12) we get the page table index or the virtual page number (inclusive of the 2 level page table -- 8 bits at each level), and from the last 12 bits (bits 11-0) we get the offset within the page.

Then, using the page table index, we can get the PTE from the user page table. The PTE is a 32-bit value that contains the physical address base or the page frame number (31-12 bits, but not all may be occupied) and flags/permissions bits (11-0 bits).

Now, when we combine the physical address base from the PTE with the offset from the VA, we get the final physical address (PA) which is again of 32 bits. 

So, this can be seen with an example from the output:
```va 0x1DEF pte 0x7FF803E pa 0x7FF8000 perm 0x3E```

- **VA**: `0x1DEF` (in binary: `0000 0000 0000 0001 1101 1110 1111`)
  - Page table index (bits 27-12): `0000 0000 0000 0001` (1)
  - Offset (bits 11-0): `1101 1110 1111` (7663)
- **PTE**: `0x7FF803E` (in binary: `0000 0111 1111 1111 1000 0000 0011 1110`)
  - PA base (bits 31-12): `0111 1111 1111 1000 0000` (0x7FF8000)
  - Flags/Perm (bits 11-0): `0000 0011 1110` (0x3E)
- **PA**: `0x7FF8000 | 0xDEF = 0x7FF8DEF` (in binary: `0000 0111 1111 1111 1000 1101 1110 1111`)
- **Perm**: `0x3E` (in binary: `0000 0011 1110`)
    - Each bit represents a specific attribute (see `mmu.h`):
        - Bit 1: Valid
        - Bit 2: Writable
        - Bit 3: Cacheable
        - Bit 4: Bufferable
        - Bit 5: Access permission

## 2. ugetpid_test()

This function simply tests the `ugetpid()` syscall and is not related to page tables. It prints `ugetpid_test: OK` if the syscall works correctly.

The syscall simply returns the process ID of the calling process.

## 3. Kernel Page Table/Directory Test (`print_kpt()`)

This function prints the kernel page table entries for the current process, similar to `print_pt()` but for kernel space.
- It calls the kernel syscall `kpt()`, which prints the first and last 10 valid kernel PDEs/PTEs:
    - For each region:
        - If the PDE is a section mapping (`KPDE_TYPE`, 1MB sections), prints:
            - `va`: Virtual address
            - `pde`: Raw PDE value
            - `pa`: Physical address (`(pde & 0xFFF00000) | (va & 0xFFFFF)`)
            - `perm`: Permission bits (`pde & 0xFFF`)
        - If the PDE is a coarse page table (`UPDE_TYPE`, 4KB pages), prints:
            - `va`: Virtual address
            - `pte`: Raw PTE value
            - `pa`: Physical address (`PTE_ADDR(pte) | (va & 0xFFF)`)
            - `perm`: Permission bits (`pte & 0xFFF`)

So, similar to user space, the VA is used to index into the page directory/table to get the PDE/PTE. The PDE/PTE contains the physical address base and flags/permissions.

Note, that here VA is of 32 bits (since kernel space uses full 32-bit addressing), and the calculations for PA differ based on whether it's a section mapping or a coarse page table.

### How VA, PA, and PERM Are Calculated (Kernel)
- **VA**: The virtual address being queried.
- **PDE/PTE**: Raw value from the kernel page directory/table.
- **PA (section)**: `(pde & 0xFFF00000) | (va & 0xFFFFF)` (basically takes the upper bits from PDE and lower 20 bits from VA)
- **PA (coarse/4KB)**: `PTE_ADDR(pte) | (va & 0xFFF)` (basically takes the upper bits from PTE and lower 12 bits from VA)
- **PERM/FLAGS**: Lower 12 bits (`pde & 0xFFF` or `pte & 0xFFF`)  

- **For PDE (section mapping):**
    - PA base: bits 31–20 (1MB aligned)
    - Flags: bits 11–0

- **For PTE (small page):**
    - PA base: bits 31–12 (4KB aligned)
    - Flags: bits 11–0

### Example from Output

```va 0x80100def pte 0x10003e pa 0x100def perm 0x3e```
- **VA**: `0x80100def` (in binary: `1000 0000 0001 0000 0000 1101 1110 1111`)
  - Page table index (bits 27-12): `1000 0000 0001 0000 0000` (32768 + 256 = 33024)
  - Offset (bits 11-0): `1101 1110 1111` (7663)
- **PTE**: `0x10003e` (in binary: `0000 0001 0000 0000 0000 0000 0011 1110`)
  - PA base (bits 31-12): `0000 0001 0000 0000 0000` (0x100000)
  - Flags/Perm (bits 11-0): `0000 0011 1110` (0x3E)
- **PA**: `0x100000 | 0xDEF = 0x100DEF`

## 4. superpg_test()

This function tests superpage mappings. The output indicates that the test failed because the expected PTE did not match the actual PTE. This is because the vanilla XV6 does not support super-paging.

---

<!-- 
## Bit Layout of PDE/PTE Entries

### ARMv6 PDE/PTE Structure (32 bits)

| Bits      | Field         | Meaning                                  |
|-----------|---------------|------------------------------------------|
| 31–12     | PA Base       | Physical address base (aligned)          |
| 11–0      | Flags/Perm    | Type, permissions, cache/buffer, etc.    |



---

### Example: PTE from Output

**Raw PTE:** `0x10003e`  
**VA:** `0x80100abc`

#### Breakdown (in binary):

| Bits      | Value (binary)         | Value (hex) | Meaning                |
|-----------|------------------------|-------------|------------------------|
| 31–12     | 0001 0000 0000 0000 0000 | 0x100000   | PA base (4KB aligned)  |
| 11–0      | 0000 0011 1110           | 0x3e       | Flags/Perm bits        |

#### Calculation:

- **Physical Address (PA):**
  - PA base: `0x10003e & 0xFFFFF000 = 0x10000000`
  - Offset: `0x80100abc & 0xFFF = 0xABC`
  - Final PA: `0x10000000 | 0xABC = 0x10000ABC`

- **Permissions/Flags:**
  - `perm = 0x10003e & 0xFFF = 0x3e`
  - In binary: `0b0000 0011 1110`
    - Each bit represents a specific attribute (see `mmu.h`):
      - Bit 1: Valid
      - Bit 2: Writable
      - Bit 3: Cacheable
      - Bit 4: Bufferable
      - Bit 5: Access permission

---

### Example: PDE from Output

**Raw PDE:** `0x40e`  
**VA:** `0x80000abc`

#### Breakdown (in binary):

| Bits      | Value (binary)         | Value (hex) | Meaning                |
|-----------|------------------------|-------------|------------------------|
| 31–20     | 0000 0000 0000 0000 0100 | 0x00000    | PA base (1MB aligned)  |
| 19–12     | 0000 0000               | 0x00        | Unused                 |
| 11–0      | 0100 0000 1110          | 0x40e       | Flags/Perm bits        |

#### Calculation:

- **Physical Address (PA):**
  - PA base: `0x40e & 0xFFF00000 = 0x0`
  - Offset: `0x80000abc & 0xFFFFF = 0xABC`
  - Final PA: `0x0 | 0xABC = 0xABC`

- **Permissions/Flags:**
  - `perm = 0x40e & 0xFFF = 0x40e`
  - In binary: `0b0100 0000 1110`
    - Each bit represents a specific attribute (see `mmu.h`).

---

### Summary Table

| Entry Type | PA Bits      | Flag Bits   | Example Value | Example PA      | Example Flags (perm) |
|------------|--------------|-------------|---------------|-----------------|----------------------|
| PDE        | 31–20        | 11–0        | 0x40e         | 0xABC           | 0x40e                |
| PTE        | 31–12        | 11–0        | 0x10003e      | 0x10000ABC      | 0x3e                 |

---

**Interpretation:**  
- The upper bits of the entry give the physical address base.
- The lower 12 bits give the flags and permissions.
- The offset within the page/section comes from the lower bits of the VA.
- Each flag bit is defined in `mmu.h` and ARM documentation. -->

<!-- ## 1. User Page Table Test

The function `print_pt()` in `test.c` prints the first and last 10 user PTEs:

- For each virtual address, it calls `pgpte()` which walks the page directory of the user process to get the raw PTE value.
- It prints:
	- `va`: Virtual address
	- `pte`: Raw PTE value
	- `pa`: Physical address (`PTE2PA(pte) = pte & ~0xFFF`)
	- `perm`: Permission bits (`PTE_FLAGS(pte) = pte & 0xFFF`)

### How VA, PA, and PERM Are Calculated (User)
- **VA**: The virtual address being queried.
- **PTE**: The raw value from the user page table.
- **PA**: `PTE2PA(pte) = pte & ~0xFFF` (removes lower 12 bits)
- **PERM/FLAGS**: `PTE_FLAGS(pte) = pte & 0xFFF` (lower 12 bits)

---

## 2. Kernel Page Table/Directory Introspection

The function `print_kpt()` calls the kernel syscall `kpt()`, which prints the first and last 10 valid kernel PDEs/PTEs:

- For each region:
	- If the PDE is a section mapping (`KPDE_TYPE`), prints:
		- `va`: Virtual address
		- `pde`: Raw PDE value
		- `pa`: Physical address (`(pde & 0xFFF00000) | (va & 0xFFFFF)`)
		- `perm`: Permission bits (`pde & 0xFFF`)
	- If the PDE is a coarse page table (`UPDE_TYPE`), prints:
		- `va`: Virtual address
		- `pte`: Raw PTE value
		- `pa`: Physical address (`PTE_ADDR(pte) | (va & 0xFFF)`)
		- `perm`: Permission bits (`pte & 0xFFF`)

### How VA, PA, and PERM Are Calculated (Kernel)
- **VA**: The virtual address being queried.
- **PDE/PTE**: Raw value from the kernel page directory/table.
- **PA (section)**: `(pde & 0xFFF00000) | (va & 0xFFFFF)`
- **PA (coarse/4KB)**: `PTE_ADDR(pte) | (va & 0xFFF)`
- **PERM/FLAGS**: Lower 12 bits (`pde & 0xFFF` or `pte & 0xFFF`)

---

## 3. Sample Output and Explanation

### User Page Table Output
```
print_pt starting
va 0x0 pte 0x7FF703E pa 0x7FF7000 perm 0x3E
va 0x1000 pte 0x7FF803E pa 0x7FF8000 perm 0x3E
va 0x2000 pte 0x7FF901E pa 0x7FF9000 perm 0x1E
va 0x3000 pte 0x7FFA03E pa 0x7FFA000 perm 0x3E
va 0x4000 pte 0xFFFFFFFF pa 0xFFFFF000 perm 0xFFF
... (truncated) ...
print_pt: OK
```
- For valid mappings, `pte` gives the raw entry, `pa` is the physical address, and `perm` shows the permission bits.
- For unmapped regions, `pte` is `0xFFFFFFFF` (invalid).

### Kernel Page Table Output
```
print_kpt starting
Top 10 PDEs/PTEs at kernel level:
va 0x80000abc pde 0x40e pa 0xabc perm 0x40e
va 0x80100abc pte 0x10003e pa 0x100abc perm 0x3e
va 0x80200abc pte 0x20003e pa 0x200abc perm 0x3e
... (truncated) ...
Last 10 PDEs/PTEs at kernel level:
va 0x87600abc pte 0x760003e pa 0x7600abc perm 0x3e
... (truncated) ...
print_kpt: OK
```
- The output shows both section mappings (`pde`) and 4KB page mappings (`pte`).
- For each, the calculation of VA, PA, and PERM is as described above.

---

## 4. Flag/Permission Bits (PERM/FLAGS)

- The lower 12 bits of each PDE/PTE encode:
	- **Type**: Section, coarse, small page
	- **Access permissions**: Kernel/user read/write
	- **Cacheable/Bufferable**: Memory attributes
	- See `mmu.h` for exact bit definitions.

---

## 5. Summary

- The test program (`test.c`) introspects both user and kernel page tables.
- It prints raw entries and decodes VA, PA, and permission bits for each mapping.
- The kernel syscall `kpt()` was enhanced to print both PDEs and PTEs, showing the actual mappings in use.
- The output demonstrates how ARM MMU mappings are represented and how to interpret the raw bits for debugging and analysis.

For more details, see `vm.c`, `mmu.h`, and `test.c` in the source tree. -->
