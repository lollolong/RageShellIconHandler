//
//	factory/factory.cpp
//

// Project
#include "factory.h"
#include "handler/handler.h"

// C/C++
#include <Shlwapi.h>

extern long g_DllRefCount;

ClassFactory::ClassFactory() : m_RefCount(1)
{
	InterlockedIncrement(&g_DllRefCount);
}

ClassFactory::~ClassFactory()
{
	InterlockedDecrement(&g_DllRefCount);
}

IFACEMETHODIMP ClassFactory::QueryInterface(REFIID riid, void** ppv)
{
	static const QITAB qit[] =
	{
		QITABENT(ClassFactory, IClassFactory),
		{ 0 },
	};
	return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) ClassFactory::AddRef()
{
	return InterlockedIncrement(&m_RefCount);
}

IFACEMETHODIMP_(ULONG) ClassFactory::Release()
{
	ULONG cRef = InterlockedDecrement(&m_RefCount);
	if (cRef == 0)
	{
		delete this;
	}
	return cRef;
}

IFACEMETHODIMP ClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
	HRESULT hr = CLASS_E_NOAGGREGATION;

	// pUnkOuter is used for aggregation. We do not support it in the sample.
	if (pUnkOuter == NULL)
	{
		hr = E_OUTOFMEMORY;

		IconHandler* pExt = new IconHandler();
		if (pExt)
		{
			hr = pExt->QueryInterface(riid, ppv);
			pExt->Release();
		}
	}

	return hr;
}

IFACEMETHODIMP ClassFactory::LockServer(BOOL fLock)
{
	if (fLock)
	{
		InterlockedIncrement(&g_DllRefCount);
	}
	else
	{
		InterlockedDecrement(&g_DllRefCount);
	}
	return S_OK;
}