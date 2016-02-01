//
//  table.cpp
//  dns_server
//
//  Created by 孙佶 on 15-3-31.
//  Copyright (c) 2015年 sunji. All rights reserved.
//

#include "table.h"

struct INFO Table::Search(uint id)
{
    time_t sec = time( 0 );
    struct INFO fail_find;
    fail_find.Id = -1;
    PRINT(("id: %d, 当前时间: %ld, 记录时间: %ld\n", id, sec, client_info[id].time_stamp));
    if((sec - client_info[id].time_stamp) <= 20)
    {
        return client_info[id];
    }
    return fail_find;
}

void Table::insert(uint id, struct sockaddr_in client)
{
    time_t sec = time( 0 );
    PRINT(("id: %d, 当前时间: %ld\n", order, sec));
    client_info[order].Id = id;
    client_info[order].time_stamp = sec;
    client_info[order].Client = client;
    order = ( order + 1 ) % 4096;
}