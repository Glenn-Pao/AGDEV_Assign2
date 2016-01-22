#include "SpatialPartition.h"
#include "stdio.h"
#include <algorithm>

/********************************************************************************
 Constructor
 ********************************************************************************/
CSpatialPartition::CSpatialPartition(void)
: theGrid(NULL)
, xSize(0)
, ySize(0)
, xGridSize(0)
, yGridSize(0)
, xNumOfGrid(0)
, yNumOfGrid(0)
{
}

//For occlusion. Sorting of grids based on distance to a position
struct MyStruct
{
	float key;
	int indexValue;

	MyStruct(float k, int iv) : key(k), indexValue(iv) {}
};

struct less_than_key
{
	inline bool operator() (const MyStruct& struct1 , const MyStruct& struct2)
	{
		return (struct1.key < struct2.key);
	}
};

std::vector<MyStruct> vec;

/********************************************************************************
 Destructor
 ********************************************************************************/
CSpatialPartition::~CSpatialPartition(void)
{
	for (int i=0; i<xNumOfGrid; i++)
	{
		for (int j=0; j<yNumOfGrid; j++)
		{
			theGrid[ i*yNumOfGrid + j ].DeleteObjects();
		}
	}
	delete [] theGrid;
}

/********************************************************************************
 Initialise the spatial partition
 ********************************************************************************/
bool CSpatialPartition::Init(const int xSize, const int ySize, const int xNumOfGrid, const int yNumOfGrid)
{
	if ((xSize>0) && (ySize>0) 
		&& (xNumOfGrid>0) && (yNumOfGrid>0))
	{
		this->xNumOfGrid = xNumOfGrid;
		this->yNumOfGrid = yNumOfGrid;
		this->xGridSize = xSize;
		this->yGridSize = ySize;
		this->xSize = xGridSize * xNumOfGrid;
		this->ySize = yGridSize * yNumOfGrid;

		// Create an array of grids
		theGrid = new CGrid[ xNumOfGrid*yNumOfGrid ];

		// Initialise the array of grids
		for (int i=0; i<xNumOfGrid; i++)
		{
			for (int j=0; j<yNumOfGrid; j++)
			{
				theGrid[ i * yNumOfGrid + j ].Init(i, j, xSize, ySize);
			}
		}
		return true;
	}
	return false;
}

/********************************************************************************
 Get xNumOfGrid
 ********************************************************************************/
int CSpatialPartition::GetxNumOfGrid(void)
{
	return xNumOfGrid;
}

/********************************************************************************
 Get yNumOfGrid
 ********************************************************************************/
int CSpatialPartition::GetyNumOfGrid(void)
{
	return yNumOfGrid;
}

/********************************************************************************
 Get a particular grid
 ********************************************************************************/
CGrid CSpatialPartition::GetGrid(const int xIndex, const int yIndex)
{
	return theGrid[ xIndex*yNumOfGrid + yIndex ];
}

/********************************************************************************
 Get a particular grid's Mesh
 ********************************************************************************/
Mesh* CSpatialPartition::GetGridMesh(const int xIndex, const int yIndex)
{
	if (theGrid[xIndex * yNumOfGrid + yIndex].m_bDisplayed == true)
		return theGrid[xIndex*yNumOfGrid + yIndex].theGridMesh;
	else
		return NULL;
}

/********************************************************************************
 Set a particular grid
 ********************************************************************************/
void CSpatialPartition::SetGridMesh(const int xIndex, const int yIndex, Mesh* theGridMesh)
{
	theGrid[ xIndex*yNumOfGrid + yIndex ].SetMesh( theGridMesh );
}

/********************************************************************************
 Add a new object model
 ********************************************************************************/
void CSpatialPartition::AddObject(CSceneNode* theObject)
{
	// Get the indices of the 2 values of each position
	int index_topleft_x = ((int) theObject->GetTopLeft().x / xGridSize);
	int index_topleft_z = ((int) theObject->GetTopLeft().z / yGridSize);
	int index_bottomright_x = ((int) theObject->GetBottomRight().x / xGridSize);
	int index_bottomright_z = ((int) theObject->GetBottomRight().z / yGridSize);

	//std::cout << theObject->GetTopLeft().x << std::endl;
	// Calculate the index of each position	
	int index_topleft = index_topleft_x*yNumOfGrid + index_topleft_z;
	int index_bottomright = index_bottomright_x*yNumOfGrid + index_bottomright_z;

	// Add them to each grid
	if ((index_topleft>=0) && (index_topleft<xNumOfGrid*yNumOfGrid))
	{
		theGrid[ index_topleft ].AddObject( theObject );
	}

	// if part of the object is in another grid, then add it in as well.
	if ((index_bottomright>0) && (index_bottomright<xNumOfGrid*yNumOfGrid))
	{
		if (index_topleft != index_bottomright)
		{
			theGrid[index_bottomright].AddObject(theObject);

		}
	}
}

