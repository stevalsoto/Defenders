//Using libs SDL, glibc
#include <SDL2/SDL.h>	//SDL version 2.0
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#define SIZE_MAPS 500
#define ESCALA 8
#define SCREEN_WIDTH 1024	//window height
#define SCREEN_HEIGHT 768	//window width
//function prototypes
//initilise SDL
int init(int w, int h, int argc, char *args[]);

typedef struct ball {

	int x, y; //position on the screen/
	int w,h; // ball width and height
	int dx, dy; /* movement vector */
} ball_t;

typedef struct disparo{
	int x, y; // posiciones del disparo
	int w,h ; // dimensiones del disparo
	int dx, dy; // movimiento
} disparo_t;

typedef struct paddle {
	int x,y;
	int w,h;
	int l;//lives
} paddle_t;

// Program globals
static ball_t balaverde[4];
static ball_t mina[4];
static ball_t balaplatillo[4];
static paddle_t paddle[20];
static disparo_t disparo[200];
int active[200];
int dirdisp = 0;
int score[] = {0,0,0,0};
int vidas=3;
int width, height;		//used if fullscreen
int z=2;
int bomba=0;
int cantbomba=3;
//
SDL_Window* window = NULL;	//The window we'll be rendering to
SDL_Renderer *renderer;		//The renderer SDL will use to draw to the screen

		//surfaces//
//------------------------
static SDL_Surface *screen;
static SDL_Surface *pointillist;
static SDL_Surface *title;
static SDL_Surface *numbermap;
static SDL_Surface *end;
//------------------------
static SDL_Surface *verde1;
static SDL_Surface *verde2;
static SDL_Surface *verde3;
static SDL_Surface *verde4;
//------------------------
static SDL_Surface *multicolor1;
static SDL_Surface *multicolor2;
static SDL_Surface *multicolor3;
static SDL_Surface *multicolor4;
//------------------------
static SDL_Surface *platillo1;
static SDL_Surface *platillo2;
static SDL_Surface *platillo3;
static SDL_Surface *platillo4;
//------------------------
static SDL_Surface *jugadorL;
static SDL_Surface *jugadorR;
//------------------------
static SDL_Surface *lives;
static SDL_Surface *bomb;
static SDL_Surface *numbermap;
static SDL_Surface *balaR;
static SDL_Surface *balaL;
//------------------------
static SDL_Surface *human1;
static SDL_Surface *human2;
static SDL_Surface *human3;
static SDL_Surface *human4;
//------------------------
static SDL_Surface *win;
//------------------------
static SDL_Surface *balaV1;
static SDL_Surface *balaV2;
static SDL_Surface *balaV3;
static SDL_Surface *balaV4;
//------------------------
static SDL_Surface *balaP1;
static SDL_Surface *balaP2;
static SDL_Surface *balaP3;
static SDL_Surface *balaP4;
//------------------------
static SDL_Surface *mina1;
static SDL_Surface *mina2;
static SDL_Surface *mina3;
static SDL_Surface *mina4;

//textures
SDL_Texture *screen_texture;

int maps[SIZE_MAPS][2];
//inisilize starting position and sizes of game elemements

int randint(int a, int b) {
	return a + rand() % (b - a + 1);
}

void drawLine(int x1, int y1, int x2, int y2, int size, uint32_t color) {
	SDL_Rect src = {0, 0, size, size}; 
	double x, y, addx, addy, length;

	x = x2 - x1;
	y = y2 - y1;
	
	length = sqrt( x*x + y*y );

	addx = x / length;
	addy = y / length;
	x = x1;
	y = y1;

	for (int i = 0; i < length; i++) {
		src.x = (int)x; src.y = (int)y;
		SDL_FillRect(screen, &src, color);
		x += addx; y += addy;
	}
}

void map() {
	int x = 0;
	for (int i = 0; i < SIZE_MAPS; i++) {
		maps[i][0] = x;
		maps[i][1] = randint(0, 70);
		x = randint(x, x+100);
	}
}

int half_map = 10000;

static void draw_framework() {

	int x = 0, y = 0;
	int offset_y0 = 75, offset_x0 = 445, offset_y1 = 650;
	for (int i = 0; i < SIZE_MAPS; i++) {

		// Map
		drawLine(x - half_map, y + offset_y1, maps[i][0] - half_map, maps[i][1] + offset_y1, 2, 0x9bff1e11);

		// Minimap
		drawLine((x - half_map) / ESCALA + offset_x0, y / ESCALA + offset_y0, (maps[i][0] - half_map) / ESCALA + offset_x0, maps[i][1] / ESCALA +offset_y0, 1, 0x9bff1e11);
		drawLine((paddle[0].x)/ESCALA+offset_x0,(paddle[0].y)/ESCALA,(paddle[0].x+10)/ESCALA +offset_x0, (paddle[0].y)/ESCALA,6,0xffffffff);
		for(int j=1;j<13;j++){
			if(paddle[j].x<1020 && paddle[j].x>0 && paddle[j].y>0 && paddle[j].x<764){
				drawLine((paddle[j].x)/ESCALA+offset_x0,(paddle[j].y)/ESCALA,(paddle[j].x+10)/ESCALA +offset_x0, (paddle[j].y)/ESCALA,3,0xff0fffff);
			}
		}
		x = maps[i][0];
		y = maps[i][1];
	}
	
		// Minimap(delete) //
	drawLine(762, 40, 1024, 40, 50, 0x000000); 
	drawLine(0, 40, 220, 40, 50, 0x000000);
	if(score[0]<3){
		// Short Horizontal line //
		drawLine(262, 0, 762, 0, 3, 0xff6fffff);
		// Long Horizontal line //
		drawLine(0, 91, 1024, 91, 5, 0xff6fffff); 
		// Left Column //
		drawLine(262, 0, 262, 91, 5, 0xff6fffff); 
		// Rigth Column //
		drawLine(762, 0, 762, 91, 5, 0xff6fffff);
	}
	if(score[0]>=3 && score[0]<6 && score[1]>=0){
		// Short Horizontal line //
		drawLine(262, 0, 762, 0, 3, 0xff529B65);
		// Long Horizontal line //
		drawLine(0, 91, 1024, 91, 5, 0xff529B65); 
		// Left Column //
		drawLine(262, 0, 262, 91, 5, 0xff529B65); 
		// Rigth Column //
		drawLine(762, 0, 762, 91, 5, 0xff529B65);
	}
	if(score[0]>=6 && score[0]<10 && score[1]>=0){
		// Short Horizontal line //
		drawLine(262, 0, 762, 0, 3, 0xB02424ff);
		// Long Horizontal line //
		drawLine(0, 91, 1024, 91, 5, 0xB02424ff); 
		// Left Column //
		drawLine(262, 0, 262, 91, 5, 0xB02424ff); 
		// Rigth Column //
		drawLine(762, 0, 762, 91, 5, 0xB02424ff);
	}

	// Short Horizontal line //
			// Wite minimap //
	drawLine(452, 00, 572, 00, 5, 0xffffffff); 
	drawLine(452, 91, 572, 91, 5, 0xffffffff);
	drawLine(452, 0, 452, 8, 4, 0xffffffff);
	drawLine(452, 86, 452, 92, 4, 0xffffffff);
	drawLine(572, 0, 572, 8, 4, 0xffffffff);
	drawLine(572, 86, 572, 92, 4, 0xffffffff);
}

int randomNumber(int min, int max){
	static bool initialized = false;
	if(!initialized){
		initialized=true;
		srand(time(NULL));
	}
	return rand() % max +min;
}

static void init_disparo(){
	for(int i=0; i<200; i++){
		disparo[i].x = 1300;
		disparo[i].y = 1300;
		disparo[i].w = 10;
		disparo[i].h = 10;
		disparo[i].dx = 0 ;
		disparo[i].dy= 0 ;
		balaR = SDL_LoadBMP("balaR.bmp");
		balaL = SDL_LoadBMP("balaL.bmp");
	}
}			
static void init_balaverde1() {	
	balaverde[0].x = (paddle[1].x+20);
	balaverde[0].y = (paddle[1].y+20);
	balaverde[0].w = 10;
	balaverde[0].h = 10;
	balaverde[0].dy = 1;
	balaverde[0].dx = 1;
	balaV1 = SDL_LoadBMP("balaER.bmp"); 
}
static void init_balaverde2() {	
	balaverde[1].x = (paddle[2].x+20);
	balaverde[1].y = (paddle[2].y+20);
	balaverde[1].w = 10;
	balaverde[1].h = 10;
	balaverde[1].dy = 1;
	balaverde[1].dx = 1;
	balaV2 = SDL_LoadBMP("balaEL.bmp");  
}

