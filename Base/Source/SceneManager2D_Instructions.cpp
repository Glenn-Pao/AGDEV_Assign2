#include "SceneManager2D_Instructions.h"


CSceneManager2D_Instructions::CSceneManager2D_Instructions()
	: m_window_width(0)
	, m_window_height(0)
{
	CSceneManager2D::CSceneManager2D();
}
CSceneManager2D_Instructions::CSceneManager2D_Instructions(const int m_window_width, const int m_window_height)
{
	this->m_window_width = m_window_width;
	this->m_window_height = m_window_height;
}

CSceneManager2D_Instructions::~CSceneManager2D_Instructions()
{
}
void CSceneManager2D_Instructions::Init()
{
	CSceneManager2D::Init();
}
void CSceneManager2D_Instructions::Update(double dt)
{
	CSceneManager2D::Update(dt);
}
void CSceneManager2D_Instructions::Render()
{
	CSceneManager2D::Render();
}
void CSceneManager2D_Instructions::Exit()
{
	CSceneManager2D::Exit();
}