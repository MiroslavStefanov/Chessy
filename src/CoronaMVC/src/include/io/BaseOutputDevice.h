#pragma once

namespace mvc
{
	class BaseOutputDevice 
	{
	public:
		virtual ~BaseOutputDevice() = default;

		virtual void Clear() = 0;
		virtual void RenderText(const std::string& text) = 0;
	};
}