#include<iostream>
using namespace std;
class wrapper_class {
public:
	void main_file();
	string encryption(string in_text);
	string decryption(string in_text);
};

class encClass {
	string text;
	int key;
	string encText = "";
public:
	void set(string in_text, int in_key);
	string get();
};

class decClass {
	string text;
	int key;
	string decText = "";
public:
	void set(string in_text, int in_key);
	string get();
};