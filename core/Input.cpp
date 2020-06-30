#include "Input.h"

bool Input::IsKeyPressed(KeyCode key)
{
	// auto window = static_cast<GLFWwindow*>(Application::Get().GetRawWindow());
	auto state = glfwGetKey(Application::Get().GetRawWindow(), static_cast<int32_t>(key));
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(MouseCode button)
{
	// auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow());
	auto state = glfwGetMouseButton(Application::Get().GetRawWindow(), static_cast<int32_t>(button));
	return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition()
{
	// auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow());
	double xpos, ypos;
	glfwGetCursorPos(Application::Get().GetRawWindow(), &xpos, &ypos);

	return { (float)xpos, (float)ypos };
}

float Input::GetMouseX()
{
	auto [x, y] = GetMousePosition();
	return x;
}

float Input::GetMouseY()
{
	auto [x, y] = GetMousePosition();
	return y;
}