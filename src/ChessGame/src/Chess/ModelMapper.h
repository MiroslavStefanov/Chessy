#pragma once

namespace chess
{
	struct TilePositionViewModel;
	struct ChessTileViewModel;
	class TilePosition;

	class ModelMapper
	{
	public:
		static TilePositionViewModel MapTilePositionView(const TilePosition& tilePostion);
		static std::vector<ChessTileViewModel> ProduceChessBoardView(const std::vector<ChessPieceId>& chessBoardState, ChessPieceId pickedPieceId);
	};
}
