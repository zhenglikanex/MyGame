#pragma once

#include <cassert>
#include <vector>
#include <iostream>
#include <array>

namespace kanex
{
	class Buffer
	{
	public:
		Buffer(size_t capactiy) :write_cursor_(0), read_cursor_(0) { buffer_.reserve(capactiy); }

		void Write(const void* data, size_t size)
		{
			if (write_cursor_ + size > buffer_.size())
			{
				buffer_.resize(write_cursor_ + size);
			}
			//std::copy(static_cast<char*>(data), static_cast<char*>(data) + size, buffer_.begin() + write_cursor_);
			//memcpy(buffer_.data() + write_cursor_, data, size);
			std::copy_n(static_cast<const char*>(data),size, buffer_.begin() + write_cursor_);
			write_cursor_ += size;
		}

		void Read(void* out, size_t size)
		{
			assert(read_cursor_ + size <= buffer_.size());
			//memcpy(out, buffer_.data() + read_cursor_, size);
			//std::copy(buffer_.begin() + read_cursor_, buffer_.begin() + read_cursor_ + size, out);
			std::copy_n(buffer_.begin() + read_cursor_,size, static_cast<char*>(out));
			read_cursor_ += size;
		}
		
		void Clear()
		{
			write_cursor_ = 0;
			read_cursor_ = 0;
		}

		void ResetRead() { read_cursor_ = 0; }
 
		void Reserve(size_t capactiy) { buffer_.reserve(capactiy); }
		size_t Size() const { return buffer_.size(); }
		const char* GetData() const { return buffer_.data(); }
	private:
		std::vector<char> buffer_;
		size_t write_cursor_;
		size_t read_cursor_;
	};

	class BinaryStream
	{
	public:
		BinaryStream(Buffer& buffer) : buffer_(buffer) { }

		Buffer& buffer() { return buffer_; }
	private:
		Buffer& buffer_;
	};
}