/********************************************************************************
 Calculate the squared distance from camera to a grid's centrepoint
 ********************************************************************************/
float CSpatialPartition::CalculateDistanceSquare(Vector3* theCameraPosition, const int xIndex, const int yIndex)
{
	float xDistance = (xIndex * xNumOfGrid + (xSize / 2)) - theCameraPosition->x;
	float yDistance = (yIndex * yNumOfGrid + (ySize / 2)) - theCameraPosition->z;

	return (float) ( xDistance*xDistance + yDistance*yDistance );
}

/********************************************************************************
Calculate the squared distance from camera to a grid's centrepoint
********************************************************************************/
float  CSpatialPartition::CalculateDistanceSquare(Vector3 thePosition, Vector3 theDirection, const int xIndex, const int yIndex)
{
	float xDistance = (xIndex * xGridSize + (xGridSize / 2)) - thePosition.x;
	float yDistance = (yIndex * yGridSize + (yGridSize / 2)) - thePosition.z;

	return (float)(xDistance*xDistance + yDistance*yDistance);
}

/********************************************************************************
 Render the spatial partition
 ********************************************************************************/
void CSpatialPartition::Render(Vector3* theCameraPosition)
{
	for (int i=0; i<xNumOfGrid; i++)
	{
		for (int j=0; j<yNumOfGrid; j++)
		{
			// Render the Grid and its objects
			//glPushMatrix();
				// Check the distance for this grid then decide the LOD to display
				if (theCameraPosition)
				{
					float distanceGrid2Camera = CalculateDistanceSquare( theCameraPosition, i, j );
					if (distanceGrid2Camera < 900)
					{
						theGrid[ i*yNumOfGrid + j ].RenderObjects(0);
					}
					else if (distanceGrid2Camera < 3600)
					{
						theGrid[ i*yNumOfGrid + j ].RenderObjects(1);
					}
					else
					{
						theGrid[ i*yNumOfGrid + j ].RenderObjects(2);
					}
				}
			//glPopMatrix();
		}
	}

	for (int i=0; i<xNumOfGrid; i++)
	{
		for (int j=0; j<yNumOfGrid; j++)
		{
			// Render the Grid and its objects
			//glPushMatrix();
				theGrid[ i*yNumOfGrid + j ].Render();
			//glPopMatrix();
		}
	}
}

/********************************************************************************
Clear grid
********************************************************************************/
void CSpatialPartition::ClearGrids(void)
{ 
	for (int i = 0; i < xNumOfGrid; i++)
		{
			for (int j = 0; j < yNumOfGrid; j++)
			{
				//clears grid
				theGrid[i*yNumOfGrid + j].ClearGrid();
			}
		}
}

/********************************************************************************
 Update the spatial partition
 ********************************************************************************/

