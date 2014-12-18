
#include <iostream>
#include "my-client-site.h"

MyClientSite::MyClientSite(IPersistStorage *pPersistStorage, IStorage *pStorage, IDataObject *pDataObject) {
    m_refCount = 0;
	m_persistStorage = pPersistStorage;
	m_storage = pStorage;
	m_dataObject = pDataObject;
}

MyClientSite::~MyClientSite() {
}

STDMETHODIMP_(ULONG) MyClientSite::AddRef() {
	return ++m_refCount;
}

STDMETHODIMP_(ULONG) MyClientSite::Release() {
	if(--m_refCount == 0) {
		delete this;
		return 0;
	} else {
		return m_refCount;
	}
}

STDMETHODIMP MyClientSite::QueryInterface(REFIID riid, void ** ppAny) {
	LPOLESTR riidString;
	HRESULT r = StringFromIID(riid, &riidString);
	if (FAILED(r)) {
		return r;
	}
	std::wcout << L"MyClientSite::QueryInterface " << riidString << std::endl;
	CoTaskMemFree(riidString);
	
    // IID_IUnknown is the REFIID of standard interface IUnknown
    if(riid == IID_IUnknown) {
		// to avoid confusion caused by virtual inheritance
		*ppAny = (IUnknown *)(IOleClientSite *)this;
		
	} else if(riid == IID_IOleClientSite) {
		*ppAny = (IOleClientSite *)this;

    } else {
		*ppAny = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown *)(*ppAny))->AddRef();
	return S_OK;
}

STDMETHODIMP MyClientSite::SaveObject() {
	std::cout << "MyClientSite::SaveObject " << std::endl;
	m_persistStorage->Save(m_storage, TRUE);
	return S_OK;
}

STDMETHODIMP MyClientSite::GetMoniker(DWORD,DWORD,LPMONIKER*) {
	std::cout << "MyClientSite::GetMoniker " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyClientSite::GetContainer(LPOLECONTAINER*) {
	std::cout << "MyClientSite::GetContainer " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyClientSite::ShowObject() {
	std::cout << "MyClientSite::ShowObject " << std::endl;
	return S_OK;
}

STDMETHODIMP MyClientSite::OnShowWindow(BOOL pShow) {
	std::cout << "MyClientSite::OnShowWindow " << pShow << std::endl;
	if (pShow == FALSE) {
		FORMATETC format;
		STGMEDIUM medium;
		
		format.cfFormat = CF_BITMAP;
		format.ptd = NULL;
		format.dwAspect = DVASPECT_CONTENT;
		format.lindex = -1;
		format.tymed = TYMED_GDI;
		
		std::cout << ">>GetData " << std::endl;
		HRESULT hr = m_dataObject->GetData(&format, &medium);
		std::cout << hr << std::endl;
	}
	return S_OK;
}

STDMETHODIMP MyClientSite::RequestNewObjectLayout() {
	std::cout << "MyClientSite::RequestNewObjectLayout " << std::endl;
	return E_NOTIMPL;
}
