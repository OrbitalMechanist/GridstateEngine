#include "../headers/ui/UIController.h"

#define NS_LICENSE_NAME "Yunaan"
#define NS_LICENSE_KEY "aUm+v2UEvr8OruLsSdY8RDX82J70l50/EZrDDIIoDtocqYXH"

UIController::UIController(std::string xaml, GameMaster* gameMaster, EntityManager entityManager)
{
	gm = gameMaster;
	this->entityManager = entityManager;
	NoesisGUIInit(xaml);
	BtnInit();
	TextBlockInit();
	PlayerInfoInit();
	BtnHandlersInit();
}

const Noesis::Ptr<Noesis::IView> UIController::GetNsguiView()
{
	return nsguiView;
}

void UIController::UIReset()
{
	//NSGUI stuff should be manually shut down before exiting the program.
	nsguiView->GetRenderer()->Shutdown();
	//All Noesis::Ptr objects must be reset to free them because they are, in fact, pointers.
	nsguiView.Reset();
	xamlProvider.Reset();
	fontProvider.Reset();
	uiElement.Reset();
	Noesis::GUI::Shutdown();
}

Noesis::Button* UIController::GetturnBtn()
{
	return turnBtn;
}

Noesis::Button* UIController::GetSelectBtn()
{
	return selectBtn;
}

Noesis::Button* UIController::GetMoveBtn()
{
	return moveBtn;
}

Noesis::Button* UIController::GetAttackBtn()
{
	return attackBtn;
}

Noesis::TextBlock* UIController::GetHealthText()
{
	return healthText;
}

Noesis::TextBlock* UIController::GetMoveText()
{
	return moveText;
}

Noesis::TextBlock* UIController::GetAttackRangeText()
{
	return attackRangeText;
}

Noesis::TextBlock* UIController::GetAttackText()
{
	return attackText;
}

Noesis::TextBlock* UIController::GetArmorText()
{
	return armorText;
}

Noesis::Grid* UIController::GetPlayerInfo()
{
	return playerInfo;
}

Noesis::Image* UIController::GetMoveIcon()
{
	return movedIcon;
}

void UIController::SetHealthBar(int hp, int maxHp)
{
	Noesis::Rect newRect(0, 0, 140 * hp / maxHp, 30);
	healthBar->SetRect(newRect);
}

void UIController::HighlightSelectMode()
{
	selectBtn->SetWidth(180);
	moveBtn->SetWidth(150);
	attackBtn->SetWidth(150);
}

void UIController::HighlightMoveMode()
{
	selectBtn->SetWidth(150);
	moveBtn->SetWidth(180);
	attackBtn->SetWidth(150);
}

void UIController::HighlightAttackMode()
{
	selectBtn->SetWidth(150);
	moveBtn->SetWidth(150);
	attackBtn->SetWidth(180);
}

void UIController::DisplayInfoPanel(Entity obj)
{
	int hp = entityManager.getComponent<HealthComponent>(obj).health;
	int maxHp = entityManager.getComponent<HealthComponent>(obj).maxHealth;

	SetHealthBar(hp, maxHp);

	std::string stringVar = std::to_string(hp) + " / " + std::to_string(maxHp);
	healthText->SetText(stringVar.c_str());
	stringVar = entityManager.getComponent<NameComponent>(obj).name;
	nameText->SetText(stringVar.c_str());
	stringVar = std::to_string(entityManager.getComponent<MoveComponent>(obj).moveRange);
	moveText->SetText(stringVar.c_str());
	stringVar = std::to_string(entityManager.getComponent<AttackComponent>(obj).range);
	attackRangeText->SetText(stringVar.c_str());
	stringVar = std::to_string(entityManager.getComponent<AttackComponent>(obj).damage);
	attackText->SetText(stringVar.c_str());
	stringVar = std::to_string(entityManager.getComponent<HealthComponent>(obj).armor);
	armorText->SetText(stringVar.c_str());
	playerInfo->SetVisibility(Noesis::Visibility_Visible);
}

void UIController::HideInfoPanel()
{
	nameText->SetText("");
	healthText->SetText("");
	moveText->SetText("");
	attackRangeText->SetText("");
	attackText->SetText("");
	playerInfo->SetVisibility(Noesis::Visibility_Hidden);
}

void UIController::HideMoveIcon()
{
	movedIcon->SetVisibility(Noesis::Visibility_Hidden);
	movedIcon1->SetVisibility(Noesis::Visibility_Hidden);
}

