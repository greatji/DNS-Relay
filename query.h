//
//  query.h
//  dns_server
//
//  Created by 孙佶 on 15-3-11.
//  Copyright (c) 2015年 sunji. All rights reserved.
//

#ifndef __dns_server__query__
#define __dns_server__query__

#include "message.h"

class Query : public Message
{
public:
    Query() {};
    ~Query() {};
    void decode(const char*buff, const int size);
    std::string to_string();
    int get_questions_size();
};

#endif /* defined(__dns_server__query__) */
