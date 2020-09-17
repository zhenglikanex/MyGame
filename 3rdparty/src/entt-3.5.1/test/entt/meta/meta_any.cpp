#include <memory>
#include <gtest/gtest.h>
#include <entt/core/hashed_string.hpp>
#include <entt/meta/factory.hpp>
#include <entt/meta/meta.hpp>
#include <entt/meta/resolve.hpp>

struct empty_t {
    virtual ~empty_t() = default;
    static void destroy(empty_t &) {
        ++counter;
    }

    inline static int counter = 0;
};

struct fat_t: empty_t {
    fat_t() = default;

    fat_t(int *value)
        : foo{value}, bar{value}
    {}

    bool operator==(const fat_t &other) const {
        return foo == other.foo && bar == other.bar;
    }

    int *foo{nullptr};
    int *bar{nullptr};
};

struct not_comparable_t {
    bool operator==(const not_comparable_t &) const = delete;
};

struct unmanageable_t {
    unmanageable_t() = default;
    unmanageable_t(const unmanageable_t &) = delete;
    unmanageable_t(unmanageable_t &&) = delete;
    unmanageable_t & operator=(const unmanageable_t &) = delete;
    unmanageable_t & operator=(unmanageable_t &&) = delete;
};

struct MetaAny: ::testing::Test {
    static void SetUpTestCase() {
        entt::meta<double>().conv<int>();
        entt::meta<empty_t>().dtor<&empty_t::destroy>();
        entt::meta<fat_t>().base<empty_t>().dtor<&fat_t::destroy>();
    }

    void SetUp() override {
        empty_t::counter = 0;
    }
};

TEST_F(MetaAny, SBO) {
    entt::meta_any any{'c'};

    ASSERT_TRUE(any);
    ASSERT_FALSE(any.try_cast<std::size_t>());
    ASSERT_EQ(any.cast<char>(), 'c');
    ASSERT_NE(any.data(), nullptr);
    ASSERT_EQ(any, entt::meta_any{'c'});
    ASSERT_NE(entt::meta_any{'h'}, any);
}

TEST_F(MetaAny, NoSBO) {
    int value = 42;
    fat_t instance{&value};
    entt::meta_any any{instance};

    ASSERT_TRUE(any);
    ASSERT_FALSE(any.try_cast<std::size_t>());
    ASSERT_EQ(any.cast<fat_t>(), instance);
    ASSERT_NE(any.data(), nullptr);
    ASSERT_EQ(any, entt::meta_any{instance});
    ASSERT_NE(fat_t{}, any);
}

TEST_F(MetaAny, Empty) {
    entt::meta_any any{};

    ASSERT_FALSE(any);
    ASSERT_FALSE(any.type());
    ASSERT_FALSE(any.try_cast<std::size_t>());
    ASSERT_EQ(any.data(), nullptr);
    ASSERT_EQ(any, entt::meta_any{});
    ASSERT_NE(entt::meta_any{'c'}, any);
}

TEST_F(MetaAny, SBOInPlaceTypeConstruction) {
    entt::meta_any any{std::in_place_type<int>, 42};

    ASSERT_TRUE(any);
    ASSERT_FALSE(any.try_cast<std::size_t>());
    ASSERT_EQ(any.cast<int>(), 42);
    ASSERT_NE(any.data(), nullptr);
    ASSERT_EQ(any, (entt::meta_any{std::in_place_type<int>, 42}));
    ASSERT_EQ(any, entt::meta_any{42});
    ASSERT_NE(entt::meta_any{3}, any);
}

TEST_F(MetaAny, SBOAsRefConstruction) {
    int value = 3;
    int other = 42;
    entt::meta_any any{std::ref(value)};

    ASSERT_TRUE(any);
    ASSERT_FALSE(any.try_cast<std::size_t>());
    ASSERT_EQ(any.cast<int>(), 3);
    ASSERT_NE(any.data(), nullptr);
    ASSERT_EQ(any, (entt::meta_any{std::ref(value)}));
    ASSERT_NE(any, (entt::meta_any{std::ref(other)}));
    ASSERT_NE(any, entt::meta_any{42});
    ASSERT_EQ(entt::meta_any{3}, any);
}

