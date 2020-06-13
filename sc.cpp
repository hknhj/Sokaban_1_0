#include <stdio.h>
#include <bangtal.h>
#include <time.h>
#include <sys/timeb.h>


#define EMPTY	-2	// �ƹ��͵� ���� ����� (�� �ܺ�)
#define WALL	-1	// ��							can't
#define TILE	0	// ������Ʈ ���� �� Ÿ��		can
#define BOX		1	// ����				moveable	can't
#define SLAB	2	// ����				fixed		can
#define HOUSE	3	// ��				fixed		can't
#define SPEAR	4	// â				fixed		can
#define THRON	5	// ����				fixed		can
#define KEY		6	// ����				fixed		can
#define TREASURE 7	// ��������			fixed		flexible
#define ROCK	8	// ��				movable		can't
#define EXIT    9   // Ż�ⱸ           fixed       can't
#define PLAYER	10	// player				
#define FULL    12  //���ڿ� ������ ������ �ִ� ��Ȳ

#define BOARD_X_PIX	50	// ���� ��ĭ�� �ȼ��ʺ�
#define BOARD_Y_PIX 50	// ���� ��ĭ�� �ȼ�����
#define BOARD_X_NUM 10	// ���� ��ü ���� ĭ��
#define BOARD_Y_NUM 10	// ���� ��ü ���� ĭ��

#define TOTAL_STAGE_NUM		10	// �� �������� ����
#define TOTAL_OBJECT_TYPE	11	// ������Ʈ ���� ���� +1


struct ObjectStruct { char x; char y; char type; };

SceneID sceneStage[TOTAL_STAGE_NUM];
SceneID selectStage, clearStage;
ObjectID tile[TOTAL_STAGE_NUM][BOARD_Y_NUM][BOARD_X_NUM];	// ���� �ٴ�(Ÿ��) ������Ʈ.
ObjectID object[TOTAL_STAGE_NUM][TOTAL_OBJECT_TYPE][30];
ObjectStruct objectStruct[TOTAL_STAGE_NUM][TOTAL_OBJECT_TYPE][30];
ObjectID player[TOTAL_STAGE_NUM];
ObjectID stageButton[TOTAL_STAGE_NUM];
ObjectID backButton[TOTAL_STAGE_NUM];
ObjectID quitbutton;

SoundID bgm, blocked, clearsound, oneclear; //���, ������ �� �Ҹ�, Ŭ����Ҹ�


