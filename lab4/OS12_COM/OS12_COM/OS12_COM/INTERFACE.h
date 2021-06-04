#pragma once
#include <objbase.h>

#define FNAME L"KIV.OS12_COM.COM"
#define VINDX L"KIV.OS12_COM.1"
#define PRGID L"KIV.OS12.COM"

// {C1A23DC5-9A06-4F66-8B5A-F9DCE6632F59}
static const GUID CLSID_CA =
{ 0xc1a23dc5, 0x9a06, 0x4f66, { 0x8b, 0x5a, 0xf9, 0xdc, 0xe6, 0x63, 0x2f, 0x59 } };

// {853561C9-6E57-4D4A-B8DE-5775A8D186F8}
static const GUID IID_IAdder =
{ 0x853561c9, 0x6e57, 0x4d4a, { 0xb8, 0xde, 0x57, 0x75, 0xa8, 0xd1, 0x86, 0xf8 } };

interface IAdder : IUnknown
{
	virtual HRESULT __stdcall Add(const double x, const double y, double& z) = 0;
	virtual HRESULT __stdcall Sub(const double x, const double y, double& z) = 0;
};

// {BE631117-B209-46A7-8006-CBD604D943A0}
static const GUID IID_IMultiplier =
{ 0xbe631117, 0xb209, 0x46a7, { 0x80, 0x6, 0xcb, 0xd6, 0x4, 0xd9, 0x43, 0xa0 } };
interface IMultiplier : IUnknown
{
	virtual HRESULT __stdcall Mul(const double x, const double y, double& z) = 0;
	virtual HRESULT __stdcall Div(const double x, const double y, double& z) = 0;
};