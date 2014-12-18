
#include <iostream>

#include "component.h"
#include "iid.h"

extern void Lock();
extern void UnLock();

Component::Component() {
    m_refCount = 0;
	m_clientSite = NULL;
	m_OAHolder = NULL;
	Lock();
}

Component::~Component() {
	if (this->m_OAHolder != NULL) {
		this->m_OAHolder->Release();
	}
	UnLock();
}

STDMETHODIMP_(ULONG) Component::AddRef() {
	return ++m_refCount;
}

STDMETHODIMP_(ULONG) Component::Release() {
	if(--m_refCount == 0) {
		delete this;
		return 0;
	} else {
		return m_refCount;
	}
}

STDMETHODIMP Component::QueryInterface(REFIID riid, void ** ppAny) {
	LPOLESTR riidString;
	HRESULT r = StringFromIID(riid, &riidString);
	if (FAILED(r)) {
		return r;
	}
	std::wcout << L"QueryInterface " << riidString << std::endl;
	CoTaskMemFree(riidString);
	
    // IID_IUnknown is the REFIID of standard interface IUnknown
    if(riid == IID_IUnknown) {
		// to avoid confusion caused by virtual inheritance
		*ppAny = (IUnknown *)(IOleObject *)this;

	} else if(riid == IID_IOleObject) {
		*ppAny = (IOleObject *)this;
		
	} else if(riid == IID_IPersistStorage) {
		*ppAny = (IPersistStorage *)this;
		
	} else if(riid == IID_IDataObject) {
		*ppAny = (IDataObject *)this;

    } else {
		*ppAny = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown *)(*ppAny))->AddRef();
	return S_OK;
}

// IOleObject

STDMETHODIMP Component::SetClientSite(LPOLECLIENTSITE pClientSite) {
	std::wcout << "SetClientSite " << std::endl;
	if (this->m_clientSite != NULL) {
		this->m_clientSite->Release();
	}
	this->m_clientSite = pClientSite;
	if (this->m_clientSite != NULL) {
		this->m_clientSite->AddRef();
	}
	return S_OK;
}

STDMETHODIMP Component::GetClientSite(LPOLECLIENTSITE *pClientSite) {
	std::wcout << "GetClientSite " << std::endl;
	*pClientSite = this->m_clientSite;
	if (this->m_clientSite != NULL) {
		this->m_clientSite->AddRef();
	}
	return S_OK;
}

STDMETHODIMP Component::SetHostNames(LPCOLESTR szContainerApp, LPCOLESTR szContainerObj) {
	std::wcout << "SetHostNames " << szContainerApp << L" - " << szContainerObj << std::endl;
	return S_OK;
}

STDMETHODIMP Component::Close(DWORD dwSaveOption) {
	std::wcout << "Close " << dwSaveOption << std::endl;
	// in:
	//  OLECLOSE_SAVEIFDIRTY  = 0,
	//  OLECLOSE_NOSAVE       = 1,
	//  OLECLOSE_PROMPTSAVE   = 2

	if (this->m_OAHolder != NULL) {
		this->m_OAHolder->SendOnClose();
	}

	// out:
	//  S_OK
	//  OLE_E_PROMPTSAVECANCELLED -> l'utilisateur choisi d'annuler
	return S_OK;
}

STDMETHODIMP Component::SetMoniker(DWORD /*OLEWHICHMK*/ dwWhichMoniker, LPMONIKER pmk) {
	if (this->m_OAHolder != NULL) {
		this->m_OAHolder->SendOnRename(pmk);
	}
	return S_OK;
}

STDMETHODIMP Component::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, LPMONIKER* ppmk) {
	if (this->m_clientSite != NULL) {
		return this->m_clientSite->GetMoniker(dwAssign, dwWhichMoniker, ppmk);
	}
	*ppmk = NULL;
	return E_UNEXPECTED;
}

