#pragma once

#include <string>
#include <vector>
#include <cstdint>

#include "Framework/Game/Geometry.hpp"
#include "Framework/Game/GAS/GameplayTag.hpp"

namespace timeline_event
{
	enum class EventType : uint8_t
	{
		kAnimation,
		kCollider,
		kTag
	};

	struct AnimationEvent
	{
		uint16_t start_frame;
		uint16_t end_frame;
		std::string name;
	};

	struct ColliderEvent
	{
		uint16_t start_frame;
		uint16_t end_frame;
		bool once;
		bool follow;
		Geometry geometry;
		mat4 transform;
	};

	struct TagEvent
	{
		uint16_t start_frame;
		uint16_t end_frame;
		GameplayTagArray granted_tags;
	};


	inline void from_json(const nlohmann::json& json, AnimationEvent& event)
	{
		json.at("StartFrame").get_to(event.start_frame);
		json.at("EndFrame").get_to(event.end_frame);
		json.at("Name").get_to(event.name);
	}

	inline void from_json(const nlohmann::json& json, ColliderEvent& event)
	{
		json.at("StartFrame").get_to(event.start_frame);
		json.at("EndFrame").get_to(event.end_frame);
		json.at("Once").get_to(event.once);
		json.at("Follow").get_to(event.follow);
		json.at("Transform").get_to(event.transform);

		std::string type = json["Type"];

		assert((type == "sphere" || type == "box" || type == "capsule") && "not support collision type");

		if (type == "sphere")
		{
			float r;
			json.at("Radius").get_to(r);

			event.geometry = Geometry(Sphere(r));
		}
		else if (type == "box")
		{
			vec3 e;
			json.at("Size").get_to(e);

			event.geometry = Geometry(OBB(e));
		}
		else
		{
			float h;
			json.at("HalfHeight").get_to(h);

			float r;
			json.at("Radius").get_to(r);

			event.geometry = Geometry(Capsule(h, r));
		}

	}

	inline void from_json(const nlohmann::json& json, TagEvent& event)
	{
		json.at("StartFrame").get_to(event.start_frame);
		json.at("EndFrame").get_to(event.end_frame);
		std::vector<GameplayTag> tags;
		json.at("GrantedTags").get_to(tags);
		event.granted_tags = GameplayTagArray(std::move(tags));
	}
}

class GameplayAbilityTimeline
{
public:
	friend  void from_json(const nlohmann::json& json, GameplayAbilityTimeline& timeline);

	enum class EventStatus : uint8_t
	{
		kStart,
		kDoing,
		kEnd
	};

	struct Event
	{
		timeline_event::EventType type;
		uint8_t index;
		EventStatus status;

		Event(timeline_event::EventType _type,uint8_t _index,EventStatus _status)
			: type(_type)
			, index(_index)
			, status(_status)
		{
			
		}
	};

	void RegisterEvents()
	{
		using namespace timeline_event;

		check_event_items_.clear();
		check_event_items_.reserve(animation_events_.size() + collider_events_.size() + tag_events_.size());

		for (uint8_t i = 0; i < animation_events_.size(); ++i)
		{
			auto& event = animation_events_[i];
			check_event_items_.emplace_back(CheckEventItem{ event.start_frame,event.end_frame,EventType::kAnimation, i });
		}

		for (uint8_t i = 0; i < collider_events_.size(); ++i)
		{
			auto& event = collider_events_[i];
			check_event_items_.emplace_back(CheckEventItem{ event.start_frame,event.end_frame,EventType::kCollider, i });
		}

		for (uint8_t i = 0; i < tag_events_.size(); ++i)
		{
			auto& event = tag_events_[i];
			check_event_items_.emplace_back(CheckEventItem{ event.start_frame,event.end_frame,EventType::kTag, i });
		}
	}

	void GetEvent(uint32_t frame, std::vector<Event>& events) const
	{
		events.clear();

		for(auto& check : check_event_items_)
		{
			if(frame == check.start_frame)
			{
				events.emplace_back(check.type, check.index, EventStatus::kStart);
			}

			if(frame > check.start_frame && frame < check.end_frame)
			{
				events.emplace_back(check.type, check.index, EventStatus::kDoing);
			}

			if(frame == check.end_frame)
			{
				events.emplace_back(check.type, check.index, EventStatus::kEnd);
			}
		}
	}

	const timeline_event::AnimationEvent& GetAnimationEventByIndex(uint16_t index) const
	{
		return animation_events_[index];
	}

	const timeline_event::ColliderEvent& GetColliderEventByIndex(uint16_t index) const
	{
		return collider_events_[index];
	}

	const timeline_event::TagEvent& GetTagEventByIndex(uint16_t index) const
	{
		return tag_events_[index];
	}

	uint16_t frame_count() const noexcept { return frame_count_; }
private:
	struct CheckEventItem
	{
		uint16_t start_frame;
		uint16_t end_frame;
		timeline_event::EventType type;
		uint16_t index;
	};

	uint16_t frame_count_;
	std::vector<CheckEventItem> check_event_items_;
	std::vector<timeline_event::AnimationEvent> animation_events_;
	std::vector<timeline_event::ColliderEvent> collider_events_;
	std::vector<timeline_event::TagEvent> tag_events_;
};

inline void from_json(const nlohmann::json& json, GameplayAbilityTimeline& timeline)
{
	json.at("FrameCount").get_to(timeline.frame_count_);
	json.at("AnimationEvents").get_to(timeline.animation_events_);
	json.at("ColliderEvents").get_to(timeline.collider_events_);
	json.at("TagEvents").get_to(timeline.tag_events_);
}