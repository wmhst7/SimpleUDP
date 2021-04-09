# SimpleUDP

A simple implementation of UDP.

网络编程技术作业

设计并实现一个UDP客户端/服务器，客户端向服务器登记自己发送数据包的IP地址和端口号，同时可以向服务器查询其他用户的IP地址和端口号。要求协议设计和实现可靠、一致。

要求提交说明文档，说明所设计的协议，以及处理方式；要求提交客户端、服务器的源代码；要求用C/C++实现。

## 协议设计

* 纯字符串流，单个信息大小不超过512Byte。

* 设计
    * 信息登记
        * Client信息登记：`client_info <用户名> <IP地址> <端口号>`
            * 用户名不含空格
            * IP地址格式为十进制数
            * 端口号为十进制数字
        * Server确认Client登记正确：`client_info_correct`
        * Server返回Client登记失败：`client_info_error`
    * 发送消息
        * Client发送消息：`<用户名> <消息>`
        * Server确认收到：`Server get %d Bytes`
    * 信息查询
        * Client信息查询：`<用户名> get_clients_info`
        * Server返回Client信息：`{<用户名>: }`
    * 其他

## 处理流程



### Server

1. 初始化
2. 监听ANY
3. 如果有新Client加入，记录信息。
    1. 检查各字段格式，若正确则返回检查正确消息`client_info_correct`
4. 如果有询问请求，返回所有Client的信息。

### Client

1. 初始化
2. 提示输入本机用户名、IP、Port，完成后发向Server。
3. 若收到确认消息，则此时可以发送任意信息
4. 若发送`get_clients_info`，Server应返回所有Client的信息。