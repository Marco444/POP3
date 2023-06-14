#ifndef MONITOR_HEADER_H 
#define MONITOR_HEADER_H  

#include <stdlib.h>
#include <string.h>
#include "../../pop3_states.h"

typedef struct {
    const char *name;
    enum monitor_states (*handler)(struct commands_state * ctx,struct selector_key *key);
} monitor_command;


enum monitor_states handle_monitor_username(struct commands_state * ctx,struct selector_key *key);
enum monitor_states handle_write_username_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

enum monitor_states handle_monitor_add_user(struct commands_state * ctx,struct selector_key *key);
enum monitor_states handle_write_add_user_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

enum monitor_states handle_monitor_password(struct commands_state * ctx,struct selector_key *key);
enum monitor_states handle_write_password_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

enum monitor_states handle_monitor_metrics(struct commands_state * ctx,struct selector_key *key);
enum monitor_states handle_write_metrics_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

enum monitor_states handle_monitor_exit(struct commands_state * ctx,struct selector_key *key);
enum monitor_states handle_write_exit_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);
#endif
