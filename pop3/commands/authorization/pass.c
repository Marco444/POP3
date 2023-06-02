#include "../command_service.h"
#include <stdio.h>
enum pop3_states handle_pass(struct commands_state * ctx) {
  puts("llegue PASS");
  return TRANSACTION_STATE;
}
