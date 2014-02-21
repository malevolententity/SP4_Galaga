#include "EnemyManagerClass.h"


ippEnemyManagerClass::ippEnemyManagerClass(void)
:	numberOfPoints(999)
,	lastPoint(1)
,	waypointTracker(0)
,	curTime(0.0f)
,	prevTime(0.0f)
,	diffTime(0.0f)
,	wave_num(0)
,	current_state(0)
,	pie(3.141592654)
,	tenDegrees(0.17453292522222222222222222222222)
,	activateSpawner(true)
,	resetAt(false)
{
	srand(time(NULL));
}

ippEnemyManagerClass::~ippEnemyManagerClass(void)
{
	if( list_of_positions != NULL )
	{
		delete [] list_of_positions;
	}
	if( theGrid != NULL )
	{
		delete theGrid;
	}
	enemyWave_.clear();
}

void ippEnemyManagerClass::Init()
{
	int value = 2 * pie * 10;
	list_of_positions = new Vec3D[ numberOfPoints ];					//	CREATING ARRAY BASED ON SIZE OF TURNING POINT
	int num = 0;

	ResetWaypointsAt( 400.0f, 400.0f );
	AddLoop( 200.0f, 300.0f, false, 0, 62, 5, 100.0f );
	AddWaypoint( 500.0f, 500.0f, 62 );
	//ShowWaypoints();
	//	=	=	=	=	=	THIS CODE IS FOR DEBUGGING VALUES	=	=	=	=	=
	//for( int i = 0; i < numberOfPoints; i++ )							//	THIS CODE WAS THE ORIGINAL CIRCLE CODES
	//{
	//	float angle = -i;
	//	list_of_positions[i] = Vec3D( 100 * cos(angle)+400, 100 * sin(angle)+300, 0.0f );
	//}
	//	=	=	=	=	=	THIS CODE IS FOR DEBUGGING VALUES	=	=	=	=	=
}

void ippEnemyManagerClass::InitGrid( float xSize, float ySize, int xArray, int yArray, float xShip, float yShip, float xGrid, float yGrid )
{
	theGrid = new ippEnemyGrid();
	theGrid->Init( xSize, ySize, xArray, yArray, xShip, yShip, xGrid, yGrid );
}

void ippEnemyManagerClass::CreateWaves( int number_of_ships, int enemy_type )
{
	for( int counter = 0; counter < number_of_ships; counter++ )
	{
		Vec3D tempPos = Vec3D( rand()%200+300, rand()%200+200, 0.0f );
		enemyWave_.push_back( new ippEnemy( tempPos ) );
		enemyWave_.at( counter )->MoveToWaypoint( list_of_positions[counter].x, list_of_positions[counter].y );
	}
}

void ippEnemyManagerClass::ChangeLastPoint( int number )
{
	if( resetAt )
	{
		lastPoint = number + 2;
	}
	else
	{
		lastPoint = number + 1;
	}
}

void ippEnemyManagerClass::ActivateSpawn()
{
	activateSpawner = true;
}

Vec3D ippEnemyManagerClass::AssignGridSlot( int num )
{
	int xMulti = 0, yMulti = num;
	while( yMulti >= theGrid->GetArraySizeX() )
	{
		yMulti -= theGrid->GetArraySizeX();
		xMulti++;
	}
	return Vec3D( theGrid->GetGridShipX()[yMulti], theGrid->GetGridShipY()[xMulti], 0 );
}

void ippEnemyManagerClass::Spawner( int number_of_ships, int enemy_type, Vec3D spawnpoint, Vec3D flyThere, float interval )
{
	wave_max = number_of_ships;
	spawnerPos = spawnpoint;
	firstWaypoint = flyThere;
	diffTime = interval;
	activateSpawner = true;
}

void ippEnemyManagerClass::Spawner( int number_of_ships, int enemy_type, float spawnX, float spawnY, float flyX, float flyY, float interval )
{
	wave_max = number_of_ships;
	spawnerPos = Vec3D( spawnX, spawnY, 0.0f );
	firstWaypoint = Vec3D( flyX, flyY, 0.0f );
	diffTime = interval;
	activateSpawner = true;
}

void ippEnemyManagerClass::AddWaypoint( Vec3D point, int tracker )
{
	ChangeLastPoint( tracker );
	list_of_positions[tracker] = point;
}

void ippEnemyManagerClass::AddWaypoint( float pointX, float pointY, int tracker )
{
	ChangeLastPoint( tracker );
	list_of_positions[tracker] = Vec3D( pointX, pointY, 0.0f );
}

void ippEnemyManagerClass::AddLoop( Vec3D point, bool clockwise, int tracker, int stopHere, int angleBreak, float size, float angle )
{
	//	=	=	=	=	=	EXTRA INFORMATION	=	=	=	=	=
	//	the for() loop below
	//	double angle is the angle of how far you want the ship to fly around
	//	if you want a full cycle, use angle <= 2 * pie
	//	if you want a half cycle, use angle <= pie
	//	
	//	as for the angle increment, angle += 0.1
	//	this is for how defined you want the points to be
	//	it can be as small as 0.001
	//	and biggest is recommended to be 0.1
	//	because 1.0 causes a huge jump from one point to another
	//	=	=	=	=	=	EXTRA INFORMATION	=	=	=	=	=
	int num = tracker;
	if( clockwise )
	{	//	CREATE WAYPOINTS BASED ON CIRCLE FORMULA
		for( double tempAngle = ( angleBreak * tenDegrees ); tempAngle <= angle + ( angleBreak * tenDegrees ); tempAngle += 0.1 )
		{
			list_of_positions[num] = Vec3D( size * cos(tempAngle) + point.x, size * sin(tempAngle) + point.y, 0.0f );
			num++;
			if( num >= stopHere + tracker )
			{
				ChangeLastPoint( num );
				break;
			}
		}
	}
	else
	{
		for( double tempAngle = angle + ( angleBreak * tenDegrees ); tempAngle >= ( angleBreak * tenDegrees ); tempAngle -= 0.1 )
		{
			list_of_positions[num] = Vec3D( size * cos(tempAngle) + point.x, size * sin(tempAngle) + point.y, 0.0f );
			num++;
			if( num >= stopHere + tracker )
			{
				ChangeLastPoint( num );
				break;
			}
		}
	}
}

