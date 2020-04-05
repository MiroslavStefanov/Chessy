#include "stdafx.h"
#include "ChessPieceRegistry.h"
#include "Utils/Utils.h"

#include "ChessPieces/Bishop.h"
#include "ChessPieces/King.h"
#include "ChessPieces/Knight.h"
#include "ChessPieces/Pawn.h"
#include "ChessPieces/Queen.h"
#include "ChessPieces/Rook.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	std::size_t ChessPieceRegistry::GetInstancesCount(EChessPieceType type) const
	{
		if (auto piece = GetPieceOfType(type))
			return piece->InstacesCount;

		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	const TilePosition& ChessPieceRegistry::GetInitialPosition(EChessPieceType type, EColor color, std::size_t instanceNumber) const
	{
		if (auto piece = GetPieceOfType(type))
		{
			if (instanceNumber < piece->InstacesCount)
			{
				if (color == EColor::Black)
					return piece->BlackInitialPositions[instanceNumber];
				if (color == EColor::White)
					return piece->WhiteInitialPositions[instanceNumber];
			}
		}
		return TilePosition::Invalid();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	char ChessPieceRegistry::GetVisualRepresentation(EChessPieceType type, EColor color) const
	{
		char visual = (char)228;
		if (auto piece = GetPieceOfType(type))
		{
			char visual = piece->Visualization;
			if (color == EColor::White)
				visual = std::toupper(visual);
		}
		return visual;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	const ChessPiece* ChessPieceRegistry::GetPieceOfType(EChessPieceType type) const
	{
		switch (type)
		{
		case chess::EChessPieceType::Pawn:
			return &pieces::Pawn;
		case chess::EChessPieceType::Knight:
			return &pieces::Knight;
		case chess::EChessPieceType::Bishop:
			return &pieces::Bishop;
		case chess::EChessPieceType::Rook:
			return &pieces::Rook;
		case chess::EChessPieceType::Queen:
			return &pieces::Queen;
		case chess::EChessPieceType::King:
			return &pieces::King;
		default:
			return nullptr;
		}
	}
}