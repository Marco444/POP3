
#ifndef PARSER_POP3_HEADER
#define PARSER_POP3_HEADER

#define N(x) (sizeof(x)/sizeof((x)[0]))

enum pop3_cmd_states {
    UNDEFINED,
    IS_COMMAND,
    INVALID_COMMAND
};


extern const struct parser_definition pop3_parser_definition;

#endif
