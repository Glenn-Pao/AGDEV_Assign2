#include <iostream>
using namespace std;

#include "GameStateManager.h"
#include "gamestate.h"
#include "menustate.h"
#include "Application.h"
#include "OptionsState.h"

COptionsState COptionsState::theOptionsState;

void COptionsState::Init()
{
#if GSM_DEBUG_MODE
	cout << "COptionsState::Init\n" << endl;
	scene = new CSceneManager2D_Options(800,600);
#endif
	scene->Init();
	dew = false;
}

void COptionsState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	cout << "COptionsState::Init\n" << endl;

	scene = new CSceneManager2D_Options(width, height);
#endif
	scene->Init();
	m_Options_Button_State = BACK;
	dew = false;
}

void COptionsState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "COptionsState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void COptionsState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "COptionsState::Pause\n" << endl;
#endif
}

void COptionsState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "COptionsState::Resume\n" << endl;
#endif
}

void COptionsState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
#endif
}

void COptionsState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
	switch (m_Options_Button_State)
	{
	case BACK:
		cout << "BACK" << endl;
		break;
	}
#endif
	//Using the down button
	static bool bUpButton = false;
	if (!bUpButton && Application::IsKeyPressed(VK_UP))
	{
		bUpButton = true;
		switch (m_Options_Button_State)
		{
		case BACK:
			m_Options_Button_State = MOUNTAIN_DEW;
			break;
		case NORMAL:
			m_Options_Button_State = BACK;
			break;
		case MOUNTAIN_DEW:
			m_Options_Button_State = NORMAL;
			break;
		}
	}
	else if (bUpButton && !Application::IsKeyPressed(VK_UP))
	{
		bUpButton = false;
	}

	//Using the up button
	static bool bDownButton = false;
	if (!bDownButton && Application::IsKeyPressed(VK_DOWN))
	{
		bDownButton = true;

		switch (m_Options_Button_State)
		{
		case BACK:
			m_Options_Button_State = NORMAL;
			break;
		case NORMAL:
			m_Options_Button_State = MOUNTAIN_DEW;
			break;
		case MOUNTAIN_DEW:
			m_Options_Button_State = BACK;
			break;
		}
	}
	else if (bDownButton && !Application::IsKeyPressed(VK_DOWN))
	{
		bDownButton = false;
	}
	
	//Using enter button
	static bool bEnterButton = false;
	if (!bEnterButton && Application::IsKeyPressed(VK_RETURN))
	{
		switch (m_Options_Button_State)
		{
		case BACK:
			theGSM->ChangeState(CMenuState::Instance());	//back to main menu
			break;
		case MOUNTAIN_DEW:
			theGSM->ChangeState(CMenuState::Instance());	//back to main menu
			dew = true;
			break;
		case NORMAL:
			theGSM->ChangeState(CMenuState::Instance());	//back to main menu
			dew = false;
			break;
		}

	}
	else if (bEnterButton && !Application::IsKeyPressed(VK_RETURN))
	{
		bEnterButton = false;
	}
}

void COptionsState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							   const int button_Left, const int button_Middle, const int button_Right)
{
#if GSM_DEBUG_MODE
#endif
}

void COptionsState::Update(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
	cout << "COptionsState::Update\n" << endl;
#endif
}

void COptionsState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
}

void COptionsState::Draw(CGameStateManager* theGSM) 
{
#if GSM_DEBUG_MODE
#endif
	// Render the scene
	scene->Render();

	if (m_Options_Button_State == NORMAL)
	{
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Normal", Color(1, 1, 1), 85, 250, 300);
	}
	else
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Normal", Color(1, 0, 0), 80, 250, 300);

	if (m_Options_Button_State == MOUNTAIN_DEW)
	{
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Mountain Dew", Color(1, 1, 1), 85, 250, 250);
	}
	else
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Mountain Dew", Color(1, 0, 0), 80, 250, 250);

	if (m_Options_Button_State == BACK)
	{
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Back", Color(1, 1, 1), 85, 250, 200);
	}
	else
		scene->RenderTextOnScreen(scene->meshList[scene->GEO_TEXT], "Back", Color(1, 0, 0), 80, 250, 200);
}
void COptionsState::SetOptionsButtonState(COptionsState::OPTIONS_BUTTON_STATE m_Options_Button_State)
{
	this->m_Options_Button_State = m_Options_Button_State;
}
COptionsState::OPTIONS_BUTTON_STATE COptionsState::GetOptionsButtonState(void)
{
	return m_Options_Button_State;
}