#pragma once
#include "mvc/ModelAndView.h"

namespace mvc
{
	class SystemDefaultsFactory
	{
	public:
		static std::unique_ptr<LoggerImpl> MakeDefaultLogger();
		static ControllerExceptionHandler MakeDefaultControllerExceptionHandler();

	private:
		static std::string GetDefaultLogFileName();

	};
}