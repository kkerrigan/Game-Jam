#ifndef _cAnimationState_HG_
#define _cAnimationState_HG_

#include <string>
#include <queue>

class cAnimationState {
public:
	cAnimationState() {};

	struct sStateDetails {
		sStateDetails() :
			currentTime(0.0f),
			totalTime(0.0f),
			frameStepTime(0.0f), run_time(0),
			bDidWeReset(false)
		{
		};

		std::string name;
		float currentTime;		// Time (frame) in current animation
		float totalTime;		// Total time animation goes
		float frameStepTime;	// Number of seconds to 'move' the animation	

		float run_time;

		// Returns true if time had to be reset
		// (for checking to see if the animation has finished or not)
		// TODO: Deal with running the animation backwards, perhaps?? 
		bool IncrementTime(float delta, bool bResetToZero = true);
		bool bDidWeReset;
	};

	std::queue<sStateDetails> vecAnimationQueue;
	sStateDetails defaultAnimation;

	sStateDetails& get_current_animation()
	{
		if (this->vecAnimationQueue.empty())
		{
			return defaultAnimation;
		}
		else
		{
			sStateDetails& frontItem = this->vecAnimationQueue.front();
			
			if (frontItem.bDidWeReset)
			{
				this->vecAnimationQueue.pop();
				return get_current_animation();
			}

			return frontItem;
		}
	}

	
};

#endif 

