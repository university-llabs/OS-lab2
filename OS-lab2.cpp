#include <iostream>
#include <Windows.h>
#include <vector>
#include <cstring>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc > 1 && strcmp(argv[1], "child") == 0) {
		cout << "CHILD PROCESS. My PID: " << GetCurrentProcessId() << endl;
		int n;
		cout << "Enter size: ";
		cin >> n;
		vector<int> v(n);
		cout << "Enter " << n << " elements";
		for (int& c : v) {
			cin >> c;
		}
		int product = 1;
		for (int& c : v) {
			if(c)	product *= c;
		}
		cout << product;
	}
	else {
		cout << "PARENT PROCESS. My PID: " << GetCurrentProcessId() << endl;
		int n;
		cout << "Enter size: ";
		cin >> n;
		vector<int> v(n);
		cout << "Enter " << n << " elements";
		for (int& c : v) {
			cin >> c;
		}
		// ��������� ��� ��������� ��������� ������������ ��� ������������ ������������
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);	// ������ ���������
		sa.bInheritHandle = TRUE;			// ��������� ����������� �����������
		sa.lpSecurityDescriptor = NULL;		// ������������ �� ���������

		//������ ����������� �������
		HANDLE hReadPipe1, hWritePipe1;
		HANDLE hReadPipe2, hWritePipe2;
		if (!CreatePipe(&hReadPipe1, &hWritePipe1, &sa, 0)) {
			cerr << "Failed to create pipe1";
			return 1;
		}
		if (!CreatePipe(&hReadPipe2, &hWritePipe2, &sa, 0)) {
			cerr << "Failed to create pipe2";
			return 1;
		}

		STARTUPINFO sui;
		ZeroMemory(&sui, sizeof(sui));		// ��������
		sui.cb = sizeof(sui);	
		sui.dwFlags = STARTF_USESTDHANDLES;  // ���������� ���� �����������
		sui.hStdInput = hReadPipe1;
		sui.hStdOutput = hWritePipe2;
		sui.hStdError = GetStdHandle(STD_ERROR_HANDLE);  // ������ � �������

		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi)); // �������� ���������

		char command[] = "OS-lab2.exe child";
		if (!CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &sui, &pi)) {
			cerr << "CreateProcess failed!" << endl;
			return 1;
		}
		cout << "Child process created successfully!" << endl;
		CloseHandle(hReadPipe1);   // ���� ����� ����� ������ �������
		CloseHandle(hWritePipe2);  // ���� ����� ����� ������ �������

		// ��������� ���������� �����������
		CloseHandle(hWritePipe1);
		CloseHandle(hReadPipe2);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	
	}
	return 0;
}
