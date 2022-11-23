#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "InputType.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

glm::vec2 AngleLengthToVector(float angle, float magnitude)
{
	return glm::vec2(cos(angle * Util::Deg2Rad) * magnitude, sin(angle * Util::Deg2Rad) * magnitude);
}

void PlayScene::Draw()
{
	DrawDisplayList();

	glm::vec2 velocityVectorEndPoint = m_launchOrigin + AngleLengthToVector(m_angle, m_speed); //  glm::vec2(cos(m_angle * Util::Deg2Rad) * m_speed, sin(m_angle) * m_speed);


	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 125, 255);
	SDL_RenderDrawLineF(Renderer::Instance().GetRenderer(), m_launchOrigin.x, m_launchOrigin.y, 100, 100);


	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 225, 255);
}

void PlayScene::Update()
{

	y1 = c + sin(t * a) * b;
	x1 = d + cos(t * a) * b;
	(*m_pPlaneSprite).GetTransform()->position = glm::vec2(x1, y1);
	UpdateDisplayList();
}

void PlayScene::Clean()
{
	RemoveAllChildren();
}


void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();

	GetPlayerInput();

	GetKeyboardInput();
}


void PlayScene::GetKeyboardInput()
{
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::START);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Set Input Type
	m_pCurrentInputType = static_cast<int>(InputType::KEYBOARD_MOUSE);
	
	// Plane Sprite
	m_pPlaneSprite = new Plane();
	AddChild(m_pPlaneSprite);

	// Player Sprite
	m_pPlayer = new Player();
	AddChild(m_pPlayer);
	
	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", GameObjectType::BACK_BUTTON);
	m_pBackButton->GetTransform()->position = glm::vec2(300.0f, 400.0f);
	m_pBackButton->AddEventListener(Event::CLICK, [&]()-> void
	{
		m_pBackButton->SetActive(false);
		Game::Instance().ChangeSceneState(SceneState::START);
	});

	m_pBackButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
	{
		m_pBackButton->SetAlpha(128);
	});

	m_pBackButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
	{
		m_pBackButton->SetAlpha(255);
	});
	AddChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", GameObjectType::NEXT_BUTTON);
	m_pNextButton->GetTransform()->position = glm::vec2(500.0f, 400.0f);
	m_pNextButton->AddEventListener(Event::CLICK, [&]()-> void
	{
		m_pNextButton->SetActive(false);
		Game::Instance().ChangeSceneState(SceneState::END);
	});

	m_pNextButton->AddEventListener(Event::MOUSE_OVER, [&]()->void
	{
		m_pNextButton->SetAlpha(128);
	});

	m_pNextButton->AddEventListener(Event::MOUSE_OUT, [&]()->void
	{
		m_pNextButton->SetAlpha(255);
	});

	AddChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->GetTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	AddChild(m_pInstructionsLabel);

	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Your Window Title Goes Here", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Text("Player Input");
	ImGui::RadioButton("Keyboard / Mouse", &m_pCurrentInputType, static_cast<int>(InputType::KEYBOARD_MOUSE)); ImGui::SameLine();
	ImGui::RadioButton("Game Controller", &m_pCurrentInputType, static_cast<int>(InputType::GAME_CONTROLLER)); ImGui::SameLine();
	ImGui::RadioButton("Both", &m_pCurrentInputType, static_cast<int>(InputType::ALL));

	ImGui::Separator();

	if(ImGui::Button("My Button"))
	{
		std::cout << "My Button Pressed" << std::endl;
	}

	ImGui::Separator();

	ImGui::SliderFloat("Launch Angle", &m_angle, 0.0f, 360.0f);
	ImGui::SliderFloat("Launch Speed", &m_speed, 0.0f, 1000.0f);
	ImGui::SliderFloat2("Launch Origin", &(m_launchOrigin.x), 0.0f, 800.0f);


	glm::vec2 velocity = AngleLengthToVector(m_angle, m_speed);
	ImGui::LabelText("Velocity Vector", "x:%f, y:%f", velocity.x, velocity.y);
	
	ImGui::End();
}
