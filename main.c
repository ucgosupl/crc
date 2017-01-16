#include <stdio.h>
#include <stdint.h>

#include "crc.h"

/** Data from which CRC is calculated. */
uint8_t buf[] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0};

int main(void)
{
	uint32_t lookup[256];
	int32_t i;
	uint32_t crc;
	uint32_t ind_seed;
	uint32_t poly;

	poly = 0x04C11DB7;
	crc_lookup_calc(lookup, poly);

	for (i = 0; i < 256 / 4; i++)
	{
		printf("0x%08X\t0x%08X\t0x%08X\t0x%08X\n",
				lookup[i*4],
				lookup[i*4 + 1],
				lookup[i*4 + 2],
				lookup[i*4 + 3]);
	}

	crc = crc_calc(lookup, 0xFFFFFFFF, buf, 8);
	crc ^= 0xFFFFFFFF;
	printf("\n0x%08X", crc);

	/*
	 * To get the same results from indirect algorithm as from direct one,
	 * initial seed must be converted to indirect and zeros must be added
	 * to processed data buffer.
	 */
	ind_seed = seed_direct2indirect(poly, 0xFFFFFFFF);
	printf("\n0x%08X", ind_seed);

	crc = crc_calc_indirect(lookup, ind_seed, buf, 12);
	crc ^= 0xFFFFFFFF;
	printf("\n0x%08X", crc);

	/*
	 * Convert indirect seed to direct one - run indirect algorithm on buffer
	 * filled with zeros with size equal to CRC size.
	 */
	crc = crc_calc_indirect(lookup, ind_seed, &buf[8], 4);
	printf("\n0x%08X", crc);

	return 0;
}
