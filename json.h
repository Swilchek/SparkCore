#ifndef __json_h
#define __json_h
#include <inttypes.h>

enum JSON_LEVELS{
    ROOT_BEGIN,
    OBJ_BEGIN,
    VAR_SET,
    VAR_SET_COMMA,
    VAR_SET_END_COMMA,
    VAR_SET_END,
    END,
};

uint16_t json_start(char **in_buf, uint16_t len);
uint16_t json_end(char **in_buf, uint16_t len);
uint16_t json_object(char **in_buf, uint16_t len, char *name);
uint16_t json_continue(char **buf, uint16_t len);
uint16_t put_json(char **in_buf, uint16_t len, uint8_t level, char *name, float value, uint8_t prec);
#endif
