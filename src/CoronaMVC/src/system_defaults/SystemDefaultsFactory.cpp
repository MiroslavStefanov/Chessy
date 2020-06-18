#include "pch.h"
#include "SystemDefaultsFactory.h"

#include "logger/FileLoggerImpl.h"

#include "system_defaults/error_view/ErrorView.h"

namespace mvc
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	std::unique_ptr<LoggerImpl> SystemDefaultsFactory::MakeDefaultLogger()
	{
		return std::make_unique<FileLoggerImpl>(GetDefaultLogFileName());
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ControllerExceptionHandler SystemDefaultsFactory::MakeDefaultControllerExceptionHandler()
	{
		return [](const std::exception& e) -> ModelAndView
		{
			return ModelAndView::CreateFromViewId(DefaultErrorView::DEFAULT_ERROR_VIEW_ID);
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::string SystemDefaultsFactory::GetDefaultLogFileName()
	{
		static constexpr auto DEFAULT_LOG_FILE_NAME_PREFIX = "Log_";

		const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		return std::string(DEFAULT_LOG_FILE_NAME_PREFIX) + std::to_string(now);
	}

}