#include <Socket/Server.hpp>
#include <Socket/Client.hpp>
#include <Websocket/Websocket.hpp>

// Пример без Tls
#ifndef WEBSOCKET_TLS_SUPPORT
int main(int argc, char* argv[])
{
#ifdef _WIN32
    Socket::InitializeWSA(MAKEWORD(2, 2));
#endif

    Server* server = Server::Bind("8081");
    Websocket websocket;
    Client* client;

    if (!server)
        return -1;

    if (!server->Listen(10))
        return -1;

    while (true)
    {
        client = server->Accept();

        if (!client)
            continue;

        websocket.ChangeClient(client);
        if (!websocket.Handshake())
        {
            delete client;
            continue;
        }

        FrameData frame = websocket.ReadFrame(10l);
        if (!frame.payload)
        {
            delete client;
            continue;
        }

        websocket.SendFrame(frame.payload, frame.payloadLength);
        websocket.Close(StatusCode::AllowedInClose::NormalClosure);

        delete client;
    }

    websocket.ChangeClient(nullptr);

    delete server;
#ifdef _WIN32
    Socket::CleanupWSA();
#endif
    
    return 0;
}
#endif
