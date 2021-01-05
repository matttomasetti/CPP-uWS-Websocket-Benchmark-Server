#include "server.h"


/**
 * Starts the websocket server.
 * Also specifies custom logic for when a new connection
 * or a new message is received by the server
 */
void Server::run(){
    uWS::Hub h;

    h.onConnection([&](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req){

        //send client initial timestamp
        notify(ws, 0);
    });

    h.onMessage([this](uWS::WebSocket<uWS::SERVER> ws, char *message, size_t length, uWS::OpCode opCode) {

        //read in incoming message from client
        char incoming_buffer[16];
        memcpy( incoming_buffer, &message[0], length );
        incoming_buffer[length] = '\0';

        //parse incoming message to a JSON object
        nlohmann::json incoming_message = nlohmann::json::parse(incoming_buffer);

        //send client timestamp with message count "c"
        notify(ws, incoming_message["c"]);
    });

    h.listen(8080);
    h.run();
}


/**
 * Constructs a JSON string containing the message count and the current timestamp
 * @param c the count of the message
 * @param buffer pointer to the char array the event will be written into
 * @return a JSON string of the message count and the current timestamp
 */
std::string Server::getEvent(int c){

    //create new JSON object
    nlohmann::json event;

    //populate JSON object
    event["c"] = c;
    event["ts"] = getTimestamp();
    
    // Convert JSON object to string.
    // A string_view would be more efficient here, which worked on CLion on OSX,
    // however, string_view would throw a segmentation fault on Ubuntu.
    std::string outgoing_string = event.dump();

    return outgoing_string;
}


/**
 * Sends the client a JSON string containing the message count and the current timestamp
 * @param ws the connection to the websocket client
 * @param c the count of the message
 * @return 0
 */
int Server::notify(uWS::WebSocket<uWS::SERVER> ws, int c){
    char message[64];

    //get the event string
    strcpy(message, getEvent(c).c_str());

    std::cout << message << std::endl;

    //send the event to the websocket client
    ws.send(message, strlen(message), uWS::OpCode::TEXT);
    return 0;
}
