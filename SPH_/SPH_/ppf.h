///		***
///
///				ppf.h - declaration of the ppf class, variables and methods - Tom
///
///		***
#ifndef PPF_H
#define PPF_H

#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;
class ppf{
public:
	ppf();
	~ppf();
   /// *** Loading prints
	void loading();
	void loading(string s);
	void loading(const char* p, string s);
	void loadingComp();
	void loadingFailed();
	void loaded(const char* p, int i, int col);
	void loaded(const char *p, string s, int col);

	/// *** Compiling Prints
	void compiling(string s);
	void compiling(const char* c, string s); 
	void compilingNoEnd(const char* c, string s);

	/// *** Linking prints
	void linking();
	void linking(string s);
	void linkingComp();
	/// *** Error prints
	void error(const char* p, string s, int col);
	void error();
	void start();
	void finish();


	/// *** Class specific prints
	void mlPrint(string s, int i, int col);
	void mlPrint(string s, string t, int i, int j,  int col);
	void mlPrint(string s, string t, int i, int col);
private:
};
#endif