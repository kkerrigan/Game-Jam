#include "cDebugRenderComponent.h"
#include "cDebugRenderer.h"

cDebugRenderComponent::cDebugRenderComponent(debug_renderer_callback_def callback) : cComponent(4) {
	this->debugRendererImpl = cDebugRenderer::getInstance();
}