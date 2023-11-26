#include "../headers/systems/MessagingSystem/MessageSystem.h"

/* 
    Takes ownership of a message and adds it to the message queue.
 - message: A unique pointer to a message object derived from IMessage.
 */
void MessageBus::postMessage(std::unique_ptr<IMessage> message) {
    std::cout << "Posting message\n";
    auto messageType = std::type_index(typeid(*message));
    if (subscribers.count(messageType) > 0) {
        for (auto& handler : subscribers[messageType]) {
            handler(*message);
        }
    }
    messages.push_back(std::move(message));
}

// Dispatches all messages in the queue to a given subscriber.
void MessageBus::dispatchMessages(ISubscriber& subscriber) {
    for (const auto& msg : messages) {
        subscriber.handleMessage(*msg);
    }
}

void MessageBus::dispatchAll() {
    std::cout << "dispatch message\n";
    for (const auto& msg : messages) {
        auto messageType = std::type_index(typeid(*msg));
        if (subscribers.count(messageType) > 0) {
            for (auto& handler : subscribers[messageType]) {
                handler(*msg);
            }
        }
    }
    clearMessages();
}

// Clears all messages from the queue.
void MessageBus::clearMessages() {
    messages.clear();
}



