	// Including libnds’ set of defines
#include <nds.h>
	// Include basic memory functions
#include <string.h>

	// Including our converted graphics
#include "arrowTile_bin.h"

	// Macro to translate 2D coord into 1D map array
#define POS2IDX(x, y)	(x + (y*32))

	// Set of defines for tile flipping
#define TILE_FLIP_NONE	(0 << 10)
#define TILE_FLIP_X		(1 << 10)
#define TILE_FLIP_Y		(2 << 10)
#define TILE_FLIP_XY	(TILE_FLIP_X | TILE_FLIP_Y)

	// Macro for palette selecting
#define TILE_PAL(n)	((n) << 12)

int main()
{
		// Setting up video mode
	REG_DISPCNT = MODE_0_2D | DISPLAY_BG0_ACTIVE;
		// This time we will use 16 colors on 16 palettes
	REG_BG0CNT = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(0) | BG_TILE_BASE(1);
	
		// Setting VRAM Bank A for Background display
	VRAM_A_CR = VRAM_ENABLE | VRAM_A_MAIN_BG;
	
		// Copying the tiles to tile base 1
	memcpy((void*)BG_TILE_RAM(1), arrowTile_bin, arrowTile_bin_size);
	
		// Setting colors on palette
	BG_PALETTE[1] = RGB15(31, 0, 0);
	BG_PALETTE[17] = RGB15(0, 31, 0);
	BG_PALETTE[33] = RGB15(0, 0, 31);
	BG_PALETTE[49] = RGB15(31, 31, 31);

		// Setting a pointer to the BG0's map base
	u16* bg0Map = (u16*)BG_TILE_RAM(0);
	
		// Plotting the first tile on the map
		// none flipped - red arrow
	bg0Map[POS2IDX(1, 1)] = 1 | TILE_FLIP_NONE | TILE_PAL(0);
		
		// Plotting the second tile on the map
		// horisontal flip - green arrow
	bg0Map[POS2IDX(1, 3)] = 1 | TILE_FLIP_X | TILE_PAL(1);

		// Plotting the thrid tile on the map
		// vertical flip - blue arrow
	bg0Map[POS2IDX(1, 5)] = 1 | TILE_FLIP_Y | TILE_PAL(2);

		// Plotting the thrid tile on the map
		// horisontal and vertical flip - white arrow
	bg0Map[POS2IDX(1, 7)] = 1 | TILE_FLIP_XY | TILE_PAL(3);

		// Infinate loop to keep the program running
	while(1);
}
