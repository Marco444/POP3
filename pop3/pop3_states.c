
#include "pop3_states.h"
#include <stdio.h>

struct state_definition pop3_server_states[]  = {
    {
        .state            = AUTHORIZATION_STATE,
        .on_arrival       = on_arrival_auth,
        .on_departure     = on_departure_auth,
        .on_read_ready    = on_read_ready_auth,
        .on_write_ready   = on_write_ready_auth,
        .on_block_ready   = on_block_ready_auth,
    },
    {
        .state            = TRANSACTION_STATE,
        .on_arrival       = on_arrival_trans,
        .on_departure     = on_departure_trans,
        .on_read_ready    = on_read_ready_trans,
        .on_write_ready   = on_write_ready_trans,
        .on_block_ready   = on_block_ready_trans,
    },
    {
        .state            = UPDATE_STATE,
        .on_arrival       = on_arrival_update,
        .on_departure     = on_departure_update,
        .on_read_ready    = on_read_ready_update,
        .on_write_ready   = on_write_ready_update,
        .on_block_ready   = on_block_ready_update,
    },
    {
        .state            = ERROR_STATE,
        .on_arrival       = on_arrival_error,
        .on_departure     = on_departure_error,
        .on_read_ready    = on_read_ready_error,
        .on_write_ready   = on_write_ready_error,
        .on_block_ready   = on_block_ready_error,
    },
     {
        .state            = FORCED_QUIT_STATE,
        .on_arrival       = on_arrival_force_quit,
        .on_departure     = on_departure_force_quit,
        .on_read_ready    = on_read_ready_force_quit,
        .on_write_ready   = on_write_ready_force_quit,
        .on_block_ready   = on_block_ready_force_quit,
    }
};

