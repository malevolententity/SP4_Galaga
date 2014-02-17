#include "ippShip.h"


ippShip::ippShip(float x, float y, float z)
	:position(0, 0, 0)
{
	position.Set ( x, y, z );
	ship = new ippQuadClass ();
}


ippShip::~ippShip(void)
{
	int size = weaponList.size ();
	for ( int i = 0; i < size; i++)
	{
		delete weaponList.at ( i );
	}

	weaponList.clear ();

	delete ship;
}

void ippShip::Init()
{
	//Sets direction to face upwards
	direction.Set(0, 1, 0); 
	moveSpeed = 1.0f;

	// Ship rendering settings
	ship->SetLW ( 20.0f, 20.0f );
}
void ippShip::Update()
{
	int size = weaponList.size ();

	// Updating all the weapons in the weaponList
	for ( int i = 0; i < size; i++ )
	{
		weaponList.at ( i )->Update ();

		if ( weaponList.at ( i )->GetPos ().x < 0 + 100 ||
			weaponList.at ( i )->GetPos ().x > Global::WIDTH_RESOLUTION - 100 ||
			weaponList.at ( i )->GetPos ().y < 0 + 100 ||
			weaponList.at ( i )->GetPos ().y < Global::HEIGHT_RESOLUTION - 100 )
		{
			delete weaponList.at ( i );
			weaponList.erase ( weaponList.begin () + i );
			size = weaponList.size ();
		}

	}

	// Update ship position
	ship->SetXY ( position.x, position.y );
}

void ippShip::Render ()
{
	/*Vec3D dimensions(20.0f, 20.0f, 0);
		
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
		glBegin(GL_QUADS);
			glTexCoord2f( 0, 1 ); glVertex2f( 0, 0);
			glTexCoord2f( 0, 0 ); glVertex2f( 0, dimensions.y);
			glTexCoord2f( 1, 0 ); glVertex2f( dimensions.x, dimensions.y );
			glTexCoord2f( 1, 1 ); glVertex2f( dimensions.x, 0);
		glEnd();
	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);*/

	// Rendering the ship
	ship->DrawQuads ();

	// Rendering the bullets
	for ( int i = 0; i < weaponList.size (); i++ )
	{
		weaponList.at ( i )->Render ();
	}
}

void ippShip::Movement ( int moveCode )
{
	switch ( moveCode )
	{
	case LEFT:
		position.x -= moveSpeed * Global::timedelta;
		break;
	case RIGHT:
		position.x += moveSpeed * Global::timedelta;
		break;

	}
}

void ippShip::FireBullet ( void )
{
	Vec3D tempPosition = position;

	tempPosition.x += ( ship->GetLength () / 2 - 5.0f );

	weaponList.push_back ( new ippBulletClass ( tempPosition, Vec3D ( 0.0f, -1.0f, 0.0f ), 1.0f, 1, 10.0f, 10.0f ) );
}