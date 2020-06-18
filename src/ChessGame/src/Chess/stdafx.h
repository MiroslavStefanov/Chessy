#pragma once

#include "pch.h"
#include "Utils/ChessPieceId.h"
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <unordered_set>
#include <algorithm>
#include <cassert>


namespace chess
{
	enum class ErrorCode;
	static constexpr auto CHESS_BOARD_SIDE = 8;
}