#pragma once
#include "Message.h"
// Message Bus
class MessageBus {
    std::vector<std::unique_ptr<IMessage>> messages;

public:
    // Takes ownership of a message and adds it to the message queue.
    void postMessage(std::unique_ptr<IMessage> message); // message: A unique pointer to a message object derived from IMessage.

     // Dispatches all messages in the queue to a given subscriber.
    void dispatchMessages(ISubscriber& subscriber);

    // Clears all messages from the queue.
    void clearMessages();

};


