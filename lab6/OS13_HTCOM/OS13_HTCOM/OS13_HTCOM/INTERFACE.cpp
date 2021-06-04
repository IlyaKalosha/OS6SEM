#include "pch.h"
#include "INTERFACE.h"
#include <ctime>


HTHANDLE::HTHANDLE()
{

}

HTHANDLE::HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512])
{
	this->Capacity = Capacity;
	this->SecSnapshotInterval = SecSnapshotInterval;
	this->MaxKeyLength = MaxKeyLength;
	this->MaxPayloadLength = MaxPayloadLength;
	this->File = NULL;
	this->FileMapping = NULL;
	this->Addr = NULL;
	memset(this->LastErrorMessage, 0, sizeof(this->LastErrorMessage));
	this->lastsnaptime = time(NULL);
	memcpy(this->FileName, FileName, 512);
}

Element::Element(LPCVOID key, int keylength)
{
	this->key = key;
	this->keylength = keylength;
	this->payload = NULL;
	this->payloadlength = NULL;
}

Element::Element(LPCVOID key, int keylength, LPCVOID payload, int payloadlength)
{
	this->key = key;
	this->keylength = keylength;
	this->payload = payload;
	this->payloadlength = payloadlength;
}

Element::Element(Element* oldelement, LPCVOID newpayload, int newpayloadlength)
{
	this->key = oldelement->key;
	this->keylength = oldelement->keylength;
	this->payload = newpayload;
	this->payloadlength = newpayloadlength;
}

Element::Element()
{
}

Element::Element(const void * key, int keylength)
{
}
