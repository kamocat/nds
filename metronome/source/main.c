#include <nds.h>
#include <nds/input.h>
#include <nds/touch.h>

void waitfor(int keys)
{
	scanKeys();
	while((keysDown() & keys) == 0)
	{
		swiWaitForVBlank();
		scanKeys();
	}
}


int channel = 0;
bool play = true;

//this function will be called by the timer.
void timerCallBack()
{
	if(play)
		soundPause(channel);
	else
		soundResume(channel);

	play = !play;
}

int main()
{
	soundEnable();
	channel = soundPlayPSG(DutyCycle_50, 10000, 127, 64);

	//calls the timerCallBack function 5 times per second.
	timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(5), timerCallBack);

	int freq = 5;
	uint32_t notprevkeys = 0;
	uint32_t newkeys = 0;
	while(1) {
		scanKeys();	// update the key status
		newkeys = keysDown();
		if( newkeys & notprevkeys & KEY_UP ) {
			freq++;
		} else if ( newkeys & notprevkeys & KEY_DOWN ) {
			freq--;
		} else if( !newkeys && ~notprevkeys ) { // if a key has been released
			timerStop(0);
			timerStart(0, ClockDivider_1024, TIMER_FREQ_1024(freq), timerCallBack);
		}
		notprevkeys = ~newkeys;
	}



	waitfor(KEY_A);

	return 0;
}
