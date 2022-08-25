#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <functional>

#include "Kanex.hpp"

#include "3rdparty/include/nlohmann/json.hpp"

class GameplayTag
{
public:
	friend  inline void from_json(const nlohmann::json& json, GameplayTag& tag);

	GameplayTag() = default;

	explicit GameplayTag(uint64_t mask,uint64_t value)
		: mask_(mask)
		, value_(value)
	{

	}

	bool operator==(const GameplayTag& tag) const
	{
		return value_ == tag.value_;
	}

	bool operator!=(const GameplayTag& tag) const
	{
		return !(*this == tag);
	}

	// "A.1".Matching."A" true "A.1".Matching."A.1" true
	bool Matching(const GameplayTag& tag) const
	{
		return (value_ & tag.mask_) == tag.value_;
	}

	// "A.1".Matching."A" false "A.1".Matching."A.1" true
	bool MatchingExact(const GameplayTag& tag) const
	{
		return value_ == tag.value_;
	}

	uint64_t mask() const
	{
		return mask_;
	}

	uint64_t value() const
	{
		return value_;
	}

	BAR(mask_,value_);
private:
	int64_t mask_;
	int64_t value_;
};

inline void from_json(const nlohmann::json& json, GameplayTag& tag)
{
	json.at("Mask").get_to(tag.mask_);
	json.at("Value").get_to(tag.value_);
}

namespace std
{
	template<>
	struct hash<GameplayTag>
	{
		_NODISCARD size_t operator()(const GameplayTag& tag) const noexcept {
			return tag.value();
		}
	};
}

class GameplayTagArray
{
public:
	GameplayTagArray() = default;

	GameplayTagArray(std::initializer_list<GameplayTag> args)
		: tags_(args)
	{

	}

	explicit GameplayTagArray(std::vector<GameplayTag> tags)
		: tags_(std::move(tags))
	{
		
	}

	bool operator==(const GameplayTagArray& other) const
	{
		return tags_ == other.tags_;
	}

	bool operator!=(const GameplayTagArray& other) const
	{
		return !(*this == other);
	}

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

	// { "A.1" }.HasTag{"A"} return true { "A.1" }.HasTagExact{"A.1"} true
	bool HasTag(const GameplayTag& check_tag) const
	{
		auto v = std::bind(&GameplayTagArray::Push,std::placeholders::_1);
		return std::find_if(tags_.begin(), tags_.end(),[&check_tag](const GameplayTag& tag)
			{
				return tag.Matching(check_tag);
			}) != tags_.end();
	}

	// { "A.1" }.HasTagExact{"A"} return false { "A.1" }.HasTagExact{"A.1"} true
	bool HasTagExact(const GameplayTag& check_tag) const
	{
		return std::find_if(tags_.begin(), tags_.end(), [&check_tag](const GameplayTag& tag)
			{
				return tag.MatchingExact(check_tag);
			}) != tags_.end();
	}

	// { "A.1"}.HasAll{ "A","B" } return false { "A.1","B.1"}.HasAll{ "A","B" } return true
	// { "A.1"}.HasAll{ }return true
	bool HasAll(const GameplayTagArray& tags) const
	{
		if (tags.Empty())
		{
			return true;
		}

		for (auto it = tags.Begin(); it != tags.End(); ++it)
		{
			auto& check_tag = *it;
			if (!HasTag(check_tag))
			{
				return false;
			}
		}

		return true;
	}

	// { "A.1"}.HasAll{ "A","B" } return false { "A.1","B.1"}.HasAll{ "A","B" } return true
	// { "A.1"}.HasAll{ }return true
	bool HasAllExact(const GameplayTagArray& tags) const
	{
		if (tags.Empty())
		{
			return true;
		}

		for (auto it = tags.Begin(); it != tags.End(); ++it)
		{
			auto& check_tag = *it;
			if (!HasTagExact(check_tag))
			{
				return false;
			}
		}

		return true;
	}

