#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <string>
#include <algorithm>

// EventBus class
class EventBus {
public:
    using Listener = std::function<void()>;

    // Register a listener for a specific event
    void listenForEvent(const std::string& eventName, Listener listener) {
        listeners[eventName].push_back(listener);
    }

    // Trigger an event, notifying all listeners
    void triggerEvent(const std::string& eventName) {
        auto it = listeners.find(eventName);
        if (it != listeners.end()) {
            for (auto& listener : it->second) {
                listener();  // Notify each listener
            }
        }
    }

private:
    std::unordered_map<std::string, std::vector<Listener>> listeners;
};

// TriggerManager class
class TriggerManager {
public:
    // Add a trigger with a corresponding boolean function
    template <typename Func>
    void addTrigger(const std::string& triggerName, Func function) {
        trigger_map[triggerName] = function;
        if (event_map.find(triggerName) == event_map.end()) {
            event_map[triggerName] = std::vector<std::string>();
        }
    }

    // Check each trigger and call the associated events if the condition is true
    void checkAndTriggerEvents(EventBus& eventBus) {
        for (const auto& [triggerName, condition] : trigger_map) {
            if (condition()) {  // If the condition evaluates to true
                auto it = event_map.find(triggerName);
                if (it != event_map.end()) {
                    for (const auto& event : it->second) {
                        eventBus.triggerEvent(event);  // Trigger events from EventBus
                    }
                }
            }
        }
    }

    // Add an event to a trigger
    void addEventToTrigger(const std::string& triggerName, const std::string& eventName) {
        event_map[triggerName].push_back(eventName);
    }

private:
    std::unordered_map<std::string, std::function<bool()>> trigger_map;
    std::unordered_map<std::string, std::vector<std::string>> event_map;
};
