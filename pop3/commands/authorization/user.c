#include "../command_service.h"
#include <stdio.h>
enum pop3_states handle_user(struct commands_state * ctx) {
  puts("llegue USER");
  return AUTHORIZATION_STATE;
}