void UIController::SetMoveIcon(bool flag)
{
	if (flag)
	{
		movedIcon1->SetVisibility(Noesis::Visibility_Visible);
		movedIcon->SetVisibility(Noesis::Visibility_Hidden);
	}
	else
	{
		movedIcon1->SetVisibility(Noesis::Visibility_Hidden);
		movedIcon->SetVisibility(Noesis::Visibility_Visible);
	}
}

void UIController::BtnHandlersInit()
{
	selectBtn->Click() += [this](Noesis::BaseComponent* sender,
		const Noesis::RoutedEventArgs& args) mutable {
			gm->switchMode(select);
			std::cout << "Select";
			HighlightSelectMode();
		};

	moveBtn->Click() += [this](Noesis::BaseComponent* sender,
		const Noesis::RoutedEventArgs& args) mutable {
			gm->switchMode(move);
			std::cout << "Move";
			HighlightMoveMode();
		};

	attackBtn->Click() += [this](Noesis::BaseComponent* sender,
		const Noesis::RoutedEventArgs& args) mutable {
			gm->switchMode(attack);
			std::cout << "Attack";
			HighlightAttackMode();
		};

	turnBtn->Click() += [this](Noesis::BaseComponent* sender,
		const Noesis::RoutedEventArgs& args) mutable {
			if (gm->currentTurn == playerTurn) {
				gm->selected = NULL;
				gm->switchMode(select);
				HighlightSelectMode();
				SetMoveIcon(false);
				gm->endTurn();
			}
			else if (gm->currentTurn == enemyTurn) {
				gm->endTurn();
			}
			else {

			}
		};
}

void UIController::NoesisGUIInit(std::string xaml)
{
	//NoesisGUI setup, seems to need to happen after the GLFW system is done setting up
	Noesis::GUI::SetLicense(NS_LICENSE_NAME, NS_LICENSE_KEY);

	Noesis::GUI::SetLogHandler([](const char*, uint32_t, uint32_t level, const char*, const char* msg)
		{
			const std::string levelText[] = { "Trace", "Debug", "Info", "Warning", "Error" };
			if (level < 3) {
				std::cout << "Noesis " << levelText[level] << " : " << msg << std::endl;
			}
			else {
				std::cerr << "Noesis " << levelText[level] << " : " << msg << std::endl;
			}
		});

	Noesis::GUI::Init();

	NoesisApp::Launcher::RegisterAppComponents();

	xamlProvider = Noesis::MakePtr<NoesisApp::LocalXamlProvider>("./assets/ui");
	fontProvider = Noesis::MakePtr<NoesisApp::LocalFontProvider>("./assets/fonts");
	textureProvider = Noesis::MakePtr<NoesisApp::LocalTextureProvider>("./assets/textures");

	NoesisApp::SetThemeProviders(xamlProvider, fontProvider, textureProvider);

	Noesis::GUI::LoadApplicationResources("Theme/NoesisTheme.DarkBlue.xaml");

	uiElement = Noesis::GUI::LoadXaml<Noesis::UserControl>(xaml.c_str());

	nsguiView = Noesis::GUI::CreateView(uiElement);
	nsguiView->SetFlags(Noesis::RenderFlags_PPAA | Noesis::RenderFlags_LCD);
	nsguiView->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	nsguiView->GetRenderer()->Init(NoesisApp::GLFactory::CreateDevice(false));
}

void UIController::BtnInit()
{
	turnBtn = nsguiView->GetContent()->FindName<Noesis::Button>("turnBtn");
	selectBtn = nsguiView->GetContent()->FindName<Noesis::Button>("selectBtn");
	moveBtn = nsguiView->GetContent()->FindName<Noesis::Button>("moveBtn");
	attackBtn = nsguiView->GetContent()->FindName<Noesis::Button>("attackBtn");
}

void UIController::TextBlockInit()
{
	nameText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("nameText");
	healthText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("healthText");
	moveText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("moveText");
	attackRangeText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("attackRangeText");
	attackText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("attackText");
	armorText = nsguiView->GetContent()->FindName<Noesis::TextBlock>("armorText");
}

void UIController::PlayerInfoInit()
{
	playerInfo = nsguiView->GetContent()->FindName<Noesis::Grid>("PlayerInfo");
	healthBar = nsguiView->GetContent()->FindName<Noesis::RectangleGeometry>("healthBar");
	movedIcon = nsguiView->GetContent()->FindName<Noesis::Image>("MoveIcon");
	movedIcon1 = nsguiView->GetContent()->FindName<Noesis::Image>("MoveIcon1");
}
