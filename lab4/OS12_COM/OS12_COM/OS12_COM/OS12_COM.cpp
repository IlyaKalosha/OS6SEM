#include "pch.h"
#include "OS12_COM.h"
#include "SEQLOG.h"

extern ULONG g_Components;


OS12_COM::OS12_COM() :m_Ref(1) {
	SEQ;
	InterlockedIncrement((LONG*)&g_Components);
	LOG("OS12::Adder g_Components = ", g_Components);
};
OS12_COM::~OS12_COM() {
	SEQ;
	InterlockedDecrement((LONG*)&g_Components);
	LOG("OS12::~Adder g_Components = ", g_Components);
};


HRESULT STDMETHODCALLTYPE OS12_COM::QueryInterface(REFIID riid, void** ppv)
{
	SEQ;
	HRESULT rc = S_OK;
	*ppv = NULL;
	if (riid == IID_IUnknown)	*ppv = (IAdder*)this;
	else if (riid == IID_IAdder)	*ppv = (IAdder*)this;
	else if (riid == IID_IMultiplier)	*ppv = (IMultiplier*)this;
	else rc = E_NOINTERFACE;

	if (rc == S_OK) this->AddRef();
	LOG("OS12::QueryInterface rc = ", rc);
	return rc;
};
ULONG STDMETHODCALLTYPE OS12_COM::AddRef(void) {
	SEQ;
	InterlockedIncrement((LONG*)&(this->m_Ref));
	LOG("OS12::AddRef m_Ref = ", this->m_Ref);
	return this->m_Ref;
};
ULONG STDMETHODCALLTYPE OS12_COM::Release(void) {
	SEQ;

	ULONG rc = this->m_Ref;
	if ((rc = InterlockedDecrement((LONG*)&(this->m_Ref))) == 0) delete this;
	LOG("OS12::Release rc = ", rc);
	return rc;
};



HRESULT  STDMETHODCALLTYPE OS12_COM::Add(
	const double x,
	const double y,
	double& z
) {
	z = x + y;
	return S_OK;
}
HRESULT STDMETHODCALLTYPE OS12_COM::Sub(
	const double x,
	const double y,
	double& z
) {
	z = y - x;
	return S_OK;

}


HRESULT STDMETHODCALLTYPE OS12_COM::Mul(
	const double x,
	const double y,
	double& z
) {
	z = x * y;
	return S_OK;
}
HRESULT STDMETHODCALLTYPE OS12_COM::Div(
	const double x,
	const double y,
	double& z
) {
	z = x / y;
	return S_OK;

}