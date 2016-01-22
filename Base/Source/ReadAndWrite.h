#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

class CReadAndWrite
{
public:
	CReadAndWrite();
	~CReadAndWrite();

	void Init(std::string FileName);
	int getStringsize(int list_pos);

	char getTextCharacters(int list_pos, int string_pos);

	std::string getLines();

	void Read(std::string filename);
	void Write(std::string filename, int size);
protected:
	std::string Array[500];
};

