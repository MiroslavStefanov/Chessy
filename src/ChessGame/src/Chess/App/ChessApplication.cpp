#include "stdafx.h"
#include "ChessApplication.h"
#include "ChessFactory.h"

#include "Views/ChessViews.h"
#include "Controllers/PlayerController.h"
#include "Controllers/BoardController.h"

#include "ChessPieces/ChessPieceRegistry.h"
#include "Services/BoardService.h"
#include "Services/PlayerService.h"

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
	ChessApplication::~ChessApplication() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessApplication::PreInitialize()
	{
		m_pieceRepository = std::make_unique<ChessPieceRegistry>();
		m_boardService = std::make_unique<BoardService>(*m_pieceRepository);
		m_playerService = std::make_unique<PlayerService>(*m_pieceRepository);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	void ChessApplication::PopulateControllers()
	{
		AddController(std::make_unique<BoardController>(*m_boardService, *m_playerService));
		AddController(std::make_unique<PlayerController>(*m_playerService));
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