// �������� ����
char initBoard[TOTAL_STAGE_NUM][BOARD_Y_NUM][BOARD_X_NUM] = {

		// STAGE 0
	{	{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
		{-2,-2,-2,-2,-2,-1,-1,-1,-1,-2},
		{-1,-1,-1,-1,-1,-1, 0, 0,-1,-2},
		{-1, 2, 2, 0, 1, 0, 1, 0,-1,-1},
		{-1, 2, 2, 1, 0, 1, 0, 0,10,-1},
		{-1,-1, 2, 0, 1,-1,-1, 0,-1,-1},
		{-2,-1,-1,-1, 0, 0, 0, 0,-1,-2},
		{-2,-2,-2,-1,-1,-1,-1,-1,-1,-2},
		{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
		{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2} },

		// STAGE 1
	{	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-1, 0, 0, 0, 0, 0,-1, 0,10, 0},
		{-1, 0, 1, 0, 0, 1, 0, 0, 1, 0},
		{-1,-1, 0,-1, 2, 2, 2,-1, 0,-1},
		{-2,-1, 0,-1, 2, 2, 2,-1, 0,-1},
		{-2,-1, 1, 0,-1,-1,-1, 0, 1,-1},
		{-2,-1, 0, 0, 0, 1, 0, 0, 0,-1},
		{-2,-1, 0, 0,-1,-1, 0, 0, 0,-1},
		{-2,-1,-1,-1,-1,-1,-1,-1,-1,-1},
		{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2} },

		//STAGE 2 (�̼��ְ� �ø� ù��° ����)
	{	{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},//10
		{-2,-1,-1,-1,-1,-1,-2,-2,-2,-2},//9
		{-2,-1, 0, 0, 0,-1,-1,-1,-1,-2},//8
		{-2,-1, 0, 0, 0,-1, 0, 0,-1,-2},//7
		{-2,-1,-1, 0, 0, 0, 0, 2,-1,-2},//6
		{-1,-1,-1, 0,-1,-1,-1, 2,-1,-2},//5
		{-1, 0, 1, 0,-1,-2,-1, 2,-1,-2},//4
		{-1, 0, 1, 1,-1,-2,-1,-1,-1,-2},//3
		{-1,10, 0, 0,-1,-2,-2,-2,-2,-2},//2
		{-1,-1,-1,-1,-1,-2,-2,-2,-2,-2} },//1
	//STAGE 3 (�ι�° ����)
	{   {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},//10
		{-2,-1,-1,-1,-1,-1,-1,-1,-1,-2},//9
		{-2,-1, 0, 0, 0, 2, 0, 0,-1,-2},//8
		{-2,-1, 1, 0,12, 1, 1, 2,-1,-2},//7
		{-2,-1, 0,-1, 0, 2, 0,-1,-1,-2},//6
		{-2,-1, 2,-1,-1, 1, 0,-1,-2,-2},//5
		{-2,-1,-1,-1, 0, 1, 2,-1,-2,-2},//4
		{-2,-1, 2,10, 1, 0, 0,-1,-2,-2},//3
		{-2,-1,-1,-1, 0, 0, 0,-1,-2,-2},//2
		{-2,-2,-2,-1,-1,-1,-1,-1,-2,-2} },//1
		//STAGE 4(����° ����)
	{   {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},//10
		{-2,-2,-2,-2,-1,-1,-1,-2,-2,-2},//9
		{-2,-2,-2,-2,-1, 2,-1,-2,-2,-2},//8
		{-2,-1,-1,-1,-1, 1,-1,-2,-2,-2},//7
		{-2,-1, 2, 0, 1,10,-1,-1,-1,-2},//6
		{-2,-1,-1,-1, 1, 0, 1, 2,-1,-2},//5
		{-2,-2,-2,-1, 0,-1,-1,-1,-1,-2},//4
		{-2,-2,-2,-1, 2,-1,-2,-2,-2,-2},//3
		{-2,-2,-2,-1,-1,-1,-2,-2,-2,-2},//2
		{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2} },//1
		//STAGE 5(�׹�° ����)
	{   {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},//10
		{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},//9
		{-2,-1,-1,-1,-1,-1,-1,-1,-1,-2},//8
		{-1,-1, 2, 2,-1, 0, 0, 0,-1,-2},//7
		{-1, 0, 2, 2,-1, 0, 1, 0,-1,-1},//6
		{-1, 0,10, 0, 1, 0, 0, 1, 0,-1},//5
		{-1,-1, 1,-1,-1,-1, 0, 0, 0,-1},//4
		{-2,-1, 0, 0, 0, 0, 0,-1,-1,-1},//3
		{-2,-1,-1,-1,-1,-1,-1,-1,-2,-2},//2
		{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2} },//1
		//STAGE 6(�ټ���° ����)
	{   {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},//10
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},//9
		{-1, 2, 2, 2, 2, 2, 0, 0,-1,-2},//8
		{-1,-1,-1, 0, 1, 0,-1, 0,-1,-1},//7
		{-2,-2,-1, 0, 1,-1,-1, 0, 0,-1},//6
		{-2,-2,-1, 1, 0, 1, 0, 1, 0,-1},//5
		{-2,-2,-1, 0, 0, 0,-1, 0, 0,-1},//4
		{-2,-2,-1,-1, 0, 0,-1, 0,10,-1},//3
		{-2,-2,-2,-1,-1,-1,-1,-1,-1,-1},//2
		{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2} },//1
		//STAGE 7(������° ����)
	{   {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},//10
		{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},//9
		{-2,-2,-2,-2,-1,-1,-1,-1,-2,-2},//8
		{-2,-1,-1,-1,-1, 0, 0,-1,-2,-2},//7
		{-2,-1, 0, 1, 2, 2, 2,-1,-1,-2},//6
		{-2,-1,10, 0, 1, 2, 2, 0,-1,-2},//5
		{-2,-1,-1,-1, 1, 1, 1, 0,-1,-2},//4
		{-2,-2,-2,-1, 0, 0, 0, 0,-1,-2},//3
		{-2,-2,-2,-1,-1,-1,-1,-1,-1,-2},//2
		{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2} },//1
		//STAGE8 (�ϰ���° ����) �̰� ���ΰ� 10���� Ŀ�� �ϴ� �ٿ����
	{   {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},//10
		{-1, 0,-1,-1,-1,-1,-1,-1, 0,-1},//9
		{-1, 1,-1,-1,-1,-1,-1,-1, 0,-1},//8
		{-1, 0, 0, 0, 0, 0, 0, 0, 0,-1},//7
		{-1, 0,-1,-1,-1,-1,-1,-1, 0, 0},//6
		{ 0, 2,-1,-2,-2,-1, 0, 1, 0,-1},//5
		{ 0, 1,-1,-1,-1,-1, 2,-1, 0,-1},//4
		{ 2,10, 0, 0,12, 0, 0, 1, 0,-1},//3
		{ 2, 2, 1, 0, 0,-1, 0, 0, 0,-1},//2
		{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1} },//1
	
		//STAGE 9
	{	{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
		{-1,-1,-1,-1,-1,-1,-1,-1,-2,-2},
		{-1, 0, 0,-1, 2,-1, 2,-1,-1,-2},
		{-1, 0, 1, 2, 1, 2, 0, 2,-1,-2},
		{-1, 0, 0, 0, 2,-1, 1,-1,-1,-2},
		{-1,-1,-1,-1, 0, 0, 0,-1,-2,-2},
		{-2,-2,-2,-1, 1, 1, 1,-1,-2,-2},
		{-2,-2,-2,-1, 0,10, 0,-1,-2,-2},
		{-2,-2,-2,-1,-1,-1,-1,-1,-2,-2},
		{-2,-2,-2,-2,-2,-2,-2,-2,-2,-2} }
};


char curStage;
char playerX[TOTAL_STAGE_NUM], playerY[TOTAL_STAGE_NUM];	// player�� X, Y ��ǥ
char dx[4] = { -1, 1, 0, 0 };	// LEFT RIGHT UP DOWN
char dy[4] = { 0, 0, 1, -1 };

short buttonX[10] = { 160, 360, 560, 760, 960, 160, 360, 560, 760, 960 };     //stage ��ư x��ǥ
short buttonY[10] = { 410, 410, 410, 410, 410, 310, 310, 310, 310, 310 };     //stage ��ư y��ǥ

char fixedObjBoard[TOTAL_STAGE_NUM][BOARD_Y_NUM][BOARD_X_NUM];
char moveableObjBoard[TOTAL_STAGE_NUM][BOARD_Y_NUM][BOARD_X_NUM];
char numOfObject[TOTAL_STAGE_NUM][TOTAL_OBJECT_TYPE];

bool playing = false;

// �Լ��� �̺κк��� -------------------------

short coolX(char boardX) {
	short x = (1280 - (BOARD_X_NUM * BOARD_X_PIX)) / 2 + (boardX * BOARD_X_PIX);
	return x;
}

short coolY(char boardY) {
	short y = (720 - (BOARD_Y_NUM * BOARD_Y_PIX)) / 2 + (boardY * BOARD_Y_PIX);
	return y;
}

char numToASCII(char num) {
	return num + 48; // �ƽ�Ű 48��° : 0
}


ObjectID createObject(const char* image, SceneID scene, short x, short y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) {
		showObject(object);
	}
	return object;
}

