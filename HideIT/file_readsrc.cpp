#include<iostream>
#include <fstream>
#include "file_read.h"
#include <vector>
#include<sys/stat.h>
using namespace std;

const int BUFFER_SIZE = 1024;


void wrapper_class::main_file() {
	struct stat results;
	string chs;
	char c;
	int bno = 0;
	ifstream inputfile("C:\\Users\\Malav\\Desktop\\pass.txt", ios::binary);
	ofstream outputfile("C:\\Users\\Malav\\Desktop\\pass_2.txt", ios::binary);
	if (inputfile.good()) {
		//vector<char> buffer(BUFFER_SIZE + 1, 0);		
		while (1) {
			char* buffer = (char*)malloc(BUFFER_SIZE + 1);
			bno++;
			inputfile.read(buffer, BUFFER_SIZE);
			//			cout << strlen(buffer);
			streamsize s = ((inputfile) ? BUFFER_SIZE : inputfile.gcount());
			buffer[s] = 0;

			outputfile.write(buffer, s);
			if (!inputfile) cout << "block number " << bno << "read characters " << s << "Buffer is " << buffer << endl;
			if (!inputfile) break;
			free(buffer);
		}
	}

	//inputfile.open("C:\\Users\\Malav\\Desktop\\file10.vhd");
	//cout << inputfile.failbit<< endl;
	////chs = (char*)malloc(10);
	//while (inputfile.get(c)) {
	//	chs.push_back(c);
	//}
	//inputfile.close();
	//cout << chs.c_str();
	//wrapper_class encryptionObj;
	//string encryptedText = encryptionObj.encryption(chs);
	//cout << endl << "Encrypted Text is : " << encryptedText << endl;
	//ofstream outputfile;
	//outputfile.open("C:\\Users\\Malav\\Desktop\\file10_enc.vhd", ios::binary);
	//outputfile << encryptedText;
	//outputfile.close();


	//ifstream encryptedFile;
	//encryptedFile.open("C:\\Users\\Malav\\Desktop\\file10_enc.vhd", ios::binary);
	//cout << encryptedFile.failbit << endl;
	//string encryptedStr;
	//while (encryptedFile.get(c)) {
	//	encryptedStr.push_back(c);
	//}
	//encryptedFile.close();
	//cout << encryptedStr.c_str();
	//wrapper_class decryptionObj;
	//string decryptedText = decryptionObj.decryption(encryptedStr);
	//cout << endl << "Decrypted Text is : " << decryptedText << endl;
	//encryptedFile.close();

	//ofstream decryptedFile;
	//decryptedFile.open("C:\\Users\\Malav\\Desktop\\file10_dec.vhd", ios::binary);
	//cout << decryptedFile.failbit << endl;
	//decryptedFile << decryptedText;
	//decryptedFile.close();
}

void encClass::set(string in_text, int in_key) {
	text = in_text;
	key = in_key;
}

string encClass::get() {
	int len = text.length();
	cout << "len is : " << len << endl;
	for (int i = 0; i < len; i++) {
		int textInt = int(text[i]);
		int newInt = textInt + key;
		encText.push_back((char)newInt);
	}
	return encText;
}

void decClass::set(string in_text, int in_key) {
	text = in_text;
	key = in_key;
}

string decClass::get() {
	int len = text.length();
	cout << "len is : " << len << endl;
	for (int i = 0; i < len; i++) {
		int textInt = int(text[i]);
		int newInt = textInt - key;
		decText.push_back((char)newInt);
	}
	return decText;
}


string wrapper_class::encryption(string in_text) {
	encClass encObj;
	int in_key = 5;
	encObj.set(in_text, in_key);
	string out_text = encObj.get();
	return out_text;
}

string wrapper_class::decryption(string in_text) {
	decClass decObj;
	int in_key = 5;
	decObj.set(in_text, in_key);
	string out_text = decObj.get();
	return out_text;
}