#include <iostream>

#include "Kanex.hpp"

class Test
{
	friend class kanex::Access;
	void Save(const kanex::BinaryOutputArchive& ar) const
	{

	}

	
	int a;
};


int main()
{
	kanex::Buffer buffer(100);
	kanex::BinaryStream stream(buffer);
	kanex::BinaryOutputArchive oar(stream);

	kanex::has_member_Save<Test,kanex::BinaryOutputArchive>::value;

	Test t;
	oar(t);

	int a;
	

	system("pause");
	return 0;
}