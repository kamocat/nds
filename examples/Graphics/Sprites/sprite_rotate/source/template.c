/*---------------------------------------------------------------------------------

Simple sprite demo
-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>


//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	int i = 0;
	int angle = 0;

	videoSetMode(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_SPRITE);

	oamInit(&oamMain, SpriteMapping_1D_32, false);

	u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);

	/* This is where we load the image.
	 * In this case we're doing a really simple striped pattern,
	 * but we could also load a bitmap. */
	for(i = 0; i < 32 * 32 / 2; i++)
	{
		//gfx[i] = 1 | (1 << 8);
		gfx[i] = 0x201;	// alternating red and yellow
	}

	SPRITE_PALETTE[1] = RGB15(31,0,0);	// full red
	SPRITE_PALETTE[2] = RGB15(28,28,0);	// bright yellow

	while(1) {

		scanKeys();

		/* Slow down the rotate if the left trigger is pressed
		 * I'd like to do a fixed-point here, but I don't know how */
		float inc = (keysHeld() & KEY_L)? 0.05 : 0.3;

		if(keysHeld() & KEY_LEFT)
			angle += degreesToAngle(inc);
		if(keysHeld() & KEY_RIGHT)
			angle -= degreesToAngle(inc);

		//-------------------------------------------------------------------------
		// Set the first rotation/scale matrix
		//
		// There are 32 rotation/scale matricies that can store sprite rotations
		// Any number of sprites can share a sprite rotation matrix or each sprite
		// (up to 32) can utilize a seperate rotation. Because this sprite is doubled
		// in size we have to adjust its position by subtracting half of its height and
		// width (20 - 16, 20 - 16, )
		//-------------------------------------------------------------------------
		oamRotateScale(&oamMain, 0, angle, intToFixed(1, 8), intToFixed(1, 8));
    
		oamSet(&oamMain, //main graphics engine context
			0,           //oam index (0 to 127)  
			20 - 16, 20 - 16,   //x and y pixle location of the sprite
			0,                    //priority, lower renders last (on top)
			0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
			SpriteSize_32x32,     
			SpriteColorFormat_256Color, 
			gfx,                  //pointer to the loaded graphics
			0,                  //sprite rotation/scale matrix index 
			true,               //double the size when rotating?
			false,			//hide the sprite?
			false, false, //vflip, hflip
			false	//apply mosaic
			);              
		
		//-------------------------------------------------------------------------
		// Because the sprite below has size double set to false it can never be larger than
		// 32x32 causing it to clip as it rotates.  
		//-------------------------------------------------------------------------
		oamSet(&oamMain, //main graphics engine context
			1,           //oam index (0 to 127)  
			204, 20,   //x and y pixle location of the sprite
			0,                    //priority, lower renders last (on top)
			0,					  //this is the palette index if multiple palettes or the alpha value if bmp sprite	
			SpriteSize_32x32,     
			SpriteColorFormat_256Color, 
			gfx,                  //pointer to the loaded graphics
			0,                  //sprite rotation/scale matrix index 
			false,               //double the size when rotating?
			false,			//hide the sprite?
			false, false, //vflip, hflip
			false	//apply mosaic
			);              
		swiWaitForVBlank();

		
		oamUpdate(&oamMain);
	}

	return 0;
}
