//
//  server.cpp
//  dns_server
//
//  Created by 孙佶 on 15-3-11.
//  Copyright (c) 2015年 sunji. All rights reserved.
//


#include "server.h"
#define received_response() query.getQR() == 1
#define INSERT(p1,p2) insert(std::map<std::string, uint32_t>::value_type(p1, p2))
#define IP adr_inet.sin_addr.s_addr

Server::Server(const char * upperIP, const char * host_path)
{
    upperServer.sin_len = sizeof(sockaddr_in);
    upperServer.sin_family = AF_INET;
    upperServer.sin_port = htons(53);
    inet_aton(upperIP, &upperServer.sin_addr);
    init_host(host_path);
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(53);
    //生成套接字
    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {//ipv4 数据报文套接字，返回一个文件描述符
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        return;
    }
    unsigned value = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));//设置套接字选项，如果*val非0，那么重复使用bind的地址
    if (bind(socketfd, (struct sockaddr *)&servAddr, sizeof(servAddr))) {
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        return;
    }
    std::cout<<"server running on port:"<<53<<std::endl;
}

void Server::init_host(const char *filename)
{
    std::ifstream fstream(filename);
    char hostStr[128];
    //transfer the ip-dns table in the file to the map Host
    while (fstream.getline(hostStr, sizeof(hostStr)))
    {
        std::stringstream sstream;
        std::string ipAddr;
        std::string name;
        sstream << hostStr;
        sstream >> ipAddr;
        sstream >> name;
    
        struct sockaddr_in adr_inet;
        memset(&adr_inet, 0, sizeof(adr_inet));
        inet_aton(ipAddr.c_str(), &adr_inet.sin_addr);
        
        hosts_table.INSERT(name, IP);
        PRINT(("%s %s\n", ipAddr.c_str(), name.c_str()));
    }
}

void Server::handle_packet(char buff[], int packet_size)
{
    socklen_t len = sizeof(struct sockaddr_in);
    const char * temp = buff;
    query.decode_header(temp);
    if(received_response())
    {
        struct INFO Packet_info;
        Packet_info = id_table.Search(query.getID());
        if (Packet_info.Id == -1)
        {
            PRINT(("这是一个超时的回答，我们已经不需要\n"));
        }
        else
        {
            struct sockaddr_in back = Packet_info.Client;
            query.setID(Packet_info.Id);
            PRINT(("答案数：%d\n这是一个响应包\n回复给客户：%s:%hu\n%d号包完成响应\n", query.getAnCount(), inet_ntoa(back.sin_addr), ntohs(back.sin_port), query.getID()));
            sendto(socketfd, buff, packet_size, 0, (struct sockaddr *)&back, len);
        }
    }
    else
    {
        query.decode(buff, packet_size);
        bool notfound = false;
        bool ifforbidden = false;
        response.Search_answer(query, notfound, ifforbidden, hosts_table);
        
        PRINT(("notfound = %d\n", notfound));
        
        if(notfound)
        {
            id_table.insert(query.getID(), clientAddr);
            PRINT(("%d, %d\n%s\n", query.getID(), id_table.get_order(), query.to_string().c_str()));
            query.setID((id_table.get_order()+4095)%4096);
            PRINT(("%s\n", query.to_string().c_str()));
            sendto(socketfd, buff, packet_size, 0, (struct sockaddr *)&upperServer, len);
        }
        
        else if(ifforbidden)
        {
            response.formalize(notfound, ifforbidden, query);
            PRINT(("域名不存在\n"));
            memset(buff, 0, sizeof(&*buff));
            packet_size = response.encode(buff);
            PRINT(("%s\n", response.to_string().c_str()));
            sendto(socketfd, buff, packet_size, 0, (struct sockaddr *)&clientAddr, len);
        }
        else
        {
            response.formalize(notfound, ifforbidden, query);
            memset(buff, 0, sizeof(&*buff));
            packet_size = response.encode(buff);
            PRINT(("%s\n", response.to_string().c_str()));
            sendto(socketfd, buff, packet_size, 0, (struct sockaddr *)&clientAddr, len);
        }
    }
}

void Server::run()
{
    while (1)
    {
        char buff[512];
        PRINT(("\n"));
        socklen_t len = sizeof(struct sockaddr_in);
        int packet_size = (int)recvfrom(socketfd, buff, sizeof(buff), 0, (struct sockaddr *)&clientAddr, &len);
        if (packet_size <= 0) continue;
        PRINT(("接受从%s:%hu发来的包\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port)));
        handle_packet(buff, packet_size);
    }
}