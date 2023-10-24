#include "Player.h"

Player::Player(LTexture texture) : mTexture(texture), mVelX(0), mVelY(0)
{
    //Initialize the collision box
    mBox.x = 0;
    mBox.y = 0;
	mBox.w = PLAYER_WIDTH;
	mBox.h = PLAYER_HEIGHT;
}

void Player::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= VEL; break;
            case SDLK_DOWN: mVelY += VEL; break;
            case SDLK_LEFT: mVelX -= VEL; break;
            case SDLK_RIGHT: mVelX += VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += VEL; break;
            case SDLK_DOWN: mVelY -= VEL; break;
            case SDLK_LEFT: mVelX += VEL; break;
            case SDLK_RIGHT: mVelX -= VEL; break;
        }
    }
}

void Player::move()
{
    //Move the dot left or right
    mBox.x += mVelX;

    //If the dot went too far to the left or right or touched a wall
    if( ( mBox.x < 0 ) || ( mBox.x + PLAYER_WIDTH > SCREEN_WIDTH ) )
    {
        //move back
        mBox.x -= mVelX;
    }

    //Move the dot up or down
    mBox.y += mVelY;

    //If the dot went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + PLAYER_HEIGHT > SCREEN_HEIGHT ) )
    {
        //move back
        mBox.y -= mVelY;
    }
}

// void Dot::setCamera( SDL_Rect& camera )
// {
// 	//Center the camera over the dot
// 	camera.x = ( mBox.x + DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
// 	camera.y = ( mBox.y + DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

// 	//Keep the camera in bounds
// 	if( camera.x < 0 )
// 	{ 
// 		camera.x = 0;
// 	}
// 	if( camera.y < 0 )
// 	{
// 		camera.y = 0;
// 	}
// 	if( camera.x > LEVEL_WIDTH - camera.w )
// 	{
// 		camera.x = LEVEL_WIDTH - camera.w;
// 	}
// 	if( camera.y > LEVEL_HEIGHT - camera.h )
// 	{
// 		camera.y = LEVEL_HEIGHT - camera.h;
// 	}
// }

void Player::render()
{
    //Show the dot
	mTexture.render( mBox.x, mBox.y );
}