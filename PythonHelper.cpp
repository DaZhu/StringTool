#include "stdafx.h"
#include "PythonHelper.h"
#include <memory>
#include "StringHelper.h"

PythonHelper::PythonHelper()
{
}


PythonHelper::~PythonHelper()
{
}

void PythonHelper::py_run(const std::wstring& path)
{
	FILE* f = NULL;
	_wfopen_s(&f, path.c_str(), L"rb");
	if (f)
	{
		char* pBuf = new char[2048];
		size_t nCount = fread(pBuf, 1, 2048, f);
		pBuf[nCount] = '\0';

		Py_Initialize();
		int ret = PyRun_SimpleString(pBuf);
		Py_Finalize();
	}
}

void PythonHelper::py_runBuffer(const std::string& buf)
{
	Py_Initialize();
	int ret = PyRun_SimpleString(buf.c_str());
	Py_Finalize();
}

