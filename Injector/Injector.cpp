#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <thread>
#include <detours.h>

using namespace std;

#pragma comment(lib, "detours.lib")

int main() {

	STARTUPINFO si;								// ָ���½��̵�����������
	PROCESS_INFORMATION pi;						// �½��̼������̵߳���Ϣ
	ZeroMemory(&si, sizeof(si));				// �ڴ������
	ZeroMemory(&pi, sizeof(pi));				// �ڴ������
	si.cb = sizeof(si);							// ��ʼ����STARTUPINFO�ṹ�е��ֽ�����ʼ��

	WCHAR EXE[MAX_PATH + 1] = { 0 };			// Ŀ������ַ
	WCHAR DirPath[MAX_PATH + 1] = { 0 };		// dllĿ¼
	// -------------------------- ��Ҫ�޸ĵĲ��� --------------------------
	wcscpy_s(EXE, MAX_PATH, L"D:\\workspace\\curriculum\\software_security_design\\ExeAnalysis\\Test\\Debug\\Test.exe");
	wcscpy_s(DirPath, MAX_PATH, L"D:\\workspace\\curriculum\\software_security_design\\ExeAnalysis\\Dll\\Debug");
	// ��ע��dll��ַ
	char DllPath[MAX_PATH + 1] = "D:\\workspace\\curriculum\\software_security_design\\ExeAnalysis\\Dll\\Debug\\Dll.dll";
	// -------------------------- ��Ҫ�޸ĵĲ��� --------------------------

	// inject�Ĳ���
	if (DetourCreateProcessWithDllEx(EXE, NULL, NULL, NULL, TRUE,
		CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED, NULL, DirPath, &si, &pi, DllPath, NULL)) {
		MessageBoxA(NULL, "CreateProcessSucceed!", "Title", MB_OK);
		ResumeThread(pi.hThread);  // �ָ��߳�
		WaitForSingleObject(pi.hProcess, INFINITE);
	}
	else {
		MessageBoxA(NULL, "Error!", "Title", MB_OK);
	}
	return 0;
}