ObjectID createObject(const char* imgLocate, SceneID scene, char boardX, char boardY, bool shown) {

	ObjectID object = createObject(imgLocate, scene, coolX(boardX), coolY(boardY), true);	
	return object;
}

// s1 : �ִ� ���� 19, num : 0~9 ����, s2 : �ִ� ���� 9
void strcat(char* s1, char num, char* s2) {

	char c = 0;
	char count = 0;     //���ѷ���������
	while (true) {
		count++;
		c = *s1;
		if (c == 0 || c == 0xCC || count >= 20) {
			break;
		}
		s1++;
	}
	*s1 = numToASCII(num);
	s1++;
	c = 0, count = 0;

	while (true) {
		c = *s2;
		if (c == 0 || c == 0xCC || count >= 10) {
			break;
		}
		*s1 = c;
		s1++;
		s2++;
	}
	*s1 = NULL;
}

ObjectID createObject(char objectType, SceneID scene, char boardX, char boardY, bool shown) {	
	char s1[20] = "Images/Object";
	char s2[10] = ".png";
	strcat(s1, objectType, s2);
	ObjectID object = createObject(s1, scene, boardX, boardY, shown);
	return object;
}


bool isFixedObject(char objectType) {
	if (objectType == SLAB || objectType == HOUSE || objectType == SPEAR || objectType == THRON || objectType == KEY || objectType == TREASURE || objectType == EXIT || objectType == FULL)
		return true;
	return false;
}

