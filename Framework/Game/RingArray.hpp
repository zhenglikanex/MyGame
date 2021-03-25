#pragma once

#include <array>

template<class T,size_t Size>
class RingArray
{
public:
	void push(T&& value)
	{
		array_[write_cursor_++] = std::forward(value);

		if (write_cursor_ >= Size)
		{
			write_cursor_ = 0;
		}
	}

	T& pop()
	{
		auto& v = array_[read_cursor_++];
		if (read_cursor_ >= Size)
		{
			read_cursor_ = 0;
		}

		return v;
	}

	T& operator[](size_t index)
	{
		assert(index > 0 && index < Size);
		
		return array_[index];
	}

	size_t size() const
	{
		return array_.size();
	}
private:
	size_t read_cursor_;
	size_t write_cursor_;
	std::array<T, Size> array_;
};