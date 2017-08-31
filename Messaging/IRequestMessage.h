// I have tested this on Windows, so only below header was added.
//#include "stdafx.h"
#include <iostream>

enum MessageType
{
	red = 1,
	yellow,
	blue
};

typedef void (*CallbackFunc)();

// Interface for sending/receiving the messages
class IRequestMessage
{
public:
	virtual void addToQueue(std::string msg, CallbackFunc cb,  MessageType type, int priority) = 0;
	virtual void GetRequestMessage(MessageType type, std::string& MessageStr, CallbackFunc& cbFunc) = 0;
};