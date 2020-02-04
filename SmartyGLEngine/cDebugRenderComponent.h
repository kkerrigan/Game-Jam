#ifndef _cDebugRenderComponent_HG_
#define _cDebugRenderComponent_HG_

#include "cEntity.h"
#include "cComponent.h"

class iDebugRenderer;

class cDebugRenderComponent : public cComponent {
public:

	using debug_renderer_callback_def = void(*)(cEntity*);

	iDebugRenderer* debugRendererImpl;
	debug_renderer_callback_def callback;

public:
	cDebugRenderComponent(debug_renderer_callback_def callback);
	~cDebugRenderComponent() {}
};

#endif
