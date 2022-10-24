#include <iostream>
#ifndef __linux__
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#else
typedef SOCKET int;
#endif

/* 注意与服务端保持一致 */
#define SOCKET_BUFFER_SIZE 20480
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 4321

#define FORMAT_PREFIX "<%s, %d> "
#define FILENAME ( __builtin_strrchr(__FILE__, '\\') ? __builtin_strrchr(__FILE__, '\\') + 1 : __FILE__ )
#define dialog_info(format, args...) printf(FORMAT_PREFIX#format, FILENAME , __LINE__, ##args);printf("\n");
#ifdef DEBUG
#define dialog_debug(format, args...) printf(FORMAT_PREFIX#format, FILENAME, __FUNCTION__ , __LINE__, ##args);printf("\n");
#else
#define dialog_debug(format, args...)
#endif
// 发送指令与接收指令
void call_server(){
    SOCKET cli_socket;
    SOCKADDR_IN ser_addr;
    char socket_buffer[SOCKET_BUFFER_SIZE];

#ifndef __linux__
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,0),&wsa);
#endif

    // 创建客户端描述符
    cli_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    // 设置连接协议和服务端地址
    ser_addr.sin_family=AF_INET;
    ser_addr.sin_port=htons(SERVER_PORT);
    ser_addr.sin_addr.S_un.S_addr=inet_addr(SERVER_IP);

    // 连接服务端
    dialog_debug("start connect server")
    if(connect(cli_socket,(SOCKADDR*)&ser_addr, sizeof(ser_addr)) != 0){
        dialog_info("connect server %s:%d error", SERVER_IP, SERVER_PORT)
    }
    dialog_info("connect server %s:%d success", SERVER_IP, SERVER_PORT)

    while(true){
        // 清理空间接收消息
        memset(socket_buffer, 0, SOCKET_BUFFER_SIZE);
        recv(cli_socket, socket_buffer, SOCKET_BUFFER_SIZE, 0);
        std::string recv = socket_buffer;
        if(recv == "exit") {
            break;
        }else{
            std::cout << recv << std::endl;
        }

        // 根据接收到的内容发送指令
        memset(socket_buffer, 0, SOCKET_BUFFER_SIZE);
        printf("> ");
        gets(socket_buffer);
        send(cli_socket, socket_buffer, SOCKET_BUFFER_SIZE, 0);
    }

    // 客户端关闭连接
    closesocket(cli_socket);

#ifndef __linux__
    WSACleanup();
#endif
}

int main() {
    call_server();
    return 0;
}
