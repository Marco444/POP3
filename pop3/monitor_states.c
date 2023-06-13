#include "monitor_states.h"
#include "shared.h"
#include <stdio.h>

struct state_definition pop3_monitor_states[]  = {
    {
        .state            =  AUTH_MONITOR,
        .on_arrival       = on_arrival_auth_monitor,
        .on_departure     = on_departure_auth_monitor,
        .on_read_ready    = on_read_ready_auth_monitor,
        .on_write_ready   = on_write_ready_auth_monitor,
        .on_block_ready   = on_block_ready_auth_monitor,
    },
    {
        .state            = TRANSACTION_MONITOR,
        .on_arrival       = on_arrival_trans_monitor,
        .on_departure     = on_departure_trans_monitor,
        .on_read_ready    = on_read_ready_trans_monitor,
        .on_write_ready   = on_write_ready_trans_monitor,
        .on_block_ready   = on_block_ready_trans_monitor,
    },
};

