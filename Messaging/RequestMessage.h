// I have tested this on Windows, so only below header was added.
//#include "stdafx.h"
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <exception>
#include "IRequestMessage.h"
#include <mutex>


// Implementation of the Interface IRequestMessage
class RequestMessage : public IRequestMessage
{
public:
	RequestMessage();
	~RequestMessage();
	MessageType GetMessageType();
	int GetPriority();
	std::string GetMessage();
	CallbackFunc GetCallbackFunc();

	virtual void addToQueue(std::string msg, CallbackFunc cb,  MessageType type, int priority);
	virtual void GetRequestMessage(MessageType type, std::string& MessageStr, CallbackFunc& cbFunc);

private:
	std::string _message;
	MessageType _messageType;
	int _priority;
	CallbackFunc _cbFunc;
	std::mutex _mutex;	// Can be replaced with std::shared_mutex for read/write lock model.
};

// Comparator class for Priority Queue
class CompareMessage {
public:
	bool operator()(std::shared_ptr<RequestMessage>& msg1, std::shared_ptr<RequestMessage>& msg2)
    {
		if (msg1->GetMessageType() > msg2->GetMessageType()) return true;
		if ((msg1->GetMessageType() == msg2->GetMessageType()) && (msg1->GetPriority() > msg2->GetPriority())) return true;
		return false;
	}
};

// Local cache for storing the Messages
typedef std::priority_queue<std::shared_ptr<RequestMessage>, std::vector<std::shared_ptr<RequestMessage>>, CompareMessage> Messages;
std::unordered_map<MessageType, Messages> MessagesMap;