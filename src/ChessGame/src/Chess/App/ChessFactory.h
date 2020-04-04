#pragma once

namespace chess
{
	enum class ViewType;

	class ChessFactory
	{
	public:
		virtual ~ChessFactory() = default;
		virtual std::unique_ptr<mvc::BaseInputDevice> CreateInputDevice() const = 0;
		virtual std::unique_ptr<mvc::BaseOutputDevice> CreateOutputDevice() const = 0;
		virtual std::unique_ptr<mvc::View> CreateView(ViewType viewType) const = 0;
	};
}