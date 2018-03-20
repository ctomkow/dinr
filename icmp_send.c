#include <stdio.h> // printf
#include <arpa/inet.h> // inet_pton (specific to Ubuntu) - also exposes <netinet/in.h>

int main() {
    
    int       sockfd; // raw_sock file descriptor
    char*     payload; // pointer to memory address for packet data
    int       payload_len; // integer defining payload size in bytes
    int       seq_num; // sequence number of sent ICMP echo request
    
    seq_num = 0;
    
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP); //create raw socket
    
    // struct used for sendto dest addr for connectionless messages
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.1.150", &dest_addr.sin_addr);
    
    payload_len = 64;
    payload     = malloc(payload_len);
    
    while(1) {
        sendto(sockfd, payload_len, 0, (struct sockaddr*) &dest_addr, sizeof(dest_addr));
        print("sent ICMP msg: &d\n", seq_num);
        
        seq_num++ // increase seq_num by one
    }
}
