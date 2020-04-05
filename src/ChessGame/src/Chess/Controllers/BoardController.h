#pragma once
#include "mvc/BaseController.h"

namespace chess
{
	class BoardController : public mvc::BaseController
	{
	public:
		BoardController(class BoardService& boardService, class PlayerService& playerService);

	protected:
		void RegisterConsumers() override;
		mvc::ModelAndView OnApplicationStartedEvent(mvc::ApplicationStartedEvent const& event) override;

	private:
		std::unique_ptr<mvc::Model> CreateChessboardViewModel() const;

	private:
		BoardService& m_boardService;
		PlayerService& m_playerService;
	};
}