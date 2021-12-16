#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "Kanex.hpp"

class GameplayTag
{
public:
	explicit GameplayTag(std::string_view tag)
		:tag_(tag)
	{
		Split();
	}

	GameplayTag(const GameplayTag& rhs)
		: tag_(rhs.tag_)
	{
		Split();
	}

	GameplayTag(GameplayTag&& rhs)
		:tag_(std::move(rhs.tag_))
	{
		Split();
	}

	const GameplayTag& operator=(const GameplayTag& rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}

		tag_ = rhs.tag_;
		hierarchy_.clear();
		Split();

		return *this;
	}

	const GameplayTag& operator=(GameplayTag&& rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}

		tag_ = std::move(rhs.tag_);
		hierarchy_.clear();
		Split();

		return *this;
	}

	bool operator==(const GameplayTag& tag) const
	{
		if (this == &tag)
		{
			return true;
		}

		return tag_ == tag.tag_;
	}

	bool operator!=(const GameplayTag& tag) const
	{
		return !(*this == tag);
	}

	bool Contain(const GameplayTag& tag) const
	{
		if (hierarchy_.size() > tag.hierarchy_.size())
		{
			return false;
		}

		for (int i = 0; i < hierarchy_.size(); ++i)
		{
			if (hierarchy_[i] != tag.hierarchy_[i])
			{
				return false;
			}
		}

		return true;
	}

	const std::string& tag() const { return tag_; }

	void Serialize(kanex::BinaryOutputArchive& ar)
	{
		ar(tag_);
	}
	void Serialize(kanex::BinaryInputArchive& ar)
	{
		ar(tag_);
		Split();
	}
private:
	void Split()
	{
		std::string_view strv = tag_;
		std::string_view delims = ".";

		size_t first = 0;

		while (first < strv.size())
		{
			const auto second = strv.find_first_of(delims, first);

			if (first != second)
				hierarchy_.emplace_back(strv.substr(first, second - first));

			if (second == std::string_view::npos)
				break;

			first = second + 1;
		}

	}

	std::string tag_;
	std::vector<std::string_view> hierarchy_;
};

namespace std
{
	template<>
	struct hash<GameplayTag>
	{
		_NODISCARD size_t operator()(const GameplayTag& value) const noexcept {
			return std::hash<std::string>()(value.tag());
		}
	};

}

class GameplayTagArray
{
public:
	void Push(const GameplayTag& tag)
	{
		tags_.push_back(tag);
	}

	void Clear()
	{
		tags_.clear();
	}

	bool Empty() const
	{
		return tags_.empty();
	}

	bool Find(const GameplayTag& tag) const
	{
		return std::find(tags_.begin(), tags_.end(), tag) != tags_.end();
	}

	bool HasAny(const GameplayTagArray& array) const
	{
		if (this == &array)
		{
			return true;
		}

		if (array.tags_.size() == 0)
		{
			return false;
		}

		for (int i = 0; i < tags_.size(); ++i)
		{
			for (int j = 0; j < array.tags_.size(); ++j)
			{
				if (tags_[i].Contain(array.tags_[j]))
				{
					return true;
				}
			}
		}

		return false;
	}

	bool HasAll(const GameplayTagArray& array) const
	{
		if (this == &array)
		{
			return true;
		}

		if (array.tags_.size() == 0)
		{
			return true;
		}

		// todo:后面改成查表的方式来做
		for (int i = 0; i < array.tags_.size(); ++i)
		{
			bool found = false;
			for (int j = 0; j < tags_.size(); ++j)
			{
				if (tags_[j].Contain(array.tags_[i]))
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				return false;
			}
		}

		return true;
	}

	std::vector<GameplayTag>::iterator Begin()
	{
		return tags_.begin();
	}

	std::vector<GameplayTag>::iterator End()
	{
		return tags_.end();
	}

	std::vector<GameplayTag>::const_iterator Begin() const
	{
		return tags_.begin();
	}

	std::vector<GameplayTag>::const_iterator End() const
	{
		return tags_.end();
	}

	BAR(tags_);
private:
	std::vector<GameplayTag> tags_;
};