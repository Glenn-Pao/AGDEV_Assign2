#include <iostream>
using namespace std;

#include "gamestate.h"
#include "GameStateManager.h"
#include "playstate.h"
#include "menustate.h"

CPlayState CPlayState::thePlayState;

void CPlayState::Init()
{
#if GSM_DEBUG_MODE
	cout << "CPlayState::Init\n" << endl;
#endif
	counter = 0;

	#if TYPE_OF_VIEW == 3
		scene = new CSceneManager(800, 600);	// Use this for 3D gameplay
	#else
		scene = new CSceneManager2D(800, 600);	// Use this for 2D gameplay
	#endif
	scene->Init();
}

void CPlayState::Init(const int width, const int height)
{
#if GSM_DEBUG_MODE
	cout << "CPlayState::Init\n" << endl;
#endif
	counter = 0;

	#if TYPE_OF_VIEW == 3
		scene = new CSceneManager(width, height);	// Use this for 3D gameplay
	#else
		scene = new CSceneManager2D(width, height);	// Use this for 2D gameplay
	#endif
	scene->Init();
}

void CPlayState::Cleanup()
{
#if GSM_DEBUG_MODE
	cout << "CPlayState::Cleanup\n" << endl;
#endif
	// Delete the scene
	scene->Exit();
	delete scene;
	scene = NULL;
}

void CPlayState::Pause()
{
#if GSM_DEBUG_MODE
	cout << "CPlayState::Pause\n" << endl;
#endif
	if (bPaused)
	{

	}
}

void CPlayState::Resume()
{
#if GSM_DEBUG_MODE
	cout << "CPlayState::Resume\n" << endl;
#endif
}

void CPlayState::HandleEvents(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
#endif
}

void CPlayState::HandleEvents(CGameStateManager* theGSM, const unsigned char key, const bool status)
{
#if GSM_DEBUG_MODE
#endif
	if (key == 32)//space//all the keypress is constrolled here
	{
		theGSM->ChangeState( CMenuState::Instance() );
	}
	else
	{
		scene->UpdateAvatarStatus( key, status );
	}
}

void CPlayState::HandleEvents(CGameStateManager* theGSM, const double mouse_x, const double mouse_y,
							  const int button_Left, const int button_Middle, const int button_Right)
{
#if GSM_DEBUG_MODE
#endif
	if (button_Left == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE);
	if (button_Right == true)
		scene->UpdateWeaponStatus(scene->WA_FIRE_SECONDARY);
}


void CPlayState::Update(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	cout << "CPlayState::Update\n" << endl;
#endif
	if (!bPaused)
	{
		// Update the scene
		scene->Update(0.16667);
	}
}

void CPlayState::Update(CGameStateManager* theGSM, const double m_dElapsedTime)
{
	if (!bPaused)
	{
		// Update the scene
		scene->Update(m_dElapsedTime);
	}
}

void CPlayState::Draw(CGameStateManager* theGSM)
{
#if GSM_DEBUG_MODE
	//cout << "CPlayState::Draw : " << counter << "\n" << endl;
#endif
	if (!bPaused)
	{
		// Render the scene
		scene->Render();
	}
}
	 

