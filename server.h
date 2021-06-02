#ifndef SERVER_H
#define SERVER_H

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>

const int SIZE_CLIENTS = 6;

SOCKET Connections[100];
std::string nameClients[SIZE_CLIENTS];
int readyClients[SIZE_CLIENTS] = {2,-1,-1,-1,-1,-1};
int countConnector = 0;

int getStatus (std::string msg)
{   
    int status = -1;
    std::string statusString = "";
    for (int i = 0; i < 4; i++)
    {
        if (msg[i] == ':') break;
        if (msg[i] == '0' || msg[i] == '1' || msg[i] == '2' || msg[i] == '3' || msg[i] == '4' || msg[i] == '5' || msg[i] == '6' || msg[i] == '7' || msg[i] == '8' || msg[i] == '9')
        statusString += msg[i];
    }
    if (statusString != "") status = stoi(statusString); 
    return status;
}

std::string getMessage (std::string msg)
{
    std::string nonStatus;
    bool firstDblDot = false;
    for (int i = 0; i < msg.length(); i++)
    {
        if (!firstDblDot && msg[i] == ':')
        {
            firstDblDot = true;
            continue;
        }
        if (firstDblDot) nonStatus += msg[i];
    }
    return nonStatus;
}

#endif // SERVER_H