#include "stdafx.h"
#include "InvalidChessPieceException.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	InvalidChessPieceException::InvalidChessPieceException(ChessPieceId chessPieceId) noexcept
		: LoggedException(GetMessage(chessPieceId))
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	InvalidChessPieceException::InvalidChessPieceException(ChessPieceId chessPieceId, const std::exception& cause) noexcept
		: LoggedException(GetMessage(chessPieceId), cause)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	std::string InvalidChessPieceException::GetMessage(ChessPieceId chessPieceId) const noexcept
	{
		return "Invalid chess piece color="
			+ std::to_string((int)chessPieceId.GetColor())
			+ ", type="
			+ std::to_string((int)chessPieceId.GetType())
			+ ", instance="
			+ std::to_string((int)chessPieceId.GetInstanceNumber());
	}

}