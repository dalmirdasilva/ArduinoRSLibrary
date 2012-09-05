#include <stdint.h>
#include "rs.h"

#include <glcd_text_line.h>

void print_ln(char *s, uint8_t r) {
    char b[3];
    i8toh(r, b);
    glcd_text_line_print_ln(s);
    glcd_text_line_print_ln(b);
}

#define rs_spec_printf print_ln

#define RS_SPEC_IGNORE_0
#define RS_SPEC_IGNORE_1
#define RS_SPEC_IGNORE_2
#define RS_SPEC_IGNORE_3
#define RS_SPEC_IGNORE_4
#define RS_SPEC_IGNORE_5
#define RS_SPEC_IGNORE_6
#define RS_SPEC_IGNORE_7
#define RS_SPEC_IGNORE_8
#define RS_SPEC_IGNORE_9
#define RS_SPEC_IGNORE_10
#define RS_SPEC_IGNORE_11
#define RS_SPEC_IGNORE_12
#define RS_SPEC_IGNORE_13
#define RS_SPEC_IGNORE_14
#define RS_SPEC_IGNORE_15
#define RS_SPEC_IGNORE_16
#define RS_SPEC_IGNORE_17
#define RS_SPEC_IGNORE_18
#define RS_SPEC_IGNORE_19
#define RS_SPEC_IGNORE_20
#define RS_SPEC_IGNORE_21
#define RS_SPEC_IGNORE_22

#include "rs_spec.h"