TEST_F(MetaAny, SBOCopyConstruction) {
    entt::meta_any any{42};
    entt::meta_any other{any};

    ASSERT_TRUE(any);
    ASSERT_TRUE(other);
    ASSERT_FALSE(other.try_cast<std::size_t>());
    ASSERT_EQ(other.cast<int>(), 42);
    ASSERT_EQ(other, entt::meta_any{42});
    ASSERT_NE(other, entt::meta_any{0});
}

TEST_F(MetaAny, SBOCopyAssignment) {
    entt::meta_any any{42};
    entt::meta_any other{3};

    other = any;

    ASSERT_TRUE(any);
    ASSERT_TRUE(other);
    ASSERT_FALSE(other.try_cast<std::size_t>());
    ASSERT_EQ(other.cast<int>(), 42);
    ASSERT_EQ(other, entt::meta_any{42});
    ASSERT_NE(other, entt::meta_any{0});
}

TEST_F(MetaAny, SBOMoveConstruction) {
    entt::meta_any any{42};
    entt::meta_any other{std::move(any)};

    ASSERT_FALSE(any);
    ASSERT_TRUE(other);
    ASSERT_FALSE(other.try_cast<std::size_t>());
    ASSERT_EQ(other.cast<int>(), 42);
    ASSERT_EQ(other, entt::meta_any{42});
    ASSERT_NE(other, entt::meta_any{0});
}

TEST_F(MetaAny, SBOMoveAssignment) {
    entt::meta_any any{42};
    entt::meta_any other{3};

    other = std::move(any);

    ASSERT_FALSE(any);
    ASSERT_TRUE(other);
    ASSERT_FALSE(other.try_cast<std::size_t>());
    ASSERT_EQ(other.cast<int>(), 42);
    ASSERT_EQ(other, entt::meta_any{42});
    ASSERT_NE(other, entt::meta_any{0});
}

TEST_F(MetaAny, SBODirectAssignment) {
    entt::meta_any any{};
    any = 42;

    ASSERT_FALSE(any.try_cast<std::size_t>());
    ASSERT_EQ(any.cast<int>(), 42);
    ASSERT_EQ(any, entt::meta_any{42});
    ASSERT_NE(entt::meta_any{0}, any);
}

TEST_F(MetaAny, NoSBOInPlaceTypeConstruction) {
    int value = 42;
    fat_t instance{&value};
    entt::meta_any any{std::in_place_type<fat_t>, instance};

    ASSERT_TRUE(any);
    ASSERT_FALSE(any.try_cast<std::size_t>());
    ASSERT_EQ(any.cast<fat_t>(), instance);
    ASSERT_NE(any.data(), nullptr);
    ASSERT_EQ(any, (entt::meta_any{std::in_place_type<fat_t>, instance}));
    ASSERT_EQ(any, entt::meta_any{instance});
    ASSERT_NE(entt::meta_any{fat_t{}}, any);
}

TEST_F(MetaAny, NoSBOAsRefConstruction) {
    int value = 3;
    fat_t instance{&value};
    entt::meta_any any{std::ref(instance)};

    ASSERT_TRUE(any);
    ASSERT_FALSE(any.try_cast<std::size_t>());
    ASSERT_EQ(any.cast<fat_t>(), instance);
    ASSERT_NE(any.data(), nullptr);
    ASSERT_EQ(any, (entt::meta_any{std::ref(instance)}));
    ASSERT_EQ(any, entt::meta_any{instance});
    ASSERT_NE(entt::meta_any{fat_t{}}, any);
}

