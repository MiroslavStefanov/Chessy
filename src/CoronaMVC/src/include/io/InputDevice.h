#pragma once

namespace mvc
{
	class InputDevice 
	{
	public:
		virtual ~InputDevice() = default;
		virtual void Update() = 0;
	};
}