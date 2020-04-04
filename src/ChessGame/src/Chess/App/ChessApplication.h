#pragma once
#include "BaseApplication.h"

namespace chess
{
	class ChessFactory;

	class ChessApplication : public mvc::BaseApplication
	{
	public:
		ChessApplication(const ChessFactory& chessFactory);

	protected:
		void PopulateControllers() override;
		void PopulateViews() override;

	private:
		const ChessFactory& m_chessFactory;
	};
}