#pragma once
#include "Message.h"
// Message Bus
class MessageBus {
    std::vector<std::unique_ptr<IMessage>> messages;
    std::unordered_map<std::type_index, std::vector<std::function<void(const IMessage&)>>> subscribers;

public:
    // Takes ownership of a message and adds it to the message queue.
    void postMessage(std::unique_ptr<IMessage> message); // message: A unique pointer to a message object derived from IMessage.

     // Dispatches all messages in the queue to a given subscriber.
    void dispatchMessages(ISubscriber& subscriber);

    // Dispatches all messages
    void dispatchAll();

    // Clears all messages from the queue.
    void clearMessages();

    template <typename T>
    void subscribe(std::function<void(const IMessage&)> handler) {
        std::type_index messageType(typeid(T));
        subscribers[messageType].push_back(handler);
    }
   
};