STDMETHODIMP Component::InitFromData(LPDATAOBJECT pDataObject, BOOL fCreation, DWORD dwReserved) {
	if (pDataObject == NULL) {
		return S_FALSE;
	}
	IEnumFORMATETC * enumFormatEtc;
	HRESULT r = pDataObject->EnumFormatEtc(DATADIR_GET, &enumFormatEtc);
	if (FAILED(r)) {
		return S_FALSE;
	}
	std::cout << "InitFromData" << std::endl;
	while (1) {
		FORMATETC rgelt[10];
		ULONG len = 0;
		r = enumFormatEtc->Next(10, rgelt, &len);
		if (FAILED(r)) {
			break;
		}
		for (ULONG i = 0; i < len; ++i) {
			std::cout << " " << rgelt[i].cfFormat << std::endl;
		}
	}
	enumFormatEtc->Release();
	return E_NOTIMPL;
}

STDMETHODIMP Component::GetClipboardData(DWORD dwReserved, LPDATAOBJECT * ppDataObject) {
	return E_NOTIMPL;
}

STDMETHODIMP Component::DoVerb(LONG iVerb, LPMSG, LPOLECLIENTSITE, LONG, HWND, LPCRECT) {
	std::cout << "DoVerb " << iVerb << std::endl;
	if (iVerb == OLEIVERB_SHOW) {
		// To perform the registration, call IOleClientSite::GetMoniker to get the full moniker of the object,
		// call the GetRunningObjectTable function to get a pointer to the ROT, and then call IRunningObjectTable::Register.
		this->m_clientSite->OnShowWindow(TRUE);
		return S_OK;
	}
	if (iVerb == 1) {
		// this->m_clientSite->OnShowWindow(TRUE);
		// Open External Editor, link it by TCP channel
		// new thread to receive messages ?
		// OnEditorClose { this->m_clientSite->SaveObject(); this->m_clientSite->OnShowWindow(FALSE); }
	}
	return OLEOBJ_S_INVALIDVERB;
}

STDMETHODIMP Component::EnumVerbs(LPENUMOLEVERB * ppEnum) {
	return OleRegEnumVerbs(CLSID_Component, ppEnum);
}

STDMETHODIMP Component::Update() {
	return S_OK;
}

STDMETHODIMP Component::IsUpToDate() {
	return S_OK;
}

STDMETHODIMP Component::GetUserClassID(LPCLSID pClsid) {
	// *pClsid = CLSID_Component;
	return S_OK;
}

STDMETHODIMP Component::GetUserType(DWORD dwFormOfType, LPOLESTR * pszUserType) {
	return OleRegGetUserType(CLSID_Component, dwFormOfType, pszUserType);
}

STDMETHODIMP Component::SetExtent(DWORD dwDrawAspect, SIZEL * psizel) {
	std::cout << "SetExtent " << dwDrawAspect << " "
			<< psizel->cx << " x " << psizel->cy << std::endl;
	return S_OK;
}

STDMETHODIMP Component::GetExtent(DWORD dwDrawAspect, SIZEL * psizel) {
	std::cout << "GetExtent " << std::endl;
	psizel->cx = 800;
	psizel->cy = 600;
	return S_OK;
}

STDMETHODIMP Component::Advise(LPADVISESINK pAdvSink, PDWORD pdwConnection) {
	if (this->m_OAHolder == NULL) {
		HRESULT r = CreateOleAdviseHolder(&m_OAHolder);
		if (FAILED(r)) {
			return r;
		}
	}
	return this->m_OAHolder->Advise(pAdvSink, pdwConnection);
}

STDMETHODIMP Component::Unadvise(DWORD dwConnection) {
	if (this->m_OAHolder == NULL) {
		return OLE_E_NOCONNECTION;
	}
	return this->m_OAHolder->Unadvise(dwConnection);
}

STDMETHODIMP Component::EnumAdvise(LPENUMSTATDATA * ppenumAdvise) {
	if (this->m_OAHolder == NULL) {
		*ppenumAdvise = NULL;
		return OLE_E_NOCONNECTION;
	}
	return this->m_OAHolder->EnumAdvise(ppenumAdvise);
}

