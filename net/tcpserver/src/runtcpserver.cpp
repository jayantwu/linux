#include "tcpserver.h"
#include <iostream>
#include <signal.h>

using namespace std;

void sig_child(int signo)
{
    pid_t pid;
    int stat;
    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0 ) {
        cout << "child " <<  pid << " termianted." << endl;
    }
    return;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        cout << "using ./tcpserver port" <<  endl;
        return -1;
    }
    
    tcpserver server;
    if (server.init_server(atoi(argv[1])) == false) {
        perror("init_server()");
        return -1;
    }

    signal(SIGCHLD, sig_child);

    while (true) {
        if (server.accept() == -1) {
            perror("accept()");
            return -1;
        }

        cout << "client connected. client ip: " << server.clientip() << endl;
        
        int pid = fork();
        if (pid == -1) {
            perror("fork()");
            return -1;
        }
        if (pid > 0) {
            server.closeclient();
            continue;  // parent process
        }
        
        // child process
        server.closelisten();

        while (true) {
            string buffer;
            if (server.recv(buffer, 1024) == false) {
                perror("recv");
                break;
            }
            cout << "recv: " << buffer << endl;

            buffer = "server recv ok!";

            if (server.send(buffer) == false) {
                perror("send");
                break;
            }

            cout << "send: " << buffer;

        }
        return 0;
    }
}