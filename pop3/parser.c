
#include "commands/command_service.h"
#include "parser.h"
#include "pop3_states.h"
#include "../lib/parser/parser.h"
#include "../lib/selector/selector.h"

static void act1_cr(struct parser_event *ret, const uint8_t c, struct commands_state * ctx);
static void act1_cmd(struct parser_event *ret, const uint8_t c, struct commands_state * ctx);
static void act1_arg2(struct parser_event *ret, const uint8_t c, struct commands_state * ctx);
static void act1_arg(struct parser_event *ret, const uint8_t c, struct commands_state * ctx);

static const struct parser_state_transition ST_CMD [] =  {
    {.when = ' ',  .dest = POP3_STATE_ARG, .act1 = act1_cmd},
};

static const struct parser_state_transition ST_ARG [] =  {
    {.when = '\r', .dest = POP3_STATE_CR,  .act1 = act1_arg},
    {.when = ' ',  .dest = POP3_STATE_ARG, .act1 = act1_arg2},
};

static const struct parser_state_transition ST_CR [] =  {
    {.when = '\n', .dest = POP3_STATE_CMD, .act1 = act1_cr},
};

// state transition mapping
static const struct parser_state_transition *pop3_states_transitions[] = {
    ST_CMD, ST_ARG, ST_CR, 
};

// size of each state transition
static const size_t pop3_states_transitions_n [] = {
    N(ST_CMD), N(ST_ARG), N(ST_CR), 
};

// final parser definition
const struct parser_definition pop3_parser_definition = {
    .states_count = N(pop3_states_transitions),
    .states       = pop3_states_transitions,
    .states_n     = pop3_states_transitions_n,
    .start_state  = POP3_STATE_CMD,
};

static void act1_cmd(struct parser_event *ret, const uint8_t c, struct commands_state * ctx) {
    // append the character to the command
    if (ctx->cmd_length < POP3_MAX_CMD_LENGTH) {
        ctx->cmd[ctx->cmd_length++] = c;
        ctx->cmd[ctx->cmd_length] = '\0';
    }
}

static void act1_arg(struct parser_event *ret, const uint8_t c, struct commands_state * ctx) {
    // append the character to the first argument
    if (ctx->arg1_length < POP3_MAX_ARG_LENGTH) {
        ctx->arg1[ctx->arg1_length++] = c;
        ctx->arg1[ctx->arg1_length] = '\0';
    }
}

static void act1_arg2(struct parser_event *ret, const uint8_t c, struct commands_state * ctx) {
    // append the character to the second argument
    if (ctx->arg2_length < POP3_MAX_ARG_LENGTH) {
        ctx->arg2[ctx->arg2_length++] = c;
        ctx->arg2[ctx->arg2_length] = '\0';
    }
}

static void act1_cr(struct parser_event *ret, const uint8_t c, struct commands_state * ctx) {
    // process the command here, then reset the command and argument buffers
    process_command(ctx);

    // reset buffers
    ctx->cmd[0] = ctx->arg1[0] = ctx->arg2[0] = '\0';
    ctx->cmd_length = ctx->arg1_length = ctx->arg2_length = 0;
}

