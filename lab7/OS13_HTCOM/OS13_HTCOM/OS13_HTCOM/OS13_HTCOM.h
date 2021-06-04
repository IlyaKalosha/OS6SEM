#pragma once
#include "INTERFACE.h"

class OS13_HTCOM : public IHTCreate, public IHTStart, public IHTStop,
	public IHTInsert, public IHTUpdate, public IHTDelete
{
public:
	OS13_HTCOM();
	~OS13_HTCOM();


	// Унаследовано через IHTCreate
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void ** ppvObject);
	virtual ULONG __stdcall AddRef(void);
	virtual ULONG __stdcall Release(void);
	virtual HRESULT __stdcall HTCreate(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength,const char HTUsersGroup[512], const char FileName[512], HT::HTHANDLE*& HTHANDLE);

	// Унаследовано через IHTStart
	virtual HRESULT __stdcall HTOpen(const char FileName[512], HT::HTHANDLE*& HTHANDLE);
	virtual HRESULT __stdcall HTOpen(const char HTUser[512], const char HTPassword[512], const char FileName[512], HT::HTHANDLE*& htHandle);

	// Унаследовано через IHTStop
	virtual HRESULT __stdcall HTSnap(const HT::HTHANDLE * HTHANDLE, BOOL & result);
	virtual HRESULT __stdcall HTClose(const HT::HTHANDLE * HTHANDLE, BOOL & result);

	// Унаследовано через IHTInsert
	virtual HRESULT __stdcall HTInsert(const HT::HTHANDLE * HTHANDLE, const HT::Element * Element, BOOL & result);
	virtual HRESULT __stdcall HTOpenMapView(const char MappingName[512], LPVOID & lp);
	virtual HRESULT __stdcall ElemCreate(LPCVOID key, int keylength, LPCVOID payload, int payloadlength, HT::Element*& elem);

	// Унаследовано через IHTUpdate
	virtual HRESULT __stdcall HTUpdate(const HT::HTHANDLE * hthandle, const HT::Element * oldelement, const void * newpayload, int newpayloadlength, BOOL & result) override;


	// Унаследовано через IHTDelete
	virtual HRESULT __stdcall HTDelete(const HT::HTHANDLE * HTHANDLE, const HT::Element * Element, BOOL & result);
	virtual HRESULT __stdcall HTGet(const HT::HTHANDLE * HTHANDLE, const HT::Element * Element, HT::Element*& getElement);

	char * GetLastError(HT::HTHANDLE * ht);
	void print(const HT::Element * Element);
	bool IsMemEmpty(LPCVOID mem, int len);
	int GenerateHash(LPCVOID bytes, int numOfBytes);
private:
	volatile ULONG m_Ref;



};