TEST_F(MetaAny, NoSBOCopyConstruction) {
    int value = 42;
    fat_t instance{&value};
    entt::meta_any any{instance};
    entt::meta_any other{any};

    ASSERT_TRUE(any);
    ASSERT_TRUE(other);
    ASSERT_FALSE(other.try_cast<std::size_t>());
    ASSERT_EQ(other.cast<fat_t>(), instance);
    ASSERT_EQ(other, entt::meta_any{instance});
    ASSERT_NE(other, fat_t{});
}

TEST_F(MetaAny, NoSBOCopyAssignment) {
    int value = 42;
    fat_t instance{&value};
    entt::meta_any any{instance};
    entt::meta_any other{3};

    other = any;

    ASSERT_TRUE(any);
    ASSERT_TRUE(other);
    ASSERT_FALSE(other.try_cast<std::size_t>());
    ASSERT_EQ(other.cast<fat_t>(), instance);
    ASSERT_EQ(other, entt::meta_any{instance});
    ASSERT_NE(other, fat_t{});
}

TEST_F(MetaAny, NoSBOMoveConstruction) {
    int value = 42;
    fat_t instance{&value};
    entt::meta_any any{instance};
    entt::meta_any other{std::move(any)};

    ASSERT_FALSE(any);
    ASSERT_TRUE(other);
    ASSERT_FALSE(other.try_cast<std::size_t>());
    ASSERT_EQ(other.cast<fat_t>(), instance);
    ASSERT_EQ(other, entt::meta_any{instance});
    ASSERT_NE(other, fat_t{});
}

TEST_F(MetaAny, NoSBOMoveAssignment) {
    int value = 42;
    fat_t instance{&value};
    entt::meta_any any{instance};
    entt::meta_any other{3};

    other = std::move(any);

    ASSERT_FALSE(any);
    ASSERT_TRUE(other);
    ASSERT_FALSE(other.try_cast<std::size_t>());
    ASSERT_EQ(other.cast<fat_t>(), instance);
    ASSERT_EQ(other, entt::meta_any{instance});
    ASSERT_NE(other, fat_t{});
}

TEST_F(MetaAny, NoSBODirectAssignment) {
    int value = 42;
    entt::meta_any any{};
    any = fat_t{&value};

    ASSERT_FALSE(any.try_cast<std::size_t>());
    ASSERT_EQ(any.cast<fat_t>(), fat_t{&value});
    ASSERT_EQ(any, entt::meta_any{fat_t{&value}});
    ASSERT_NE(fat_t{}, any);
}

TEST_F(MetaAny, VoidInPlaceTypeConstruction) {
    entt::meta_any any{std::in_place_type<void>};

    ASSERT_TRUE(any);
    ASSERT_FALSE(any.try_cast<char>());
    ASSERT_EQ(any.data(), nullptr);
    ASSERT_EQ(any.type(), entt::resolve<void>());
    ASSERT_EQ(any, entt::meta_any{std::in_place_type<void>});
    ASSERT_NE(entt::meta_any{3}, any);
}

TEST_F(MetaAny, VoidCopyConstruction) {
    entt::meta_any any{std::in_place_type<void>};
    entt::meta_any other{any};

    ASSERT_TRUE(any);
    ASSERT_TRUE(other);
    ASSERT_EQ(any.type(), entt::resolve<void>());
    ASSERT_EQ(other, entt::meta_any{std::in_place_type<void>});
}

TEST_F(MetaAny, VoidCopyAssignment) {
    entt::meta_any any{std::in_place_type<void>};
    entt::meta_any other{std::in_place_type<void>};

    other = any;

    ASSERT_TRUE(any);
    ASSERT_TRUE(other);
    ASSERT_EQ(any.type(), entt::resolve<void>());
    ASSERT_EQ(other, entt::meta_any{std::in_place_type<void>});
}

TEST_F(MetaAny, VoidMoveConstruction) {
    entt::meta_any any{std::in_place_type<void>};
    entt::meta_any other{std::move(any)};

    ASSERT_FALSE(any);
    ASSERT_TRUE(other);
    ASSERT_EQ(other.type(), entt::resolve<void>());
    ASSERT_EQ(other, entt::meta_any{std::in_place_type<void>});
}

