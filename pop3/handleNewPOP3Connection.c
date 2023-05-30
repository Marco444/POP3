int handleNewPOP3Connection() {

    // Initialize the state machine
    struct state_machine stm = {
        .initial   = AUTHORIZATION_STATE,
        .states    = server_states,
        .max_state = SERVER_STATE_COUNT,
    };
    stm_init(&stm);
}
