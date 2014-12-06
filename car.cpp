
#include "car.h"
#include "iid.h"

extern ULONG g_lockCount;
extern ULONG g_objCount;

Car::Car() {
    m_refCount = 0;
    g_objCount++;
	::memset(m_pcOwner, 0, 80);
}

Car::~Car() {
	g_objCount--;
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

STDMETHODIMP Car::SetHostNames(LPCOLESTR,LPCOLESTR) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::Close(DWORD) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::SetMoniker(DWORD,LPMONIKER) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::GetMoniker(DWORD,DWORD,LPMONIKER*) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::InitFromData(LPDATAOBJECT,BOOL,DWORD) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::GetClipboardData(DWORD,LPDATAOBJECT*) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::DoVerb(LONG,LPMSG,LPOLECLIENTSITE,LONG,HWND,LPCRECT) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::EnumVerbs(LPENUMOLEVERB*) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::Update() {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::IsUpToDate() {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::GetUserClassID(LPCLSID) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::GetUserType(DWORD,LPOLESTR*) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::SetExtent(DWORD,SIZEL*) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::GetExtent(DWORD,SIZEL*) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::Advise(LPADVISESINK,PDWORD) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::Unadvise(DWORD) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::EnumAdvise(LPENUMSTATDATA*) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::GetMiscStatus(DWORD,PDWORD) {
	return E_UNEXPECTED;
}

STDMETHODIMP Car::SetColorScheme(LPLOGPALETTE) {
	return E_UNEXPECTED;
}

