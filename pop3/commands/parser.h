
#ifndef PARSER_POP3_HEADER
#define PARSER_POP3_HEADER

#define N(x) (sizeof(x)/sizeof((x)[0]))

enum pop3_parser_states {
    POP3_STATE_CMD,
    POP3_STATE_ARG,
    POP3_STATE_CR,
    POP3_STATE_LF,
};

extern const struct parser_definition pop3_parser_definition;

#endif
