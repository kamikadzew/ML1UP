#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>
#include <string>
#include "objects.h"

using namespace std;

//GLOBALS
const int fps=60;
const int width=854;
const int height=480;
enum KEYS{UP,DOWN,LEFT,RIGHT,FIRE};
bool keys[5] = {false,false,false,false,false};

//PROTOTYPES
void InitTS(PC &TS);
void DrawTS(PC &TS);
void MoveTS(PC &TS);
PCB* Fire(PCB *TSB, PC &TS);
void InitTSB(PCB *TSB,PC &TS);
void DrawTSB(PCB *TSB);
PCB* MoveTSB(PCB *TSB);
void InitNM(NPC &NM);
void DrawNM(NPC &NM);
void MoveNM(NPC &NM);
void InitNMB(NPCB *NMB,NPC &NM);
void DrawNMB(NPCB *NMB);
NPCB* MoveNMB(NPCB *NMB);
NPCB* FireN(NPCB *NMB, NPC &NM);
NPCB* ColideTS(PC &TS, NPCB *NMB);
PCB* ColideNM(NPC &NM, PCB *TSB, PC &TS);
void GUI(NPC NM, PC TS,ALLEGRO_FONT *font24);
void Win(PC TS, NPC NM, ALLEGRO_FONT *font36);
void Loser(PC TS, NPC NM, ALLEGRO_FONT *font36);


int main (void){
	srand(time(NULL));
	//ALLEGRO VIRABLES
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer =NULL;

	//ALLEGRO CHECK
	if(!al_init())
	{
		al_show_native_message_box(NULL,NULL,NULL,"FAIL TO INIT ALLEGRO",NULL,NULL);
		return -1;
	}
	
	//VARIABLES
	bool done = false;
	bool redraw = false;
	
	//OBJECTS
	struct PC TS;
	struct PCB *TSB=NULL;
	struct NPC NM;
	struct NPCB *NMB=NULL;
	
	//DISPLAY INIT
	display = al_create_display(width,height);
	
	//DISPLAY CHECK
	if(!display)
	{
		al_show_native_message_box(NULL,NULL,NULL,"FAIL TO INIT DISPLAY",NULL,NULL);
		return -1;
	}
	
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0/fps);
	
	ALLEGRO_FONT *font24=al_load_font("COMIC.TTF",24,0);
	ALLEGRO_FONT *font36=al_load_font("COMIC.TTF",36,0);
	
	//INITS
	InitTS(TS);
	InitNM(NM);
	
	//EVENTS CATCHER
	al_register_event_source(event_queue,al_get_keyboard_event_source());
	al_register_event_source(event_queue,al_get_display_event_source(display));
	al_register_event_source(event_queue,al_get_timer_event_source(timer));
	
	//GAME LOOP
	al_start_timer(timer);
	while(!done){
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue,&ev);
		
		//KEY DOWN
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
			switch(ev.keyboard.keycode){
			case ALLEGRO_KEY_UP:
				keys[UP]=true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN]=true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT]=true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT]=true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[FIRE]=true;
				break;
			}
		}
		
		//KEY UP
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode){
			case ALLEGRO_KEY_UP:
				keys[UP]=false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN]=false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT]=false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT]=false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[FIRE]=false;
				break;
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			}
		}
		
		//THE RED "X"
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			done=true;
		}
		
		//TIMER & MOVMENT
		else if(ev.type==ALLEGRO_EVENT_TIMER){
			MoveTS(TS);
			MoveNM(NM);
			TSB=Fire(TSB,TS);
			TSB=MoveTSB(TSB);
			NMB=FireN(NMB,NM);
			NMB=MoveNMB(NMB);
			NMB=ColideTS(TS,NMB);
			TSB=ColideNM(NM,TSB,TS);
			redraw=true;
		}
		//DRAWING
		if(redraw && al_is_event_queue_empty(event_queue)){
			redraw=false;

			DrawTS(TS);
			DrawNM(NM);
			DrawTSB(TSB);
			DrawNMB(NMB);
			GUI(NM,TS,font24);
			if(NM.lives<=0){
				Win(TS,NM,font36);
			}
			else if(TS.lives<=0){
				Loser(TS,NM,font36);
			}
			else{
				TS.score=TS.score-1;
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}
	
	al_destroy_display(display);
	return 0;
}

//********************************************************************************************************************************************************************************************
//********************************************************************************************************************************************************************************************

//TWILIGHT SPARKLE
void InitTS(PC &TS){
	TS.x=35;
	TS.y=(height/2);
	TS.ID=PLAYER;
	TS.lives=3;
	TS.speed=7;
	TS.boundx=(60/2);
	TS.boundy=(20/2);
	TS.score=(fps*60*2);
	TS.hits=0;
}

