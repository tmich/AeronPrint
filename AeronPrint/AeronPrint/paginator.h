#pragma once
#include <vector>
#include "Order.h"

template<typename T>
class Paginator
{
public:
	Paginator(std::vector<T> data, size_t pageSize);
	~Paginator();

	std::vector<T> Page(int page);
	int Pages();
	int Current();
protected:
	std::vector<T> data_;
	size_t  pageSize_;
	int currPage_;
};


/*** IMPLEMENTATION ***/

template<typename T>
Paginator<T>::Paginator(std::vector<T> data, size_t pageSize) : data_{ data }, pageSize_{ pageSize }, currPage_{ -1 }
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
	std::vector<T>::const_iterator end;

	if (data_.size() < pageSize_)
	{
		return data_;
	}

	if (page == Pages() - 1)
	{
		// richiesta ultima pagina
		end = data_.cend();
	}
	else
	{
		end = start + pageSize_;
	}

	std::vector<T> vec(end - start);
	int i = 0;

	for (; start < end; start++, i++)
	{
		vec.at(i) = *start;
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
	return currPage_;
}
