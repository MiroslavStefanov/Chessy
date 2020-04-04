#include "stdafx.h"
#include "ChessApplication.h"
#include "ChessFactory.h"
#include "Views/ChessViews.h"
#include "Controllers/PlayerController.h"
#include "mvc/View.h"
#include "io/BaseInputDevice.h"
#include "io/BaseOutputDevice.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	ChessApplication::ChessApplication(const ChessFactory& chessFactory) 
		: BaseApplication(chessFactory.CreateInputDevice(), chessFactory.CreateOutputDevice())
		, m_chessFactory(chessFactory)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessApplication::PopulateControllers()
	{
		AddController(std::make_unique<PlayerController>());
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessApplication::PopulateViews()
	{
		for (int viewType = 0; viewType < (int)ViewType::COUNT; ++viewType)
		{
			AddView(mvc::ViewId(viewType), m_chessFactory.CreateView((ViewType)viewType));
		}
	}

}