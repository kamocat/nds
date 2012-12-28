#include <nds.h>

#include <stdio.h>

// git adds a nice header we can include to access the data
// this has the same name as the image
#include "drunkenlogo.h"
#include "sprites.h"

int main(void)
{
    // set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_3_2D);
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

	consoleDemoInit();

	iprintf("\n\n\tHello DS devers\n");
	iprintf("\twww.drunkencoders.com\n");
	iprintf("\t256 color bitmap demo\n");
	iprintf("\tNow with tiling!");

	int bg3 = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	int bg0 = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0, 1);

	memcpy((void*)BG_TILE_RAM(1), spritesBitmap, 64 );	// made-up size.  Fix later.

	dmaCopy(drunkenlogoBitmap, bgGetGfxPtr(bg3), 256*256);	// put drunkenlogo as background image 3.
	dmaCopy(drunkenlogoPal, BG_PALETTE, 256*2); // not used anymore
	dmaCopy(spritesPal, BG_PALETTE, 18*2);	// copy the whole palette
	

	// Set a pointer to BG0's map base
	u16 * bg0map = (u16 *)BG_TILE_RAM(0);

	/* Plot some positions on the map 
	 * Just for the sake of the test, we'll do them
	 * sequentially.
	 */
	for( int i = 0; i < 20; i++ ) {
		bg0map[i] = i;
	}

	while(1) {
		swiWaitForVBlank();
		scanKeys();
		if (keysDown()&KEY_START) break;
	}

	return 0;
}
