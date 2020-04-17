#pragma once
#include "Models/ChessboardViewModel.h"
#include "Models/PlayerTurnViewModel.h"
#include "mvc/View.h"

namespace chess
{
	class ChessboardView : public mvc::View
	{
	public:
		void SetModel(StringId modelId, std::unique_ptr<mvc::Model>&& model) override;
		void ProcessInput(mvc::InputDevice* inputDevice) override;
		void ProcessOutput(mvc::OutputDevice* outputDevice) override;

	private:
		void UpdateBoardModel(ChessboardViewModel* viewModel);
		void UpdateTurnModel(PlayerTurnViewModel* viewModel);

	private:
		ChessboardViewModel m_boardModel;
		PlayerTurnViewModel m_turnModel;
	};
}