void DrawTS(PC &TS){
	al_draw_filled_rectangle(TS.x-30, TS.y-10, TS.x+30, TS.y+10, al_map_rgb(205, 50, 255));
}

void MoveTS(PC &TS){
	TS.y-=keys[UP]*TS.speed;
	TS.y+=keys[DOWN]*TS.speed;
	TS.x-=keys[LEFT]*TS.speed;
	TS.x+=keys[RIGHT]*TS.speed;
	if(TS.y<10)
		TS.y=10;
	if(TS.y>height-10)
		TS.y=height-10;
	if(TS.x<30)
		TS.x=30;
	if(TS.x>width/2-30)
		TS.x=width/2-30;
}

void InitTSB(PCB *TSB, PC &TS){
	TSB->bound=2;
	TSB->ID=BULLETP;
	TSB->speed=10;
	TSB->x=TS.x;
	TSB->y=TS.y;
	TSB->next=NULL;
}

PCB* Fire(PCB *TSB, PC &TS){
	if (keys[FIRE]){
		keys[FIRE]=0;
		if (TSB==NULL){
			TSB=(PCB*)malloc(sizeof(PCB));
			InitTSB(TSB, TS);
			return TSB;
		}
		else{
			PCB *TMP;
			TMP=TSB;
			while(TSB->next!=NULL){
			TSB=TSB->next;
			}
			TSB->next=(PCB*)malloc(sizeof(PCB));
			InitTSB(TSB->next, TS);
			return TMP;
		}
		
	}
	return TSB;
}

void DrawTSB(PCB *TSB){
	while(TSB!=NULL){
		al_draw_filled_circle(TSB->x,TSB->y,10,al_map_rgb(255,255,255));
		TSB=TSB->next;
	}
}

PCB* MoveTSB(PCB *TSB){
	PCB *last;
	PCB *TMP;
	TMP=TSB;
	last=NULL;
	while(TSB!=NULL){
		TSB->x+=TSB->speed;
		if(TSB->x>width){
				TMP=TSB;
				TSB=TSB->next;
				free(TMP);
				TMP=TSB;
		}
		last=TSB;
		if (TSB!=NULL){
			TSB=TSB->next;
		}
	}
	return TMP;
}

//NIGHTMARE MOON
void InitNM(NPC &NM){
	NM.x=width-35;
	NM.y=height/2-20;
	NM.ID=NONPLAYER;
	NM.lives=40;
	NM.speed=2;
	NM.boundx=(50/2);
	NM.boundy=(40/2);
	NM.up=true;
}

void DrawNM(NPC &NM){
	al_draw_filled_rectangle(NM.x+25, NM.y-20, NM.x-25, NM.y+20, al_map_rgb(0, 0, 128));
}

void MoveNM(NPC &NM){
	if(rand()%20==1){
		NM.up=!NM.up;
	}
	if(NM.up && NM.y<0)
		NM.up=false;
	else if(!(NM.up) && NM.y>height-40)
		NM.up=true;
	NM.y-=NM.up*NM.speed;
	NM.y+=!(NM.up)*NM.speed;
}

void InitNMB(NPCB *NMB,NPC &NM){
	NMB->bound=(12/2);
	NMB->ID=BULLETN;
	NMB->speed=10;
	NMB->x=NM.x;
	NMB->y=NM.y;
	NMB->deg=1;
	NMB->next=NULL;
}

void DrawNMB(NPCB *NMB){
	while(NMB!=NULL){
		al_draw_filled_circle(NMB->x,NMB->y,10,al_map_rgb(0,0,255));
		NMB=NMB->next;
	}
}

NPCB* MoveNMB(NPCB *NMB){
	NPCB *last;
	NPCB *TMP;
	TMP=NMB;
	last=NULL;
	while(NMB!=NULL){
		NMB->x-=NMB->speed;
		if(NMB->x<0){
				TMP=NMB;
				NMB=NMB->next;
				free(TMP);
				TMP=NMB;
		}
		last=NMB;
		if (NMB!=NULL){
			NMB=NMB->next;
		}
	}
	return TMP;
}

NPCB* FireN(NPCB *NMB, NPC &NM){
	if(rand()%20==1){
		if (NMB==NULL){
			NMB=(NPCB*)malloc(sizeof(NPCB));
			InitNMB(NMB, NM);
			return NMB;
		}
		else{
			NPCB *TMP;
			TMP=NMB;
			while(NMB->next!=NULL){
			NMB=NMB->next;
			}
			NMB->next=(NPCB*)malloc(sizeof(NPCB));
			InitNMB(NMB->next, NM);
			return TMP;
		}
	}
	return NMB;
}

