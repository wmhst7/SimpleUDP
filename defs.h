#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>

#define BUFF_LEN 512
#define MAX_NAME 64
#define SERVER_IP (INADDR_ANY) // 192.168.0.102 ?
#define SERVER_PORT (8888)


// Color
#define info_pr_fmt(fmt) "\x1b[34m[INFO] " fmt "\x1b[0m"
#define debug_pr_fmt(fmt) "\x1b[32m[DEBUG] " fmt "\x1b[0m"
#define trace_pr_fmt(fmt) "\x1b[90m[TRACE] " fmt "\x1b[0m"
#define warn_pr_fmt(fmt) "\x1b[93m[WARN] " fmt "\x1b[0m"
#define error_pr_fmt(fmt) "\x1b[31m[ERROR] " fmt "\x1b[0m"

#define info(fmt, ...) printf(info_pr_fmt(fmt), ##__VA_ARGS__)
#define debug(fmt, ...) printf(debug_pr_fmt(fmt), ##__VA_ARGS__)
#define trace(fmt, ...) printf(trace_pr_fmt(fmt), ##__VA_ARGS__)
#define warn(fmt, ...) printf(warn_pr_fmt(fmt), ##__VA_ARGS__)
#define error(fmt, ...) printf(error_pr_fmt(fmt), ##__VA_ARGS__)

struct ClientAddr
{
    uint32_t ip;
    uint16_t port;
};

const char INFO_CORRECT[] = "client_info_correct";
const char INFO_ERROR[] = "client_info_error";