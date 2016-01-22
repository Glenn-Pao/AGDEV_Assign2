#pragma once
#include "SceneManager2D.h"

class CSceneManager2D_Highscore : public CSceneManager2D
{
public:
	CSceneManager2D_Highscore();
	CSceneManager2D_Highscore(const int m_window_width, const int m_window_height);
	virtual ~CSceneManager2D_Highscore();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	int m_window_width;
	int m_window_height;
};