bool isMovableObject(char objectType) {
	if (objectType == BOX || objectType == ROCK || objectType == FULL) {
		return true;
	}
	return false;
}



// ObjectType �� �ִ°����� ������ �� �ִ°�?
bool canMove() {

	return false;

}

// ObjectID �� ��ȯ�ϴ°� �ƴ�. objectStruct[sceneNum][objectType][objectNum] ���� ���������� ã������ �Լ�.
char findObjectNum(char sceneNum, char objectType, char x, char y) {
	for (int i = 0; i < 30; i++) {
		if (objectStruct[sceneNum][objectType][i].x == x &&
			objectStruct[sceneNum][objectType][i].y == y) {
			return i;
		}			
	}
	printf("at func findObjectNum, cant find num\n"); // �����޼���
	return -1; 
}

void moveObject(char stageNum, char objectType, char fromX, char fromY, char toX, char toY) {
	if (objectType == BOX || objectType == ROCK) {
		char objNum = findObjectNum(stageNum, objectType, fromX, fromY);
		moveableObjBoard[stageNum][fromY][fromX] = 0;
		moveableObjBoard[stageNum][toY][toX] = objectType;		
		objectStruct[stageNum][objectType][objNum] = { toX, toY, objectType };
		locateObject(object[stageNum][objectType][objNum], sceneStage[stageNum], coolX(toX), coolY(toY));
	}
	else {
		printf("at func moveObject, tried to move unmovable object\n");	//�����޼���
	}
}

void movePlayer(char stageNum, char toX, char toY) {
	playerX[stageNum] = toX;
	playerY[stageNum] = toY;
	locateObject(player[stageNum], sceneStage[stageNum], coolX(toX), coolY(toY));
}

bool isStageClear(char stageNum) {
	for (char i = 0; i < numOfObject[stageNum][SLAB]; i++) {
		ObjectStruct objStr = objectStruct[stageNum][SLAB][i];
		if (moveableObjBoard[stageNum][objStr.y][objStr.x] != BOX) {
			return false;
		}
	}
	return true;
}

void initStage(char stageNum) {
	playing = false;
	char count[TOTAL_OBJECT_TYPE];
	for (char i = 0; i < TOTAL_OBJECT_TYPE; i++) {
		count[i] = 0;
	}
	printf("stage %d\n", stageNum);
	// initPlayer()
	for (char j = 0; j < BOARD_Y_NUM; j++) {
		for (char i = 0; i < BOARD_X_NUM; i++) {
			char objectType = initBoard[stageNum][j][i];
			if (objectType == PLAYER) {
				playerX[stageNum] = i; playerY[stageNum] = j;
			}
		}
	}
	movePlayer(stageNum, playerX[stageNum], playerY[stageNum]);
	
	// init movableObjBoard to 0
	for (char j = 0; j < BOARD_Y_NUM; j++) {
		for (char i = 0; i < BOARD_X_NUM; i++) {
			moveableObjBoard[stageNum][i][j] = 0;
		}
	}

	// init movableObj
	for (char j = 0; j < BOARD_Y_NUM; j++) {
		for (char i = 0; i < BOARD_X_NUM; i++) {
			char objectType = initBoard[stageNum][j][i];
			if (objectType == FULL) { objectType = BOX; }
			if (isMovableObject(objectType)) {
				printf("%d %d has movable\n", i, j);
				moveableObjBoard[stageNum][j][i] = objectType;
				objectStruct[stageNum][objectType][count[objectType]] = { i,j,objectType };
				locateObject(object[stageNum][objectType][count[objectType]], sceneStage[stageNum], coolX(i), coolY(j));
								
				count[objectType]++;
			}
		}
	}
	printf("Movable Obj Board\n");
	for (char j = BOARD_Y_NUM - 1; j >= 0; j--) {
		for (char i = 0; i < BOARD_X_NUM; i++) {
			printf("%d ", moveableObjBoard[3][j][i]);
		}
		printf("\n");
	}

	playing = true;
}

