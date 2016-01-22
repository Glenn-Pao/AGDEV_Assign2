#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "menustate.h"
#include "introstate.h"
#include "playstate.h"
#include "Application.h"

CIntroState CIntroState::theIntroState;

void CIntroState::Init()
{
#if GSM_DEBUG_MODE
	cout << "CIntroState::Init\n" << endl;
	scene = new CSceneManager2D_Intro(800,600);
#endif
	scene->Init();
}

void CIntroState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	cout << "CIntroState::Init\n" << endl;

	scene = new CSceneManager2D_Intro(width, height);
#endif
	scene->Init();
}

void CIntroState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CIntroState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CIntroState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CIntroState::Pause\n" << endl;
#endif
}

void CIntroState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CIntroState::Resume\n" << endl;
#endif
}

void CIntroState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
#endif
}

void CIntroState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
#endif
	//Using the Enter button
	static bool bEnterButton = false;
	if (!bEnterButton && Application::IsKeyPressed(VK_RETURN))
	{
		bEnterButton = true;
		theGSM->ChangeState(CMenuState::Instance());
	}
	else if (bEnterButton && !Application::IsKeyPressed(VK_RETURN))
	{
		bEnterButton = false;
	}
}

void CIntroState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							   const int button_Left, const int button_Middle, const int button_Right)
{
#if GSM_DEBUG_MODE
	int m_iUserChoice = -1;
#endif
}

void CIntroState::Update(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
	cout << "CIntroState::Update\n" << endl;
#endif
}

void CIntroState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	scene->UpdateSounds(m_dElapsedTime);
}

void CIntroState::Draw(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
	cout << "CIntroState::Draw\n" << endl;
	
#endif
	// Render the scene
	scene->Render();
}
