
#include "car-class-factory.h"
#include "iid.h"

// Voir "HTML Document" {25336920-03F9-11CF-8FD0-00AA00686F13}

ULONG g_lockCount = 0;
ULONG g_objCount = 0;

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void ** ppAny) {
	if(rclsid != CLSID_Car) {
		return CLASS_E_CLASSNOTAVAILABLE;
	}
	CarClassFactory * pFactory = new CarClassFactory;
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
