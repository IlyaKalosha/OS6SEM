#pragma once
#include "interface.h"

class OS12_COM : public IAdder, public IMultiplier
{
public:
	OS12_COM();
	~OS12_COM();

	virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppv);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);

	virtual HRESULT __stdcall Add(const double x, const double y, double& z);
	virtual HRESULT __stdcall Sub(const double x, const double y, double& z);

	virtual HRESULT __stdcall Mul(const double x, const double y, double& z);
	virtual HRESULT __stdcall Div(const double x, const double y, double& z);

private:
	volatile ULONG m_Ref;
};
