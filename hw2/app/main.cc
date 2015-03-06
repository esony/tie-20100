//
//  main.cpp
//  HW2_candy
//
//  Created by Kilamo Terhi on 26/02/15.
//  Copyright (c) 2015 Kilamo Terhi. All rights reserved.
//



#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include "datastructure.hh"
#include "utility.hh"

const string READ = "R";
const string ADD = "A";
const string REMOVE = "D";
const string FIND = "F";
const string EMPTY = "E";
const string COUNT = "C";
const string QUIT = "Q";

const string PRINT = "P";
const string PROMPT = "> ";
const string DEFAULT_FILE = "varasto.txt";

//void ReadFile( Datastructure& DS );

int main() {
    Datastructure DS;
    string command = " ";
    
    ReadFile( DEFAULT_FILE, DS);
    
    string name;
    string loc;
    string ID;
    string file;
    char nextchar;
    unsigned int amnt = 0;

    do{
        cout << PROMPT;
        cin >> command;
        
        if( command == READ )
        {
            nextchar = cin.peek();
            if(nextchar == '\n' || nextchar == '\r')
            {
                file = DEFAULT_FILE;
            }
            else
            {
                cin >> file;
            }            
            //DS.empty(); 
            ReadFile( file, DS );
        }
        else if( command == ADD )
        {
            cin >> ID >> loc >> amnt;
            cin.get();
            getline(cin, name);
            DS.add(ID, loc, amnt, name);
        }
        else if( command == REMOVE )
        {
            cin >> ID >> amnt;
            DS.substract( ID, amnt );
        }
        else if( command == FIND )
        {
            cin >> ID;
            DS.find( ID );
        }
        else if( command == EMPTY )
        {
            DS.empty();
        }
        else if( command == COUNT )
        {
            cout << DS.count() << " erilaista makeista." << endl;
        }
        else if( command == PRINT )
        {
            nextchar = cin.peek();
            if(nextchar == '\n' || nextchar == '\r')
            {
                file = DEFAULT_FILE;
            }
            else
            {
                cin >> file;
            }            
            DS.save_to_file( file );
        }
        else
        {
            continue;
        }
    }while( command != QUIT );
    
    DS.save_to_file( DEFAULT_FILE );
    return EXIT_SUCCESS;
}
