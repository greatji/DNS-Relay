//
//  message.cpp
//  dns_server
//
//  Created by 孙佶 on 15-3-11.
//  Copyright (c) 2015年 sunji. All rights reserved.
//


#include "message.h"

using namespace std;

void encode_address(char *addr, char *&buff)
{
    string address(addr);//copy
    int pos, current = 0;
    while ((pos = (int)address.find('.')) != -1)
    {
        address.erase(0, pos+1);
        *buff++ = pos;
        memcpy(buff, addr+current, pos);
        buff += pos;
        current += pos+1;
    }
    *buff++ = address.size();
    memcpy(buff, addr+current, address.size());
    buff += address.size();
    *buff++ = 0;
}

//encode the message to the buffer
void Message::encode_header(char *&buff)
{
    headerptr = (MHeader *)buff;
    headerptr->hId = htons(ID);
    headerptr->ra = Ra;
    headerptr->z = Zero;
    headerptr->rcode = Rcode;
    headerptr->qr = Qr;
    headerptr->opcode = Opcode;
    headerptr->aa = Aa;
    headerptr->tc = Tc;
    headerptr->rd = Rd;
    headerptr->queryCount = htons(QdCount);
    headerptr->answCount = htons(AnCount);
    headerptr->authCount = htons(NsCount);
    headerptr->addiCount = htons(ArCount);
    buff += sizeof(MHeader);
}

//encode the questions of message to the buffer
void Message::encode_questions(char *&buff)
{
    //encode each question
    for (int i = 0; i < getQdCount(); i ++)
    {
        MQuestion question = questions_set[i];
        encode_address(question.qName, buff);
        
        uint16_t nQType = htons(question.qType);
        memcpy(buff, &nQType, sizeof(uint16_t));
        buff+=sizeof(uint16_t);
        
        uint16_t nQClass = htons(question.qClass);
        memcpy(buff, &nQClass, sizeof(uint16_t));
        buff+=sizeof(uint16_t);
    }
}

//encode the answers of the message to the buffer
void Message::encode_answers(char *&buff)
{
    for (int i = 0; i < getAnCount(); i ++)
    {
        MResource resource = answers_set[i];
        encode_address(resource.rName, buff);
        
        uint16_t nRType = htons(resource.rType);
        memcpy(buff, &nRType, sizeof(uint16_t));
        buff+=sizeof(uint16_t);
        
        uint16_t nRClass = htons(resource.rClass);
        memcpy(buff, &nRClass, sizeof(uint16_t));
        buff+=sizeof(uint16_t);
        
        uint32_t nTTL = htonl(resource.rTTL);
        memcpy(buff, &nTTL, sizeof(uint32_t));
        buff+=sizeof(uint32_t);
        
        uint16_t nRDLen = htons(resource.rdLength);
        memcpy(buff, &nRDLen, sizeof(uint16_t));
        buff+=sizeof(uint16_t);
        
        if (MT_A == resource.rType)
        {
            memcpy(buff,&resource.rData, sizeof(uint32_t));
            buff+=sizeof(uint32_t);
        }
    }
}

//decode the message header from the buffer
void Message::decode_header(const char *&buff)
{
    
    headerptr = (MHeader *)buff;
    ID = ntohs(headerptr->hId);
    Ra = (uint)headerptr->ra;
    Zero = (uint)headerptr->z;
    Rcode = (uint)headerptr->rcode;
    Qr = (uint)headerptr->qr;
    Opcode = (uint)headerptr->opcode;
    Aa = (uint)headerptr->aa;
    Tc = (uint)headerptr->tc;
    Rd = (uint)headerptr->rd;
    QdCount = ntohs(headerptr->queryCount);
    AnCount = ntohs(headerptr->answCount);
    NsCount = ntohs(headerptr->authCount);
    ArCount = ntohs(headerptr->addiCount);
    buff+= sizeof(MHeader);
}

//decode the questions of the message from the buffer
void Message::decode_questions(const char *&buff)
{
    questions_set.clear();
    for (int i=0; i < getQdCount(); i ++)
    {
        MQuestion question = {0};
        while (1)
        {
            uint len = *buff++;
            if (len==0) break;
            if (strlen(question.qName)!=0) strcat(question.qName, ".");
            memcpy(question.qName+strlen(question.qName), buff, len);
            buff+=len;
        }
        question.qType = ntohs(*((uint16_t *)buff));
        buff+=sizeof(uint16_t);
        question.qClass = ntohs(*((uint16_t *)buff));
        buff+=sizeof(uint16_t);
        questions_set.push_back(question);
    }
}