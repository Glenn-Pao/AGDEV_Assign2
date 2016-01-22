#pragma once
#include "SceneManager2D.h"

class CSceneManager2D_Splash : public CSceneManager2D
{
public:
	CSceneManager2D_Splash();
	CSceneManager2D_Splash(const int m_window_width, const int m_window_height);
	virtual ~CSceneManager2D_Splash();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	int m_window_width;
	int m_window_height;
};

