// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
    {
        .state            = QUIT_MONITOR,
        .on_arrival       = on_arrival_quit_monitor,
        .on_departure     = on_departure_quit_monitor,
        .on_read_ready    = on_read_ready_quit_monitor,
        .on_write_ready   = on_write_ready_quit_monitor,
        .on_block_ready   = on_block_ready_quit_monitor,
    },
    {
        .state            = FORCED_QUIT_MONITOR,
        .on_arrival       = on_arrival_forced_quit_monitor,
        .on_departure     = on_departure_forced_quit_monitor,
        .on_read_ready    = on_read_ready_forced_quit_monitor,
        .on_write_ready   = on_write_ready_forced_quit_monitor,
        .on_block_ready   = on_block_ready_forced_quit_monitor,
    },
    {
        .state            = ERROR_MONITOR,
        .on_arrival       = on_arrival_quit_monitor,
        .on_departure     = on_departure_quit_monitor,
        .on_read_ready    = on_read_ready_quit_monitor,
        .on_write_ready   = on_write_ready_quit_monitor,
        .on_block_ready   = on_block_ready_quit_monitor,
    }
};

