#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;

#define MAX_PORT_NUM 0xffff

int port_num_sanity_check(const string& _port_no)
{
    int port_arg {0};

    try {
        port_arg = stoi(_port_no);
    }
    catch(std::invalid_argument& e) {
        cout << "ERROR: malformed port number. Exception: " << e.what() << endl;
        return -1;
    }
    catch(std::out_of_range& e) {
        cout << "ERROR: port number too large. Exception: " << e.what() << endl;
        return -1;
    }

    if(port_arg < 0) {
        cout << "ERROR: negative port numbers not supported." << endl;
        return -1;
    }
    else if(port_arg > MAX_PORT_NUM) {
        cout << "ERROR: port number too large, max supported: " << MAX_PORT_NUM
             << endl;
        return -1;
    }

    return 0;
}
