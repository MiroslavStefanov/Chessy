#pragma once

namespace chess
{
	struct TilePositionViewModel;
	class TilePosition;

	class ModelMapper
	{
	public:
		static TilePositionViewModel MapTilePositionView(const TilePosition& tilePostion);
	};
}
