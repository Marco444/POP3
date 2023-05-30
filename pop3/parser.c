
#include "pop3_states.h"
#include "../lib/parser/parser.h"
#include "../lib/selector/selector.h"

enum pop3_parser_states {
    POP3_STATE_CMD,
    POP3_STATE_ARG,
    POP3_STATE_CR,
    POP3_STATE_LF,
};

void act1_cmd(struct parser_event *ret, const uint8_t c, struct connection_state * ctx) {
    // append the character to the command
    // if (ctx->cmd_len < POP3_MAX_CMD_LENGTH) {
    //     ctx->cmd[ctx->cmd_len++] = c;
    //     ctx->cmd[ctx->cmd_len] = '\0';
    // }
}

void act1_arg(struct parser_event *ret, const uint8_t c, struct connection_state * ctx) {
    // append the character to the argument
    // if (ctx->arg_len < POP3_MAX_ARG_LENGTH) {
    //     ctx->arg[ctx->arg_len++] = c;
    //     ctx->arg[ctx->arg_len] = '\0';
    // }
}

void act1_cr(struct parser_event *ret, const uint8_t c, struct connection_state * ctx) {
    // process the command here, then reset the command and argument buffers
    // process_command(ctx->cmd, ctx->arg);
    // ctx->cmd[0] = ctx->arg[0] = '\0';
    // ctx->cmd_len = ctx->arg_len = 0;
}

struct parser_state_transition pop3_transitions[] = {
    { ' ', POP3_STATE_ARG, act1_cmd, NULL },  // when in cmd state, space takes us to arg state
    { '\r', POP3_STATE_CR, act1_arg, NULL },  // when in arg state, '\r' takes us to cr state
    { '\n', POP3_STATE_CMD, act1_cr, NULL },  // when in cr state, '\n' takes us back to cmd state
};

