#include <enet.h>
#include "engine.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <thread>

#define SERVER_PORT 6777
#define SLEEP_MS 10

struct PlayerInfo {
    EVec position;
    PlayerColor color;
};

std::unordered_map<ENetPeer*, PlayerInfo> players;  // Map of connected players and their states

ENetHost* server;

void StartServer();
void ProcessPackets();
void StopServer();

int main() {
    StartServer();

    while (true) {
        ProcessPackets();

        ENetEvent event;
        while (enet_host_service(server, &event, 0) > 0) {
            if (event.type == ENET_EVENT_TYPE_CONNECT) {
                char ip[INET6_ADDRSTRLEN];
                enet_address_get_host_ip(&event.peer->address, ip, sizeof(ip));
                std::cout << "A new client connected from " << ip << ":" << event.peer->address.port << std::endl;

                players[event.peer] = PlayerInfo{{960.0f, 540.0f}};  // Start at a default position
            } else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                if (players.find(event.peer) == players.end()) {
                    // This is the first packet from this client, and it should contain the player's color
                    PlayerColor* receivedColor = (PlayerColor*)event.packet->data;
                    players[event.peer].color = *receivedColor;
                    std::cout << "Received color from client: " << (int)receivedColor->r << ", "
                            << (int)receivedColor->g << ", " << (int)receivedColor->b << std::endl;
                } else {
                    // Handle movement updates
                    EVec* receivedMovementDelta = (EVec*)event.packet->data;
                    PlayerInfo& playerInfo = players[event.peer];
                    playerInfo.position.x += receivedMovementDelta->x;
                    playerInfo.position.y += receivedMovementDelta->y;
                }

                enet_packet_destroy(event.packet);
            } else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
                std::cout << "Client disconnected." << std::endl;
                players.erase(event.peer);  // Remove the player from the map
            }
        }

        // Broadcast all player positions to all clients
        for (auto& pair : players) {
            ENetPeer* peer = pair.first;
            std::vector<std::pair<EVec, PlayerColor>> playerData;

            for (auto& player : players) {
                playerData.push_back({player.second.position, player.second.color});
            }

            ENetPacket* packet = enet_packet_create(playerData.data(), playerData.size() * sizeof(std::pair<EVec, PlayerColor>), ENET_PACKET_FLAG_RELIABLE);
            enet_peer_send(peer, 0, packet);
        }
    }

    StopServer();
    return 0;
}

void StartServer() {
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet." << std::endl;
        exit(EXIT_FAILURE);
    }

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = SERVER_PORT;

    server = enet_host_create(&address, 32, 2, 0, 0);

    if (server == NULL) {
        std::cerr << "An error occurred while trying to create an ENet server host." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Server started on port " << SERVER_PORT << "." << std::endl;
}

void StopServer() {
    enet_host_destroy(server);
    enet_deinitialize();
}

void ProcessPackets() {
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
}
