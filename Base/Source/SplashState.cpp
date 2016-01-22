#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "introstate.h"
#include "SplashState.h"
#include "playstate.h"
#include "Application.h"

CSplashState CSplashState::theSplashState;

void CSplashState::Init()
{
#if GSM_DEBUG_MODE
	cout << "CSplashState::Init\n" << endl;
#endif
	scene = new CSceneManager2D_Splash(800, 600);
	scene->Init();
	scene->setOffset(-700.f);
	canPress = false;
}

void CSplashState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	cout << "CSplashState::Init\n" << endl;

	scene = new CSceneManager2D_Splash(width, height);
#endif
	scene->Init();
}

void CSplashState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CSplashState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CSplashState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CSplashState::Pause\n" << endl;
#endif
}

void CSplashState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CSplashState::Resume\n" << endl;
#endif
}

void CSplashState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
#endif
}

void CSplashState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
#endif
	if (scene->getOffset() > 0)
	{
		canPress = true;
	}
	else
	{
		canPress = false;
	}
	//Using the Enter button
	static bool bEnterButton = false;
	if (!bEnterButton && Application::IsKeyPressed(VK_RETURN) && canPress)
	{
		bEnterButton = true;
		theGSM->ChangeState(CIntroState::Instance());
	}
	else if (bEnterButton && !Application::IsKeyPressed(VK_RETURN))
	{
		bEnterButton = false;
	}
}

void CSplashState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							   const int button_Left, const int button_Middle, const int button_Right)
{
#if GSM_DEBUG_MODE
	int m_iUserChoice = -1;
#endif
}

void CSplashState::Update(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
	cout << "CSplashState::Update\n" << endl;
#endif
}

void CSplashState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	scene->Update(m_dElapsedTime);
}

void CSplashState::Draw(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
	//cout << "CSplashState::Draw\n" << endl;
	
#endif
	// Render the scene
	scene->Render();
}
