//
//  message.h
//  dns_server
//
//  Created by 孙佶 on 15-3-11.
//  Copyright (c) 2015年 sunji. All rights reserved.
//


#ifndef __dns_server__message__
#define __dns_server__message__

#define PRINT(s) if(iflog) {printf s;}

#include <iostream>
#include <vector>
#include <sstream>

class Message
{
protected:
    
    enum MQueryType //the resource info which query
    {
        MT_A=0x01, //ipv4 address
        MT_NS=0x02, //dns name server
        MT_CNAME=0x05, //host name
        MT_SOA=0x06, //the first auth server
        MT_PTR=0x0C, //
        MT_HINFO=0x0D, //host info
        MT_MINFO=0x0E, //main info
        MT_MX=0x0F, //mail exchange
        MT_TXT=0x10, //text
        MT_UINFO=0x64, //user info
        MT_UID=0x65, //user id
        MT_GID=0x66, //group id
        MT_ANY=0xFF //any type of data
    };

    struct MHeader
    {
        uint16_t hId;
        unsigned rd:1;
        unsigned tc:1;
        unsigned aa:1;
        unsigned opcode:4;
        unsigned qr:1;
        unsigned rcode:4;
        unsigned z:3;
        unsigned ra:1;
        uint16_t queryCount;
        uint16_t answCount;
        uint16_t authCount;
        uint16_t addiCount;
    };
    
    struct MQuestion
    {
        char qName[64];
        uint16_t qType;
        uint16_t qClass;
    };
    
    struct MResource
    {
        char rName[64];
        uint16_t rType;
        uint16_t rClass;
        uint32_t rTTL;
        uint16_t rdLength;
        uint32_t rData;
    };
    
    uint ID;
    uint Ra;
    uint Zero;
    uint Rcode;
    uint Qr;
    uint Opcode;
    uint Aa;
    uint Tc;
    uint Rd;
    uint QdCount;
    uint AnCount;
    uint NsCount;
    uint ArCount;
    
    std::vector<MQuestion> questions_set;//存放问题的容器
    std::vector<MResource> answers_set;//存放答案的容器
    
    enum MResponseCode //the status code server response
    {
        MC_NO_ERROR=0, //no error
        MC_FORMAT_ERROR=1, //format error
        MC_SERVER_ERROR=2, //server error
        MC_NAME_ERROR=3, //name error
        MC_NOT_SUPPORT=4, //server not support
        MC_REFUSE=5 //server refuse
    };
    MHeader *headerptr;
    
    void decode_questions(const char*&);
    void encode_header(char *&);
    void encode_questions(char *&);
    void encode_answers(char *&);
    void decode_response(const char *&);
public:
    Message() {};
    ~Message() {};
    void setID(uint id) { headerptr->hId = htons(id); return; }
    uint getID() const { return ID; }
    uint getQR() const { return Qr; }
    uint getQdCount() const { return QdCount; }
    uint getAnCount() const { return AnCount; }
    uint getNsCount() const { return NsCount; }
    uint getArCount() const { return ArCount; }
    void decode_header(const char*&);
    std::vector<MQuestion> getQuestions() const { return questions_set; };
};
extern bool iflog;
#endif /* defined(__dns_server__message__) */