static void init_balaverde3() {	
	balaverde[2].x = (paddle[3].x+20);
	balaverde[2].y = (paddle[3].y+20);
	balaverde[2].w = 10;
	balaverde[2].h = 10;
	balaverde[2].dy = 1;
	balaverde[2].dx = 1;
	balaV3 = SDL_LoadBMP("balaER.bmp");
}
static void init_balaverde4() {	
	balaverde[3].x = (paddle[4].x+20);
	balaverde[3].y = (paddle[4].y+20);
	balaverde[3].w = 10;
	balaverde[3].h = 10;
	balaverde[3].dy = 1;
	balaverde[3].dx = 1;
	balaV4 = SDL_LoadBMP("balaEL.bmp");  
}

static void init_verde1(){
	paddle[1].x = randomNumber(50,150);
	paddle[1].y = 725;
	paddle[1].w = 20;
	paddle[1].h = 30;
	paddle[1].l =0;
	verde1 = SDL_LoadBMP("verde1.bmp");
}
static void init_verde2(){
	paddle[2].x = randomNumber(850,950);
	paddle[2].y = 725;
	paddle[2].w = 20;
	paddle[2].h = 30;
	paddle[2].l =0;
	verde2 = SDL_LoadBMP("verde2.bmp");
}
static void init_verde3(){
	paddle[3].x = randomNumber(150,250);
	paddle[3].y = 720;
	paddle[3].w = 20;
	paddle[3].h = 30;
	paddle[3].l =0;
	verde3 = SDL_LoadBMP("verde3.bmp");
}
static void init_verde4(){
	paddle[4].x = randomNumber(750,850);
	paddle[4].y = 725;
	paddle[4].w = 20;
	paddle[4].h = 30;
	paddle[4].l =0;
	verde4 = SDL_LoadBMP("verde4.bmp");
}
static void init_multicolor1(){
	paddle[5].x = randomNumber(100,200);
	paddle[5].y = randomNumber(100,550);
	paddle[5].w = 20;
	paddle[5].h = 20;
	paddle[5].l =0;
	multicolor1 = SDL_LoadBMP("multicolor1.bmp");
}
static void init_multicolor2(){
	paddle[6].x = randomNumber(700,800);
	paddle[6].y = randomNumber(100,550);
	paddle[6].w = 20;
	paddle[6].h = 20;
	paddle[6].l =0;
	multicolor2 = SDL_LoadBMP("multicolor2.bmp");
}
static void init_multicolor3(){
	paddle[7].x = randomNumber(200,300);
	paddle[7].y = randomNumber(100,550);
	paddle[7].w = 20;
	paddle[7].h = 20;
	paddle[7].l =0;	
	multicolor3 = SDL_LoadBMP("multicolor3.bmp");
}
static void init_multicolor4(){
	paddle[8].x = randomNumber(800,900);
	paddle[8].y = randomNumber(100,550);
	paddle[8].w = 20;
	paddle[8].h = 20;
	paddle[8].l =0;
	multicolor4 = SDL_LoadBMP("multicolor4.bmp");
}
static void init_platillo1(){
	paddle[9].x = randomNumber(100,200);
	paddle[9].y = randomNumber(100,550);
	paddle[9].w = 50;
	paddle[9].h = 20;
	paddle[9].l =0;
	platillo1 = SDL_LoadBMP("platillo1.bmp");
}
static void init_platillo2(){
	paddle[10].x = randomNumber(700,800);
	paddle[10].y = randomNumber(100,550);
	paddle[10].w = 50;
	paddle[10].h = 20;
	paddle[10].l =0;
	platillo2 = SDL_LoadBMP("platillo2.bmp");
}
static void init_platillo3(){
	paddle[11].x = randomNumber(200,300);
	paddle[11].y = randomNumber(100,550);
	paddle[11].w = 50;
	paddle[11].h = 20;
	paddle[11].l =0;
	platillo3 = SDL_LoadBMP("platillo3.bmp");
}
static void init_platillo4(){
	paddle[12].x = randomNumber(850,900);
	paddle[12].y = randomNumber(100,550);
	paddle[12].w = 50;
	paddle[12].h = 20;
	paddle[12].l =0;
	platillo4 = SDL_LoadBMP("platillo4.bmp");
}
static void init_jugador(){
	paddle[0].x = screen->w / 2;
	paddle[0].y = screen->h / 2;
	paddle[0].w = 80;
	paddle[0].h = 25;
	paddle[0].l =0;
	jugadorL = SDL_LoadBMP("jugadorL1.bmp");	
	jugadorR = SDL_LoadBMP("jugadorR1.bmp");
}
static void init_human1(){
	paddle[13].x = 200;
	paddle[13].y = 725;
	paddle[13].w = 20;
	paddle[13].h = 30;
	paddle[13].l=1;
	human1 = SDL_LoadBMP("human1.bmp");
}
static void init_human2(){
	paddle[14].x = 400;
	paddle[14].y = 725;
	paddle[14].w = 20;
	paddle[14].h = 30;
	paddle[14].l=1;
	human2 = SDL_LoadBMP("human2.bmp");
}
static void init_human3(){
	paddle[15].x = 600;
	paddle[15].y = 725;
	paddle[15].w = 20;
	paddle[15].h = 30;
	paddle[15].l=1;
	human3 = SDL_LoadBMP("human3.bmp");
}
static void init_human4(){
	paddle[16].x = 800;
	paddle[16].y = 725;
	paddle[16].w = 20;
	paddle[16].h = 30;
	paddle[16].l=1;
	human4 = SDL_LoadBMP("human4.bmp");
}
static void init_mina1() {	
	mina[0].x = (paddle[5].x+30);
	mina[0].y = (paddle[5].y+30);
	mina[0].w = 10;
	mina[0].h = 10;
	mina[0].dy = 1;
	mina[0].dx = 1;
	mina1 = SDL_LoadBMP("mina.bmp");
}

static void init_mina2() {	
	mina[1].x = (paddle[6].x+30);
	mina[1].y = (paddle[6].y+30);
	mina[1].w = 10;
	mina[1].h = 10;
	mina[1].dy = 1;
	mina[1].dx = 1;
	mina2 = SDL_LoadBMP("mina.bmp");
}

static void init_mina3() {	
	mina[2].x = (paddle[7].x+30);
	mina[2].y = (paddle[7].y+30);
	mina[2].w = 10;
	mina[2].h = 10;
	mina[2].dy = 1;
	mina[2].dx = 1;
	mina3 = SDL_LoadBMP("mina.bmp"); 
}
static void init_mina4() {	
	mina[3].x = (paddle[8].x+30);
	mina[3].y = (paddle[8].y+30);
	mina[3].w = 10;
	mina[3].h = 10;
	mina[3].dy = 1;
	mina[3].dx = 1;
	mina4 = SDL_LoadBMP("mina.bmp");
}

static void init_balaplatillo1() {	
	balaplatillo[0].x = (paddle[9].x+20);
	balaplatillo[0].y = (paddle[9].y+20);
	balaplatillo[0].w = 10;
	balaplatillo[0].h = 10;
	balaplatillo[0].dy = 1;
	balaplatillo[0].dx = 1;	
	balaP1 = SDL_LoadBMP("balaER.bmp");
}

static void init_balaplatillo2() {	
	balaplatillo[1].x = (paddle[10].x+20);
	balaplatillo[1].y = (paddle[10].y+20);
	balaplatillo[1].w = 10;
	balaplatillo[1].h = 10;
	balaplatillo[1].dy = 1;
	balaplatillo[1].dx = 1;
	balaP2 = SDL_LoadBMP("balaEL.bmp");	
}

static void init_balaplatillo3() {	
	balaplatillo[2].x = (paddle[11].x+20);
	balaplatillo[2].y = (paddle[11].y+20);
	balaplatillo[2].w = 10;
	balaplatillo[2].h = 10;
	balaplatillo[2].dy = 1;
	balaplatillo[2].dx = 1;	
	balaP3 = SDL_LoadBMP("balaER.bmp");
}
static void init_balaplatillo4() {	
	balaplatillo[3].x = (paddle[12].x+20);
	balaplatillo[3].y = (paddle[12].y+20);
	balaplatillo[3].w = 10;
	balaplatillo[3].h = 10;
	balaplatillo[3].dy = 1;
	balaplatillo[3].dx = 1;
	balaP4 = SDL_LoadBMP("balaEL.bmp");	
}

static void vida() {
	int last_time=0;
	int time_now=SDL_GetTicks();	
	if(time_now>last_time+5000){
		vidas-=1;			
		last_time=time_now;	
		init_jugador();	
	}
}

