#pragma once
#include "systems/MessagingSystem/MessageSystem.h"
#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

// health
struct HealthComponent : public ISubscriber {
    int entityID;
    int health;
    int armor;
    MessageBus& bus;
public:
    HealthComponent(MessageBus& messageBus, int id, int hp, int armor) : bus(messageBus), entityID(id), health(hp), armor(armor) {}

    void handleMessage(const IMessage& message) override {
        std::cout << "HealthComponent received a message.\n";
        if (auto reqMsg = dynamic_cast<const RequestHealthMessage*>(&message)) {
            if (reqMsg->entityID == entityID) {
                bus.postMessage(std::make_unique<ResponseHealthMessage>(reqMsg->requestID, entityID, health));
            }
        }
        else if (auto updateMsg = dynamic_cast<const UpdateHealthMessage*>(&message)) {
            if (updateMsg->entityID == entityID) {
                std::cout << "Updating health from " << health << " to " << updateMsg->newHealth << "\n";
                health = updateMsg->newHealth;
            }

        }
    }
};

#endif 