STDMETHODIMP Component::GetMiscStatus(DWORD dwAspect, PDWORD pdwStatus) {
	return OleRegGetMiscStatus(CLSID_Component, dwAspect, pdwStatus);
}

STDMETHODIMP Component::SetColorScheme(LPLOGPALETTE) {
	return E_NOTIMPL;
}

// IPersistObject

STDMETHODIMP Component::GetClassID(CLSID*) {
	std::wcout << "GetClassID " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP Component::IsDirty() {
	std::wcout << "IsDirty " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP Component::InitNew(LPSTORAGE pStorage) {
	std::wcout << "InitNew " << std::endl;
	
	STATSTG stat;
	pStorage->Stat(&stat, STATFLAG_DEFAULT);
	std::wcout << L"- pwcsName: " << stat.pwcsName << std::endl;
	std::wcout << L"- type: " << stat.type << std::endl;
	
	return S_OK;
}

STDMETHODIMP Component::Load(LPSTORAGE) {
	std::wcout << "Load " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP Component::Save(LPSTORAGE,BOOL) {
	std::wcout << "Save " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP Component::SaveCompleted(LPSTORAGE) {
	std::wcout << "SaveCompleted " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP Component::HandsOffStorage() {
	std::wcout << "HandsOffStorage " << std::endl;
	return E_NOTIMPL;
}

// IDataObject
STDMETHODIMP Component::GetData(FORMATETC*,STGMEDIUM*) {
	std::wcout << "GetData " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP Component::GetDataHere(FORMATETC*,STGMEDIUM*) {
	std::wcout << "GetDataHere " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP Component::QueryGetData(FORMATETC * pformatetc) {
	std::wcout << "QueryGetData"
		<< " cfFormat=" << pformatetc->cfFormat
		<< " ptd=" << pformatetc->ptd
		<< " dwAspect=" << pformatetc->dwAspect
		<< " lindex=" << pformatetc->lindex
		<< " tymed=" << pformatetc->tymed
		<< std::endl;
		
	return DV_E_FORMATETC;
}

STDMETHODIMP Component::GetCanonicalFormatEtc(FORMATETC * pformatectIn, FORMATETC * pformatetcOut) {
	std::wcout << "GetCanonicalFormatEtc "
		<< " cfFormat=" << pformatectIn->cfFormat
		<< " ptd=" << pformatectIn->ptd
		<< " dwAspect=" << pformatectIn->dwAspect
		<< " lindex=" << pformatectIn->lindex
		<< " tymed=" << pformatectIn->tymed
		<< std::endl;
	pformatetcOut->cfFormat = CF_BITMAP;
	pformatetcOut->dwAspect = DVASPECT_CONTENT;
	pformatetcOut->tymed = TYMED_GDI;
	return S_OK;
}

STDMETHODIMP Component::SetData(FORMATETC*,STGMEDIUM*,BOOL) {
	std::wcout << "SetData " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP Component::EnumFormatEtc(DWORD,IEnumFORMATETC**) {
	std::wcout << "EnumFormatEtc " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP Component::DAdvise(FORMATETC * pformatetc, DWORD advf, IAdviseSink * pAdvSink, PDWORD pdwConnection) {
	if (this->m_ODAHolder == NULL) {
		HRESULT r = CreateDataAdviseHolder(&m_ODAHolder);
		if (FAILED(r)) {
			return r;
		}
	}
	return this->m_ODAHolder->Advise(this, pformatetc, advf, pAdvSink, pdwConnection);
}

STDMETHODIMP Component::DUnadvise(DWORD dwConnection) {
	if (this->m_ODAHolder == NULL) {
		return OLE_E_NOCONNECTION;
	}
	return this->m_ODAHolder->Unadvise(dwConnection);
}

STDMETHODIMP Component::EnumDAdvise(IEnumSTATDATA ** ppenumAdvise) {
	if (this->m_ODAHolder == NULL) {
		*ppenumAdvise = NULL;
		return OLE_E_NOCONNECTION;
	}
	return this->m_ODAHolder->EnumAdvise(ppenumAdvise);
}
