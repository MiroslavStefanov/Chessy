#pragma once

namespace chess
{
	enum class ViewType
	{
		Chessboard = 0,
		GameOver = 1,
		COUNT
	};

	mvc::ViewId ViewTypeToId(ViewType viewType);
}