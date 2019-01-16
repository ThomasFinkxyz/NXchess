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


int board[boardlw][boardlw] =  {{0,0,0,0,0,0,0,0},
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
}

struct piece{
	enum {pawn,knight,bishop,rook,queen,king}type;
	bool isWhite;
	int x;
	int y;
}

struct team{
	bool isWhite;
	struct piece pieces[teamsize];
}

struct *team createTeam(bool isWhite){
	struct team* teamPointer = (struct team*)malloc(sizeof(struct team));
	int backRowY;
	int frontRowY;
	if(isWhite){
		backRowY = whiteStartBackRow;
		frontRowY = whiteStartFrontRow;
	}else{
		backRowY = blackStartBackRow;
		frontRowY = blackStartFrontRow;
	}
	for(int i =0; i<boardlw;i++){
		teamPointer->pieces[i] = piece{pawn,isWhite,i,frontRowY};
	}
	for(int i =0;i<boardlw;i++){
		switch(i){
			case 0:
			case 7:
				teamPointer->pieces[i+boardlw] = piece{rook,isWhite,i,backRowY};
				break;
			case 1:
			case 6:
				teamPointer->pieces[i+boardlw] = piece{knight,isWhite,i,backRowY};
				break;
			case 2:
			case 5:
				teamPointer->pieces[i+boardlw] = piece{bishop,isWhite,i,backRowY};
				break;
			case 4:
				teamPointer->pieces[i+boardlw] = piece{king,isWhite,i,backRowY};
				break;
			case 3:
				teamPointer->pieces[i+boardlw] = piece{queen,isWhite,i,backRowY};
				break;
		}
	}
	return teamPointer;
}

struct playerCursor* createNewCursor(bool iswhite){
	struct playerCursor* cursor = (struct playerCursor*)malloc(sizeof(struct PlayerCursor))
	cursor->x = 0;
	if(iswhite){
		cursor->y = whiteStartFrontRow;
	} else{
		cursor->y = blackStartFrontRow;
	}
	cursor->isWhite = iswhite;
	return cursor;
}


// Main program entrypoint
int main(int argc, char* argv[]){

	struct *team whiteTeam = createTeam(true);
	struct *team blackTeam = createTeam(false);
	struct *playerCursor p1cursor = createNewCursor(true);

    while (appletMainLoop()){
        // Scan all the inputs. This should be done once for each frame
        hidScanInput();

        // hidKeysDown returns information about which buttons have been
        // just pressed in this frame compared to the previous one
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS)
            break; // break in order to return to hbmenu


    }

	free(whiteTeam);
	free(blackTeam);
	free(p1cursor);
    return 0;
}
