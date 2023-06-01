#include "../command_service.h"
#include <stdio.h>
void handle_pass(struct commands_state * ctx) {
  printf("\nllegue a leer PASS command! arg1: %s, arg2: %s", ctx->arg1, ctx->arg2);
}
