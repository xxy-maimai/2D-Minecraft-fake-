#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <cmath>

#define MAPX 128
#define MAPY 128

ExMessage m;

bool jump = false;

class Block{
	public:
		int x,y;
		int ID;
		int type;
};

class Player{
	public:
		int x,y;
};

class Item{
	public:
		int ID;
};

Item backpack[9];
int seclet_backpack = 0;
Block map[MAPX][MAPY];

Item stone_item;
Item grass_block_item;
Item dirt_item;
Item oak_log_item;

Block air;
Block stone;
Block grass_block;
Block bedrock;
Block dirt;
Block oak_log;
Block oak_leaves;

Player player;

IMAGE stoneImg;
IMAGE grass_blockImg;
IMAGE bedrockImg;
IMAGE dirtImg;
IMAGE playerImg;
IMAGE oak_logImg;
IMAGE oak_leavesImg;

IMAGE front_backpackImg;
IMAGE seclet_backpackImg;

IMAGE stone_itemImg;
IMAGE grass_block_itemImg;
IMAGE dirt_itemImg;
IMAGE oak_log_itemImg;

void initgame();
void createMap();
void display();
void gameloop();
void messageHandling();
void move(int x,int y);

int main(){
	initgraph(1000,600);
	
	//block和player
	loadimage(&grass_blockImg,"images/grass_block.png");
	loadimage(&stoneImg,"images/stone.png");
	loadimage(&bedrockImg,"images/bedrock.png");
	loadimage(&dirtImg,"images/dirt.png");
	loadimage(&playerImg,"images/player.png");
	loadimage(&oak_logImg,"images/oak_log.png");
	loadimage(&oak_leavesImg,"images/oak_leaves.png");
	
	//UI
	loadimage(&front_backpackImg,"images/front_backpack.png");
	loadimage(&seclet_backpackImg,"images/seclet_backpack.png");
	
	//item
	loadimage(&stone_itemImg,"images/stone_item.png");
	loadimage(&grass_block_itemImg,"images/grass_block_item.png");
	loadimage(&dirt_itemImg,"images/dirt_item.png");
	loadimage(&oak_log_itemImg,"images/oak_log_item.png");
	
	setbkcolor(WHITE);
	cleardevice();
	settextstyle(20,10,_T("Courier New"));
	settextcolor(BLACK);
	
	initgame();
	
	createMap();
	
	gameloop();
	
	_getch();
	
	closegraph();
	return 0;
}

void initgame(){
	air.ID = 0;
	stone.ID = 1;
	grass_block.ID = 2;
	bedrock.ID = 3;
	dirt.ID = 4;
	oak_log.ID = 5;
	oak_leaves.ID = 6;
	
	air.type = 0;
	stone.type = 1;
	grass_block.type = 1;
	bedrock.type = 1;
	dirt.type = 1;
	oak_log.type = 1;
	oak_leaves.type = 1;
	
	player.x = 11;
	player.y = 7;
	
	stone_item.ID = 0;
	grass_block_item.ID = 1;
	dirt_item.ID = 2;
	oak_log_item.ID = 3;
	
	for(int i = 0;i < 9;i++){
		backpack[i].ID = -1;
	}
	
	backpack[0] = stone_item;
	backpack[1] = grass_block_item;
	backpack[2] = dirt_item;
	backpack[3] = oak_log_item;
}

void createMap(){
	Block tree[6][5] = {
		{air,oak_leaves,oak_leaves,oak_leaves,air},
		{oak_leaves,oak_leaves,oak_leaves,oak_leaves,oak_leaves},
		{oak_leaves,oak_leaves,oak_leaves,oak_leaves,oak_leaves},
		{air,air,oak_log,air,air},
		{air,air,oak_log,air,air},
		{air,air,oak_log,air,air}
	};
	//srand((unsigned)time(NULL));
	for(int i = 0;i < MAPY;i++){
		for(int j = 0;j < MAPX;j++){
			if(i < 7){
				map[i][j] = air;
			}
			if(i == 7){
				map[i][j] = grass_block;
			}
			if(i > 7 && i < 9){
				map[i][j] = dirt;
			}
			if(i >= 9 && i < MAPY-1){
				map[i][j] = stone;
			}
			if(i == MAPY-1){
				map[i][j] = bedrock;
			}
			map[i][j].x = j;
			map[i][j].y = i;
		}
	}
	for(int i = 0;i < 6;i++){
		for(int j = 0;j < 5;j++){
			map[i+1][j] = tree[i][j];
			map[i+1][j].x = j;
			map[i+1][j].y = i+1;
		}
	}
	/*map[6][15] = dirt;
	map[6][15].x = 15;
	map[6][15].y = 6;*/
}

