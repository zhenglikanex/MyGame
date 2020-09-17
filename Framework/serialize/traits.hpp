#pragma once

#include <type_traits>

#include "serialize/access.hpp"

namespace serialize
{
	template<class T,class Frist,class ... Other>
	struct is_same_and : std::integral_constant<bool,std::is_same_v<Frist,T> && is_same_and<T,Other...>::value> {};

	template<class T,class Last>
	struct is_same_and<T, Last> : std::integral_constant<bool,std::is_same_v<Last,T>> {};

	template<class T,class ... Other>
	constexpr bool is_same_and_v = is_same_and<T, Other...>::value;

	template<class T,class Frist,class ... Other>
	struct is_same_or : std::integral_constant<bool,std::is_same_v<Frist,T> || is_same_or<T,Other...>::value> {};
	
	template<class T,class Last>
	struct is_same_or<T,Last> : std::integral_constant<bool,std::is_same_v<Last,T>> {};

	template<class T,class ... Other>
	constexpr bool is_same_or_v = is_same_or<T, Other...>::value;

	
#define SERIALIZE_MAKE_HAS_MEMEBER_TEST(name)																						\
	template<class T,class A>																										\
	struct has_memeber_##name##_impl																								\
	{																																\
		template<class TT,class AA>																									\
		static auto test(int) -> decltype(access::name(std::declval<AA&>(),std::declval<TT&>()), std::true_type());					\
																																	\
		template<class,class>																									\
		static std::false_type test(...);																							\
																																	\
		static const bool value = std::is_same_v<decltype(test<T,A>(0)),std::true_type>;											\
	};																																\
	template<class T,class A>																										\
	struct has_member_##name : std::integral_constant<bool,has_memeber_##name##_impl<T,A>::value> {};
	
#define SERIALIZE_MAKE_HAS_NON_MEMBER_TEST(name)																	\
	template<class T,class A>																						\
	struct has_non_member_##name##_impl																				\
	{																												\
		template<class TT,class AA>																					\
		static auto test(int) -> decltype(name(std::declval<AA&>(), std::declval<std::remove_cv_t<std::remove_reference_t<TT>>&>()), std::true_type());			\
																													\
																													\
		template<class,class>																						\
		static std::false_type test(...);																			\
																													\
		static const bool value = std::is_same_v<decltype(test<T,A>(0)),std::true_type>;							\
	};																												\
	template<class T,class A>																						\
	struct has_non_member_##name : std::integral_constant<bool,has_non_member_##name##_impl<T,A>::value> {};				\
	
	// 定义has_memeber_Serialize、Save、Load
	SERIALIZE_MAKE_HAS_MEMEBER_TEST(Serialize);
	SERIALIZE_MAKE_HAS_MEMEBER_TEST(Save);
	SERIALIZE_MAKE_HAS_MEMEBER_TEST(Load);

	// 定义has_non_memeber_Serialize、Save 、Load
	SERIALIZE_MAKE_HAS_NON_MEMBER_TEST(Serialize);
	SERIALIZE_MAKE_HAS_NON_MEMBER_TEST(Save);
	SERIALIZE_MAKE_HAS_NON_MEMBER_TEST(Load);


	template<class Archive, class First, class ...Other>
	struct is_output_serializable : std::integral_constant<bool, is_output_serializable<Archive, First>::value && is_output_serializable<Archive, Other...>::value> {};

	template<class Archive, class T>
	struct is_output_serializable<Archive,T> :
		std::integral_constant<bool,has_member_Serialize<T,Archive>::value || has_non_member_Serialize<T,Archive>::value || has_non_member_Save<T,Archive>::value>{};

	template<class Archive, class First, class ...Other>
	struct is_input_serializable : std::integral_constant<bool, is_input_serializable<Archive, First>::value && is_input_serializable<Archive, Other...>::value> {};

	template<class Archive, class T>
	struct is_input_serializable<Archive, T> :
		std::integral_constant<bool, has_member_Serialize<T, Archive>::value || has_non_member_Serialize<T, Archive>::value || has_non_member_Load<T, Archive>::value> {};
}
