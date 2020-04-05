#pragma once


namespace chess
{
	struct ChessPiece;

	enum class EDirection
	{
		ForwardLeft,
		Forward,
		ForwardRight,
		Right,
		BackwardRight,
		Backward,
		BackwardLeft,
		Left,
		NoDirection
	};

	enum class EMoveType
	{
		PawnMove,
		PawnJump,
		PawnHit,
		KnightMove,
		KingMove,
		Castle,
		Multiple,
		Error
	};

	enum class EColor
	{
		White,
		Black,
		Colorless
	};

	enum class EChessPieceType
	{
		None = -1,
		Pawn = 0,
		Knight,
		Bishop,
		Rook,
		Queen,
		King,
		COUNT
	};

	enum class EChessboardSide
	{
		Top,
		Bottom
	};

	enum class ETurnState
	{
		StartGame,
		EndTurn,
		Select,
		Unselect,
		ErrorState,
		Checkmate
	};

	const static std::int8_t sChessBoardSide = 8;
	const static std::int8_t sChessBoardSize = sChessBoardSide * sChessBoardSide;

	struct ChessPieceMove
	{
		EDirection direction = EDirection::NoDirection;
		EMoveType type = EMoveType::Error;

		ChessPieceMove(EDirection _direction, EMoveType _type) : direction(_direction), type(_type) { }
	};

	struct TileHit
	{
		EMoveType type;
		const ChessPiece* hitter;

		TileHit(EMoveType _type = EMoveType::Error, const ChessPiece* _hitter = nullptr) : type(_type), hitter(_hitter) { }
	};

	struct Position
	{
		std::int8_t row;
		std::int8_t col;

		bool operator==(const Position& pos) const { return pos.row == row && pos.col == col; }

		const Position operator+(const Position& other)	{ return Position(row + other.row, col + other.col); }
		void operator+=(const Position& other) { *this = *this + other; }

		Position(int8_t index = 0) : row(index / sChessBoardSide), col(index % sChessBoardSide) { }
		Position(int8_t _row, int8_t _col) : row(_row), col(_col) { }
	};

	struct Jump
	{
		Position sneakyTakePosition;
		const ChessPiece* piece;

		Jump(const Position& position = Position(0), const ChessPiece* chessPiece = nullptr) : sneakyTakePosition(position), piece(chessPiece) {}
	};

	class TilePosition
	{
	public:
		static const TilePosition& Invalid() { static const TilePosition invalid(Position(-1, -1)); return invalid; }

		TilePosition(const Position& poisition) : position(poisition) { }

		bool IsValid() const { return AsIndex() >= 0 && AsIndex() < CHESS_BOARD_SIDE * CHESS_BOARD_SIDE; }


		std::int8_t AsIndex() const { return position.row * sChessBoardSide + position.col; }
		const Position& AsPosition() const { return position; }
		Position& AsMutablePosition() { return position; }

		bool operator==(const TilePosition& pos) const { return position == pos.position; }

	private:
		Position position;
	};

	struct TilePositionHash
	{
		std::size_t operator()(const TilePosition& position) const { return position.AsIndex(); }
	};


	EDirection GetOpositeDirection(EDirection direction);
	int8_t GetKingPosition(EColor color);
	EColor GetAlternateColor(EColor color);
}