//
// Created by Hushe on 11/26/2023.
// Edited by Mike
//

#include "../headers/ai/EnemyAI.h"



// Calculate Euclidean distance between two positions
double EnemyAI::calculateDistance(std::pair<int, int> pos1, std::pair<int, int> pos2) {
    int dx = pos2.first - pos1.first;
    int dy = pos2.second - pos1.second;
    return abs(sqrt(dx * dx + dy * dy));
}

void EnemyAI::enemyPerform(Entity attacker, Entity target) {
    std::pair<int, int> aiPos = std::make_pair(manager.getComponent<GridPositionComponent>(attacker).gridX , manager.getComponent<GridPositionComponent>(attacker).gridY);
    std::pair<int, int> playerPos = std::make_pair(manager.getComponent<GridPositionComponent>(target).gridX, manager.getComponent<GridPositionComponent>(target).gridY);
    std::cout << aiPos.first << " : " << aiPos.second << std::endl;
    double distance = calculateDistance(aiPos, playerPos);

    // check if within range
    if (distance <= manager.getComponent<AttackComponent>(attacker).range) {
        
        // check attack hit or missed
        bool attackResult = performAttack(attacker, target);

        if (attackResult) {
            std::cout << "AI unit attacks player!" << std::endl;
        } else {
            std::cout << "AI unit misses the attack!" << std::endl;
        }  
    }
    else {
        std::cout << "Out of AttackRange" << std::endl;
    }
}

bool EnemyAI::performAttack(Entity attacker, Entity target) {
    // Generate a random number between 1 and 4 (1d4 roll)
    int attackRoll = rand() % 4 + 1;

    // Calculate total attack value by adding the attack roll and attack modifier
    int attackModifier = manager.getComponent<AttackComponent>(attacker).attackModifier;
    int totalAttack = attackRoll + attackModifier;

    // Check if the total attack value is greater than or equal to the target's armor
    if (totalAttack >= manager.getComponent<HealthComponent>(attacker).armor) {
        // If the attack hits, calculate damage
        int damageDealt = manager.getComponent<AttackComponent>(attacker).damage;

        // Reduce target's health by the damage dealt
        manager.getComponent<HealthComponent>(target).health -= damageDealt; // changet this one later using GMS

        std::cout << "Attack hits! Damage dealt: " << damageDealt << std::endl;
        return true;
    }
    else {
        std::cout << "Attack misses!" << std::endl;
        return false;
    }
}

// Function to simulate calculating hit chance
float EnemyAI::calculateHitChance(Entity attacker, Entity targetID) {
    int attackRoll = rand() % 4 + 1;
    int totalAttack = attackRoll + manager.getComponent<AttackComponent>(attacker).attackModifier;

    float hitChance = static_cast<float>(totalAttack) / static_cast<float>(manager.getComponent<HealthComponent>(attacker).armor);

    return hitChance;
}

// use to find the closest player to the enemy
Entity EnemyAI::GetClosestPlayer(Entity attacker) {
    // Get the list of players
    std::vector<Entity> entitiesWithPlayer = manager.getEntitiesWithComponent<PlayerComponent>();
    
    // Closest Player Entity
    Entity closestPlayer = manager.createEntity();
    int closestDistance = -1;
    for (Entity player : entitiesWithPlayer) {
        
        // Get this player position
        std::pair<int, int> attackerPos = std::make_pair(manager.getComponent<GridPositionComponent>(attacker).gridX, manager.getComponent<GridPositionComponent>(attacker).gridY);
        std::pair<int, int> playerPos = std::make_pair(manager.getComponent<GridPositionComponent>(player).gridX, manager.getComponent<GridPositionComponent>(player).gridY);
      
        int currentDistance = calculateDistance(playerPos, attackerPos);
        if (closestDistance == -1 || closestDistance > currentDistance) {
            closestDistance = currentDistance;
            closestPlayer = player;
        }
    }
    if (closestDistance == -1) {
        std::cout << "Is null" << std::endl;
        return NULL;
    }
    else {
        std::cout << "Distance " << closestDistance << std::endl;
    }
    return closestPlayer;
}