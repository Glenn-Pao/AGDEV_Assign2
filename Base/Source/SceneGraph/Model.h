#pragma once

#include "Vector3.h"
#include "GL\glew.h"
#include "..\Mesh.h"
#include "Mtx44.h"

class CModel
{
private:
	Vector3 vTopLeft, vBottomRight;
	float red, green, blue;
	
	// Handle to the Mesh
	Mesh* m_cModelMesh;

	Mesh** theArrayOfMeshes;

	//current resolution
	int m_iCurrentResolution;

	bool active;

public:
	enum RESOLUTION_TYPE
	{
		RESOLUTION_LOW,
		RESOLUTION_MEDIUM,
		RESOLUTION_HIGH,
		NUM_RESOLUTIONS,
	};
	CModel(void);
	~CModel(void);

	void Init(void);
	void Draw(bool m_bLight = false);
	void SetColor(const float red, const float green, const float blue);

	//set the top left of the bounding box of the instance
	void SetTopLeft(Vector3);
	//set the bottom right of the bounding box of the instance
	void SetBottomRight(Vector3);
	void SetMesh(Mesh* mesh);

	// Get the top left of the bounding box of the instance
	Vector3 GetTopLeft(void);
	// Get the bottomright of the bounding box of the instance
	Vector3 GetBottomRight(void);

	//Get current resolution
	int GetResolution(void);

	//set current resolution
	void SetResolution(const int resolutionType);

	//active or not?
	bool GetActive(void);
	void SetActive(bool active);
};
