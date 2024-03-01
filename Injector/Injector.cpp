#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <thread>
#include <detours.h>

using namespace std;

#pragma comment(lib, "detours.lib")

int main() {

	STARTUPINFO si;								// 指定新进程的主窗口特性
	PROCESS_INFORMATION pi;						// 新进程及其主线程的信息
	ZeroMemory(&si, sizeof(si));				// 内存块清零
	ZeroMemory(&pi, sizeof(pi));				// 内存块清零
	si.cb = sizeof(si);							// 初始化含STARTUPINFO结构中的字节数初始化

	WCHAR EXE[MAX_PATH + 1] = { 0 };			// 目标程序地址
	WCHAR DirPath[MAX_PATH + 1] = { 0 };		// dll目录
	// -------------------------- 需要修改的部分 --------------------------
	wcscpy_s(EXE, MAX_PATH, L"D:\\workspace\\curriculum\\software_security_design\\ExeAnalysis\\Test\\Debug\\Test.exe");
	wcscpy_s(DirPath, MAX_PATH, L"D:\\workspace\\curriculum\\software_security_design\\ExeAnalysis\\Dll\\Debug");
	// 被注入dll地址
	char DllPath[MAX_PATH + 1] = "D:\\workspace\\curriculum\\software_security_design\\ExeAnalysis\\Dll\\Debug\\Dll.dll";
	// -------------------------- 需要修改的部分 --------------------------

	// inject的部分
	if (DetourCreateProcessWithDllEx(EXE, NULL, NULL, NULL, TRUE,
		CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DirPath, &si, &pi, DllPath, NULL)) {
		MessageBoxA(NULL, "CreateProcessSucceed!", "Title", MB_OK);
		ResumeThread(pi.hThread);  // 恢复线程
		WaitForSingleObject(pi.hProcess, INFINITE);
	}
	else {
		MessageBoxA(NULL, "Error!", "Title", MB_OK);
	}
	return 0;
}