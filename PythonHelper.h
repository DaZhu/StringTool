#ifndef __PYTHONHELPER__
#define __PYTHONHELPER__

#include <Python.h>
#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <memory>
#include "StringHelper.h"
#pragma comment(lib, "python34.lib")

struct PyObjectDel
{
	void operator()(PyObject* ptr) const
	{
		Py_DECREF(ptr);
	}
};

template<typename ...Args>
decltype(std::get<0>(obj)) getElement(size_t i, std::tuple<Args...>& obj)
{
	switch (i)
	{
	case 0: return std::get<0>(obj);
	case 1: return std::get<1>(obj);
	case 2: return std::get<2>(obj);
	case 3: return std::get<3>(obj);
	case 4: return std::get<4>(obj);
	case 5: return std::get<5>(obj);
	case 6: return std::get<6>(obj);
	case 7: return std::get<7>(obj);
	case 8: return std::get<8>(obj);
	case 9: return std::get<9>(obj);
	}
}

typedef std::unique_ptr<PyObject, PyObjectDel> phPyObject;

class PythonHelper
{
public:
	static PythonHelper& GetInstance() { static PythonHelper s; return s; }
	PythonHelper();

	~PythonHelper();

public:
	void py_run(const std::wstring& path);
	void py_runBuffer(const std::string& buf);


	template< typename ...Args>
	void py_call(const std::wstring& path, const std::string& fName, Args ... args)
	{
		PyObject* o = PyUnicode_FromString(StringHelper::UnicodeToAnsi(path).c_str());
		PyObject* pModule = PyImport_Import(o);
		Py_DECREF(o);
		if (!pModule)
		{
			PyObject* oFun = PyObject_GetAttrString(pModule, fName.c_str());
			if (oFun && PyCallable_Check(oFun))
			{
				std::tuple<Args...> parm(args...);
				PyObject* pRet = PyObject_CallObject(oFun, PyobjectFrom(parm).get());
				if (pRet)
				{
					int n = 0;
				}
			}
		}
	}

private:
	phPyObject PyobjectFrom(long obj)
	{
		return phPyObject(PyLong_FromLong(obj));
	}

	phPyObject PyobjectFrom(std::string& obj)
	{
		return phPyObject(PyUnicode_FromString(obj.c_str()));
	}

	phPyObject PyobjectFrom(bool obj)
	{
		return phPyObject(PyBool_FromLong(obj ? 1 : 0));
	}

	template< typename ...Args>
	phPyObject PyobjectFrom(std::tuple<Args...>& obj)
	{
		size_t nSize = std::tuple_size<std::tuple<Args...>>::value;
		PyObject* pTuple = PyTuple_New(nSize);
		for (size_t i = 0; i < nSize; i++) 
		{
			
			PyTuple_SetItem(pTuple, i, PyObjectFrom(getElement(i, obj)));
		}
		return phPyObject(pTuple);
	}

	template<typename k, typename v>
	phPyObject PyobjectFrom(std::map<k, v>& obj)
	{
		PyObject* pDict = PyDict_New();
		for (auto it = obj.begin(); it != obj.end(); it++)
		{
			phPyObject k = PyObjectFrom(it->first);
			phPyObject v = PyObjectFrom(it->sencond);
			PyDict_SetItem(pDict, k.get(), v.get());
		}
		return phPyObject(pDict);
	}

	template<typename t>
	phPyObject PyObjectFrom(std::vector<t>& obj)
	{
		PyObject* pList = PyList_New(obj.size());
		for (int it = 0; it < obj.size(); it++)
		{
			PyList_SetItem(pList, it, PyObjectFrom(obj[it]).get());
		}
		return phPyObject(pList);
	}


};

#endif //__PYTHONHELPER__