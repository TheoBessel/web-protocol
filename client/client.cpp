#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

class TCPClient {
public:
    TCPClient() {}
    bool createTCPClient(const std::string& ip, const int& port) {
        if(!createWebSocket())
            throw std::runtime_error("Cannot create WebSocket !");
        if(!convertAddress(ip, port))
            throw std::runtime_error("Cannot convert ip address !");
        if(!connectToServer())
            throw std::runtime_error("Cannot connect to web server !");
        if(!requestWebServer())
            throw std::runtime_error("Cannot request web server !");
    }
private:
    int clientSocket;
    struct sockaddr_in serverAddress;
    std::string inValue;
    std::string outValue;

    bool createWebSocket() {
        bool output = ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) != 0);
        std::cerr << "WebSocket create result : " << output << std::endl;
        return output;
    }
    bool convertAddress(const std::string ip, const int& port) {
        memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        bool output = inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr) > 0;
        std::cerr << "Address conversion result : " << output << std::endl;
        return output;
    }
    bool connectToServer() {
        bool output = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == 0;
        std::cerr << "Connection result : " << output << std::endl;
        return output;
    }
    bool sendValue(std::string& value) {
        bool output = send(clientSocket, value.c_str() , value.length(), 0) >= 0;
        std::cerr << "Read result : " << output << std::endl;
        return output;
    }
    bool readValue() {
        inValue.resize(30000);
        auto readsize = read(clientSocket, inValue.data(), 30000);
        inValue.resize(readsize);
        bool output = readsize >= 0;
        std::cerr << "Read size : " << readsize << std::endl;
        std::cerr << "Read result : " << output << std::endl;
        return output;
    }
    bool requestWebServer() {
        inValue = "";
        outValue = "Hello, I'm the Client !";
        if(!sendValue(outValue))
            throw std::runtime_error("Cannot send value !");
        std::cout << "Returned value : " << outValue << std::endl;
        if(!readValue())
            throw std::runtime_error("Cannot read value !");
        std::cout << "Received value : " << inValue << std::endl;
        close(clientSocket);
        return true;
    }
};

int main() {
    TCPClient client1;
    client1.createTCPClient("127.0.0.1", 8080);
    return 0;
}