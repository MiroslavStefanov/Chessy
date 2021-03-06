#pragma once

namespace mvc
{
	class OutputDevice 
	{
	public:
		virtual ~OutputDevice() = default;
		virtual void Update() = 0;
		virtual void RenderText(const std::string& text) = 0;
	};
}