TEST_F(MetaAny, VoidMoveAssignment) {
    entt::meta_any any{std::in_place_type<void>};
    entt::meta_any other{std::in_place_type<void>};

    other = std::move(any);

    ASSERT_FALSE(any);
    ASSERT_TRUE(other);
    ASSERT_EQ(other.type(), entt::resolve<void>());
    ASSERT_EQ(other, entt::meta_any{std::in_place_type<void>});
}

TEST_F(MetaAny, SBOMoveInvalidate) {
    entt::meta_any any{42};
    entt::meta_any other{std::move(any)};
    entt::meta_any valid = std::move(other);

    ASSERT_FALSE(any);
    ASSERT_FALSE(other);
    ASSERT_TRUE(valid);
}

TEST_F(MetaAny, NoSBOMoveInvalidate) {
    int value = 42;
    fat_t instance{&value};
    entt::meta_any any{instance};
    entt::meta_any other{std::move(any)};
    entt::meta_any valid = std::move(other);

    ASSERT_FALSE(any);
    ASSERT_FALSE(other);
    ASSERT_TRUE(valid);
}

TEST_F(MetaAny, VoidMoveInvalidate) {
    entt::meta_any any{std::in_place_type<void>};
    entt::meta_any other{std::move(any)};
    entt::meta_any valid = std::move(other);

    ASSERT_FALSE(any);
    ASSERT_FALSE(other);
    ASSERT_TRUE(valid);
}

TEST_F(MetaAny, SBODestruction) {
    ASSERT_EQ(empty_t::counter, 0);
    { entt::meta_any any{empty_t{}}; }
    ASSERT_EQ(empty_t::counter, 1);
}

TEST_F(MetaAny, NoSBODestruction) {
    ASSERT_EQ(fat_t::counter, 0);
    { entt::meta_any any{fat_t{}}; }
    ASSERT_EQ(fat_t::counter, 1);
}

TEST_F(MetaAny, VoidDestruction) {
    // just let asan tell us if everything is ok here
    [[maybe_unused]] entt::meta_any any{std::in_place_type<void>};
}

TEST_F(MetaAny, Emplace) {
    entt::meta_any any{};
    any.emplace<int>(42);

    ASSERT_TRUE(any);
    ASSERT_FALSE(any.try_cast<std::size_t>());
    ASSERT_EQ(any.cast<int>(), 42);
    ASSERT_NE(any.data(), nullptr);
    ASSERT_EQ(any, (entt::meta_any{std::in_place_type<int>, 42}));
    ASSERT_EQ(any, entt::meta_any{42});
    ASSERT_NE(entt::meta_any{3}, any);
}

TEST_F(MetaAny, EmplaceVoid) {
    entt::meta_any any{};
    any.emplace<void>();

    ASSERT_TRUE(any);
    ASSERT_EQ(any.data(), nullptr);
    ASSERT_EQ(any.type(), entt::resolve<void>());
    ASSERT_EQ(any, (entt::meta_any{std::in_place_type<void>}));
}

TEST_F(MetaAny, SBOSwap) {
    entt::meta_any lhs{'c'};
    entt::meta_any rhs{42};

    std::swap(lhs, rhs);

    ASSERT_FALSE(lhs.try_cast<char>());
    ASSERT_EQ(lhs.cast<int>(), 42);
    ASSERT_FALSE(rhs.try_cast<int>());
    ASSERT_EQ(rhs.cast<char>(), 'c');
}

TEST_F(MetaAny, NoSBOSwap) {
    int i, j;
    entt::meta_any lhs{fat_t{&i}};
    entt::meta_any rhs{fat_t{&j}};

    std::swap(lhs, rhs);

    ASSERT_EQ(lhs.cast<fat_t>().foo, &j);
    ASSERT_EQ(rhs.cast<fat_t>().bar, &i);
}

