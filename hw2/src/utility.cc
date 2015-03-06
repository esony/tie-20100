#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <sstream>
using std::istringstream;

#include "utility.hh"

//const string file = "varasto.txt";


using std::string;

void ReadFile( const string& file, Datastructure& DS )
{
    ifstream file_stream( file );
    if( !file_stream )
    {
        file_stream.close();
        cout << "Tiedostoa ei löydy." << endl;
        return;
    }

    DS.empty();

    string line;
    string ID;
    string loc;
    unsigned int amnt;
    string name;
    while( std::getline( file_stream, line ))
    {
        istringstream line_stream( line );
        getline(line_stream, ID, ';');
        line_stream >> amnt;
        line_stream.get();
        getline(line_stream, loc, ';');
        getline(line_stream, name);
        DS.add( ID, loc, amnt, name );
    }
    file_stream.close();
}
