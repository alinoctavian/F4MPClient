//
// Created by again on 8/21/2020.
//

#ifndef F4MPCLIENT_HELPERS_H
#define F4MPCLIENT_HELPERS_H

#include <iostream>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void _Error(std::string msg){

    SetConsoleTextAttribute(hConsole, 04);
    std::cout << msg << std::endl;
    SetConsoleTextAttribute(hConsole, 15);
}

void _Log(std::string msg){
    std::cout << msg << std::endl;
}

#define LOG(msg) _Log(msg);
#define LOGERR(msg) _Error(msg);

#endif //F4MPCLIENT_HELPERS_H