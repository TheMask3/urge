/* autogenerated. */
/* src="transcode-tblgen.rb", len=28509, checksum=52888 */
/* src="newline.trans", len=3162, checksum=42485 */

#include "transcode_data.h"



static const unsigned char
newline_byte_array[516] = {
#define universal_newline_offsets 0
0, 255,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,

#define crlf_newline_offsets 258
0, 255,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  1,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,    0,  0,  0,  0,  0,  0,  0,  0,

};
static const unsigned int
newline_word_array[11] = {
#define universal_newline_infos WORDINDEX2INFO(0)
     FUNso,

#define universal_newline WORDINDEX2INFO(1)
    universal_newline_offsets,
    universal_newline_infos,

#define crlf_newline_infos WORDINDEX2INFO(3)
             NOMAP, o2(0x0d,0x0a),

#define crlf_newline WORDINDEX2INFO(5)
    crlf_newline_offsets,
    crlf_newline_infos,

#define cr_newline_infos WORDINDEX2INFO(7)
        NOMAP, o1(0x0d),

#define cr_newline WORDINDEX2INFO(9)
    crlf_newline_offsets,
    cr_newline_infos,

};
#define TRANSCODE_TABLE_INFO newline_byte_array, 516, newline_word_array, 11, ((int)sizeof(unsigned int))


#define STATE (sp[0])
#define NORMAL 0
#define JUST_AFTER_CR 1

/* no way to access this information, yet. */
#define NEWLINES_MET (sp[1])
#define MET_LF          0x01
#define MET_CRLF        0x02
#define MET_CR          0x04

static int
universal_newline_init(void *statep)
{
    unsigned char *sp = statep;
    STATE = NORMAL;
    NEWLINES_MET = 0;
    return 0;
}

static ssize_t
fun_so_universal_newline(void *statep, const unsigned char *s, size_t l, unsigned char *o, size_t osize)
{
    unsigned char *sp = statep;
    int len;
    if (s[0] == '\n') {
        if (STATE == NORMAL) {
            NEWLINES_MET |= MET_LF;
        }
        else { /* JUST_AFTER_CR */
            NEWLINES_MET |= MET_CRLF;
        }
        o[0] = '\n';
        len = 1;
        STATE = NORMAL;
    }
    else {
        len = 0;
        if (STATE == JUST_AFTER_CR) {
            o[0] = '\n';
            len = 1;
            NEWLINES_MET |= MET_CR;
        }
        if (s[0] == '\r') {
            STATE = JUST_AFTER_CR;
        }
        else {
            o[len++] = s[0];
            STATE = NORMAL;
        }
    }

    return len;
}

static ssize_t
universal_newline_finish(void *statep, unsigned char *o, size_t osize)
{
    unsigned char *sp = statep;
    int len = 0;
    if (STATE == JUST_AFTER_CR) {
        o[0] = '\n';
        len = 1;
        NEWLINES_MET |= MET_CR;
    }
    STATE = NORMAL;
    return len;
}

static const rb_transcoder
rb_universal_newline = {
    "", "universal_newline", universal_newline,
    TRANSCODE_TABLE_INFO,
    1, /* input_unit_length */
    1, /* max_input */
    2, /* max_output */
    asciicompat_converter, /* asciicompat_type */
    2, universal_newline_init, universal_newline_init, /* state_size, state_init, state_fini */
    NULL, NULL, NULL, fun_so_universal_newline,
    universal_newline_finish
};

static const rb_transcoder
rb_crlf_newline = {
    "", "crlf_newline", crlf_newline,
    TRANSCODE_TABLE_INFO,
    1, /* input_unit_length */
    1, /* max_input */
    2, /* max_output */
    asciicompat_converter, /* asciicompat_type */
    0, NULL, NULL, /* state_size, state_init, state_fini */
    NULL, NULL, NULL, NULL
};

static const rb_transcoder
rb_cr_newline = {
    "", "cr_newline", cr_newline,
    TRANSCODE_TABLE_INFO,
    1, /* input_unit_length */
    1, /* max_input */
    1, /* max_output */
    asciicompat_converter, /* asciicompat_type */
    0, NULL, NULL, /* state_size, state_init, state_fini */
    NULL, NULL, NULL, NULL
};

void
Init_newline(void)
{
    rb_register_transcoder(&rb_universal_newline);
    rb_register_transcoder(&rb_crlf_newline);
    rb_register_transcoder(&rb_cr_newline);
}

