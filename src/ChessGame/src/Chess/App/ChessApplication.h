#pragma once
#include "BaseApplication.h"


namespace chess
{
	class ChessApplication : public mvc::BaseApplication
	{
	public:
		ChessApplication(class ChessFactory const& chessFactory);
		~ChessApplication();

	protected:
		void PreInitialize() override;
		void PopulateControllers() override;
		void PopulateViews() override;

	private:
		const ChessFactory& m_chessFactory;
		std::unique_ptr<class ChessPieceRegistry> m_pieceRepository;
		std::unique_ptr<class BoardService> m_boardService;
		std::unique_ptr<class PlayerService> m_playerService;
	};
}