TEST_F(MetaAny, VoidSwap) {
    entt::meta_any lhs{std::in_place_type<void>};
    entt::meta_any rhs{std::in_place_type<void>};
    const auto *pre = lhs.data();

    std::swap(lhs, rhs);

    ASSERT_EQ(pre, lhs.data());
}

TEST_F(MetaAny, SBOWithNoSBOSwap) {
    int value = 42;
    entt::meta_any lhs{fat_t{&value}};
    entt::meta_any rhs{'c'};

    std::swap(lhs, rhs);

    ASSERT_FALSE(lhs.try_cast<fat_t>());
    ASSERT_EQ(lhs.cast<char>(), 'c');
    ASSERT_FALSE(rhs.try_cast<char>());
    ASSERT_EQ(rhs.cast<fat_t>().foo, &value);
    ASSERT_EQ(rhs.cast<fat_t>().bar, &value);
}

TEST_F(MetaAny, SBOWithEmptySwap) {
    entt::meta_any lhs{'c'};
    entt::meta_any rhs{};

    std::swap(lhs, rhs);

    ASSERT_FALSE(lhs);
    ASSERT_EQ(rhs.cast<char>(), 'c');

    std::swap(lhs, rhs);

    ASSERT_FALSE(rhs);
    ASSERT_EQ(lhs.cast<char>(), 'c');
}

TEST_F(MetaAny, SBOWithVoidSwap) {
    entt::meta_any lhs{'c'};
    entt::meta_any rhs{std::in_place_type<void>};

    std::swap(lhs, rhs);

    ASSERT_EQ(lhs.type(), entt::resolve<void>());
    ASSERT_EQ(rhs.cast<char>(), 'c');
}

TEST_F(MetaAny, NoSBOWithEmptySwap) {
    int i;
    entt::meta_any lhs{fat_t{&i}};
    entt::meta_any rhs{};

    std::swap(lhs, rhs);

    ASSERT_EQ(rhs.cast<fat_t>().bar, &i);

    std::swap(lhs, rhs);

    ASSERT_EQ(lhs.cast<fat_t>().bar, &i);
}

TEST_F(MetaAny, NoSBOWithVoidSwap) {
    int i;
    entt::meta_any lhs{fat_t{&i}};
    entt::meta_any rhs{std::in_place_type<void>};

    std::swap(lhs, rhs);

    ASSERT_EQ(rhs.cast<fat_t>().bar, &i);

    std::swap(lhs, rhs);

    ASSERT_EQ(lhs.cast<fat_t>().bar, &i);
}

TEST_F(MetaAny, Comparable) {
    entt::meta_any any{'c'};

    ASSERT_EQ(any, any);
    ASSERT_EQ(any, entt::meta_any{'c'});
    ASSERT_NE(entt::meta_any{'a'}, any);
    ASSERT_NE(any, entt::meta_any{});

    ASSERT_TRUE(any == any);
    ASSERT_TRUE(any == entt::meta_any{'c'});
    ASSERT_FALSE(entt::meta_any{'a'} == any);
    ASSERT_TRUE(any != entt::meta_any{'a'});
    ASSERT_TRUE(entt::meta_any{} != any);
}

TEST_F(MetaAny, NotComparable) {
    entt::meta_any any{not_comparable_t{}};

    ASSERT_EQ(any, any);
    ASSERT_NE(any, entt::meta_any{not_comparable_t{}});
    ASSERT_NE(entt::meta_any{}, any);

    ASSERT_TRUE(any == any);
    ASSERT_FALSE(any == entt::meta_any{not_comparable_t{}});
    ASSERT_TRUE(entt::meta_any{} != any);
}

