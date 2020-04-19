#include "stdafx.h"
#include "ChessApplication.h"
#include "ChessFactory.h"

#include "Views/ChessViews.h"
#include "Controllers/ChessGameController.h"

#include "ChessPieces/ChessPieceRegistry.h"
#include "Services/BoardService.h"
#include "Services/PlayerService.h"

#include "mvc/View.h"
#include "io/InputDevice.h"
#include "io/OutputDevice.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	ChessApplication::ChessApplication(const ChessFactory& chessFactory) : m_chessFactory(chessFactory)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ChessApplication::~ChessApplication() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessApplication::PreInitialize()
	{
		SetInputDevice(m_chessFactory.CreateInputDevice());
		SetOutputDevice(m_chessFactory.CreateOutputDevice());
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessApplication::PopulateControllers()
	{
		AddController(std::make_unique<ChessGameController>());
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