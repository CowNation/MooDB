#include <iostream>
#include <vector>
#include <string>
#include "Data/MooDatabase.hpp"

template <class T>
void PrintVector( std::vector <T> vec )
{
    std::cout << std::endl;
    for ( int i = 0; i < vec.size( ); i++ )
    {
        std::cout << vec[ i ];
        if ( i != vec.size( ) - 1 )
        {
            std::cout << ", ";
        }
    }
}

int main( )
{
    Database db( "test.txt" );
    Table tab( "Patients", { "Name", "Gender", "Age", "DOB", "Height", "Weight" } );
    tab.Insert( { "Funanya Radomir", "Female", "3", "6/1/2015", "3 ft 1 in", "29.5 lbs" } );
    tab.Insert( { "Reanna Shekhar", "Female", "15", "10/31/2003", "5 ft 6 in", "130 lbs" } );
    db.InsertTable( tab );
    Table hp( "Medications", { "Name", "Stock", "RequiresID" } );
    hp.Insert( { "Ibuprofin", "423,243", "False" } );
    hp.Insert( { "Psuedophedrine", "345,223", "True" } );
    hp.Insert( { "Tylenol", "452,334", "False" } );
    db.InsertTable( hp );
    db.Print( );
    db.SaveData( );

    std::vector <std::vector <std::string> > r = hp.GetRowsWithValue( "RequiresID", "True" );

    for (const std::vector <std::string> & i : r)
    {
        PrintVector( i );
        std::cout << std::endl;
    }

    Database datab( "test.txt" );
    datab.LoadTables( );

    Table tb = datab.GetTable( "Patients" );
    tb.Print( );
    PrintVector( tb.GetColumn( "Name" ));
    PrintVector( tb.GetRow( 0 ));
}