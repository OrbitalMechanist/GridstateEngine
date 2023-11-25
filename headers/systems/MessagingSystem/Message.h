#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>
#include <memory>

// Forward declaration
struct IMessage;

// Base class for all systems that can receive messages
class ISubscriber {
public:
    virtual ~ISubscriber() = default;
    virtual void handleMessage(const IMessage& message) = 0;
};

// The IMessage interface
struct IMessage {
    virtual ~IMessage() = default;
};