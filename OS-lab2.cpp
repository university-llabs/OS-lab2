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
		// структура для настройки атрибутов безопасности для наследования дескрипторов
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);	// размер структуры
		sa.bInheritHandle = TRUE;			// разрешить наследовать дескрипторы
		sa.lpSecurityDescriptor = NULL;		// безопасность по умолчанию

		//Создаём дескрипторы каналов
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
