#include <iostream>
#include <vector>
#include <string>
#include "MooTable.hpp"
#include "CowConfig.hpp"

class Database
{

private:

    static std::string FindStrBetween( std::string &str, char firstL, char lastL, bool Remove )
    {
        std::string ret;

        bool between = false;

        for (char i : str)
        {
            if ( between && i == lastL )
            {
                break;
            }

            if ( i == firstL && !between )
            {
                between = true;
            }
            else if ( between )
            {
                ret += i;
            }
        }

        str.erase( 0, ret.length( ) + 2 );

        return ret;
    }

    static std::vector <std::string> UnseperateCommas( std::string str )
    {
        std::vector <std::string> ret;

        std::string temp;

        for (char i : str)
        {
            if ( i != '' )
            {
                temp += i;
            }
            else
            {
                ret.push_back( temp );
                temp = "";
            }
        }

        return ret;
    }

    CowConfig cfg;

public:

    std::vector <Table> Tables;

    explicit Database( std::string filePath )
    {
        cfg.OpenFile( filePath );
    }

    void Print( )
    {
        for (Table &Table : Tables)
        {
            Table.Print( );
        }
    }

    void InsertTable( Table &table )
    {
        Tables.push_back( table );
        table.parentDB = this;
    }

    void SaveData( )
    {
        /*
        Saved data format:
        1: {Table1Name{Column1Name,Column2Name}{OneOne,OneTwo}{TwoOne,TwoTwo}}
        */
        for (Table &Table : Tables)
        {
            std::string tableLine = "{" + Table.Name + "}{";

            for (const std::string &Column : Table.Columns)
            {
                tableLine += Column + "";
            }

            tableLine += "}{";

            for ( int p = 0; p < Table.Data.size( ); p++ )
            {
                for (const std::string &q : Table.Data[ p ])
                {
                    tableLine += q + "";
                } // looping horizontally

                tableLine += "}";

                if ( p != Table.Data.size( ) - 1 )
                {
                    tableLine += "{";
                }

            }// looping vertically

            cfg.WriteLine( "", tableLine );
        }
    }

    void LoadTables( )
    {
        std::vector <std::string> Lines = cfg.GetLines( );

        for (std::string & Line : Lines)
        {
            std::string Name = FindStrBetween( Line, '{', '}', true );

            Table tab( Name, UnseperateCommas( FindStrBetween( Line, '{', '}', true )));

            while ( Line.length( ) > 0 )
            {
                std::vector <std::string> vc = UnseperateCommas( FindStrBetween( Line, '{', '}', true ));
                tab.Insert( vc );
            }

            Tables.push_back( tab );
        } // loop through each individual line
    }

    Table &GetTable( std::string TableName )
    {
        for (Table &Table : Tables)
        {
            if ( Table.Name == TableName )
            {
                return Table;
            }
        }

        throw "Could not find table '" + TableName + "'";
    }
};

void Table::RefreshFile( )
{
    parentDB->SaveData( );
}