#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "usual.hpp"

class TCPServer {
public:
    TCPServer() {}
    bool createTCPServer(const int& port) {
        if(!createWebSocket())
            throw std::runtime_error("Cannot create WebSocket !");
        if(!bindServerSocket(port))
            throw std::runtime_error("Cannot bind Server socket !");
        if(!listenToClient())
            throw std::runtime_error("Cannot listen to client request !");
        if(!requestClient())
            throw std::runtime_error("Cannot request client !");
    }

private:
    int serverSocket, newSocket;
    struct sockaddr_in address;
    int addressLength;
    std::string inValue;
    std::string outValue;

    bool createWebSocket() {
        bool output = ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) != 0);
        std::cerr << "WebSocket create result : " << output << std::endl;
        return output;
    }
    bool bindServerSocket(const int& port) {
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);
        memset(address.sin_zero, 0, sizeof address.sin_zero);
        bool output = bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) == 0;
        std::cerr << "Bind result : " << output << std::endl;
        return output;
    }
    bool listenToClient() {
        bool output = listen(serverSocket, 10) == 0;
        std::cerr << "Listen result : " << output << std::endl;
        return output;
    }
    bool acceptWebRequest() {
        bool output = (newSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addressLength)) > 0;
        std::cerr << "Accept result : " << output << std::endl;
        return output;
    }
    bool readValue() {
        inValue.resize(30000);
        auto readsize = read(newSocket, inValue.data(), 30000);
        inValue.resize(readsize);
        bool output = readsize >= 0;
        std::cerr << "Read size : " << readsize << std::endl;
        std::cerr << "Read result : " << output << std::endl;
        return output;
    }
    bool writeValue(std::string& value) {
        bool output = write(newSocket, value.c_str(), sizeof(char)*value.size()) > 0;
        std::cerr << "Write result : " << output << std::endl;
        return output;
    }
    bool requestClient() {
        while(true) {
            inValue = "";
            std::cout << "Server waiting for a request ..." << std::endl;
            if(!acceptWebRequest())
                throw std::runtime_error("Cannot accept client request !");
            if(!readValue())
                throw std::runtime_error("Cannot read received value !");
            std::cout << "Received value : " << inValue << std::endl;
            outValue = readFile("testfile.txt");
            if(!writeValue(outValue))
                throw std::runtime_error("Cannot write returned value !");
            std::cout << "Returned value : " << outValue << std::endl;
            close(newSocket);
        }
    }
};

int main() {
    TCPServer server1;
    server1.createTCPServer(8080);
    return 0;
}