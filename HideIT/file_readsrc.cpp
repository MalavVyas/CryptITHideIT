#include<iostream>
#include <fstream>
#include "file_read.h"
#include <vector>
#include<sys/stat.h>
#include "base64.h"
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

void wrapper_class::base64decodewrap(char* encoded_filename, char* decoded_filename) {

	ifstream in2(encoded_filename);
	ofstream out2(decoded_filename, ios::binary);

	in2.seekg(0, ios::end);
	int iSize = in2.tellg();
	in2.seekg(0, ios::beg);

	char* pBuff = new char[iSize];
	//memset(pBuff, 0, sizeof(pBuff));
	in2.read(pBuff, iSize);
	int read_bytes = in2.gcount();

	//todo
	//Properly manage base64 padding length
	int alloc_len = Base64decode_len(pBuff);
	char* decoded_str = (char*)malloc(alloc_len);
	alloc_len = alloc_len - 2;
	Base64decode(decoded_str, pBuff);
	out2.write(decoded_str, alloc_len);
	cout << "Decoding Done" << endl;
	cout << "Alloc len is " << alloc_len << endl;


	free(decoded_str);
	delete[] pBuff;

	in2.close();
	out2.close();
}
void wrapper_class::base64encodewrap(char* filename, char* encoded_filename) {
	ifstream in(filename, ios::binary);
	ofstream out(encoded_filename);
	in.seekg(0, ios::end);
	int iSize = in.tellg();
	in.seekg(0, ios::beg);

	char* pBuff = new char[iSize];
	//memset(pBuff, 0, sizeof(pBuff));
	in.read(pBuff, iSize);
	int read_bytes = in.gcount();
	cout << "Read bytes are" << read_bytes << endl;
	int alloc_len = Base64encode_len(read_bytes);
	char* encoded_str = (char*)malloc(alloc_len);
	Base64encode(encoded_str, pBuff, read_bytes);
	out.write(encoded_str, alloc_len);
	cout << "Encoding Done" << endl;
	free(encoded_str);
	delete[] pBuff;

	in.close();
	out.close();

	system("pause");
}