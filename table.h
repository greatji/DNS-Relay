//
//  table.h
//  dns_server
//
//  Created by 孙佶 on 15-3-31.
//  Copyright (c) 2015年 sunji. All rights reserved.
//

#ifndef _dns_server_table_h
#define _dns_server_table_h

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <Map>
#include "message.h"
#include "time.h"
struct INFO
{
    uint Id;
    long time_stamp;
    struct sockaddr_in Client;
};

class Table
{
private:
    struct INFO client_info[4096];
    uint order;
public:
    Table(){order = 0;}
    ~Table(){};
    struct INFO Search(uint id);
    void insert(uint id, struct sockaddr_in client);
    uint get_order(){return order;}
};

#endif
