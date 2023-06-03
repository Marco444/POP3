#include "shout_down.h"
#include "pop3_states.h"

void clean_user_data(void *user_data){
    struct connection_state * clientData = (struct connection_state *)user_data;
    parser_destroy(&clientData->parser);
    free(clientData);
}