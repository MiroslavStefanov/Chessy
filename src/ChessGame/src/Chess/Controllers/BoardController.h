#pragma once
#include "mvc/BaseController.h"
#include "dependency/Depender.h"

namespace chess
{
	class BoardService;
	class PlayerService;

	class BoardController : public mvc::BaseController, public mvc::Depender<BoardService, PlayerService>
	{
	public:
		BoardController();

	protected:
		void RegisterConsumers() override;
		mvc::ModelAndView OnApplicationStartedEvent(mvc::ApplicationStartedEvent const& event) override;

	private:
		mvc::ModelAndView OnCellClickedEvent(class CellClickedEvent const& event);

		std::unique_ptr<mvc::Model> CreateChessboardViewModel() const;
	};
}