#include "stdafx.h"
#include "Chessy.h"
#include "ChessBoard.h"
#include "ChessPieces/ChessPiece.h"
#include "Utils/Utils.h"

namespace chess
{
	EColor Chessy::m_turnColor = EColor::White;
	static auto OutOfBounds = [](int row, int col) {
		return (row < 0 || row >= 8 ||
			col < 0 || col >= 8);
	};
	//////////////////////////////////////////////////////////////////////////////////////////
	Chessy::Chessy(EChessboardSide whiteSide) : 
		m_checkmate(false),
		m_topSide(ChessySide(EChessboardSide::Top, whiteSide == EChessboardSide::Top ? EColor::White : EColor::Black)),
		m_bottomSide(ChessySide(EChessboardSide::Bottom, whiteSide == EChessboardSide::Bottom ? EColor::White : EColor::Black))
	{
		m_chessBoard = new ChessBoard(whiteSide);
		m_chessBoard->Initialize(m_topSide.Color, m_bottomSide.Color);

		auto tiles = m_chessBoard->GetTiles();
		for (int8_t i = 0; i < sChessBoardSize; ++i)
		{
			auto tile = tiles[i];
			CalculateGameMoves(tile->GetTilePosition());
			if (tile->GetChessPiece())
			{
				if (tile->GetChessPiece()->GetColor() == m_topSide.Color)
				{
					m_topSide.AddChessPiece(tiles[i]->GetChessPiece());
				}
				else
				{
					m_bottomSide.AddChessPiece(tiles[i]->GetChessPiece());
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	ETurnState Chessy::ChessyTurn(const TilePosition& position)
	{
		auto tile = m_chessBoard->GetTileOnPosition(position);
		if (!tile || (!m_chessBoard->GetPickedUp() && tile->GetChessPiece() == nullptr))
		{
			return ETurnState::ErrorState;
		}
		if (!m_chessBoard->GetPickedUp())
		{
			if (tile->GetChessPiece()->GetColor() != m_turnColor)
			{
				return ETurnState::ErrorState;
			}
			else
			{
				m_chessBoard->PickUp(position);
				return ETurnState::Select;
			}
		}
		else
		{
			if (m_chessBoard->GetPickedUp() == tile->GetChessPiece())
			{
				m_chessBoard->Drop();
				return ETurnState::Unselect;
			}
			else if (IsPossibleMove(position))
			{
				m_chessBoard->MovePickedUpTo(position);
				RecalculateGameMoves(position);
			}
			else
			{
				return ETurnState::ErrorState;
			}
		}

		CheckmateCheck();

		if (m_checkmate == true)
		{
			return ETurnState::Checkmate;
		}

		return ETurnState::EndTurn;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool Chessy::IsPossibleMove(const TilePosition& position)
	{
		auto& chessboard = m_chessBoard;
		auto possibleMoves = m_chessBoard->GetPickedUp()->GetPossibleGameMoves();
		auto it = std::find_if(possibleMoves.begin(), possibleMoves.end(), [&position, &chessboard](const TilePosition& pos) {
			return pos == position;
		});
		return it != possibleMoves.end();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessy::RecalculateGameMoves(const TilePosition& position)
	{
		auto tile = m_chessBoard->GetTileOnPosition(position);
		auto tileChessPiece = tile->GetChessPiece();
		auto possibleMoves = tileChessPiece->GetPossibleGameMoves();
		if (!possibleMoves.empty())
		{
			auto tiles = m_chessBoard->GetTiles();
			for (auto position : possibleMoves)
			{
				tiles[position.AsIndex()]->RemoveHitter(tileChessPiece);
			}
		}

		for (auto pos : m_chessBoard->GetPositionsForRelculate())
		{
			RecalculateGameMovesForBlocked(pos);
		}

		CalculateGameMoves(position);

		if (m_activateCheck == false)
		{
			m_check = false;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessy::RecalculateGameMovesForBlocked(const TilePosition& position)
	{
		auto tile = m_chessBoard->GetTileOnPosition(position);
		auto tileHits = tile->GetHits();
		for (auto hit : tileHits)
		{
			CalculateGameMoves(hit.hitter->GetOnwerTilePosition());
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessy::HandleKing(const TilePosition& position, const ChessPieceMove& chessPieceMove)
	{
		if (chessPieceMove.type == EMoveType::Castle)
		{
			CastleCheck();
			return;
		}

		auto chessPiece = m_chessBoard->GetTileOnPosition(position)->GetMutableChessPiece();
		auto modifier = CalculateModifier(chessPieceMove, chessPiece);

		Position pos = position.AsPosition();
		pos += modifier;
		auto forwardTile = m_chessBoard->GetTileOnPosition(TilePosition(pos));
		if(forwardTile == nullptr)
		{
			return;
		}
		auto& forwardTileHits = forwardTile->GetHits();

		m_chessBoard->AddHit(pos, chessPieceMove.type, chessPiece);

		for (auto& hit : forwardTileHits)
		{
			if (hit.hitter->GetColor() != chessPiece->GetColor())
			{
				return;
			}

			if (m_check)
			{
				auto behindPosition = position.AsPosition();
				behindPosition += CalculateModifier(ChessPieceMove(GetOpositeDirection(chessPieceMove.direction), chessPieceMove.type), chessPiece);

				auto behindTile = m_chessBoard->GetTileOnPosition(TilePosition(behindPosition));
				if (behindTile == nullptr)
				{
					continue;
				}

				auto& behindTileHits = behindTile->GetHits();
				for (auto& backwardHit : behindTileHits)
				{
					if (backwardHit.hitter == hit.hitter)
					{
						return;
					}
				}
			}
		}

		m_possibleGameMoves.push_back(pos);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessy::HandlePawn(const TilePosition& position, const ChessPieceMove& chessPieceMove)
	{
		auto chessPiece = m_chessBoard->GetTileOnPosition(position)->GetMutableChessPiece();
		auto modifier = CalculateModifier(chessPieceMove, chessPiece);

		Position pos = position.AsPosition();
		pos += modifier;
		auto forwardTile = m_chessBoard->GetTileOnPosition(TilePosition(pos));

		m_chessBoard->AddHit(pos, chessPieceMove.type, chessPiece);
		
		switch (chessPieceMove.type)
		{
		case chess::EMoveType::PawnMove:
			{
				if (forwardTile && !forwardTile->GetChessPiece())
				{
					m_possibleGameMoves.push_back(pos);
				}
				break;
			}
		case chess::EMoveType::PawnJump:
			{
				auto& jumpPos = pos + modifier;
				auto jumpTile = m_chessBoard->GetTileOnPosition(jumpPos);
				if ((jumpTile && jumpTile->GetChessPiece()) || chessPiece->HasMoved())
				{
					break;
				}
				m_possibleGameMoves.push_back(jumpPos);
				break;
			}
		case chess::EMoveType::PawnHit:
			if ((forwardTile && forwardTile->GetChessPiece() && forwardTile->GetChessPiece()->GetColor() != chessPiece->GetColor()) ||
				pos == m_chessBoard->GetPreviousJump().sneakyTakePosition && m_chessBoard->GetPreviousJump().piece->GetColor() != chessPiece->GetColor())
			{
				m_possibleGameMoves.push_back(pos);
			}
			break;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessy::HandleChessPiece(const TilePosition& position, const ChessPieceMove& move)
	{
		auto chessPiece = m_chessBoard->GetTileOnPosition(position)->GetMutableChessPiece();
		auto modifier = CalculateModifier(move, chessPiece);
		bool blocked = false;
		Position pos = position.AsPosition();
		do
		{
			pos += modifier;
			auto tempTile = m_chessBoard->GetTileOnPosition(pos);
			if (tempTile)
			{
				auto tempChessPiece = tempTile->GetChessPiece();
				if (tempChessPiece == nullptr)
				{
					m_possibleGameMoves.push_back(pos);
					m_chessBoard->AddHit(pos, move.type, chessPiece);
				}
				else if (tempChessPiece->GetColor() != chessPiece->GetColor())
				{
					m_possibleGameMoves.push_back(pos);
					if (tempChessPiece->GetType() == EChessPieceType::KingType)
					{
						m_activateCheck = true;
					}
					m_chessBoard->AddHit(pos, move.type, chessPiece);
					blocked = true;
				}
				else if (tempChessPiece->GetColor() == chessPiece->GetColor())
				{
					m_chessBoard->AddHit(pos, move.type, chessPiece);
					blocked = true;
				}
				else
				{
					blocked = true;
				}
			}
			else
			{
				blocked = true;
			}
		} while (!blocked && move.type == EMoveType::Multiple);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessy::CalculateGameMoves(const TilePosition& position)
	{
		auto tile = m_chessBoard->GetTileOnPosition(position);
		if (!tile || tile->GetChessPiece() == nullptr)
		{
			return;
		}

		m_possibleGameMoves.clear();
		auto tileChessPiece = tile->GetMutableChessPiece();
		auto chessPieceMoves = tileChessPiece->GetChessPieceMoves();
		for (auto chessPieceMove : chessPieceMoves)
		{
			if (tileChessPiece->GetType() == EChessPieceType::KingType)
			{
				HandleKing(position, chessPieceMove);
				continue;
			}

			if (tileChessPiece->GetType() == EChessPieceType::PawnType)
			{
				HandlePawn(position, chessPieceMove);
				continue;
			}
			HandleChessPiece(position, chessPieceMove);
		}

		tileChessPiece->SetPossibleGameMoves(m_possibleGameMoves);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessy::Draw()
	{
		m_chessBoard->Draw();
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessy::OnEndTurn()
	{
		m_turnColor = m_turnColor == EColor::Black ? EColor::White : EColor::Black;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessy::CastleCheck()
	{
		auto tiles = m_chessBoard->GetTiles();
		auto side = m_topSide.Color == m_turnColor ? m_topSide : m_bottomSide;
		auto castlePositions = side.CastlePositions;

		std::vector<TilePosition>& possibleMoves = m_possibleGameMoves;
		ChessboardTile* kingTile = tiles[castlePositions[0].AsIndex()];
		auto CheckPossibleCastle = [&kingTile, &tiles, &possibleMoves](ChessboardTile* rookTile) {
			bool isClearToCastle = true;
			if (rookTile->GetChessPiece() && kingTile->GetChessPiece()
				&& rookTile->GetChessPiece()->GetType() == EChessPieceType::RookType
				&& rookTile->GetChessPiece()->HasMoved() == false
				&& kingTile->GetChessPiece()->GetType() == EChessPieceType::KingType
				&& kingTile->GetChessPiece()->HasMoved() == false)
			{
				auto tilesToCheckSize = kingTile->GetTilePosition().AsPosition().col;
				for (auto i = 1; i < tilesToCheckSize; ++i)
				{
					if (tiles[kingTile->GetTilePosition().AsPosition().row * sChessBoardSide + i]->GetChessPiece() != nullptr || !tiles[i]->GetHits().empty())
					{
						isClearToCastle = false;
						break;
					}
				}

				if (isClearToCastle)
				{
					possibleMoves.push_back(rookTile->GetTilePosition());
				}
			}
		};

		CheckPossibleCastle(tiles[castlePositions[1].AsIndex()]);
		CheckPossibleCastle(tiles[castlePositions[2].AsIndex()]);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void Chessy::CheckmateCheck()
	{
		if (m_activateCheck == false || m_check == true)
		{
			return;
		}

		m_check = true;

		auto turnSide = m_turnColor == m_topSide.Color ? m_topSide : m_bottomSide;
		for (auto piece : turnSide.ChessPieces)
		{
			RecalculateGameMoves(piece->GetOnwerTilePosition());
		}

		for (auto piece : turnSide.ChessPieces)
		{
			if (piece->GetPossibleGameMoves().empty() == false)
			{
				m_checkmate = true;
			}
		}

		m_activateCheck = false;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	// X - Move, Y - Hit
	// Knight			   Normal
	//	 YXY			
	//	Y X Y				YYY
	//	XXOXX				YOY
	//	Y X Y				YYY
	//	 YXY			
	//////////////////////////////////////////////////////////////////////////////////////////
	Position Chessy::CalculateModifier(const ChessPieceMove& move, const ChessPiece* piece)
	{
		Position modifier;
		switch (move.direction)
		{
		case EDirection::ForwardLeft:
			modifier = move.type == EMoveType::KnightMove ? Position(2, -1) : Position(1, -1);
			break;
		case EDirection::Forward:
			modifier = move.type == EMoveType::KnightMove ? Position(2, 1) : Position(1, 0);
			break;
		case EDirection::ForwardRight:
			modifier = move.type == EMoveType::KnightMove ? Position(1, 2) : Position(1, 1);
			break;
		case EDirection::Right:
			modifier = move.type == EMoveType::KnightMove ? Position(-1, 2) : Position(0, 1);
			break;
		case EDirection::BackwardRight:
			modifier = move.type == EMoveType::KnightMove ? Position(-2, 1) : Position(-1, 1);
			break;
		case EDirection::Backward:
			modifier = move.type == EMoveType::KnightMove ? Position(-2, -1) : Position(-1, 0);
			break;
		case EDirection::BackwardLeft:
			modifier = move.type == EMoveType::KnightMove ? Position(-1, -2) : Position(-1, -1);
			break;
		case EDirection::Left:
			modifier = move.type == EMoveType::KnightMove ? Position(1, -2) : Position(0, -1);
			break;
		}

		int sideModifier = m_bottomSide.Color == piece->GetColor() ? -1 : 1;
		modifier.row *= sideModifier;
		modifier.col *= sideModifier;

		return modifier;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#include <chrono>
//#include <thread>

//int main()
//{
//	auto chessy = new chess::Chessy(chess::EChessboardSide::Bottom);
//	
//	auto ToStr = [](chess::ETurnState state) {
//		switch (state)
//		{
//		case chess::ETurnState::EndTurn:
//			return "EndTurn";
//		case chess::ETurnState::Select:
//			return "Select";
//		case chess::ETurnState::Unselect:
//			return "Unselect";
//		case chess::ETurnState::ErrorState:
//			return "ErrorState";
//		default:
//			return "";
//		}
//	};
//
//	bool stop = false;
//	chess::ETurnState state = chess::ETurnState::StartGame;
//	while (!stop)
//	{
//		chessy->Draw();
//		std::cout << ToStr(state) << std::endl;
//
//		int row, col;
//		std::cin >> row >> col;
//
//		state = chessy->ChessyTurn(chess::TilePosition(chess::Position(row, col)));
//		if (state == chess::ETurnState::EndTurn)
//		{
//			chessy->OnEndTurn();
//		}
//
//		system("CLS");
//
//	}
//
//	return 0;
//}