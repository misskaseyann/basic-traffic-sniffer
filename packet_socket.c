#include <sys/socket.h>
#include <netpacket/packet.h> // uses a different type of socket
#include <net/ethernet.h> // ethernet packets
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <net/if.h> // specific network interfaces
#include <linux/ip.h>
#include <linux/kernel.h>

int main(int argc, char** arv) {
        // create a socket. can see all the headers and manipulate all the headers.
        // need root access in order to manipulate packet headers.
        int packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
        if (packet_socket < 0) {
                perror("socket");
                return 1;
        }
        // sockaddr_ll is in man packet, device independent physical layer address
        // USEFUL FOR NEXT PROJECT ***HINT HINT***
        struct sockaddr_ll serveraddr, clientaddr;
        // structure contains more than just these three...
        serveraddr.sll_family = AF_PACKET; // socket matches only this type value (AF_PACKET = any), such as if IPv4
, then it will only get IPv4 packets
        servaeraddr.sll_protocol = htons(ETH_P_ALL); // identifier for the interface
        serveraddr.sll_ifindex = if_nametoindex("h2-eth0"); // h2-eth0 runs on h2 of mininet, hardcoded
        // ora use en0 on mac
        int e = bind(packet_socket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
        if (e a< 0) {
                perror("bind error");
                return 2;
        }

        while(1) {
                char buf[1514];
                int len = sizeof(clientaddr);
                int n = recvfrom(packet_socket, buf, 1514, 0, (struct sockaddr*)&clientaddr, &len);
                
                if (clientaddr.sll_pkttype == PACKET_OUTGOING) {
                        continue;
                }

                printf("\nGot a %d byte packet, first byte is %hhx\n", n, buf[0]);

                struct ether_header *header = (struct ether_header*)buf;

                // destination
                printf("Destination: ");
                for (int i = 0; i < 5; i++) {
                        printf("%x:", header->ether_dhost[i]);
                }
                printf("%x\n", header->ether_dhost[5]);

                // source
                printf("Source: ");
                for (int i = 0; i < 5; i++) {
                        printf("%x:", header->ether_shost[i]);
                }
                printf("%x\n", header->ether_shost[5]);

                // type
                printf("Type: %x", header->ether_type);
                if (ntohs(header->ether_type) == ETHERTYPE_IP) {
                        printf("\nIPv4\n");
                        struct iphdr *ip = (struct iphdr*) (buf + sizeof(struct ether_header));
                        // make into array
                        char store[4];
                        memcpy(store, &(ip->saddr), 4);
                        printf("IP Source: ");
                        for (int i = 0; i < 4; i++) {
                                printf("%d.", store[i]);
                        }
                        memcpy(store, &(ip->daddr), 4);
                        printf("\nIP Destination: ");
                        for (int i = 0; i < 4; i++) {
                                printf("%d.", store[i]);
                        }
                }
        }
        // do ping 10.0.0.2 to send the packet in h1 to h2

}
