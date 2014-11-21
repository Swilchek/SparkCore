#include "tcp_client.h"

TcpClientObj::TcpClientObj(){
}

void TcpClientObj::init(byte *rest_ip, uint16_t rest_port, char *rest_endpoint){
    //memcpy(server, rest_ip, 4);
    server = rest_ip;
    port = rest_port;
    _endpoint = rest_endpoint;
    buffer = NULL;
    buffer_len = 0;
}

void TcpClientObj::init_phant(char *url, uint16_t url_port, char *endpoint,
        char *private_key){
    server_url = url;
    port = url_port;
    _endpoint = endpoint;
    _private_key = private_key;
}

void TcpClientObj::set_buffer(char *buf, uint16_t len){
    buffer = buf;
    buffer_len = len;
}

void TcpClientObj::print(){
    if(server){
        Serial.print(server[0]); Serial.write('.');
        Serial.print(server[1]); Serial.write('.');
        Serial.print(server[2]); Serial.write('.');
        Serial.print(server[3]); Serial.write(':');
        Serial.print(port);
    }
    else {
        Serial.print(server_url);
    }
    Serial.print(_endpoint);
}

void TcpClientObj::flush(){
    while(client.available()){
        client.read();
    }
}

void TcpClientObj::print_flush(){
    while(client.available()){
        Serial.print(client.read());
    }
}

#define MIN_COM_PERIOD 80
int8_t TcpClientObj::transmit_available(){
    if(state == IO_SENDING){
        if(client.connected() == false){
            // we are done sending last message
            state = IO_FREE;
        }
        else{
            if(TIME_SINCE(last_ran) > MIN_COM_PERIOD) return true;
            return false;
        }
    }
    if(state == IO_FREE){
        if(client.connected()){
            return true;
        }
        if(server){
            return client.connect(server, port);
        }
        else{
            return client.connect(server_url, port);
        }
    }
    return false;
}

/* Transmit for if you set the buffer.
 * data should be a character array that is in JSON form
 **/
int8_t TcpClientObj::transmit(char *message, uint16_t message_len){
    return transmit(buffer, buffer_len, message, message_len);
}

/**
 * Transmit data if you haven't or don't want to set the buffer.
 * Send the pointer to the buffer and the remaining length for the
 * first two variables. The rest is the same as above.
 **/
int8_t TcpClientObj::transmit(char *big_buf, uint16_t big_buf_len,
        char *message, uint16_t message_len){
    char small_buf[10];
    char *place = big_buf;
    uint16_t sbuf_len= big_buf_len;

    if(big_buf == NULL){
        return -1;
    }

    if(not transmit_available()){
        return false;
    }
    sbuf_len = bstrncpy(&place, "POST ", sbuf_len);
    sbuf_len  = bstrncpy(&place, _endpoint, sbuf_len);
    sbuf_len = bstrncpy(&place, " HTTP/1.1\r\nHost: ", sbuf_len);
    for(int8_t n = 0; n < 4; n++){
        itoa(server[n], small_buf, 10);
        sbuf_len = bstrncpy(&place, small_buf, sbuf_len);
        if(n!=3) sbuf_len = bstrncpy(&place, ".", sbuf_len);
    }
    sbuf_len = bstrncpy(&place, "\r\nContent-Length: ", sbuf_len);
    itoa(message_len, small_buf, 10);
    sbuf_len = bstrncpy(&place, small_buf, sbuf_len);
    sbuf_len = bstrncpy(&place, "\r\nContent-Type: application/json\r\n", sbuf_len);
    sbuf_len = bstrncpy(&place, "Connection: keep-alive\r\n", sbuf_len);
    sbuf_len = bstrncpy(&place, "\r\n", sbuf_len);
    sbuf_len = bstrncpy(&place, message, sbuf_len);
    sbuf_len = bstrncpy(&place, "\r\n", sbuf_len);

    if(sbuf_len == 0){
        Serial.println("BUF_LEN not enough!");
        return -1;
    }

    client.write((const uint8_t *)big_buf, big_buf_len - sbuf_len); // Believe it or not -- this won't work for client.print!!!

    last_ran = millis();
    state = IO_SENDING;
    return true;
}

