
#include <iostream>
#include "my-storage.h"
#include "my-stream.h"

MyStorage::MyStorage() {
    m_refCount = 0;
}

MyStorage::~MyStorage() {
}

STDMETHODIMP_(ULONG) MyStorage::AddRef() {
	return ++m_refCount;
}

STDMETHODIMP_(ULONG) MyStorage::Release() {
	if(--m_refCount == 0) {
		delete this;
		return 0;
	} else {
		return m_refCount;
	}
}

STDMETHODIMP MyStorage::QueryInterface(REFIID riid, void ** ppAny) {
	LPOLESTR riidString;
	HRESULT r = StringFromIID(riid, &riidString);
	if (FAILED(r)) {
		return r;
	}
	std::wcout << L"MyStorage::QueryInterface " << riidString << std::endl;
	CoTaskMemFree(riidString);
	
    // IID_IUnknown is the REFIID of standard interface IUnknown
    if(riid == IID_IUnknown) {
		// to avoid confusion caused by virtual inheritance
		*ppAny = (IUnknown *)(IStorage *)this;
		
	} else if(riid == IID_IStorage) {
		*ppAny = (IStorage *)this;

    } else {
		*ppAny = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown *)(*ppAny))->AddRef();
	return S_OK;
}

STDMETHODIMP MyStorage::CreateStream(LPCWSTR pwcsName, DWORD grfMode, DWORD reserved1, DWORD reserved2, IStream ** ppstm) {
	std::wcout << "MyStorage::CreateStream '" << pwcsName << "' mode=" << std::hex << grfMode << std::endl;
	*ppstm = new MyStream();
	return S_OK;
}

STDMETHODIMP MyStorage::OpenStream( LPCWSTR,PVOID,DWORD,DWORD,IStream**) {
	std::cout << "MyStorage::OpenStream " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStorage::CreateStorage( LPCWSTR,DWORD,DWORD,DWORD,IStorage**) {
	std::cout << "MyStorage::CreateStorage " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStorage::OpenStorage( LPCWSTR,IStorage*,DWORD,SNB,DWORD,IStorage**) {
	std::cout << "MyStorage::OpenStorage " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStorage::CopyTo( DWORD,IID const*,SNB,IStorage*) {
	std::cout << "MyStorage::CopyTo " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStorage::MoveElementTo( LPCWSTR,IStorage*,LPCWSTR,DWORD) {
	std::cout << "MyStorage::MoveElementTo " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStorage::Commit( DWORD) {
	std::cout << "MyStorage::Commit " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStorage::Revert() {
	std::cout << "MyStorage::Revert " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStorage::EnumElements( DWORD,PVOID,DWORD,IEnumSTATSTG**) {
	std::cout << "MyStorage::EnumElements " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStorage::DestroyElement( LPCWSTR) {
	std::cout << "MyStorage::DestroyElement " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStorage::RenameElement( LPCWSTR,LPCWSTR) {
	std::cout << "MyStorage::RenameElement " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStorage::SetElementTimes( LPCWSTR,FILETIME const*,FILETIME const*,FILETIME const*) {
	std::cout << "MyStorage::SetElementTimes " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStorage::SetClass(REFCLSID clsid) {
	LPOLESTR clsidString;
	HRESULT r = StringFromIID(clsid, &clsidString);
	if (FAILED(r)) {
		return r;
	}
	std::wcout << "MyStorage::SetClass " << clsidString << std::endl;
	CoTaskMemFree(clsidString);
	return S_OK;
}

STDMETHODIMP MyStorage::SetStateBits( DWORD,DWORD) {
	std::cout << "MyStorage::SetStateBits " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStorage::Stat(STATSTG * pstatstg, DWORD grfStatFlag) {
	std::cout << "MyStorage::Stat " << grfStatFlag << std::endl;
	pstatstg->pwcsName = L"Test";
	pstatstg->type = STGTY_STORAGE;
	pstatstg->cbSize.QuadPart = 0L;
	pstatstg->clsid = CLSID_NULL;

	//std::wcout << L"- cbSize: " << pstatstg->cbSize << std::endl;
	//std::wcout << L"- mtime: " << pstatstg->mtime << std::endl;
	//std::wcout << L"- ctime: " << pstatstg->ctime << std::endl;
	//std::wcout << L"- atime: " << pstatstg->atime << std::endl;

//  DWORD          grfMode;
//  DWORD          grfLocksSupported;
//  DWORD          grfStateBits;
//  DWORD          reserved;
	return S_OK;
}
