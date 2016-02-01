//
//  query.cpp
//  dns_server
//
//  Created by 孙佶 on 15-3-11.
//  Copyright (c) 2015年 sunji. All rights reserved.
//


#include "query.h"

void Query::decode(const char *buff, const int size)
{
    if (sizeof(MHeader) > size) return;
    decode_header(buff);
    decode_questions(buff);
}

//content to be printed
std::string Query::to_string()
{
    std::stringstream stream;
    stream<<"Query:"<<std::endl;
    for (std::vector<MQuestion>::iterator iter=questions_set.begin(); iter!=questions_set.end(); iter++)
    {
        MQuestion question = *iter;
        stream<<getID()<<" "<<question.qName<<" "<<question.qType<<std::endl;
    }
    return stream.str();
}

int Query::get_questions_size()
{
    return (int)questions_set.size();
}