void CSpatialPartition::Update(Vector3 theCameraPosition, Vector3 theCameraNormal)
{
	for (int i=0; i<xNumOfGrid; i++)
	{
		for (int j=0; j<yNumOfGrid; j++)
		{
			
			// Update the Grids
			theGrid[ i*yNumOfGrid + j ].Update();

			//check for collision for all objects in all grids
			vector<CSceneNode*> list = theGrid[i * yNumOfGrid + j].GetListOfObject();
			for (int k = 0; k < list.size(); k++)
				CheckForCollision(list[k]);
		}
	}
//
//	//Check for occlusion
//	FindNearestGrid(theCameraPosition, theCameraNormal); // find the grid which is nearest to the camera
//
//	theOcclusionChecker.SetScreenCoordinate(theCameraPosition); // set the position of the plane where all grids will map to
//	theOcclusionChecker.SetScreenNormal(theCameraNormal);
//
//#if _DEBUG
//	cout << "theCameraPosition: " << theCameraPosition << endl;
//#endif
//
//#if _DEBUG
//#endif
//
	if ((int)vec.size() > 1)
	{
		//use the nearest grid as the reference grid
		Vector3 positionReference_Topleft			= theOcclusionChecker.GetProjectedCoordinate(theGrid[vec[0].indexValue].GetTopLeft());
		Vector3 positionReference_BottomRight		= theOcclusionChecker.GetProjectedCoordinate(theGrid[vec[0].indexValue].GetBottomRight());

#if _DEBUG
		cout << "0. Before projection: " << theGrid[vec[0].indexValue].GetTopLeft() << ", " << theGrid[vec[0].indexValue].GetBottomRight();
		cout << "0. After projection: " << positionReference_Topleft << ", " << positionReference_BottomRight << endl << endl;
#endif

		//Set the nearest gird's displayed boolean flag to true first
		theGrid[vec[0].indexValue].m_bDisplayed = true;

		for (int i = 1; i < (int)vec.size(); i++)
		{
			//use the nearest grid as the reference positions
			Vector3 positionCheck_TopLeft		= theOcclusionChecker.GetProjectedCoordinate(theGrid[vec[i].indexValue].GetTopLeft());
			Vector3 positionCheck_BottomRight	= theOcclusionChecker.GetProjectedCoordinate(theGrid[vec[i].indexValue].GetBottomRight());

#if _DEBUG
			cout << i << ". Before projection: " << theGrid[vec[i].indexValue].GetTopLeft() << ", " << theGrid[vec[i].indexValue].GetBottomRight() << endl;
			cout << i << ". After projection: " << positionCheck_TopLeft << ", " << positionReference_BottomRight << endl;
			cout << "-----Start of Occlusion------" << endl;
#endif

			//Set the nearest gird's displayed boolean flag to true first
			theGrid[vec[0].indexValue].m_bDisplayed = false;

			//check the top left corner of a grid against the reference positions
			if (positionReference_Topleft.x > positionCheck_TopLeft.x)
			{
#if _DEBUG
				cout << "Positionreference_topLeft.x set from " << positionCheck_TopLeft.x << "to" << positionCheck_TopLeft.x << endl;
#endif
				positionReference_Topleft.x = positionCheck_TopLeft.x;
				theGrid[vec[i].indexValue].m_bDisplayed = true;
			}
			if (positionReference_Topleft.z > positionCheck_TopLeft.z)
			{
#if _DEBUG
				cout << "Positionreference_topLeft.x set from " << positionCheck_TopLeft.z << "to" << positionCheck_TopLeft.z << endl;
#endif
				positionReference_Topleft.z = positionCheck_TopLeft.z;
				theGrid[vec[i].indexValue].m_bDisplayed = true;
			}

			//check the bottom right corner of a grid against the reference positions
			if (positionReference_BottomRight.x > positionCheck_BottomRight.x)
			{
#if _DEBUG
				cout << "Positionreference_topLeft.x set from " << positionCheck_BottomRight.x << "to" << positionCheck_BottomRight.x << endl;
#endif
				positionReference_BottomRight.x = positionCheck_BottomRight.x;
				theGrid[vec[i].indexValue].m_bDisplayed = true;
			}
			if (positionReference_BottomRight.z > positionCheck_BottomRight.z)
			{
#if _DEBUG
				cout << "Positionreference_topLeft.x set from " << positionCheck_BottomRight.z << "to" << positionCheck_BottomRight.z << endl;
#endif
				positionReference_BottomRight.z = positionCheck_BottomRight.z;
				theGrid[vec[i].indexValue].m_bDisplayed = true;
			}
#if _DEBUG
			cout << "====== End of Occlusion =====" << endl;
			cout << "Current Reference: " << positionReference_Topleft << ", " << positionReference_BottomRight << endl << endl;
#endif
		}
#if _DEBUG
		cout << endl << "Printout of vec " << endl;
		for (int i = 0; i < (int)vec.size(); i++)
		{
			cout << i << " : " << theGrid[vec[i].indexValue].GetTopLeft() << ", "
				<< theGrid[vec[i].indexValue].GetBottomRight()
				<< theGrid[vec[i].indexValue].m_bDisplayed << endl;
		}
#endif
		
		//clear the list of grids in the vec vector
		vec.clear();
	}
	for (int i = 0; i < xNumOfGrid; i++)
	{
		for (int j = 0; j < yNumOfGrid; j++)
		{
			float distanceGrid2Camera = CalculateDistanceSquare(theCameraPosition, theCameraNormal, i, j);
			if (distanceGrid2Camera < 10000)
			{
				//update the grids 
				theGrid[i * yNumOfGrid + j].Update(CModel::RESOLUTION_HIGH);
			}
			else if (distanceGrid2Camera < 20000)
			{
				//update the grids 
				theGrid[i * yNumOfGrid + j].Update(CModel::RESOLUTION_MEDIUM);
			}
			else
			{
				//update the grids 
				theGrid[i * yNumOfGrid + j].Update(CModel::RESOLUTION_LOW);
			}
		}
	}
}

