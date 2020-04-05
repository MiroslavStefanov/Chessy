#include "stdafx.h"
#include "ChessTileViewModel.h"
#include "event/Event.h"

namespace chess
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	ChessTileViewModel::ChessTileViewModel()
		: Color(EColor::Colorless)
		, Piece(ChessPieceId::Invalid())
		, IsPicked(false)
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ChessTileViewModel::ChessTileViewModel(ChessTileViewModel&& other) noexcept
		: Color(other.Color)
		, Piece(other.Piece)
		, IsPicked(other.IsPicked)
		, OnClickEvents(std::move(other.OnClickEvents))
	{
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	ChessTileViewModel::~ChessTileViewModel() = default;
}

