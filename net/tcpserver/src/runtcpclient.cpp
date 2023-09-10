#include "tcpclient.h"
#include <iostream>


using namespace std;

int main(int argc, char** argv)
{
    tcpclient client;

    if (client.connect(argv[1], atoi(argv[2])) == false)
    {
        perror("connect()");
        return -1;
    }
    
    string buffer;
    for (int i = 0; i < 10; i++) {
        int iret;
        buffer = "client send data " + to_string(i);
        if (client.send(buffer) == false) {
            perror("send");
            break;
        }

        cout << "send: " << buffer << endl;


        if (client.recv(buffer, 1024) == false) {
            perror("recv()");
            break;
        }

        cout << "recv: " << buffer << endl;

        sleep(1);
    }

    // will close by dctor
    
    return 0;
}