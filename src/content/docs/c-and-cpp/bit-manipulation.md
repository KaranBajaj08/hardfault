---
title: Bit manipulation — from truth tables to register fields
description: Learn AND, OR, XOR, NOT, shifts, masks, and common bit manipulation patterns in C.
sidebar:
  order: 1
---

A firmware engineer often needs to change one part of a value while preserving everything else: enable a peripheral, inspect a status flag, or extract a sensor reading from a packed message. Bit manipulation gives us the tools to do that.

This chapter starts with one bit and builds up to manipulating groups of bits in C. You should already be comfortable with variables, `if` statements, and basic arithmetic.

By the end, you should be able to explain a bit operation using a truth table, construct a mask, set or clear flags, and extract or replace a field without damaging its neighbors.

## 1. Start with one bit

A bit has two possible values: **0** and **1**. For Boolean logic, we can interpret these as false and true. A truth table lists every possible input combination and the corresponding output.

### AND: both inputs must be 1

| A | B | A AND B |
| --- | --- | --- |
| 0 | 0 | 0 |
| 0 | 1 | 0 |
| 1 | 0 | 0 |
| 1 | 1 | 1 |

Think of a motor that may run only when it is enabled **and** its safety interlock is satisfied.

For manipulating individual bits in C, AND uses `&`. Two observations will become useful when we build masks:

- AND with **0** forces the result to 0.
- AND with **1** preserves the original bit.

### OR: at least one input must be 1

| A | B | A OR B |
| --- | --- | --- |
| 0 | 0 | 0 |
| 0 | 1 | 1 |
| 1 | 0 | 1 |
| 1 | 1 | 1 |

An alarm might activate when there is an overtemperature condition **or** an overcurrent condition. Both conditions being true still produces 1.

C's bitwise OR operator is `|`:

- OR with **0** preserves the original bit.
- OR with **1** forces the result to 1.

### XOR: the inputs must differ

| A | B | A XOR B |
| --- | --- | --- |
| 0 | 0 | 0 |
| 0 | 1 | 1 |
| 1 | 0 | 1 |
| 1 | 1 | 0 |

XOR means **exclusive OR**. Unlike OR, its result is 0 when both inputs are 1.

C uses `^`:

- XOR with **0** preserves the original bit.
- XOR with **1** flips the original bit.

This makes XOR useful for toggling flags and identifying which bits changed between two readings. In C, `^` does **not** mean exponentiation.

### NOT: invert the input

| A | NOT A |
| --- | --- |
| 0 | 1 |
| 1 | 0 |

C's bitwise NOT operator, `~`, flips every bit in its operand. We will return to the importance of the operand's width later.

**Checkpoint:** Which operation forces a selected bit to 1? Which operation flips it? Answer: OR with 1 sets it; XOR with 1 toggles it.

## 2. Apply the same rules to a whole number

Number bit positions from the right, beginning at **bit 0**. Bit position `n` has unsigned value 2 to the power of `n`.

```text
Position:  7   6   5   4   3   2   1   0
Weight:  128  64  32  16   8   4   2   1
Bits:      0   0   1   0   1   1   0   0
```

This pattern represents `32 + 8 + 4 = 44`. Bit 0 is the **least significant bit (LSB)**. In this eight-bit view, bit 7 is the **most significant bit (MSB)**. Bit numbering describes a value; it does not depend on the order in which its bytes are stored in memory.

Each hexadecimal digit represents four bits. The same value is `0x2C`: `2` represents `0010`, and `C` represents `1100`.

Bitwise operators apply the single-bit truth table independently to every position:

```text
       A = 0010 1100 = 0x2C
       B = 0000 1111 = 0x0F
--------------------------------
   A & B = 0000 1100 = 0x0C
   A | B = 0010 1111 = 0x2F
   A ^ B = 0010 0011 = 0x23
```

For example, the mask `0000 1111` used with AND preserves the lower four bits and clears the upper four bits.

### Conventions for the C examples

Examples target **C11/C17 on systems with eight-bit bytes and 32-bit `unsigned int`**, a common embedded configuration. They use `<stdint.h>` types such as `uint32_t`, and `<stdbool.h>` for `bool`. The runnable example checks these platform assumptions.

