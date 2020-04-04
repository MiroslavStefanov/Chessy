#pragma once

namespace mvc
{
	template<typename T>
	class BufferQueue : private std::deque<T>
	{
	public:
		bool	IsEmpty() const;
		T		PopNextEntry();
		void	AddEntry(T&& entry);
	};

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline bool BufferQueue<T>::IsEmpty() const
	{
		return this->empty();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline T BufferQueue<T>::PopNextEntry()
	{
		T entry = std::move(this->front());
		this->pop_front();
		return entry;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	inline void BufferQueue<T>::AddEntry(T&& entry)
	{
		this->push_back(std::move(entry));
	}
}