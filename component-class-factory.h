
#ifndef CAR_CLASS_FACTORY_H
#define CAR_CLASS_FACTORY_H

#include <windows.h>

class ComponentClassFactory : public IClassFactory {

public:
	ComponentClassFactory();
	virtual ~ComponentClassFactory();

	HRESULT __stdcall QueryInterface(REFIID riid, void ** ppAny);
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

    HRESULT __stdcall CreateInstance (LPUNKNOWN pUnkOuter, REFIID riid, void ** ppAny);
	HRESULT __stdcall LockServer(BOOL fLock);

private:
	ULONG m_refCount;
};

#endif /* CAR_CLASS_FACTORY_H */