// 
void move(char keycode) {
	char code = keycode - 82;	// LEFT : 0, RIGHT : 1, UP : 2, DOWN : 3
	char mPlayerX = playerX[curStage] + dx[code];	// �̵��� ��
	char mPlayerY = playerY[curStage] + dy[code];
	char mvObjType = moveableObjBoard[curStage][mPlayerY][mPlayerX];
	char fxObjType = fixedObjBoard[curStage][mPlayerY][mPlayerX];
	
	
	// �����°��� �ڽ���
	if (mvObjType == BOX) {		
		// �� ����ĭ�� �ڽ��� ������ �� �ִ� ������ Ȯ���ϰ�
		char mBoxX = mPlayerX + dx[code];
		char mBoxY = mPlayerY + dy[code];
		char mvObjTypeB = moveableObjBoard[curStage][mBoxY][mBoxX];
		char fxObjTypeB = fixedObjBoard[curStage][mBoxY][mBoxX];
		// movableType�� �̹� �ִ� ���̸� ����
		if (mvObjTypeB > 0) {			
			// TODO : ���������� ǥ���ϴ� ������ ������ ������.
			
			//������ �� �Ҹ������� ����.
			playSound(blocked, false);
			return;
		}
		// fixed�� ���̸� ����
		if (fxObjTypeB == WALL || fxObjTypeB == TREASURE) {			
			playSound(blocked, false);
			return;
		}
		//���� ������		
		moveObject(curStage, BOX, mPlayerX, mPlayerY, mBoxX, mBoxY);
	}
	else if (fxObjType == WALL) {
		return;
	}
	//TODO ������� ���¸� TREASURE�� ���������� �ؾ���.

	movePlayer(curStage, mPlayerX, mPlayerY);

	if (isStageClear(curStage)) {
		playing = false;
		curStage++;
		if (curStage < TOTAL_STAGE_NUM)
		{
			//������ stageButton�� clear�̹����� ����
			setObjectImage(stageButton[curStage-1], "Images/clear.png");
			enterScene(selectStage);
			playing = true;
			playSound(oneclear, false);
		}
		else if (curStage == TOTAL_STAGE_NUM) {
			//��� ������ �������� Ŭ���� ȭ������ ������ ����.
			enterScene(clearStage);
			stopSound(bgm);
			playSound(clearsound);
		}
	}
}


void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	for (int i = 0; i < TOTAL_STAGE_NUM; i++) {
		if (object == stageButton[i]) {
			curStage = i;
			enterScene(sceneStage[i]);
			playing = true;
		}
		else if (object == backButton[i]) {
			playing = false;
			initStage(curStage);
			enterScene(selectStage);			
		}
		
	}
	if (object == quitbutton) {
		endGame();
	}
}

void keyboardCallback(KeyCode code, KeyState state)
{
	// ������ ���ۿ��� ����. ���°Ϳ� ���� X
	if (state == KeyState::KEYBOARD_PRESSED && playing && 0<=curStage && curStage<TOTAL_STAGE_NUM) {
		if (code >= 82 && code <= 85) {		// ����Ű
			move(code);

		}
		else if (code == 75 || code == 18) {	// SPACEBAR or 'R'
			initStage(curStage);
		}
		else {
			printf("undefined keycode : %d\n", code);
		}
	}

}


