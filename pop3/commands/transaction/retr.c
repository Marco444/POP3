
#include "../command_service.h"
#include <stdio.h>

enum pop3_states handle_retr(struct commands_state * ctx) {
  puts("llegue RETR");
  return TRANSACTION_STATE;
}
