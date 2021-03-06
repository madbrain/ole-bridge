
#ifndef MY_STREAM_H
#define MY_STREAM_H

#include <windows.h>

class MyStream : public IStream {

public:
	MyStream();
	virtual ~MyStream();
	
	STDMETHODIMP QueryInterface(REFIID, PVOID*);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
	
	STDMETHODIMP Read(void*,ULONG,ULONG*);
	STDMETHODIMP Write(void const*,ULONG,ULONG*);
	STDMETHODIMP Seek(LARGE_INTEGER,DWORD,ULARGE_INTEGER*);
	STDMETHODIMP SetSize(ULARGE_INTEGER);
	STDMETHODIMP CopyTo(IStream*,ULARGE_INTEGER,ULARGE_INTEGER*,ULARGE_INTEGER*);
	STDMETHODIMP Commit(DWORD);
	STDMETHODIMP Revert();
	STDMETHODIMP LockRegion(ULARGE_INTEGER,ULARGE_INTEGER,DWORD);
	STDMETHODIMP UnlockRegion(ULARGE_INTEGER,ULARGE_INTEGER,DWORD);
	STDMETHODIMP Stat(STATSTG*,DWORD);
	STDMETHODIMP Clone(LPSTREAM*);
  
private:
	ULONG m_refCount;
};

#endif /* MY_STREAM_H */