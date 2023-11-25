#include "../headers/systems/MessagingSystem/MessageSystem.h"

/* 
    Takes ownership of a message and adds it to the message queue.
 - message: A unique pointer to a message object derived from IMessage.
 */
void MessageBus::postMessage(std::unique_ptr<IMessage> message) {
    messages.push_back(std::move(message));
}

// Dispatches all messages in the queue to a given subscriber.
void MessageBus::dispatchMessages(ISubscriber& subscriber) {
    for (const auto& msg : messages) {
        subscriber.handleMessage(*msg);
    }
}

// Clears all messages from the queue.
void MessageBus::clearMessages() {
    messages.clear();
}