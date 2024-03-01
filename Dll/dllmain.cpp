// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <windows.h>
#include <detours.h>

#pragma comment(lib, "detours.lib")

using namespace std;

SYSTEMTIME st;  // 系统时间

// 消息框操作
static int (WINAPI* OldMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;

static int (WINAPI* OldMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;

extern "C" __declspec(dllexport) int WINAPI NewMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) {
	GetLocalTime(&st);
	printf("----------------------------------------------------------\n");
	printf("Hook MessageBoxA: %d-%d-%d %02d:%02d:%02d:%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	printf("hWnd = %08X\nlpText = %s\nlpCaption = %s\nuType = %08X\n", (int)hWnd, lpText, lpCaption, uType);
	printf("----------------------------------------------------------\n");
	// 返回原始接口
	return OldMessageBoxA(hWnd, "Hooked!", "MessageBoxA", MB_OK);
}

extern "C" __declspec(dllexport) int WINAPI NewMessageBoxW(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) {
	GetLocalTime(&st);
	char lpTextChar[70];
	char lpCaptionChar[70];
	memset(lpTextChar, 0, sizeof(lpTextChar));
	memset(lpCaptionChar, 0, sizeof(lpCaptionChar));
	// 宽字节转char
	if (lpText != NULL) {
		WideCharToMultiByte(CP_ACP, 0, lpText, wcslen(lpText), lpTextChar, sizeof(lpTextChar), NULL, NULL);
	}
	if (lpCaption != NULL) {
		WideCharToMultiByte(CP_ACP, 0, lpCaption, wcslen(lpCaption), lpCaptionChar, sizeof(lpCaptionChar), NULL, NULL);
	}

	printf("----------------------------------------------------------\n");
	printf("Hook MessageBoxW: %d-%d-%d %02d:%02d:%02d:%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	printf("hWnd = %08X\nlpText = %s\nlpCaption = %s\nuType = %08X\n", (int)hWnd, lpTextChar, lpCaptionChar, uType);
	printf("----------------------------------------------------------\n");
	// 返回原始接口
	return OldMessageBoxW(hWnd, L"Hooked!", L"MessageBoxW", MB_OK);
}

// 堆操作
static HANDLE(WINAPI* OldHeapCreate)(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) = HeapCreate;

static BOOL(WINAPI* OldHeapDestory)(HANDLE) = HeapDestroy;

static BOOL(WINAPI* OldHeapFree)(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem) = HeapFree;

static LPVOID(WINAPI* OldHeapAlloc)(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) = HeapAlloc;

extern "C" __declspec(dllexport)HANDLE WINAPI NewHeapCreate(DWORD fIOoptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize) {
	GetLocalTime(&st);
	HANDLE hFile = OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
	printf("----------------------------------------------------------\n");
	printf("Hook HeapCreate: %d-%d-%d %02d:%02d:%02d:%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	printf("hFile(HANDLE) = %08X\nfIOoptions = %08X\ndwInitialSize = %08X\ndwMaximumSize = %08X\n", (int)hFile, fIOoptions, dwInitialSize, dwMaximumSize);
	printf("----------------------------------------------------------\n");
	return OldHeapCreate(fIOoptions, dwInitialSize, dwMaximumSize);
}

extern "C" __declspec(dllexport) BOOL WINAPI NewHeapDestory(HANDLE hHeap) {
	GetLocalTime(&st);
	printf("----------------------------------------------------------\n");
	printf("Hook HeapDestory: %d-%d-%d %02d:%02d:%02d:%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	printf("hHeap = %08X\n", (int)hHeap);
	printf("----------------------------------------------------------\n");
	return OldHeapDestory(hHeap);
}

extern "C" __declspec(dllexport) BOOL WINAPI NewHeapFree(HANDLE hHeap, DWORD dwFlags, _Frees_ptr_opt_ LPVOID lpMem) {
	GetLocalTime(&st);
	printf("----------------------------------------------------------\n");
	printf("Hook HeapFree: %d-%d-%d %02d:%02d:%02d:%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	printf("hHeap = %08X\ndwFlags = %08X\nlpMem = %08X\n", (int)hHeap, dwFlags, (int)lpMem);
	printf("----------------------------------------------------------\n");
	return OldHeapFree(hHeap, dwFlags, lpMem);
}

extern "C" __declspec(dllexport) LPVOID WINAPI NewHeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes) {
	LPVOID pHeap = OldHeapAlloc(hHeap, dwFlags, dwBytes);
	//printf("----------------------------------------------------------\n");
	//printf("Hook HeapAlloc: %d-%d-%d %02d:%02d:%02d:%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	//printf("hHeap = %08X\ndwFlags = %08X\nlpMem = %08X\n", (int)hHeap, dwFlags, (int)dwBytes);
	//printf("----------------------------------------------------------\n");
	//cout << "----------------------------------------------------------" << endl;
	//cout << "Hook HeapAlloc: " << st.wYear << "-" << st.wMonth << "-" << st.wDay << " ";
	//cout << st.wMinute << ":" << st.wSecond << ":" << st.wMilliseconds << endl;
	//cout << "----------------------------------------------------------" << endl;
	return OldHeapAlloc(hHeap, dwFlags, dwBytes);
}

// dll主程序
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
		// dll刚刚被加载到一个进程中
	case DLL_PROCESS_ATTACH: {
		DisableThreadLibraryCalls(hModule);							// 优化线程，禁用DLL_THREAD_ATTACH和DLL_THREAD_DETACH通知
		DetourTransactionBegin();									// 开始一次hook，进行初始化
		DetourUpdateThread(GetCurrentThread());						// 加入待hook的线程
		DetourAttach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);		// hook
		DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
		DetourAttach(&(PVOID&)OldHeapCreate, NewHeapCreate);
		DetourAttach(&(PVOID&)OldHeapDestory, NewHeapDestory);
		DetourAttach(&(PVOID&)OldHeapFree, NewHeapFree);
		DetourAttach(&(PVOID&)OldHeapAlloc , NewHeapAlloc);
		DWORD nErrAttach = DetourTransactionCommit();				// 提交Detour函数，返回值判断
		if (nErrAttach != NO_ERROR) {
			printf("AttachError!\n");
		}
		break;
	}
	case DLL_THREAD_ATTACH:											// 被禁用
	case DLL_THREAD_DETACH:											// 被禁用
		// dll刚刚从一个进程中卸载
	case DLL_PROCESS_DETACH: {
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);		// 取消hook
		DetourDetach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
		DetourDetach(&(PVOID&)OldHeapCreate, NewHeapCreate);
		DetourDetach(&(PVOID&)OldHeapDestory, NewHeapDestory);
		DetourDetach(&(PVOID&)OldHeapFree, NewHeapFree);
		DetourDetach(&(PVOID&)OldHeapAlloc, NewHeapAlloc);
		DWORD nErrDetach = DetourTransactionCommit();
		if (nErrDetach != NO_ERROR) {
			printf("DetachError!\n");
		}
		break;
	}
	break;
	}
	return TRUE;
}

