#include "cAnimationState.h"

// Returns true if time had to be reset
// (for checking to see if the animation has finished or not)
bool cAnimationState::sStateDetails::IncrementTime(float delta, bool bResetToZero /*=true*/) {
	
	this->run_time += delta;

	this->bDidWeReset = false;

	this->currentTime += delta;

	if (this->currentTime >= this->totalTime) {
		bDidWeReset = true;
		this->currentTime = 0.0f + (this->currentTime - this->totalTime);
	}

	return bDidWeReset;
}

