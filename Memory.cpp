#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <Windows.h>
#undef max

using namespace std;
void spawnThreads(HWND hwnd, DWORD ProcID, HANDLE handle);
bool Process_change(HWND hwnd, DWORD ProcID, HANDLE handle);
int Process_Check_1(HWND hwnd, DWORD ProcID, HANDLE handle);
int Process_Check(HWND hwnd, DWORD ProcID, HANDLE handle);

int main()
{
	DWORD ProcID = NULL;
	HWND hwnd = FindWindowA(NULL, "C:\\Users\\Manu\\source\\repos\\ConsoleApplication1\\Debug\\ConsoleApplication1.exe");
	GetWindowThreadProcessId(hwnd, &ProcID);
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcID);
	spawnThreads(hwnd, ProcID, handle);
	return 0;
}

void spawnThreads(HWND hwnd, DWORD ProcID, HANDLE handle)
{
	thread threads[2];
	threads[0] = thread(Process_Check_1, hwnd, ProcID, handle);
	threads[1] = thread(Process_Check, hwnd, ProcID, handle);
	for (auto& th : threads) 
		th.join();

}

int Process_Check_1(HWND hwnd, DWORD ProcID, HANDLE handle)
{
	if (hwnd == NULL)
	{
		cout << "No window finded." << endl;
		Sleep(3000);
		exit(-1);
	}
	else
	{
		Process_change(hwnd, ProcID, handle);
		return 1;
	}
	return 1;
}

int Process_Check(HWND hwnd, DWORD ProcID, HANDLE handle)
{
	DWORD exitCode = 0;
	while (1) 
	{
		GetExitCodeProcess(handle, &exitCode);
		if (exitCode != STILL_ACTIVE)
		{
			system("cls");
			cout << "Processo terminato." << endl;
			Sleep(3000);
			exit(-1);
		}
	}
	return 0;
}

bool Process_change(HWND hwnd, DWORD ProcID, HANDLE handle)
{
	/* Start declaration*/
	stringstream str;
	int addr = 0;
	string addr_string = "";
	/* End declaration*/

	cout << "Insert memory address: ";
	cin >> addr_string;

	/*Conversion*/
	str << addr_string; /* Conversione string esadecimale */ str >> std::hex >> addr;
	/*END Conversion*/

	LPVOID address = (VOID*)addr; //Assigned value of addr as an address to be pointed
	cout << address;
	if (ProcID == NULL)
	{
		cout << "Cannot obtain the process." << endl;
		Sleep(3000);
		return false;
	}
	else
	{
		int value_out = 0;
		int value_in = 0;
		while (1)
		{
			ReadProcessMemory(handle, address, &value_out, sizeof(int), 0);
			cout << "\n\nValore attuale: " << value_out;
			cout << "\nInserisci valore: ";
			cin >> value_in;
			if (!(cin.fail()))
			{
				cout << endl;
				WriteProcessMemory(handle, address, &value_in, sizeof(int), 0); // HANDLE - ADRESS - VALORE - SIZE INTEGER
			}
			else if (cin.fail()){
				cout << "Tipo di valore errato" << endl;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
	}
	return true;
}

