// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#define SIGNOFF_MSG "+OK POP3 server deleted the mails\r\n"
#define QUIT_ERROR_MSG "-ERR POP3 server error\r\n"
int write_in_fd(struct selector_key *key);
enum pop3_states handle_update_quit(struct commands_state * ctx, struct selector_key *key) {
  return UPDATE_STATE;
}

enum pop3_states handle_write_update_quit(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
    if(commands->pop3_current_command->quit_update_state.has_deleted){
        if (!commands->pop3_current_command->quit_update_state.has_error) {
            bool has_place = enters_the_buffer(key, SIGNOFF_MSG);
            if (has_place) {
                long offset = write_in_buffer(key, SIGNOFF_MSG, strlen(SIGNOFF_MSG), 0);
                if (offset == -1) {
                    commands->pop3_current_command->is_finished = true;
                }
            }
        }else{
            bool has_place = enters_the_buffer(key, QUIT_ERROR_MSG);
            if (has_place) {
                long offset = write_in_buffer(key, QUIT_ERROR_MSG, strlen(QUIT_ERROR_MSG), 0);
                if (offset == -1) {
                    commands->pop3_current_command->is_finished = true;
                }
            }
        }
    }
    write_in_fd(key);
    return FORCED_QUIT_STATE;
}
