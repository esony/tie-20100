#include <string>

#ifndef TESTMATERIALDIR
#define TESTMATERIALDIR "material"
#endif

const std::string TESTDIR=std::string(TESTMATERIALDIR);

#include <iostream>
#include <sstream>
#include <fstream>

#include "datastructure.hh"
#include "utility.hh"

const std::string READ = "R";
const std::string LINES = "B";
const std::string ROUTE = "N";
const std::string NAME = "P";
const std::string EMPTY = "E";
const std::string COUNT = "C";
const std::string QUIT = "Q";

const std::string PROMPT = "> ";

const std::string LISENSSI = "Sisältää Tampereen kaupungin aineistoa:";

void printLicence(const std::string& directory)
{
    std::ifstream ifs(directory + "/agency.txt");
    std::string line;
    // First line is header line
    std::getline(ifs, line );

    // Line with licence information
    std::getline(ifs, line );

    std::istringstream line_stream(line);
    std::string tmp;

    std::cout << LISENSSI << std::endl;

    std::getline(line_stream, tmp, ',' );
    std::cout << tmp << ", ";
    std::getline(line_stream, tmp, ',' );
    std::cout << tmp <<"," << std::endl;
    std::getline(line_stream, tmp, ',' );
    std::cout << tmp << std::endl;;
}

int main()
{
    set_limits();
    Datastructure DS;
    std::string command = " ";

    std::string directory;
    std::string stop_id1;
    std::string stop_id2;
    std::string time;

    do{
        std::cout << PROMPT;
        std::cin >> command;

        if( command == READ )
        {
            std::cin >> directory;
            DS.loadData(TESTDIR + "/" + directory );
        }
        else if( command == LINES )
        {
            std::cin >> stop_id1;
            DS.routesFromStop(stop_id1);
        }
        else if( command == NAME )
        {
            std::cin >> stop_id1;
            DS.stopName( stop_id1 );
        }
        else if( command == ROUTE )
        {
            std::cin >> time >> stop_id1 >> stop_id2;
            DS.routeSearch(time, stop_id1, stop_id2 );
        }
        else if( command == EMPTY )
        {
            DS.empty();
        }
        else if( command == COUNT )
        {
            DS.count();
        }
        else
        {
            continue;
        }
    }while( command != QUIT );

    printLicence(TESTDIR + "/full");

    return EXIT_SUCCESS;

}
