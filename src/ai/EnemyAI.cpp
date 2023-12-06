//
// Created by Hushe on 11/26/2023.
// Edited by Mike
//

#include "../headers/ai/EnemyAI.h"



// Calculate Euclidean distance between two positions
double EnemyAI::calculateDistance(std::pair<int, int> pos1, std::pair<int, int> pos2) {
    int dx = pos2.first - pos1.first;
    int dy = pos2.second - pos1.second;
    return std::hypot(dx, dy);
}

void EnemyAI::enemyPerform(Entity attacker, Entity target) {
    std::pair<int, int> aiPos = std::make_pair(manager.getComponent<TransformComponent>(attacker).pos.x , manager.getComponent<TransformComponent>(attacker).pos.y);
    std::pair<int, int> playerPos = std::make_pair(manager.getComponent<TransformComponent>(target).pos.x, manager.getComponent<TransformComponent>(target).pos.y);
    //std::cout << aiPos.first << " : " << aiPos.second << std::endl;
    double distance = calculateDistance(aiPos, playerPos);

    // check if within range
    if (distance <= manager.getComponent<AttackComponent>(attacker).range) {
        
        // check attack hit or missed
        bool attackResult = performAttack(attacker, target);

        if (attackResult) {
           // std::cout << "AI unit attacks player!" << std::endl;
        } else {
           // std::cout << "AI unit misses the attack!" << std::endl;
        }  
    }
    else {
        //std::cout << "Out of AttackRange" << std::endl;
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
        int currentHealth = manager.getComponent<HealthComponent>(target).health;
        int updateHealth = currentHealth - damageDealt; // change this one later using GMS

        if (updateHealth <= 0) {
            manager.getComponent<HealthComponent>(target).health = 0;
            manager.destroyEntity(target);
        }
        else {
            manager.getComponent<HealthComponent>(target).health = updateHealth;
        }

        //std::cout << "Attack hits! Damage dealt: " << damageDealt << std::endl;
        return true;
    }
    else {
       // std::cout << "Attack misses!" << std::endl;
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
    Entity closestPlayer = NULL;
    int closestDistance = -1;
    std::pair<int, int> attackerPos = std::make_pair(manager.getComponent<TransformComponent>(attacker).pos.x, manager.getComponent<TransformComponent>(attacker).pos.y);
    for (Entity player : entitiesWithPlayer) {
        
        // Get this player position
        std::pair<int, int> playerPos = std::make_pair(manager.getComponent<TransformComponent>(player).pos.x, manager.getComponent<TransformComponent>(player).pos.y);
      
        int currentDistance = calculateDistance(playerPos, attackerPos);
        if (closestDistance == -1 || closestDistance > currentDistance) {
            closestDistance = currentDistance;
            closestPlayer = player;
        }
    }
    if (closestDistance == -1) {
        //std::cout << "Is null" << std::endl;
        return NULL;
    }
    else {
       // std::cout << "Distance " << closestDistance << std::endl;
    }
    return closestPlayer;
}