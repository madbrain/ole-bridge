
#include "component-class-factory.h"
#include "iid.h"

#ifdef MAKE_DLL

ULONG g_lockCount = 0;
ULONG g_objCount = 0;

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void ** ppAny) {
	if(rclsid != CLSID_Car) {
		return CLASS_E_CLASSNOTAVAILABLE;
	}
	ComponentClassFactory * pFactory = new ComponentClassFactory;
	HRESULT hr = pFactory->QueryInterface(riid, ppAny);
	if(FAILED(hr)) {
		delete pFactory;
	}
	return hr;
}

STDAPI DllCanUnloadNow() {
	if(g_lockCount == 0 && g_objCount == 0) {
		return S_OK;
	} else {
		return S_FALSE;
	}
}
#else

DWORD g_allLocks;
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow) {

    CoInitialize(NULL);
	
#if 0
    // register the type lib
	ITypeLib* pTLib = NULL;
	LoadTypeLibEx(L"AnyEXETypeInfo.tlb", REGKIND_REGISTER, &pTLib);
	pTLib->Release();
#endif

	if(strstr(lpCmdLine, "/Embedding") || strstr(lpCmdLine, "-Embedding")) {
        ComponentClassFactory cf;
		DWORD regID = 0;
		CoRegisterClassObject(CLSID_Component, (IClassFactory*)&cf, CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE, &regID);
		MSG ms;
		while(GetMessage(&ms, 0, 0, 0)) {
			TranslateMessage(&ms);
			DispatchMessage(&ms);
		}
		CoRevokeClassObject(regID);
    }
	CoUninitialize();   
	return 0;
}

void Lock() {
    ++g_allLocks;
}

void UnLock() {
	--g_allLocks;
	if(g_allLocks == 0) {
        PostQuitMessage(0);
	}
}

#endif /* MAKE_DLL */