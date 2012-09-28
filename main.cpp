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
void MoveTSB(PCB *TSB);
void InitNM(NPC &NM);
void DrawNM(NPC &NM);
void MoveNM(NPC &NM);
void InitNMB(NPCB *NMB,NPC &NM);
void DrawNMB(NPCB *NMB);
void MoveNMB(NPCB *NMB);

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
	//NPCB NMB;
	
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
			MoveTSB(TSB);
			redraw=true;
		}
		//DRAWING
		if(redraw && al_is_event_queue_empty(event_queue)){
			redraw=false;

			DrawTS(TS);
			DrawNM(NM);
			DrawTSB(TSB);
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
	TS.x=10;
	TS.y=height/2;
	TS.ID=PLAYER;
	TS.lives=3;
	TS.speed=7;
	TS.boundx=6;
	TS.boundx=7;
	TS.score=0;
}

void DrawTS(PC &TS){
	al_draw_filled_rectangle(TS.x, TS.y, TS.x+60, TS.y+20, al_map_rgb(205, 50, 255));
}

void MoveTS(PC &TS){
	TS.y-=keys[UP]*TS.speed;
	TS.y+=keys[DOWN]*TS.speed;
	TS.x-=keys[LEFT]*TS.speed;
	TS.x+=keys[RIGHT]*TS.speed;
	if(TS.y<0)
		TS.y=0;
	if(TS.y>height-20)
		TS.y=height-20;
	if(TS.x<0)
		TS.x=0;
	if(TS.x>width/2-60)
		TS.x=width/2-60;
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
		//printf("FIRE\n");
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
}

void DrawTSB(PCB *TSB){
	while(TSB!=NULL){
		al_draw_filled_circle(TSB->x,TSB->y,10,al_map_rgb(255,255,255));
		TSB=TSB->next;
	}
}

void MoveTSB(PCB *TSB){
	while(TSB!=NULL){
		TSB->x+=TSB->speed;
		if(TSB->x>width){
			//TODO DESTROY BUILET
		}
		TSB=TSB->next;
	}
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

void InitNMB(NPCB &NMB,NPC NM){
	NMB.bound=2;
	NMB.ID=BULLETN;
	NMB.speed=10;
	NMB.x=NM.x;
	NMB.y=NM.y;
	NMB.deg=1;
	NMB.next=NULL;
}

void DrawNMB(NPCB &NMB);

void MoveNMB(NPCB &NMB);



