#pragma once
#include "MessageSystem.h";
/*
Test MessageSystem only - Health 
*/

// A specific message type
struct HealthChangedMessage : public IMessage {
    int entityID;
    int newHealth;

    HealthChangedMessage(int id, int health) : entityID(id), newHealth(health) {}
};

// Health System that cares about HealthChangedMessage
class HealthSystem : public ISubscriber {
public:
	// put this function in systems that need handle message
    void handleMessage(const IMessage& message) override {
        if (auto healthMsg = dynamic_cast<const HealthChangedMessage*>(&message)) {
            std::cout << "Entity " << healthMsg->entityID
                << " health changed to " << healthMsg->newHealth << std::endl;
        }
    }
};




/*
put this in main before game while loop
// MessageSystem Test start:

		MessageBus bus;
		HealthSystem healthSystem;

		// Simulating events
		bus.postMessage(std::make_unique<HealthChangedMessage>(1, 100));
		bus.postMessage(std::make_unique<HealthChangedMessage>(2, 150));

		// Dispatch messages to interested subscribers
		bus.dispatchMessages(healthSystem);

		// Clear messages after dispatching
		bus.clearMessages();

// MessageSystem Test end
*/