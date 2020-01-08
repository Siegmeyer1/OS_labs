#include <iostream>
#include <string>
#include <sstream>
#include <zconf.h>
#include <csignal>
#include <unordered_map>
#include "functions.h"

int main (int argc, char* argv[]) {
    int id = std::atoi(argv[1]);
    int parent_id = std::atoi(argv[2]);
    int parent_port = std::atoi(argv[3]);

    zmq::context_t context(3);
    zmq::socket_t parent_socket(context, ZMQ_REP);

    parent_socket.connect(get_port_name(parent_port));

    std::unordered_map<int, zmq::socket_t> sockets;
    std::unordered_map<int, int> pids;
    std::unordered_map<int, int> ports;

    while(true) {
        std::string request_string;
        request_string = recieve_message(parent_socket);
        std::istringstream command_stream(request_string);
        std::string command;
        command_stream >> command;
        if (command == "id") {
            std::string parent_string = "Ok:" + std::to_string(id);
            send_message(parent_socket, parent_string);
        } else if (command == "pid") {
            std::string parent_string = "Ok:" + std::to_string(getpid());
            send_message(parent_socket, parent_string);
        } else if (command == "create") {
            int size, node_id;
            command_stream >> size;
            std::vector<int> path(size);
            for (int i = 0; i < size; ++i) {
                command_stream >> path[i];
            }
            command_stream >> node_id;
            if (size == 0) {
                sockets.emplace(std::piecewise_construct,
                                std::forward_as_tuple(node_id),
                                std::forward_as_tuple(context, ZMQ_REQ));
                int port = bind_socket(sockets.at(node_id));
                int pid = fork();
                if (pid == -1) {
                    send_message(parent_socket, "Cannot fork");
                    continue;
                } else if (pid == 0) {
                    create_node(node_id, id, port);
                } else {
                    ports[node_id] = port;
                    pids[node_id] = pid;
                    send_message(sockets.at(node_id), "pid");
                    send_message(parent_socket, recieve_message(sockets.at(node_id)));
                }
            } else {
                int next_id = path.front();
                path.erase(path.begin());
                std::ostringstream msg_stream;
                msg_stream << "create " << path.size();
                for (int i : path) {
                    msg_stream << " " << i;
                }
                msg_stream << " " << node_id;
                send_message(sockets.at(next_id), msg_stream.str());
                send_message(parent_socket, recieve_message(sockets.at(next_id)));
            }
        } else if (command == "remove") {
            int size, node_id;
            command_stream >> size;
            std::vector<int> path(size);
            for (int i = 0; i < size; ++i) {
                command_stream >> path[i];
            }
            command_stream >> node_id;
            if (path.empty()) {
                send_message(sockets.at(node_id), "kill");
                recieve_message(sockets.at(node_id));
                kill(pids[node_id], SIGTERM);
                kill(pids[node_id], SIGKILL);
                pids.erase(node_id);
                sockets.at(node_id).disconnect(get_port_name(ports[node_id]));
                ports.erase(node_id);
                sockets.erase(node_id);
                send_message(parent_socket, "Ok");
            } else {
                int next_id = path.front();
                path.erase(path.begin());
                std::ostringstream msg_stream;
                msg_stream << "remove " << path.size();
                for (int i : path) {
                    msg_stream << " " << i;
                }
                msg_stream << " " << node_id;
                send_message(sockets.at(next_id), msg_stream.str());
                send_message(parent_socket, recieve_message(sockets.at(next_id)));
            }
        } else if (command == "exec") {
            int path_size;
            std::string str, substr;
            command_stream >> path_size;
            std::vector<int> path(path_size);
            for (int i = 0; i < path_size; ++i) {
                command_stream >> path[i];
            }
            command_stream >> str;
            command_stream >> substr;
            if (path.empty()) {
                std::string msg_string("Ok ");
                msg_string += std::to_string(id);
                int pos = str.find(substr);
                if (pos != std::string::npos) {
                    msg_string += ": " + std::to_string(pos);
                } else {
                    msg_string += ": no matches";
                }
                send_message(parent_socket, msg_string);
            } else {
                int next_id = path.front();
                path.erase(path.begin());
                std::ostringstream message_stream;
                message_stream << "exec " << path.size();
                for (int i : path) {
                    message_stream << " " << i;
                }
                message_stream << " " << str << " " << substr;
                send_message(sockets.at(next_id),message_stream.str());
                send_message(parent_socket, recieve_message(sockets.at(next_id)));
            }
        } else if (command == "heartbit") {
            std::ostringstream res;
            for (auto& [child_id, child_socket] : sockets) {
                send_message(child_socket, "heartbit");
                std::string local_result = recieve_message(child_socket);
                if (!local_result.empty() && local_result.substr(std::min<int>(local_result.size(),5)) != "Error") {
                    res << local_result << " ";
                }
            }
            res << id << " ";
            send_message(parent_socket, res.str());
        } else if (command == "kill") {
            for (auto& [child_id, child_socket] : sockets) {
                send_message(child_socket, "kill");
                recieve_message(child_socket);
                kill(pids[child_id], SIGTERM);
                kill(pids[child_id], SIGKILL);
            }
            send_message(parent_socket, "Ok");
        }
        if (parent_port == 0) {
            break;
        }
    }
}
