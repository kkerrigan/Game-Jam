#pragma once

class cScriptUtil
{
public:
	static float clamp(float x, float lowerlimit, float upperlimit);
	
	static float smoothstep_wikipedia(float edge0, float edge1, float x);
};

