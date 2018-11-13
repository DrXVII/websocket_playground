//c++ std lib
#include <climits>
#include <cstring>
#include <string>
using std::string;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <thread>
using std::thread;
#include <chrono>
#include <iostream>
using std::cout;
using std::endl;
using std::cin;

//3rd party
#include "server_ws.hpp"

//homebrew
#include "utils.hpp"

using Ws_server = SimpleWeb::SocketServer<SimpleWeb::WS>;

void replace_char(string* _str, const char _from, const char _to);
int get_port_from_argv(int _argc, char** _argv, unsigned short* port_no_);
void help();

int main(int argc, char** argv) {
    if(argc > 1 && strcmp(argv[1], "-h") == 0) {
        help();
        return 0;
    }

    unsigned short selected_port {0};
    if(get_port_from_argv(argc, argv, &selected_port) != 0) {
        cout << "Fatal error, terminating." << endl;
        help();
        exit(-1);
    }

    // WebSocket (WS)-server using 1 thread
    Ws_server server;
    server.config.port = selected_port;
    cout << "selected port: " << selected_port << endl;

    // echo WebSocket endpoint
    auto &echo = server.endpoint["^/ws/?$"];

    echo.on_message =
        [](shared_ptr<Ws_server::Connection> connection,
           shared_ptr<Ws_server::Message> message)
    {
        string message_str = message->string();

        if((message->fin_rsv_opcode & 0x1) != 0x1) {
            cout << "Server: Received non-text data, closing connection"
                 << endl;
            connection->send_close(1000, "non-text payload received");
            return;
        }

        cout << "Server: Message received: \"" << message_str << "\" "
             << "from " << connection.get() << endl;

        //replace all question marks with exclamation marks
        replace_char(&message_str, '?', '!');

        cout << "Server: Sending response \"" << message_str
             << "\" to " << connection.get() << endl;

        auto send_stream = make_shared<Ws_server::SendStream>();
        *send_stream << message_str;
        // connection->send is an asynchronous function
        connection->send(send_stream, [](const SimpleWeb::error_code &ec) {
            /* See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html,
             * Error Codes for error code meanings */
            if(ec) {
                cout << "Server: Error sending message. "
                     << "Error: " << ec << ", error message: " << ec.message()
                     << endl;
            }
        });
    };

    echo.on_open = [](shared_ptr<Ws_server::Connection> connection) {
        cout << "Server: Opened connection " << connection.get() << endl;
    };

    // See RFC 6455 7.4.1. for status codes
    echo.on_close =
        [](shared_ptr<Ws_server::Connection> connection,
           int status, const string& reason) {
        cout << "Server: Closed connection " << connection.get() << " "
             << "with status code " << status << " "
             << "reason: " << reason
             << endl;
    };

    /* See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html,
     * Error Codes for error code meanings */
    echo.on_error =
        [](shared_ptr<Ws_server::Connection> connection,
           const SimpleWeb::error_code &ec) {
        cout << "Server: Error in connection " << connection.get() << ". "
                 << "Error: " << ec << ", error message: " << ec.message() << endl;
    };

    thread server_thread([&server]() {
        // Start WS-server
        server.start();
    });

    //waiting for the server to start up
    std::this_thread::sleep_for(std::chrono::seconds(1));

    cout << "to kill the server, type in 'q' and pres enter" << endl;
    char cmd {' '};
    while(cmd != 'q') {
        cin >> cmd;
    }

    cout << "stopping the server..." << endl;
    server.stop();

    server_thread.join();

    return 0;
}

void help()
{
    cout << "\n*** USAGE ***\n"
         << "When calling, supply port number as argument: ./server <port>\n"
         << "Usage example: ./server 8080" << endl;
}

void replace_char(string* _str, const char _from, const char _to)
{
    for(size_t i {0}; i < _str->size(); ++i) {
        if((*_str)[i] == _from) {(*_str)[i] = _to;}
    }
}

int get_port_from_argv(int _argc, char** _argv, unsigned short* port_no_) {
    if(_argc != 2) {
        cout << "You must supply port number as first executable argumaent!"
             << endl;
        return -1;
    }

    //let's see if this is an OK port number
    if(port_num_sanity_check(string(_argv[1])) != 0) { return -1; }
    else { *port_no_ = (unsigned short) stoi(string(_argv[1])); }

    //int port_arg {0}; //buffer to store the full value returned by stoi()

    //try {
    //    if(_argc == 2) { port_arg = stoi(string(_argv[1])); }
    //}
    //catch(std::invalid_argument& e) {
    //    cout << "ERROR: malformed port number. Exception: " << e.what() << endl;
    //    return -1;
    //}
    //catch(std::out_of_range& e) {
    //    cout << "ERROR: port number too large. Exception: " << e.what() << endl;
    //    return -1;
    //}

    //if(port_arg < 0) {
    //    cout << "ERROR: negative port numbers not supported." << endl;
    //    return -1;
    //}
    //else if(port_arg > USHRT_MAX) {
    //    cout << "ERROR: port number too large, max supported: " << USHRT_MAX
    //         << endl;
    //    return -1;
    //}
    //else { *port_no_ = (unsigned short) port_arg; }

    return 0;
}
