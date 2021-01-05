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
void Server::getEvent(int c, char* buffer){

    //create new JSON object
    nlohmann::json event;

    //populate JSON object
    event["c"] = c;
    event["ts"] = getTimestamp();

    //convert JSON object to string (string_view's are more efficient)
    std::string_view outgoing_string = event.dump();

    //copy the JSON string in the buffer
    snprintf(buffer, 64, "%s", outgoing_string);
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
    getEvent(c, message);

    //std::cout << message << std::endl;

    //send the event to the websocket client
    ws.send(message, strlen(message), uWS::OpCode::TEXT);
    return 0;
}