struct List{
	void *element;
	struct List *next;
};


List *ListDel(List *LST, List *last);
List *ListAdd(List *LST);
