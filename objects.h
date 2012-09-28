// Objecht ids
enum IDS{PLAYER,BULLETP,BULLETN,NONPLAYER};

//Player Caracter
struct PC{
	int ID;
	int x;
	int y;
	int lives;
	int speed;
	int boundx;
	int boundy;
	int score;
};

//Non Player Character
struct NPC{
	int ID;
	int x;
	int y;
	int lives;
	int speed;
	int boundx;
	int boundy;
	bool up;
};

//Player Builets
struct PCB{
	int ID;
	int x;
	int y;
	int speed;
	int bound;
	struct PCB *next;
};

//NonPlayer Builets
struct NPCB{
	int ID;
	int x;
	int y;
	int speed;
	int bound;
	int deg;
	NPCB *next;
};