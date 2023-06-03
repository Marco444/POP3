#include "../pop3_states.h"
#include <stdio.h>
#include <sys/socket.h>
#include "../commands/write_list.h"
#include <stdio.h>

void on_arrival_auth(const unsigned state, struct selector_key *key){ 
       return; 
}

void on_departure_auth(const unsigned state, struct selector_key *key){ return; }

enum pop3_states on_read_ready_auth(struct selector_key *key) { 
    selector_set_interest_key(key, OP_WRITE);
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
        for (size_t i = elem->offset; buff[i] != '\n'; i++)
        {
            if(buffer_can_write(&((struct connection_state *)key->data)->commands.write_buffer))
                buffer_write(&((struct connection_state *)key->data)->commands.write_buffer, buff[i]);
            else{
                elem->offset = i;
                elem->is_done = false;
                selector_set_interest_key(key, OP_WRITE);
                return AUTHORIZATION_STATE;
            }    
        }
        buffer_write(&((struct connection_state *)key->data)->commands.write_buffer, '\n');
        dequeue(((struct connection_state *)key->data)->commands.write_list,&elem);
        return AUTHORIZATION_STATE;
        }
    case USER:
        /* code */
        break;
    case PASS:
        /* code */
        break;
    default :
        /*ERROR*/
        break;
    }
    
    return 0; 
}

enum pop3_states on_block_ready_auth(struct selector_key *key){ 

    char buff[1] = {0};
    write(key->fd, buff, 1);
    return 0; 
}
