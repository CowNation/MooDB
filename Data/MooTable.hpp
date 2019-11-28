#include <iostream>
#include <vector>
#include <string>

class Database;

class Table
{
private:
    bool StringVectorHas( std::vector <std::string> vec, std::string substr )
    {
        for ( int i = 0; i < vec.size( ); i++ )
        {
            if ( vec[ i ].find( substr ) != std::string::npos )
            {
                return true;
            }
        }
        return false;
    }

public:
    std::vector <std::string> Columns; // A horizonal vector containing the number of columns and their names
    std::vector <std::vector <std::string> > Data; // A vertical vector (list) of horizontal vectors containing string data

    Database *parentDB = nullptr;
    std::string Name;

    void RefreshFile( );

    void Insert( std::vector <std::string> data )
    {
        if ( data.size( ) != Columns.size( ))
        {
            throw "Inserted data does not match columns";
        }
        if ( StringVectorHas( data, "{" ) || StringVectorHas( data, "}" ) || StringVectorHas( data, "" ))
        {
            throw "Inserted data contains a forbidden character";
        }
        Data.push_back( data );
    }

    void Print( )
    {
        std::vector <int> ColumnOffsets;

        for ( int i = 0; i < Columns.size( ); i++ )
        {
            ColumnOffsets.push_back( -1 );
        }

        for ( int i = 0; i < Data.size( ); i++ )
        {
            for ( int j = 0; j < Data[ i ].size( ); j++ )
            {
                if (( Data[ i ][ j ].length( ) - Columns[ j ].length( ) > ColumnOffsets[ j ] ||
                      ColumnOffsets[ j ] == -1 ) && Data[ i ][ j ].length( ) > Columns[ j ].length( ))
                {
                    ColumnOffsets[ j ] = Data[ i ][ j ].length( ) - Columns[ j ].length( );
                }
            } // we are looping horizontally
        } // we are looping vertically

        std::cout << std::endl << Name << ":" << std::endl;

        std::string Headers;
        std::string Seperator;

        Headers += "| ";
        for ( int i = 0; i < Columns.size( ); i++ )
        {
            Headers += Columns[ i ];
            if ( ColumnOffsets[ i ] != -1 )
            {
                for ( int j = 0; j < ColumnOffsets[ i ]; j++ )
                {
                    Headers += " ";
                }
            }
            Headers += " | ";
        }

        Seperator += "+";
        for ( int i = 0; i < Headers.length( ) - 3; i++ )
        {
            if ( Headers.at( i + 1 ) == '|' )
            {
                Seperator += "+";
            }
            else
            {
                Seperator += "-";
            }
        }

        Seperator += "+";

        std::cout << Seperator << "\n" << Headers << "\n" << Seperator;

        for ( int i = 0; i < Data.size( ); i++ )
        {
            std::cout << "\n| ";
            for ( int j = 0; j < Data[ i ].size( ); j++ )
            {
                std::cout << Data[ i ][ j ];

                if ( ColumnOffsets[ j ] != -1 )
                {
                    if ( Data[ i ][ j ].length( ) < ( ColumnOffsets[ j ] + Columns[ j ].length( )))
                    {
                        for ( int p = 0;
                              p < ( ColumnOffsets[ j ] + Columns[ j ].length( )) - Data[ i ][ j ].length( ); p++ )
                        {
                            std::cout << " ";
                        }
                    }
                }
                else
                {
                    for ( int p = 0; p < Columns[ j ].length( ) - Data[ i ][ j ].length( ); p++ )
                    {
                        std::cout << " ";
                    }
                }

                std::cout << " | ";
            } // we are looping horizontally
        } // we are looping vertically

        std::cout << std::endl << Seperator << std::endl;
    }

    Table( std::string tableName, std::vector <std::string> columns )
    {
        Name = tableName;
        Columns = columns;
    }

    std::vector <std::string> GetRow( int Index )
    {
        if ( Index > Data.size( ) || Index < 0 )
        {
            throw "Row at index " + std::to_string( Index ) + " is out of bounds";
        }
        return Data[ Index ];
    }

    std::vector <std::vector <std::string> > GetRowsWithValue( std::string nColumn, std::string ColumnValue )
    {
        std::vector <std::vector <std::string> > ret;
        std::vector <std::string> vColumn = GetColumn( nColumn );
        for ( int i = 0; i < vColumn.size( ); i++ )
        {
            if ( vColumn[ i ] == ColumnValue )
            {
                ret.push_back( GetRow( i ));
            }
        }
        return ret;
    } // will return a vector of vectors of strings where that row's column value equals the 'ColumnValue'

    std::vector <std::string> GetColumn( std::string ColumnName )
    {
        std::vector <std::string> ret;
        int ColumnIndex = -1;
        for ( int i = 0; i < Columns.size( ); i++ )
        {
            if ( Columns[ i ] == ColumnName )
            {
                ColumnIndex = i;
                break;
            }
        }

        if ( ColumnIndex == -1 )
        {
            throw "Could not find column '" + ColumnName + "'";
        }

        for ( int i = 0; i < Data.size( ); i++ )
        {
            ret.push_back( Data[ i ][ ColumnIndex ] );
        }

        return ret;
    }
};