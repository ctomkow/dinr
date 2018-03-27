/*

Craig Tomkow
March 25, 2018
COMP689 - Advanced Distributed Systems
A Distributed ICMP Network Responder (DINR) service

icmp_send.c

-This program sends ICMP echo requests (type 8, code 0) as fast as possible to a target addr.
 It is used to test the limits of a linux kernel responding to small (64 byte) echo requests.

*/


#include <stdio.h> // printf()
#include <stdlib.h> // malloc()
#include <arpa/inet.h> // inet_pton (specific to Ubuntu) - also exposes <netinet/in.h>
#include <netinet/ip_icmp.h> // icmphdr struct
#include <unistd.h> // sleep()
#include <time.h> // localtime(), strftime()
#include <sys/time.h> // gettimeofday(), timeval struct
#include <string.h> // memset()

// declare functions
void print_time();
unsigned short in_cksum(unsigned short *, int);

int main() {
    
    int       sockfd; // raw_sock file descriptor
    char     *icmp_packet; // pointer to memory address for packet data
    int       icmp_packet_len; // integer defining icmp packet size in bytes
    long int      seq_num; // sequence number of sent ICMP echo request
    struct    icmphdr *echo_req; // pointer to icmphdr in memory
    struct    sockaddr_in dest_addr; // struct used for sendto des addr for connectionless messages
   
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP); // create raw socket
    
    // fill in sockaddr_in struct with destination address details
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.1.150", &dest_addr.sin_addr);

    // specify payload and packet size in bytes, reserve memory    
    icmp_packet_len     = 26; // ICMP_HDR + PAYLOAD (8 + 18)
    icmp_packet         = malloc(icmp_packet_len);

    // 0's for packet buff (checksum is calculated only for icmp header, so packet is all 0's)
    memset (icmp_packet, 0, icmp_packet_len);

    // reserve and specify ICMP header details
    echo_req = malloc(sizeof(struct icmphdr)); // reserve memory for icmphdr
    echo_req = (struct icmphdr *) (icmp_packet) ; // icmp_packet cast as icmphdr
    echo_req->type     = 8; // icmp echo request, type 8 code 0
    echo_req->code     = 0;
    echo_req->checksum = 0;

    echo_req->checksum = in_cksum((unsigned short *)echo_req, sizeof(struct icmphdr)); // compute checksum
  
    // print start time
    print_time();
 
    // send ICMP echo requests 
    for (seq_num = 0; seq_num < 100000000; seq_num++) {
        sendto(sockfd, icmp_packet, icmp_packet_len, 0, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
    }

    // print end time
    print_time();

    printf("numbr sent: %ld\n", seq_num); 

}

void print_time() {

    // Time vars
    char      time_buff[26]; // time buffer
    int       ms; // store milliseconds
    struct    tm *time_info; // a date and time struct (only sec precision)
    struct    timeval time; // sec and microsec time struct
 
    gettimeofday(&time, NULL); // get time

    ms = time.tv_usec / 1000.0; // convert from microseconds to milliseconds
    if (ms >= 1000) {
        ms -= 1000;
        time.tv_sec++;  // if millisecond is >= 1000, incr second
    }

    time_info = localtime(&time.tv_sec); // assigns time to time_info struct
    strftime(time_buff, 26, "%Y:%m:%d %H:%M:%S", time_info); // formats time

    printf("%s.%03d\n", time_buff, ms); // prints time

}

// checksum func'n src: cs.cmu.edu/afs/cs/academic/class/15213-f00/unpv12e/libfree/in_cksum.c
unsigned short in_cksum(unsigned short *addr, int len) {

    int             nleft     = len;
    int             sum       = 0;
    unsigned short *w         = addr;
    unsigned short  answer    = 0;

    while (nleft > 1) {
        sum   += *w++;
        nleft -= 2;
    }

    if (nleft == 1) {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }

    sum    = (sum >> 16) + (sum & 0xffff);
    sum   += (sum >> 16);
    answer = ~sum;
    return(answer);

}
