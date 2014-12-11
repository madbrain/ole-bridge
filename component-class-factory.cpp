
#include "component-class-factory.h"
#include "component.h"

#ifdef MAKE_DLL
extern ULONG g_lockCount;
extern ULONG g_objCount;
#endif

ComponentClassFactory::ComponentClassFactory() {
	m_refCount = 0;
#ifdef MAKE_DLL
	g_objCount++;
#endif
}

ComponentClassFactory::~ComponentClassFactory() {
#ifdef MAKE_DLL
	g_objCount--;
#endif
}

ULONG __stdcall ComponentClassFactory::AddRef() {
	return ++m_refCount;
}

ULONG __stdcall ComponentClassFactory::Release() {
	if(--m_refCount == 0) {
		delete this;
		return 0;
	}
	return m_refCount;
}

HRESULT __stdcall ComponentClassFactory::QueryInterface(REFIID riid, void ** ppAny) {
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

HRESULT __stdcall ComponentClassFactory::CreateInstance (
		LPUNKNOWN pUnkOuter, REFIID riid, void ** ppAny) {
	if(pUnkOuter != NULL) { 
		return CLASS_E_NOAGGREGATION; 
	} 
	Component * pComponent = new Component(); 
	HRESULT hr = pComponent->QueryInterface(riid, ppAny); 
	if(FAILED(hr)) {
		delete pComponent; 
	}
	return hr; 
} 

HRESULT __stdcall ComponentClassFactory::LockServer(BOOL fLock) {
#ifdef MAKE_DLL
	if(fLock) {
		g_lockCount++;
	} else {
		g_lockCount--;
	}
#endif
	return S_OK;
}

