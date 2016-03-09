#include <iostream>
#include <fstream>

using namespace std;

int memory[128][32];

void readFile(){
	ifstream fin;
	fin.open("memory.txt");
	for(int i = 0;i < 128; i++){
		for(int j = 0; j < 32;j++){
			fin >> hex >> memory[i][j];
		}
	}
	fin.close();
}

int main(){
	readFile();
	cout << "Please input virtual address: 0x";
	int virtualAddress;
	int PDRB = 0x220;
	cin >> hex >> virtualAddress;
	cout << "Virtual Address " << hex << virtualAddress << ":" << endl;

	int pdeIndex = (0x7c00 & virtualAddress) >> 10;
	int pteIndex = (0x03e0 & virtualAddress) >> 5;
	int offset = (0x001f & virtualAddress);

	int pdeContents = memory[(pdeIndex + PDRB) / 32][(pdeIndex + PDRB) % 32];
	int valid = pdeContents >> 7;
	int pfn = pdeContents & 0x7f;
	cout << "\t--> pde index:0x" << hex << pdeIndex << "  pde contents:(valid " << valid << ", pfn 0x" << hex << pfn << ")" << endl;

	if(valid == 0){
		cout << "\t\t--> Fault (page directory entry not valid)" << endl;
	}
	else{
		int pteContents = memory[pfn][pteIndex];
		valid = pteContents >> 7;
		pfn = pteContents & 0x7f;
		cout << "\t\t--> pte index:0x" << hex << pteIndex << "  pte contents:(valid " << valid << ", pfn 0x" << hex << pfn << ")" << endl;
		if(valid == 0){
			cout << "\t\t\t--> Fault (page table entry not valid)" << endl;
		}
		else{
			int physicalAddress = (pfn << 5) + offset;
			int value = memory[pfn][offset];
			cout << "\t\t\t--> Translates to Physical Address 0x" << hex << physicalAddress << " --> Value: " << hex << value << endl;
		}
	}
}