struct List{
	void *element;
	struct List *next;
};


List *ListDel(List *LST,List *last);
List *ListAdd(List *LST);


List *ListDel(List *LST,List *last){
List *TMP;
if(last==NULL){
		TMP=LST;
		LST=LST->next;
		free(TMP);
		TMP=NULL;
	}
	else{
		TMP=LST;
		last->next=LST->next;
		LST=last;
		free(TMP);
		TMP=NULL;
	}
	return LST;
}

List *ListAdd(List *LST){
	if (LST==NULL){
		LST=(List*)malloc(sizeof(List));
	}
	else{
		List *TMP;
		TMP=LST;
		while(TMP->next!=NULL){
			TMP=TMP->next;
		}
		TMP->next=(List*)malloc(sizeof(List));
	}
	return LST;
}