//
//  main.cpp
//  dns_server
//
//  Created by 孙佶 on 15-3-11.
//  Copyright (c) 2015年 sunji. All rights reserved.
//


#include <iostream>
#include "server.h"
#include <unistd.h>
#include <getopt.h>
bool iflog = false;
int main(int argc, char * argv[])
{
    const char * upperIP = "10.3.9.5";
    const char * host_path = "/Users/sunji/360云盘/大三下/DNS/dnsrelay.txt";
    for(int i = 0; i <= argc-1; i++){
        std::cout << argv[i] << std::endl;
    }
    char oc,ec;
    while((oc = getopt(argc, argv, "ds:p:")) != -1)
    {
        switch(oc)
        {
            case 'd': //参数p
                iflog = true;
                break;
            case 's': //参数s
                printf("Port:%s\n\n", optarg);
                upperIP = optarg;
                break;
            case 'p': //参数p
                printf("Thread Num:%s\n\n", optarg);
                host_path = optarg;
                break;
            case '?': //如果设置了未定义的参数，则返回这边的错误信息
                ec = (char)optopt;
                printf("未定义的参数:%c\r\n", ec);
                exit(0);
                break;
            case ':': //缺少选项
                printf("缺少选项参数\r\n");
                exit(0);
                break;
        }
    }
    Server *m_server = new Server(upperIP, host_path);
    PRINT(("%s %s",upperIP,host_path));
    m_server->run();
    return 0;
}

