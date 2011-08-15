#pragma once
#ifndef __LINQTOCPP_HEADER__
#define __LINQTOCPP_HEADER__

#include <functional>
#include <memory>
#include <vector>

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
	private:
		container_type _internal_container;
	public:
		DataSet(const container_type &ct)
		{
			_internal_container.assign(ct.begin(), ct.end());
		}

		typename container_type::const_iterator begin() 
		{
			return _internal_container.begin();
		}

		typename container_type::const_iterator end() {
			return _internal_container.end();
		}

		DataSet<_T, container_type> where(std::function<bool (const _T&)> pred)
		{
			container_type tmpCnt;
			for (auto i = _internal_container.begin(); i != _internal_container.end(); ++i)
			{
				if (pred(*i))
					tmpCnt.push_back(*i);
			}

			return DataSet<_T, container_type>(tmpCnt);
		}

		template <class _U>
		DataSet<_U, decltype (testfunc<_U>(container_type()))> 
			select(std::function<_U (const _T&)> pred)
		{
			typedef decltype (testfunc<_U>(container_type())) tmpcont_type;
			tmpcont_type tmpCont;
			for (auto i = _internal_container.begin(); i!=_internal_container.end(); ++i){
				tmpCont.push_back(pred(*i));
			}
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