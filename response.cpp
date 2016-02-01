//
//  response.cpp
//  dns_server
//
//  Created by 孙佶 on 15-3-11.
//  Copyright (c) 2015年 sunji. All rights reserved.
//

#include "response.h"
#define find_this_answer() hIter != hosts_table.end()
using namespace std;

void Response::Search_answer(Query &query, bool &notfound, bool &ifforbidden, std::map<std::string, uint32_t> hosts_table)
{
    questions_set.clear();
    answers_set.clear();
    
    std::vector<Response::MQuestion> questions = query.getQuestions();
    for (std::vector<Response::MQuestion>::iterator qIter = questions.begin(); qIter != questions.end(); ++qIter)
    {
        Response::MQuestion question = *qIter;
        Response::MResource resource;
        notfound = true;
        if(question.qType == Message::MT_A)
        {//we only need to search the IPV4 query
            PRINT(("查找中...\n"));
            
            std::map<std::string, uint32_t>::iterator hIter = hosts_table.find(question.qName);
            
            if (find_this_answer())
            {
                notfound = false;
                PRINT(("找到了\n"));
                strcpy(resource.rName, question.qName);
                resource.rType = question.qType;
                resource.rClass = question.qClass;
                resource.rTTL = 10;
                resource.rdLength = sizeof(uint32_t);
                if(hIter->second == 0)
                    ifforbidden = true;
                resource.rData = hIter->second;
                answers_set.push_back(resource);
            }
            else break;
            questions_set.push_back(question);
        }
    }
}

void Response::formalize(bool notfound, bool ifforbidden, Query &query)
{
    Ra = 1;
    Zero = 0;
    Qr = 1;
    Opcode = 0;
    Aa = 0;
    Tc = 0;
    Rd = 1;

    ID = query.getID();
    if (notfound)
    {
        Rcode = 2;
    }
    else if(ifforbidden)
    {
        Rcode = 3;
    }
    else
    {
        Rcode = 0;
    }
    QdCount = query.get_questions_size();
    AnCount = (int)answers_set.size();
    NsCount = 0;
    ArCount = 0;
}

int Response::encode(char *buff)
{
    char *start = buff;
    encode_header(buff);
    encode_questions(buff);
    encode_answers(buff);
    int len = (int)(buff-start);
    return len;
}

//content to be printed
std::string Response::to_string()
{
    std::stringstream stream;
    stream << "Response:" << std::endl;
    stream << "answer_num" << getAnCount() << std::endl;
    stream << "question_num" << getQdCount() <<std::endl;
    for (std::vector<MResource>::iterator iter=answers_set.begin(); iter!=answers_set.end(); iter++)
    {
        MResource resource = *iter;
        struct in_addr addr;
        addr.s_addr = resource.rData;
        char *rIp = inet_ntoa(addr);
        stream<<rIp<<" "<<resource.rType<<std::endl;
    }
    return stream.str();
}

int Response::get_answers_size()
{
    return (int)answers_set.size();
}
