#ifndef COMMAND_SERVICE_HEADER
#define COMMAND_SERVICE_HEADER 

#include <stdlib.h>
#include <string.h>
#include "../pop3_states.h"
#include "server/server.h"
#include "monitor/monitor.h"


enum pop3_states process_command(struct commands_state * ctx,struct selector_key *key,enum pop3_states pop3_state);

enum monitor_states process_command_monitor(struct commands_state *ctx,struct selector_key *key, enum monitor_states pop3_state);


#endif
