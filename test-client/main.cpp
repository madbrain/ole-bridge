
#include <iostream>
#include <windows.h>

#include "my-storage.h"
#include "my-client-site.h"
#include "iid.h"

int main(int argc, char* argv[]) {

	//std::cout << "DV_E_LINDEX " << std::hex << DV_E_LINDEX << std::endl;
	//std::cout << "DV_E_FORMATETC " << std::hex << DV_E_FORMATETC << std::endl;
	//std::cout << "DV_E_TYMED " << std::hex << DV_E_TYMED << std::endl;
	//std::cout << "DV_E_CLIPFORMAT " << std::hex << DV_E_CLIPFORMAT << std::endl;
	//std::cout << "DV_E_DVASPECT " << std::hex << DV_E_DVASPECT << std::endl;
	//std::cout << "OLE_E_NOTRUNNING " << std::hex << OLE_E_NOTRUNNING << std::endl;
	//std::cout << "STG_E_MEDIUMFULL " << std::hex << STG_E_MEDIUMFULL << std::endl;
	
	//exit(1);

	CoInitialize(NULL);

	std::cout << "Creation instance" << std::endl;
	IOleObject * pOleObject;
	HRESULT hr = CoCreateInstance(CLSID_Marvin, NULL, CLSCTX_SERVER, IID_IOleObject, (void **) &pOleObject);

	if(FAILED(hr)) {
		std::cout << "CoCreateInstance failed!" << std::endl;
		return 1;
	} 

	std::cout << "cast en IPersistStorage" << std::endl;
	IPersistStorage * pPersistStorage;
    hr = pOleObject->QueryInterface(IID_IPersistStorage, (void**)&pPersistStorage);

	if(FAILED(hr)) {
		std::cout << "QueryInterface IPersistantStorage failed!" << std::endl;
		pOleObject->Release();
		return 1;
	}
	
	std::cout << "cast en IDataObject" << std::endl;
	IDataObject * pDataObject;
    hr = pOleObject->QueryInterface(IID_IDataObject, (void**)&pDataObject);
	
	if(FAILED(hr)) {
		std::cout << "QueryInterface IDataObject failed!" << std::endl;
		pPersistStorage->Release();
		pOleObject->Release();
		return 1;
	}
	
	std::cout << ">>InitNew" << std::endl;
	IStorage * pStorage = new MyStorage();
	hr = pPersistStorage->InitNew(pStorage);
	std::cout << hr << std::endl;
	
	std::cout << ">>SetClientSite" << std::endl;
	IOleClientSite * pClientSite = new MyClientSite(pPersistStorage, pStorage, pDataObject);
	hr = pOleObject->SetClientSite(pClientSite);
	std::cout << hr << std::endl;

	std::cout << ">>SetHostNames" << std::endl;
	hr = pOleObject->SetHostNames(L"Microsoft Word", L"Document1");
	std::cout << hr << std::endl;
	
	std::cout << ">>QueryGetData" << std::endl;
	FORMATETC format;
	format.cfFormat = 14;
	format.ptd = NULL;
	format.dwAspect = 1;
	format.lindex = -1;
	format.tymed = 64;
	hr = pDataObject->QueryGetData(&format);
	std::cout << std::hex << hr << std::endl;
	
	FORMATETC informat, outformat;
	std::cout << ">>GetCanonicalFormatEtc" << std::endl;
	informat.cfFormat = 3;
	informat.ptd = NULL; // not null
	informat.dwAspect = 1;
	informat.lindex = -1;
	informat.tymed = 32;
	hr = pDataObject->GetCanonicalFormatEtc(&informat, &outformat);
	std::cout << std::hex << hr << std::endl;
	std::cout << "->cfFormat " << outformat.cfFormat << std::endl;
	std::cout << "->dwAspect " << outformat.dwAspect << std::endl;
	std::cout << "->tymed " << outformat.tymed << std::endl;
	
	std::cout << ">>GetCanonicalFormatEtc" << std::endl;
	informat.cfFormat = CF_BITMAP;
	informat.ptd = NULL; // not null
	informat.dwAspect = DVASPECT_CONTENT;
	informat.lindex = -1;
	informat.tymed = TYMED_GDI;
	hr = pDataObject->GetCanonicalFormatEtc(&informat, &outformat);
	std::cout << std::hex << hr << std::endl;
	std::cout << "->cfFormat " << outformat.cfFormat << std::endl;
	std::cout << "->dwAspect " << outformat.dwAspect << std::endl;
	std::cout << "->tymed " << outformat.tymed << std::endl;
	
	std::cout << ">>DoVerb" << std::endl;
	RECT rect = { 0, 0, 800, 600 };
	hr = pOleObject->DoVerb(OLEIVERB_SHOW, NULL, pClientSite, 0, 0L /* HWND */, &rect);
	std::cout << hr << std::endl;
	
	MSG ms;
	while(GetMessage(&ms, 0, 0, 0)) {
		TranslateMessage(&ms);
		DispatchMessage(&ms);
	}
	
	pDataObject->Release();
	pPersistStorage->Release();
	pOleObject->Release();
	
	return 0;
}