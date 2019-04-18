#ifndef cppCowConfig
#define cppCowConfig

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

class CowConfig {
private:
	std::ofstream WriteConfig;
	std::fstream ReadConfig;
	bool FirstLine = true, FirstRead = true;
	std::string FileName;
  std::vector< std::string >Lines;
  ///////////////////
  void RemoveSubStr(std::string substr, std::string& str){
    size_t pos = std::string::npos;
		// Search for the substring in string in a loop untill nothing is found
		while ((pos = str.find(substr)) != std::string::npos)
		{
			// If found then erase it from string
			str.erase(pos, substr.length());
		}
  }
  ///////////////////
  void ReadAllLines(){
    std::string str;
    while (std::getline(ReadConfig, str)) {
      Lines.push_back(str);
    }
  }
  ///////////////////
  int FindElement(std::string Section, std::string offsetText){
    Section = "[" + Section + "]";
    bool SectionFound;
    for (int i = 0; i < Lines.size(); i++){
      if (Lines[i] == Section)
        SectionFound = true;
      else if (SectionFound && Lines[i].find("[") != std::string::npos && Lines[i].find("]") != std::string::npos)
        break;
      if (SectionFound && Lines[i].find(offsetText) != std::string::npos)
        return i;
    }
    return -1;
  }
  ///////////////////
public:
	CowConfig() {}
	CowConfig(std::string fileName){
    OpenFile(fileName);
	}
	~CowConfig() {
		if (ReadConfig.is_open())
			ReadConfig.close();
		if (WriteConfig.is_open())
			WriteConfig.close();
	}
	///////////////////
  std::vector< std::string >GetLines(){
    if (FirstRead)
      ReadAllLines();
    return Lines;
  }
  ///////////////////
	bool OpenFile(std::string fileName) {
		FileName = fileName;

		ReadConfig.open(fileName);
		return (ReadConfig.is_open());
	} // Will open file with specified filename and will return whether the file is open
	//////////////////
	void WriteLine(std::string offsetText, char* valToWrite) {
		if (ReadConfig.is_open())
			ReadConfig.close(); //Close fstream file before writing to the file

		if (FirstLine) {
			WriteConfig.open(FileName);
			FirstLine = false;
		}

		WriteConfig << offsetText << valToWrite << std::endl;

		ReadConfig.open(FileName);
	}
	template <class c>
	void WriteLine(std::string offsetText, c valToWrite) {
		if (ReadConfig.is_open())
			ReadConfig.close(); //Close fstream file before writing to the file

		if (FirstLine) {
			WriteConfig.open(FileName);
			FirstLine = false;
		}

		WriteConfig << offsetText << valToWrite << std::endl;

		ReadConfig.open(FileName);
	}
  void Section(std::string SectionText) {
    SectionText = "[" + SectionText + "]";
    WriteLine("", SectionText);
  }
	//////////////////
	int iRead(std::string Section, std::string offsetText) {
    if (FirstRead)
      ReadAllLines();
    int LineFound = FindElement(Section, offsetText);
    if (LineFound == -1)
      return 0;
    else{
      try{
        std::string temp = Lines[LineFound];
        RemoveSubStr(offsetText, temp);
        return stoi(temp);
      }
      catch (...){
        return 0;
      }
    }
	}
	float fRead(std::string Section, std::string offsetText) {
    if (FirstRead)
      ReadAllLines();

    int LineFound = FindElement(Section, offsetText);
    if (LineFound == -1)
      return 0.0f;
    else{
      try{
        std::string temp = Lines[LineFound];
        RemoveSubStr(offsetText, temp);
        return stof(temp);
      }
      catch (...){
        return 0.0f;
      }
    }
	}
	std::string sRead(std::string Section, std::string offsetText) {
    if (FirstRead)
      ReadAllLines();

    int LineFound = FindElement(Section, offsetText);
    if (LineFound == -1)
      return "";
    else{
      try{
        std::string temp = Lines[LineFound];
        RemoveSubStr(offsetText, temp);
        return temp;
      }
      catch (...){
        return "";
      }
    }
	}
	bool bRead(std::string Section, std::string offsetText) {
    if (FirstRead)
      ReadAllLines();

    int LineFound = FindElement(Section, offsetText);
    if (LineFound == -1)
      return false;
    else{
      try{
        std::string temp = Lines[LineFound];
        RemoveSubStr(offsetText, temp);
        return temp == "1";
      }
      catch (...){
        return false;
      }
    }
	}
	//////////////////
	void ClearFile() {
		std::ofstream Clear;
		Clear.open(FileName);
		Clear.close();
	}
};

void v2Test(){
  CowConfig cfg("test.txt");
  cfg.Section("Strings");
  cfg.WriteLine("string_thing: ", (std::string)"T");
  cfg.WriteLine("str_thing: ", (std::string)"L");
  cfg.Section("Integers");
  cfg.WriteLine("thing_stuff: ", 3);
  std::cout << cfg.sRead("Strings", "str_thing: ") << std::endl << cfg.iRead("Integers", "thing_stuff: ") << std::endl << cfg.iRead("Integers", "thing_stuff: ") << std::endl << cfg.sRead("Strings", "string_thing: ");
}

#endif
