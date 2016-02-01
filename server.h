//
//  server.h
//  dns_server
//
//  Created by 孙佶 on 15-3-11.
//  Copyright (c) 2015年 sunji. All rights reserved.
//


#ifndef __dns_server__server__
#define __dns_server__server__

#include <iostream>
#include "query.h"
#include "response.h"
#include "table.h"
#include <netinet/in.h>
#include <fstream>
#include <sstream>
#include <arpa/inet.h>
#include <sys/socket.h>
class Server
{
public:
    Server(const char * upperIP, const char * host_path);
    ~Server() {};
    void run();
private:
    void handle_packet(char *buff, int packet_size);
    void init_host(const char *);
    struct sockaddr_in clientAddr;
    struct sockaddr_in upperServer;
    int socketfd;
    Query query;
    Response response;
    Table id_table;
protected:
    std::map<std::string, uint32_t> hosts_table;
};

#endif /* defined(__dns_server__server__) */
