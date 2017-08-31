// Messaging.cpp : Defines the entry point for the console application.
//
// I have tested this on Windows, so only below header was added.
//#include "stdafx.h"
#include "RequestMessage.h"

RequestMessage::RequestMessage() {}

RequestMessage::~RequestMessage() {}

// Getters
MessageType RequestMessage::GetMessageType()
{
	return _messageType;
}

int RequestMessage::GetPriority()
{
	return _priority;
}

std::string RequestMessage::GetMessage()
{
	return _message;
}

CallbackFunc RequestMessage::GetCallbackFunc()
{
	return _cbFunc;
}

void RequestMessage::addToQueue(std::string msg, CallbackFunc cb, MessageType type, int priority)
{
	try
	{
		if ((type > MessageType::blue) || (type < MessageType::red))	//Invalid scenario.
			return;

		if (priority > 3)	//Invalid scenario.
			return;

		if (cb == NULL)		//Invalid scenario.
			return;

		// Add to the Queue if it is a valid input.
		auto reqMessage = std::shared_ptr<RequestMessage>(new RequestMessage());
		reqMessage->_message = msg;
		reqMessage->_messageType = type;
		reqMessage->_priority = priority;
		reqMessage->_cbFunc = cb;

		_mutex.lock();
		MessagesMap[type].push(reqMessage);
		_mutex.unlock();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in RequestMessage::AddToQueue: " << e.what() << '\n';
	}
}

void RequestMessage::GetRequestMessage(MessageType type, std::string& MessageStr, CallbackFunc& cbFunc)
{
	try
	{
		_mutex.lock();
		// Return if no messages are found
		if (MessagesMap[type].empty())
		{
			std::cout << "No messages to return" << std::endl;
			return;
		}

		// Get the topmost message
		auto reqMsg = MessagesMap[type].top();
		MessageStr = reqMsg->GetMessage();
		cbFunc = reqMsg->GetCallbackFunc();

		MessagesMap[type].pop();
		_mutex.unlock();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception in RequestMessage::GetRequestMessage: " << e.what() << '\n';
	}
}

// Temporary method for callback function
void func()
{
	std::cout << "Callback method" << std::endl;
}

int main()
{
	IRequestMessage* r = new RequestMessage();

	// Sample inputs
	r->addToQueue("test1", func, MessageType::blue, 3);
	r->addToQueue("test2", func, MessageType::red, 2);
	r->addToQueue("test3", func, MessageType::red, 1);

	std::string str;
	CallbackFunc cb = 0;

	// Retrieves Message string and callback function
	r->GetRequestMessage(MessageType::red, str, cb);
	std::cout << str << std::endl;
	cb();

	str.clear();
	cb=0;
	r->GetRequestMessage(MessageType::red, str, cb);
	std::cout << str << std::endl;
	cb();
	
	str.clear();
	cb=0;
	r->GetRequestMessage(MessageType::blue, str, cb);
	std::cout << str << std::endl;
	cb();
	
	str.clear();
	cb=0;
	r->GetRequestMessage(MessageType::red, str, cb);
}

