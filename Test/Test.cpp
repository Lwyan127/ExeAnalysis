#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <windows.h>
#define PAGE_SIZE	4096

using namespace std;

void ShowMenu();
void HeapCreateAndDestroy();

int main() {
	printf("Test Start.\n");
	int op;
	while (true) {
		ShowMenu();
		cin >> op;
		switch (op) {
			case 0: {
				cout << "Quit." << endl;
				exit(0);
			}
			case 1: {
				MessageBoxA(NULL, "I'm MessageBoxA", "Title", MB_YESNO);
				break;
			}
			case 2: {
				MessageBoxW(NULL, L"I'm MessageBoxW", L"Title", MB_YESNO);
				break;
			}
			case 3: {
				HeapCreateAndDestroy();
				break;
			}
			break;
		}
	}
	return 0;
}

void ShowMenu() {
	cout << "Please choose:" << endl;
	cout << "0.Quit" << endl;
	cout << "1.MessageBoxA" << endl;
	cout << "2.MessageBoxW" << endl;
	cout << "3.HeapCreate" << endl;
	return;
}

void HeapCreateAndDestroy() {
	// HeapCreate
	printf("Start to create heap...\n");
	HANDLE hHeap = HeapCreate(HEAP_NO_SERIALIZE, PAGE_SIZE * 10, PAGE_SIZE * 100);
	if (hHeap == NULL) {  // HeapCreate句柄为NULL错误处理
		printf("hHeap is NULL.HeapCreate failed!\n");
		return;
	}
	int* pHeap = (int*)HeapAlloc(hHeap, 0, sizeof(int) * 10);
	if (pHeap == NULL) {  // HeapAlloc返回指针为NULL错误处理
		printf("pHeap is NULL.HeapAlloc failed!\n");
		return;
	}
	for (int i = 0; i < 10; i++) {
		pHeap[i] = i;  // 向堆里赋值
	}
	printf("Successfully created!\n");
	printf("In heap:");
	for (int i = 0; i < 10; i++) {
		printf("%d ", pHeap[i]);  // 输出堆中元素
	}
	printf("\n");
	
	// HeapDestroy
	printf("Start to destroy heap...\n");
	HeapFree(hHeap, 0, pHeap);
	HeapDestroy(hHeap);
	printf("Successfully destoryed!\n");
	return;
}