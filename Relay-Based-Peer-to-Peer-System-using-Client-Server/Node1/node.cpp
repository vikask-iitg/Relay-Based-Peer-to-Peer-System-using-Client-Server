#include<bits/stdc++.h>
#include <errno.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char *argv[]) {

    if(argc > 4 || argc < 4) 
    {
    	cout<<endl;
        cout << "FORMATE OF INPUT: " << argv[0] << " <SERVER IP PEER_NODE_ADDRESS> <PORT NUMBER OF THE SERVER> <PORT NUMBER OF THE PEER>";
        cout<<endl;
        return -1;
    }

    int socket_id = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_id < 0) 
    {
        cout<<endl;
        perror("SOME ERROR ENCOUNTERED IN SOCKET CREATION OF PEER NODE, TERMINATING! ");
        cout<<endl;
        return -1;
    }

    sockaddr_in server_address, peer_node_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[2]));

    char buffer[1024] = {0};

    if(inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0) 
    {
    	cout<<endl;
        perror(" INVALID SERVER IP PEER_NODE_ADDRESS, TERMINATING!!! ");
        cout<<endl;
        return -1;
    }

    int connection_check = connect(socket_id, (struct sockaddr *)&server_address, sizeof(server_address));
    if(connection_check < 0) 
    {
    	cout<<endl;
        perror("FALIUR ENCOUNTERED IN CONNECTION OF PEER NODE TO THE SERVER, TERMINATING!!! ");
        cout<<endl;
        return -1;
    }

    char message[] = "1#HI THERE! THIS IS THE PEER NODE.";
    sprintf(message, "%s#%d", message, atoi(argv[3]));
    int message_len = strlen(message);

    if(send(socket_id, message, message_len, 0) != message_len) 
    {
        perror("MESSAGE IS NOT SENT FROM PEER NODE TO THE SERVER");
        cout<<endl;
        return -1;
    }

    if(recv(socket_id, buffer, 1024, 0) < 0) 
    {
        perror("SERVER HAS SENT THE MESSAGE BUT IT IS NOT RECIVED BY THE PEER NODE");
        cout<<endl;
        return -1;
    }

    cout << "SERVER SAYS: " << buffer << endl;

    close(socket_id);

    peer_node_address.sin_family = AF_INET;
    peer_node_address.sin_addr.s_addr = htonl(INADDR_ANY);
    peer_node_address.sin_port = htons(atoi(argv[3]));

    if ((socket_id = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        cout<<endl;
        perror(" SOCKET CREATION ERROR FOR PEER NODE IN PHASE THREE ");
        cout<<endl;
        return -1;
    }

    int bind_check = bind(socket_id, (struct sockaddr *)&peer_node_address, sizeof(peer_node_address));
    if(bind_check < 0) 
    {
        perror("FALIUR IN BINDING THE PEER NODE IN PHASE THREE!");
        return -1;
    }

    listen(socket_id, 10);
    cout << "PORT NUMBER OF THE PEER NODE: " << ntohs(peer_node_address.sin_port) << endl;

    int peer_node_address_len = sizeof(peer_node_address);

    while(1)
    {

        char filename[1024] = {0};

        struct sockaddr_in client_address;
        int client_address_len = sizeof(client_address);
        int client_id;

        if((client_id = accept(socket_id, (struct sockaddr *)&client_address, (socklen_t *)&client_address_len)) < 0) 
        {
            perror("CLIENT CONNECTION REQUEST COULDN'T BE ACCEPTED");
            cout<<endl;
            return -1;
        }

        int recv_check = recv(client_id, filename, 1024, 0);
        if(recv_check < 0) 
        {
            perror("FALIUR IN RECIVING FILE NAME");
            cout<<endl;
            return -1;
        }

        cout << "THE CLIENT SERVER IS REQUESTING FOR THE FILE WITH THE NAME OF FILE = " << filename << endl;
        int file_descriptor = open(filename, O_RDONLY);

        char f_message[1024];

        if(file_descriptor == -1) 
        {
            sprintf(f_message, "0@");
            int f_message_len = strlen(f_message);
            cout << "THE REQUIRED FILE IS NOT FOUND";
            cout<<endl;

            if(send(client_id, f_message, f_message_len, 0) != f_message_len) 
            {
                perror("THE MESSAGE IS NOT SENT");
                cout<<endl;
                return -1;
            }

            continue;
        }

        struct stat file_stats;

        if (fstat(file_descriptor, &file_stats) < 0) 
        {
            fprintf(stderr, "ERROR IN OBTAINING FILE STATS: %s", strerror(errno));
            exit(EXIT_FAILURE);
        }

        fprintf(stdout, "SIZE OF FILE: %ld BYTES\n", file_stats.st_size);

        sprintf(f_message, "1@%ld", file_stats.st_size);
        int f_message_len = strlen(f_message);
        int p = send(client_id, f_message, f_message_len, MSG_NOSIGNAL);

        if(p != f_message_len) 
        {
            cout << "FILE STATS NOT SENT";
            cout<<endl;
            return -1;
        }

        off_t offset = 0;
        long int sent_bytes = 0, remain_data = file_stats.st_size;

        while (((sent_bytes = sendfile(client_id, file_descriptor, &offset, BUFSIZ)) > 0) && (remain_data > 0)) 
        {
            remain_data -= sent_bytes;
            fprintf(stdout, "SERVER SENT %ld BYTES FROM FILE'S DATA, OFFSET = %ld AND REMAINING DATA = %ld\n",
                    sent_bytes, offset, remain_data);
        }

        cout << "TRANSFER OF FILE IS COMPLETED!!!";
        cout<<endl;
        cout<<endl;
    }

    close(socket_id);

    return 0;
}
