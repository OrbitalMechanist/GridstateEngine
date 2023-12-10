#pragma once
#include <string>
#include <iostream>

#include "Constants.h"
#include <gamemaster/GameMaster.h>

//Noesis stuff, not all of this may be needed
#include <NoesisPCH.h>
#include <NsApp/Launcher.h>
#include <NsApp/LocalXamlProvider.h>
#include <NsApp/LocalFontProvider.h>
#include <NsApp/LocalTextureProvider.h>
#include <NsApp/ThemeProviders.h>
#include <NsRender/GLFactory.h>

using namespace Noesis;

class UIController
{
public:
	UIController(std::string xaml, GameMaster* gm, EntityManager entityManager);
	const Noesis::Ptr<Noesis::IView> GetNsguiView();
	void UIReset();
	Noesis::Button* GetturnBtn();
	Noesis::Button* GetSelectBtn();
	Noesis::Button* GetMoveBtn();
	Noesis::Button* GetAttackBtn();
	Noesis::TextBlock* GetHealthText();
	Noesis::TextBlock* GetMoveText();
	Noesis::TextBlock* GetAttackRangeText();
	Noesis::TextBlock* GetAttackText();
	Noesis::TextBlock* GetArmorText();
	Noesis::TextBlock* GetCanMoveText();
	Noesis::Grid* GetPlayerInfo();
	Noesis::Ellipse* GetMoveIcon();

	void SetHealthBar(int hp, int maxHp);
	void HighlightSelectMode();
	void HighlightMoveMode();
	void HighlightAttackMode();
	void DisplayInfoPanel(Entity obj);
	void HideInfoPanel();
	void SetMoveIcon(bool flag);

private:
	Noesis::Ptr<Noesis::IView> nsguiView;
	Noesis::Ptr<NoesisApp::LocalXamlProvider> xamlProvider;
	Noesis::Ptr<NoesisApp::LocalFontProvider> fontProvider;
	Noesis::Ptr<NoesisApp::LocalTextureProvider> textureProvider;
	Noesis::Ptr<Noesis::UserControl> uiElement;
	GameMaster* gm;
	EntityManager entityManager;

	Noesis::Button *turnBtn, *selectBtn, *moveBtn, *attackBtn;
	Noesis::TextBlock *nameText, *healthText, *moveText, *attackRangeText, *attackText, *armorText, *canMoveText;
	Noesis::Grid* playerInfo;
	Noesis::RectangleGeometry* healthBar;
	//Noesis::Image* movedIcon;

	void NoesisGUIInit(std::string xaml);
	void BtnInit();
	void TextBlockInit();
	void PlayerInfoInit();
	void BtnHandlersInit();
};