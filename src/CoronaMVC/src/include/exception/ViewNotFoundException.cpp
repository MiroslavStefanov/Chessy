#include "pch.h"
#include "ViewNotFoundException.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	ViewNotFoundException::ViewNotFoundException(ViewId viewId) noexcept
		: LoggedException("Could not find view with id: " + std::to_string((int)viewId))
	{
	}

}