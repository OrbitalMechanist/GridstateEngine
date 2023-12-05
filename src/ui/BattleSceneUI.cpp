#include "../headers/UI/BattleSceneUI.h"

void BattleSceneUI::SetnsguiView(Noesis::Ptr<Noesis::IView> nsguiView)
{
	_nsguiView = nsguiView;
}

const std::string BattleSceneUI::GetEnemyName()
{
	return _enemyName;
}

const float BattleSceneUI::GetEnemyHP()
{
	return _enemyHP;
}

const float BattleSceneUI::GetMaxEnemyHP()
{
	return _maxEnemyHP;
}

const float BattleSceneUI::GetPlayerHP()
{
	return _playerHP;
}

const float BattleSceneUI::GetPlayerMana()
{
	return _playerMP;
}

const float BattleSceneUI::GetMaxPlayerHP()
{
	return _maxPlayerHP;
}

const float BattleSceneUI::GetMaxPlayerMP()
{
	return _maxPlayerMP;
}

const int BattleSceneUI::GetEnemyNum()
{
	return _enemyNum;
}

void BattleSceneUI::SetEnemyName(std::string enemyName)
{
	_enemyName = enemyName;

	if (_nsguiView->GetContent()->FindName<Noesis::TextBlock>("enemyName"))
	{
		_nsguiView->GetContent()->FindName<Noesis::TextBlock>("enemyName")->SetText(_enemyName.c_str());
	}
}

void BattleSceneUI::SetEnemyHP(float enemyHP)
{
	_enemyHP = enemyHP;

	if (_nsguiView->GetContent()->FindName<Noesis::RectangleGeometry>("enemyHP"))
	{
		Noesis::Rect newRect(((_maxEnemyHP - _enemyHP) / _maxEnemyHP) * 200, 0, 200, 30);
		_nsguiView->GetContent()->FindName<Noesis::RectangleGeometry>("enemyHP")->SetRect(newRect);
	}
}

void BattleSceneUI::SetMaxEnemyHP(float maxEnemyHP)
{
	_maxEnemyHP = maxEnemyHP;
}

void BattleSceneUI::SetPlayerHP(float playerHP)
{
	_playerHP = playerHP;

	if (_nsguiView->GetContent()->FindName<Noesis::RectangleGeometry>("palyerHP"))
	{
		Noesis::Rect newRect(0, ((_maxPlayerHP - _playerHP) / _maxPlayerHP) * 100, 100, 100);
		_nsguiView->GetContent()->FindName<Noesis::RectangleGeometry>("palyerHP")->SetRect(newRect);
	}
}

void BattleSceneUI::SetPlayerMP(float playerMP)
{
	_playerMP = playerMP;

	if (_nsguiView->GetContent()->FindName<Noesis::RectangleGeometry>("palyerMP"))
	{
		Noesis::Rect newRect(0, ((_maxPlayerMP - _playerMP) / _maxPlayerMP) * 100, 100, 100);
		_nsguiView->GetContent()->FindName<Noesis::RectangleGeometry>("palyerMP")->SetRect(newRect);
	}
}

void BattleSceneUI::SetMaxPlayerHP(float maxPlayerHP)
{
	if (maxPlayerHP > 0)
	{
		_maxPlayerHP = maxPlayerHP;
	}
}

void BattleSceneUI::SetMaxPlayerMP(float maxPlayerMP)
{
	if (maxPlayerMP > 0)
	{
		_maxPlayerMP = maxPlayerMP;
	}
}

void BattleSceneUI::DisplayPlayerUI(bool flag)
{
	if (flag && _nsguiView->GetContent()->FindName<Noesis::Grid>("playerUI"))
	{
		_nsguiView->GetContent()->FindName<Noesis::Grid>("playerUI")->SetVisibility(Visibility_Visible);
	}
	else
	{
		_nsguiView->GetContent()->FindName<Noesis::Grid>("playerUI")->SetVisibility(Visibility_Hidden);
	}
}

void BattleSceneUI::DisplayEnemyInfoUI(bool flag)
{
	if (flag && _nsguiView->GetContent()->FindName<Noesis::Grid>("enemyInfo"))
	{
		_nsguiView->GetContent()->FindName<Noesis::Grid>("enemyInfo")->SetVisibility(Visibility_Visible);
	}
	else
	{
		_nsguiView->GetContent()->FindName<Noesis::Grid>("enemyInfo")->SetVisibility(Visibility_Hidden);
	}
}

void BattleSceneUI::SetEnemyNum(int enemyNum)
{
	_enemyNum = enemyNum;

	if (_nsguiView->GetContent()->FindName<Noesis::Run>("ennmyNum"))
	{
		_nsguiView->GetContent()->FindName<Noesis::Run>("ennmyNum")->SetText(std::to_string(_enemyNum).c_str());
	}
}
