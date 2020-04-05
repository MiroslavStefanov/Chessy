#include "stdafx.h"
#include "ChessViews.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	mvc::ViewId ViewTypeToId(ViewType viewType)
	{
		return mvc::ViewId((int)viewType);
	}

}