//if return value is 1 collision occured. if return is 0, no collision.
int check_collision(disparo_t a, paddle_t b) {

	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	left_b = b.x;
	right_b = b.x + b.w;
	top_b = b.y;
	bottom_b = b.y + b.h;
	
	if (left_a > right_b) {
		return 0;
	}

	if (right_a < left_b) {
		return 0;
	}

	if (top_a > bottom_b) {
		return 0;
	}

	if (bottom_a < top_b) {
		return 0;
	}

	return 1;
}

int check_collisionBE(ball_t a, paddle_t b) {

	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	left_b = b.x;
	right_b = b.x + b.w;
	top_b = b.y;
	bottom_b = b.y + b.h;
	
	if (left_a > right_b) {
		return 0;
	}

	if (right_a < left_b) {
		return 0;
	}

	if (top_a > bottom_b) {
		return 0;
	}

	if (bottom_a < top_b) {
		return 0;
	}

	return 1;
}


int check_collision_enemy(paddle_t a, paddle_t b) {

	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	left_b = b.x;
	right_b = b.x + b.w;
	top_b = b.y;
	bottom_b = b.y + b.h;
	

	if (left_a > right_b) {
		return 0;
	}

	if (right_a < left_b) {
		return 0;
	}

	if (top_a > bottom_b) {
		return 0;
	}

	if (bottom_a < top_b) {
		return 0;
	}

	return 1;
}
int k=0;
static void reiniciar_enemigo1(){
	paddle[1].x=2000;
	paddle[1].y=558;
	k=randomNumber(0,12);	
	if (k==1){
		init_verde1();
	}
}
static void reiniciar_enemigo2(){
	paddle[2].x=2000;
	paddle[2].y=758;
	k=randomNumber(0,12);
	if (k==2){
		init_verde2();
	}
}
static void reiniciar_enemigo3(){
	paddle[3].x=2000;
	paddle[3].y=758;
	k=randomNumber(0,12);
	if (k==3){
		init_verde3();
	}
}
static void reiniciar_enemigo4(){
	paddle[4].x=2000;
	paddle[4].y=758;
	k=randomNumber(0,12);
	if (k==4){
		init_verde4();
	}
}
static void reiniciar_enemigo5(){
	paddle[5].x=2000;
	paddle[5].y=758;
	k=randomNumber(0,12);
	if (k==5){
		init_multicolor1();
	}
}
static void reiniciar_enemigo6(){
	paddle[6].x=2000;
	paddle[6].y=758;
	k=randomNumber(0,12);
	if (k==6){
		init_multicolor2();
	}
}	
static void reiniciar_enemigo7(){
	paddle[7].x=2000;
	paddle[7].y=758;
	k=randomNumber(0,12);
	if (k==7){
		init_multicolor3();
	}
}
static void reiniciar_enemigo8(){
	paddle[8].x=2000;
	paddle[8].y=758;
	k=randomNumber(0,12);
	if (k==8){
		init_multicolor4();
	}
}
static void reiniciar_enemigo9(){
	paddle[9].x=2000;
	paddle[9].y=758;
	k=randomNumber(0,12);
	if (k==9){
		init_platillo1();
	}
}
static void reiniciar_enemigo10(){
	paddle[10].x=2000;
	paddle[10].y=758;
	k=randomNumber(0,12);
	if (k==10){
		init_platillo2();
	}
}
static void reiniciar_enemigo11(){
	paddle[11].x=2000;
	paddle[11].y=758;
	k=randomNumber(0,12);
	if (k==11){
		init_platillo3();
	}
}
static void reiniciar_enemigo12(){
	paddle[12].x=2000;
	paddle[12].y=758;
	k=randomNumber(0,12);
	if (k==12){
		init_platillo4();
	}
}

static void collision(){
	for(int i=0; i<200;i++){
		if(paddle[1].l==1){
			if(check_collision(disparo[i],paddle[1])==1){
				init_verde1();
				disparo[i].x = 1300;
				active[i] = 0;
				score[1]+=1;
				score[2]+=5;
				score[3]+=0;
				score[0]+=0;
			}
		}
		if(paddle[2].l==1){
			if(check_collision(disparo[i],paddle[2])==1){
				reiniciar_enemigo2();
				score[1]+=1;
				score[2]+=5;
				score[3]+=0;
				score[0]+=0;
				active[i] = 0;
				disparo[i].x = 1300;
			}
		}
		if(paddle[3].l==1){
			if(check_collision(disparo[i],paddle[3])==1){
				reiniciar_enemigo3();
				score[1]+=1;
				score[2]+=5;
				score[3]+=0;
				score[0]+=0;
				active[i] = 0;
				disparo[i].x = 1300;
			}
		}
		if(paddle[4].l==1){
			if(check_collision(disparo[i],paddle[4])==1){
				reiniciar_enemigo4();
				score[1]+=1;
				score[2]+=5;
				score[3]+=0;
				score[0]+=0;
				active[i] = 0;
				disparo[i].x = 1300;
			}
		}
		if(paddle[5].l==1){
			if(check_collision(disparo[i],paddle[5])==1){
				reiniciar_enemigo5();
				score[1]+=1;
				score[2]+=5;
				score[3]+=0;
				score[0]+=0;
				active[i] = 0;
				disparo[i].x = 1300;
			}
		}
		if(paddle[6].l==1){
			if(check_collision(disparo[i],paddle[6])==1){
				reiniciar_enemigo6();
				score[1]+=1;
				score[2]+=5;
				score[3]+=0;
				score[0]+=0;
				active[i] = 0;
				disparo[i].x = 1300;
			}
		}
		if(paddle[7].l==1){
			if(check_collision(disparo[i],paddle[7])==1){
				reiniciar_enemigo7();
				score[1]+=1;
				score[2]+=5;
				score[3]+=0;
				score[0]+=0;
				active[i] = 0;
				disparo[i].x = 1300;
			}
		}
		if(paddle[8].l==1){
			if(check_collision(disparo[i],paddle[8])==1){
				reiniciar_enemigo8();
				score[1]+=1;
				score[2]+=5;
				score[3]+=0;
				score[0]+=0;		
				active[i] = 0;
				disparo[i].x = 1300;
			}
		}
		if(paddle[9].l==1){
			if(check_collision(disparo[i],paddle[9])==1){
				reiniciar_enemigo9();
				score[0]+=1;
				score[1]+=0;
				score[2]+=0;
				score[3]+=0;
				
				
				active[i] = 0;
				disparo[i].x = 1300;
			}
		}
		if(paddle[10].l==1){
			if(check_collision(disparo[i],paddle[10])==1){
				reiniciar_enemigo10();
				score[0]+=1;
				score[1]+=0;
				score[2]+=0;
				score[3]+=0;
				
				
				active[i] = 0;
				disparo[i].x = 1300;
			}
		}
		if(paddle[11].l==1){
			if(check_collision(disparo[i],paddle[11])==1){
				reiniciar_enemigo11();
				score[0]+=1;
				score[1]+=0;
				score[2]+=0;
				score[3]+=0;
				active[i] = 0;
				disparo[i].x = 1300;
			}
		}
		if(paddle[12].l==1){
			if(check_collision(disparo[i],paddle[12])==1){
				reiniciar_enemigo12();
				score[0]+=1;
				score[1]+=0;
				score[2]+=0;
				score[3]+=0;
				active[i] = 0;
				disparo[i].x = 1300;
			}
		}
	}
	for(int i=1;i<13;i++){
		if(paddle[i].l==1){
			if(check_collision_enemy(paddle[0],paddle[i])==1){
				vida();
				reiniciar_enemigo1();
				reiniciar_enemigo2();
				reiniciar_enemigo3();
				reiniciar_enemigo4();
				reiniciar_enemigo5();
				reiniciar_enemigo6();
				reiniciar_enemigo7();
				reiniciar_enemigo8();
				reiniciar_enemigo9();
				reiniciar_enemigo10();
				reiniciar_enemigo11();
				reiniciar_enemigo12();
			}
		}
	}
	for(int i=0;i<4;i++){
		if(check_collisionBE(balaverde[i],paddle[0])==1){
			vida();
			init_mina1();
			init_mina2();
			init_mina3();
			init_mina4();
			reiniciar_enemigo1();
			reiniciar_enemigo2();
			reiniciar_enemigo3();
			reiniciar_enemigo4();
			reiniciar_enemigo5();
			reiniciar_enemigo6();
			reiniciar_enemigo7();
			reiniciar_enemigo8();
			reiniciar_enemigo9();
			reiniciar_enemigo10();
			reiniciar_enemigo11();
			reiniciar_enemigo12();
		}
	}
	for(int i=0;i<4;i++){
		if(check_collisionBE(balaplatillo[i],paddle[0])==1){
			vida();
			init_mina1();
			init_mina2();
			init_mina3();
			init_mina4();
			reiniciar_enemigo1();
			reiniciar_enemigo2();
			reiniciar_enemigo3();
			reiniciar_enemigo4();
			reiniciar_enemigo5();
			reiniciar_enemigo6();
			reiniciar_enemigo7();
			reiniciar_enemigo8();
			reiniciar_enemigo9();
			reiniciar_enemigo10();
			reiniciar_enemigo11();
			reiniciar_enemigo12();
		}
	}
	for(int i=0;i<4;i++){
		if(check_collisionBE(mina[i],paddle[0])==1){
			vida();
			init_mina1();
			init_mina2();
			init_mina3();
			init_mina4();
			reiniciar_enemigo1();
			reiniciar_enemigo2();
			reiniciar_enemigo3();
			reiniciar_enemigo4();
			reiniciar_enemigo5();
			reiniciar_enemigo6();
			reiniciar_enemigo7();
			reiniciar_enemigo8();
			reiniciar_enemigo9();
			reiniciar_enemigo10();
			reiniciar_enemigo11();
			reiniciar_enemigo12();
		}
	}

	if(score[2]>9){
		score[2]=0;
		score[1]+=1;
	}
	if(score[1]>9){
		score[1]=0;
		score[0]+=1;
	}
}
//Disparos enemigos verdes//
static void disparoE1(){
	balaverde[0].x +=5;
	balaverde[0].y +=1;	
	if(balaverde[0].x > 1500 && paddle[1].l==1){
		init_balaverde1();
	} 
}

