/*
 * Client
 * client.h
 * ver 0.20
 */
/*
https://www.geeksforgeeks.org/socket-programming-cc/
https://www.bogotobogo.com/cplusplus/sockets_server_client.php
https://www.youtube.com/watch?v=0Zr_0Jy8mWE     TCP
https://www.youtube.com/watch?v=uIanSvWou1M     UDP

*/
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>

#define AddressString   "127.0.0.1"
#define PORT            8080

using str = std::string;
using str_vector = std::vector<str>;

class Client {
    public:
        Client();
        ~Client();
        int  Connect();
        int  Disconnect();
        int  PutMessage(const str text, const uint16_t flags);
        bool GetMessage(str& text);
        uint16_t GetClientStatus();
        void Serving();
    private:
        struct cMsg {
            int         id;
            uint16_t    status;
            str         cMessage;
        };
        int message_id;
        std::vector<cMsg> MessagesIn;
        std::vector<cMsg> MessagesOut;
        struct sockaddr_in Server_addr;
        struct sockaddr_in Client_addr;
        int ServerSocket = 0;
        int ValRead = 0;
        int ServerPort = 8080;
        uint16_t ClStatus;
        bool isConnected;
        char buffer[1024];
};

#define ERR_OPEN_SOCKET     0x0001
#define ERR_CREATE_SOCKET   0x0002
#define ERR_SOCKET_OPTIONS  0x0004
#define ERR_BINDING         0x0008
#define ERR_LISTEN          0x0010
#define ERR_ACCEPTING       0x0020
#define ERR_CONNECTING      0x0040
#define ERR_INVALID_ADDRESS 0x0080

#endif     //end _CLIENT_H_
