#include "tcpclient.h"
#include <iostream>


using namespace std;

int main(int argc, char** argv)
{
    tcpclient client;
    cout << "start ...." << endl;
    if (client.connect(argv[1], atoi(argv[2])) == false)
    {
        perror("connect()");
        return -1;
    }
    
    char buffer[1024];
    for (int i = 0; i < 10; i++) {
        int iret;
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "client send data %d", i);
        if ((iret = send(client.get_client_fd(), buffer, strlen(buffer), 0)) <= 0) {
            perror("send");
            break;
        }

        cout << "send: " << buffer << endl;

        memset(buffer, 0, sizeof(buffer));

        if ((iret = recv(client.get_client_fd(), buffer, sizeof(buffer), 0)) <= 0) {
            cout << "iret=" << iret << endl;
            break;
        }

        cout << "recv: " << buffer << endl;

        sleep(1);
    }

    close(client.get_client_fd());
    
    return 0;
}