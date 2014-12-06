
#ifndef INTERFACES_H
#define INTERFACES_H

#include <unknwn.h>
#include <windows.h>

struct IRegistration : public IUnknown {
	virtual HRESULT __stdcall GetOwner(BSTR * pBstrOwner) = 0;
	virtual HRESULT __stdcall SetOwner(BSTR bstrOwner) = 0;
};

struct IStatus : public IUnknown {
	virtual HRESULT __stdcall GetSpeed(int * pnSpeed) = 0;
	virtual HRESULT __stdcall SetSpeed(int nSpeed) = 0;
};

#endif /* INTERFACES_H */
