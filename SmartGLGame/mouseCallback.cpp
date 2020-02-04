#include <Global.h>
#include <cSceneManager.h>

void cursor_enter_callback(GLFWwindow* window, int entered)
{
	cScene* activeScene = cSceneManager::getInstance()->getActiveScene();

	if (entered)
	{
		activeScene->isCursorInWindow = true;
	}
	else
	{
		activeScene->isCursorInWindow = false;
	}
	return;
}//cursor_enter_callback(...


// Mouse (cursor) callback
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

	return;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	return;
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cScene* activeScene = cSceneManager::getInstance()->getActiveScene();

	// A regular mouse wheel returns the y value
	activeScene->flyCamera.setMouseWheelDelta(yoffset);

	return;
}

void mouseCallbackAsync(GLFWwindow* window)
{
	cScene* activeScene = cSceneManager::getInstance()->getActiveScene();

	double x, y;
	glfwGetCursorPos(window, &x, &y);
	activeScene->flyCamera.setMouseXY(x, y);
	const float MOUSE_SENSITIVITY = 0.1f;
	//	std::cout << ::g_pFlyCamera->getMouseX() << ", " << ::g_pFlyCamera->getMouseY() << std::endl;
	   // Mouse left (primary?) button pressed? 
	   // AND the mouse is inside the window...
	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		&& activeScene->isCursorInWindow)
	{
		// Mouse button is down so turn the camera
		activeScene->flyCamera.Yaw_LeftRight(activeScene->flyCamera.getDeltaMouseX() * -MOUSE_SENSITIVITY);
		activeScene->flyCamera.Pitch_UpDown(activeScene->flyCamera.getDeltaMouseY() * MOUSE_SENSITIVITY);
	}
	// Adjust the mouse speed
	if (activeScene->isCursorInWindow)
	{
		const float MOUSE_WHEEL_SENSITIVITY = 0.1f;
		// Adjust the movement speed based on the wheel position
		activeScene->flyCamera.movementSpeed = activeScene->flyCamera.getMouseWheel() * MOUSE_WHEEL_SENSITIVITY;

		if (activeScene->flyCamera.movementSpeed <= 0.0f) {
			activeScene->flyCamera.movementSpeed = 0.0f;
		}
	}
	return;
}//ProcessAsyncMouse(...