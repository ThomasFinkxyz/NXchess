// Include the main libnx system header, for Switch development
#include <switch.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int boardlw = 8;
const int teamsize = 16;
const int kingStartX = 4;
const int queenStartX = 3;
const int whiteStartBackRow = 7;
const int whiteStartFrontRow = 6;
const int blackStartBackRow = 0;
const int blackStartFrontRow = 1;
const int spaceAndPieceSize = 90;
const int piecesTotal = 12;
const int blackPieceTextureOffset = 6;
//I need this to skip over the white textures in the array of piece textures.

//cant use boardlw because C is dumb
int board[8][8] =  {{0,0,0,0,0,0,0,0},
				   				{0,0,0,0,0,0,0,0},
				   				{0,0,0,0,0,0,0,0},
				   				{0,0,0,0,0,0,0,0},
				   				{0,0,0,0,0,0,0,0},
				   				{0,0,0,0,0,0,0,0},
				   				{0,0,0,0,0,0,0,0},
				   				{0,0,0,0,0,0,0,0}};

struct playerCursor{
	int x;
	int y;
	bool isWhite;
};

struct piece{
	enum {pawn,knight,bishop,rook,queen,king}type;
	bool isWhite;
	int x;
	int y;
	bool isAlive;
};

struct team{
	bool isWhite;
	struct piece pieces[16];
//16 is the teamsize
};

struct piece* createPiece(int type, bool isWhite, int x, int y, bool isAlive){
	struct piece* piecePointer = (struct piece*)malloc(sizeof(struct piece));
	piecePointer->type = type;
	piecePointer->isWhite = isWhite;
	piecePointer->x = x;
	piecePointer->y = y;
	piecePointer->isAlive = isAlive;
	return piecePointer;
}

struct team* createTeam(bool isWhite){
	struct team* teamPointer = (struct team*)malloc(sizeof(struct team));
	int backRowY;
	int frontRowY;
	int pawnNum = 0;   //this is spaghetti but it will probably work
	int knightNum = 1;
	int bishopNum = 2;
	int rookNum = 3;
	int queenNum = 4;
	int kingNum = 5;

	if(isWhite){
		backRowY = whiteStartBackRow;
		frontRowY = whiteStartFrontRow;
	}else{
		backRowY = blackStartBackRow;
		frontRowY = blackStartFrontRow;
	}
	for(int i =0; i<boardlw;i++){
		teamPointer->pieces[i] = (*createPiece(pawnNum,isWhite,i,frontRowY,true));
	}
	for(int i =0;i<boardlw;i++){
		switch(i){
			case 0:
			case 7:
				teamPointer->pieces[i+boardlw] = (*createPiece(rookNum,isWhite,i,backRowY,true));
				break;
			case 1:
			case 6:
				teamPointer->pieces[i+boardlw] = (*createPiece(knightNum,isWhite,i,backRowY,true));
				break;
			case 2:
			case 5:
				teamPointer->pieces[i+boardlw] = (*createPiece(bishopNum,isWhite,i,backRowY,true));
				break;
			case 4:
				teamPointer->pieces[i+boardlw] = (*createPiece(kingNum,isWhite,i,backRowY,true));
				break;
			case 3:
				teamPointer->pieces[i+boardlw] = (*createPiece(queenNum,isWhite,i,backRowY,true));
				break;
		}
	}
	return teamPointer;
}

struct playerCursor* createNewCursor(bool iswhite){
	struct playerCursor* cursor = (struct playerCursor*)malloc(sizeof(struct playerCursor));
	cursor->x = 0;
	if(iswhite){
		cursor->y = whiteStartFrontRow;
	} else{
		cursor->y = blackStartFrontRow;
	}
	cursor->isWhite = iswhite;
	return cursor;
}

void drawPiece(SDL_Renderer *r, SDL_Texture *t, int x, int y){
	SDL_Rect destRec;
	destRec.x = x;
	destRec.y = y;
	destRec.w = spaceAndPieceSize;
	destRec.h = spaceAndPieceSize;
	SDL_RenderCopy(r,t,NULL,&destRec);
}


// Main program entrypoint
int main(int argc, char* argv[]){

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	romfsInit();
	//TTF_Init();

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *bgsurface;
	SDL_Texture *bgtexture;
	SDL_Texture *piecetextures[piecesTotal];
	SDL_Surface *piecesurface;


	SDL_CreateWindowAndRenderer(0,0,SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
	bgsurface = IMG_Load("romfs:/image/chessboard-magick.png");
	bgtexture = SDL_CreateTextureFromSurface(renderer,bgsurface);
	SDL_FreeSurface(bgsurface);

	const char *filepaths[12] = {"romfs:/image/whitepawn.png", "romfs:/image/whiteknight.png", "romfs:/image/whitebishop.png", "romfs:/image/whiterook.png", "romfs:/image/whitequeen.png", "romfs:/image/whiteking.png", "romfs:/image/blackpawn.png", "romfs:/image/blackknight.png", "romfs:/image/blackbishop.png", "romfs:/image/blackrook.png", "romfs:/image/blackqueen.png", "romfs:/image/blackking.png"};

	//load all the block textures into an array for later use.
	for(int i = 0; i<piecesTotal;i++){
		piecesurface = IMG_Load(filepaths[i]);
		piecetextures[i] = SDL_CreateTextureFromSurface(renderer,piecesurface);
		SDL_FreeSurface(piecesurface);
	}

	struct team* whiteTeam = createTeam(true);
	struct team* blackTeam = createTeam(false);
	struct playerCursor* p1cursor = createNewCursor(true);

    while (appletMainLoop()){
        // Scan all the inputs. This should be done once for each frame
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break; // break in order to return to hbmenu

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer,bgtexture,NULL,NULL);
		for(int i = 0;i<teamsize;i++){
			if(whiteTeam->pieces[i].isAlive){
				drawPiece(renderer,piecetextures[i],whiteTeam->pieces[i].x*spaceAndPieceSize,whiteTeam->pieces[i].y*spaceAndPieceSize);
			}
			if(blackTeam->pieces[i].isAlive){
				drawPiece(renderer,piecetextures[i+blackPieceTextureOffset],blackTeam->pieces[i].x*spaceAndPieceSize,whiteTeam->pieces[i].y*spaceAndPieceSize);
			}
		}
		SDL_RenderPresent(renderer);

    }

	//TTF_Quit();
	romfsExit();
	IMG_Quit();
	SDL_DestroyTexture(bgtexture);
	for(int i =0; i<piecesTotal;i++){
		SDL_DestroyTexture(piecetextures[i]);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	free(whiteTeam);
	free(blackTeam);
	free(p1cursor);
    return 0;
}
