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
		mvc::DependencyWrapper<class BoardService> m_boardService;
		mvc::DependencyWrapper<class PlayerService> m_playerService;
	};
}