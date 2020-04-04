#pragma once
#include "io/BaseInputDevice.h"

namespace chess
{
	class ConsoleChessInputDevice : public mvc::BaseInputDevice
	{
	public:
		void CollectInputEvents() override;
	};
}