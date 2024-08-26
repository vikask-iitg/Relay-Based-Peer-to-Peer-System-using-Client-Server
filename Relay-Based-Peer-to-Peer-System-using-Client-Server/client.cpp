#include <bits/stdc++.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char *argv[]) 
{
    if(argc > 3 || argc < 3) 
    {
    	cout<<endl;
        cout << "FORMATE FOR INPUT: " << argv[0] << " <IP ADRESS OF THE SERVER> <PORT NUMBER OF THE SERVER> ";
        cout<<endl;
        return 1;
    }

    int socket_id = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_id < 0) 
    {
    	cout<<endl;
        cout << "SOME ERROR ENCOUNTERED IN SOCKET CREATION, TERMINATING!! ";
        cout<<endl;
        return -1;
    }

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[2]));

    char buffer[1024] = {0};

    if(inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0) 
    {
    	cout<<endl;
        cout << "THE IP ADRESS OF SERVER FOUND TO BE INVALID, TERMINATING!!";
        cout<<endl;
        return -1;
    }

    int connection_check = connect(socket_id, (struct sockaddr *)&server_address, sizeof(server_address));

    if(connection_check < 0) 
    {
    	cout<<endl;
        cout << "CONNECTION TO SERVER PROCESS FAILED!!, TERMINATING!! ";
        cout<<endl;
        return -1;
    }

    
    const char *message = "0#HI THERE! THIS IS THE CLIENT.";
    int message_len = strlen(message);

    if(send(socket_id, message, message_len, 0) != message_len) 
    {
        perror("FAILED IN SENDING THE MESSAGE FROM CLIENT TO SERVER!!");
        cout<<endl;
        return -1;
    }

    // Response from relay server
    int recv_check = recv(socket_id, buffer, 1024, 0);
    if(recv_check < 0)
    {
        perror("MESSAGE IS SENT BY THE SERVER, BUT SOMEHOW CLIENT DIDN'T RECIVE IT");
        cout<<endl;
        return -1;
    }

    char *temp = strtok(buffer, "$");
    cout << "SERVER SAYS: " << temp ;
    cout<<endl;
    cout<<endl;
    temp = strtok(NULL, "$");

    int peernode_count = 0;

    peernode_count = atoi(strtok(temp, ":"));
    cout << "PEERNODE COUNT : " << peernode_count;
    cout<<endl;
    cout<<endl;

    temp = strtok(NULL, ":");

    
    char* peernode_ports[peernode_count];
    
    struct sockaddr_in peernode_address[peernode_count];
    int i = 0;

    while(temp != NULL) 
    {
        peernode_address[i].sin_family = AF_INET;

        if(inet_pton(AF_INET, temp, &(peernode_address[i].sin_addr.s_addr)) < 0) 
        {
            cout << "ADDRESS FOUND TO BE INVALID!!!";
            cout<<endl;
            return -1;
        }

        temp = strtok(NULL, ":");

        peernode_address[i].sin_port = htons(atoi(temp));
        temp = strtok(NULL, ":");
        i++;
    }

    
    for(i = 0; i < peernode_count; i++) 
    {
        char ip_addr[1024];
        cout << "IP ADRESS OF PEER NODE: " << inet_ntop(AF_INET, &(peernode_address[i].sin_addr), ip_addr, 1024) ;
        cout<<endl;
        cout << "PORT OF PEER NODE: " << ntohs(peernode_address[i].sin_port);
        cout<<endl;
    }

    close(socket_id);

    
    char filename[1024];
    cout << "PLEASE ENTER THE NAME OF FILE: ";
    cout<<endl;
    cin >> filename; 

    int found = 0;
    for(i = 0; i < peernode_count; i++) 
    { 
        char ip_addr[1024];
        cout << "PEER NODE NUMBER:" << i + 1 ;
        cout<<endl;
        cout << "PEER NODE PORT: " << ntohs(peernode_address[i].sin_port) ;
        cout<<endl;
        cout << "PEER NODE IP: " << inet_ntop(AF_INET, &(peernode_address[i].sin_addr), ip_addr, 1024) ;
        cout<<endl;

        if ((socket_id = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        {
            cout<<endl;
            cout << "SOME ERROR ENCOUNTERED IN CREATION OF SOCKET";
            cout<<endl;
            return -1;
        }

        
        if(connect(socket_id, (struct sockaddr *)&(peernode_address[i]), sizeof(peernode_address[i])) < 0) 
        {
            cout<<endl;
            cout << " FALIUR IN CONNECTION WITH PEER NODE ";
            cout<<endl;
            return -1;
        }

        cout << "SUCESSFULLY CONNECTED TO PEER NODE ";
        cout<<endl;

        if(send(socket_id, filename, strlen(filename), 0) != strlen(filename)) 
        { 
            perror("COULDN'T SEND FILENAME");
            cout<<endl;
            return -1;
        }

        memset(buffer, '\0', 1024);

        if (recv(socket_id, buffer, 1024, 0) < 0) 
        {
            perror("RECEIVE ERROR");
            cout<<endl;
            exit(-1);
        }

        long int filesize;

        char *temp = strtok(buffer, "@");
        char *found_in_peer = temp;

        if(*found_in_peer == '0') 
        { 
            cout << "FILE IS NOT FOUND IN PEER NODE NUMBER " << i + 1 ;
            cout<<endl;
            cout<<endl;
            close(socket_id);
            continue;
        }

        cout << "FILE IS FOUND IN PEER NODE NUMBER " << i + 1 ;
        cout<<endl;
        found = 1; 

        if(temp != NULL) 
        {
            temp = strtok(NULL, "$");
            filesize = atoi(temp);
        }

        long int remaining_bytes = filesize; 

        cout << "FILE SIZE = " << filesize ;
        cout<<endl;

        FILE *rcvd_file = fopen(filename, "w");
        int len;

        while(remaining_bytes > 0 && ((len = recv(socket_id, buffer, 1024, 0)) > 0)) 
        {
            fwrite(buffer, sizeof(char), len, rcvd_file);
            remaining_bytes -= len;
            cout << "BUFFER CURRENTLY CONTAINS: " << buffer ; 
            cout<<endl;
            cout << "RECEIVED = " << len << " BYTES, REMAINING BYTES = " << remaining_bytes << " BYTES "; 
            cout<<endl;
        }

        cout << "SUCESSFULL FILE TRANSFER "; 
        cout<<endl;
        cout<<endl;

        fclose(rcvd_file); 
        close(socket_id);  
    }

    if(found == 0) 
    { 
        cout << "FILE IS NOT FOUND IN ALL PEER_NODES";
        cout<<endl;
    }

    return 0;
}
