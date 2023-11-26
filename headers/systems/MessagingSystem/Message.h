#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <functional>

// Forward declaration
struct IMessage;

// Base class for all systems that can receive messages
class ISubscriber {
public:
    virtual ~ISubscriber() = default;
    virtual void handleMessage(const IMessage& message) = 0;
};


struct IMessage {
	virtual ~IMessage() = default;
};



// damage request message
struct RequestDamageMessage : IMessage {
	int entityID;
	RequestDamageMessage(int id) : entityID(id) {}
};

// damage health message
struct RequestHealthMessage : IMessage {
	int entityID;
	RequestHealthMessage(int id) : entityID(id) {}
};

// damage response message 
struct ResponseDamageMessage : IMessage {
	int entityID;
	int damage;
	ResponseDamageMessage(int id, int dm) : entityID(id), damage(dm){}
};

// health response message
struct ResponseHealthMessage : IMessage {
	int entityID;
	int health;
	ResponseHealthMessage(int id, int hp) : entityID(id), health(hp) {}
};

// health update message
struct UpdateHealthMessage : IMessage {
	int entityID;
	int newHealth;
	UpdateHealthMessage(int id, int hp) : entityID(id), newHealth(hp) {}
};