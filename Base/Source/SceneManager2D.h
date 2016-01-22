#ifndef SCENE_MANAGER_2D_H
#define SCENE_MANAGER_2D_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Vector2.h"

class CSceneManager2D : public Scene
{
public:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_LIGHTENABLED,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_BACKGROUND,
		GEO_SPLASH,
		GEO_SPLASH_MASK,
		GEO_SPLASH_FINAL,
		GEO_INTRO,
		GEO_OPTIONS,
		GEO_MAIN_MENU,
		GEO_INSTRUCTIONS_1,
		GEO_INSTRUCTIONS_2,
		GEO_HIGHSCORE,
		GEO_OBJECT,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

public:
	CSceneManager2D();
	CSceneManager2D(const int m_window_width, const int m_window_height);
	virtual ~CSceneManager2D();

	virtual void Init();
	virtual void Update(double dt);
	// Update Camera status
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true);
	// Update Weapon status
	virtual void UpdateWeaponStatus(const unsigned char key);
	virtual void Render();
	virtual void Exit();

	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderBackground();
	void Render2DMesh(Mesh *mesh, const bool enableLight, const int size=1, const int x=0, const int y=0, const bool rotate=false, const bool flip=false);

	enum WEAPON_ACTION
	{
		WA_NIL = 0,
		WA_FIRE,
		WA_RELOAD,
		WA_CHANGEWEAPON,
		WA_TOTAL,
	};
	
	void LoadHighscore();

	void RenderHighscore();
	void RenderSplash();
	void RenderIntro();
	void RenderMenu();
	void RenderOptions();
	void RenderInstructions1();
	void RenderInstructions2();
	Mesh* meshList[NUM_GEOMETRY];

	void UpdateSounds(double dt);

	void setOffset(float offset);
	float getOffset(void);
private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	float rotateAngle;

	MS modelStack;
	MS viewStack;
	MS projectionStack;


	float fps;
	// Window size
	int m_window_width;
	int m_window_height;

	float offset;
	int m_highscore[10];
};

#endif