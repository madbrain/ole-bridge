
#include <iostream>
#include "my-stream.h"

MyStream::MyStream() {
    m_refCount = 0;
}

MyStream::~MyStream() {
}

STDMETHODIMP_(ULONG) MyStream::AddRef() {
	return ++m_refCount;
}

STDMETHODIMP_(ULONG) MyStream::Release() {
	if(--m_refCount == 0) {
		delete this;
		return 0;
	} else {
		return m_refCount;
	}
}

STDMETHODIMP MyStream::QueryInterface(REFIID riid, void ** ppAny) {
	LPOLESTR riidString;
	HRESULT r = StringFromIID(riid, &riidString);
	if (FAILED(r)) {
		return r;
	}
	std::wcout << L"MyStream::QueryInterface " << riidString << std::endl;
	CoTaskMemFree(riidString);
	
    // IID_IUnknown is the REFIID of standard interface IUnknown
    if(riid == IID_IUnknown) {
		// to avoid confusion caused by virtual inheritance
		*ppAny = (IUnknown *)(IStream *)this;
		
	} else if(riid == IID_IStream) {
		*ppAny = (IStream *)this;

    } else {
		*ppAny = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown *)(*ppAny))->AddRef();
	return S_OK;
}

STDMETHODIMP MyStream::Read(void*,ULONG,ULONG*) {
	std::wcout << L"MyStream::Read " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStream::Write(void const * pv, ULONG cb, ULONG * pcbWritten) {
	std::wcout << L"MyStream::Write " << std::endl;
	*pcbWritten = cb;
	for (ULONG i = 0; i < cb; ++i) {
		if ((i % 20) == 0) {
			std::cout << std::endl;
		}
		std::wcout << std::hex << (int)((char *)pv)[i] << " ";
	}
	std::cout << std::endl;
	return S_OK;
}

STDMETHODIMP MyStream::Seek(LARGE_INTEGER,DWORD,ULARGE_INTEGER*) {
	std::wcout << L"MyStream::Seek " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStream::SetSize(ULARGE_INTEGER) {
	std::wcout << L"MyStream::SetSize " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStream::CopyTo(IStream*,ULARGE_INTEGER,ULARGE_INTEGER*,ULARGE_INTEGER*) {
	std::wcout << L"MyStream::CopyTo " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStream::Commit(DWORD) {
	std::wcout << L"MyStream::Commit " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStream::Revert() {
	std::wcout << L"MyStream::Revert " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStream::LockRegion(ULARGE_INTEGER,ULARGE_INTEGER,DWORD) {
	std::wcout << L"MyStream::LockRegion " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStream::UnlockRegion(ULARGE_INTEGER,ULARGE_INTEGER,DWORD) {
	std::wcout << L"MyStream::UnlockRegion " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStream::Stat(STATSTG * pstatstg, DWORD grfStatFlag) {
	std::wcout << L"MyStream::Stat " << std::endl;
	return E_NOTIMPL;
}

STDMETHODIMP MyStream::Clone(LPSTREAM*) {
	std::wcout << L"MyStream::Clone " << std::endl;
	return E_NOTIMPL;
}
