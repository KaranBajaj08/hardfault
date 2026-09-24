#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

_Static_assert(CHAR_BIT == 8, "Examples assume eight-bit bytes");
_Static_assert(UINT_MAX == UINT32_MAX, "Examples assume 32-bit unsigned int");

static bool is_power_of_two(uint32_t value)
{
    return (value != 0u) && ((value & (value - 1u)) == 0u);
}

int main(void)
{
    /* Truth tables, checked against Boolean definitions. */
    for (unsigned int a = 0u; a <= 1u; ++a) {
        for (unsigned int b = 0u; b <= 1u; ++b) {
            assert((a & b) == (unsigned int)(a != 0u && b != 0u));
            assert((a | b) == (unsigned int)(a != 0u || b != 0u));
            assert((a ^ b) == (unsigned int)(a != b));
        }
        assert(((~a) & 1u) == (unsigned int)(a == 0u));
    }

    assert((0x2Cu & 0x0Fu) == 0x0Cu);
    assert((0x2Cu | 0x0Fu) == 0x2Fu);
    assert((0x2Cu ^ 0x0Fu) == 0x23u);
    unsigned int a = 2u;
    unsigned int b = 4u;
    assert((a & b) == 0u);
    assert((a && b) == 1);
    assert((!a) == 0);
    assert((13u << 1) == 26u);
    assert((13u >> 1) == 6u);

    uint32_t value = 0x2Cu;
    const uint32_t mask = 1u << 4;
    assert((value | mask) == 0x3Cu);
    assert((value & ~mask) == 0x2Cu);
    assert((value & ~(1u << 3)) == 0x24u);
    assert((value ^ mask) == 0x3Cu);
    assert(((value ^ mask) ^ mask) == value);
    assert((value & mask) == 0u);

    const uint32_t READY = 1u << 0;
    const uint32_t ERROR = 1u << 2;
    const uint32_t BUSY = 1u << 5;
    uint32_t flags = READY | BUSY;
    uint32_t selected = READY | ERROR;
    assert((flags & selected) != 0u);
    assert((flags & selected) != selected);
    flags |= ERROR;
    flags &= ~(READY | BUSY);
    assert(flags == ERROR);

    const unsigned int MODE_SHIFT = 4u;
    const uint32_t MODE_MASK = 0x7u << MODE_SHIFT;
    uint32_t config = 0xB5u;
    assert(((config & MODE_MASK) >> MODE_SHIFT) == 3u);
    config = (config & ~MODE_MASK) | ((5u << MODE_SHIFT) & MODE_MASK);
    assert(config == 0xD5u);

    /* Check every input byte and valid mode against an arithmetic oracle. */
    for (uint32_t original = 0u; original < 256u; ++original) {
        for (uint32_t mode = 0u; mode < 8u; ++mode) {
            uint32_t replaced = (original & ~MODE_MASK)
                              | ((mode << MODE_SHIFT) & MODE_MASK);
            uint32_t expected = (original / 128u) * 128u
                              + mode * 16u + original % 16u;
            assert(replaced == expected);
            assert(((replaced & MODE_MASK) >> MODE_SHIFT) == mode);
            assert((replaced & ~MODE_MASK) == (original & ~MODE_MASK));
        }
    }

    /* Include bit 0 and bit 31, and preservation of unrelated high bits. */
    for (unsigned int bit = 0u; bit < 32u; ++bit) {
        uint32_t bit_mask = 1u << bit;
        uint32_t original = 0xA5A55A5Au;
        assert(((original | bit_mask) & bit_mask) == bit_mask);
        assert(((original & ~bit_mask) & bit_mask) == 0u);
        assert(((original ^ bit_mask) ^ bit_mask) == original);
        assert(((original | bit_mask) & ~bit_mask) == (original & ~bit_mask));
        assert(is_power_of_two(bit_mask));
    }
    assert(!is_power_of_two(0u));
    assert(!is_power_of_two(3u));
    assert(!is_power_of_two(UINT32_MAX));
    assert((0x2Cu & (0x2Cu - 1u)) == 0x28u);
    for (uint32_t x = 0u; x < 256u; ++x) {
        assert(((x & 1u) != 0u) == ((x % 2u) != 0u));
    }
    uint8_t byte = 0x0Fu;
    assert((uint8_t)(~(unsigned int)byte) == 0xF0u);

    /* Exercise answers. */
    assert((0xA6u & 0x3Cu) == 0x24u);
    assert((0xA6u | 0x3Cu) == 0xBEu);
    assert((0xA6u ^ 0x3Cu) == 0x9Au);
    value = 0x52u;
    value |= 1u;
    assert(value == 0x53u);
    value &= ~(1u << 4);
    assert(value == 0x43u);
    value ^= 1u << 6;
    assert(value == 0x03u);
    selected = (1u << 1) | (1u << 7);
    assert((0x82u & selected) == selected);
    assert((0x80u & selected) != selected);
    assert(((0xD8u & 0x38u) >> 3) == 3u);
    assert(((0xD8u & ~0x38u) | ((2u << 3) & 0x38u)) == 0xD0u);

    puts("All bit manipulation checks passed.");
    return 0;
}
