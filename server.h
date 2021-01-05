#ifndef MAIN_SERVER_H
#define MAIN_SERVER_H

#include <sstream>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <uWS/uWS.h>
#include "json.hpp"

class Server {
public:
    void run();

private:
    /**
     * Gets the current unix timestamp of the server
     * @return current unix timestamp
     */
    time_t getTimestamp(){ return time(nullptr);}
    void getEvent(int c, char * buffer);

    int notify(uWS::WebSocket<uWS::SERVER> ws, int c);
};


#endif //MAIN_SERVER_H
