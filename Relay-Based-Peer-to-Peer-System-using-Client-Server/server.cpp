#include <bits/stdc++.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char *argv[]) 
{
    if (argc < 2 || argc > 2) 
    {
        cout << endl;
        cout << "FORMAT FOR INPUT: " << argv[0] << " <PORT NUMBER OF THE SERVER>, TERMINATE!!";
        cout << endl;
        return -1;
    }

    int server_id = socket(AF_INET, SOCK_STREAM, 0);

    if (server_id < 0) 
    {
        perror("THERE IS SOME ERRORR IN SOCKET, TERMINATE!!");
        cout << endl;
        return -1;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));

    char buffer[1024];

    int peernode_count = 0;
    int peernode_ids[1024];
    sockaddr_in peernode_addresses[1024];

    int bind_check = bind(server_id, (struct sockaddr *)&server_address, sizeof(server_address));

    if (bind_check < 0) 
    {
        perror("SOME FAILURE ENCOUNTERED IN THE BINDING PROCESS, TERMINATE!!");
        cout << endl;
        return -1;
    }

    cout << "SERVER IS SUCCESSFULLY ESTABLISHED AND RUNNING ON PORT NUMBER = " << ntohs(server_address.sin_port);
    cout << endl;

    listen(server_id, 5);

    sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    while(1) 
    {
        memset(buffer, '\0', sizeof(buffer));

        int client_id = accept(server_id, (struct sockaddr *)&client_address, &client_address_len);

        if (client_id < 0)
        {
            perror("ACCEPT ERROR, COULDN'T ACCEPT CONNECTION");
            cout << endl;
            return -1;
        }
        cout << "CONNECTION ACCEPTED";
        cout << endl;

        int recv_check = recv(client_id, buffer, 1024, 0);
        if (recv_check < 0) 
        {
            perror("SERVER HAS NOR RECIVED THE REQUIRED MESSAGE");
            cout << endl;
            return -1;
        }

        char *type = strtok(buffer, "#");
        char *temp = strtok(NULL, "#");
        cout << "MESSAGE: " << temp << endl;
        temp = strtok(NULL, "#");

        int portno;
        if (temp != NULL) 
        {
            portno = atoi(temp);
        }

        char message[] = "HELLO THERE!! THIS IS THE SERVER...";
        int message_len = strlen(message);

        if (*type == '1') 
        {
            peernode_ids[peernode_count] = client_id;
            peernode_addresses[peernode_count].sin_family = client_address.sin_family;
            peernode_addresses[peernode_count].sin_port = portno;
            peernode_addresses[peernode_count].sin_addr.s_addr = client_address.sin_addr.s_addr;

            cout << "PEER NODE PORT: " << portno << endl;
            cout << "PEER NODE IP: " << inet_ntoa(client_address.sin_addr) << endl;

            if (send(client_id, message, message_len, 0) != message_len) 
            {
                perror("MESSAGE NOT SENT FROM THE SERVER SIDE"); 
                cout << endl;
                return -1;
            }

            peernode_count++;
        } 
        
        else if (*type == '0') 
        {
            char ip_addr[1024];

            cout << "PEER CLIENT PORT: " << ntohs(client_address.sin_port) << endl;
            inet_ntop(AF_INET, &(client_address.sin_addr), ip_addr, 1024);
            cout << "PEER CLIENT IP: " << ip_addr << endl;

            sprintf(message, "%s$%d", message, peernode_count);
            cout << "TOTAL NUMBER OF PEER NODES = " << peernode_count << endl;

            for (int i = 0; i < peernode_count; i++) 
            {
                inet_ntop(AF_INET, &(peernode_addresses[i].sin_addr), ip_addr, 1024);
                sprintf(message, "%s:%s:%d", message, ip_addr, peernode_addresses[i].sin_port);
            }

            if (send(client_id, message, strlen(message), 0) != strlen(message)) 
            {
                perror("MESSAGE NOT SENT");
                cout << endl;
                exit(-1);
            }
        } 
        
        else 
        {
            perror("INVALID CLIENT REQUEST");
            cout << endl;
            return -1;
        }

        cout << endl;
    }

    close(server_id);
    return 0;
}
