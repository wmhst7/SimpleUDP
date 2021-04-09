#include "defs.h"
#include <map>

// using namespace std;

std::map<std::string, ClientAddr> client_map;

std::vector<std::string> split(const std::string &str, const std::string &pattern)
{
    char *strc = new char[strlen(str.c_str()) + 1];
    strcpy(strc, str.c_str()); //string转换成C-string
    std::vector<std::string> res;
    char *temp = strtok(strc, pattern.c_str());
    while (temp != NULL)
    {
        res.push_back(std::string(temp));
        temp = strtok(NULL, pattern.c_str());
    }
    delete[] strc;
    return res;
}

void get_reply(char* buf){
    std::string res = std::string(buf);
    memset(buf, 0, BUFF_LEN);
    // sprintf(buf, "I have recieved %d bytes data!\n", count);
    std::vector<std::string> seg = split(res, " ");
    // for(auto i: seg){
    //     debug("%s\n", i.c_str());
    // }
    if(seg.size() == 0){
        warn("Receive nothing\n");
        memset(buf, 0, BUFF_LEN);
        return;
    }
    if(seg[0] == "client_info"){
        // 信息登记
        if(seg.size() < 4){
            memset(buf, 0, BUFF_LEN);
            error("Wrong client info\n");
            sprintf(buf, "client_info_error");
            return;
        }else{
            ClientAddr caddr;
            sscanf(seg[2].c_str(), "%x", &caddr.ip);
            caddr.port = stoi(seg[3]);
            client_map[seg[1]] = caddr;
            sprintf(buf, "client_info_correct");
            info("Enroll client: %s\n", seg[1].c_str());
            return;
        }
    }
    else if (client_map.find(seg[0]) != client_map.end()){
        // Client 存在
        ClientAddr cad = client_map[seg[0]];
        
        if((seg.size() == 2) && (seg[1] == std::string("get_clients_info"))){
            // 信息查询
            // sprintf(buf, "Clients info:\n");
            std::string info;
            for(auto &iter: client_map){
                // debug("Client:%s, IP:%u, Port:%d\n", iter.first.c_str(), iter.second.ip, iter.second.port);
                sprintf(buf, "Client:%s, IP:%u, Port:%d; ", iter.first.c_str(), iter.second.ip, iter.second.port);
                info += std::string(buf);
            }
            sprintf(buf, "%s",info.c_str());
            return;
        }else{
            // 随意
            sprintf(buf, "Send %lu data success!", res.size());
            return;
        }
        
    }
}

void handle_udp_msg(int fd)
{
    char buf[BUFF_LEN];  //接收缓冲区，1024字节
    socklen_t len;
    int count;
    struct sockaddr_in client_addr;  //client_addr用于记录发送方的地址信息
    while(1){
        memset(buf, 0, BUFF_LEN);
        len = sizeof(client_addr);

        //recvfrom是拥塞函数，没有数据就一直拥塞
        count = recvfrom(fd, buf, BUFF_LEN, 0, (struct sockaddr*)&client_addr, &len);
        if(count == -1){
            error("Server recvfrom fail\n");
            return;
        }
        trace("receive:%s\n", buf);  //打印client发过来的信息
        
        get_reply(buf);

        // trace("send:%s\n",buf);  //打印自己发送的信息给
        sendto(fd, buf, BUFF_LEN, 0, (struct sockaddr*)&client_addr, len);  //发送信息给client，注意使用了client_addr结构体指针

    }
}


int main(int argc, char* argv[])
{
    // Initial
    int server_fd, ret;
    struct sockaddr_in ser_addr;
    server_fd = socket(AF_INET, SOCK_DGRAM, 0); //AF_INET:IPV4;SOCK_DGRAM:UDP
    if(server_fd < 0)
    {
        error("Inital socket fail\n");
        return -1;
    }

    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
    ser_addr.sin_port = htons(SERVER_PORT);  //端口号，需要网络序转换

    ret = bind(server_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    if(ret < 0)
    {
        error("Server bind fail\n");
        return -1;
    }

    handle_udp_msg(server_fd);

    close(server_fd);
    return 0;
}