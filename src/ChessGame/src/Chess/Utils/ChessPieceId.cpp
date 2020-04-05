#include "stdafx.h"
#include "ChessPieceId.h"

#include "Utils/Utils.h"
#include <numeric>

namespace chess
{
	//////////////////////////////////////////////////////////////////////////////////////////
	const ChessPieceId& ChessPieceId::Invalid()
	{
		static const ChessPieceId invalid(EChessPieceType::None, EColor::Colorless, std::numeric_limits<std::size_t>::max());
		return invalid;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	ChessPieceId::ChessPieceId() : ChessPieceId(Invalid())
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	ChessPieceId::ChessPieceId(EChessPieceType type, EColor color, std::size_t instanceNumber) : m_type(type), m_color(color), m_instanceNumber(instanceNumber)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool ChessPieceId::IsValid() const
	{
		return m_type != EChessPieceType::None && m_type != EChessPieceType::COUNT && m_color != EColor::Colorless;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	EChessPieceType ChessPieceId::GetType() const
	{
		return m_type;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	EColor ChessPieceId::GetColor() const
	{
		return m_color;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	std::size_t ChessPieceId::GetInstanceNumber() const
	{
		return m_instanceNumber;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool ChessPieceId::operator==(const ChessPieceId& other) const
	{
		return m_type == other.m_type && m_color == other.m_color && m_instanceNumber == other.m_instanceNumber;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool ChessPieceId::operator!=(const ChessPieceId& other) const
	{
		return !(*this == other);
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	ChessPieceId& ChessPieceId::operator=(const ChessPieceId& other)
	{
		m_type = other.m_type;
		m_color = other.m_color;
		m_instanceNumber = other.m_instanceNumber;
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	std::size_t ChessPieceIdHash::operator()(const ChessPieceId& id) const
	{
		assert(id.IsValid());
		if (!id.IsValid())
		{
			return std::numeric_limits<std::size_t>::max();
		}

		std::size_t hash = (std::size_t)id.GetType();
		std::size_t remainingShifts = (std::size_t)EChessPieceType::COUNT;
		while (remainingShifts > 0)
		{
			hash = hash << 1;
			remainingShifts = remainingShifts >> 1;
		}

		hash |= (std::size_t)id.GetColor();
		hash << 1;
		hash |= id.GetInstanceNumber();
	}

}