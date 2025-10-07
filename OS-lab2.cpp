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
		DWORD bytesRead;

		if (!ReadFile(GetStdHandle(STD_INPUT_HANDLE), &n, sizeof(n), &bytesRead, NULL)) {
			cerr << "Child: failed to read size" << endl;
			return 1;
		}

		vector<int> v(n);
		if (!ReadFile(GetStdHandle(STD_INPUT_HANDLE), v.data(), n * sizeof(int), &bytesRead, NULL)) {
			cerr << "Child: failed to read array" << endl;
			return 1;
		}
		int product = 1;
		bool hasNonZero = false;
		for (int c : v) {
			if (c != 0) {
				product *= c;
				hasNonZero = true;
			}
		}
		if (!hasNonZero) {
			product = 0;  
		}

		DWORD bytesWritten;
		if (!WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), &product, sizeof(product), &bytesWritten, NULL)) {
			cerr << "Child: failed to write result" << endl;
			return 1;
		}

		return 0;
	}
	else {
		cout << "PARENT PROCESS. My PID: " << GetCurrentProcessId() << endl;
		int n;
		cout << "Enter size: ";
		cin >> n;
		vector<int> v(n);
		cout << "Enter " << n << " elements: ";
		for (int& c : v) {
			cin >> c;
		}
		
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.bInheritHandle = TRUE;
		sa.lpSecurityDescriptor = NULL;

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

		STARTUPINFOA sui;
		ZeroMemory(&sui, sizeof(sui));
		sui.cb = sizeof(sui);
		sui.dwFlags = STARTF_USESTDHANDLES;
		sui.hStdInput = hReadPipe1;
		sui.hStdOutput = hWritePipe2;
		sui.hStdError = GetStdHandle(STD_ERROR_HANDLE);

		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi)); 

		char command[] = "OS-lab2.exe child";
		if (!CreateProcessA(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &sui, &pi)) {
			cerr << "CreateProcess failed!" << endl;
			return 1;
		}
		cout << "Child process created successfully!" << endl;
		CloseHandle(hReadPipe1);
		CloseHandle(hWritePipe2);

		DWORD bytes_written;
		if (!WriteFile(hWritePipe1, &n, sizeof(n), &bytes_written, NULL)) {
			cerr << "Failed to write size to pipe" << endl;
			return 1;
		}
		if (!WriteFile(hWritePipe1, v.data(), n * sizeof(int), &bytes_written, NULL)) {
			cerr << "Failed to write array to pipe" << endl;
			return 1;
		}
		CloseHandle(hWritePipe1);
		int result;
		DWORD bytes_readen;
		if (!ReadFile(hReadPipe2, &result, sizeof(result), &bytes_readen, NULL)) {
			cerr << "Failed to read result from pipe" << endl;
			return 1;
		}

		cout << "Result readen from child: " << result << endl;
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(hReadPipe2);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

	}
	return 0;
}
