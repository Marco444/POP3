
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
    if (ctx->in_buffer_length < POP3_MAX_CMD_LENGTH) {
        ctx->in_buffer[ctx->in_buffer_length++] = c;
        ctx->in_buffer[ctx->in_buffer_length] = '\0';
    }
}

void act1_arg(struct parser_event *ret, const uint8_t c, struct connection_state * ctx) {
    // append the character to the argument
    if (ctx->out_buffer_length < POP3_MAX_ARG_LENGTH) {
        ctx->out_buffer[ctx->out_buffer_length++] = c;
        ctx->out_buffer[ctx->out_buffer_length] = '\0';
    }
}

void act1_arg2(struct parser_event *ret, const uint8_t c, struct connection_state * ctx) {
    // append the character to the argument
    if (ctx->out_buffer_length < POP3_MAX_ARG_LENGTH) {
        ctx->out_buffer[ctx->out_buffer_length++] = c;
        ctx->out_buffer[ctx->out_buffer_length] = '\0';
    }
}

void act1_cr(struct parser_event *ret, const uint8_t c, struct connection_state * ctx) {
    // process the command here, then reset the command and argument buffers
    // process_command(ctx->cmd, ctx->arg);
    ctx->in_buffer[0] = ctx->out_buffer[0] = '\0';
    ctx->in_buffer_length = ctx->out_buffer_length = 0;
}

struct parser_state_transition pop3_transitions[] = {
    { ' ', POP3_STATE_ARG, act1_cmd, NULL },  // when in cmd state, space takes us to arg state
    { '\r', POP3_STATE_CR, act1_arg, NULL },  // when in arg state, '\r' takes us to cr state
    { ' ', POP3_STATE_ARG, act1_arg2, NULL },  // when in arg state, ' ' takes us to the second argument! 
    { '\n', POP3_STATE_CMD, act1_cr, NULL },  // when in cr state, '\n' takes us back to cmd state
};

