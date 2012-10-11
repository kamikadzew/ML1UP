#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>
#include "objects.h"



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
	
	//ALLEGRO INIT*********************************
	display = al_create_display(width,height);
	//DISPLAY CHECK
	if(!display)
	{
		al_show_native_message_box(NULL,NULL,NULL,"FAIL TO INIT DISPLAY",NULL,NULL);
		return -1;
	}
	
	al_init_primitives_addon();
	al_install_keyboard();
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0/fps);
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
		//THE RED X
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
			redraw=true;
		}
		//DRAWING
		if(redraw && al_is_event_queue_empty(event_queue)){
			redraw=false;

			DrawTS(TS);
			DrawNM(NM);
			DrawTSB(TSB);
			DrawNMB(NMB);
                        NMB=ColideTS(TS,NMB); // for debugging purposes
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
	TS.score=0;
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
	NM.x=width-10;
	NM.y=height/2-20;
	NM.ID=NONPLAYER;
	NM.lives=10;
	NM.speed=2;
	NM.boundx=6;
	NM.boundx=7;
	NM.up=true;
}

void DrawNM(NPC &NM){
	al_draw_filled_rectangle(NM.x, NM.y, NM.x-50, NM.y+40, al_map_rgb(0, 0, 128));
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
				//printf("dupa\n");
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
