#include "paginator.h"


template<typename T>
Paginator<T>::Paginator(std::vector<T> data, int pageSize) : data_{data}, pageSize_{pageSize}, currPage_{-1}
{
	
}

template<typename T>
Paginator<T>::~Paginator()
{
}

template<typename T>
std::vector<T> Paginator<T>::Page(int page)
{
	currPage_ = page;
	
	auto start = data_.cbegin() + (pageSize_ * page);
	auto end = start + pageSize_;

	std::vector<T> vec(pageSize_);

	for (; start < end; start++)
	{
		vec.push_back(*start);
	}

	return vec;
}

template<typename T>
int Paginator<T>::Pages()
{
	int n1 = data_.size() / pageSize_;
	int rx = data_.size() % pageSize_;
	if (rx > 0)
	{
		n1++;
	}

	return n1;
}

template<typename T>
int Paginator<T>::Current()
{
	return currPage_ + 1;
}
