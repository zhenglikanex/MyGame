#define CATCH_CONFIG_MAIN

#include <iostream>
#include <bitset>
#include "3rdparty/include/catch2/catch.hpp"
#include "Framework/Game/GAS/GameplayTagDefine.hpp"

struct Test
{

};

TEST_CASE_METHOD(Test, "GameplayTag")
{
	REQUIRE(!GameplayTagDefine::kAbilitySprint.Matching(GameplayTagDefine::kAbilityJump));

	REQUIRE(GameplayTagDefine::kAbilityJump.Matching(GameplayTagDefine::kAbility));
	REQUIRE(GameplayTagDefine::kAbility.Matching(GameplayTagDefine::kAbility));
	REQUIRE(!GameplayTagDefine::kAbility.Matching(GameplayTagDefine::kAbilityJump));
	REQUIRE(!GameplayTagDefine::kAbility.Matching(GameplayTagDefine::kState));
	
	REQUIRE(GameplayTagDefine::kAbilityJump.MatchingExact(GameplayTagDefine::kAbilityJump));
	REQUIRE(!GameplayTagDefine::kAbilityJump.MatchingExact(GameplayTagDefine::kAbility));
}


TEST_CASE_METHOD(Test, "GameplayTagArray")
{
	GameplayTagArray tags;

	REQUIRE(!tags.HasTag(GameplayTagDefine::kAbility));
	REQUIRE(!tags.HasTagExact(GameplayTagDefine::kAbility));

	tags.Push(GameplayTagDefine::kAbility);
	tags.Push(GameplayTagDefine::kAbilityJump);
	tags.Push(GameplayTagDefine::kStateSprint);

	REQUIRE(tags.HasTag(GameplayTagDefine::kState));
	REQUIRE(tags.HasTag(GameplayTagDefine::kStateSprint));
	REQUIRE(!tags.HasTag(GameplayTagDefine::kAbilityMeleeAttack));

	REQUIRE(!tags.HasTagExact(GameplayTagDefine::kState));
	REQUIRE(tags.HasTagExact(GameplayTagDefine::kAbility));
	REQUIRE(tags.HasTagExact(GameplayTagDefine::kAbilityJump));

	REQUIRE(tags.HasAll({ GameplayTagDefine::kAbility,GameplayTagDefine::kState }));
	REQUIRE(tags.HasAll({ GameplayTagDefine::kAbilityJump,GameplayTagDefine::kStateSprint }));
	REQUIRE(tags.HasAll({}));
	REQUIRE(!tags.HasAll({ GameplayTagDefine::kAbilityMeleeAttack }));
	REQUIRE(!tags.HasAll({ GameplayTagDefine::kStateAttack }));

	REQUIRE(tags.HasAllExact({ GameplayTagDefine::kAbility,GameplayTagDefine::kStateSprint }));
	REQUIRE(tags.HasAllExact({}));
	REQUIRE(!tags.HasAllExact({ GameplayTagDefine::kAbility,GameplayTagDefine::kState }));

	REQUIRE(tags.HasAny({ GameplayTagDefine::kAbility,GameplayTagDefine::kAbilityMeleeAttack }));
	REQUIRE(tags.HasAny({ GameplayTagDefine::kState }));
	REQUIRE(tags.HasAny({ GameplayTagDefine::kAbilityJump }));
	REQUIRE(!tags.HasAny({ GameplayTagDefine::kAbilityMeleeAttack,GameplayTagDefine::kAbilityRoll }));
	REQUIRE(!tags.HasAny({}));

	REQUIRE(tags.HasAnyExact({ GameplayTagDefine::kAbility,GameplayTagDefine::kState }));
	REQUIRE(!tags.HasAnyExact({ GameplayTagDefine::kState }));
	REQUIRE(!tags.HasAnyExact({}));
}

TEST_CASE_METHOD(Test, "GameplayTagCountContainer")
{
	GameplayTagCountContainer tag_count;
	
	tag_count.UpdateTagCount({ GameplayTagDefine::kAbilityJump,GameplayTagDefine::kStateAttack }, 2);
	
	REQUIRE(tag_count.HasTag(GameplayTagDefine::kAbilityJump));
	REQUIRE(tag_count.HasTag(GameplayTagDefine::kStateAttack));
	REQUIRE(tag_count.HasTag(GameplayTagDefine::kAbility));
	REQUIRE(tag_count.HasTag(GameplayTagDefine::kState));
	REQUIRE(!tag_count.HasTag(GameplayTagDefine::kStateJump));

	REQUIRE(tag_count.HasTagExact(GameplayTagDefine::kAbilityJump));
	REQUIRE(tag_count.HasTagExact(GameplayTagDefine::kStateAttack));
	REQUIRE(!tag_count.HasTagExact(GameplayTagDefine::kAbility));
	REQUIRE(!tag_count.HasTagExact(GameplayTagDefine::kState));
	REQUIRE(!tag_count.HasTagExact(GameplayTagDefine::kStateJump));

	REQUIRE(tag_count.GetTagCount(GameplayTagDefine::kAbilityJump) == 2);
	REQUIRE(tag_count.GetTagCount(GameplayTagDefine::kStateAttack) == 2);

	tag_count.SetTagCount(GameplayTagDefine::kAbilityJump, 0);
	REQUIRE(tag_count.GetTagCount(GameplayTagDefine::kAbilityJump) == 0);
	REQUIRE(!tag_count.HasTag(GameplayTagDefine::kAbilityJump));

	tag_count.UpdateTagCount({ GameplayTagDefine::kAbilityJump ,GameplayTagDefine::kStateJump,GameplayTagDefine::kStateAttack }, 1);
	REQUIRE(tag_count.GetTagCount(GameplayTagDefine::kAbilityJump) == 1);
	REQUIRE(tag_count.GetTagCount(GameplayTagDefine::kStateJump) == 1);
	REQUIRE(tag_count.GetTagCount(GameplayTagDefine::kStateAttack) == 3);
	REQUIRE(tag_count.HasTag(GameplayTagDefine::kStateJump));
	REQUIRE(tag_count.HasTag(GameplayTagDefine::kAbilityJump));

	REQUIRE(tag_count.HasAll({ GameplayTagDefine::kAbility, GameplayTagDefine::kState }));
	REQUIRE(tag_count.HasAll({ GameplayTagDefine::kAbility,GameplayTagDefine::kStateJump}));
	REQUIRE(tag_count.HasAll({}));
	//REQUIRE(!tag_count.HasAll({ GameplayTagDefine::kAbility,GameplayTagDefine::kStateFlying }));

	REQUIRE(tag_count.HasAny({ GameplayTagDefine::kAbility,GameplayTagDefine::kAbilityMeleeAttack }));
	REQUIRE(tag_count.HasAny({ GameplayTagDefine::kStateAttack,GameplayTagDefine::kAbilityMeleeAttack }));
	REQUIRE(tag_count.HasAny({ GameplayTagDefine::kState,GameplayTagDefine::kAbilityMeleeAttack }));
	REQUIRE(!tag_count.HasAny({ GameplayTagDefine::kStateHurtFloating,GameplayTagDefine::kAbilityMeleeAttack }));
}
