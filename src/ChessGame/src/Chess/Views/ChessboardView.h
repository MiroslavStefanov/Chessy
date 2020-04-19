#pragma once
#include "mvc/View.h"
#include "Models/ChessGameViewModel.h"

namespace chess
{
	class ChessboardView : public mvc::View
	{
	public:
		void SetModel(StringId modelId, std::unique_ptr<mvc::Model>&& model) override;
		void ProcessInput(mvc::InputDevice* inputDevice) override;
		void ProcessOutput(mvc::OutputDevice* outputDevice) override;

	private:
		ChessGameViewModel m_model;
	};
}