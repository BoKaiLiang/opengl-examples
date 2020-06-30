#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <string>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Event.h"

class Application {
public:
	using EventCallbackFn = std::function<void(Event&)>;

	virtual ~Application();

	Application(const Application& other) = delete;
	Application& operator=(const Application& other) = delete;

	virtual void Run();

	void ShutDown();

	inline GLFWwindow* GetRawWindow() { return m_Window; }

	virtual void OnEvent(Event& e) = 0;
	virtual void OnUpdate(float dt) = 0;
	virtual void OnRender() = 0;
	virtual void OnImGui() = 0;

	inline static Application& Get() { return *s_Instance; }

protected:
	GLFWwindow* m_Window;

	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;

		EventCallbackFn EventCallback;
	};

	WindowData m_Data;

	static Application* s_Instance;

protected:
	Application(unsigned int width, unsigned int height, const std::string& title);

	void WindowClear(float r = 0.1f, float g = 0.1f, float b = 0.1f, float a = 1.0f);
	void ImGuiClear();

	void WindowUpdate();
	void ImGuiRender();
};

#endif // _APPLICATION_H_