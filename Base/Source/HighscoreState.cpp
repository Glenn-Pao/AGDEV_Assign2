#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "menustate.h"
#include "Application.h"
#include "HighscoreState.h"

CHighscoreState CHighscoreState::theHighscoreState;

void CHighscoreState::Init()
{
#if GSM_DEBUG_MODE
	cout << "CHighscoreState::Init\n" << endl;
	scene = new CSceneManager2D_Highscore(800,600);
#endif
	scene->Init();
}

void CHighscoreState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	cout << "CHighscoreState::Init\n" << endl;

	scene = new CSceneManager2D_Highscore(width, height);
#endif
	scene->Init();
	m_Highscore_Button_State = BACK;
}
void CHighscoreState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CHighscoreState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CHighscoreState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CHighscoreState::Pause\n" << endl;
#endif
}

void CHighscoreState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CHighscoreState::Resume\n" << endl;
#endif
}

void CHighscoreState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
#endif
}

void CHighscoreState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
	/*switch (m_Highscore_Button_State)
	{
	case BACK:
		cout << "BACK" << endl;
		break;
	}*/
#endif
	//Using the down button
	static bool bLeftButton = false;
	if (!bLeftButton && Application::IsKeyPressed(VK_LEFT))
	{
		bLeftButton = true;
		switch (m_Highscore_Button_State)
		{
		case BACK:
			m_Highscore_Button_State = BACK;
			break;
		}
	}
	else if (bLeftButton && !Application::IsKeyPressed(VK_LEFT))
	{
		bLeftButton = false;
	}

	//Using the up button
	static bool bRightButton = false;
	if (!bRightButton && Application::IsKeyPressed(VK_RIGHT))
	{
		bRightButton = true;

		switch (m_Highscore_Button_State)
		{
		case BACK:
			m_Highscore_Button_State = BACK;
			break;
		}
	}
	else if (bRightButton && !Application::IsKeyPressed(VK_RIGHT))
	{
		bRightButton = false;
	}
	
	//Using enter button
	static bool bEnterButton = false;
	if (!bEnterButton && Application::IsKeyPressed(VK_RETURN))
	{
		switch (m_Highscore_Button_State)
		{
		case BACK:
			theGSM->ChangeState(CMenuState::Instance());	//back to main menu
			break;
		}

	}
	else if (bEnterButton && !Application::IsKeyPressed(VK_RETURN))
	{
		bEnterButton = false;
	}
}

void CHighscoreState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							   const int button_Left, const int button_Middle, const int button_Right)
{
#if GSM_DEBUG_MODE
#endif
}

void CHighscoreState::Update(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
	cout << "CHighscoreState::Update\n" << endl;
#endif
}

void CHighscoreState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
}

void CHighscoreState::Draw(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
#endif
	// Render the scene
	scene->Render();
	if (m_Highscore_Button_State == BACK)
	{
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Back", Color(1, 1, 1), 85, 5, 0);
	}

}
void CHighscoreState::SetHighscoreButtonState(CHighscoreState::HIGHSCORE_BUTTON_STATE)
{
	this->m_Highscore_Button_State = m_Highscore_Button_State;
}
CHighscoreState::HIGHSCORE_BUTTON_STATE CHighscoreState::GetHighscoreButtonState(void)
{
	return m_Highscore_Button_State;
}