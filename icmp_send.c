#include <stdio.h> // printf()
#include <stdlib.h> // malloc()
#include <arpa/inet.h> // inet_pton (specific to Ubuntu) - also exposes <netinet/in.h>
#include <netinet/ip_icmp.h> // struct icmphdr, ICMP_ECHO
#include <unistd.h> // sleep()
#include <time.h> // time calls
#include <sys/time.h> // gettimeofday()
#include <math.h> // lrint rounding ... mhmm something is borked when compliling..
#include <string.h> // memset()

void print_time(); // declare function

int main() {
    
    int       sockfd; // raw_sock file descriptor
    char*     icmp_packet; // pointer to memory address for packet data
    int       icmp_packet_len; // integer defining payload size in bytes
    int       seq_num; // sequence number of sent ICMP echo request
   
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP); // create raw socket
    
    // struct used for sendto dest addr for connectionless messages
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.1.150", &dest_addr.sin_addr);

    // specify payload and packet size and reserve memory    
    icmp_packet_len     = 26;
    icmp_packet         = malloc(icmp_packet_len);

    // 0's for packet buff
    memset (icmp_packet, 0, icmp_packet_len);

    // specify ICMP header
    struct icmphdr echo_req;
    echo_req.type     = 8; // icmp echo request, type 8 code 0
    echo_req.code     = 0;
    // checksum not needed, IPROTO_ICMP will have kernel fill in checksum

    // set the icmp_packet payload with random data (for testing)
    memset(icmp_packet + sizeof(struct icmphdr), rand() % 255, icmp_packet_len);
  
    // print start time
    print_time();
 
    for (seq_num = 0; seq_num < 1; seq_num++) {
        sendto(sockfd, icmp_packet, icmp_packet_len, 0, (struct sockaddr*) &dest_addr, sizeof(dest_addr));
    }

    // print end time
    print_time();

    printf("numbr sent: %d\n", seq_num); 

}

void print_time() {

    // Time vars
    char      time_buff[26]; // time buffer
    int       millisec; // store millisec
    struct    tm* tm_info; 
    struct    timeval tv; 
 
    gettimeofday(&tv, NULL); // get time

    millisec = tv.tv_usec / 1000.0; // millisec handling
    if (millisec >= 1000) {
        millisec -= 1000;
        tv.tv_sec++; 
    }

    tm_info = localtime(&tv.tv_sec);
    strftime(time_buff, 26, "%Y:%m:%d %H:%M:%S", tm_info);

    printf("%s.%03d\n", time_buff, millisec); // print time

}
