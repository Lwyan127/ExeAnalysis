#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <windows.h>
using namespace std;

void ShowMenu();

int main() {
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
				MessageBoxA(NULL, "I'm MessageBoxA", "Title", MB_OK);
				break;
			}
			case 2: {
				MessageBoxW(NULL, L"I'm MessageBoxW", L"Title", MB_OK);
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
	return;
}