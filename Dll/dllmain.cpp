// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <detours.h>

#pragma comment(lib, "detours.lib")

SYSTEMTIME st;  // 系统时间

static int (WINAPI* OldMessageBoxW)(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) = MessageBoxW;
static int (WINAPI* OldMessageBoxA)(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) = MessageBoxA;

extern "C" __declspec(dllexport) int WINAPI NewMessageBoxA(_In_opt_ HWND hWnd, _In_opt_ LPCSTR lpText, _In_opt_ LPCSTR lpCaption, _In_ UINT uType) {
	printf("-----------------------------\n");
	GetLocalTime(&st);
	printf("Hook MessageBoxA: %d-%d-%d %02d:%02d:%02d:%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	printf("-----------------------------\n");
	// 返回原始接口
	return OldMessageBoxA(hWnd, "Hooked!", "MessageBoxA", MB_OK);
}

extern "C" __declspec(dllexport) int WINAPI NewMessageBoxW(_In_opt_ HWND hWnd, _In_opt_ LPCWSTR lpText, _In_opt_ LPCWSTR lpCaption, _In_ UINT uType) {
	printf("-----------------------------\n");
	GetLocalTime(&st);
	printf("Hook MessageBoxA: %d-%d-%d %02d:%02d:%02d:%03d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	printf("-----------------------------\n");
	// 返回原始接口
	return OldMessageBoxW(hWnd, L"Hooked!", L"MessageBoxW", MB_OK);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
		// dll刚刚被加载到一个进程中
	case DLL_PROCESS_ATTACH: {
		DisableThreadLibraryCalls(hModule);							// 优化线程，禁用DLL_THREAD_ATTACH和DLL_THREAD_DETACH通知
		DetourTransactionBegin();									// 开始一次hook，进行初始化
		DetourUpdateThread(GetCurrentThread());						// 加入待hook的线程
		DetourAttach(&(PVOID&)OldMessageBoxW, NewMessageBoxW);		// hook
		DetourAttach(&(PVOID&)OldMessageBoxA, NewMessageBoxA);
		DWORD nErrAttach = DetourTransactionCommit();				// 提交Detour函数，返回值判断
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
		DWORD nErrDetach = DetourTransactionCommit();
		break;
	}
	break;
	}
	return TRUE;
}