static void disparoE2(){
	balaverde[1].x -=5;
	balaverde[1].y +=1;	
	if(balaverde[1].x < -500 && paddle[2].l==1){
		init_balaverde2();
	} 
}

static void disparoE3(){
	balaverde[2].x +=5;
	balaverde[2].y -=1;	
	if(balaverde[2].x > 1500 && paddle[3].l==1){
		init_balaverde3();
	} 
}

static void disparoE4(){
	balaverde[3].x -=5;
	balaverde[3].x -=1;	
	if(balaverde[3].x < -500 && paddle[4].l==1){
		init_balaverde4();
	} 
}
//Minas
static void disparoE5(){
	if(paddle[5].l==1 && mina[0].x!=250)
		init_mina1();
	if(paddle[5].x==250)
		mina[0].x=250;	
}

static void disparoE6(){
	if(paddle[6].l==1 && mina[1].x!=350)
		init_mina2();
	if(paddle[6].x==350)
		mina[1].x=350;	 
}

static void disparoE7(){
	if(paddle[7].l==1 && mina[2].x!=450)
		init_mina3();
	if(paddle[7].x==450)
		mina[2].x=450;	
}

static void disparoE8(){
	if(paddle[8].l==1 && mina[3].x!=550)
		init_mina1();
	if(paddle[8].x==550)
		mina[3].x=550;	 
}
//Disparo platillos
static void disparoE9(){
	balaplatillo[0].x +=7;
	balaplatillo[0].y +=2;	
	if(balaplatillo[0].x > 1500 && paddle[9].l==1){
		init_balaplatillo1();
	} 
}

static void disparoE10(){
	balaplatillo[1].x -=7;
	balaplatillo[1].y +=2;	
	if(balaplatillo[1].x < -500 && paddle[10].l==1){
		init_balaplatillo2();
	} 
}

static void disparoE11(){
	balaplatillo[2].x +=7;
	balaplatillo[2].y -=2;	
	if(balaplatillo[2].x > 1500 && paddle[11].l==1){
		init_balaplatillo3();
	} 
}

static void disparoE12(){
	balaplatillo[3].x -=7;
	balaplatillo[3].y -=2;	
	if(balaplatillo[3].x < -500 && paddle[12].l==1){
		init_balaplatillo4();
	} 
}

static void bombas(){
	if (bomba==1 && cantbomba!=0){
		bomba=0;
		init_mina1();
		init_mina2();
		init_mina3();
		init_mina4();
		cantbomba-=1;
		for(int i=1;i<13;i++){
			paddle[i].x=randomNumber(2000,3000);
		}
		int last_time=0;
		int time_now=SDL_GetTicks();
		if(time_now>last_time+1000){
			if(paddle[1].l==1){
				if (paddle[1].x>0 || paddle[1].x<SCREEN_WIDTH){
					last_time=time_now;	
					init_verde1();
					score[1]+=1;
					score[2]+=5;
				}
			}
			if(paddle[2].l==1){
				if (paddle[2].x>0 || paddle[2].x<SCREEN_WIDTH){
					last_time=time_now;
					init_verde2();
					score[1]+=1;
					score[2]+=5;
				}
			}
			if(paddle[3].l==1){
				if (paddle[3].x>0 || paddle[3].x<SCREEN_WIDTH){
					last_time=time_now;
					init_verde3();
					score[1]+=1;
					score[2]+=5;
				}
			}
			if(paddle[4].l==1){
				if (paddle[4].x>0 || paddle[4].x<SCREEN_WIDTH){
					last_time=time_now;	
					init_verde4();
					score[1]+=1;
					score[2]+=5;
				}
			}
			if(paddle[5].l==1){
				if (paddle[5].x>0 || paddle[5].x<SCREEN_WIDTH){	
					last_time=time_now;
					init_multicolor1();
					score[1]+=1;
					score[2]+=5;
				}
			}
			if(paddle[6].l==1){
				if (paddle[6].x>0 || paddle[6].x<SCREEN_WIDTH){	
					last_time=time_now;
					init_multicolor2();
					score[1]+=1;
					score[2]+=5;
				}
			}
			if(paddle[7].l==1){
				if (paddle[7].x>0 || paddle[7].x<SCREEN_WIDTH){
					last_time=time_now;
					init_multicolor3();	
					score[1]+=1;
					score[2]+=5;
				}
			}
			if(paddle[8].l==1){
				if (paddle[8].x>0 || paddle[8].x<SCREEN_WIDTH){	
					last_time=time_now;
					init_multicolor4();
					score[1]+=1;
					score[2]+=5;
				}
			}
			if(paddle[9].l==1){
				if (paddle[9].x>0 || paddle[9].x<SCREEN_WIDTH){
					last_time=time_now;
					init_platillo1();
					score[1]+=1;
					score[2]+=5;
				}
			}
			if(paddle[10].l==1){
				if (paddle[10].x>0 || paddle[10].x<SCREEN_WIDTH){
					last_time=time_now;
					init_platillo2();
					score[1]+=1;
					score[2]+=5;
				}
			}
			if(paddle[11].l==1){
				if (paddle[11].x>0 || paddle[11].x<SCREEN_WIDTH){
					last_time=time_now;	
					init_platillo3();
					score[1]+=1;
					score[2]+=5;
				}
			}
			if(paddle[12].l==1){
				if (paddle[12].x>0 || paddle[12].x<SCREEN_WIDTH){
					last_time=time_now;
					init_platillo4();
					score[1]+=1;
					score[2]+=5;
				}
			}
		}
	}
}

static void move_verde1() {
	paddle[1].x--;
	if(check_collision_enemy(paddle[1],paddle[13])==1){
		paddle[13].x=paddle[1].x+5;
		paddle[13].y=paddle[1].y+20;
		paddle[1].y--;
		paddle[1].x++;
		if(paddle[1].x>1030 || paddle[1].x<-10)
			paddle[13].x=1300;
		if (paddle[1].y<=300){
			paddle[1].y++;
			if(paddle[1].y<=750){
			paddle[1].y+=randomNumber(0,3);
			if (paddle[1].y<=91)
				paddle[1].y+=randomNumber(0,3);
			else
				paddle[1].y-=randomNumber(0,3);
			paddle[1].x+=1;
			}
		}
	}

	if(check_collision_enemy(paddle[1],paddle[13])!=1 && paddle[13].y<725){
		paddle[13].y++;
		if(check_collision_enemy(paddle[0],paddle[13])==1){
			//paddle[13].y--;
			paddle[13].x=paddle[0].x;
			paddle[13].y=paddle[0].y+10;
			if (paddle[0].y>=665){
				score[1]+=5;
				paddle[13].x=1300;
			}
		}
		if(paddle[13].y==725){
			paddle[13].x=1300;
		}
	}
	

	if(paddle[13].x==1300){
		paddle[1].x++;
		paddle[1].y--;
		if (paddle[1].y<=300){
			paddle[1].y++;
			if(paddle[1].y<=750){
				paddle[1].y+=randomNumber(0,3);
				if (paddle[1].y<=91)
					paddle[1].y+=randomNumber(0,3);
				else
					paddle[1].y-=randomNumber(0,3);
				paddle[1].x+=1;
			}
		}
	}
}

