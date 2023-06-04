#include "../pop3_states.h"
#include <sys/socket.h>
#include <stdio.h>

void write_in_buffer(elem_type elem,struct selector_key * key,char * buff);

// State function declarations for TRANSACTION_STATE
void on_arrival_trans(const unsigned state, struct selector_key *key){ return; }
void on_departure_trans(const unsigned state, struct selector_key *key){ return; }
enum pop3_states on_write_ready_trans(struct selector_key *key){
    selector_set_interest_key(key, OP_READ);
    elem_type elem =  peek(((struct connection_state *)key->data)->commands.write_list);
    if(elem == NULL)
        return ERROR_STATE;
    switch (elem->cmd_id)
    {
    case LIST:{
        char buff[100] = "+OK list\r\n";
        write_in_buffer(elem,key,buff);
        return TRANSACTION_STATE;
        }
        break;
    case RETR:
        {
        char buff[100] = "+OK RETR\r\n";
        write_in_buffer(elem,key,buff);
        return TRANSACTION_STATE;
        }    
        break;
    case DELE:
        {
        char buff[100] = "+OK DELE\r\n";
        write_in_buffer(elem,key,buff);
        return TRANSACTION_STATE;
        }    
        break;
    case NOOP:
        {
        char buff[100] = "+OK NOOP\r\n";
        write_in_buffer(elem,key,buff);
        return TRANSACTION_STATE;
        }
        break;
    case RSET:
        {
        char buff[100] = "+OK RSET\r\n";
        write_in_buffer(elem,key,buff);
        return TRANSACTION_STATE;
        }
        break;
    case QUIT:
        {
        char buff[100] = "+OK QUIT\r\n";
        write_in_buffer(elem,key,buff);
        return TRANSACTION_STATE;
        }
        break;
    case STAT:
        {
        char buff[100] = "+OK STAT\r\n";
        write_in_buffer(elem,key,buff);
        return TRANSACTION_STATE;
        }
        break;
    default :
        break;
    }
}
enum pop3_states on_block_ready_trans(struct selector_key *key){ return 0; }

unsigned on_read_ready_trans(struct selector_key *key) {
   enum pop3_states state = read_commands(key, TRANSACTION_STATE, true);
    return state;
}
