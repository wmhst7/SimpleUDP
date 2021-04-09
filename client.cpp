#include "defs.h"
using namespace std;

// my ip: 192.168.0.102
// zydembp:104
char username[MAX_NAME];
ClientAddr caddr;

char buf[BUFF_LEN];

void run(int fd, struct sockaddr* dst, struct sockaddr* src)
{
    socklen_t len;
    // 发送任意信息
    while(1)
    {
        printf("send something:");
        scanf("%s", buf);
        len = sizeof(*dst);
        trace("send:%s\n",buf);  //打印自己发送的信息
        sendto(fd, buf, BUFF_LEN, 0, dst, len);
        memset(buf, 0, BUFF_LEN);
        recvfrom(fd, buf, BUFF_LEN, 0, (struct sockaddr*)&src, &len);  //接收来自server的信息
        trace("receive:%s\n",buf);
    }
}


int main(int argc, char* argv[])
{
    int client_fd;
    struct sockaddr_in ser_addr;
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //注意网络序转换
    ser_addr.sin_port = htons(SERVER_PORT);       //注意网络序转换
    
    struct sockaddr_in cli_addr;

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_fd < 0)
    {
        error("create socket fail!\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));

    // 设置信息
    while (1)
    {
        printf("Plese type in your user name:");
        scanf("%s", username);
        printf("Plese type in your ip address:");
        scanf("%u", &(caddr.ip));
        printf("Plese type in your port:");
        scanf("%d", &(caddr.port));

        

        socklen_t len;

        cli_addr.sin_family = AF_INET;
        cli_addr.sin_addr.s_addr = htonl(caddr.ip);
        cli_addr.sin_port = htons(caddr.port);

        sprintf(buf, "client_info %s %u %d\n", username, caddr.ip, caddr.port);
        bind(client_fd, (struct sockaddr *)&cli_addr, sizeof(cli_addr));

        trace("send:%s\n", buf); //打印自己发送的信息

        len = sizeof(ser_addr);

        sendto(client_fd, buf, BUFF_LEN, 0, (struct sockaddr *)&ser_addr, len);
        memset(buf, 0, BUFF_LEN);
        recvfrom(client_fd, buf, BUFF_LEN, 0, (struct sockaddr *)&cli_addr, &len); //接收来自server的信息
        trace("receive:%s\n", buf);
        string res = string(buf);
        if (res ==  string(INFO_CORRECT))
        {
            break;
        }
        info("Client info upload error, please try again~\n");
    }

    run(client_fd, (struct sockaddr *)&ser_addr, (struct sockaddr *)&cli_addr);

    close(client_fd);

    return 0;
}