void display(){
	BeginBatchDraw();
	for(int i = 0;i < MAPY;i++){
		for(int j = 0;j < MAPX;j++){
			int x = map[i][j].x * 40;
			int y = map[i][j].y * 40;
			if(x < (MAPX*40)  && y < (MAPY*40)){
				switch(map[i][j].ID){
					case 1:
						putimage(x,y,&stoneImg);
						break;
					case 2:
						putimage(x,y,&grass_blockImg);
						break;
					case 3:
						putimage(x,y,&bedrockImg);
						break;
					case 4:
						putimage(x,y,&dirtImg);
						break;
					case 5:
						putimage(x,y,&oak_logImg);
						break;
					case 6:
						putimage(x,y,&oak_leavesImg);
						break;
				}	
			}
		}
	}
	putimage(10*40,5*40,&playerImg);//x = 11,y(foot) = 7
	putimage(312.5,520,&front_backpackImg);
	putimage(312.5+42*seclet_backpack,520,&seclet_backpackImg);
	for(int i = 0;i < 9;i++){
		switch(backpack[i].ID){
			case 0:
				putimage(312.5+42*i+10,520+10,&stone_itemImg);
				break;
			case 1:
				putimage(312.5+42*i+10,520+10,&grass_block_itemImg);
				break;
			case 2:
				putimage(312.5+42*i+10,520+10,&dirt_itemImg);
				break;
			case 3:
				putimage(312.5+42*i+10,520+10,&oak_log_itemImg);
				break;
		}
	}
	EndBatchDraw();
}

void messageHandling(){
	while(peekmessage(&m,EX_KEY|EX_MOUSE)){
		int tempx = floor(m.x / 40);
		int tempy = floor(m.y / 40);
		switch(m.message){
			case WM_KEYDOWN:
				if(m.vkcode == 0x41 && !( (map[player.y - 1][player.x - 2].type == 1) || (map[player.y - 2][player.x - 2].type == 1) ) && !(player.x < 2)){
					move(-1,0);
					cleardevice();
				}
				if(m.vkcode == 0x44 && !( (map[player.y - 1][player.x].type == 1) || (map[player.y - 2][player.x].type == 1) ) && !(player.x > MAPX - 1)){
					move(1,0);
					cleardevice();
				}
				if(m.vkcode == VK_SPACE && (map[player.y - 3][player.x - 1].type != 1)){
					jump = 1;
					move(0,-1);
					cleardevice();
				}
				//设置当前选择物品（seclet_backpack）
				if(m.vkcode == 0x31){
					seclet_backpack = 0;
					cleardevice();
				}
				if(m.vkcode == 0x32){
					seclet_backpack = 1;
					cleardevice();
				}
				if(m.vkcode == 0x33){
					seclet_backpack = 2;
					cleardevice();
				}
				if(m.vkcode == 0x34){
					seclet_backpack = 3;
					cleardevice();
				}
				if(m.vkcode == 0x35){
					seclet_backpack = 4;
					cleardevice();
				}
				if(m.vkcode == 0x36){
					seclet_backpack = 5;
					cleardevice();
				}
				if(m.vkcode == 0x37){
					seclet_backpack = 6;
					cleardevice();
				}
				if(m.vkcode == 0x38){
					seclet_backpack = 7;
					cleardevice();
				}
				if(m.vkcode == 0x39){
					seclet_backpack = 8;
					cleardevice();
				}
				break;
			case WM_KEYUP:
				if(m.vkcode == VK_SPACE){
					jump = 0;
				}
				break;
			case WM_LBUTTONDOWN:
				for(int i = 0;i < MAPY;i++){
					for(int j = 0;j < MAPX;j++){
						if(map[i][j].y == tempy && map[i][j].x == tempx){
							if(map[i][j].ID != 3){
								map[i][j] = air;
								map[i][j].x = tempx;
								map[i][j].y = tempy;
							}
						}
					}
				}
				cleardevice();
				break;
			case WM_RBUTTONDOWN:
				for(int i = 0;i < MAPY;i++){
					for(int j = 0;j < MAPX;j++){
						if(map[i][j].x == tempx && map[i][j].y == tempy){
							if(map[i][j].type == 0 && (map[i+1][j].type == 1 || map[i][j-1].type == 1 || map[i][j+1].type == 1 || map[i-1][j].type == 1)){
								switch(backpack[seclet_backpack].ID){
									case 0:
										map[i][j] = stone;
										break;
									case 1:
										map[i][j] = grass_block;
										break;
									case 2:
										map[i][j] = dirt;
										break;
									case 3:
										map[i][j] = oak_log;
								}
							}
							map[i][j].x = tempx;
							map[i][j].y = tempy;
						}
					}
				}
		}
	}
	if(!(map[player.y][player.x - 1].type == 1) && jump == 0){
		move(0,1);
		Sleep(25);
		cleardevice();
	}	
}

void move(int x,int y){
	player.x += x;
	player.y += y;
	for(int i = 0;i < MAPY;i++){
		for(int j = 0;j < MAPX;j++){
			map[i][j].x -= x;
			map[i][j].y -= y;
		}
	}
}

void gameloop(){
	while(1){
		display();
		messageHandling();
	}
}