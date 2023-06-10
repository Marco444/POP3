#include "../pop3_states.h"
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "write_buffer_helpers.h"

#define OK_LIST "+OK scan list follows\r\n"
#define OK_RETR "+OK message follows\r\n"
#define OK_QUIT "+OK Quit\r\n"
#define ERRORS_LIST "-ERR no such message\r\n"
#define ERRORS_RETR "-ERR no such mesage\r\n"
#define ERRORS_QUIT "-ERR Quit\r\n"
#define FINISH_RETR "\r\n.\r\n"

// State function declarations for TRANSACTION_STATE
void on_arrival_trans(const unsigned state, struct selector_key *key){ return; }
void on_departure_trans(const unsigned state, struct selector_key *key){ return; }
enum pop3_states on_write_ready_trans(struct selector_key *key) {
    pop3_current_command *current_command = ((struct connection_state *) key->data)->commands.pop3_current_command;
    struct commands_state *commands = &((struct connection_state *) key->data)->commands;

    switch (current_command->cmd_id) {
        //sprintf(buff2,"%ld %ld\r\n",i+1,commands->email_files[i].size);
        case LIST: {
            if (current_command->has_error) {
                bool has_place = enters_the_buffer(key, ERRORS_LIST);
                if (has_place) {
                    long offset = write_in_buffer(key, ERRORS_LIST, strlen(ERRORS_LIST), 0);
                    if (offset == -1) {
                        current_command->is_finished = true;
                    }
                }
            } else {
                if (!current_command->list_state.title_sent) {
                    bool has_place = enters_the_buffer(key, OK_LIST);
                    if (has_place) {
                        long offset = write_in_buffer(key, OK_LIST, strlen(OK_LIST), 0);
                        if (offset == -1) {
                            current_command->list_state.title_sent = true;
                        }
                    }
                }
                for (int i = current_command->list_state.current_index; i < commands->email_files_length; ++i) {
                    if (!commands->email_files[i].is_deleted) {
                        char buff2[30];
                        sprintf(buff2, "%d %ld\r\n", i + 1, commands->email_files[i].size);
                        bool has_place = enters_the_buffer(key, buff2);
                        if (has_place) {
                            long offset = write_in_buffer(key, buff2, strlen(buff2), 0);
                            if (offset == -1) {
                                current_command->list_state.current_index = i + 1;
                            }
                        } else {
                            current_command->list_state.current_index = i;
                            current_command->is_finished = false;
                            break;
                        }
                    }
                }
                if (current_command->list_state.current_index == commands->email_files_length) {
                    bool has_place = enters_the_buffer(key, FINISH_RETR);
                    if (has_place) {
                        long offset = write_in_buffer(key, FINISH_RETR, strlen(FINISH_RETR), 0);
                        if (offset == -1) {
                            current_command->is_finished = true;
                        }
                    }
                }

            }
            return TRANSACTION_STATE;
        }
            break;
        case RETR: {
            if (!current_command->retr_state.title_sent) {
                bool has_place = enters_the_buffer(key, OK_RETR);
                if (has_place) {
                    long offset = write_in_buffer(key, OK_RETR, strlen(OK_RETR), 0);
                    if (offset == -1) {
                        current_command->retr_state.title_sent = true;
                    }
                }
            }
            if (current_command->has_error) {
                bool has_place = enters_the_buffer(key, ERRORS_RETR);
                if (has_place) {
                    long offset = write_in_buffer(key, ERRORS_RETR, strlen(ERRORS_RETR), 0);
                    if (offset == -1) {
                        current_command->is_finished = true;
                    }
                }
            } else {
                if (current_command->retr_state.mail_finished) {
                    bool has_place = enters_the_buffer(key, FINISH_RETR);
                    if (has_place) {
                        long offset = write_in_buffer(key, FINISH_RETR, strlen(FINISH_RETR), 0);
                        if (offset == -1) {
                            current_command->is_finished = true;
                        }
                    }
                }
            }
            return TRANSACTION_STATE;
            case DELE: {
                char buff[100] = "+OK DELE\r\n";
                //write_in_buffer(elem,key,buff);
                return TRANSACTION_STATE;
            }
            break;
            case NOOP: {
                char buff[100] = "+OK NOOP\r\n";
                //write_in_buffer(elem,key,buff);
                return TRANSACTION_STATE;
            }
            break;
            case RSET: {
                char buff[100] = "+OK RSET\r\n";
                //write_in_buffer(elem,key,buff);
                return TRANSACTION_STATE;
            }
            break;
            case QUIT: {
                char buff[100] = "+OK QUIT\r\n";
                //write_in_buffer(elem,key,buff);
                return TRANSACTION_STATE;
            }
            break;
            case STAT: {
                char buff[100] = "+OK STAT\r\n";
                //write_in_buffer(elem,key,buff);
                return TRANSACTION_STATE;
            }
            break;
            default :
                break;
        }
    }
}
enum pop3_states on_block_ready_trans(struct selector_key *key) { return 0; }
unsigned on_read_ready_trans(struct selector_key *key) {
        enum pop3_states state = read_commands(key, TRANSACTION_STATE, true);
        return state;
}
