///		***
///
///		ppf.cpp - implementation of the ppf (pretty print format) class - Tom
///		This class will handle printing out to the console in different colours so that looking through 
///		the console and being able to differentiate between different calls will be easier!
///		Only some colours are actually supported by the console window and this will only work on Windows OS machines.
///
///		***

#include "ppf.h"

//7 => default
//0 => black
//1 => blue
//2 => green
//3 => aqua
//4 => red
//5 => purple
//6 => yellow
//7 => light gray
//8 => gray
//9 => light blue
//10 => light green
//11 => light aqua
//12 => light red
//13 => light purple
//14 => light yellow
//15 => white

ppf::ppf(){}

/******************************************************************************
							Loading Prints
******************************************************************************/
void ppf::loaded(const char* p, int i, int col)
{
	const char* isLoaded = "[LOADED] ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col), 
	cout << isLoaded << i << p << endl;
	finish();
}

void ppf::loaded(const char *p, string s, int col)
{
	const char* isLoaded = "[LOADED] ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col), 
	cout << isLoaded << p << s << endl;
	finish();
}

void ppf::loading()
{
	const char* l = "[LOADING] ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2), 
	cout << l << endl;
	finish();
}

void ppf::loading(string s)
{
	const char* l = "[LOADING] ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2), 
	cout << l ;
	finish();
}

void ppf::loading(const char* p,string s)
{
	const char* l = "[LOADING] ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2), 
	cout << l  ;
	finish();
	cout << p << s;
}

void ppf::loadingComp()
{
	const char* l = "[COMPLETE]";
	cout << " . . . " ;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2), 
	cout << l << endl;
	finish();
}

void ppf::loadingFailed()
{
	const char* l = "[FAILED]";
	cout << " . . . " ;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4), 
	cout << l << endl;
	finish();
}

/******************************************************************************
								Compiling Prints
******************************************************************************/

void ppf::compiling(string s)
{
	const char* l = "[COMPILING] ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2), 
	cout << s ;
	finish();
}

void ppf::compiling(const char* c, string s)
{
	const char* l = "[COMPILING] ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2), 
	cout << l ;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7), 
	finish();
}

void ppf::compilingNoEnd(const char* c, string s)
	{
	const char* l = "[COMPILING] ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2), 
	cout << l ;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7),
	cout << s;
	finish();
}

/******************************************************************************
								Linking Prints
******************************************************************************/

void ppf::linking()
{
	const char* l = "[LINKING PROGRAM] ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2), 
	cout << "\t" << l ;
	finish();
}

void ppf::linking(string s)
{
	const char* l = "[LINKING PROGRAM] ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2), 
	cout << l ;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7), 
	cout << s;
	finish();
}

void ppf::linkingComp()
{
	const char* l = "[COMPLETE]";
	cout << " . . . " ;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2),
		cout<< l << endl;
	finish();
}


/******************************************************************************
								Error Prints
******************************************************************************/
void ppf::error(const char* p, string s, int col)
{
	const char* erroneous = "[ERROR] ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col), 
	cout << erroneous << p << s << endl;
	finish();
}

/******************************************************************************
							Class specific Prints
******************************************************************************/
void ppf::mlPrint(string s, int i, int col)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col), 
		cout << s << i  ;
	finish();
}

void ppf::mlPrint(string s, string t, int i, int j,  int col)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col), 
		cout << s << i << t << j << endl;
	finish();
}
void ppf::mlPrint(string s, string t, int i, int col)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col), 
		cout << s << i << t << endl;
	finish();
}





/******************************************************************************
						Set console back to original
******************************************************************************/
void ppf::finish()
{
	//call this after so it sets back to default
	SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 15);
	
}