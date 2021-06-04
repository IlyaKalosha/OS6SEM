#pragma once
#include <objbase.h>
#include "HT.h"
#define OS13HANDLE void*	

namespace HTCOM_STATIC
{
	OS13HANDLE Init();

	void Dispose(OS13HANDLE h);

	HT::HTHANDLE* Create(OS13HANDLE h, int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]);

	HT::HTHANDLE* Start(OS13HANDLE h, const char FileName[512]);

	void Stop(OS13HANDLE h, const wchar_t* Process1Name, const wchar_t* Process2Name, const wchar_t* Process3Name, const wchar_t* Process4Name);

	void Insert(OS13HANDLE h, const char MappingName[512]);

	void Update(OS13HANDLE h, const char MappingName[512]);

	void Delete(OS13HANDLE h, const char MappingName[512]);

	void killProcessByName(const wchar_t* filename);
}
// {797753C6-8424-4CAD-8A88-55294A25F1B6}
static const GUID CLSID_CA =
{ 0x797753c6, 0x8424, 0x4cad, { 0x8a, 0x88, 0x55, 0x29, 0x4a, 0x25, 0xf1, 0xb6 } };


// {ED5638A8-82F9-4CAF-84D0-7D7928569C7C}
static const GUID IID_IHTCreate =
{ 0xed5638a8, 0x82f9, 0x4caf, { 0x84, 0xd0, 0x7d, 0x79, 0x28, 0x56, 0x9c, 0x7c } };
interface IHTCreate : IUnknown
{
	virtual HRESULT __stdcall HTCreate
	(
		int Capacity,
		int SecSnapshotInterval,
		int MaxKeyLength,
		int MaxPayloadLength,
		const char FileName[512],
		HT::HTHANDLE*& htHandle) = 0;
};

// {2066EABB-1FC5-42D4-8BFF-68C19EC6A92A}
static const GUID IID_IHTStart =
{ 0x2066eabb, 0x1fc5, 0x42d4, { 0x8b, 0xff, 0x68, 0xc1, 0x9e, 0xc6, 0xa9, 0x2a } };
interface IHTStart : IUnknown
{
	virtual HRESULT __stdcall HTOpen
	(
		const char FileName[512],
		HT::HTHANDLE*& htHandle
	) = 0;
};

// {9559655F-9AEB-4B57-83A2-B3DC5CE27D03}
static const GUID IID_IHTStop =
{ 0x9559655f, 0x9aeb, 0x4b57, { 0x83, 0xa2, 0xb3, 0xdc, 0x5c, 0xe2, 0x7d, 0x3 } };
interface IHTStop : IUnknown
{
	virtual HRESULT __stdcall HTSnap
	(
		const HT::HTHANDLE* hthandle,
		BOOL& result
	) = 0;
	virtual HRESULT __stdcall HTClose
	(
		const HT::HTHANDLE* hthandle,
		BOOL& result
	) = 0;
};

// {768B0D2D-DFF8-4502-971C-30C1475FC6C1}
static const GUID IID_IHTInsert =
{ 0x768b0d2d, 0xdff8, 0x4502, { 0x97, 0x1c, 0x30, 0xc1, 0x47, 0x5f, 0xc6, 0xc1 } };
interface IHTInsert : IUnknown
{
	virtual HRESULT __stdcall HTInsert
	(
		const HT::HTHANDLE* hthandle,
		const HT::Element*  element,
		BOOL& result
	) = 0;

	virtual HRESULT __stdcall HTOpenMapView
	(
		const char MappingName[512],
		LPVOID& lp
	) = 0;

	virtual HRESULT __stdcall HTGet
	(
		const HT::HTHANDLE* hthandle,
		const HT::Element*  element,
		HT::Element*& getElement
	) = 0;

	virtual HRESULT __stdcall ElemCreate
	(
		LPCVOID key,
		int keylength,
		LPCVOID payload,
		int payloadlength,
		HT::Element*& elem
	);
};

// {FDDCC302-1368-4C43-8D61-635F26FDED60}
static const GUID IID_IHTUpdate =
{ 0xfddcc302, 0x1368, 0x4c43, { 0x8d, 0x61, 0x63, 0x5f, 0x26, 0xfd, 0xed, 0x60 } };
interface IHTUpdate : IUnknown
{
	virtual HRESULT __stdcall HTUpdate
	(
		const HT::HTHANDLE* hthandle,
		const HT::Element*  oldelement,
		const void* newpayload,
		int newpayloadlength,
		BOOL& result
	) = 0;

	virtual HRESULT __stdcall HTOpenMapView
	(
		const char MappingName[512],
		LPVOID& lp
	) = 0;

	virtual HRESULT __stdcall HTGet
	(
		const HT::HTHANDLE* hthandle,
		const HT::Element*  element,
		HT::Element*& getElement
	) = 0;

	virtual HRESULT __stdcall ElemCreate
	(
		LPCVOID key,
		int keylength,
		LPCVOID payload,
		int payloadlength,
		HT::Element*& elem
	);
};

// {C575697D-5A00-40B7-9E3E-06B4FBB61FAF}
static const GUID IID_IHTDelete =
{ 0xc575697d, 0x5a00, 0x40b7, { 0x9e, 0x3e, 0x6, 0xb4, 0xfb, 0xb6, 0x1f, 0xaf } };
interface IHTDelete : IUnknown
{
	virtual HRESULT __stdcall HTDelete
	(
		const HT::HTHANDLE* hthandle,
		const HT::Element*  element,
		BOOL& result
	) = 0;

	virtual HRESULT __stdcall HTOpenMapView
	(
		const char MappingName[512],
		LPVOID& lp
	) = 0;

	virtual HRESULT __stdcall HTGet
	(
		const HT::HTHANDLE* hthandle,
		const HT::Element*  element,
		HT::Element*& getElement
	) = 0;

	virtual HRESULT __stdcall ElemCreate
	(
		LPCVOID key,
		int keylength,
		LPCVOID payload,
		int payloadlength,
		HT::Element*& elem
	);
};