static void move_verde2() {
	paddle[2].x--;
	if(check_collision_enemy(paddle[2],paddle[14])==1){
		paddle[14].x=paddle[2].x+5;
		paddle[14].y=paddle[2].y+20;
		paddle[2].y--;
		paddle[2].x++;
		if(paddle[2].x>1030 || paddle[2].x<-10)
			paddle[14].x=1300;
		if (paddle[2].y<=400){
			paddle[2].y++;
			if(paddle[2].y<=750){
			paddle[2].y+=randomNumber(0,3);
			if (paddle[2].y<=91)
				paddle[2].y+=randomNumber(0,3);
			else
				paddle[2].y-=randomNumber(0,3);
			paddle[2].x+=1;
			}
		}
	}
	if(check_collision_enemy(paddle[2],paddle[14])!=1 && paddle[14].y<725){
		paddle[14].y++;
		if(check_collision_enemy(paddle[0],paddle[14])==1){
			paddle[14].x=paddle[0].x;
			paddle[14].y=paddle[0].y+10;
			if (paddle[0].y>=665){
				score[1]+=5;
				paddle[14].x=1300;
			}
		}
		if(paddle[14].y==725){
			paddle[14].x=1300;
		}
	}

	if(paddle[14].x==1300){
		paddle[2].x++;
		paddle[2].y--;
		if (paddle[2].y<=400){
			paddle[2].y++;
			if(paddle[2].y<=750){
				paddle[2].y+=randomNumber(0,3);
				if (paddle[2].y<=91)
					paddle[2].y+=randomNumber(0,3);
				else
					paddle[2].y-=randomNumber(0,3);
				paddle[2].x-=1;
			}
		}
	}
}

static void move_verde3() {
	paddle[3].x--;
	if(check_collision_enemy(paddle[3],paddle[15])==1 && paddle[3].l==1){
		paddle[15].x=paddle[3].x+5;
		paddle[15].y=paddle[3].y+20;
		paddle[3].y--;
		paddle[3].x++;
		if(paddle[3].x>1030 || paddle[3].x<-10)
			paddle[15].x=1300;
		if (paddle[3].y<=350){
			paddle[3].y++;
			if(paddle[3].y<=750){
			paddle[3].y+=randomNumber(0,3);
			if (paddle[3].y<=91)
				paddle[3].y+=randomNumber(0,3);
			else
				paddle[3].y-=randomNumber(0,3);
			paddle[3].x+=1;
			}
		}
	}
	if(check_collision_enemy(paddle[3],paddle[15])!=1 && paddle[15].y<725){
		paddle[15].y++;
		if(check_collision_enemy(paddle[0],paddle[15])==1){
			paddle[15].x=paddle[0].x;
			paddle[15].y=paddle[0].y+10;
			if (paddle[0].y>=665){
				score[1]+=5;
				paddle[15].x=1300;
			}
		}
		if(paddle[15].y==725){
			paddle[15].x=1300;
		}
	}

	if(paddle[15].x==1300){
		paddle[3].x++;
		paddle[3].y--;
		if (paddle[3].y<=350){
			paddle[3].y++;
			if(paddle[3].y<=750){
				paddle[3].y+=randomNumber(0,3);
				if (paddle[3].y<=91)
					paddle[3].y+=randomNumber(0,3);
				else
					paddle[3].y-=randomNumber(0,3);
				paddle[3].x+=1;
			}
		}
	}
}

static void move_verde4() {
	paddle[4].x--;
	if(check_collision_enemy(paddle[4],paddle[16])==1 && paddle[4].l==1){
		paddle[16].x=paddle[4].x+5;
		paddle[16].y=paddle[4].y+20;
		paddle[4].y--;
		paddle[4].x++;
		if(paddle[4].x>1030 || paddle[4].x<-10)
			paddle[16].x=1300;
		if (paddle[4].y<=450){
			paddle[4].y++;
			if(paddle[4].y<=750){
			paddle[4].y+=randomNumber(0,3);
			if (paddle[4].y<=91)
				paddle[4].y+=randomNumber(0,3);
			else
				paddle[4].y-=randomNumber(0,3);
			paddle[4].x+=1;
			}
		}
	}
	if(check_collision_enemy(paddle[4],paddle[16])!=1 && paddle[16].y<725){
		paddle[16].y++;
		if(check_collision_enemy(paddle[0],paddle[16])==1){
			paddle[16].x=paddle[0].x;
			paddle[16].y=paddle[0].y+10;
			if (paddle[0].y>=665){
				score[1]+=5;
				paddle[16].x=1300;
			}
		}
		if(paddle[16].y==725){
			paddle[16].x=1300;
		}
	}

	if(paddle[16].x==1300){
		paddle[4].x++;
		paddle[4].y--;
		if (paddle[4].y<=450){
			paddle[4].y++;
			if(paddle[4].y<=750){
				paddle[4].y+=randomNumber(0,3);
				if (paddle[4].y<=91)
					paddle[4].y+=randomNumber(0,3);
				else
					paddle[4].y-=randomNumber(0,3);
				paddle[4].x-=1;
			}
		}
	}
}

static void move_multicolor() {
	for(int i=5;i<9;i+=2){
		if(paddle[i].y<=750){
			paddle[i].y+=randomNumber(0,3);
			if (paddle[i].y<=91)
				paddle[i].y+=randomNumber(0,3);
			else
				paddle[i].y-=randomNumber(0,3);
			paddle[i].x+=1;
		}
		if(paddle[i+1].y<=750){
			paddle[i+1].y+=randomNumber(0,3);
			if (paddle[i+1].y<=91)
				paddle[i+1].y+=randomNumber(0,3);
			else
				paddle[i+1].y-=randomNumber(0,3);
		paddle[i+1].x-=1;
		}
	}
}

static void ubicar_enemigos(){

	if (z==3)
		half_map+=5;
	else if (z==2)
		half_map-=5;
	k=randomNumber(0,120);
	for(int i = 1; i<5;i++){
		if (k==10){
			if (paddle[i].x>SCREEN_WIDTH || paddle[i].x<0){
				paddle[i].x=randomNumber(100,800);
				paddle[i].y=725;
			}
		}
	}	
	for(int i = 5; i<13;i++){
		if (k==10){
			if (paddle[i].x>SCREEN_WIDTH){
				paddle[i].x=randomNumber(100,200);
				paddle[i].y=randomNumber(100,500);
			}
		}
		else if(k==20){			
			if (paddle[i].x<0){
				paddle[i].x=randomNumber(700,800);
				paddle[i].y=randomNumber(100,500);
			}
		}
	}
}

static void move_platillo(){
	int i;	
	int c = rand()%5;
	for (i = 9; i < 13 ; i++) { // buscar en coord x
		if(paddle[i].x < paddle[0].x ){ // paddle[1] es el jugador
			paddle[i].x +=1;
			
		}
		else{
			paddle[i].x -=1;
		}
	}
	
	for ( i =9; i<13; i++){	// buscar en coord y
		if(paddle[i].y < paddle[0].y && i ==2 ){
			paddle[i].y +=1;		
		}
		
		if(paddle[i].y > paddle[0].y && i ==2){
			paddle[i].y -=1;		
		}
		
		if(paddle[i].y < paddle[0].y && i != 2 && c==3 ){
			paddle[i].y += 3;
		}	
			
		if(paddle[i].y > paddle[0].y && i != 2 && c==3 ){
			paddle[i].y -= 3;
		}
	}
}

static void move_paddle(int d) {

	// if the down arrow is pressed move paddle down
	if (d == 0) {
		
		if(paddle[0].y >= screen->h - paddle[0].h) {
		
			paddle[0].y = screen->h - paddle[0].h;
		
		} else { 
		
			paddle[0].y += 5;
		}
	}
	
	// if the up arrow is pressed move paddle up
	if (d == 1) {

		if(paddle[0].y <= 100) {
		
			paddle[0].y = 100;

		} else {
		
			paddle[0].y -= 5;
		}
	}

// MOVIMIENTO DERECHA E IZQUIERDA
	// if the left arrow is pressed move paddle LEFT
	if (d == 2) {
		dirdisp = 0;
		z=2;
		if(paddle[0].x <= 200) {
		
			paddle[0].x = 200;

		} else {
		
			paddle[0].x -= 5;
		}
	}


    // if the right arrow is pressed move paddle RIGHT
	if (d == 3) {
		dirdisp = 1;
		z=3;
		if(paddle[0].x >= screen->w - 200) {
		
			paddle[0].x = screen->w - 200;
		
		} else { 
		
			paddle[0].x += 5;
		}
		
	}
}

