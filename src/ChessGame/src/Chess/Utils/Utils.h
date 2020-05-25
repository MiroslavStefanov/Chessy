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
		COUNT
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

	enum class ETurnState
	{
		StartGame,
		EndTurn,
		Select,
		Unselect,
		ErrorState,
		Checkmate,
		PawnPromotion
	};

	class ChessPieceMove
	{
	public:
		static const ChessPieceMove& Invalid();

		ChessPieceMove(EDirection direction, EMoveType type);
		bool IsValid() const;

		EDirection GetDirection() const;
		EMoveType GetMoveType() const;

	private:
		EDirection m_direction = EDirection::COUNT;
		EMoveType m_type = EMoveType::Error;
	};

	struct Position
	{
		std::int8_t row;
		std::int8_t col;

		bool operator==(const Position& pos) const { return pos.row == row && pos.col == col; }
		bool operator!=(const Position& pos) const { return !((*this) == pos); }

		Position operator+(const Position& other) const	{ return Position(row + other.row, col + other.col); }
		Position operator-(const Position& other) const { return *this + (other * -1); }
		Position operator*(int multiplier) const { Position ret(*this); ret *= multiplier; return ret; }
		void operator+=(const Position& other) { *this = *this + other; }
		void operator*=(int multiplier) { row *= multiplier; col *= multiplier; }

		Position(int8_t index = 0) : row(index / CHESS_BOARD_SIDE), col(index % CHESS_BOARD_SIDE) { }
		Position(int8_t _row, int8_t _col) : row(_row), col(_col) { }
	};

	class TilePosition
	{
	public:
		static const TilePosition& Invalid() { static const TilePosition invalid(Position(-1, -1)); return invalid; }

		TilePosition(const Position& poisition) : position(poisition) { }

		bool IsValid() const;


		std::int8_t AsIndex() const { return position.row * CHESS_BOARD_SIDE + position.col; }
		const Position& AsPosition() const { return position; }

		bool operator==(const TilePosition& pos) const { return position == pos.position; }
		bool operator!=(const TilePosition& pos) const { return !((*this) == pos); }

	private:
		Position position;
	};

	struct TilePositionHash
	{
		std::size_t operator()(const TilePosition& tilePosition) const 
		{ 
			if (!tilePosition.IsValid())
			{
				return std::numeric_limits<std::size_t>::max();
			}

			return tilePosition.AsIndex();
		}
	};
	using TilePositionSet = std::unordered_set<TilePosition, TilePositionHash>;


	EDirection GetOpositeDirection(EDirection direction);
	int8_t GetKingPosition(EColor color);
	EColor GetAlternateColor(EColor color);
	bool IsPawnPromotionPosition(EColor pawnColor, const TilePosition& position);
	bool IsValidCastleDirection(EDirection direction);

	template<class Key, class Value, class KeyEqual = std::equal_to<Key>> 
	class VectorMap
	{
	public:
		using value_type = std::pair<const Key, Value>;
	public:
		VectorMap() = default;
		VectorMap(std::initializer_list<value_type> initializerList) : m_entries(initializerList) {}

		const Value& operator[](const Key& key) const
		{
			return Find(key)->second;
			return const_cast<VectorMap*>(this)->Find(key)->second;
		}

		Value& operator[](const Key& key)
		{
			auto constIterator = Find(key);
			return const_cast<Value&>(constIterator->second);
		}

		bool HasKey(const Key& key) const
		{
			return Find(key) != m_entries.cend();
		}

	private:
		typename std::vector<value_type>::const_iterator Find(const Key& key) const
		{
			return std::find_if(m_entries.cbegin(), m_entries.cend(), [&key](const value_type& val) {return KeyEqual{}(key, val.first); });
		}

	private:
		std::vector<value_type> m_entries;
	};
}