TEST_F(MetaAny, CompareVoid) {
    entt::meta_any any{std::in_place_type<void>};

    ASSERT_EQ(any, any);
    ASSERT_EQ(any, entt::meta_any{std::in_place_type<void>});
    ASSERT_NE(entt::meta_any{'a'}, any);
    ASSERT_NE(any, entt::meta_any{});

    ASSERT_TRUE(any == any);
    ASSERT_TRUE(any == entt::meta_any{std::in_place_type<void>});
    ASSERT_FALSE(entt::meta_any{'a'} == any);
    ASSERT_TRUE(any != entt::meta_any{'a'});
    ASSERT_TRUE(entt::meta_any{} != any);
}

TEST_F(MetaAny, TryCast) {
    entt::meta_any any{fat_t{}};

    ASSERT_TRUE(any);
    ASSERT_EQ(any.type(), entt::resolve<fat_t>());
    ASSERT_EQ(any.try_cast<void>(), nullptr);
    ASSERT_NE(any.try_cast<empty_t>(), nullptr);
    ASSERT_EQ(any.try_cast<fat_t>(), any.data());
    ASSERT_EQ(std::as_const(any).try_cast<empty_t>(), any.try_cast<empty_t>());
    ASSERT_EQ(std::as_const(any).try_cast<fat_t>(), any.data());
}

TEST_F(MetaAny, Cast) {
    entt::meta_any any{fat_t{}};

    ASSERT_TRUE(any);
    ASSERT_EQ(any.type(), entt::resolve<fat_t>());
    ASSERT_EQ(any.try_cast<std::size_t>(), nullptr);
    ASSERT_NE(any.try_cast<empty_t>(), nullptr);
    ASSERT_EQ(any.try_cast<fat_t>(), any.data());
    ASSERT_EQ(std::as_const(any).try_cast<empty_t>(), any.try_cast<empty_t>());
    ASSERT_EQ(std::as_const(any).try_cast<fat_t>(), any.data());
}

TEST_F(MetaAny, Convert) {
    entt::meta_any any{42.};

    ASSERT_TRUE(any);
    ASSERT_EQ(any.type(), entt::resolve<double>());
    ASSERT_TRUE(any.convert<double>());
    ASSERT_FALSE(any.convert<char>());
    ASSERT_EQ(any.type(), entt::resolve<double>());
    ASSERT_EQ(any.cast<double>(), 42.);
    ASSERT_TRUE(any.convert<int>());
    ASSERT_EQ(any.type(), entt::resolve<int>());
    ASSERT_EQ(any.cast<int>(), 42);
}

TEST_F(MetaAny, ConstConvert) {
    const entt::meta_any any{42.};

    ASSERT_TRUE(any);
    ASSERT_EQ(any.type(), entt::resolve<double>());
    ASSERT_TRUE(any.convert<double>());
    ASSERT_FALSE(any.convert<char>());
    ASSERT_EQ(any.type(), entt::resolve<double>());
    ASSERT_EQ(any.cast<double>(), 42.);

    auto other = any.convert<int>();

    ASSERT_EQ(any.type(), entt::resolve<double>());
    ASSERT_EQ(any.cast<double>(), 42.);
    ASSERT_EQ(other.type(), entt::resolve<int>());
    ASSERT_EQ(other.cast<int>(), 42);
}

TEST_F(MetaAny, UnmanageableType) {
    unmanageable_t instance;
    entt::meta_any any{std::ref(instance)};
    entt::meta_any other = any;

    std::swap(any, other);

    ASSERT_TRUE(any);
    ASSERT_TRUE(other);

    ASSERT_EQ(any.type(), entt::resolve<unmanageable_t>());
    ASSERT_NE(any.data(), nullptr);
    ASSERT_EQ(any.try_cast<int>(), nullptr);
    ASSERT_NE(any.try_cast<unmanageable_t>(), nullptr);

    ASSERT_TRUE(any.convert<unmanageable_t>());
    ASSERT_FALSE(any.convert<int>());

    ASSERT_TRUE(std::as_const(any).convert<unmanageable_t>());
    ASSERT_FALSE(std::as_const(any).convert<int>());
}
