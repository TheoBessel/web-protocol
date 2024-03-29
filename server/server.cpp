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
            //outValue = readFile("../../../server/testfile.txt");
            outValue = "HTTP/1.1 200 OK\n"
                       "Content-Type: text/HTML\n"
                       "Content-Length: 1000\n"
                       "\n"
                       "<!DOCTYPE html>\n"
                       "\n"
                       "<html>\n"
                       "<head>\n"
                       "    <title>Test HTTP Server</title>\n"
                       "    <link rel=\"stylesheet\" type=\"text/css\" href=\"https://cdnjs.cloudflare.com/ajax/libs/semantic-ui/2.4.1/semantic.css\">\n"
                       "</head>\n"
                       "<body>\n"
                       "    <h1>Welcome to my test server !</h1>\n"
                       "    <p>If you can read those lines, it means that the server is working perfectly :)</p>\n"
                       "    <div class=\"ui primary button\">Hellow, I'm a button</div>\n"
                       "    <a href=\"https://google.com\">An anchor example</a>\n"
                       "</body>\n"
                       "</html>";
            if(!writeValue(outValue))
                throw std::runtime_error("Cannot write returned value !");
            std::cout << "Returned value : " << outValue << std::endl;
            close(newSocket);
        }
    }
};

int main() {
    TCPServer server1;
    int port = atoi(readFile("/var/portfile.txt").c_str());
    std::cout << "$PORT = " << port << std::endl;
    server1.createTCPServer(port);
    return 0;
}
