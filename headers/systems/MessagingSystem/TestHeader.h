#pragma once
#include "MessageSystem.h";


// test healthComp
class HealthComponent : public ISubscriber {
    int entityID;
    int health;
    MessageBus& bus;
public:
    HealthComponent(MessageBus& messageBus,int id, int hp) : bus(messageBus), entityID(id), health(hp) {}

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

class DamageComponent : public ISubscriber {
    int entityID;
    int damage;
    MessageBus& bus;
public:
   
    DamageComponent(MessageBus& messageBus,int id, int dmg) : bus(messageBus), entityID(id), damage(dmg) {}

    void handleMessage(const IMessage& message) override {
        std::cout << "DamageComponent received a message.\n";
        if (auto reqMsg = dynamic_cast<const RequestDamageMessage*>(&message)) {
            if (reqMsg->entityID == entityID) {
                bus.postMessage(std::make_unique<ResponseDamageMessage>(reqMsg->requestID, entityID, damage));
            }
        }
    }
};

// test gms
class GameMasterSystem : public ISubscriber {
    MessageBus& bus;
    std::unordered_map<int, int> requestToEntityMap;
    int damage;
    int health;
    bool damageReceived;
    bool healthReceived;


public:
    GameMasterSystem(MessageBus& messageBus)
        : bus(messageBus), damage(0), health(0),
        damageReceived(false), healthReceived(false) {}

    void handleMessage(const IMessage& message) override {
        int requestID = -1;
        std::cout << "GMS received a message\n";

        if (auto dmgMsg = dynamic_cast<const ResponseDamageMessage*>(&message)) {
            damage = dmgMsg->damage;
            requestID = dmgMsg->requestID;
            damageReceived = true;
        }
        else if (auto healthMsg = dynamic_cast<const ResponseHealthMessage*>(&message)) {
            health = healthMsg->health;
            requestID = healthMsg->requestID;
            healthReceived = true;
        }

        if (damageReceived && healthReceived && requestID != -1) {
            int aiEntityID = requestToEntityMap[requestID];
            int newHealth = health - damage;
            bus.postMessage(std::make_unique<UpdateHealthMessage>(aiEntityID, newHealth));
            requestToEntityMap.erase(requestID);

            // reset
            damageReceived = false;
            healthReceived = false;
        }
    }

    void calculateDamage(int requestID, int playerEntityID, int aiEntityID) {
        requestToEntityMap[requestID] = aiEntityID;
        bus.postMessage(std::make_unique<RequestDamageMessage>(requestID, playerEntityID));
        bus.postMessage(std::make_unique<RequestHealthMessage>(requestID, aiEntityID)); 
       
    }
};



/*
put in main
// test messageSystem
        MessageBus bus;
        GameMasterSystem gms(bus);

        HealthComponent aiHealth(bus, 1, 100); // Pass bus reference
        DamageComponent playerDamage(bus, 2, 10); // Pass bus reference

        // Subscribe components to the MessageBus
        bus.subscribe<RequestHealthMessage>([&aiHealth](const IMessage& msg) {
            aiHealth.handleMessage(msg);
            });
        bus.subscribe<RequestDamageMessage>([&playerDamage](const IMessage& msg) {
            playerDamage.handleMessage(msg);
            });
        bus.subscribe<UpdateHealthMessage>([&aiHealth](const IMessage& msg) {
            aiHealth.handleMessage(msg);
            });
        bus.subscribe<ResponseDamageMessage>([&gms](const IMessage& msg) {
            gms.handleMessage(msg);
            });
        bus.subscribe<ResponseHealthMessage>([&gms](const IMessage& msg) {
            gms.handleMessage(msg);
            });

        
        // Perform calculations
        int requestID = 1; // Example unique request identifier
        gms.calculateDamage(requestID, 2, 1);

       
*/