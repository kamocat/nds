#include <nds.h>

#include <stdio.h>

#include <time.h>	// for seeding rand

// git adds a nice header we can include to access the data
// this has the same name as the image
#include "spritesheet.h"

int main(void)
{
	// Seed rand
	srand(time(NULL));
	//
	// set the mode for 4 text layers
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

	consoleDemoInit();
	iprintf("\t256 color bitmap demo\n");
	iprintf("\tNow with tiling!\n");

	//bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 8, 1);
	//bgInit(1, BgType_Text8bpp, BgSize_T_256x256, 8, 1);
	//bgInit(2, BgType_Text8bpp, BgSize_T_256x256, 10, 1);
	bgInit(3, BgType_Text8bpp, BgSize_T_256x256, 0, 1);

	memcpy((void*)BG_TILE_RAM(1), spritesheetTiles, spritesheetTilesLen );

	dmaCopy(spritesheetPal, BG_PALETTE, spritesheetPalLen);	// set up the palette


	// Set up pointers to the map bases
	u16 * creatures = (u16 *)BG_TILE_RAM(8);
	u16 * ground = (u16 *)BG_TILE_RAM(0);

	// Put up random ground tiles
	for( int i = 0; i < (24 * 32); ++i ) {
		ground[i] = rand()%2;
	}

	/* random creatures array */
	for( int i = 0; i < (24*32); ++i ) {
		creatures[i] = 16; //rand()%6 + 32;
	}

	while(1) {
		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;
	}

	return 0;
}
