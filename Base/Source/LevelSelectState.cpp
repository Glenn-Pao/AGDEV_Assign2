#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "menustate.h"
#include "playstate.h"
#include "Application.h"
#include "LevelSelectState.h"

CLevelSelectState CLevelSelectState::theLevelSelectState;

void CLevelSelectState::Init()
{
#if GSM_DEBUG_MODE
	cout << "CLevelSelectState::Init\n" << endl;
	scene = new CSceneManager2D_Menu(800,600);
#endif
	scene->Init();
}

void CLevelSelectState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	cout << "CLevelSelectState::Init\n" << endl;

	scene = new CSceneManager2D_Menu(width, height);
#endif
	scene->Init();
}

void CLevelSelectState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CLevelSelectState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CLevelSelectState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CLevelSelectState::Pause\n" << endl;
#endif
}

void CLevelSelectState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CLevelSelectState::Resume\n" << endl;
#endif
}

void CLevelSelectState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
#endif
}

void CLevelSelectState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
	/*switch (m_Level_Select_State)
	{
	case NORMAL:
		cout << "NORMAL" << endl;
		break;
	case HARD:
		cout << "HARD" << endl;
		break;
	case BACK:
		cout << "BACK" << endl;
		break;
	}*/
#endif
	//Using the down button
	static bool bDownButton = false;
	if (!bDownButton && Application::IsKeyPressed(VK_DOWN))
	{
		bDownButton = true;

		switch (m_Level_Select_State)
		{
		case NORMAL:
			m_Level_Select_State = HARD;
			break;
		case HARD:
			m_Level_Select_State = BACK;
			break;
		case BACK:
			m_Level_Select_State = NORMAL;
			break;
		}
	}
	else if (bDownButton && !Application::IsKeyPressed(VK_DOWN))
	{
		bDownButton = false;
	}

	//Using the up button
	static bool bUpButton = false;
	if (!bUpButton && Application::IsKeyPressed(VK_UP))
	{
		bUpButton = true;

		switch (m_Level_Select_State)
		{
		case NORMAL:
			m_Level_Select_State = BACK;
			break;
		case HARD:
			m_Level_Select_State = NORMAL;
			break;
		case BACK:
			m_Level_Select_State = HARD;
			break;
		}
	}
	else if (bUpButton && !Application::IsKeyPressed(VK_UP))
	{
		bUpButton = false;
	}
	
	//Using enter button
	static bool bEnterButton = false;
	if (!bEnterButton && Application::IsKeyPressed(VK_RETURN))
	{
		switch (m_Level_Select_State)
		{
		case NORMAL:
			theGSM->ChangeState(CPlayState::Instance());
			break;
		case HARD:
			theGSM->ChangeState(CPlayState::Instance());
			break;
		case BACK:
			theGSM->ChangeState(CMenuState::Instance());	//go back to main menu
		}
	}
	else if (bEnterButton && !Application::IsKeyPressed(VK_RETURN))
	{
		bEnterButton = false;
	}
}

void CLevelSelectState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							   const int button_Left, const int button_Middle, const int button_Right)
{
#if GSM_DEBUG_MODE
#endif
}

void CLevelSelectState::Update(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
	cout << "CLevelSelectState::Update\n" << endl;
#endif
}

void CLevelSelectState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
}

void CLevelSelectState::Draw(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
#endif
	// Render the scene
	scene->Render();

	if (m_Level_Select_State == NORMAL)
	{
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Normal", Color(1, 1, 1), 85, 5, 250);
	}
	else
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Normal", Color(1, 0, 0), 80, 5, 250);

	if (m_Level_Select_State == HARD)
	{
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Hard", Color(1, 1, 1), 85, 5, 200);
	}
	else
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Hard", Color(1, 0, 0), 80, 5, 200); 

	if (m_Level_Select_State == BACK)
	{
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Back", Color(1, 1, 1), 85, 5, 150);
	}
	else
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Back", Color(1, 0, 0), 80, 5, 150);
}
void CLevelSelectState::SetLevelSelectButtonState(CLevelSelectState::LEVEL_SELECT m_Level_Select_State)
{
	this->m_Level_Select_State = m_Level_Select_State;
}
CLevelSelectState::LEVEL_SELECT CLevelSelectState::GetLevelSelectButtonState(void)
{
	return m_Level_Select_State;
}