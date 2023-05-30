
#include "commands/command_service.h"
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
    if (ctx->cmd_length < POP3_MAX_CMD_LENGTH) {
        ctx->cmd[ctx->cmd_length++] = c;
        ctx->cmd[ctx->cmd_length] = '\0';
    }
}

void act1_arg(struct parser_event *ret, const uint8_t c, struct connection_state * ctx) {
    // append the character to the first argument
    if (ctx->arg1_length < POP3_MAX_ARG_LENGTH) {
        ctx->arg1[ctx->arg1_length++] = c;
        ctx->arg1[ctx->arg1_length] = '\0';
    }
}

void act1_arg2(struct parser_event *ret, const uint8_t c, struct connection_state * ctx) {
    // append the character to the second argument
    if (ctx->arg2_length < POP3_MAX_ARG_LENGTH) {
        ctx->arg2[ctx->arg2_length++] = c;
        ctx->arg2[ctx->arg2_length] = '\0';
    }
}

void act1_cr(struct parser_event *ret, const uint8_t c, struct connection_state * ctx) {
    // process the command here, then reset the command and argument buffers
    process_command(ctx, ctx->cmd, ctx->arg1, ctx->arg2);

    // reset buffers
    ctx->cmd[0] = ctx->arg1[0] = ctx->arg2[0] = '\0';
    ctx->cmd_length = ctx->arg1_length = ctx->arg2_length = 0;
}

struct parser_state_transition pop3_transitions[] = {
    { ' ', POP3_STATE_ARG, act1_cmd, NULL },  // when in cmd state, space takes us to arg state
    { '\r', POP3_STATE_CR, act1_arg, NULL },  // when in arg state, '\r' takes us to cr state
    { ' ', POP3_STATE_ARG, act1_arg2, NULL },  // when in arg state, ' ' takes us to the second argument! 
    { '\n', POP3_STATE_CMD, act1_cr, NULL },  // when in cr state, '\n' takes us back to cmd state
};

