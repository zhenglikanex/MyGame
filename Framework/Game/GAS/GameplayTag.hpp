#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "3rdparty/include/sol/sol.hpp"

class GameplayTag
{
public:
	explicit GameplayTag(const std::string& tag)
		:tag_(tag)
	{
		split();
	}

	GameplayTag(const GameplayTag& rhs)
		: tag_(rhs.tag_)
	{
		split();
	}

	GameplayTag(GameplayTag&& rhs)
		:tag_(std::move(rhs.tag_))
	{
		split();
	}

	const GameplayTag& operator=(const GameplayTag& rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}

		tag_ = rhs.tag_;
		hierarchy_.clear();
		split();
	}

	const GameplayTag& operator=(GameplayTag&& rhs)
	{
		if (this == &rhs)
		{
			return *this;
		}

		tag_ = std::move(rhs.tag_);
		hierarchy_.clear();
		split();
	}

	bool operator==(const GameplayTag& tag)
	{
		return tag_ == tag.tag_;
	}

	bool operator!=(const GameplayTag& tag)
	{
		return !(*this == tag);
	}

	bool Contain(const GameplayTag& tag)
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
private:
	void split()
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

void RegisterGameplayTag(sol::state& lua)
{
	auto type = lua.new_usertype<GameplayTag>("GameplayTag", sol::constructors<GameplayTag(const std::string&), GameplayTag(const GameplayTag&)>());
	type["tag"] = &GameplayTag::tag;
	type["Contain"] = &GameplayTag::Contain;
}