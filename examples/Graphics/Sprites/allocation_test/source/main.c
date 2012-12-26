#include <nds.h>
#include <stdio.h>
#include <time.h> // for time()

//our very simple sprite engine...
#define SPRITE_MAX 256


// All sprites are the same size
#define SPRITE_SIZE SpriteSize_8x8
#define SPRITE_COLORF SpriteColorFormat_256Color

//this is our game entity. Notice it has a bit more info than
//would fit into OAM.  This method is a lot more flexible than trying
//to treat oam as a game object directly.
typedef struct {
	int x,y;
	int dx, dy;
	bool alive;
	u16* gfx;
}mySprite;

//an array of sprites
mySprite sprites[SPRITE_MAX];

u32 spriteMemoryUsage = 0;

u32 oomCount = 0;
u32 allocationCount = 0;
u32 spriteMemSize = 128 * 1024;	// how is this calculated?

bool oom = false;	// out of memory?
OamState *oam = &oamMain;

//a sprite constructor
void createSprite(mySprite* s, int x, int y, int dx, int dy) {
	s->alive = true;
	s->x = x;
	s->y = y;
	s->dx = dx;
	s->dy = dy;
    
	//api: allocate a chunk of sprite graphics memory
	s->gfx = oamAllocateGfx(oam, SPRITE_SIZE, SPRITE_COLORF);
	
	allocationCount++;
	if(s->gfx) {
		spriteMemoryUsage += (SPRITE_SIZE & 0xFFF) << 5;
		oom = false;
	} else {
		oom = true;
		//only a failure of the allocator if there was enough room
		if(spriteMemoryUsage + ((SPRITE_SIZE & 0xFFF) << 5) < spriteMemSize)
			oomCount++;
	}
}

//sprite deconstructor
void killSprite(mySprite *s) {
	s->alive = false;  
 
	//api: free the graphics
	if(s->gfx) {	
		oamFreeGfx(oam, s->gfx);
		spriteMemoryUsage -= (SPRITE_SIZE& 0xFFF) << 5;
	}

	s->gfx = 0;
}


//map our sprite to oam entries
void updateSprites(void) {
	int i;

	//set oam to values required by my sprite
	for(i = 0; i < SPRITE_MAX; i++) {
		//an api function: void oamSet(OamState* oam, int id,  int x, int y, int priority, int palette_alpha, SpriteSize size, SpriteColorFormat format, const void* gfxOffset, int affineIndex, bool sizeDouble, bool hide);
		oamSet(oam, 
			i, 
			sprites[i].x, sprites[i].y, 
			0, 
			0,
			SPRITE_SIZE,
			SPRITE_COLORF,
			sprites[i].gfx, 
			-1, 
			false, 
			!sprites[i].alive,
			false,
			false, 
			false);
	}
}

//create a sprite with a random position, speed, and size 
void randomSprite(mySprite* s) {
	//pick a random color index 
	//u8 c = rand() % 2;
	u8 c = 1;

	//two pixels at a time
	u16 color = c | (c << 8);

	//create a randomly oriented sprite going off in a random direction
	createSprite(s, rand() % 256, rand() % 192, rand() % 4 - 2, rand() % 4 - 2);

	//dont let sprites get stuck with 0 velocity
	if(s->dx == 0 && s->dy == 0) {   
		s->dx = rand() % 3 + 1;
		s->dy = rand() % 3 + 1;
	}

	//the size (in pixels) is encoded in the low 12 bits of the Size attribute (shifted left by 5)
	//we load new graphics each time as this is as much a test of my allocator as an example of api usage
	if(s->gfx) {
		swiCopy(&color, s->gfx, ((SPRITE_SIZE & 0xFFF) << 4) | COPY_MODE_FILL);
	}	else {	
		s->alive = false;
	}
}

//move the sprite based on its velocity, kill it if it goes off screen and create a new random one
void moveSprites(void) {
	int i;

	for(i = 0; i < SPRITE_MAX; i++) {
		sprites[i].x += sprites[i].dx;
		sprites[i].y += sprites[i].dy;

		if(sprites[i].x >= 256 || sprites[i].x < 0 || sprites[i].y >= 192 || sprites[i].y < 0) {
			killSprite(&sprites[i]);
			randomSprite(&sprites[i]);
		}
	}
}

int main(void)  {
	int i;
	int memUsageTemp = 0xFFFFFFFF;

	// seed our rand
	srand(time(NULL));

	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_SPRITE);
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	vramSetBankD(VRAM_D_SUB_SPRITE);

	consoleDemoInit();
//	consoleDebugInit(DebugDevice_NOCASH); //send stderr to no$gba debug window

	//api: initialize OAM to 1D mapping with XX byte offsets and no external palette
	oamInit(oam, SpriteMapping_1D_128, false);

	//create some sprites
	for(i = 0; i < SPRITE_MAX; i++)
		randomSprite(&sprites[i]);
	//load a randomly colored palette
	for(i = 0; i < 256; i++) {
      SPRITE_PALETTE[i] = rand();
      SPRITE_PALETTE_SUB[i] = rand();
	}

	while(1) { 
		moveSprites();

		updateSprites();

		swiWaitForVBlank();
		
		//api: updates real oam memory 
		oamUpdate(oam);

		if(oom) {	
			memUsageTemp = memUsageTemp > spriteMemoryUsage ? spriteMemoryUsage : memUsageTemp;
    	}	

		consoleClear();
		
		printf("Memory usage: %i %i%% \n",  spriteMemoryUsage, 100 * spriteMemoryUsage / (spriteMemSize));
		printf("Percentage fail: %i%% \n", oomCount * 100 / allocationCount);
		printf("Lowest Usage at fail %i %i%% \n", memUsageTemp, 100 * memUsageTemp / (spriteMemSize));				
	}

	return 0;
}
