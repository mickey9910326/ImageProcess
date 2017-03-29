#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

void u32_to_bytes(uint32_t u32, uint8_t *bytes);
void u16_to_bytes(uint16_t u16, uint8_t *bytes);
void bytes_to_u32(uint8_t *bytes, uint32_t *u32);
void bytes_to_u16(uint8_t *bytes, uint16_t *u16);