static void crear_disparo(int contador){
		
	active[contador] = 1;
	
	disparo[contador].x = paddle[0].x-10 ;
	disparo[contador].y = paddle[0].y+20 ;
	
	disparo[contador].dx = dirdisp;		
	
}

static void mover_disparo(){
	for(int i=0; i<200; i++){
		if(active[i]==1){
		
			if(disparo[i].dx == 0){
				disparo[i].x -= 10;
			}
			
			if(disparo[i].dx == 1){
				disparo[i].x += 10;
			}
			
			if(disparo[i].x <-100 || disparo[i].x > 1100 ){
				active[i] =0;
			}
		}
	}
}

static void draw_game_over() { 

	SDL_Rect dest;
	dest.x = 0;
	dest.y = 100;
	dest.w = 0;
	dest.h = 0;		
	SDL_BlitSurface(end, NULL, screen, &dest);	
}

static void draw_menu1() {

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = pointillist->w;
	src.h = pointillist->h;

	dest.x = (screen->w / 2) - (src.w / 2);
	dest.y = (screen->h / 2) - (src.h / 2);
	dest.w = pointillist->w;
	dest.h = pointillist->h;

	SDL_BlitSurface(pointillist, &src, screen, &dest);
}
static void draw_menu() {

	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = title->w;
	src.h = title->h;

	dest.x = (screen->w / 3) - (src.w / 3);
	dest.y = (screen->h / 3) - (src.h / 3);
	dest.w = title->w;
	dest.h = title->h;

	SDL_BlitSurface(title, &src, screen, &dest);
}
static void draw_background() {
 
	SDL_Rect src;
	
	//draw bg with net
	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h;
}

static void draw_ballVerde() {
	
	SDL_Rect src;
	for(int i=0;i<4;i++){
	src.x = balaverde[i].x;
	src.y = balaverde[i].y;
	src.w = balaverde[i].w;
	src.h = balaverde[i].h;
		if(i==0){
			SDL_BlitSurface(balaV1, NULL, screen, &src);
			}
		if(i==1){
			SDL_BlitSurface(balaV2, NULL, screen, &src);
			}
		if(i==2){
			SDL_BlitSurface(balaV3, NULL, screen, &src);
			}
		if(i==3){
			SDL_BlitSurface(balaV4, NULL, screen, &src);
			}
	}	
}

static void draw_mina() {
	
SDL_Rect src;
	for(int i=0;i<4;i++){
	src.x = mina[i].x;
	src.y = mina[i].y;
	src.w = mina[i].w;
	src.h = mina[i].h;
		if(i==0){
			SDL_BlitSurface(mina1, NULL, screen, &src);
			}
		else if(i==1){
			SDL_BlitSurface(mina2, NULL, screen, &src);
			}
		else if(i==2){
			SDL_BlitSurface(mina3, NULL, screen, &src);
			}
		else if(i==3){
			SDL_BlitSurface(mina4, NULL, screen, &src);
			}
	}
}

static void draw_ballPlatillo() {
	
SDL_Rect src;
	for(int i=0;i<4;i++){
	src.x = balaplatillo[i].x;
	src.y = balaplatillo[i].y;
	src.w = balaplatillo[i].w;
	src.h = balaplatillo[i].h;
		if(i==0){
			SDL_BlitSurface(balaP1, NULL, screen, &src);
			}
		else if(i==1){
			SDL_BlitSurface(balaP2, NULL, screen, &src);
			}
		else if(i==2){
			SDL_BlitSurface(balaP3, NULL, screen, &src);
			}
		else if(i==3){
			SDL_BlitSurface(balaP4, NULL, screen, &src);
			}
	}
}

static void draw_paddle1() {

	SDL_Rect src;
	int i;

	SDL_Rect scale1;
	SDL_Rect scale2;
	SDL_Rect scale3;
	SDL_Rect scale4;
	SDL_Rect scale5;

	for (i = 0; i < 30; i++) {
	//JUGADOR
		scale1.w=80;
		scale1.h=30;
		scale1.x=paddle[i].x;
		scale1.y=paddle[i].y;
	//MULTICOLOR				
		scale2.w=20;
		scale2.h=20;
		scale2.x=paddle[i].x;
		scale2.y=paddle[i].y;
	//VERDE 						
		scale3.w=30;
		scale3.h=30;
		scale3.x=paddle[i].x;
		scale3.y=paddle[i].y;
	//PLATILLO//							
		scale4.w=40;
		scale4.h=20;
		scale4.x=paddle[i].x;
		scale4.y=paddle[i].y;
	//human1//							
		scale5.w=10;
		scale5.h=30;
		scale5.x=paddle[i].x;
		scale5.y=paddle[i].y;	
	
		if(i==0){
			if(z==2){
			SDL_BlitScaled(jugadorL,NULL,screen,&scale1);
			}
			else if(z==3){
			SDL_BlitScaled(jugadorR,NULL,screen,&scale1);
			}
		}
		if(i==1){
			SDL_BlitScaled(verde1,NULL,screen,&scale2);
		}
		if(i==2){
			SDL_BlitScaled(verde2,NULL,screen,&scale2);
		}
		if(i==5){
			SDL_BlitScaled(multicolor1,NULL,screen,&scale3);
		}
		if(i==6){
			SDL_BlitScaled(multicolor2,NULL,screen,&scale3);
		}
		if(i==9){
			SDL_BlitScaled(platillo1,NULL,screen,&scale4);
		}
		if(i==10){
			SDL_BlitScaled(platillo2,NULL,screen,&scale4);
		}
		if(i==13){
			SDL_BlitScaled(human1,NULL,screen,&scale5);
		}
		if(i==14){
			SDL_BlitScaled(human2,NULL,screen,&scale5);
		}
		if(i==15){
			SDL_BlitScaled(human3,NULL,screen,&scale5);
		}
		if(i==16){
			SDL_BlitScaled(human4,NULL,screen,&scale5);
		}
	}
}

