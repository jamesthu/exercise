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
	bool flag;
};

block *base;

void blockInit(){
	base = new block();
	base->startAddress = 0;
	base->size = MAX;
	base->last = NULL;
	base->next = NULL;
	base->flag = false;
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
	}
}

void *blockMalloc(int num){
	block *head = base;
	while(1){
		if(!head->flag){
			if(head->size >= num){
				blockSplit(head, num);
				return head;
			}
			else{
				if(head->next != NULL){
					head = head->next;
				}
				else{
					return NULL;
				}
			}
		}
		if(head->next != NULL){
			head = head->next;
		}
		else{
			return NULL;
		}
	}
}

void blockFree(void *p){
	block *head = (block *)p;
	head->flag = false;
	if(head->last != NULL && head->last->flag == false){
		head = head->last;
		head->size += head->next->size;
		head->next = head->next->next;
		if(head->next != NULL){
			head->next->last = head;
		}
	}
	if(head->next != NULL && head->next->flag == false){
		head->size += head->next->size;
		head->next = head->next->next;
		if(head->next != NULL){
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