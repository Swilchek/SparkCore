
#include "general.h"
#include "json.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Puts JSON into the buffer
 *  buf -- character buffer to put JSON into
 *  len -- remaining length of buffer
 *  level -- level of buffer.
 *  name -- name of variable
 *  value -- float variable
 *  prec -- precision of value
 *
 *  returns remaining length of buffer
 *
 *  {                   ROOT_BEGIN
 *    "PhotoR": 4000,   VAR_SET
 *    "accel":{         OBJ_BEGIN
 *       "x":1.2,       VAR_SET
 *       "y":1.3,       VAR_SET
 *       "z":1.5        VAR_SET_END
 *      }
 *  }                   END
 **/

uint16_t json_start(char **in_buf, uint16_t len){
    return put_json(in_buf, len, ROOT_BEGIN, NULL, 0, 0);
}

uint16_t json_end(char **in_buf, uint16_t len){
    return put_json(in_buf, len, END, NULL, 0, 0);
}

uint16_t json_object(char **in_buf, uint16_t len, char *name){
    return put_json(in_buf, len, OBJ_BEGIN, name, 0, 0);
}

uint16_t json_continue(char **buf, uint16_t len){
    if((*buf)[-1] == '{') return len;
    return bstrncpy(buf, ",", len);
}

uint16_t put_json(char **buf, uint16_t len, uint8_t level, char *name, float value, uint8_t prec){
    uint16_t count;
    switch(level){
        case ROOT_BEGIN:
            len = bstrncpy(buf, "{", len);
            break;
        case OBJ_BEGIN:
        case VAR_SET:
        case VAR_SET_COMMA:
        case VAR_SET_END:
        case VAR_SET_END_COMMA:
            // all of these begin by writing a name
            // with quotes around it
            len = bstrncpy(buf, "\"", len);
            len = bstrncpy(buf, name, len);
            len = bstrncpy(buf, "\":", len);
            if(level == OBJ_BEGIN){
                len = bstrncpy(buf, "{", len);
                break;
            }
            //d2strf(value, 0, prec, *buf);
            dtostrf(value, 0, prec, *buf);
            count = strlen(*buf);
            *buf += count;
            len -= count;
            if(level == VAR_SET_COMMA){
                len = bstrncpy(buf, ",", len);
            }
    }

    switch(level){
        case VAR_SET_END:
        case VAR_SET_END_COMMA:
        case END:
            len = bstrncpy(buf, "}", len);
            if((level) == VAR_SET_END_COMMA){
                len = bstrncpy(buf, ",", len);
            }
            break;
    }

    return len;
}
