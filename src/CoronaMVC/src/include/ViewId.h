#pragma once
#include <cstddef>

namespace mvc
{
	struct ViewId
	{
		static ViewId Invalid();
		explicit ViewId(int id);

		bool IsValid() const;
		bool operator==(const ViewId& other) const;
		bool operator!=(const ViewId& other) const;

		ViewId& operator= (const ViewId& other);

		operator int() const;
	private:
		int value;
	};

	struct ViewIdHash
	{
		std::size_t operator()(const ViewId& id) const { return id; }
	};
}
