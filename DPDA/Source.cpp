#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include <Windows.h>
#include <conio.h>
using namespace std;

struct dpdaState{

	int trans_num;
	char trans_symbol[10];
	int trans_state[10];
	char stackpop[10];
	string stackpush[10];

};

int *acc_state;
string str;
dpdaState curr_node;
int num_states;
dpdaState *dpda;
int num_sym;
char sym[10];
int num_acc;
stack<char> stck;
int init_state;

void gotoxy(int x, int y)
{
	COORD pos;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (INVALID_HANDLE_VALUE != hConsole)
	{
		pos.X = x;
		pos.Y = y;
		SetConsoleCursorPosition(hConsole, pos);
	}
}

void check_str()
{
	system("cls");

	cout << "\n\tEnter your string:\t";
	cin >> str;
	int y;
	bool check = false;
	bool check_alph = false;
	int s = str.size();
	for (int i = 0; i < s; i++)
	{
		check_alph = false;
		for (int j = 0; j < num_sym + 1; j++)
		{
			if (str[i] == sym[j])
				check_alph = true;
		}
		if (!check_alph)
		{
			system("color f4");
			int f;
			cout << "Foreign Alphabet !!";
			cout << "\n\n\tDo you want to enter a new string?\n\t\t1-YES\n\t\t2-NO\n\t";
			cin >> f;
			if (f == 1)
			{
				curr_node = dpda[init_state];
				check_str();
			}
			else return;
		}
	}

	bool check_top = true;
	stck.push('$');

	for (int i = 0; i < s; i++)
	{
		for (int j = 0; j < curr_node.trans_num; j++)
		{
			if (curr_node.trans_symbol[j] == str[i])
			{
				char top = stck.top();
				if (top != curr_node.stackpop[j])
				{
					check_top = false;
					break;
				}
				stck.pop();

				for (int k = curr_node.stackpush[j].size(); k > 0; k--)
				{
					char s_push = curr_node.stackpush[j][k - 1];

					if (s_push != 'l')
					{
						stck.push(s_push);
					}
				}

				y = curr_node.trans_state[j];
			}

			else if (curr_node.trans_symbol[j] == 'l')
			{
				int ltrans = 0;
				char top = stck.top();

				if (top == '$')
					ltrans++;

				if (curr_node.stackpush[j].size() == 1)
				{
					char s_push = curr_node.stackpush[j][0];
					if (s_push == '$')
						ltrans++;
				}

				if (ltrans == 2)
					y = curr_node.trans_state[j];
			}
		}
		if (!check_top) break;
		else curr_node = dpda[y];
	}


	int d = 25;

	for (int i = 0; i < s; i++)
	{
		system("cls");
		cout << "\n\t\t" << str << " : " << endl << endl << endl;
		gotoxy(25, 5);
		cout << str;
		gotoxy(d, 6);
		cout << "^";
		Sleep(1000);
		d++;
	}

	gotoxy(16 + s, 1);


	for (int i = 0; i < num_acc; i++)
	{
		if (acc_state[i] == y && check == false && check_top == true){
			system("color f2");
			check = true;
			cout << "\n\n\t\tACCEPT!";
		}
	}

	if (check == false || check_top == false)
	{
		system("color f4");
		cout << "\n\n\t\tREJECT!";
	}

	gotoxy(5, 10);
	cout << "Press any key..";
	system("pause>0");

	system("cls");
	system("color f3");
	int f;
	cout << "\n\n\tDo you want to continue?\n\t\t1-YES\n\t\t2-NO\n\t";
	cin >> f;
	if (f == 1)
	{
		curr_node = dpda[init_state];
		check_str();
	}
	else return;

}

