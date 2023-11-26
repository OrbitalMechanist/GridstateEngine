#pragma once
#include "MessageSystem.h";


// test healthComp
class HealthComponent : public ISubscriber {
public:
    int entityID;
    int health;

    HealthComponent(int id, int hp) : entityID(id), health(hp) {}

    void handleMessage(const IMessage& message) override {
        if (auto reqMsg = dynamic_cast<const RequestHealthMessage*>(&message)) {
            if (reqMsg->entityID == entityID) {
              
                ResponseHealthMessage respMsg{ entityID, health };
               
            }
        }
        else if (auto updateMsg = dynamic_cast<const UpdateHealthMessage*>(&message)) {
            if (updateMsg->entityID == entityID) {
                //std::cout << "Updating health from " << health << " to " << updateMsg->newHealth << "\n";
                health = updateMsg->newHealth;
            }
            
        }
    }
};

class DamageComponent : public ISubscriber {
public:
    int entityID;
    int damage;

    DamageComponent(int id, int dmg) : entityID(id), damage(dmg) {}

    void handleMessage(const IMessage& message) override {
        if (auto reqMsg = dynamic_cast<const RequestDamageMessage*>(&message)) {
            if (reqMsg->entityID == entityID) {
                
                ResponseDamageMessage respMsg{ entityID, damage };
               
            }
        }
    }
};

// test gms
class GameMasterSystem : public ISubscriber {
    MessageBus& bus;

public:
    GameMasterSystem(MessageBus& messageBus) : bus(messageBus) {}

    void handleMessage(const IMessage& message) override {
        
    }

    void calculateDamage(int playerEntityID, int aiEntityID) {
       // std::cout << "Calculating damage\n";
        bus.postMessage(std::make_unique<RequestDamageMessage>(playerEntityID));
        
        bus.postMessage(std::make_unique<RequestHealthMessage>(aiEntityID));
        
        int damage = 10; 
        int health = 100; 
        int newHealth = health - damage;
        
        bus.postMessage(std::make_unique<UpdateHealthMessage>(aiEntityID, newHealth));
    }
};



/*
put in main

// test messageSystem
        MessageBus bus;
        GameMasterSystem gms(bus);

        HealthComponent aiHealth(1, 100);
        DamageComponent playerDamage(2, 10);

        // Subscribe components to the MessageBus
        bus.subscribe<UpdateHealthMessage>([&aiHealth](const IMessage& msg) {
            aiHealth.handleMessage(msg);
            });


        // Perform calculations
        gms.calculateDamage(2, 1);
        bus.dispatchAll();

        std::cout << "AI Health: " << aiHealth.health << std::endl;
*/