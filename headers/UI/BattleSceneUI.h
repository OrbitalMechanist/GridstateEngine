#pragma once
#include <string>
#include <NoesisPCH.h>

using namespace Noesis;

class BattleSceneUI
{
public:
	void SetnsguiView(Noesis::Ptr<Noesis::IView> nsguiView);

	const std::string GetEnemyName();
	const float GetEnemyHP();
	const float GetMaxEnemyHP();
	const float GetPlayerHP();
	const float GetPlayerMana();
	const float GetMaxPlayerHP();
	const float GetMaxPlayerMP();

	void SetEnemyName(std::string enemyName);
	void SetEnemyHP(float enemyHP);
	void SetMaxEnemyHP(float maxEnemyMP);
	void SetPlayerHP(float playerHP);
	void SetPlayerMP(float playerMP);
	void SetMaxPlayerHP(float maxPlayerHP);
	void SetMaxPlayerMP(float maxPlayerMP);

private:
	std::string _enemyName;
	float _enemyHP, _maxEnemyHP, _playerHP, _playerMP, _maxPlayerHP, _maxPlayerMP;
	Noesis::Ptr<Noesis::IView> _nsguiView;
};