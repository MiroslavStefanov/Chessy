#pragma once
#include "mvc/BaseController.h"
#include "dependency/Depender.h"

namespace chess
{
	enum class ETurnState : int;
	class PlayerService;
	class BoardService;

	class PlayerController : public mvc::BaseController, public mvc::Depender<PlayerService, BoardService>
	{
	public:
		PlayerController();

	protected:
		void RegisterConsumers() override;
		mvc::ModelAndView OnApplicationStartedEvent(mvc::ApplicationStartedEvent const& event) override;

	private:
		mvc::ModelAndView OnCellClickedEvent(class CellClickedEvent const& event);

		std::unique_ptr<mvc::Model> CreatePlayerTurnViewModel() const;
	};
}