static void draw_paddle2() {

	SDL_Rect src;
	int i;

	SDL_Rect scale1;
	SDL_Rect scale2;
	SDL_Rect scale3;
	SDL_Rect scale4;
	SDL_Rect scale5;
	for (i = 0; i < 30; i++) {
		//JUGADOR
		scale1.w=80;
		scale1.h=30;
		scale1.x=paddle[i].x;
		scale1.y=paddle[i].y;
		//MULTICOLOR				
		scale2.w=20;
		scale2.h=20;
		scale2.x=paddle[i].x;
		scale2.y=paddle[i].y;
		//VERDE 						
		scale3.w=30;
		scale3.h=30;
		scale3.x=paddle[i].x;
		scale3.y=paddle[i].y;
		//PLATILLO//							
		scale4.w=40;
		scale4.h=20;
		scale4.x=paddle[i].x;
		scale4.y=paddle[i].y;
		//human//							
		scale5.w=10;
		scale5.h=30;
		scale5.x=paddle[i].x;
		scale5.y=paddle[i].y;	

		if(i==0){
			if(z==2){
				SDL_BlitScaled(jugadorL,NULL,screen,&scale1);
			}
			else if(z==3){
				SDL_BlitScaled(jugadorR,NULL,screen,&scale1);
			}
		}
		if(i==1){
			SDL_BlitScaled(verde1,NULL,screen,&scale2);
		}
		if(i==2){
			SDL_BlitScaled(verde2,NULL,screen,&scale2);
		}
		if(i==3){
			SDL_BlitScaled(verde3,NULL,screen,&scale2);
		}
		if(i==5){
			SDL_BlitScaled(multicolor1,NULL,screen,&scale3);
		}
		if(i==6){
			SDL_BlitScaled(multicolor2,NULL,screen,&scale3);
		}
		if(i==7){
			SDL_BlitScaled(multicolor3,NULL,screen,&scale3);
		}
		if(i==9){
			SDL_BlitScaled(platillo1,NULL,screen,&scale4);
		}
		if(i==10){
			SDL_BlitScaled(platillo2,NULL,screen,&scale4);
		}
		if(i==11){
			SDL_BlitScaled(platillo3,NULL,screen,&scale4);
		}
		if(i==13){
			SDL_BlitScaled(human1,NULL,screen,&scale5);
		}
		if(i==14){
			SDL_BlitScaled(human2,NULL,screen,&scale5);
		}
		if(i==15){
			SDL_BlitScaled(human3,NULL,screen,&scale5);
		}
		if(i==16){
			SDL_BlitScaled(human4,NULL,screen,&scale5);
		}
	}
}
static void draw_paddle3() {

	SDL_Rect src;
	int i;

	SDL_Rect scale1;
	SDL_Rect scale2;
	SDL_Rect scale3;
	SDL_Rect scale4;
	SDL_Rect scale5;
	for (i = 0; i < 30; i++) {
	//JUGADOR
		scale1.w=80;
		scale1.h=30;
		scale1.x=paddle[i].x;
		scale1.y=paddle[i].y;
	//MULTICOLOR				
		scale2.w=20;
		scale2.h=20;
		scale2.x=paddle[i].x;
		scale2.y=paddle[i].y;
	//VERDE 						
		scale3.w=30;
		scale3.h=30;
		scale3.x=paddle[i].x;
		scale3.y=paddle[i].y;
	//PLATILLO//							
		scale4.w=40;
		scale4.h=20;
		scale4.x=paddle[i].x;
		scale4.y=paddle[i].y;
	//human1//							
		scale5.w=10;
		scale5.h=30;
		scale5.x=paddle[i].x;
		scale5.y=paddle[i].y;		
		if(i==0){
			if(z==2){
			SDL_BlitScaled(jugadorL,NULL,screen,&scale1);
			}
			else if(z==3){
			SDL_BlitScaled(jugadorR,NULL,screen,&scale1);
			}
		}
		if(i==1){
			SDL_BlitScaled(verde1,NULL,screen,&scale2);
		}
		if(i==2){
			SDL_BlitScaled(verde2,NULL,screen,&scale2);
		}
		if(i==3){
			SDL_BlitScaled(verde3,NULL,screen,&scale2);
		}
		if(i==4){
			SDL_BlitScaled(verde4,NULL,screen,&scale2);
		}
		if(i==5){
			SDL_BlitScaled(multicolor1,NULL,screen,&scale3);
		}
		if(i==6){
			SDL_BlitScaled(multicolor2,NULL,screen,&scale3);
		}
		if(i==7){
			SDL_BlitScaled(multicolor3,NULL,screen,&scale3);
		}
		if(i==8){
			SDL_BlitScaled(multicolor4,NULL,screen,&scale3);
		}
		if(i==9){
			SDL_BlitScaled(platillo1,NULL,screen,&scale4);
		}
		if(i==10){
			SDL_BlitScaled(platillo2,NULL,screen,&scale4);
		}
		if(i==11){
			SDL_BlitScaled(platillo3,NULL,screen,&scale4);
		}
		if(i==12){
			SDL_BlitScaled(platillo4,NULL,screen,&scale4);
		}
		if(i==13){
			SDL_BlitScaled(human1,NULL,screen,&scale5);
		}
		if(i==14){
			SDL_BlitScaled(human2,NULL,screen,&scale5);
		}
		if(i==15){
			SDL_BlitScaled(human3,NULL,screen,&scale5);
		}
		if(i==16){
			SDL_BlitScaled(human4,NULL,screen,&scale5);
		}

	}
}	
static void nivel(){
	if(score[0]<3){
		draw_paddle1();
		paddle[0].l=1;		
		paddle[1].l=1;
		paddle[2].l=1;
		paddle[5].l=1;
		paddle[6].l=1;
		paddle[9].l=1;
		paddle[10].l=1;
	}
	if(score[0]>=3 && score[0]<6 && score[1]>=0){
		draw_paddle2();
		paddle[0].l=1;		
		paddle[1].l=1;
		paddle[2].l=1;
		paddle[3].l=1;
		paddle[5].l=1;
		paddle[6].l=1;
		paddle[7].l=1;
		paddle[9].l=1;
		paddle[10].l=1;
		paddle[11].l=1;
		paddle[12].l=0;
	}
	if(score[0]>=6 && score[0]<10 && score[1]>=0){
		draw_paddle3();
		paddle[0].l=1;		
		paddle[1].l=1;
		paddle[2].l=1;
		paddle[3].l=1;
		paddle[4].l=1;
		paddle[5].l=1;
		paddle[6].l=1;
		paddle[7].l=1;
		paddle[8].l=1;
		paddle[9].l=1;
		paddle[10].l=1;
		paddle[11].l=1;
		paddle[12].l=1;	
	}
}

static void draw_disparo(){
	SDL_Rect src;
	for(int i=0; i<200 ; i++){
		src.x = disparo[i].x;
		src.y = disparo[i].y;
		src.w = disparo[i].w;
		src.h = disparo[i].h;
		if(dirdisp==0){
			SDL_BlitSurface(balaL, NULL, screen, &src);
		
		}
		else if(dirdisp==1){
			SDL_BlitSurface(balaR, NULL, screen, &src);
		}
	
	}
}

static void draw_lives() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 35;
	src.h = 15;
	if(vidas == 3){
		dest.x = 100;
		dest.y = 50-src.h;
		dest.w = 35;
		dest.h = 15;
		
		SDL_BlitSurface(lives, NULL, screen, &dest);		
		dest.x 	= dest.x - src.w;
		SDL_BlitSurface(lives, NULL, screen, &dest);		
		dest.x 	= dest.x - src.w;
		SDL_BlitSurface(lives, NULL, screen, &dest);		
	}	

	if(vidas == 2){
		dest.x = 100;
		dest.y = 50-src.h;
		dest.w = 35;
		dest.h = 15;
		SDL_BlitSurface(lives, NULL, screen, &dest);		
		dest.x 	= dest.x - src.w;
		SDL_BlitSurface(lives, NULL, screen, &dest);		
	}
	if(vidas == 1){
		dest.x = 100;
		dest.y = 50-src.h;
		dest.w = 35;
		dest.h = 15;
		SDL_BlitSurface(lives, &src, screen, &dest);
	}
}

static void draw_bombas() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 35;
	src.h = 15;
	
	if(cantbomba == 3){
		dest.x = 200;
		dest.y = 50-src.h;
		dest.w = 35;
		dest.h = 15;
		SDL_BlitSurface(bomb, NULL, screen, &dest);		
		dest.y 	= dest.y - src.h;
		SDL_BlitSurface(bomb, NULL, screen, &dest);
		dest.y 	= dest.y - src.h;
		SDL_BlitSurface(bomb, NULL, screen, &dest);			
	}	

	if(cantbomba == 2){
		dest.x = 200;
		dest.y = 50-src.h;
		dest.w = 35;
		dest.h = 15;
		SDL_BlitSurface(bomb, NULL, screen, &dest);		
		dest.y 	= dest.y - src.h;
		SDL_BlitSurface(bomb, NULL, screen, &dest);
			
	}
	if(cantbomba == 1){
		dest.x = 200;
		dest.y = 50-src.h;
		dest.w = 35;
		dest.h = 15;
		dest.y 	= dest.y -src.h;
		SDL_BlitSurface(bomb, &src, screen, &dest);	
	}
}

static void draw_mil_score() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 25;
	src.h = 25;

	dest.x = 25; //12 is just padding spacing
	dest.y = 60;
	dest.w = 25;
	dest.h = 25;

	if (score[0] >= 0 && score[0] < 10) {
		
		src.x = src.w * score[0];
	}
	
	SDL_BlitSurface(numbermap, &src, screen, &dest);
}
static void draw_cent_score() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 25;
	src.h = 25;

	dest.x = 50; //12 is just padding spacing
	dest.y = 60;
	dest.w = 25;
	dest.h = 25;

	if (score[1] >= 0 && score[1] < 10) {
		
		src.x = src.w * score[1];
	}
	
	SDL_BlitSurface(numbermap, &src, screen, &dest);
}
static void draw_dec_score() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 25;
	src.h = 25;

	dest.x = 75;
	dest.y = 60;
	dest.w = 25;
	dest.h = 25;
	
	if (score[2] >= 0 && score[2] < 10) {
		
		src.x = src.w * score[2];
	}

	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

static void draw_unid_score() {
	
	SDL_Rect src;
	SDL_Rect dest;

	src.x = 0;
	src.y = 0;
	src.w = 25;
	src.h = 25;

	dest.x = 100;
	dest.y = 60;
	dest.w = 25;
	dest.h = 25;
	
	if (score[3] >= 0 && score[3] < 10) {
		
		src.x = src.w * score[3];
	}

	SDL_BlitSurface(numbermap, &src, screen, &dest);
}

int contador=0;