We use hexadecimal constants with a `u` suffix, such as `0x2Cu` or `1u`, to make them unsigned. Binary strings in diagrams are illustrations, not C literals. Most diagrams show only the low eight bits of a 32-bit value; omitted upper bits are zero unless stated otherwise.

## 3. Bitwise logic is different from Boolean conditions

| Purpose | Operators | What they operate on |
| --- | --- | --- |
| Bit manipulation | `&`, `\|`, `^`, `~` | Individual bits of integer values |
| Logical conditions | `&&`, `\|\|`, `!` | Whether an entire value is zero or nonzero |

Consider two numbers that are both nonzero but have no set bits in common:

```c
unsigned int a = 2u;  /* 0010 */
unsigned int b = 4u;  /* 0100 */

/* a & b  is 0: no shared 1 bits. */
/* a && b is 1: both numbers are nonzero. */
/* !a     is 0: a is true as a condition. */
```

In C, logical results are `int` values of 0 or 1. `&&` and `||` also short-circuit: they skip the right operand when the left operand determines the result. Bitwise `&` and `|` do not provide that protection. Use logical operators to combine conditions; use bitwise operators to inspect or change bits.

## 4. Move bits with shifts

The operator `<<` shifts left; `>>` shifts right.

```text
Value:          0000 1101 = 13
Shift left 1:   0001 1010 = 26
Shift right 1:  0000 0110 =  6
```

For unsigned values, right shift fills vacated high positions with zeros. Left shift fills vacated low positions with zeros. Bits shifted beyond the operation's width are discarded; shifting does not rotate them to the other end.

Shifting left by `n` multiplies an unsigned value by 2 to the power of `n`, modulo its unsigned range. Shifting right divides by that power of two, discarding the remainder. Choose shifts when expressing bit positions, rather than assuming they are always faster than arithmetic.

### Construct a one-bit mask

A **mask** is a pattern that selects the bits an operation should affect.

```c
unsigned int bit = 5u;
uint32_t mask = 1u << bit;  /* 0x20: only bit 5 is set. */
```

```text
1u:       0000 0001
1u << 5:  0010 0000
```

Under our 32-bit `unsigned int` assumption, the shift count must be between **0 and 31**. A count of 32 is not a way to obtain zero: the expression has undefined behavior. Check a runtime bit index before constructing the mask.

## 5. Set, clear, toggle, and test a bit

Start each example independently with `value = 0x2C` (`0010 1100`) and select bit 4 using `mask = 1u << 4` (`0001 0000`).

### Set: OR with the mask

```c
value |= mask;  /* Equivalent here to value = value | mask. */
```

```text
Value:  0010 1100
Mask:   0001 0000
OR:     0011 1100 = 0x3C
```

The mask's 1 forces bit 4 to 1. Every mask bit that is 0 leaves its corresponding value bit alone. Setting an already-set bit leaves it set.

### Clear: AND with the inverted mask

```c
value &= ~mask;
```

```text
Value:  0010 1100
~Mask:  1110 1111  (low eight bits)
AND:    0010 1100 = 0x2C
```

The inverted mask has a 0 only at the selected position. Bit 4 was already 0, so the value is unchanged. Clearing bit 3 instead produces `0010 0100` (`0x24`).

Why not `value &= mask`? That keeps the selected bit and clears everything else—the opposite of the intended operation.

### Toggle: XOR with the mask

```c
value ^= mask;
```

Bit 4 changes from 0 to 1, producing `0x3C`. Repeat the operation and it returns to `0x2C`. Setting twice preserves the set state; toggling twice restores the original state.

### Test: AND, then compare with zero

```c
bool is_set = (value & mask) != 0u;
```

The result is false for the original value. When a selected bit is set, `value & mask` returns that bit's weight, not necessarily 1. For bit 4, the nonzero result is `0x10`.

Use `(value & mask) != 0u`, not `(value & mask) == 1u`.

## 6. Work with multiple flags

Combine single-bit masks using OR. Named masks make the intent easier to read:

```c
const uint32_t READY = 1u << 0;
const uint32_t ERROR = 1u << 2;
const uint32_t BUSY  = 1u << 5;

uint32_t flags = READY | BUSY;
uint32_t selected = READY | ERROR;

bool any = (flags & selected) != 0u;        /* true */
bool all = (flags & selected) == selected;  /* false */
bool none = (flags & selected) == 0u;       /* false */

flags |= ERROR;              /* Add ERROR; preserve READY and BUSY. */
flags &= ~(READY | BUSY);    /* Clear both; leave only ERROR. */
```

