#pragma once
#include "exception/LoggedException.h"

namespace chess
{
	class InvalidChessPieceException : public mvc::LoggedException
	{
	public:
		explicit InvalidChessPieceException(ChessPieceId chessPieceId) noexcept;
		InvalidChessPieceException(ChessPieceId chessPieceId, const std::exception& cause) noexcept;

	private:
		std::string GetMessage(ChessPieceId chessPieceId) const noexcept;
	};
}