int main (int argc, char *args[]) {
	for(int g=0; g<200; g++ ){
		active[g] = 0;	
	}
	//SDL Window setup
	if (init(SCREEN_WIDTH, SCREEN_HEIGHT, argc, args) == 1) {
		
		return 0;
	}
	
	SDL_GetWindowSize(window, &width, &height);
	
	int sleep = 0;
	int quit = 0;
	int state = 5;
	Uint32 next_game_tick = SDL_GetTicks();
	
	// Initialize the ball player and ai position data. 
	init_verde1();
	init_verde2();
	init_verde3();
	init_verde4();

	init_multicolor1();
	init_multicolor2();
	init_multicolor3();
	init_multicolor4();

	init_platillo1();
	init_platillo2();
	init_platillo3();
	init_platillo4();

	init_jugador();
	init_disparo();

	init_human1();
	init_human2();
	init_human3();
	init_human4();

	uint8_t c_anterior;
	uint8_t x_anterior;
	//render loop
	while(quit == 0) {
	
		//check for new events every frame
		SDL_PumpEvents();
        //draw background
		SDL_RenderClear(renderer);
		SDL_FillRect(screen, NULL, 0x000000ff);
		const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		
		if (keystate[SDL_SCANCODE_ESCAPE]) {
		
			quit = 1;
		}
		
		if (keystate[SDL_SCANCODE_DOWN]) {
			
			move_paddle(0);
		}

		if (keystate[SDL_SCANCODE_UP]) {
			
			move_paddle(1);
		}

		if (keystate[SDL_SCANCODE_LEFT]){
			move_paddle(2);
			half_map-=5;
		}
		if (keystate[SDL_SCANCODE_RIGHT]){
			move_paddle(3);
			half_map+=5;
		}
		int puntero=0;
		for(puntero=0; puntero<200;puntero++){
			if(active[puntero]==0){
				break;
			}	
		}
		if(active[puntero] == 0 ){

			if (keystate[SDL_SCANCODE_C ]!=c_anterior && keystate[SDL_SCANCODE_C]){
				crear_disparo(puntero);
			}
			c_anterior=keystate[SDL_SCANCODE_C ];
		}	

		if (keystate[SDL_SCANCODE_X]!=x_anterior && keystate[SDL_SCANCODE_X]==1){
			bomba=1;
		}
		x_anterior=keystate[SDL_SCANCODE_X];
		draw_framework();

		SDL_UpdateTexture(screen_texture, NULL, screen->pixels, screen->w * 4);
		SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
		
		//display main menu
		if (state==5){

			draw_menu();			
			state=8;
		}
		else if (state==8){
			
			SDL_Delay(1000);
			state=0;
		}
		else if (state == 0 ) {
			if (keystate[SDL_SCANCODE_SPACE]) {
				state = 1;
			}
			draw_menu1();
		//display gameover
		} else if (state == 2) {
			draw_game_over();
		}	
		else if (state == 3){				
			SDL_Rect src;
			src.x = 0;
			src.y = 95;
			src.w = 35;
			src.h = 15;
  			SDL_BlitSurface(win, NULL, screen, &src);	
			int last_time=0;
			int time_now=SDL_GetTicks();	
			if(time_now>last_time+100000){			
				last_time=time_now;				
				quit=1;
			}	
		}
		//display the game
		else if (state == 1) {
			
			//check score
			if(vidas==0){
				state = 2;	
			}
			else if(score[0]==9 && score[1]>=0 &&score[2]>=0 && score[3]>=0){	
				state = 3;
			}
			collision();
			draw_lives();
			bombas();			
			draw_bombas();
			draw_disparo();
			//paddle ai movement
			move_verde1();
			move_verde2();	
			move_verde3();
			move_verde4();			
			move_multicolor();
			ubicar_enemigos();
			move_platillo();
			//* Move the balls for the next frame. 
			for(int i=0; i<200; i++){			
				if( active[i] ==1 ){
					mover_disparo();
				}
			}
			disparoE1();
			disparoE2();
			disparoE3();
			disparoE4();
			disparoE5();
			disparoE6();
			disparoE7();
			disparoE8();
			disparoE9();
			disparoE10();
			disparoE11();
			disparoE12();
			nivel();
			//* Put the ball on the screen.
			draw_ballVerde();
			draw_mina();
			draw_ballPlatillo();
			//draw lives
			draw_lives();
			//draw the score			
			draw_mil_score();
			draw_cent_score();
			draw_dec_score();
			draw_unid_score();
		}
		SDL_UpdateTexture(screen_texture, NULL, screen->pixels, screen->w * sizeof (Uint32));
		SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
		//draw to the display
		SDL_RenderPresent(renderer);	
		//time it takes to render  frame in milliseconds
		next_game_tick += 1000 / 60;
		sleep = next_game_tick - SDL_GetTicks();
	
		if( sleep >= 0 ) {
            				
			SDL_Delay(sleep);
		}
	}

	//free loaded images
	SDL_FreeSurface(screen);
	SDL_FreeSurface(pointillist);
	SDL_FreeSurface(numbermap);
	SDL_FreeSurface(end);
	SDL_FreeSurface(title);
	SDL_FreeSurface(lives);
	SDL_FreeSurface(verde1);
	SDL_FreeSurface(verde2);
	SDL_FreeSurface(verde3);
	SDL_FreeSurface(verde4);
	SDL_FreeSurface(multicolor1);
	SDL_FreeSurface(multicolor2);
	SDL_FreeSurface(multicolor3);
	SDL_FreeSurface(multicolor4);
	SDL_FreeSurface(platillo1);
	SDL_FreeSurface(platillo2);
	SDL_FreeSurface(platillo3);
	SDL_FreeSurface(platillo4);
	SDL_FreeSurface(jugadorL);
	SDL_FreeSurface(jugadorR);
	SDL_FreeSurface(balaR);
	SDL_FreeSurface(balaL);
	SDL_FreeSurface(human1);
	SDL_FreeSurface(human2);
	SDL_FreeSurface(human3);
	SDL_FreeSurface(human4);
	SDL_FreeSurface(win);
	SDL_FreeSurface(balaV1);
	SDL_FreeSurface(balaV2);
	SDL_FreeSurface(balaV3);
	SDL_FreeSurface(balaV4);
	SDL_FreeSurface(balaP1);
	SDL_FreeSurface(balaP2);
	SDL_FreeSurface(balaP3);
	SDL_FreeSurface(balaP4);
	SDL_FreeSurface(bomb);



	//free renderer and all textures used with it
	SDL_DestroyRenderer(renderer);
	
	//Destroy window 
	SDL_DestroyWindow(window);

	//Quit SDL subsystems 
	SDL_Quit(); 
	 
	return 0;

}
int init(int width, int height, int argc, char *args[]) {
	map();
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	} 
	
	int i;
	
	for (i = 0; i < argc; i++) {
		
		//Create window	
		if(strcmp(args[i], "-f")) {
			
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
		
		} else {
		
			SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
		}
	}

	if (window == NULL) { 
		
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	}

	//create the screen surface where all the elemnts will be drawn onto (ball, paddles, net etc)
	screen = SDL_CreateRGBSurfaceWithFormat(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
	
	if (screen == NULL) {
		
		printf("Could not create the screen surfce! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	//create the screen texture to render the screen surface to the actual display
	screen_texture = SDL_CreateTextureFromSurface(renderer, screen);

	if (screen_texture == NULL) {
		
		printf("Could not create the screen_texture! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	//Load the title image
	pointillist = SDL_LoadBMP("pointillist.bmp");

	if (pointillist == NULL) {
		
		printf("Could not Load pointillist image! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	title = SDL_LoadBMP("title.bmp");

	if (title == NULL) {
		
		printf("Could not Load title image! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}	
	//Load the numbermap image
	numbermap = SDL_LoadBMP("numbermap.bmp");

	if (numbermap == NULL) {
		
		printf("Could not Load numbermap image! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}
	
	//Load the gameover image
	end = SDL_LoadBMP("gameover.bmp");

	if (end == NULL) {
		
		printf("Could not Load pointillist image! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}
	//Load the lives image
	lives = SDL_LoadBMP("lives.bmp");

	if (lives == NULL) {
		
		printf("Could not Load lives image! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	win = SDL_LoadBMP("win.bmp");

	if (win == NULL) {
		
		printf("Could not Load win image! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}


	bomb = SDL_LoadBMP("bomba.bmp");

	if (bomb == NULL) {
		
		printf("Could not Load bomba image! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}



	// Set the title colourkey. 
	Uint32 colorkey = SDL_MapRGB(title->format, 255, 0, 255);
	SDL_SetColorKey(numbermap, SDL_TRUE, colorkey);
	SDL_SetColorKey(title, SDL_TRUE, colorkey);
	return 0;
}
