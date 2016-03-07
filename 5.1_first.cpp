//First Fit Allocation

#include <iostream>

#define MAX 1024	//the size of memory 
#define MIN 4 		//4 byte aligned

using namespace std;

struct block
{
	int startAddress;
	int size;
	struct block *last;
	struct block *next;
	struct block *lastFree;
	struct block *nextFree;
	bool flag;
};

block *base;
block *freeBase;

void blockInit(){
	base = new block();
	base->startAddress = 0;
	base->size = MAX;
	base->last = NULL;
	base->next = NULL;
	base->lastFree = NULL;
	base->nextFree = NULL;
	base->flag = false;
	freeBase = new block();
	freeBase->startAddress = -1;
	freeBase->size = -1;
	freeBase->last = NULL;
	freeBase->next = NULL;
	freeBase->lastFree = NULL;
	freeBase->nextFree = base;
	base->lastFree = freeBase;
}

void blockSplit(block *head, int num){
	int a = num % 4;
	if(a != 0){
		num = (num/4+1) * 4;
	}
	if(num == head->size){
		head->flag = true;
	}
	else{
		block *myBlock = new block();
		myBlock->startAddress = head->startAddress + num;
		myBlock->size = head->size - num;
		head->size = num;
		myBlock->next = head->next;
		head->next = myBlock;
		myBlock->last = head;
		head->flag = true; 
		myBlock->flag = false;
		myBlock->lastFree = head->lastFree;
		if(myBlock->lastFree != NULL){
			myBlock->lastFree->nextFree = myBlock;
		}
		myBlock->nextFree = head->nextFree;
		if(myBlock->nextFree != NULL)
			myBlock->nextFree->lastFree = myBlock;
		head->lastFree = head->nextFree = NULL;
	}
}

void *blockMalloc(int num){
	block *head;
	if(freeBase->nextFree != NULL){
		head = freeBase->nextFree;
	}
	else{
		return NULL;
	}
	while(1){
		if(head->size >= num){
			blockSplit(head, num);
			return head;
		}
		else{
			if(head->nextFree != NULL){
				head = head->nextFree;
			}
			else{
				return NULL;
			}
		}
		if(head->nextFree != NULL){
			head = head->nextFree;
		}
		else{
			return NULL;
		}
	}
}

void blockFree(void *p){
	block *freeHead = freeBase->nextFree;
	block *head = (block *)p;
	head->flag = false;
	while(freeHead){
		if(freeHead->startAddress < head->startAddress){
			if(freeHead->nextFree != NULL){
				if(freeHead->nextFree->startAddress < head->startAddress){
					freeHead = freeHead->nextFree;
					continue;
				}
				else
					break;
			}
			else
				break;
		}
		else{
			freeHead = freeHead->lastFree;
			break;
		}
	}
	if(!freeHead){
		freeBase->nextFree = head;
		head->lastFree = freeBase;
		head->nextFree = NULL;
	}
	else{		
		if(freeHead->startAddress+freeHead->size == head->startAddress){
			head = head->last;
			head->size = head->size + head->next->size;
			head->next = head->next->next;
			if(head->next != NULL)
				head->next->last = head;
		}
		else{
			head->nextFree = freeHead->nextFree;
			head->lastFree = freeHead;
			freeHead->nextFree = head;
		}
		if(head->nextFree != NULL && head->startAddress + head->size == head->nextFree->startAddress){
			head->nextFree = head->nextFree->nextFree;
			if(head->nextFree != NULL)
				head->nextFree->lastFree = head;
			head->size = head->size + head->next->size;
			head->next = head->next->next;
			if(head->next != NULL)
				head->next->last = head;
		}
	}

}

void blockPrint(){
	block *head = base;
	int i = 1;
	while(head){
		cout << "No." << i << "block, " << "size: " << head->size << ", ";
		if(head->flag == false){
			cout << "not used." << endl;
		}
		else{
			cout << "used." << endl;
		}
		head = head->next;
		i++;
	}
}

int main(){
	blockInit();
	blockPrint();
	int *a = (int *)blockMalloc(39);
	blockPrint();
	char *b = (char *)blockMalloc(100);
	blockPrint();
	blockFree(a);
	blockPrint();
	blockFree(b);
	blockPrint();
}