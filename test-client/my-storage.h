
#ifndef MY_STORAGE_H
#define MY_STORAGE_H

#include <windows.h>

class MyStorage : public IStorage {

public:
	MyStorage();
	virtual ~MyStorage();
	
  STDMETHODIMP QueryInterface( REFIID,PVOID*);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();
  
  STDMETHODIMP CreateStream( LPCWSTR,DWORD,DWORD,DWORD,IStream**);
  STDMETHODIMP OpenStream( LPCWSTR,PVOID,DWORD,DWORD,IStream**);
  STDMETHODIMP CreateStorage( LPCWSTR,DWORD,DWORD,DWORD,IStorage**);
  STDMETHODIMP OpenStorage( LPCWSTR,IStorage*,DWORD,SNB,DWORD,IStorage**);
  STDMETHODIMP CopyTo( DWORD,IID const*,SNB,IStorage*);
  STDMETHODIMP MoveElementTo( LPCWSTR,IStorage*,LPCWSTR,DWORD);
  STDMETHODIMP Commit( DWORD);
  STDMETHODIMP Revert();
  STDMETHODIMP EnumElements( DWORD,PVOID,DWORD,IEnumSTATSTG**);
  STDMETHODIMP DestroyElement( LPCWSTR);
  STDMETHODIMP RenameElement( LPCWSTR,LPCWSTR);
  STDMETHODIMP SetElementTimes( LPCWSTR,FILETIME const*,FILETIME const*,FILETIME const*);
  STDMETHODIMP SetClass( REFCLSID);
  STDMETHODIMP SetStateBits( DWORD,DWORD);
  STDMETHODIMP Stat( STATSTG*,DWORD);
  
private:
	ULONG m_refCount;
};

#endif /* MY_STORAGE_H */