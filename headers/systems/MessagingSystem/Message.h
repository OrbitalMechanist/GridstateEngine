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
	int requestID;
	int entityID;
	RequestDamageMessage(int reqID, int id) : requestID(reqID), entityID(id) {}
};

// damage health message
struct RequestHealthMessage : IMessage {
	int entityID;
	int requestID;
	RequestHealthMessage(int reqID, int id) : requestID(reqID), entityID(id) {}
};

// damage response message 
struct ResponseDamageMessage : IMessage {
	int requestID;
	int entityID;
	int damage;
	ResponseDamageMessage(int reqID, int id, int dmg) : requestID(reqID), entityID(id), damage(dmg) {}
};

// health response message
struct ResponseHealthMessage : IMessage {
	int requestID;
	int entityID;
	int health;
	ResponseHealthMessage(int reqID, int id, int hp) : requestID(reqID), entityID(id), health(hp) {}
};

// health update message
struct UpdateHealthMessage : IMessage {
	int entityID;
	int newHealth;
	UpdateHealthMessage(int id, int hp) : entityID(id), newHealth(hp) {}
};

