#include "SceneNode.h"
#include "../SceneManager.h"
#include "Vector3.h"

#include <cstdlib>
#include <iostream>
CSceneNode::CSceneNode(void)
: theModel( NULL )
, theChildren( NULL )
, theTransform ( NULL )
, sceneNodeID (-1)
, gridIndex(Vector2(0,0))
{
}

CSceneNode::CSceneNode(const int sceneNodeID)
{
	CSceneNode();
	SetSceneNodeID( sceneNodeID );
}


CSceneNode::~CSceneNode(void)
{
	if (theChildren.size() != 0)
	{
		CNode* aChild = NULL;
		for (unsigned i=0; i<theChildren.size(); i++)
		{
			aChild = theChildren[i];
			delete aChild;
			aChild = NULL;
		}
		theChildren.clear();
	}

	if (theModel != NULL)
	{
		delete theModel;
		theModel = NULL;
	}

	if (theTransform != NULL)
	{
		delete theTransform;
		theTransform = NULL;
	}
}

/*****************************************************************
check where a line segment between 2 positions intersect a plane
*****************************************************************/
int CSceneNode::GetIntersection(float fDist1, float fDist2, Vector3 p1, Vector3 p2, Vector3 &hit)
{
	if ((fDist1 * fDist2) >= 0.f)
		return 0;
	if (fDist1 == fDist2)
		return 0;
	hit = p1 + (p2 - p1) * (-fDist1 / (fDist2 - fDist1));
	return 1;
}

/*****************************************************************
check 2 positions are within a box region
*****************************************************************/
int CSceneNode::InBox(Vector3 hit, Vector3 b1, Vector3 b2, const int axis)
{
	if (axis == 1 && hit.z > b1.z && hit.z < b2.z
					&& hit.y > b1.y && hit.y < b2.y)
		return 1;

	if (axis == 2 && hit.z > b1.z && hit.z < b2.z
					&& hit.x > b1.x && hit.x < b2.x)
		return 1;

	if (axis == 3 && hit.x > b1.x && hit.x < b2.x
					&& hit.y > b1.y && hit.y < b2.y)
		return 1;
	
		return 0;
}

void CSceneNode::Draw(void)
{
	if (theTransform)
	{
		theTransform->PreRendering();
		if (theModel)
			theModel->Draw();

		if (theChildren.size() != 0)
		{
			CNode* aChild = NULL;
			for (unsigned i = 0; i<theChildren.size(); i++)
			{
				aChild = theChildren[i];
				aChild->Draw();
			}
		}

		theTransform->PostRendering();
	}
}

void CSceneNode::Draw(CSceneManager* theSceneManager)
{
	if (theTransform)
	{
		//theTransform->PreRendering();
		float x, y, z;
		theTransform->GetOffset(x, y, z);
		theSceneManager->PreRendering(Vector3(x, y, z), false);
		if (theModel)
			theModel->Draw();

		if (theChildren.size() != 0)
		{
			CNode* aChild = NULL;
			for (unsigned i = 0; i<theChildren.size(); i++)
			{
				aChild = theChildren[i];
				((CSceneNode*)aChild)->Draw(theSceneManager);
			}
		}

		theSceneManager->PostRendering();
	}
}

void CSceneNode::SetModel(CModel* theModel)
{
	this->theModel = theModel;
}

int CSceneNode::SetNode(CTransform* aNewTransform, CModel* aNewModel)
{
	this->theModel = aNewModel;
	this->theTransform = aNewTransform;

	// Set sceneNodeID to 1 as this is the root node
	sceneNodeID = 1;
	return sceneNodeID;
}

void CSceneNode::SetSceneNodeID(const int sceneNodeID)
{
	this->sceneNodeID = sceneNodeID;
}

int CSceneNode::AddChild(CTransform* aNewTransform, CModel* aNewModel)
{
	CSceneNode* aNewNode = new CSceneNode();
	aNewNode->SetNode( aNewTransform, aNewModel );
	aNewNode->SetSceneNodeID( sceneNodeID*10 + (theChildren.size()+1) );
	aNewNode->SetGridIndex(gridIndex.x, gridIndex.y);
	theChildren.push_back( aNewNode );

	return aNewNode->GetSceneNodeID();
}

int CSceneNode::AddChild(const int sceneNodeID, CTransform* aNewTransform, CModel* aNewModel)
{
	CSceneNode* aNewNode = new CSceneNode( sceneNodeID );
	aNewNode->SetNode( aNewTransform, aNewModel );
	theChildren.push_back( aNewNode );
	aNewNode->SetGridIndex(gridIndex.x, gridIndex.y);
	return aNewNode->GetSceneNodeID();
}

void CSceneNode::SetChildGridIndex(void)
{
	for (int i = 0; i < theChildren.size(); i++)
	{
		//set current child grid index
		static_cast<CSceneNode*>(theChildren[i])->SetGridIndex(this->gridIndex.x, this->gridIndex.y);
		//set the child's child grid index
		static_cast<CSceneNode*>(theChildren[i])->SetChildGridIndex();
	}
}

