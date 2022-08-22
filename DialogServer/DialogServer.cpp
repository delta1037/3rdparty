//
// Created by delta1037 on 2022/8/18.
//
#include <cstdio>
#include <cstdarg>

#include "DialogServer.h"


DialogServer::DialogServer() {
    init_status = false;
    // 初始化socket之类的
    m_dialog_idx = 0;

    // 初始化SOCKET
    m_dialog_server_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    SOCKADDR_IN ser_addr;

    ser_addr.sin_family=AF_INET;
    ser_addr.sin_port=htons(SERVER_PORT);
    ser_addr.sin_addr.S_un.S_addr=inet_addr(SERVER_IP);

    // 绑定
    if(bind(m_dialog_server_socket,(SOCKADDR*)&ser_addr, sizeof(ser_addr)) != 0){
        dialog_log("server bind socket error")
        return;
    }

    // 开启监听
    if(listen(m_dialog_server_socket, 5) != 0){
        dialog_log("server listen error")
        return;
    }

    // 启动线程接收连接 dialog_thread

    // 初始化完毕
    init_status = true;
}

DialogServer::~DialogServer() {
    // 关闭socket
    closesocket(m_dialog_server_socket);
}

void DialogServer::dialog_thread() {
    // 启动一个线程，用来监听客户端发过来的内容
    // accept之后开始互相tong
    // 发生了新的连接时，就把菜单发过去
}

void DialogServer::register_menu(const std::string &name, MENU_FUNC menu_func) {
    for(const auto &it : m_dialog_map){
        if(it.second.name == name){
            printf("name %s already exists", name.c_str());
            return;
        }
    }

    // 构造新的菜单
    DialogRegisterData dialog_data;
    dialog_data.name = name;
    dialog_data.func = menu_func;
    m_dialog_map.emplace_back(m_dialog_idx++, dialog_data);
}

void DialogServer::dialog_print(const char *format, ...) {
    char buffer[MAX_BUFFER]{0};

    va_list va_args;
    va_start(va_args, format);
    std::vsnprintf(buffer, MAX_BUFFER, format, va_args);
    va_end(va_args);

    m_dialog_buffer += buffer;
    m_dialog_buffer += '\n';
}
