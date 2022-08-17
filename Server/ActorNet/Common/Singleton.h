#pragma once

#include <memory>

template<typename T>
class Singleton
{
public:
	Singleton() = default;
	virtual ~Singleton() = default;

	Singleton(const Singleton& rhs) = delete;
	Singleton(Singleton&& rhs) = delete;
	Singleton& operator=(const Singleton& rhs) = delete;
	Singleton& operator=(Singleton&& rhs) = delete;

	static std::shared_ptr<T>  GetInstance()
	{
		return instance_;
	}
private:
	static std::shared_ptr<T> instance_;
};

template<typename T>
std::shared_ptr<T> Singleton<T>::instance_ = std::make_shared<T>();
