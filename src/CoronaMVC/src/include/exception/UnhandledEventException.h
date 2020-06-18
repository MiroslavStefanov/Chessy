#pragma once
#include "LoggedException.h"

namespace mvc
{
	class UnhandledEventExcpetion : public LoggedException
	{
	public:
		explicit UnhandledEventExcpetion(const Event& event) noexcept;
	};
}