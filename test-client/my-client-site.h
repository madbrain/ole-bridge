
#ifndef MY_CLIENT_SITE_H
#define MY_CLIENT_SITE_H

#include <windows.h>

class MyClientSite : public IOleClientSite {
public:

	MyClientSite(IPersistStorage*, IStorage *, IDataObject*);
	virtual ~MyClientSite();

	STDMETHODIMP QueryInterface(REFIID,PVOID*);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();
  
	STDMETHODIMP SaveObject();
	STDMETHODIMP GetMoniker(DWORD,DWORD,LPMONIKER*);
	STDMETHODIMP GetContainer(LPOLECONTAINER*);
	STDMETHODIMP ShowObject();
	STDMETHODIMP OnShowWindow(BOOL);
	STDMETHODIMP RequestNewObjectLayout();

private:
	ULONG m_refCount;
	IPersistStorage *m_persistStorage;
	IStorage *m_storage;
	IDataObject *m_dataObject;
};

#endif /* MY_CLIENT_SITE_H */