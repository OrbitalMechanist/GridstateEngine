#include "../headers/ai/AISystemDemoTest.h"


AISystemDemoTest::AISystemDemoTest(EntityManager& entityManager) {
	Entity aiEntity = entityManager.createEntity();
	newPos.gridX = 5;// assume this one is the ai position
	newPos.gridY = 5;// assume this one is the ai position
	newAIComponent.state = AIState::Idle;
	entityManager.registerComponentType<AIComponent>();
	entityManager.addComponent<AIComponent>(aiEntity, newAIComponent);
	entityManager.addComponent<MapComponent>(aiEntity, newMap);
	entityManager.addComponent<GridPositionComponent>(aiEntity, newPos);
}