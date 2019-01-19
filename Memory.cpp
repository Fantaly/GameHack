#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <Windows.h>

#undef max

using namespace std;
void spawnThreads(HWND hwnd);
bool Process_change(HWND hwnd_id);
int Process_Check_1(HWND hwnd);
int Process_Check(HWND hwnd);

int main()
{
	HWND hwnd = FindWindowA(NULL, "C:\\Users\\Manu\\source\\repos\\ConsoleApplication1\\Debug\\ConsoleApplication1.exe");
	spawnThreads(hwnd);
	Process_Check_1(hwnd);
	return 0;
}

void spawnThreads(HWND hwnd)
{
	thread threads[2];
	threads[0] = thread(Process_Check_1, hwnd);
	threads[1] = thread(Process_Check, hwnd);
	for (auto& th : threads) 
		th.join();

}

int Process_Check_1(HWND hwnd)
{
	if (hwnd == NULL)
	{
		cout << "No window finded." << endl;
		Sleep(3000);
		exit(-1);
	}
	else
	{
		Process_change(hwnd);
		return 1;
	}
	return 1;
}

int Process_Check(HWND hwnd)
{
	while (1) 
	{
		HWND hwnd = FindWindowA(NULL, "C:\\Users\\Manu\\source\\repos\\ConsoleApplication1\\Debug\\ConsoleApplication1.exe");
		if (hwnd == NULL)
		{
			cout << "No window finded." << endl;
			Sleep(3000);
			exit(-1);
		}
	}
	return 0;
}

bool Process_change(HWND hwnd_id)
{
	/* Start declaration*/
	stringstream str;
	SIZE_T Size_n = sizeof(int);
	DWORD ProcID = NULL;
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
	GetWindowThreadProcessId(hwnd_id, &ProcID);
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcID);
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
			ReadProcessMemory(handle, address, &value_out, Size_n, 0);
			cout << "\n\nValore attuale: " << value_out;
			cout << "\nInserisci valore: ";
			cin >> value_in;
			if (!(cin.fail()))
			{
				cout << endl;
				WriteProcessMemory(handle, address, &value_in, Size_n, 0); // HANDLE - ADRESS - VALORE - SIZE INTEGER
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

