#include <winunit.h>
#include <vector>
#include <list>
#include "linqcpp.h"

using namespace linq_to_cpp;
using std::vector;
using std::list;

template <class _T>
void printCollection(_T& coll)
{
	for (auto i = coll.begin(); i!= coll.end(); ++i){
		WIN_TRACE("%d ", (*i));
	}
	WIN_TRACE("\n");
}


BEGIN_TEST(Test_where)
{
	vector<int> collection;
	for (int i = 0; i < 10; ++i){
		collection.push_back(i);
	}

	auto res = create_dataset(collection).where([&](int i) -> bool {return i % 2 == 0 ? true : false;});

	printCollection(res);

}
END_TEST

	typedef struct testStr
{
	int val;
	int mul;
	int div;
	testStr(int _val, int _mul, int _div){
		val = _val;
		mul = _mul;
		div = _div;
	}
}testStr;


BEGIN_TEST(decltype)
{
	vector<int> v1;
	decltype (testfunc<double>(v1)) type1;
	type1.push_back(0.00021);
	WIN_ASSERT_EQUAL(0.00021, type1.back());
}
END_TEST

	BEGIN_TEST(Test_select)
{
	vector<int> coll;
	for (int i = 0; i < 10; ++i)
		coll.push_back(i);
	auto res = create_dataset(coll).select<testStr>([] (int i) {return testStr(i, i*2, i/2);});
	for (auto i = res.begin(); i != res.end(); ++i){
		WIN_TRACE("val=%d, mul=%d, div=%d\n", (*i).val, (*i).mul, (*i).div);
	}
}
END_TEST

BEGIN_TEST(Test_select_where)
{
	vector<int> coll;
	for (int i = 0; i < 10; ++i)
		coll.push_back(i);
	auto res = create_dataset(coll)
		.where([](int i)
	{
		return i % 2 == 0 ? true : false;
	})
		.select<testStr>([] (int i) 
	{
		return testStr(i, i*2, i/2);
	});
	for (auto i = res.begin(); i != res.end(); ++i){
		WIN_TRACE("val=%d, mul=%d, div=%d\n", (*i).val, (*i).mul, (*i).div);
	}
}
END_TEST

BEGIN_TEST(Test_with_lists)
{
	list<int> coll;
	for (int i = 0; i < 10; ++i)
		coll.push_back(i);

	auto res = create_dataset(coll).where([](int i) {return i % 2 == 0? true : false;});

	printCollection(res);

}
END_TEST

BEGIN_TEST(Test_orderby)
{
	vector<int> v;
	for (int i = 0; i < 10; ++i)
		v.push_back(i);
	auto res = create_dataset(v).orderby([](int a, int b){ return a > b; });
	printCollection(res);
}
END_TEST

BEGIN_TEST(Test_orderby_with_list)
{
	list<int> v;
	for (int i = 0; i < 10; ++i)
		v.push_back(i);
	auto res = create_dataset(v).orderby([](int a, int b){ return a > b; });
	printCollection(res);
}
END_TEST