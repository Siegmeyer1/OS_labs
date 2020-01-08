#ifndef SRC_FUNCTIONS_H
#define SRC_FUNCTIONS_H

#include <string>
#include <zconf.h>
#include "zmq.hpp"

bool send_message(zmq::socket_t& socket, const std::string& message_string);

std::string recieve_message(zmq::socket_t& socket);

std::string get_port_name(int port);

int bind_socket(zmq::socket_t& socket);

void create_node(int id, int parent_id, int port);

#endif //SRC_FUNCTIONS_H
