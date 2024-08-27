# Relay-Based Peer-to-Peer System using Client-Server Socket Programming

## Overview

This project implements a simple relay-based peer-to-peer communication system using TCP sockets in C. It involves three main components: Peer_Client, Relay_Server, and Peer_Nodes. These components work together to facilitate communication in a peer-to-peer network.

## Components

### 1. Peer_Client

- **Role:** Acts as the client that connects to the Relay_Server to send and receive messages.
- **Functionality:** Establishes a TCP connection to the Relay_Server and communicates messages to Peer_Nodes via the relay.

### 2. Relay_Server

- **Role:** Functions as an intermediary that forwards messages between Peer_Clients and Peer_Nodes.
- **Functionality:** Receives messages from Peer_Clients and relays them to the appropriate Peer_Nodes, ensuring that messages are properly routed within the network.

### 3. Peer_Nodes

- **Role:** Represents nodes in the peer-to-peer network that interact with Peer_Clients through the Relay_Server.
- **Functionality:** Receives and processes messages forwarded by the Relay_Server, and may also send responses or new messages back to Peer_Clients.

## Objectives

- **Implement TCP Communication:** Establish reliable TCP connections for communication between Peer_Clients, Relay_Server, and Peer_Nodes.
- **Message Relay:** Ensure effective message forwarding through the Relay_Server to facilitate peer-to-peer communication.
- **Socket Programming:** Utilize socket programming in C to handle network communication and data transfer.

## Conclusion

The project demonstrates the implementation of a relay-based peer-to-peer system using TCP sockets. By creating and integrating Peer_Client, Relay_Server, and Peer_Nodes, it achieves efficient message relaying and network communication in a simple peer-to-peer architecture.

## Getting Started

To replicate this analysis, clone the repository and follow the instructions provided in the `setup.md` file.

```bash
git clone https://github.com/vikask-iitg/Relay-Based-Peer-to-Peer-System-using-Client-Server.git
