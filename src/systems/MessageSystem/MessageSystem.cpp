#include "../headers/systems/MessagingSystem/MessageSystem.h"

/* 
    Takes ownership of a message and adds it to the message queue.
 - message: A unique pointer to a message object derived from IMessage.
 */
void MessageBus::postMessage(std::unique_ptr<IMessage> message) {
    std::cout << "Posting message of type: " << typeid(*message).name() << std::endl;
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

// Dispatches all messages
void MessageBus::dispatchAll() {
    std::cout << "Dispatching messages, count: " << messages.size() << std::endl;
    for (const auto& msg : messages) {
        if (!msg) {
            std::cout << "Warning: Null message encountered in dispatchAll.\n";
            
        }
        auto messageType = std::type_index(typeid(*msg));
        if (subscribers.count(messageType) > 0) {
            for (auto& handler : subscribers[messageType]) {
                std::cout << "Dispatching message of type: " << typeid(*msg).name() << std::endl;
                handler(*msg);
            }
        }
        else {
            std::cout << "No subscribers for message type: " << typeid(*msg).name() << std::endl;
        }
        
    }

    clearMessages();
}

// Clears all messages from the queue.
void MessageBus::clearMessages() {
    messages.clear();
}



