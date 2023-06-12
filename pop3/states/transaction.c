#include "../pop3_states.h"
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "write_buffer_helpers.h"



#define CAPA_MSG "+OK Capability list follows\nUSER\nPASS\nQUIT\nLIST\nRETR\nDELE\nNOOP\nRSET\nSTAT\nCAPA\r\n.\r\n"


enum pop3_states on_write_ready_trans(struct selector_key *key) {
    return write_command(key);
}

// State function declarations for TRANSACTION_STATE
void on_arrival_trans(const unsigned state, struct selector_key *key){ return; }
void on_departure_trans(const unsigned state, struct selector_key *key){ return; }
//
// enum pop3_states on_write_ready_trans(struct selector_key *key) {
//     pop3_current_command *current_command = ((struct connection_state *) key->data)->commands.pop3_current_command;
//     struct commands_state *commands = &((struct connection_state *) key->data)->commands;
//
//     switch (current_command->cmd_id) {
//         case LIST: 
//         case RETR: {
//
//             case DELE: {
//                 char buff[100] = "+OK DELE\r\n";
//                 write_in_buffer(key,buff,strlen(buff), 0);
//                 return TRANSACTION_STATE;
//             }
//             break;
//             case NOOP: {
//                 char buff[100] = "+OK NOOP\r\n";
//                 //write_in_buffer(elem,key,buff);
//                 return TRANSACTION_STATE;
//             }
//             break;
//             case RSET: {
//                 char buff[100] = "+OK RSET\r\n";
//                 //write_in_buffer(elem,key,buff);
//                 return TRANSACTION_STATE;
//             }
//             break;
//             case QUIT: {
//                 char buff[100] = "+OK QUIT\r\n";
//                 //write_in_buffer(elem,key,buff);
//                 return TRANSACTION_STATE;
//             }
//             break;
//             case STAT: {
//                 char buff[100] = "+OK STAT\r\n";
//                 // write_in_buffer(elem,key,buff);
//                 return TRANSACTION_STATE;
//             }
//             case CAPA: {
//                 write_in_buffer(key, CAPA_MSG, strlen(CAPA_MSG), 0);
//                 current_command->is_finished = true;
//                 return TRANSACTION_STATE;
//             }
//             break;
//             default :
//                 break;
//         }
//     }
// }
enum pop3_states on_block_ready_trans(struct selector_key *key) { return 0; }
unsigned on_read_ready_trans(struct selector_key *key) {
        enum pop3_states state = read_commands(key, TRANSACTION_STATE, true);
        return state;
}