//COLLISIONS
NPCB* ColideTS(PC &TS, NPCB *NMB){
	NPCB *RET=NMB;
	NPCB *last=NULL;

	while(NMB!=NULL){

				al_draw_rectangle(NMB->x-NMB->bound, NMB->y-NMB->bound, NMB->x+NMB->bound, NMB->y+NMB->bound, al_map_rgb(255,0,0), 1);
				al_draw_rectangle(TS.x-TS.boundx, TS.y-TS.boundy, TS.x+TS.boundx, TS.y+TS.boundy, al_map_rgb(0,255,0), 1);

		if((( NMB->y-NMB->bound)<(TS.y+TS.boundy)) && 
			((NMB->y+NMB->bound)>(TS.y-TS.boundy)) &&
			((NMB->x-NMB->bound)>(TS.x-TS.boundx)) && 
			((NMB->x+NMB->bound)<(TS.x+TS.boundx))){
				NPCB *TMP;
				if(last==NULL){
					TMP=NMB;
					NMB=NMB->next;
					free(TMP);
					TMP=NULL;
					RET=NMB;
				}
				else{
					TMP=NMB;
					last->next=NMB->next;
					NMB=last;
					free(TMP);
					TMP=NULL;
				}
				TS.lives-=1;
				printf("Lives: %i\n",TS.lives);
		}
		if (NMB!=NULL){
			last=NMB;
			NMB=NMB->next;
		}
	}
	return RET;
}

PCB* ColideNM(NPC &NM, PCB *TSB,PC &TS){
	PCB *RET=TSB;
	PCB *last=NULL;

	while(TSB!=NULL){

				al_draw_rectangle(TSB->x-TSB->bound, TSB->y-TSB->bound, TSB->x+TSB->bound, TSB->y+TSB->bound, al_map_rgb(255,0,0), 1);
				al_draw_rectangle(NM.x-NM.boundx, NM.y-NM.boundy, NM.x+NM.boundx, NM.y+NM.boundy, al_map_rgb(0,255,0), 1);

		if((( TSB->y-TSB->bound)<(NM.y+NM.boundy)) && 
			((TSB->y+TSB->bound)>(NM.y-NM.boundy)) &&
			((TSB->x-TSB->bound)>(NM.x-NM.boundx)) && 
			((TSB->x+TSB->bound)<(NM.x+NM.boundx))){
				TS.hits++;
				PCB *TMP;
				if(last==NULL){
					TMP=TSB;
					TSB=TSB->next;
					free(TMP);
					TMP=NULL;
					RET=TSB;
				}
				else{
					TMP=TSB;
					last->next=TSB->next;
					TSB=last;
					free(TMP);
					TMP=NULL;
				}
				NM.lives-=1;
				printf("NM Lives: %i\n",NM.lives);
		}
		if (TSB!=NULL){
			last=TSB;
			TSB=TSB->next;
		}
	}
	return RET;
}

//GUI
void GUI(NPC NM, PC TS, ALLEGRO_FONT *font24){
	char text[25];
	//TS Lives Draw
	sprintf(text,"Twilights lives: %i",TS.lives);
	al_draw_text(font24,al_map_rgb(205, 50, 255),20,30,0,text);
	//Score Draw
	sprintf(text,"Score: %i",(TS.score+(500*TS.hits)));
	al_draw_text(font24,al_map_rgb(205, 50, 255),width-20,30,ALLEGRO_ALIGN_RIGHT,text);
	//NM Live
	al_draw_filled_rectangle(20, 5,((width)/40)*NM.lives , 25, al_map_rgb(50, 50, 255));
}

void Win(PC TS, NPC NM, ALLEGRO_FONT *font36){
	char text[50];
	al_clear_to_color(al_map_rgb(0,0,0));
	sprintf(text,"You Win! Score: %i",(TS.score+(500*TS.hits)+(5000*TS.lives)));
	al_draw_text(font36,al_map_rgb(205, 50, 255),width/2,height/2,ALLEGRO_ALIGN_CENTRE,text);
}

void Loser(PC TS, NPC NM, ALLEGRO_FONT *font36){
	char text[50];
	al_clear_to_color(al_map_rgb(0,0,0));
	sprintf(text,"You Lost! Score: %i",(500*TS.hits));
	al_draw_text(font36,al_map_rgb(205, 50, 255),width/2,height/2,ALLEGRO_ALIGN_CENTRE,text);
}