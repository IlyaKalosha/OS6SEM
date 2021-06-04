#include "pch.h"
#include "HT.h"
#include <ctime>


HT::HTHANDLE::HTHANDLE()
{

}

HT::HTHANDLE::HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char HTUsersGroup[512], const char FileName[512])
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
	memcpy(this->HTUsersGroup, HTUsersGroup,512);
	memcpy(this->FileName, FileName, 512);
}

HT::Element::Element()
{
}

HT::Element::Element(LPCVOID key, int keylength)
{
	this->key = key;
	this->keylength = keylength;
	this->payload = NULL;
	this->payloadlength = NULL;
}

HT::Element::Element(LPCVOID key, int keylength, LPCVOID payload, int payloadlength)
{
	this->key = key;
	this->keylength = keylength;
	this->payload = payload;
	this->payloadlength = payloadlength;
}

HT::Element::Element(Element* oldelement, LPCVOID newpayload, int newpayloadlength)
{
	this->key = oldelement->key;
	this->keylength = oldelement->keylength;
	this->payload = newpayload;
	this->payloadlength = newpayloadlength;
}