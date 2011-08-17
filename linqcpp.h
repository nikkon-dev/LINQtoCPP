#pragma once
#ifndef __LINQTOCPP_HEADER__
#define __LINQTOCPP_HEADER__

#include <functional>
#include <memory>
#include <vector>
#include <algorithm>
#include <iterator>

namespace linq_to_cpp
{
	using std::vector;
	using std::allocator;
	
	
	template <class _C, class _T, template <class, class> class _U>
	_U<_C, allocator<_T>> testfunc(_U<_T, allocator<_T>> &obj){};

	template <class _C, class _T, template <class> class _U>
	_U<_C> testfunc(_U<_T> &obj){};


	template <class _T, class container_type = vector<_T>>
	class DataSet
	{
		typedef DataSet<_T, container_type> dataset_type;
	private:
		container_type _internal_container;

		template <class _Cnt, class _Pr>
		void _internal_sort(_Cnt& cnt, _Pr pr)
		{
			std::sort(cnt.begin(), cnt.end(), pr);
		}

		template <class _Pr>
		void _internal_sort(std::list<_T>& cnt, _Pr pr)
		{
			cnt.sort(pr);
		}

		template <class _Obj>
		void _internal_reserve(_Obj &cnt, size_t size)
		{
			cnt.reserve(size);
		}
		template <>
		void _internal_reserve(std::list<_T>& cnt, size_t size)
		{
			//do nothing
		}

	public:
		DataSet(const container_type &ct) : _internal_container(ct)
		{
		}

		DataSet(container_type&& ct)
		{
			_internal_container = std::move(ct);
		}
		DataSet(DataSet&& ds)
		{
			_internal_container = std::move(ds._internal_container);
		}

		typename container_type::const_iterator begin() 
		{
			return _internal_container.begin();
		}

		typename container_type::const_iterator end() {
			return _internal_container.end();
		}

		dataset_type where(std::function<bool (const _T&)> pred)
		{
			container_type tmpCnt;
			_internal_reserve(tmpCnt, _internal_container.size());
			auto Iter = std::back_inserter(tmpCnt);
			std::for_each(_internal_container.begin(), _internal_container.end(), [=,&Iter](const _T& i){
				if (pred(i))
					*Iter++ = i;
			});
			return create_dataset(tmpCnt);
		}

		dataset_type orderby(std::function<bool (const _T&, const _T&)> pred)
		{
			container_type tmpCnt;
			tmpCnt = _internal_container;
			_internal_sort(tmpCnt, pred);
			return create_dataset(tmpCnt);
		}

		template <class _U>
		DataSet<_U, decltype (testfunc<_U>(container_type()))> 
			select(std::function<_U (const _T&)> pred)
		{
			typedef decltype (testfunc<_U>(container_type())) tmpcont_type;
			tmpcont_type tmpCont;
			_internal_reserve(tmpCont, _internal_container.size());

			auto Iter = std::back_inserter(tmpCont);
			std::for_each(_internal_container.begin(), _internal_container.end(), [=,&Iter](const _T& i){
				*Iter++ = pred(i);
			});

			return DataSet<_U, decltype (testfunc<_U>(container_type()))>(tmpCont);
		}
	};

	template <class _C>
	DataSet<typename _C::value_type, _C> create_dataset(const _C& v)
	{
		return DataSet<typename _C::value_type, _C>(v);
	}

} //namespace linq_to_cpp

#endif //__LINQTOCPP_HEADER__