int CSceneNode::GetSceneNodeID(void)
{
	return sceneNodeID;
}

CSceneNode* CSceneNode::GetNode(const int sceneNodeID)
{
	if (this->sceneNodeID == sceneNodeID)
		return this;

	CSceneNode* theTarget = NULL;
	if (theChildren.size() != 0)
	{
		CSceneNode* aChild = NULL;
		for (unsigned i=0; i<theChildren.size(); i++)
		{
			aChild = (CSceneNode*)theChildren[i];

			theTarget = ((CSceneNode*)aChild)->GetNode( sceneNodeID );
			if (theTarget != NULL)
				break;
		}
	}

	return theTarget;
}

void CSceneNode::SetTranslate(const float dx, const float dy, const float dz)
{
	theTransform->SetTranslate(dx, dy, dz);
}
void CSceneNode::SetScale(const float dx, const float dy, const float dz)
{
	theTransform->SetScale(dx, dy, dz);
}
void CSceneNode::ApplyTranslate( const float dx, const float dy, const float dz )
{
	if (theTransform)
		theTransform->SetTranslate( dx, dy, dz );

	if (theChildren.size() != 0)
	{
		CSceneNode* child = NULL;
		for (unsigned i = 0; i < theChildren.size(); i++)
		{
			child = (CSceneNode*)theChildren[i];
			//child->ApplyTranslate(dx, dy, dz);
		}
	}
}
//set the gridIndex for current Node;
void CSceneNode::SetGridIndex(const int x, const int y)
{
	this->gridIndex.x = x;
	this->gridIndex.y = y;
}

//get current node grid index
Vector2 CSceneNode::GetGridIndex(void)
{
	return gridIndex;
}

//get noode current position
Vector3 CSceneNode::GetTranslate(void)
{
	float x, y, z;
	theTransform->GetOffset(x, y, z);
	
	return Vector3(x, y, z);
}

void CSceneNode::ApplyRotate( const float angle, const float rx, const float ry, const float rz )
{
	if (theTransform)
	{
		// Then rotate the children
		theTransform->SetRotate( angle, rx, ry, rz );
	}
}
void CSceneNode::ApplyScale(const float dx, const float dy, const float dz)
{
	if (theTransform)
		theTransform->SetScale(dx, dy, dz);

	if (theChildren.size() != 0)
	{
		CSceneNode* child = NULL;
		for (unsigned i = 0; i < theChildren.size(); i++)
		{
			child = (CSceneNode*)theChildren[i];
			child->ApplyScale(dx, dy, dz);
		}
	}
}

// Get top left corner of the group
Vector3 CSceneNode::GetTopLeft(void)
{
	if (theTransform == NULL)
		return Vector3( theModel->GetTopLeft().x, 
						 theModel->GetTopLeft().y, 
						 theModel->GetTopLeft().z);
	else
	{
		return (theTransform->GetTransform() * theModel->GetTopLeft());
	}
}

// Get bottom right corner of the group
Vector3 CSceneNode::GetBottomRight(void)
{
	if (theTransform == NULL)
		return Vector3( theModel->GetBottomRight().x, 
						 theModel->GetBottomRight().y, 
						 theModel->GetBottomRight().z);
	else
		return (theTransform->GetTransform() * theModel->GetBottomRight());
}

void CSceneNode::SetColor(const float red, const float green, const float blue)
{
	theModel->SetColor(red, green, blue);
}

// Return the number of children in this group
int CSceneNode::GetNumOfChild(void)
{
	return (int)theChildren.size();
}

// Get top left corner of a child
bool CSceneNode::GetTopLeft(const int m_iChildIndex, Vector3& Vector3_TopLeft)
{
		Vector3_TopLeft = Vector3( -999, -999, -999 );
	CSceneNode* aChild = NULL;
	for (unsigned i=0; i<theChildren.size(); i++)
	{
		aChild = (CSceneNode*)theChildren[i];

		if (aChild->GetSceneNodeID() == m_iChildIndex)
		{
			Vector3_TopLeft = theTransform->GetTransform() * aChild->GetTopLeft();
			return true;
		}
	}
	return false;
}

// Get bottom right corner of a child
bool CSceneNode::GetBottomRight(const int m_iChildIndex, Vector3& Vector3_BottomRight)
{
	Vector3_BottomRight = Vector3( -999, -999, -999 );
	CSceneNode* aChild = NULL;
	for (unsigned i=0; i<theChildren.size(); i++)
	{
		aChild = (CSceneNode*)theChildren[i];

		if (aChild->GetSceneNodeID() == m_iChildIndex)
		{
			Vector3_BottomRight = theTransform->GetTransform() * aChild->GetBottomRight();
			return true;
		}
	}
	return false;
}