	// { "A.1"}.HasAny{ "A","B" } return true { "C.1" }.HasAny{ "A","B" } return false
	// { "A.1"}.HasAny{ } return false
	bool HasAny(const GameplayTagArray& tags) const
	{
		if (tags.Empty())
		{
			return false;
		}

		for (auto it = tags.Begin(); it != tags.End(); ++it)
		{
			auto& check_tag = *it;
			if (HasTag(check_tag))
			{
				return true;
			}
		}

		return false;
	}

	// { "A.1"}.HasAny{ "A","B" } return false { "A.1"}.HasAny{ "A.1","B.1" } return true
	// { "A.1"}.HasAny{ } return false
	bool HasAnyExact(const GameplayTagArray& tags) const
	{
		if (tags.Empty())
		{
			return false;
		}

		for (auto it = tags.Begin(); it != tags.End(); ++it)
		{
			auto& check_tag = *it;
			if (HasTagExact(check_tag))
			{
				return true;
			}
		}

		return false;
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

class GameplayTagCountContainer
{
public:
	bool operator==(const GameplayTagCountContainer& other) const
	{
		return tag_count_map_ == other.tag_count_map_;
	}

	bool operator!=(const GameplayTagCountContainer& other) const
	{
		return !(*this == other);
	}

	GameplayTagArray GetExplicitGameplayTags() const
	{
		GameplayTagArray tags;

		for (auto& item : tag_count_map_)
		{
			if(item.second > 0)
			{ 
				tags.Push(item.first);
			}
		}

		return tags;
	}

	int32_t GetTagCount(const GameplayTag& tag) const
	{
		auto it = tag_count_map_.find(tag);
		if (it != tag_count_map_.end())
		{
			return it->second;
		}
		return 0;
	}

	// { "A.1" }.HasTag{"A"} return true { "A.1" }.HasTagExact{"A.1"} true
	bool HasTag(const GameplayTag& check_tag) const
	{
		return std::find_if(tag_count_map_.begin(), tag_count_map_.end(), [&check_tag](const std::pair<GameplayTag, uint8_t>& value)
			{
				return value.second > 0 && value.first.Matching(check_tag);
			}) != tag_count_map_.end();
	}

	// { "A.1" }.HasTagExact{"A"} return false { "A.1" }.HasTagExact{"A.1"} true
	bool HasTagExact(const GameplayTag& check_tag) const
	{
		return GetTagCount(check_tag) > 0;
	}

	// { "A.1"}.HasAll{ "A","B" } return false { "A.1","B.1"}.HasAll{ "A","B" } return true
	// { "A.1"}.HasAll{ }return true
	bool HasAll(const GameplayTagArray& tags) const
	{
		if (tags.Empty())
		{
			return true;
		}

		for (auto it = tags.Begin(); it != tags.End(); ++it)
		{
			auto& check_tag = *it;
			if (!HasTag(check_tag))
			{
				return false;
			}
		}

		return true;
	}

	// { "A.1"}.HasAny{ "A","B" } return true { "C.1" }.HasAny{ "A","B" } return false
	// { "A.1"}.HasAny{ } return false
	bool HasAny(const GameplayTagArray& tags) const
	{
		if (tags.Empty())
		{
			return false;
		}

		for (auto it = tags.Begin(); it != tags.End(); ++it)
		{
			auto& check_tag = *it;
			if (HasTag(check_tag))
			{
				return true;
			}
		}

		return false;
	}

	void SetTagCount(const GameplayTag& tag, int32_t count)
	{
		if (count <= 0)
		{
			tag_count_map_.erase(tag);
			return;
		}

		tag_count_map_[tag] = count;
	}

	void UpdateTagCount(const GameplayTag& tag, int32_t delta)
	{
		auto it = tag_count_map_.find(tag);
		if (it != tag_count_map_.end())
		{
			it->second += delta;

			if (it->second < 0)
			{
				tag_count_map_.erase(it);
			}
		}
		else
		{
			if (delta > 0)
			{
				tag_count_map_[tag] = delta;
			}
		}
	}

	void UpdateTagCount(const GameplayTagArray& tags, int32_t delta)
	{
		for (auto it = tags.Begin(); it != tags.End(); ++it)
		{
			UpdateTagCount(*it, delta);
		}
	}

	BAR(tag_count_map_);
private:
	std::unordered_map<GameplayTag, int32_t> tag_count_map_;
};