bool CSpatialPartition::CheckForCollision(Vector3 position)
{
	int GridIndex_x = ((int) position.x / (xSize * xNumOfGrid));
	int GridIndex_Z = ((int) position.z / (ySize * yNumOfGrid));

	//calculate the index of each position
	int GridIndex = GridIndex_x * yNumOfGrid + GridIndex_Z;

	//Add them to each grid
	if ((GridIndex >= 0) && (GridIndex < xNumOfGrid * yNumOfGrid))
	{
		vector<CSceneNode* >theListOfObjects = theGrid[GridIndex].GetListOfObject();

		Vector3 ObjectTopLeft, ObjectBottomRight;
		for (int i = 1; i < (int)theListOfObjects.size(); i++)
		{
			return theListOfObjects[i]->CheckForCollision(position);
		}
	}
	else
		return false;
}

bool CSpatialPartition::CheckForCollision(Vector3 position_start, Vector3 position_end)
{
	int GridIndex_x = ((int)position_start.x / (xSize * xNumOfGrid));
	int GridIndex_Z = ((int)position_start.z / (ySize * yNumOfGrid));

	//calculate the index of each position
	int GridIndex = GridIndex_x * yNumOfGrid + GridIndex_Z;
	position_start.y = 0.f;

	//Add them to each grid
	if ((GridIndex >= 0) && (GridIndex < xNumOfGrid * yNumOfGrid))
	{
		vector<CSceneNode* >theListOfObjects = theGrid[GridIndex].GetListOfObject();

		Vector3 ObjectTopLeft, ObjectBottomRight;
		for (int i = 1; i < (int)theListOfObjects.size(); i++)
		{
			Vector3 hits = Vector3(0, 0, 0);
			return theListOfObjects[i]->CheckForCollision(position_start, position_end, hits);
		}
	}
	return false;
}

bool CSpatialPartition::CheckForCollision(CSceneNode* n)
{
	int GridIndex_x = ((int)n->GetTopLeft().x / (xSize * xNumOfGrid));
	int GridIndex_Z = ((int)n->GetTopLeft().z / (ySize * yNumOfGrid));

	//calculate the index of each position
	int GridIndex = GridIndex_x * yNumOfGrid + GridIndex_Z;

	//Add them to each grid
	if ((GridIndex >= 0) && (GridIndex < xNumOfGrid * yNumOfGrid))
	{
		vector<CSceneNode* >theListOfObjects = theGrid[GridIndex].GetListOfObject();

		Vector3 ObjectTopLeft, ObjectBottomRight;
		for (int i = 0; i < (int)theListOfObjects.size(); i++)
		{
			Vector3 hits = Vector3(0, 0, 0);
			return theListOfObjects[i]->CheckForCollision(n->GetTopLeft(), n->GetBottomRight(), hits);
		}
	}
	return false;
}

/*******************************************************************************
Find the index of a grid which is nearest to a position
*********************************************************************************/
void CSpatialPartition::FindNearestGrid(Vector3 position, Vector3 direction)
{
	float m_fDistance = -1.0f;
	for (int i = 0; i < xNumOfGrid; i++)
	{
		for (int j = 0; j < yNumOfGrid; j++)
		{
			m_fDistance = CalculateDistanceSquare(position, direction, i, j);
			vec.push_back(MyStruct(m_fDistance, i * yNumOfGrid + j));
		}
	}

#if _DEBUG
	//cout display the list of grids before sorting
	cout << "BEFORE SORT" << endl;
	for (int i = 0; i < (int)vec.size(); i++)
	{
		cout << i << ": index =" << vec[i].indexValue << ", key = " << vec[i].key << " => " << theGrid[vec[i].indexValue].m_bDisplayed << endl;
	}
#endif

	//sort the vector
	std::sort(vec.begin(), vec.end(), less_than_key());

#if _DEBUG
	//cout display the list of grids before sorting
	cout << "AFTER SORT" << endl;
	for (int i = 0; i < (int)vec.size(); i++)
	{
		cout << i << ": index =" << vec[i].indexValue << ", key = " << vec[i].key << " => " << theGrid[vec[i].indexValue].m_bDisplayed << endl;
	}
#endif
}


/********************************************************************************
 PrintSelf
 ********************************************************************************/
void CSpatialPartition::PrintSelf()
{
	cout << "* CSpatialPartition::PrintSelf() ***********************************************" << endl;
	cout << "xSize        : " << xSize << endl;
	cout << "ySize        : " << ySize << endl;
	cout << "xNumOfGrid   : " << xNumOfGrid << endl;
	cout << "xNumOfGrid   : " << xNumOfGrid << endl;
	if (theGrid)
	{
		cout << "theGrid : OK" << endl;
		cout << "Printing out theGrid below: " << endl;
		for (int i=0; i<xNumOfGrid; i++)
		{
			for (int j=0; j<yNumOfGrid; j++)
			{
				theGrid[ i*yNumOfGrid + j ].PrintSelf();
			}
		}
	}
	else
		cout << "theGrid : NULL" << endl;
	cout << "********************************************************************************" << endl;
}
