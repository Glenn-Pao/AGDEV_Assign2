#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "PlayInfo3PV.h"
#include "Projectile/ProjectileManager.h"
#include "Map.h"
#include "Weapon.h"
const float SKYBOXSIZE = 1000.f;

class CSceneManager : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};
	enum GEOMETRY_TYPE
	{
		GEO_RAY,
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_SPHERE,
		GEO_SKYPLANE,
		GEO_QUAD,
		GEO_CUBE,
		GEO_BULLET,
		GEO_AMMO,
		GEO_PISTOL,
		GEO_PISTOL_XHAIR,
		GEO_MG,
		GEO_MG_XHAIR,
		GEO_LG,
		GEO_AMMO_BOX,
		GEO_RELOAD,
		GEO_EMPTY,
		GEO_SCORE,
		GEO_SPATIAL_GRID,
		GEO_SPATIAL_FILLED,
		GEO_SPATIAL_GRID_ICON_FILLED,
		GEO_SPATIAL_GRID_ICON,
		GEO_TILE,
		GEO_TEXT,
		NUM_GEOMETRY,
	};

public:
	CSceneManager(void);
	CSceneManager(const int m_window_width, const int m_window_height);
	~CSceneManager(void);

	virtual void Init();
	virtual void Update(double dt);
	// Update Camera status
	virtual void UpdateCameraStatus(const unsigned char key, const bool status = true);
	// Update Avatar position
 	virtual void UpdateAvatarStatus(const unsigned char key, const bool status = true);
	// Update Weapon status
	virtual void UpdateWeaponStatus(const unsigned char key);
	virtual void Render();
	virtual void Exit();

	// Low level render tools
	void PreRendering(Vector3 translate, bool enableLight);
	void PreRendering(Mtx44 rotate, bool enableLight);
	void PostRendering(void);

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size=1.0f, float x=0.0f, float y=0.0f, bool rotate=false);
	void RenderMesh(Mesh *mesh, bool enableLight);

	// Render the main components of this scene
	void RenderGUI();
	void RenderMobileObjects();
	void RenderFixedObjects();
	void RenderLights();
	void RenderGround();
	void RenderSkyPlane(Mesh* mesh, Color color, int slices, float
		PlanetRadius, float AtmosphereRadius, float hTile, float vTile);

	enum WEAPON_ACTION
	{
		WA_NIL = 0,
		WA_FIRE,
		WA_FIRE_SECONDARY,
		WA_RELOAD,
		WA_CHANGEWEAPON,
		WA_TOTAL,
	};

	void InitPistol();		//initialize pistol
	void UpdatePistol(double dt);

	void InitMG();	//initialize machine gun
	void UpdateMG(double dt);

	void InitLG();		//initialize laser gun
	void UpdateLG(double dt);

	void SwapWeapon(double dt);	//swap weapons

	void UpdateParticles(double dt);	 //this includes projectiles, static items and anything in between
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	// Handler to the camera
	Camera3 camera;

	// Rotation angle for use with the model stack
	float rotateAngle;

	// Matrices for Model, View and Projection stack
	MS modelStack;
	MS viewStack;
	MS projectionStack;

	// Window size
	int m_window_width;
	int m_window_height;

	Light lights[2];
	bool bLightEnabled;

	float fps;

	// Handle to 3rd Person View's avatar
	CPlayInfo3PV* m_cAvatar;

	// Projectile
	CProjectileManager* m_cProjectileManager;

	float timer;
	int score;

	CWeapon *m_cPistol;	//pistol
	CWeapon *m_cMG;		//machine gun
	CWeapon *m_cLG;		//laser gun
};

#endif