// �����Լ�
int main() {

	setKeyboardCallback(keyboardCallback);
	setMouseCallback(mouseCallback);

	bgm = createSound("sounds/���ڹ� ���.mp3");  //��� ���ѹݺ�
	playSound(bgm, true);

	blocked = createSound("sounds/�ε����� �Ҹ�.mp3");

	clearsound = createSound("sounds/Ŭ���� �Ҹ�.mp3");

	oneclear = createSound("sounds/�� �� Ŭ����.mp3");

	selectStage = createScene("�������� ����", "Images/Background.png");
	clearStage = createScene("�������� Ŭ����", "Images/stageclear.png");

	stageButton[0] = createObject("Images/Button0.png", selectStage, (short)buttonX[0], (short)buttonY[0], true);
	for (char n = 1; n < TOTAL_STAGE_NUM; n++) {
		char s1[20] = "Images/Button";
		char s2[10] = ".png";
		strcat(s1, n, s2);		
		stageButton[n] = createObject(s1, selectStage, (short)buttonX[n], (short)buttonY[n], true);				
	}



	for (char n = 0; n < TOTAL_STAGE_NUM; n++) {
		sceneStage[n] = createScene("���ڹ�", "Images/Background.png");
		backButton[n] = createObject("Images/Back.png", sceneStage[n], (short)100, (short)610, true);
	}


	// n : �������� ��ȣ,	j : y����,	i : x����
	for (char n = 0; n < TOTAL_STAGE_NUM; n++) {
		for (char j = 0; j < BOARD_Y_NUM; j++) {
			for (char i = 0; i < BOARD_X_NUM; i++) {
				char objectType = initBoard[n][j][i];
				
				if (objectType == EMPTY)	continue;
				// ���� �ٴ� �����		
				if (objectType == WALL) {
					tile[n][j][i] = createObject("Images/Wall.png", sceneStage[n], i, j, true);

					fixedObjBoard[n][j][i] = objectType;					
				}
				else {
					tile[n][j][i] = createObject("Images/Tile.png", sceneStage[n], i, j, true);		

					if (objectType == FULL) {
						fixedObjBoard[n][j][i] = SLAB;
						moveableObjBoard[n][j][i] = BOX;
					}
					else if (isFixedObject(objectType)) {
						fixedObjBoard[n][j][i] = objectType;
					}
					else if (objectType == PLAYER) {
						playerX[n] = i; playerY[n] = j;
					}
					else{
						moveableObjBoard[n][j][i] = objectType;
					}
				}								
			}
		}
	}
	printf("Fixed Obj Board\n");
	for (char j = BOARD_Y_NUM-1; j >=0 ; j--) {
		for (char i = 0; i < BOARD_X_NUM; i++) {
			printf("%d ", fixedObjBoard[3][j][i]);
		}
		printf("\n");
	}
	printf("Movable Obj Board\n");
	for (char j = BOARD_Y_NUM - 1; j >= 0; j--) {
		for (char i = 0; i < BOARD_X_NUM; i++) {			
			printf("%d ", moveableObjBoard[3][j][i]);
		}
		printf("\n");
	}

	

	// ������Ʈ�����
	for (char n = 0; n < TOTAL_STAGE_NUM; n++) {
		for (char j = 0; j < BOARD_Y_NUM; j++) {
			for (char i = 0; i < BOARD_X_NUM; i++) {
				char fxObjType = fixedObjBoard[n][j][i];				
				if (isFixedObject(fxObjType)) {
					object[n][fxObjType][numOfObject[n][fxObjType]] = createObject(fxObjType, sceneStage[n], i, j, true);
					objectStruct[n][fxObjType][numOfObject[n][fxObjType]] = { i, j, fxObjType};
					numOfObject[n][fxObjType] += 1;
				}								
			}
		}
	}
	for (char n = 0; n < TOTAL_STAGE_NUM; n++) {
		for (char j = 0; j < BOARD_Y_NUM; j++) {
			for (char i = 0; i < BOARD_X_NUM; i++) {				
				char mvObjType = moveableObjBoard[n][j][i];
				if (isMovableObject(mvObjType)) {
					object[n][mvObjType][numOfObject[n][mvObjType]] = createObject(mvObjType, sceneStage[n], i, j, true);
					objectStruct[n][mvObjType][numOfObject[n][mvObjType]] = { i, j, mvObjType };
					numOfObject[n][mvObjType] += 1;
				}
			}
		}
	}


	
	
	// player �����
	for (char n = 0; n < TOTAL_STAGE_NUM; n++) {
		player[n] = createObject("Images/Player.png", sceneStage[n], playerX[n], playerY[n], true);
	}

	quitbutton = createObject("Images/quit.png");
	locateObject(quitbutton, clearStage, 505, 130);
	showObject(quitbutton);

	playing = false;
	curStage = 0;
	startGame(selectStage);
}

