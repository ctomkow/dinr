#include <stdio.h> // printf()
#include <stdlib.h> // malloc()
#include <arpa/inet.h> // inet_pton (specific to Ubuntu) - also exposes <netinet/in.h>
#include <netinet/ip_icmp.h> // struct icmphdr
#include <unistd.h> // sleep()

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
    inet_pton(AF_INET, "8.8.8.8", &dest_addr.sin_addr);

    // specify payload size and reserve memory    
    payload_len     = 64;
    payload         = malloc(payload_len);

    // specify ICMP header
    struct icmphdr echo_req;
    echo_req.type   = 8; // icmp echo request
    echo_req.code   = 0;
    
    while(1) {
        sendto(sockfd, payload, payload_len, 0, (struct sockaddr*) &dest_addr, sizeof(dest_addr));
        printf("sent ICMP msg: %d\n", seq_num);
        
        seq_num++; // increase seq_num by one
        sleep(1); // for testing (sec)
    }
}
