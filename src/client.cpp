//c++ std lib
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <memory>
using std::shared_ptr;
using std::make_shared;
#include <vector>
using std::vector;

//3rd party
#include "client_ws.hpp"

//homebrew
#include "utils.hpp"

using Ws_client = SimpleWeb::SocketClient<SimpleWeb::WS>;

string make_address(int argc, char** argv);
void help();

int main(int argc, char** argv) {
    string address(make_address(argc, argv));
    if(address.size() == 0) {
        cout << "could not formulate address, please verify parameters" << endl
             << "the program will now terminate" << endl;
        help();
        return -1;
    }

    cout << "will connect to: \"" << address << "\"" << endl;

    Ws_client client(address);

    client.on_message =
        [](shared_ptr<Ws_client::Connection> connection,
           shared_ptr<Ws_client::Message> message)
    {
        cout << "Client: Message received: \"" << message->string() << "\""
             << endl;

        cout << "Client: Sending close connection" << endl;
        connection->send_close(1000);
    };

    client.on_open = [](shared_ptr<Ws_client::Connection> connection) {
        cout << "Client: Opened connection" << endl;

        /* strings we want to send (various things to check if all question
         * marks are converted to exclamation marks without any unexpected
         * results) */
        vector<string> messages;
        messages.push_back(string("Hello? Is anybody listening? Am I alone?"));
        messages.push_back(string("?"));
        messages.push_back(string("???"));
        messages.push_back(string("!!!"));
        messages.push_back(string("? Woot? Yay!"));

        auto send_stream = make_shared<Ws_client::SendStream>();
        for(size_t i {0}; i < messages.size(); ++i) {
            cout << "Client: Sending message: \"" << messages[i]
                 << "\"" << endl;

            *send_stream << messages[i];
            connection->send(send_stream);
        }

        //*send_stream << "non_text";
        ////130 is the websocket opcode indicating binary data (single fragment)
        //connection->send(send_stream, nullptr, 130);
    };

    client.on_close =
        [](shared_ptr<Ws_client::Connection> /*connection*/,
           int status, const string& reason)
    {
        cout << "Client: Closed connection with status code " << status
             << "reason: \"" << reason << "\" "
             << endl;
    };

    /* See
     * http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html,
     * Error Codes for error code meanings */
    client.on_error =
        [](shared_ptr<Ws_client::Connection> /*connection*/,
           const SimpleWeb::error_code &ec)
    {
        cout << "Client: Error: " << ec << ", error message: " << ec.message()
             << endl;
    };

    client.start();
}

string make_address(int argc, char** argv)
{
    const string host("localhost:");
    const string path("/ws");
    string selected_port("");

    if(argc != 2) {
        cout << "please provide port number as argument, when launching program"
             << endl;
        return string("");
    }
    else { selected_port = string(argv[1]); }

    //verify if this is a valid port number
    if(port_num_sanity_check(selected_port) != 0) { return string(""); }

    return string(host + selected_port + path);
}

void help()
{
    cout << "\n*** USAGE ***\n"
         << "When calling, supply port number as argument: ./client <port>\n"
         << "Usage example: ./client 8080" << endl;
}
