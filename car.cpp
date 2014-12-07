
#include <iostream>

#include "car.h"
#include "iid.h"

extern ULONG g_lockCount;
extern ULONG g_objCount;

Car::Car() {
    m_refCount = 0;
	m_clientSite = NULL;
	m_OAHolder = NULL;
    g_objCount++;
	::memset(m_pcOwner, 0, 80);
}

Car::~Car() {
	g_objCount--;
	if (this->m_OAHolder != NULL) {
		this->m_OAHolder->Release();
	}
}

STDMETHODIMP_(ULONG) Car::AddRef() {
	return ++m_refCount;
}

STDMETHODIMP_(ULONG) Car::Release() {
	if(--m_refCount == 0) {
		delete this;
		return 0;
	} else {
		return m_refCount;
	}
}

STDMETHODIMP Car::QueryInterface(REFIID riid, void ** ppAny) {
    // IID_IUnknown is the REFIID of standard interface IUnknown
    if(riid == IID_IUnknown) {
		// to avoid confusion caused by virtual inheritance
		*ppAny = (IUnknown *)(IStatus *)this;

    } else if(riid == IID_IRegistration) {
		*ppAny = (IRegistration *)this;

	} else if(riid == IID_IStatus) {
		*ppAny = (IStatus *)this;

	} else if(riid == IID_IOleObject) {
		*ppAny = (IOleObject *)this;

    } else {
		*ppAny = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown *)(*ppAny))->AddRef();
	return S_OK;
}

STDMETHODIMP Car::GetOwner(BSTR * pBstrOwner) {
	mbstowcs(*pBstrOwner, m_pcOwner, 80);
	return S_OK;
}

STDMETHODIMP Car::SetOwner(BSTR bstrOwner) {
	wcstombs(m_pcOwner, bstrOwner, 80);
	return S_OK;
}

STDMETHODIMP Car::GetSpeed(int * pnSpeed) {
	*pnSpeed = m_nSpeed;
	return S_OK;
}

STDMETHODIMP Car::SetSpeed(int nSpeed) {
	m_nSpeed = nSpeed;
	return S_OK;
}

// IOleObject

STDMETHODIMP Car::SetClientSite(LPOLECLIENTSITE pClientSite) {
	if (this->m_clientSite != NULL) {
		this->m_clientSite->Release();
	}
	this->m_clientSite = pClientSite;
	if (this->m_clientSite != NULL) {
		this->m_clientSite->AddRef();
	}
	return S_OK;
}

STDMETHODIMP Car::GetClientSite(LPOLECLIENTSITE *pClientSite) {
	*pClientSite = this->m_clientSite;
	if (this->m_clientSite != NULL) {
		this->m_clientSite->AddRef();
	}
	return S_OK;
}

STDMETHODIMP Car::SetHostNames(LPCOLESTR szContainerApp, LPCOLESTR szContainerObj) {
	std::wcout << szContainerApp << L" - " << szContainerObj << std::endl;
	return S_OK;
}

STDMETHODIMP Car::Close(DWORD dwSaveOption) {
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

STDMETHODIMP Car::SetMoniker(DWORD /*OLEWHICHMK*/ dwWhichMoniker, LPMONIKER pmk) {
	if (this->m_OAHolder != NULL) {
		this->m_OAHolder->SendOnRename(pmk);
	}
	return S_OK;
}

STDMETHODIMP Car::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, LPMONIKER* ppmk) {
	if (this->m_clientSite != NULL) {
		return this->m_clientSite->GetMoniker(dwAssign, dwWhichMoniker, ppmk);
	}
	*ppmk = NULL;
	return E_UNEXPECTED;
}

STDMETHODIMP Car::InitFromData(LPDATAOBJECT pDataObject, BOOL fCreation, DWORD dwReserved) {
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

STDMETHODIMP Car::GetClipboardData(DWORD dwReserved, LPDATAOBJECT * ppDataObject) {
	return E_NOTIMPL;
}

STDMETHODIMP Car::DoVerb(LONG iVerb, LPMSG, LPOLECLIENTSITE, LONG, HWND, LPCRECT) {
	std::cout << "DoVerb " << iVerb << std::endl;
	if (iVerb == 1) {
		// this->m_clientSite->OnShowWindow(TRUE);
		// Open External Editor, link it by TCP channel
		// new thread to receive messages ?
		// OnEditorClose { this->m_clientSite->SaveObject(); this->m_clientSite->OnShowWindow(FALSE); }
	}
	return E_NOTIMPL;
}

STDMETHODIMP Car::EnumVerbs(LPENUMOLEVERB * ppEnum) {
	return OleRegEnumVerbs(CLSID_Car, ppEnum);
}

STDMETHODIMP Car::Update() {
	return S_OK;
}

STDMETHODIMP Car::IsUpToDate() {
	return S_OK;
}

STDMETHODIMP Car::GetUserClassID(LPCLSID pClsid) {
	// *pClsid = CLSID_Car;
	return S_OK;
}

STDMETHODIMP Car::GetUserType(DWORD dwFormOfType, LPOLESTR * pszUserType) {
	return OleRegGetUserType(CLSID_Car, dwFormOfType, pszUserType);
}

STDMETHODIMP Car::SetExtent(DWORD dwDrawAspect, SIZEL * psizel) {
	std::cout << "SetExtent " << dwDrawAspect << " "
			<< psizel->cx << " x " << psizel->cy << std::endl;
	return S_OK;
}

STDMETHODIMP Car::GetExtent(DWORD dwDrawAspect, SIZEL * psizel) {
	std::cout << "GetExtent " << std::endl;
	psizel->cx = 800;
	psizel->cy = 600;
	return S_OK;
}

STDMETHODIMP Car::Advise(LPADVISESINK pAdvSink, PDWORD pdwConnection) {
	if (this->m_OAHolder == NULL) {
		HRESULT r = CreateOleAdviseHolder(&m_OAHolder);
		if (FAILED(r)) {
			return r;
		}
	}
	return this->m_OAHolder->Advise(pAdvSink, pdwConnection);
}

STDMETHODIMP Car::Unadvise(DWORD dwConnection) {
	if (this->m_OAHolder == NULL) {
		return OLE_E_NOCONNECTION;
	}
	return this->m_OAHolder->Unadvise(dwConnection);
}

STDMETHODIMP Car::EnumAdvise(LPENUMSTATDATA * ppenumAdvise) {
	if (this->m_OAHolder == NULL) {
		*ppenumAdvise = NULL;
		return OLE_E_NOCONNECTION;
	}
	return this->m_OAHolder->EnumAdvise(ppenumAdvise);
}

STDMETHODIMP Car::GetMiscStatus(DWORD dwAspect, PDWORD pdwStatus) {
	return OleRegGetMiscStatus(CLSID_Car, dwAspect, pdwStatus);
}

STDMETHODIMP Car::SetColorScheme(LPLOGPALETTE) {
	return E_NOTIMPL;
}

