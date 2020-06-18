#pragma once

namespace chess
{
	enum class ErrorCode
	{
		InvalidTile,
		CannotPickChessPiece,
		CannotDropChessPiece,
		InvalidPickedChessPiece,
		InvalidChessPieceMove,
		CannotPromotePawn,
		ChessPieceNotOnBoard,
		InternalError
	};
	
	using ErrorCodes = std::vector<ErrorCode>;
}