**Any** asks whether at least one selected bit is set. **All** asks whether every selected bit is set. With an empty mask, `any` is false and `all` is true; reject an empty selection if that is inappropriate for your API.

## 7. Extract and replace a field

A flag represents one yes/no state. A **field** uses several bits to encode a number. Imagine a configuration value where bits **[6:4]** hold a three-bit mode, from 0 through 7. The notation includes both endpoints.

```text
Position:  7  6  5  4  3  2  1  0
Value:     1  0  1  1  0  1  0  1 = 0xB5
              -------
Mode:         0  1  1             = 3
```

### Extract: mask, then shift down

```c
const unsigned int MODE_SHIFT = 4u;
const uint32_t MODE_MASK = 0x7u << MODE_SHIFT;  /* 0x70 */
uint32_t config = 0xB5u;

uint32_t mode = (config & MODE_MASK) >> MODE_SHIFT;  /* 3 */
```

```text
0xB5 & 0x70:  1011 0101 & 0111 0000 = 0011 0000
Shift by 4:                            0000 0011
```

### Replace: clear the old field, then insert the new one

```c
uint32_t new_mode = 5u;  /* Must be in the range 0..7. */
config = (config & ~MODE_MASK)
       | ((new_mode << MODE_SHIFT) & MODE_MASK);  /* 0xD5 */
```

```text
Original:     1011 0101
Clear [6:4]:  1000 0101
New mode:     0101 0000
Combine:      1101 0101 = 0xD5
```

Simply OR-ing the new mode into the old value would combine the two encodings. Here, old mode `011` OR new mode `101` becomes `111` (7), rather than the intended 5.

Masking the shifted input prevents it from changing neighboring bits, but does not validate it: input 13 would silently truncate to 5. Reject an out-of-range mode when that indicates a caller error.

## 8. Two common interview patterns

### Check whether an unsigned value is odd

```c
bool odd = (value & 1u) != 0u;
```

Every binary place except bit 0 contributes an even number. Only bit 0 determines oddness.

### Clear the lowest set bit

```c
value &= value - 1u;
```

Subtracting 1 changes the lowest 1 to 0 and changes all lower zeros to ones. AND with the original value clears that lowest 1 and preserves the higher bits:

```text
value:      0010 1100
value - 1:  0010 1011
AND:        0010 1000
```

A nonzero unsigned power of two has exactly one set bit, so removing it leaves zero:

```c
bool power_of_two = (value != 0u)
                 && ((value & (value - 1u)) == 0u);
```

The nonzero check matters: zero is not a power of two. Under unsigned arithmetic, `0u - 1u` wraps, and the AND expression alone would incorrectly accept zero.

## 9. C pitfalls worth learning early

### Parenthesize the bit operation before comparing

```c
/* Wrong grouping: value & (mask == 0u) */
if (value & mask == 0u) { /* ... */ }

/* Intended grouping */
if ((value & mask) == 0u) { /* ... */ }
```

Similarly, prefer `1u << (bit + 1u)` to relying on readers remembering precedence.

### The operation's type matters, not just its destination

`uint8_t` values undergo integer promotion before many operations. On the platform assumed here, they promote to `int`:

```c
uint8_t byte = 0x0Fu;
uint8_t inverted = (uint8_t)(~(unsigned int)byte);  /* 0xF0 */
```

The explicit unsigned conversion makes the complement operate on an unsigned value. The final conversion retains the low eight bits. The complement itself was not an eight-bit operation.

Likewise, `uint32_t result = 1 << 31;` still shifts the signed literal `1` before assignment. On our assumed platform, use `1u << 31` for the top bit. On a target with a different integer width, choose a sufficiently wide unsigned operand and revisit the shift bounds.

### Use unsigned operands for bit patterns

Signed shifts introduce extra rules, especially for negative values and overflow. Keep the introductory patterns unsigned. The shift count must be less than the width of the **promoted left operand**, regardless of the destination variable's size.

