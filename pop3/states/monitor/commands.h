
#ifndef COMMANDS_MONITOR_H
#define COMMANDS_MONITOR_H 

#include "../../commands/command_service.h"
#include "../../commands/parser.h"
#include "../../pop3_states.h"
#include <stdio.h>
#include <sys/socket.h>

enum monitor_states read_commands_monitor(struct selector_key *key, enum monitor_states pop3_state, bool toRead);

#endif