int8_t TcpClientObj::transmit_phant(char *big_buf, uint16_t big_buf_len,
        char *message, uint16_t message_len){

    char small_buf[10];
    char *place = big_buf;
    uint16_t sbuf_len= big_buf_len;
    if(big_buf == NULL){
        return -1;
    }
    if(not transmit_available()){
        return false;
    }

    sbuf_len = bstrncpy(&place, "POST /input/", sbuf_len);
    sbuf_len = bstrncpy(&place, _endpoint, sbuf_len);
    sbuf_len = bstrncpy(&place, " HTTP/1.1\r\n", sbuf_len);
    //sbuf_len = bstrncpy(&place, "User-Agent: SparkCoreLibrary\r\n", sbuf_len);
    sbuf_len = bstrncpy(&place, "Host: ", sbuf_len);
    sbuf_len = bstrncpy(&place, server_url, sbuf_len);
    sbuf_len = bstrncpy(&place, "\r\nConnection: keep-alive\r\n", sbuf_len);
    //sbuf_len = bstrncpy(&place, "\r\nConnection: close\r\n", sbuf_len);
    sbuf_len = bstrncpy(&place, "Phant-Private-Key: ", sbuf_len);
    sbuf_len = bstrncpy(&place, _private_key, sbuf_len);
    sbuf_len = bstrncpy(&place, "\r\nContent-Type: application/x-www-form-urlencoded\r\n", sbuf_len);
    sbuf_len = bstrncpy(&place, "Content-Length: ", sbuf_len);
    itoa(message_len, small_buf, 10);
    sbuf_len = bstrncpy(&place, small_buf, sbuf_len);
    sbuf_len = bstrncpy(&place, "\r\n", sbuf_len);
    sbuf_len = bstrncpy(&place, "\r\n", sbuf_len);
    sbuf_len = bstrncpy(&place, message, sbuf_len);
    sbuf_len = bstrncpy(&place, "\r\n", sbuf_len);

    if(sbuf_len == 0){
        return -1;
    }

    client.write((const uint8_t *)big_buf, big_buf_len - sbuf_len); // Believe it or not -- this won't work for client.print!!!
    Serial.println(big_buf);

    last_ran = millis();
    state = IO_SENDING;
    return true;
}

int8_t TcpClientObj::transmit_phant_url(char *big_buf, uint16_t big_buf_len,
        char *message, uint16_t message_len){
    char *place = big_buf;
    uint16_t sbuf_len= big_buf_len;

    if(big_buf == NULL){
        return -1;
    }
    if(not transmit_available()){
        return false;
    }

    sbuf_len = bstrncpy(&place, "GET /input/", sbuf_len);
    sbuf_len = bstrncpy(&place, _endpoint, sbuf_len);
    sbuf_len = bstrncpy(&place, "?private_key=", sbuf_len);
    sbuf_len = bstrncpy(&place, _private_key, sbuf_len);
    sbuf_len = bstrncpy(&place, "&", sbuf_len);
    sbuf_len = bstrncpy(&place, message, sbuf_len);
    sbuf_len = bstrncpy(&place, "\r\nHost: ", sbuf_len);
    sbuf_len = bstrncpy(&place, server_url, sbuf_len);
    sbuf_len = bstrncpy(&place, "\r\nConnection: close\r\n\r\n", sbuf_len);

    if(sbuf_len == 0){
        return -1;
    }

    client.write((const uint8_t *)big_buf, big_buf_len - sbuf_len); // Believe it or not -- this won't work for client.print!!!
    Serial.println();
    Serial.println(big_buf);
    Serial.println();
    last_ran = millis();
    state = IO_SENDING;

    return true;
}


// These are the objects that you can use to send data sequentially. Here's what you do:
// in the init you phant_put_headers
// you then put data one at a time
//   you end each data putting with phant_end_data. This makes it easier to put
//   an array of data
// when you are ready to send, you send with phant_send
uint16_t TcpClientObj::phant_put_headers(char **buf, uint16_t len){
    // the buffer you input should not be your "big buffer".
    // It should be your data buffer, as it is where your data will go
    len = bstrncpy(buf, "GET /input/", len);
    len = bstrncpy(buf, _endpoint, len);
    len = bstrncpy(buf, "?private_key=", len);
    len = bstrncpy(buf, _private_key, len);
    len = bstrncpy(buf, "&", len);
    return len;
}

uint16_t TcpClientObj::phant_put_data(char **buf, uint16_t len, char *message){
    len = bstrncpy(buf, message, len);
    return len;
}

uint16_t TcpClientObj::phant_end_data(char **buf, uint16_t len){
    len = bstrncpy(buf, "&", len);
    return len;
}

int8_t TcpClientObj::phant_send(char *buf_start, char *buf_end, uint16_t len_left){
    len_left = bstrncpy(&buf_end, "\r\nHost: ", len_left);
    len_left = bstrncpy(&buf_end, server_url, len_left);
    len_left = bstrncpy(&buf_end, "\r\nConnection: close\r\n\r\n", len_left);

    if(len_left == 0){
        return -1;
    }

    client.write((const uint8_t *)buf_start, buf_end - buf_start); // Believe it or not -- this won't work for client.print!!!
    Serial.println();
    Serial.println(buf_start);
    Serial.println();
    last_ran = millis();
    state = IO_SENDING;
    return true;
}

