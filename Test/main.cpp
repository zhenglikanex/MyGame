#include <iostream>
#include "nlohmann/json.hpp"
#include <string_view>
#include <memory>

#include "Framework/serialize/binary_archive.hpp"
#include "Framework/serialize/binary_stream.hpp"

class B
{
	virtual void Print() const = 0;
};

class D : public B
{
	void Print() const override
	{
		std::cout << "D" << std::endl;
	}
};

int main()
{
	std::unique_ptr<B> bp = std::make_unique<D>();
	
	return 0;
}