#include "../../command_service.h"

enum pop3_states handle_update_quit(struct commands_state * ctx, struct selector_key *key) {
  return UPDATE_STATE;
}

enum pop3_states handle_write_update_quit(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
  return UPDATE_STATE;
}
