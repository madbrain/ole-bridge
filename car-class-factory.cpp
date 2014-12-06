
#include "car-class-factory.h"
#include "car.h"

extern ULONG g_lockCount;
extern ULONG g_objCount;

CarClassFactory::CarClassFactory() {
	m_refCount = 0;
	g_objCount++;
}

CarClassFactory::~CarClassFactory() {
	g_objCount--;
}

ULONG __stdcall CarClassFactory::AddRef() {
	return ++m_refCount;
}

ULONG __stdcall CarClassFactory::Release() {
	if(--m_refCount == 0) {
		delete this;
		return 0;
	}
	return m_refCount;
}

HRESULT __stdcall CarClassFactory::QueryInterface(REFIID riid, void ** ppAny) {
	// IID_IUnknown is the REFIID of standard interface IUnknown
	if(riid == IID_IUnknown) { 
		*ppAny = (IUnknown *)this; 
	} else if(riid == IID_IClassFactory) { 
		*ppAny = (IClassFactory *)this; 
	} else { 
		*ppAny = NULL; 
		return E_NOINTERFACE; 
	} 
	((IUnknown *)(*ppAny))->AddRef(); 
	return S_OK; 
}

HRESULT __stdcall CarClassFactory::CreateInstance (
		LPUNKNOWN pUnkOuter, REFIID riid, void ** ppAny) {
	if(pUnkOuter != NULL) { 
		return CLASS_E_NOAGGREGATION; 
	} 
	Car * pCar = new Car(); 
	HRESULT hr = pCar->QueryInterface(riid, ppAny); 
	if(FAILED(hr)) {
		delete pCar; 
	}
	return hr; 
} 

HRESULT __stdcall CarClassFactory::LockServer(BOOL fLock) { 
	if(fLock) {
		g_lockCount++;
	} else {
		g_lockCount--;
	}
	return S_OK;
}

