#pragma once
#include "mvc/View.h"
#include "Models/GameOverViewModel.h"

namespace chess
{
	class GameOverView : public mvc::View
	{
	public:
		void SetModel(StringId modelId, std::unique_ptr<mvc::Model>&& model) override;
		void ProcessInput(mvc::InputDevice* inputDevice) override;
		void ProcessOutput(mvc::OutputDevice* outputDevice) override;

	private:
		GameOverViewModel m_model;
	};
}