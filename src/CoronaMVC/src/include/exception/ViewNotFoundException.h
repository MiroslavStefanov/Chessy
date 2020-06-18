#pragma once
#include "LoggedException.h"

namespace mvc
{
	class ViewNotFoundException : public LoggedException
	{
	public:
		explicit ViewNotFoundException(ViewId viewId) noexcept;
	};
}