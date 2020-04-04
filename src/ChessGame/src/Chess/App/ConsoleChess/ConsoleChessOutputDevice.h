#pragma once
#include "io/BaseOutputDevice.h"

namespace chess
{
	class ConsoleChessOutputDevice : public mvc::BaseOutputDevice
	{
	public:
		void Clear() override;
		void RenderText(const std::string& text) override;
	};
}