#include  "cEngine.h"

#include "cEulerMovementSystem.h"
#include "cMeshRenderSystem.h"
#include "cDebugRenderSystem.h"
#include "cColliderSystem.h"
#include "cDebugRenderer.h"
#include "cShaderManager.h"
#include "cLightManager.h"
#include "sLight.h"
#include "cSoundManager.h"
#include "cScriptingCommandSystem.h"
#include "cParticleSystem.h"
#include "cThreadWorkerComponent.h"
#include "cBehaviourSystem.h"

// c'tor
cEngine::cEngine() : pWindow(nullptr), settings(nullptr), physics_factory(nullptr), physics_world(nullptr),
                     pFuncUpdateCallback(nullptr),
                     pFuncKeyboardCallbackAsync(nullptr), pFuncMouseCallbackAsync(nullptr)
{
}

// d'tor
cEngine::~cEngine() {
	this->terminate();

	return;
}

cEngine* cEngine::getInstance() {
	static cEngine instance;

	return &instance;
}

GLFWwindow* cEngine::getWindow() {
	return this->pWindow;
}

/*
*	Initialize the engine (things like glfw, render window and glad)
*
*	returns true on success and false on failure
*/
bool cEngine::initialize(sWindowSettings* pWindowSettings) {

	// check if we have initialized the engine already
	if (this->pWindow != nullptr) {
		return false;
	}

	this->settings = pWindowSettings;

	// initialize glfw
	if (!glfwInit()) {
		return false;
	}

	// set some hints for the driver
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWmonitor* monitor = nullptr;// glfwGetPrimaryMonitor();

	// create a window
	this->pWindow = glfwCreateWindow(pWindowSettings->width, pWindowSettings->height, pWindowSettings->title.c_str(), monitor, nullptr);
	if (!this->pWindow)
	{
		glfwTerminate();
		return false;
	}

	// make the window the current context
	glfwMakeContextCurrent(this->pWindow);
	glfwSwapInterval(1);

	// get all the openGL function now that we have a window
	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	//load the physics implementation
	if (!this->physics_dll_loader.load_dll(this->settings->physics_implementation))
	{
		return false;
	}

	//find the create factory function in the dll
	const physics::interfaces::f_create_physics_factory func = 
		this->physics_dll_loader.get_proc_address<physics::interfaces::f_create_physics_factory>("create_factory");

	if (func == nullptr)
	{
		return false;
	}

	//create the factory
	this->physics_factory = func();
	if (this->physics_factory == nullptr)
	{
		return false;
	}

	//create the physics world
	this->physics_world = this->physics_factory->create_physics_world();

	//critical section
	InitializeCriticalSection(&cThreadWorkerComponent::should_run_critical_section);

	//we good?
	return this->physics_world != nullptr;
}

/*
*	Enter the game loop
*
*/
void cEngine::startGameLoop() {

	int program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

	double lastFrameTime = glfwGetTime();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(this->pWindow)) {

		// keep track of delta time
		double nowTime = glfwGetTime();
		double deltaTime = nowTime - lastFrameTime;
		lastFrameTime = nowTime;

		if (deltaTime > 0.01f)
		{
			deltaTime = 0.01f;
		}

		// clear the backbuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// push lights to gpu
		cLightManager* lightManager = cLightManager::getInstance();
		lightManager->writeLightsToShader();

		// process the keys async
		if (this->pFuncKeyboardCallbackAsync != nullptr) {
			this->pFuncKeyboardCallbackAsync(this->pWindow, deltaTime);
		}

		// get mouse input
		if (this->pFuncMouseCallbackAsync != nullptr) {
			this->pFuncMouseCallbackAsync(this->pWindow);
		}

		//physics system
		if(this->physics_world != nullptr)
		{
			this->physics_world->update(deltaTime);
		}

		// run all the component systems
		cEulerMovementSystem::getInstance()->update(deltaTime);
		cColliderSystem::getInstance()->update(deltaTime);
		//cDebugRenderSystem::getInstance()->update(deltaTime);
		cMeshRenderSystem::getInstance()->update(deltaTime);
		
		//scripted stuff system
		cScriptingCommandSystem::getInstance()->update(deltaTime);

		cBehaviourSystem::get_instance()->update(deltaTime);

		//update 3d sounds
		cSoundManager::getInstance()->update();

		// present the back buffer
		glfwSwapBuffers(this->pWindow);

		// allow user input
		glfwPollEvents();
	}

	this->terminate();
}

/*
*	Clean-up the engine (glfw..)
*
*/
void cEngine::terminate() {

	cThreadWorkerComponent::stop_threads();

	glfwDestroyWindow(this->pWindow);
	glfwTerminate();

	this->physics_dll_loader.unload();

	return;
}

physics::interfaces::iPhysicsFactory* cEngine::get_physics_factory()
{
	return this->physics_factory;
}

physics::interfaces::iPhysicsWorld* cEngine::get_world()
{
	return this->physics_world;
}

sWindowSettings* cEngine::get_window_settings()
{
	return this->settings;
}

void cEngine::setUpdateCallback(update_callback_function_def func) {
	this->pFuncUpdateCallback = func;

	return;
}

bool cEngine::setKeyboardCallback(keyboard_callback_function_def func) {
	if (this->pWindow == nullptr) {
		return false;
	}

	glfwSetKeyCallback(this->pWindow, func);

	return true;
}

void cEngine::setKeyboardCallbackAsync(keyboard_callback_async_function_def func) {
	this->pFuncKeyboardCallbackAsync = func;

	return;
}

void cEngine::setMouseCallbackAsync(mouse_callback_async_function_def func) {
	this->pFuncMouseCallbackAsync = func;

	return;
}

void cEngine::setMouseCursorPositionCallback(mouse_cursor_pos_function_def func) {
	glfwSetCursorPosCallback(this->pWindow, func);

	return;
}

void cEngine::setMouseButtonCallback(mouse_button_callback_function_def func) {
	glfwSetMouseButtonCallback(this->pWindow, func);

	return;
}

void cEngine::setMouseEnterCallback(mouse_cursor_enter_function_def func) {
	glfwSetCursorEnterCallback(this->pWindow, func);

	return;
}

void cEngine::setMouseScrollCallback(mouse_scroll_callback_function_def func) {
	glfwSetScrollCallback(this->pWindow, func);

	return;
}

void cEngine::setResizeCallback(resize_callback_function_def func)
{
	glfwSetWindowSizeCallback(this->pWindow, func);
}
