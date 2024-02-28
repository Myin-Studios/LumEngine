#pragma once

//types

using CMUInt = unsigned int; //CryoMoon unsigned int
using CMUInt_Ptr = unsigned int*; //CryoMoon unsigned int pointer

struct CMString
{
	const char* text;
};

//CryoMoon event type
struct CMEvent
{
	static CMUInt value;

	const CMEvent* type = this;
};

CMUInt CMEvent::value = 0;