These type and shift rules follow the C language specification; CERT's unsigned-operand guidance is a useful coding convention. See the references below.

## 10. Applying this to actual hardware

Our `config` and `flags` variables are ordinary values in RAM. A hardware register may have additional behavior defined by the device manual.

- `REGISTER |= mask` performs a read-modify-write. It is not automatically atomic; concurrent updates by an interrupt handler, another core, or hardware can be lost.
- Some devices provide dedicated SET and CLEAR registers. Use the device's documented mechanism when appropriate.
- A **write-one-to-clear** status bit is cleared by writing 1 to it. The ordinary `&= ~mask` recipe does not apply. A read-modify-write can also accidentally clear other pending flags.
- Read side effects and reserved bits require following the register's documented access rules.

`volatile` does not make a read-modify-write atomic. Treat the recipes as operations on values first, then check the peripheral's register semantics before applying them to hardware.

## 11. Practice

Try these before opening the answers. Treat the values as unsigned, using the platform assumptions above.

1. For `A = 0xA6` and `B = 0x3C`, calculate `A & B`, `A | B`, and `A ^ B`.
2. Start with `0x52`. Set bit 0, then clear bit 4, then toggle bit 6. What remains?
3. Write a condition that checks whether **both** bits 1 and 7 are set.
4. Extract bits [5:3] from `0xD8`.
5. Replace bits [5:3] in `0xD8` with the value 2, preserving all other bits.
6. Explain why `value & (1u << 5) == 1u` is not a correct test for bit 5.
7. Explain why `config |= new_mode << 4` cannot reliably replace a field.
8. Why is `1u << 32` invalid under this chapter's platform assumptions?

<details>
<summary>Answers and explanations</summary>

1. AND: `0x24`; OR: `0xBE`; XOR: `0x9A`.
2. `0x52 → 0x53 → 0x43 → 0x03`.
3. With `mask = (1u << 1) | (1u << 7)`, use `(value & mask) == mask`.
4. `(0xD8u & 0x38u) >> 3` gives 3.
5. `(0xD8u & ~0x38u) | ((2u << 3) & 0x38u)` gives `0xD0`.
6. Equality binds before AND. Even with corrected parentheses, a set bit 5 yields `0x20`, not 1. Use `(value & (1u << 5)) != 0u`.
7. OR cannot clear old 1 bits that need to become 0. Clear the field first.
8. The shift count equals the width of the promoted left operand. C requires a smaller count.

</details>

## Quick reference

For unsigned `value` and `mask`, with valid shift counts:

| Intent | Expression |
| --- | --- |
| Construct a mask for bit `n` | `1u << n` |
| Set selected bits | `value \|= mask` |
| Clear selected bits | `value &= ~mask` |
| Toggle selected bits | `value ^= mask` |
| Any selected bit is set | `(value & mask) != 0u` |
| All selected bits are set | `(value & mask) == mask` |
| No selected bits are set | `(value & mask) == 0u` |
| Extract a field | `(value & mask) >> shift` |
| Replace a field | `(value & ~mask) \| ((field << shift) & mask)` |
| Clear the lowest set bit | `value &= value - 1u` |

## Run the examples

The repository includes [examples/bit-manipulation/basics.c](https://github.com/KaranBajaj08/hardfault/blob/main/examples/bit-manipulation/basics.c), a host-runnable C11 program. It checks the truth tables, worked examples, and exercise answers, plus boundary bit positions and field preservation. No board is required.

From the repository root:

```sh
cc -std=c11 -Wall -Wextra -Werror -pedantic examples/bit-manipulation/basics.c -o /tmp/hardfault-bit-basics
/tmp/hardfault-bit-basics
```

Expected output: `All bit manipulation checks passed.`

## References

- [WG14 C11 committee draft N1570](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf): integer promotions (§6.3.1.1), unary operators (§6.5.3.3), shifts (§6.5.7), and bitwise/logical operators (§6.5.10–14).
- [SEI CERT C: INT13-C — Use bitwise operators only on unsigned operands](https://cmu-sei.github.io/secure-coding-standards/sei-cert-c-coding-standard/recommendations/integers-int/int13-c/).
- [SEI CERT C: INT34-C — Shift-count bounds](https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=88014861).

For hardware applications, consult the reference manual for the exact microcontroller and register being accessed.
