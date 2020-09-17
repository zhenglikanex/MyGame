#include <iostream>
#include <sstream>
#include <type_traits>
#include "serialize/internal/types.hpp"
#include "serialize/types/vector.hpp"
#include "serialize/types/string.hpp"
#include "serialize/types/pair.hpp"
#include "serialize/types/map.hpp"
#include "binary_Archive.hpp"
#include "traits.hpp"

#include <map>
#include <set>
#include <unordered_map>
template<class T>
auto Test(T&& data) -> decltype(std::forward<T>(data))
{
	std::cout << std::is_same_v<T, int> << std::endl;
	std::cout << std::is_same_v<T, int&> << std::endl;
	return std::forward<T>(data);
}

enum MyEnum
{
	type1,
	type2
};
class c
{
public:
	c(int b) { a = b; }
	int a;
};

int main()
{
	serialize::BinaryStream stream;
	serialize::BinaryOutputArchive oar(stream);

	int x = 10;
	int& x1 = x;
	int arr[] = { 1,2,3 };
	
	int* p = &x;
	//oar(p);

	std::vector<c> vee;

	std::vector<int> vec{ 1,2,3 };
	std::vector<bool> bvec{ true,true,false };
	std::string s = "123";
	const std::wstring ws = L"ÄãºÃ";
	const std::pair<int, int> pair = { 10,10 };
	std::multimap<int, int> mmap;
	MyEnum myenum = MyEnum::type1;

	mmap.emplace(10, 1);
	mmap.emplace(10, 2);
	mmap.emplace(10, 3);
	mmap.emplace(10, 1);
	mmap.emplace(1, 2);
	oar(x,vec);
	oar(bvec);
	oar(s);
	oar(ws);
	oar(pair);
	oar(mmap);
	oar(myenum);

	serialize::BinaryInputArchive iar(stream);

	std::vector<int> copy_vec;
	std::vector<bool> cbvec;
	std::string cs;
	std::wstring cws;
	std::pair<int, int> cpair;
	std::multimap<int, int> cmmap;
	MyEnum cenum = MyEnum::type2;
	cenum = MyEnum::type1;
	int v;
	iar(v);
	iar(copy_vec);
	iar(cbvec);
	iar(cs);
	iar(cws);
	iar(cpair);
	iar(cmmap);
	iar(cenum);

	

	//serialize::has_non_member_Serialize<std::in>
	/*auto item = mmap.find(10);
	std::set<int> set;
	for (auto& v : mmap)
	{

		std::cout << v.first << " " << v.second << std::endl;
	}
	std::unordered_map<int, int> m;
	std::map<int, int> m2;
	*/
	//m.emplace_hint()

	//func<int, int, std::map<int, int>::key_compare, std::allocator<std::pair<const int, int>>,bool>();

	//decltype(x1) var;
	/*auto& arr1 = arr;
	decltype(serialize::Save(std::declval<serialize::BinaryOutputArchive&>(), v11), std::true_type()) b = std::true_type();*/
	/*int* p = &v;
	oar(p);

	serialize::BinaryInputArchive iar(stream);*/
	//int v;
	//iar(v);
	//std::is_same_v<decltype(, std::true_type>
	//decltype(serialize::Save(std::declval<HAS::AType&>(), std::declval<HAS::TType&>()), std::true_type()) b = std::true_type();

	//std::cout << serialize::has_non_member_Save<int, serialize::BinaryOutputArchive>::value << std::endl;
	

	
	//Stream stream;
	//OutputArchive ar;
	//ar(x,...)

	

	system("pause");
	return 0;
}