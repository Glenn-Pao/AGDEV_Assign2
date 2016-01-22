#pragma once
#include "SceneManager2D.h"

class CSceneManager2D_Intro : public CSceneManager2D
{
public:
	CSceneManager2D_Intro();
	CSceneManager2D_Intro(const int m_window_width, const int m_window_height);
	virtual ~CSceneManager2D_Intro();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	int m_window_width;
	int m_window_height;
};

