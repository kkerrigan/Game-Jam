#ifndef _cEngine_HG_
#define _cEngine_HG_

#include "Global.h"
#include "sWindowSettings.h"
#include "cDllLoader.h"
#include "../physics_interfaces/physics/interfaces/iPhysicsFactory.h"
#include "../physics_interfaces/physics/interfaces/iPhysicsWorld.h"

class cEngine {
public:
	using update_callback_function_def = void(*)(double deltaTime);
	using keyboard_callback_function_def = void(*)(GLFWwindow* window, int key, int scancode, int action, int mods);
	using keyboard_callback_async_function_def = void(*)(GLFWwindow* window, float delta_time);
	using mouse_callback_async_function_def = void(*)(GLFWwindow* window);
	
	using mouse_cursor_pos_function_def = void(*)(GLFWwindow* window, double xpos, double ypos);
	using mouse_button_callback_function_def = void(*)(GLFWwindow* window, int button, int action, int mods);
	using mouse_cursor_enter_function_def = void(*)(GLFWwindow* window, int entered);
	using mouse_scroll_callback_function_def = void(*)(GLFWwindow* window, double offsetX, double offsetY);
	using resize_callback_function_def = void (*)(GLFWwindow* window, int width, int height);

	// Singleton setup
	static cEngine* getInstance();
	cEngine(cEngine const&) = delete;
	void operator=(cEngine const&) = delete;

	// callbacks
	void setUpdateCallback(update_callback_function_def func);
	bool setKeyboardCallback(keyboard_callback_function_def func);
	void setKeyboardCallbackAsync(keyboard_callback_async_function_def func);
	void setMouseCallbackAsync(mouse_callback_async_function_def func);
	void setMouseCursorPositionCallback(mouse_cursor_pos_function_def func);
	void setMouseButtonCallback(mouse_button_callback_function_def func);
	void setMouseEnterCallback(mouse_cursor_enter_function_def func);
	void setMouseScrollCallback(mouse_scroll_callback_function_def func);
	void setResizeCallback(resize_callback_function_def func);

	bool initialize(sWindowSettings* windowSettings);
	void startGameLoop();
	GLFWwindow* getWindow();
	
private:
	cEngine();
	~cEngine();

	// pointer to our render window
	GLFWwindow* pWindow;

	sWindowSettings* settings;

	//physics
	SmartyGL::cDllLoader physics_dll_loader;
	physics::interfaces::iPhysicsFactory* physics_factory;
	physics::interfaces::iPhysicsWorld* physics_world;



	
	keyboard_callback_async_function_def pFuncKeyboardCallbackAsync;
	mouse_callback_async_function_def pFuncMouseCallbackAsync;

	// called internaly to clean up
	void terminate();

public:

	update_callback_function_def pFuncUpdateCallback;
	physics::interfaces::iPhysicsFactory* get_physics_factory();
	physics::interfaces::iPhysicsWorld* get_world();

	sWindowSettings* get_window_settings();
};

#endif