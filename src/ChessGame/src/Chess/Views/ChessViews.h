#pragma once

namespace chess
{
	enum class ViewType
	{
		Chessboard = 0,
		COUNT
	};

	mvc::ViewId ViewTypeToId(ViewType viewType);
}