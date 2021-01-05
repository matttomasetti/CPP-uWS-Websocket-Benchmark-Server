#include "server.h"


int main()
{
    //call the websocket server
    Server *s = new Server();
    s->run();

    return 0;
}