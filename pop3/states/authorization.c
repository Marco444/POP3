#include "../pop3_states.h"
#include <stdio.h>
#include <sys/socket.h>
#include "../commands/write_list.h"
#include <stdio.h>
void write_in_buffer(elem_type elem,struct selector_key * key,char * buff);


void on_arrival_auth(const unsigned state, struct selector_key *key){ 
       return; 
}

void on_departure_auth(const unsigned state, struct selector_key *key){ return; }

enum pop3_states on_read_ready_auth(struct selector_key *key) { 
    return read_commands(key, AUTHORIZATION_STATE, true);
}

enum pop3_states on_write_ready_auth(struct selector_key *key){
    selector_set_interest_key(key, OP_READ);
    elem_type elem =  peek(((struct connection_state *)key->data)->commands.write_list);
    if(elem == NULL)
        return ERROR_STATE;
    switch (elem->cmd_id)
    {
    case EHLO:{
        char buff[100] = "+OK POP3 server ready\r\n";
        write_in_buffer(elem,key,buff);
        return AUTHORIZATION_STATE;
        }
        break;
    case USER:{
        char buff[100] = "+OK User\r\n";
        write_in_buffer(elem,key,buff);
        return AUTHORIZATION_STATE;  }  
        break;
    case PASS:{
        char buff[100] = "+OK Pass\r\n";
        write_in_buffer(elem,key,buff); 
        return TRANSACTION_STATE;
        }
        break;
    default :
        break;
    }
    
    return 0; 
}

enum pop3_states on_block_ready_auth(struct selector_key *key){ 

    char buff[1] = {0};
    write(key->fd, buff, 1);
    return 0; 
}
