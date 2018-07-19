/*-------------------------------------------------------------------------
   level-hack.c - source code for STM8S001 to control shock level in the Cock Shock 
                  Hack for adjustable shock intensity.

                  Philipp Klaus Krause (2018)

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

   In other words, you are welcome to use, share and improve this program.
   You are forbidden to forbid anyone else to use, share and improve
   what you give them.   Help stamp out software-hoarding!
-------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>

#define PA_ODR (*((volatile uint8_t *)0x5000))
#define PA_IDR (*((volatile uint8_t *)0x5001))
#define PA_DDR (*((volatile uint8_t *)0x5002))
#define PA_CR1 (*((volatile uint8_t *)0x5002))
#define PB_ODR (*((volatile uint8_t *)0x5005))
#define PB_IDR (*((volatile uint8_t *)0x5006))
#define PB_DDR (*((volatile uint8_t *)0x5007))
#define PB_CR1 (*((volatile uint8_t *)0x5008))
#define PC_ODR (*((volatile uint8_t *)0x500a))
#define PC_DDR (*((volatile uint8_t *)0x500c))
#define PC_CR1 (*((volatile uint8_t *)0x500d))

volatile uint_fast32_t rfword;
volatile bool rfword_valid;

void handle_rfbit(bool bit, bool bit_valid)
{
	static uint_fast8_t rfbitcount;
	static uint_fast32_t rfword_part;

	if(!bit_valid)
	{
		rfword_part = 0;
		rfbitcount = 0;
		return;
	}

	rfword_part <<= 1;
	rfword_part |= bit;
	rfbitcount++;

	if(rfbitcount == 24)
	{
		rfword = rfword_part;
		rfword_valid = true;
		rfbitcount = 0;
	}
}

void rf(void) __interrupt
{
	const bool rfstate = PA_IDR & (1 << 1); // PA bit 1 is the input from the RF module

	static uint_fast8_t highcount;
	static uint_fast8_t lowcount;

	if(rfstate && !lowcount)
		highcount++;
	else if(!rfstate)
		lowcount++;
	else // Start of next bit
	{
		if (highcount >= 2 && highcount <= 3 && lowcount >= 7 && lowcount <= 9) // Valid 0 bit
			handle_rfbit(0, true);
		else if (highcount >= 7 && highcount <= 9 && lowcount >= 2 && lowcount <= 3) // Valid 1 bit
			handle_rfbit(1, true);
		else // Invalid bit
			handle_rfbit(0, false);

		highcount = 1;
		lowcount = 0;
	}
}

void shock(uint_fast8_t level)
{
	// The lower bit of the shock level is connected to PA3 and PA5.
	PA_ODR = (level & 1) << 3;
	PB_ODR = (level & 1) << 5;

	// The upper bit of the shock level is connected to PC3, PC4, PC5.
	PC_ODR = (level & 2) ? 0x38 : 0x00;
}

void init(void)
{
	// Initalize I/O pins for output.
	PA_DDR = (1 << 3);
	PA_CR1 = (1 << 3);
	PB_DDR = (1 << 5);
	PB_CR1 = (1 << 5);
	PC_DDR = (1 << 3) | (1 << 4) | (1 << 5);
	PC_CR1 = (1 << 3) | (1 << 4) | (1 << 5);

	// Initialize timer for RF interrupt at 6.66 kHz

	// TODO
}

void main(void)
{
	uint_fast8_t shock_level = 3;
	uint32_t address;

	init();

	// Initialize address form first valid RF word received to pair to remote control.
	while(!rfword_valid);
	address = rfword >> 4;

	for(;;)
	{
		if (rfword_valid)
		{
			if ((rfword >> 4) == address)
				shock_level = rfword & 3;
			rfword_valid = false;
			if(!shock_level)
				shock_level = 3;
		}

		shock((PB_IDR & (1 << 3)) ? shock_level : 0); // PB bit 3 is the shock output from the other µC.
	}
}

