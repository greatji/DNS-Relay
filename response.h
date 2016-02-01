//
//  response.h
//  dns_server
//
//  Created by 孙佶 on 15-3-11.
//  Copyright (c) 2015年 sunji. All rights reserved.
//


#ifndef __dns_server__response__
#define __dns_server__response__

#include "message.h"
#include "query.h"
#include <Map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Response : public Message
{
public:
    Response() {};
    ~Response() {};
    int encode(char *buff);
    std::string to_string();
    void formalize(bool notfound, bool ifforbidden, Query &query);
    void Search_answer(Query &query, bool &notfound, bool &ifforbidden, std::map<std::string, uint32_t> hosts_table);
    int get_answers_size();
};

#endif
