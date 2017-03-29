#include "byte_op.h"

static void u32_to_bytes(uint32_t u32, uint8_t *bytes) {
	*bytes = (uint8_t) (u32 >> 24);
	*(bytes + 1) = (uint8_t) (u32 >> 16);
	*(bytes + 2) = (uint8_t) (u32 >> 8);
	*(bytes + 3) = (uint8_t) u32;
}

static void bytes_to_u32(uint8_t *bytes, uint32_t *u32) {
	*u32 = (uint32_t) ((*(bytes) ) |
           (uint32_t) (*(bytes + 1) << 8 ) |
		   (uint32_t) (*(bytes + 2) << 16) |
           (uint32_t) (*(bytes + 3) << 24) );
}

static void u16_to_bytes(uint16_t u16, uint8_t *bytes) {
	*bytes = (uint8_t) (u16 >> 8);
	*(bytes + 1) = (uint8_t) u16;
}

static void bytes_to_u16(uint8_t *bytes, uint16_t *u16) {
	*u16 = (uint16_t) ((*bytes ) | *(bytes + 1) << 8);
}
