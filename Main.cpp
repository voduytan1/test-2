#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 2225;

int main() {
    // Khởi tạo Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock\n";
        return 1;
    }

    // Tạo socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket\n";
        WSACleanup();
        return 1;
    }

    // Cấu hình thông tin server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Kết nối đến server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to the server\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Nhận phản hồi từ server
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Server response: " << buffer << std::endl;

    // Gửi lệnh EHLO để bắt đầu phiên làm việc
    std::string ehloCommand = "EHLO localhost\r\n";
    send(clientSocket, ehloCommand.c_str(), ehloCommand.size(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Server response after EHLO: " << buffer << std::endl;

    //// Gửi lệnh AUTH LOGIN để xác thực (Base64 encode)
    //std::string authCommand = "AUTH LOGIN\r\n";
    //send(clientSocket, authCommand.c_str(), authCommand.size(), 0);
    //memset(buffer, 0, sizeof(buffer));
    //recv(clientSocket, buffer, sizeof(buffer), 0);
    //std::cout << "Server response after AUTH LOGIN: " << buffer << std::endl;

    //// Gửi tên đăng nhập và mật khẩu (Base64 encode)
    //std::string username = "your_base64_encoded_username"; // Đổi thành tên đăng nhập của bạn (Base64 encode)
    //std::string password = "your_base64_encoded_password"; // Đổi thành mật khẩu của bạn (Base64 encode)

    //send(clientSocket, (username + "\r\n").c_str(), username.size() + 2, 0);
    //memset(buffer, 0, sizeof(buffer));
    //recv(clientSocket, buffer, sizeof(buffer), 0);
    //std::cout << "Server response after username: " << buffer << std::endl;

    //send(clientSocket, (password + "\r\n").c_str(), password.size() + 2, 0);
    //memset(buffer, 0, sizeof(buffer));
    //recv(clientSocket, buffer, sizeof(buffer), 0);
    //std::cout << "Server response after password: " << buffer << std::endl;

    // Gửi lệnh MAIL FROM
    std::string mailFromCommand = "MAIL FROM:<8@gmail.com>\r\n";
    send(clientSocket, mailFromCommand.c_str(), mailFromCommand.size(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Server response after MAIL FROM: " << buffer << std::endl;

    // Gửi lệnh RCPT TO
    std::string rcptToCommand = "RCPT TO:<n8@gmail.com>\r\n";
    send(clientSocket, rcptToCommand.c_str(), rcptToCommand.size(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Server response after RCPT TO: " << buffer << std::endl;

    // Gửi lệnh DATA
    std::string dataCommand = "DATA\r\n";
    send(clientSocket, dataCommand.c_str(), dataCommand.size(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Server response after DATA: " << buffer << std::endl;

    // Gửi nội dung email
    std::string emailContent = "Subject: Test Email\r\n"
        "To: n8@gmail.com\r\n"
        "\r\n"
        "This is a test email sent from C++ using Winsock.\r\n"
        ".\r\n"; // Dấu chấm cuối cùng để kết thúc nội dung

    send(clientSocket, emailContent.c_str(), emailContent.size(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Server response after email content: " << buffer << std::endl;

    // Gửi lệnh QUIT để kết thúc phiên làm việc
    std::string quitCommand = "QUIT\r\n";
    send(clientSocket, quitCommand.c_str(), quitCommand.size(), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Server response after QUIT: " << buffer << std::endl;

    // Đóng kết nối
    closesocket(clientSocket);

    // Cleanup Winsock
    WSACleanup();

    return 0;
}
