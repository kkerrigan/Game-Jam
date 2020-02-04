
#include "cScriptUtil.h"


float cScriptUtil::clamp(float x, float lowerlimit, float upperlimit) {
	if (x < lowerlimit)
		x = lowerlimit;
	if (x > upperlimit)
		x = upperlimit;
	return x;
}

float cScriptUtil::smoothstep_wikipedia(float edge0, float edge1, float x) {

	if (x == 0.0f)
	{
		return edge0;
	}

	// Scale, bias and saturate x to 0..1 range
	x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	// Evaluate polynomial
	float retvalue = x * x * (3 - 2 * x);
	return retvalue;
}