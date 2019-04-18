#include <iostream>
#include <vector>
#include <string>
#include "MooTable.hpp"
#include "CowConfig.hpp"

class Database{
  private:
  std::string FindStrBetween(std::string& str, char firstL, char lastL, bool Remove){
    std::string ret;
    bool between = false;
    for (int i = 0; i < str.length(); i++){
      if (between && str[i] == lastL){
        break;
      }
      
      if (str[i] == firstL && between == false){
        between = true;
      }
      else if (between){
        ret += str[i];
      }
    }
    str.erase(0, ret.length() + 2);
    return ret;
  }
  std::vector< std::string > UnseperateCommas(std::string str){
    std::vector< std::string > ret;
    std::string temp;
    for (int i = 0; i < str.length(); i++){
      if (str[i] != '')
        temp += str[i];
      else{
        ret.push_back(temp);
        temp = "";
      }
    }
    if (ret.size() == 0)
      return {"he1", "He2", "he3", "he4"};
    return ret;
  }
  CowConfig cfg;
  public:
  std::vector< Table > Tables;

  Database(std::string filePath){
    cfg.OpenFile(filePath);
  }
  void Print(){
    for (int i = 0; i < Tables.size(); i++)
      Tables[i].Print();
  }
  void InsertTable(Table& table){
    Tables.push_back(table);
    table.parentDB = this;
  }
  void SaveData(){
    /*
      Saved data form:
      1: {Table1Name{Column1Name,Column2Name}{OneOne,OneTwo}{TwoOne,TwoTwo}}
    */
    for (int i = 0; i < Tables.size(); i++){
      std::string tableLine = "{" + Tables[i].Name + "}{";
      for (int p = 0; p < Tables[i].Columns.size(); p++){
        tableLine += Tables[i].Columns[p] + "";
      }
      tableLine += "}{";
      for (int p = 0; p < Tables[i].Data.size(); p++){
        for (int q = 0; q < Tables[i].Data[p].size(); q++){
          tableLine += Tables[i].Data[p][q] + "";
        } // looping horizontally
        tableLine += "}";
        if (p != Tables[i].Data.size() - 1)
          tableLine += "{";
      }// looping vertically
      cfg.WriteLine("", tableLine);
    }
  }
  void LoadTables(){
    std::vector< std::string >Lines = cfg.GetLines();
    for (int i = 0; i < Lines.size(); i++) {
      std::string Name = FindStrBetween(Lines[i], '{', '}', true);
      Table tab(Name, UnseperateCommas(FindStrBetween(Lines[i], '{', '}', true)));
      while (Lines[i].length() > 0){
        std::vector< std::string > vc = UnseperateCommas(FindStrBetween(Lines[i], '{', '}', true));
        tab.Insert(vc);
      }

      Tables.push_back(tab);
    } // loop through each individual line
  }
  Table& GetTable(std::string TableName){
    for (int i = 0; i < Tables.size(); i++){
      if (Tables[i].Name == TableName)
        return Tables[i];
    }
    throw "Could not find table '" + TableName + "'";
  }
};
