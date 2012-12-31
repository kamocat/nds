#include <nds.h>

#include <stdio.h>

#include <time.h>	// for seeding rand

// git adds a nice header we can include to access the data
// this has the same name as the image
#include "spritesheet.h"

#define TILEBASE 1

int main(void)
{
	// Seed rand
	srand(time(NULL));
	//
	// set the mode for 4 text layers
	videoSetMode(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

	consoleDemoInit();
	iprintf("\n\tRandom tiles\n");

	bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0, TILEBASE);
	bgInit(1, BgType_Text8bpp, BgSize_T_256x256, 1, TILEBASE);
	bgInit(2, BgType_Text8bpp, BgSize_T_256x256, 2, TILEBASE);
	bgInit(3, BgType_Text8bpp, BgSize_T_256x256, 3, TILEBASE);


	dmaCopy(spritesheetTiles, (void*)BG_TILE_RAM(TILEBASE), spritesheetTilesLen );	// set up the tile base

	dmaCopy(spritesheetPal, BG_PALETTE, spritesheetPalLen);	// set up the palette based on spritesheet.png

	// init all backgrounds to a transparent tile
	for( int b = 0; b < 4; ++b ) {
		u16 * map = bgGetMapPtr( b );
		for( int i = 0; i < (32 * 32); ++i ) {
			map[i] = 23; // a transparent tile
		}
	}


	// Set up pointers to the map bases
	u16 * gasses = bgGetMapPtr( 0 );
	u16 * creatures = bgGetMapPtr( 1 );
	u16 * plants = bgGetMapPtr( 2 );
	u16 * ground = bgGetMapPtr( 3 );

	// Put up random ground tiles
	for( int i = 0; i < (32 * 32); ++i ) {
		ground[i] = rand()%8;
	}
	
	// Put up random plant tiles
	for( int i = 0; i < (32 * 32); ++i ) {
		plants[i] = rand()%24 + 8;
	}

	// random creatures array
	for( int i = 0; i < (32 * 32); ++i ) {
		creatures[i] = rand()%22 + 16;	// make most blank
	}



	int x, y = 0;
	while(1) {
		swiWaitForVBlank();
		scanKeys();
		if(keysDown() & KEY_UP)
			x += -1;
		else if(keysDown() & KEY_DOWN)
			x += 1;
		if(keysDown() & KEY_RIGHT)
			y += 1;
		else if(keysDown() & KEY_LEFT)
			y += -1;

		// Scroll the backgrounds
		for( int i = 0; i < 4; ++i ) {
			bgSetScroll( i, x, y );
		}

	}

	return 0;
}
