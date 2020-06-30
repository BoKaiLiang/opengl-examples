#ifndef _INPUT_H_
#define _INPUT_H_

#include <utility>

#include "Application.h"
#include "InputCodes.h"

class Input
{
public:
	static bool IsKeyPressed(KeyCode key);
	static bool IsMouseButtonPressed(MouseCode button);
	static std::pair<float, float> GetMousePosition();
	static float GetMouseX();
	static float GetMouseY();
};

#endif // _INPUT_H_