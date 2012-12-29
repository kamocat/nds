#include <nds.h>

#include <stdio.h>

// git adds a nice header we can include to access the data
// this has the same name as the image
#include "drunkenlogo.h"
#include "test.h"

int main(void)
{
	// set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_5_2D);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

	consoleDemoInit();

	iprintf("\n\n\tHello DS devers\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\t256 color bitmap demo\n");
	iprintf("\tNow with tiling!\n");

	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 3, 0);
	bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0, 1);

	memcpy((void*)BG_TILE_RAM(1), testTiles, testTilesLen );

	dmaCopy(drunkenlogoBitmap, bgGetGfxPtr(bg3), 256*256);	// put drunkenlogo as background image 3.
	dmaCopy(drunkenlogoPal, BG_PALETTE, 256*2); // not used anymore
	dmaCopy(testPal, BG_PALETTE, testPalLen);	// replace some of the palette colors


	// Set a pointer to BG0's map base
	u16 * bg0map = (u16 *)BG_TILE_RAM(0);

	/* Plot some positions on the map 
	*/
	for( int i = 0; i < 32; ++i ) {
		bg0map[i] = i%3;
		bg0map[i+32] = i%3 + 3;
	}

	while(1) {
		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;
	}

	return 0;
}
