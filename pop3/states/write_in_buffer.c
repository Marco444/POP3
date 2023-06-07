
#include "../commands/command_service.h"
#include "../pop3_states.h"
#include <stdio.h>
#include <sys/socket.h>
bool write_in_buffer(elem_type elem,struct selector_key * key,char * buff){
    for (size_t i = elem->offset; buff[i] != '\n'; i++)
        {
            if(buffer_can_write(&((struct connection_state *)key->data)->commands.write_buffer))
                buffer_write(&((struct connection_state *)key->data)->commands.write_buffer, buff[i]);
            else{
                elem->offset = i;
                elem->is_done = false;
                selector_set_interest_key(key, OP_WRITE);
            }    
        }
        if(buffer_can_write(&((struct connection_state *)key->data)->commands.write_buffer)){
            buffer_write(&((struct connection_state *)key->data)->commands.write_buffer, '\n');
            //free(elem);
            return true;
        }
        return false;
}