void CSceneNode::SetColorForChild(const int m_iChildIndex, const float red, const float green, const float blue)
{
	CSceneNode* aChild = NULL;
	for (unsigned i=0; i<theChildren.size(); i++)
	{
		aChild = (CSceneNode*)theChildren[i];

		if (aChild->GetSceneNodeID() == m_iChildIndex)
		{
			aChild->SetColor(red, green, blue);
		}
	}
}

/************************************** *************************************************
Check a position for collision with objects in any of the grids
****************************************************************************************/
bool CSceneNode::CheckForCollision(Vector3 position)
{
	Vector3 ObjectTopLeft		= GetTopLeft();
	Vector3 ObjectBottomRight	= GetBottomRight();

	if (position.x < ObjectTopLeft.x && position.x > ObjectBottomRight.x)
	{
		if (position.y < ObjectTopLeft.y && position.y > ObjectBottomRight.y)
		{
			if (position.z < ObjectTopLeft.z && position.z > ObjectBottomRight.z)
			{
				return true;
			}
		}
	}
	return false;
}
/********************************************************************************
Check two positions for collision with objects in any of the grids
********************************************************************************/ 
int CSceneNode::CheckForCollision(Vector3 position_start, Vector3 position_end, Vector3 &Hit) 
{
	Vector3 ObjectTopLeft = GetTopLeft();  
	Vector3 ObjectBottomRight = GetBottomRight();   
	if (position_end.x < ObjectBottomRight.x && position_start.x < ObjectBottomRight.x)    
		return false;  
	if (position_end.x > ObjectTopLeft.x && position_start.x > ObjectTopLeft.x)    
		return false;  
	if (position_end.y < ObjectBottomRight.y && position_start.y < ObjectBottomRight.y)    
		return false;  
	if (position_end.y > ObjectTopLeft.y && position_start.y > ObjectTopLeft.y)    
		return false;  
	if (position_end.z < ObjectBottomRight.z && position_start.z < ObjectBottomRight.z)    
		return false;  
	if (position_end.z > ObjectTopLeft.z && position_start.z > ObjectTopLeft.z)    
		return false;  
	if (position_start.x > ObjectBottomRight.x && position_start.x < ObjectTopLeft.x &&   
		position_start.y > ObjectBottomRight.y && position_start.y < ObjectTopLeft.y &&   
		position_start.z > ObjectBottomRight.z && position_start.z < ObjectTopLeft.z	)   
	{ 
		Hit = position_start;    
		return true; 
	}  
	if ((GetIntersection(position_start.x - ObjectBottomRight.x, position_end.x - ObjectBottomRight.x, position_start, position_end, Hit) 
		&& InBox(Hit, ObjectBottomRight, ObjectTopLeft, 1)) 
		|| (GetIntersection(position_start.y - ObjectBottomRight.y, position_end.y - ObjectBottomRight.y, position_start, position_end, Hit) 
		&& InBox(Hit, ObjectBottomRight, ObjectTopLeft, 2)) 
		|| (GetIntersection(position_start.z - ObjectBottomRight.z, position_end.z - ObjectBottomRight.z, position_start, position_end, Hit)
		&& InBox(Hit, ObjectBottomRight, ObjectTopLeft, 3)) 
		|| (GetIntersection(position_start.x - ObjectTopLeft.x, position_end.x - ObjectTopLeft.x, position_start, position_end, Hit) 
		&& InBox(Hit, ObjectBottomRight, ObjectTopLeft, 1)) 
		|| (GetIntersection(position_start.y - ObjectTopLeft.y, position_end.y - ObjectTopLeft.y, position_start, position_end, Hit) 
		&& InBox(Hit, ObjectBottomRight, ObjectTopLeft, 2)) 
		|| (GetIntersection(position_start.z - ObjectTopLeft.z, position_end.z - ObjectTopLeft.z, position_start, position_end, Hit) 
		&& InBox(Hit, ObjectBottomRight, ObjectTopLeft, 3)))   
		return true;   
	return false; 
}

// PrintSelf
void CSceneNode::PrintSelf(void)
{
	cout << "CSceneNode::PrintSelf(void)" << endl;
	cout << "===========================" << endl;
	//theTransform->PrintSelf();

	if (theChildren.size() != 0)
	{
		CNode* aChild = NULL;
		for (unsigned i=0; i<theChildren.size(); i++)
		{
			cout << "Child #" << i << endl;
			aChild = theChildren[i];
			((CSceneNode*)aChild)->PrintSelf();
		}
	}
}

/***************************************************************
Update the spatial partition
***************************************************************/
void CSceneNode::Update(const int resolutionType)
{
	if (theModel)
	{
		theModel->SetResolution(resolutionType);
	}
	if (theChildren.size() != 0)
	{
		CNode* theChild = NULL;
		for (unsigned int i = 0; i < theChildren.size(); i++)
		{
			theChild = theChildren[i];
			((CSceneNode*)theChild)->Update(resolutionType);
		}
	}
}