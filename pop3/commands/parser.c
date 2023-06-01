
#include "command_service.h"
#include "parser.h"
#include "../pop3_states.h"
#include "../../lib/parser/parser.h"
#include "../../lib/selector/selector.h"

#include <stdio.h>


enum pop3_parser_states {
    POP3_STATE_START,
    POP3_STATE_CMD,
    POP3_STATE_ARG1,
    POP3_STATE_ARG2,
    POP3_STATE_END,
};

static void consume_space(struct parser_event *ret, const uint8_t c, struct commands_state * ctx, enum pop3_states pop3_state, enum pop3_states * next_state){}

static void consume_into_cmd(struct parser_event *ret, const uint8_t c, struct commands_state * ctx, enum pop3_states pop3_state, enum pop3_states * next_state) {
    // append the character to the command
    if (ctx->cmd_length < POP3_MAX_CMD_LENGTH) {
        ctx->cmd[ctx->cmd_length++] = c;
        ctx->cmd[ctx->cmd_length] = '\0';
    }
}

static void consume_into_arg1(struct parser_event *ret, const uint8_t c, struct commands_state * ctx, enum pop3_states pop3_state, enum pop3_states * next_state) {
    // append the character to the first argument
    if (ctx->arg1_length < POP3_MAX_ARG_LENGTH) {
        ctx->arg1[ctx->arg1_length++] = c;
        ctx->arg1[ctx->arg1_length] = '\0';
    }
}


static void consume_into_arg2(struct parser_event *ret, const uint8_t c, struct commands_state * ctx, enum pop3_states pop3_state, enum pop3_states * next_state) {
    // append the character to the first argument
    if (ctx->arg2_length < POP3_MAX_ARG_LENGTH) {
        ctx->arg2[ctx->arg2_length++] = c;
        ctx->arg2[ctx->arg2_length] = '\0';
    }
}


static void process_command_handler(struct parser_event *ret, const uint8_t c, struct commands_state * ctx, enum pop3_states pop3_state, enum pop3_states * next_state) {
    // process the command here, then reset the command and argument buffers
    *next_state = process_command(ctx, pop3_state);

    // reset buffers
    ctx->cmd_length = ctx->arg1_length = ctx->arg2_length = 0;
}


static void invalid_arguments(struct parser_event *ret, const uint8_t c, struct commands_state * ctx, enum pop3_states pop3_state, enum pop3_states * next_state) {
    //notify invalid command

    // reset buffers
    ctx->cmd[0] = ctx->arg1[0] = ctx->arg2[0] = '\0';
    ctx->cmd_length = ctx->arg1_length = ctx->arg2_length = 0;
}

static const struct parser_state_transition st_start [] =  {
    {.when = ' ',  .dest = POP3_STATE_START, .act1 = consume_space},
    {.when = ANY,  .dest = POP3_STATE_CMD, .act1 = consume_into_cmd},
};

static const struct parser_state_transition st_cmd [] =  {
    {.when = ' ',  .dest = POP3_STATE_ARG1, .act1 = consume_space},
    {.when = '\r', .dest = POP3_STATE_END, .act1 = consume_space},
    {.when = ANY,  .dest = POP3_STATE_CMD, .act1 = consume_into_cmd},
};

static const struct parser_state_transition st_arg1 [] =  {
    {.when = ' ',  .dest = POP3_STATE_ARG2, .act1 = consume_space},
    {.when = '\r', .dest = POP3_STATE_END, .act1 = consume_space},
    {.when = ANY,  .dest = POP3_STATE_ARG1, .act1 = consume_into_arg1},
};

static const struct parser_state_transition st_arg2 [] =  {
    {.when = ' ',  .dest = POP3_STATE_START, .act1 = invalid_arguments},
    {.when = '\r', .dest = POP3_STATE_END, .act1 = consume_space},
    {.when = ANY,  .dest = POP3_STATE_ARG2, .act1 = consume_into_arg2},
};

static const struct parser_state_transition st_end [] =  {
    {.when = '\n', .dest = POP3_STATE_START, .act1 = process_command_handler},
    {.when = ANY,  .dest = POP3_STATE_ARG2, .act1 = invalid_arguments},
};

 // state transition mapping
 static const struct parser_state_transition *pop3_states_transitions[] = {
    st_start, st_cmd, st_arg1, st_arg2, st_end
 };

 // size of each state transition
 static const size_t pop3_states_transitions_n [] = {
     N(st_start), N(st_cmd), N(st_arg1), N(st_arg2), N(st_end)
 };

 // final parser definition
 const struct parser_definition pop3_parser_definition = {
     .states_count = N(pop3_states_transitions),
     .states       = pop3_states_transitions,
     .states_n     = pop3_states_transitions_n,
     .start_state  = POP3_STATE_START,
 };


