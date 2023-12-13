//
// Created by Hushe on 11/26/2023.
// Edited by Mike
//

#include "../headers/ai/EnemyAI.h"



// Calculate Euclidean distance between two positions
double EnemyAI::calculateDistance(std::pair<int, int> pos1, std::pair<int, int> pos2) {
    int dx = pos2.first - pos1.first;
    int dy = pos2.second - pos1.second;
    double distance = static_cast<int>(std::hypot(dx, dy)); // allow diagonal attack

    return distance;
}

void EnemyAI::enemyPerform(Entity attacker, Entity target) {
    std::pair<int, int> aiPos = std::make_pair(manager.getComponent<TransformComponent>(attacker).pos.x , manager.getComponent<TransformComponent>(attacker).pos.y);
    std::pair<int, int> playerPos = std::make_pair(manager.getComponent<TransformComponent>(target).pos.x, manager.getComponent<TransformComponent>(target).pos.y);
    double distance = calculateDistance(aiPos, playerPos);

    // check if within range
    if (distance <= manager.getComponent<AttackComponent>(attacker).range) {
        
        // check attack hit or missed
        bool attackResult = performAttack(attacker, target);

        if (attackResult) {
            glm::vec3 attackerPos = { manager.getComponent<TransformComponent>(attacker).pos.x, manager.getComponent<TransformComponent>(attacker).pos.y, 0 };
            manager.getComponent<AudioComponent>(attacker).sourceA->SetPosition(attackerPos);
            manager.getComponent<AudioComponent>(attacker).sourceA->Play(audio.getSoundEffect("meleeHit"));
            glm::vec3 targetPos = { manager.getComponent<TransformComponent>(target).pos.x, manager.getComponent<TransformComponent>(target).pos.y, 0 };
            manager.getComponent<AudioComponent>(target).sourceA->SetPosition(targetPos);
            manager.getComponent<AudioComponent>(target).sourceA->Play(audio.getSoundEffect("injured"));
        }
    }
}

bool EnemyAI::performAttack(Entity attacker, Entity target) {

    // Check if the total attack value is greater than or equal to the target's armor
    if (manager.getComponent<AttackComponent>(attacker).damage >= manager.getComponent<HealthComponent>(target).armor) {
        // If the attack hits, calculate damage
        int damageDealt = manager.getComponent<AttackComponent>(attacker).damage - manager.getComponent<HealthComponent>(target).armor;
        if (manager.getComponent<HealthComponent>(target).health > damageDealt) {
            manager.getComponent<HealthComponent>(target).health -= damageDealt;
        }
        else {
            manager.destroyEntity(target);
        }

        return true;
    }
    else {
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
        return NULL;
    }
    return closestPlayer;
}