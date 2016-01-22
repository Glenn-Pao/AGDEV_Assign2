#pragma once
#include "node.h"
#include "Model.h"
#include "Vector2.h"
#include "Transform.h"
//#include <list>

#include <vector>
using namespace std;

class CSceneManager;

class CSceneNode :
	public CNode
{
private:
	vector<CNode*> theChildren;

	CModel* theModel;
	CTransform* theTransform;
	int sceneNodeID;

	//check where a line segment between 2 position intersect a plane
	int GetIntersection(float fDist1, float fDist2, Vector3 p1, Vector3 p2, Vector3 &hit);
	//check 2 position are within a box reegion
	int InBox(Vector3 hit, Vector3 b1, Vector3 b2, const int axis);

	Vector2 gridIndex;
	Vector3 translate;

public:
	CSceneNode(void);
	CSceneNode(const int sceneNodeID);
	~CSceneNode(void);
	
	//upadte the sceneNode
	void Update(const int resolutionType);

	// Draw this Node and its children
	void Draw(void);
	void Draw(CSceneManager* theSceneManager);

	// Set model and other info to this Node
	void SetModel(CModel* theModel);
	int SetNode(CTransform* aNewTransform, CModel* aNewModel);
	void SetSceneNodeID(const int sceneNodeID);
	void SetTranslate(const float dx, const float dy, const float dz);
	void SetScale(const float dx, const float dy, const float dz);

	int AddChild(CTransform* aNewTransform, CModel* aNewModel);
	int AddChild(const int sceneNodeID, CTransform* aNewTransform, CModel* aNewModel);
	//update the childGrid index
	void SetChildGridIndex(void);

	// Get methods
	int GetSceneNodeID(void);
	CSceneNode* GetNode(const int sceneNodeID);

	// Apply transformation to this Node and its children
	void ApplyTranslate( const float dx, const float dy, const float dz );
	void ApplyRotate( const float angle, const float rx, const float ry, const float rz );
	void ApplyScale(const float dx, const float dy, const float dz);

	//get noode current position
	Vector3 GetTranslate(void);

	//set the gridIndex for current Node;
	void SetGridIndex(const int x, const int y);

	//get current node grid index
	Vector2 GetGridIndex(void);

	// Get top left corner of the group
	Vector3 GetTopLeft(void);
	// Get bottom right corner of the group
	Vector3 GetBottomRight(void);

	// Set colour of the instance
	void SetColor(const float red, const float green, const float blue);

	// Return the number of children in this group
	int GetNumOfChild(void);
	// Get top left corner of a child
	bool GetTopLeft(const int m_iChildIndex, Vector3& vector3D_TopLeft);
	// Get bottom right corner of a child
	bool GetBottomRight(const int m_iChildIndex, Vector3& vector3D_BottomRight);

	// Set the Colour of a child
	void SetColorForChild(const int m_iChildIndex, const float red, const float green, const float blue);

	//check a position for collision with objects in any of the grids
	bool CheckForCollision(Vector3 position);

	//check 2 position for collision with any objects in any of the grids
	int CheckForCollision(Vector3 position_start, Vector3 position_end, Vector3 &Hit);

	// PrintSelf
	void PrintSelf(void);
};


struct Base_SCENENODE
{
	CSceneNode* node;
};