void ippEnemyManagerClass::AddLoop( float pointX, float pointY, bool clockwise, int tracker, int stopHere, int angleBreak, float size, float angle )
{
	int num = tracker;
	if( clockwise )
	{
		for( double tempAngle = ( angleBreak * tenDegrees ); tempAngle <= angle + ( angleBreak * tenDegrees ); tempAngle += 0.1 )
		{
			list_of_positions[num] = Vec3D( size * cos(tempAngle) + pointX, size * sin(tempAngle) + pointY, 0.0f );
			num++;
			if( num >= stopHere + tracker )
			{
				ChangeLastPoint( num );
				break;
			}
		}
	}
	else
	{
		for( double tempAngle = angle + ( angleBreak * tenDegrees ); tempAngle >= ( angleBreak * tenDegrees ); tempAngle -= 0.1 )
		{
			list_of_positions[num] = Vec3D( size * cos(tempAngle) + pointX, size * sin(tempAngle) + pointY, 0.0f );
			num++;
			if( num >= stopHere + tracker )
			{
				ChangeLastPoint( num );
				break;
			}
		}
	}
}

void ippEnemyManagerClass::ClearWaypoints()
{
	resetAt = false;
	for( int counter = 0; counter < numberOfPoints; counter++ )
	{
		list_of_positions[ counter ] = Vec3D(0,0,0);
	}
}

void ippEnemyManagerClass::ResetWaypointsAt( Vec3D point )
{
	resetAt = true;
	for( int counter = 0; counter < numberOfPoints; counter++ )
	{
		list_of_positions[ counter ] = point;
	}
}

void ippEnemyManagerClass::ResetWaypointsAt( float pointX, float pointY )
{
	resetAt = true;
	for( int counter = 0; counter < numberOfPoints; counter++ )
	{
		list_of_positions[ counter ] = Vec3D( pointX, pointY, 0.0f );
	}
}

void ippEnemyManagerClass::ShowWaypoints()
{
	for( int counter = 0; counter < numberOfPoints; counter ++ )
	{
		printf( "%d -->> %3.3f, %3.3f,\n", counter, list_of_positions[ counter ].x, list_of_positions[ counter ].y );
	}
}

void ippEnemyManagerClass::Update()
{
	if( activateSpawner )
	{
		curTime += Global::timedelta;
		if( curTime > prevTime + diffTime )							//	TIMER TO DELAY SENDING OUT SHIPS	
		{
			prevTime = curTime;
			enemyWave_.push_back( new ippEnemy( spawnerPos ) );
			enemyWave_.at(wave_num)->StateMove();
			enemyWave_.at(wave_num)->MoveToWaypoint( firstWaypoint );
			enemyWave_.at(wave_num)->SetGridPos( AssignGridSlot( wave_num ) );
			wave_num ++;											//	COUNTER TO KEEP TRACK OF NUMBER OF SHIPS CREATED
		}															//	SHOULD NOT RESET UNTIL ALL SHIPS DESTROYED
		if( wave_num >= wave_max )
		{
			activateSpawner = false;
		}
	}

	for( list_of_enemy::iterator enemy = enemyWave_.begin();	//	UPDATES FOR ALL SHIPS EXISITING
		enemy != enemyWave_.end(); enemy++ )
	{
		
		if( (*enemy)->GetState() == (*enemy)->ON_THE_MOVE )
		{
			//	=	=	=	=	=	CODES FOR FLYING ROUND IN CIRCLES	=	=	=	=	=
			if( (*enemy)->waypoint.Reached( (*enemy)->GetPos() ) )	//	ONCE THIS SHIP HAS ARRIVED AT ITS WAYPOINT
			{
				(*enemy)->at_waypoint++;							//	CHANGE THE ARRAY NUMBER TO THE NEXT POINT
				if( (*enemy)->at_waypoint == lastPoint/*numberOfPoints*/ )		//	WHEN THE ARRAY HAS REACHED IT'S LAST VALUE
				{
					(*enemy)->StateFloat();
					break;
				}
				(*enemy)->MoveToWaypoint( list_of_positions[(*enemy)->at_waypoint] );
			}
			//	=	=	=	=	=	CODES FOR FLYING ROUND IN CIRCLES	=	=	=	=	=
		}

		if( (*enemy)->GetState() == (*enemy)->FLOATING )
		{
			(*enemy)->MoveToWaypoint( (*enemy)->GetGridPos() );
			(*enemy)->StateFlyToGrid();
		}

		if( (*enemy)->GetState() == (*enemy)->FLYING_TO_GRID )
		{
			if( (*enemy)->waypoint.Reached( (*enemy)->GetPos() ) )
			{
			}
		}

		(*enemy)->Update();
	}
}

void ippEnemyManagerClass::Render()
{
	theGrid->Render();

	for( list_of_enemy::iterator enemy = enemyWave_.begin();
		enemy != enemyWave_.end(); enemy++ )
	{
		(*enemy)->Render();
	}
}