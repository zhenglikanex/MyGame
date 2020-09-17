#include <gtest/gtest.h>
#include <entt/core/attribute.h>
#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>
#include "types.h"

ENTT_API void set_up();
ENTT_API void tear_down();
ENTT_API entt::meta_any wrap_int(int);

TEST(Lib, Meta) {
    ASSERT_FALSE(entt::resolve_id("position"_hs));
    ASSERT_FALSE(entt::resolve_id("velocity"_hs));

    set_up();
    entt::meta<double>().conv<int>();

    ASSERT_TRUE(entt::resolve_id("position"_hs));
    ASSERT_TRUE(entt::resolve_id("velocity"_hs));

    ASSERT_EQ(entt::resolve<position>(), entt::resolve_id("position"_hs));
    ASSERT_EQ(entt::resolve<velocity>(), entt::resolve_id("velocity"_hs));

    auto pos = entt::resolve_id("position"_hs).construct(42., 3.);
    auto vel = entt::resolve_id("velocity"_hs).ctor<>().invoke();

    ASSERT_TRUE(pos && vel);

    ASSERT_EQ(pos.type().data("x"_hs).type(), entt::resolve<int>());
    ASSERT_TRUE(pos.type().data("y"_hs).get(pos).try_cast<int>());
    ASSERT_EQ(pos.type().data("x"_hs).get(pos).cast<int>(), 42);
    ASSERT_EQ(pos.type().data("y"_hs).get(pos).cast<int>(), 3);

    ASSERT_EQ(vel.type().data("dx"_hs).type(), entt::resolve<double>());
    ASSERT_TRUE(vel.type().data("dy"_hs).get(vel).convert<double>());
    ASSERT_EQ(vel.type().data("dx"_hs).get(vel).cast<double>(), 0.);
    ASSERT_EQ(vel.type().data("dy"_hs).get(vel).cast<double>(), 0.);

    ASSERT_EQ(wrap_int(42).type(), entt::resolve<int>());
    ASSERT_EQ(wrap_int(42).cast<int>(), 42);

    tear_down();

    ASSERT_FALSE(entt::resolve_id("position"_hs));
    ASSERT_FALSE(entt::resolve_id("velocity"_hs));
}
