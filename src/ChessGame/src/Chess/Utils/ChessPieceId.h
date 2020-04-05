#pragma once

namespace chess
{
	enum class EChessPieceType;
	enum class EColor;

	class ChessPieceId
	{
	public:
		static const ChessPieceId& Invalid();

		ChessPieceId();
		ChessPieceId(EChessPieceType type, EColor color, std::size_t instanceNumber);

		bool			IsValid() const;
		EChessPieceType GetType() const;
		EColor			GetColor() const;
		std::size_t		GetInstanceNumber() const;

		bool operator== (const ChessPieceId& other) const;
		bool operator!= (const ChessPieceId& other) const;
		ChessPieceId& operator= (const ChessPieceId& other);

	private:
		EChessPieceType m_type;
		EColor m_color;
		std::size_t m_instanceNumber;
	};

	struct ChessPieceIdHash
	{
		std::size_t operator()(const ChessPieceId& id) const;
	};
}

