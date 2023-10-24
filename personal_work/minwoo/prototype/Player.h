#include "LTexture.h"


class Player
{
    public:
		//The dimensions of the dot
		static const int PLAYER_WIDTH = 40;
		static const int PLAYER_HEIGHT = 40;

		//Maximum axis velocity of the dot
		static const int VEL = 10;

		//Initializes the variables
		Player(LTexture texture);

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot and check collision against tiles
		void move();

		// //Centers the camera over the dot
		// void setCamera( SDL_Rect& camera );

		//Shows the dot on the screen
		void render( );

    private:
		//Collision box of the dot
		SDL_Rect mBox;
        LTexture mTexture;

		//The velocity of the dot
		int mVelX, mVelY;
};