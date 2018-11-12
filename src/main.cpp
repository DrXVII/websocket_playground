#include "client_ws.hpp"
#include "server_ws.hpp"

using namespace std;

using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;
using WsClient = SimpleWeb::SocketClient<SimpleWeb::WS>;

void replace_char(string* _str, const char _from, const char _to);

int main() {
    // WebSocket (WS)-server at port 8080 using 1 thread
    WsServer server;
    server.config.port = 8080;

    // echo WebSocket endpoint
    auto &echo = server.endpoint["^/echo/?$"];

    echo.on_message =
        [](shared_ptr<WsServer::Connection> connection,
           shared_ptr<WsServer::Message> message)
        {
        auto message_str = message->string();

        //if(message->fin_rsv_opcode != WS_DATA_TYPE_TXT) {
        //    cout << "Server: Received non-text data, closing connection"
        //         << endl;
        //    connection->send_close(999);
        //    return;
        //}

        cout << "Server: Message received: \"" << message_str << "\" "
             << "from " << connection.get() << endl;

        cout << "Server: Creating response..." << endl;
        replace_char(&message_str, '?', '!');

        cout << "Server: Sending response \"" << message_str
             << "\" to " << connection.get() << endl;

        auto send_stream = make_shared<WsServer::SendStream>();
        *send_stream << message_str;
        // connection->send is an asynchronous function
        connection->send(send_stream, [](const SimpleWeb::error_code &ec) {
            if(ec) {
                cout << "Server: Error sending message. " <<
                        // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
                        "Error: " << ec << ", error message: " << ec.message() << endl;
            }
        });
    };

    echo.on_open = [](shared_ptr<WsServer::Connection> connection) {
        cout << "Server: Opened connection " << connection.get() << endl;
    };

    // See RFC 6455 7.4.1. for status codes
    echo.on_close = [](shared_ptr<WsServer::Connection> connection, int status, const string & /*reason*/) {
        cout << "Server: Closed connection " << connection.get() << " with status code " << status << endl;
    };

    // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
    echo.on_error = [](shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec) {
        cout << "Server: Error in connection " << connection.get() << ". "
                 << "Error: " << ec << ", error message: " << ec.message() << endl;
    };

    //--------------------------------------------------------------------------

    thread server_thread([&server]() {
        // Start WS-server
        server.start();
    });

    // Wait for server to start so that the client can connect
    this_thread::sleep_for(chrono::seconds(1));

    // Example 4: Client communication with server
    // Possible output:
    //     Server: Opened connection 0x7fcf21600380
    //     Client: Opened connection
    //     Client: Sending message: "Hello"
    //     Server: Message received: "Hello" from 0x7fcf21600380
    //     Server: Sending message "Hello" to 0x7fcf21600380
    //     Client: Message received: "Hello"
    //     Client: Sending close connection
    //     Server: Closed connection 0x7fcf21600380 with status code 1000
    //     Client: Closed connection with status code 1000
    WsClient client("localhost:8080/echo");
    client.on_message = [](shared_ptr<WsClient::Connection> connection, shared_ptr<WsClient::Message> message) {
        cout << "Client: Message received: \"" << message->string() << "\"" << endl;

        cout << "Client: Sending close connection" << endl;
        connection->send_close(1000);
    };

    client.on_open = [](shared_ptr<WsClient::Connection> connection) {
        cout << "Client: Opened connection" << endl;

        //remembering things we want to send
        vector<string> messages;
        //string message = "Hello? Is anybody listening? Am I alone?";
        messages.push_back(string("Hello? Is anybody listening? Am I alone?"));
        messages.push_back(string("?"));
        messages.push_back(string("???"));
        messages.push_back(string("!!!"));
        messages.push_back(string("? Woot? Yay!"));

        auto send_stream = make_shared<WsClient::SendStream>();
        for(size_t i {0}; i < messages.size(); ++i) {
            cout << "Client: Sending message: \"" << messages[i]
                 << "\"" << endl;

            *send_stream << messages[i];
            connection->send(send_stream);
        }

        //*send_stream << "before non_text";
        //connection->send(send_stream);

        //*send_stream << "non_text";
        //connection->send(send_stream, nullptr, WS_DATA_TYPE_BIN);

        //*send_stream << "after non_text";
        //connection->send(send_stream);
    };

    client.on_close = [](shared_ptr<WsClient::Connection> /*connection*/, int status, const string & /*reason*/) {
        cout << "Client: Closed connection with status code " << status << endl;
    };

    // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
    client.on_error = [](shared_ptr<WsClient::Connection> /*connection*/, const SimpleWeb::error_code &ec) {
        cout << "Client: Error: " << ec << ", error message: " << ec.message() << endl;
    };

    client.start();

    server_thread.join();
}

void replace_char(string* _str, const char _from, const char _to)
{
    for(size_t i {0}; i < _str->size(); ++i) {
        if((*_str)[i] == _from) {(*_str)[i] = _to;}
    }
}
