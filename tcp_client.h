#ifndef __tcp_client_h
#define __tcp_client_h

#include "allduino.h"

enum IO_STATUS{
    IO_FREE,
    IO_SENDING
};

class TcpClientObj{
public:
    TCPClient client;
    byte *server = NULL;
    char *server_url = NULL;
    char *_host;
    char *_private_key;
    //IpAddress server;
    uint16_t port;
    char *_endpoint;
    char *buffer;

    unsigned char state = IO_FREE;
    unsigned int last_ran;
    uint16_t buffer_len;

    //TcpClientObj(byte *ip_address, uint16_t port, char *endpoint);
    TcpClientObj();
    void init(byte ip_address[4], uint16_t url_port, char *endpoint);
    void init_phant(char *url, uint16_t url_port, char *endpoint, char *private_key);
    void set_buffer(char *buffer, uint16_t buf_len);
    void print();
    void flush();
    void print_flush();
    int8_t transmit_available();
    int8_t transmit(char *message, uint16_t message_len);
    int8_t transmit(char *buffer, uint16_t buf_len, char *message, uint16_t message_len);
    int8_t transmit_phant(char *big_buf, uint16_t big_buf_len,
            char *message, uint16_t message_len);
    int8_t transmit_phant_url(char *big_buf, uint16_t big_buf_len,
        char *message, uint16_t message_len);
    uint16_t phant_put_headers(char **buf, uint16_t len);
    uint16_t phant_put_data(char **buf, uint16_t len, char *message);
    uint16_t phant_end_data(char **buf, uint16_t len);
    int8_t phant_send(char *buf_start, char *buf_end, uint16_t len_left);
private:
};
#endif
