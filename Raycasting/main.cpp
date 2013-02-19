#include "main.h"

void setPixel(int x, int y, Uint32 pixel) {
	Uint32 *pixels = (Uint32 *)screen->pixels;
	pixels[(y * screen->w) + x] = pixel;
}

Uint32 getWallColor(int x, int y) {
	int num = worldMap[x][y];
	if (num == 1) {
		return SDL_MapRGB(screen->format, 0xFF, 0, 0);
	}
	else if (num == 2) {
		return SDL_MapRGB(screen->format, 0, 0xFF, 0);
	}
	else {
		return SDL_MapRGB(screen->format, 0, 0, 0xFF);
	}
}

int getHeightForWallDist(double dist) {
    int height = fabs(projPlaneHeight/dist);
    if (height > projPlaneHeight) {
        height = projPlaneHeight;
    }
    return height;
}

int playerInWall() {
    int x = (int)playerPos.x;
    int y = (int)playerPos.y;
    
    if (worldMap[x][y]) {
        return 1;
    }
    else {
        return 0;
    }
}

Wall getWallForRay(Point rayPos, Point rayDir) {
    Wall w;
    int mapX = (int)rayPos.x;
    int mapY = (int)rayPos.y;
    
    double sideDistX, sideDistY;
    
    double deltaDistX = sqrt(1 + (rayDir.y * rayDir.y)/(rayDir.x * rayDir.x));
    double deltaDistY = sqrt(1 + (rayDir.x * rayDir.x)/(rayDir.y * rayDir.y));
    
    int stepX, stepY;
    int hit = 0;
    int side;
    
    if (rayDir.x < 0) {
        stepX = -1;
        sideDistX = (rayPos.x - mapX) * deltaDistX;
    }
    else {
        stepX = 1;
        sideDistX = (mapX + 1.0 - rayPos.x) * deltaDistX;
    }
    if (rayDir.y < 0) {
        stepY = -1;
        sideDistY = (rayPos.y - mapY) * deltaDistY;
    }
    else {
        stepY = 1;
        sideDistY = (mapY + 1.0 - rayPos.y) * deltaDistY;
    }
    
    while (!hit) {
        if (sideDistX < sideDistY) {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
        }
        else {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
        }
        
        if (worldMap[mapX][mapY] > 0) {
            hit = 1;
        }
    }
    
    double wallDist;
    if (side == 0) {
        wallDist = fabs((mapX - rayPos.x + (1 - stepX) / 2) / rayDir.x);
	}
    else {
        wallDist = fabs((mapY - rayPos.y + (1 - stepY) / 2) / rayDir.y);
	}
    
    w.height = getHeightForWallDist(wallDist);
    w.color = getWallColor(mapX, mapY);
    
    //shade side walls
    if (side) {
        w.color *= .75;
    }
    
    return w;
}

int main(int argc, char * argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(projPlaneWidth, projPlaneHeight, 32, SDL_SWSURFACE);
	SDL_WM_SetCaption("Raycaster", NULL);
	SDL_EnableKeyRepeat(100, SDL_DEFAULT_REPEAT_INTERVAL);
	
	int quit = 0;
	Uint32 blackColor = SDL_MapRGB(screen->format, 0, 0, 0);
    
    Point rayPos;
    Point rayDir;
	
	while(!quit) {
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						playerPos.x += playerDir.x * .15;
                        playerPos.y += playerDir.y * .15;
                        if (playerInWall()) {
                            playerPos.x -= playerDir.x * .15;
                            playerPos.y -= playerDir.y * .15;
                        }
						break;
					case SDLK_DOWN:
						playerPos.x -= playerDir.x * .15;
                        playerPos.y -= playerDir.y * .15;
                        if (playerInWall()) {
                            playerPos.x += playerDir.x * .15;
                            playerPos.y += playerDir.y * .15;
                        }
						break;
					//rotate by multiplying by rotation matrix
					case SDLK_LEFT:
                    {
                        Point oldPlayerDir, oldCameraPlane;
                        oldPlayerDir.x = playerDir.x;
                        playerDir.x = playerDir.x * cos(.03) - playerDir.y * sin(.03);
                        playerDir.y = oldPlayerDir.x * sin(.03) + playerDir.y * cos(.03);
                        oldCameraPlane.x = cameraPlane.x;
                        cameraPlane.x = cameraPlane.x * cos(.03) - cameraPlane.y * sin(.03);
                        cameraPlane.y = oldCameraPlane.x * sin(.03) + cameraPlane.y * cos(.03);
						break;
                    }
					case SDLK_RIGHT:
					{
                        Point oldPlayerDir, oldCameraPlane;
                        oldPlayerDir.x = playerDir.x;
                        playerDir.x = playerDir.x * cos(-.03) - playerDir.y * sin(-.03);
                        playerDir.y = oldPlayerDir.x * sin(-.03) + playerDir.y * cos(-.03);
                        oldCameraPlane.x = cameraPlane.x;
                        cameraPlane.x = cameraPlane.x * cos(-.03) - cameraPlane.y * sin(-.03);
                        cameraPlane.y = oldCameraPlane.x * sin(-.03) + cameraPlane.y * cos(-.03);
						break;
                    }
					case SDLK_ESCAPE:
						quit = 1;
						break;
                    default:
                        break;
				}
			}
			else if (event.type == SDL_QUIT) {
				quit = 1;
			}
		}
		
		SDL_FillRect(screen, NULL, blackColor);
		
		if (SDL_MUSTLOCK(screen)) {
			SDL_LockSurface(screen);
		}
		
        for (int i = 0; i < projPlaneWidth; i++) {
            double cameraXOffset = 2 * i / double(projPlaneWidth) - 1; //maps camera plane from -1 to 1
            rayPos.x = playerPos.x;
            rayPos.y = playerPos.y;
            rayDir.x = playerDir.x + cameraPlane.x * cameraXOffset;
            rayDir.y = playerDir.y + cameraPlane.y * cameraXOffset;
            
            Wall curWall = getWallForRay(rayPos, rayDir);
            int wallHeight = curWall.height;
            
            int offset = (projPlaneHeight - wallHeight)/2;
            
            for (int j = offset; j < offset + wallHeight; j++) {
                setPixel(i, j, curWall.color);
            }
        }
		
		if (SDL_MUSTLOCK(screen)) {
			SDL_UnlockSurface(screen);
		}
		
		SDL_Flip(screen);
	}
	
	SDL_Quit();
    
	return 0;
}
