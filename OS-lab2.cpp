#include <iostream>
#include <Windows.h>
#include <vector>
#include <cstring>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc > 1 && strcmp(argv[1], "child") == 0) {

	}
	else {
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
		if (!CreatePipe(&hReadPipe1, &hWritePipe2, &sa, NULL)) {
			cerr << "Failed to create pipe1";
		}
		if (!CreatePipe(&hReadPipe2, &hWritePipe2, &sa, NULL)) {
			cerr << "Failed to create pipe2";
		}
		STARTUPINFO sui;
		sui.hStdInput = stdin;
		sui.hStdOutput = stdout;
	}
	return 0;
}
