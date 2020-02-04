#include <cEngine.h>
#include "cEntity.h"
#include "cFrameBufferComponent.h"

extern std::vector<cEntity*> fbos;

void window_size_callback(GLFWwindow* window, int width, int height)
{
	for(size_t i=0; i<fbos.size(); ++i)
	{
		std::string error;
		static_cast<cFrameBufferComponent*>(fbos[i]->components[9])->pFbo->reset(width,height, error);
	}
}