#pragma once
#include "mvc/View.h"
#include "Models/ChessGameViewModel.h"

namespace chess
{
	class ChessboardView : public mvc::View
	{
	public:
		void SetModel(std::unique_ptr<mvc::Model>&& model) override;
		void ProcessInput(mvc::InputDevice* inputDevice) override;
		void ProcessOutput(mvc::OutputDevice* outputDevice) override;

	private:
		void PickChessPiceOnPosition(const TilePosition& position);
		void PlaySelectedPieceOnPosition(const TilePosition& position);
		void PromotePawnToChessPieceType(EChessPieceType chessPieceType);

	private:
		ChessGameViewModel m_model;
	};
}