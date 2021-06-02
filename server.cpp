#include "server.h"

int Counter = 0;

void ClientHandler(int index)
{
    int msg_size;
    while (true)
    {
        recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
        
        char *msgTemp = new char[msg_size + 1];
        msgTemp[msg_size] = '\0';
        
        int recieve = recv(Connections[index], msgTemp, msg_size, NULL);
        if (recieve <= 0)
        {
            closesocket(Connections[index]);
            delete[] msgTemp;
            break;
        }

        std::string msgString = msgTemp;
        delete[] msgTemp;
        int msgStatus = getStatus(msgString);
        msgString = getMessage(msgString);

        std::cout << "Clinet[" << index << "] ( " << "STATUS: " << msgStatus << "; MESSAGE: " << msgString << " )" << std::endl;

        std::string msgIndex = nameClients[index] + " << ";
        std::string msg;

        switch (msgStatus) //Перевод из string в int
        {
            case 7: //Запрос о старте игры
                msg = std::to_string(msgStatus) + ':' + msgString;
                msg_size = msg.size();
                for (int i = 0; i < Counter; i++)
                {
                    send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
                    send(Connections[i], msg.c_str(), msg_size, NULL);
                }
                break;
            case 8: //Запрос клиента о готовности
                msg = std::to_string(msgStatus) + ':' + msgString;
                msg_size = msg.size();
                for (int i = 0; i < Counter; i++)
                {
                    send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
                    send(Connections[i], msg.c_str(), msg_size, NULL);
                }
                break;
            case 9: //Запрос клиента на обновление всех клиентов
                nameClients[index] = msgString;
                msg = std::to_string(msgStatus) + ':';
                for (int i = 0; i < Counter; i++)
                {
                    msg += nameClients[i] + ":";
                }
                msg_size = msg.size();
                for (int i = 0; i < Counter; i++)
                {
                    send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
                    send(Connections[i], msg.c_str(), msg_size, NULL);
                }
                break;
            default: //Типо ЧАТ
                msg = msgIndex + msgString; 
                msg_size = msg.size();
                for (int i = 0; i < Counter; i++)
                {
                    if(i == index) continue;
            
                    send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
                    send(Connections[i], msg.c_str(), msg_size, NULL);
                }
                break;
        }
    }
}

int main(int argc, char* argv[])
{
    WSAData wsaData;
    WORD DLLersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLersion, &wsaData) != 0)
    {
        std::cout << "Error" << std::endl;
        WSACleanup();
        exit(1);
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);

    std::cout << "Server is running ..." << std::endl;

    SOCKET Connection;
    for (int i = 0; i < 100; i++)
    {
        Connection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

        if(Connection == 0) std::cout << "Error #2" << std::endl;
        else 
        {
            Connections[i] = Connection;
            nameClients[i] = "Player_" + std::to_string(i);
            Counter++;

            std::string msg = "9:";
            for (int i = 0; i < Counter; i++)
            {
                msg += nameClients[i] + ":";
            }
            
            int msg_size = msg.size();
            
            for (int i = 0; i < Counter; i++)
            {
                //if(i == index) continue;
            
                send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
                send(Connections[i], msg.c_str(), msg_size, NULL);
            }
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
        }
    }
    system("pause");
    return 0;
}