void Write_DPDA()
{
	fstream output;
	output.open("DPDA.txt", ios::out);	//Make File
	if (!output)
	{
		cerr << "File can not open." << endl;
		exit(1);
	}

	cout << "\n\tEnter the number of states:\t";
	cin >> num_states;
	output << num_states << endl;

	dpda = new dpdaState[num_states];

	cout << "\n\tEnter the number of symbols: (Except for Lambda)\t";
	cin >> num_sym;
	output << num_sym << endl;

	cout << "\n\tEnter symbols: (press Enter button after each symbol)\n\n";
	sym[0] = 'l';
	for (int i = 1; i < num_sym + 1; i++)
	{
		cout << "\t\t symbol " << i << " : ";
		cin >> sym[i];
		output << sym[i] << endl;
	}

	system("cls");

	cout << "\n\tDefine transition rules:\t( )\n\n\t\t";
	for (int i = 0; i < num_states; i++)
	{
		system("cls");

		cout << "How many transitions does q" << i << " have? ";
		cin >> dpda[i].trans_num;
		output << dpda[i].trans_num << endl;

		for (int j = 0; j <dpda[i].trans_num; j++)
		{
			cout << "\nq" << i << " --> q";
			cin >> dpda[i].trans_state[j];
			output << dpda[i].trans_state[j] << endl;
			cout << "\n\tinput alphabet:  ";
			cin >> dpda[i].trans_symbol[j];
			output << dpda[i].trans_symbol[j] << endl;
			cout << "\n\tpop:  ";
			cin >> dpda[i].stackpop[j];
			output << dpda[i].stackpop[j] << endl;
			cout << "\n\tpush:  ";
			cin >> dpda[i].stackpush[j];
			output << dpda[i].stackpush[j] << endl;
		}
	}

	system("cls");

	cout << "\n\tEnter the number of accepted states:\t";
	cin >> num_acc;
	output << num_acc << endl;

	acc_state = new int[num_acc];

	cout << "\n\tEnter the accepted states:(press Enter button after each state)\n";
	for (int i = 0; i < num_acc; i++)
	{
		int n;
		cout << "\n\t\tq";
		cin >> n;
		output << n << endl;
		acc_state[i] = n;
	}


	cout << "\n\tWhich state is initial state?\tq";
	cin >> init_state;
	output << init_state << endl;
	curr_node = dpda[init_state];
	output.close();	
	system("cls");
	cout << "\n\n\n\t\tDPDA SAVED!\n\n";
	system("pause>0");
}

void Read_DPDA()
{
	ifstream inputFile("DPDA.txt", ios::in | ios::beg);
	if (!inputFile)	//check for file opening
	{
		cerr << "some thing wrong during opening file!" << endl;
		_getche();
		exit(1);
	}

	inputFile >> num_states;

	dpda = new dpdaState[num_states];

	inputFile >> num_sym;

	sym[0] = 'l';
	for (int i = 1; i < num_sym + 1; i++)
	{
		inputFile >> sym[i];
	}

	for (int i = 0; i < num_states; i++)
	{
		inputFile >> dpda[i].trans_num;

		for (int j = 0; j <dpda[i].trans_num; j++)
		{
			inputFile >> dpda[i].trans_state[j];
			inputFile >> dpda[i].trans_symbol[j];
			inputFile >> dpda[i].stackpop[j];
			inputFile >> dpda[i].stackpush[j];
		}
	}

	inputFile >> num_acc;

	acc_state = new int[num_acc];

	for (int i = 0; i < num_acc; i++)
	{
		int n;
		inputFile >> n;
		acc_state[i] = n;
	}


	inputFile >> init_state;
	curr_node = dpda[init_state];
	system("cls");
	cout << "\n\n\n\t\tDPDA LOADED!\n\n";
	system("pause>0");
}

int main()
{

	while (true)
	{
		system("cls");
		system("color f3");

		int menu;
		cout << "\n\t\t\t1- Enter a string\n\t\t\t2- Enter a new DPDA\n\t\t\t3- Exit\n\t\t\t";
		cin >> menu;
		system("cls");

		switch (menu)
		{
		case 1:
			Read_DPDA();
			check_str();
			break;
		case 2:
			Write_DPDA();
			break;
		case 3:
			exit(1);
		}
	}
}
