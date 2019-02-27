/*
Modified by FCaminade

Sources : 
https://stackoverflow.com/questions/10824827/raw-sockets-communication-over-wifi-receiver-not-able-to-receive-packets

1/Find your wifi interface:
$ iwconfig

2/Setup the board in monitor mode :
$ sudo ifconfig wlp5s0 down
$ sudo iwconfig wlp5s0 mode monitor
$ sudo ifconfig wlp5s0 up

3/Launch the test with the good rights! :
$ ./test3 wlp5s0 or sudo ./test3 wlp5s0

*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_arp.h>
#include <arpa/inet.h>

#define NIC_NAME "mon0"

/*our MAC address*/
static uint8_t gu8a_src_mac[6] = {0xF8, 0x1A, 0x67, 0xB7, 0xeB, 0x0B};

/*other host MAC address*/
//static uint8_t gu8a_dest_mac[6] = {0x84,0xF3,0xEB,0x73,0x55,0x0D};
static uint8_t gu8a_dest_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
//static uint8_t gu8a_dest_mac[6] = {0x00,0x0C,0xE6,0x02,0x4F,0x76};//laptop

int32_t get_nic_index(uint8_t *pu8_nic_card_name);

int main(void)
{
    struct sockaddr_ll s_dest_addr;
    int32_t s32_sock = -1;
    int32_t s32_res = -1;

    printf("Socket raw test\n");

    (void)memset(&s_dest_addr, 0, sizeof(s_dest_addr));

    s32_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (-1 == s32_sock)
    {
        perror("Could not create the socket");
        goto LABEL_CLEAN_EXIT;
    }

    printf("Socket created\n");

    fflush(stdout);

    sleep(1);

    s_dest_addr.sll_family = AF_PACKET;
    /*we don't use a protocol above ethernet layer, just use anything here*/
    s_dest_addr.sll_protocol = htons(ETH_P_ALL);
    s_dest_addr.sll_ifindex = get_nic_index((uint8_t *)NIC_NAME);
    s_dest_addr.sll_hatype = ARPHRD_ETHER;
    s_dest_addr.sll_pkttype = PACKET_OTHERHOST; //PACKET_OUTGOING
    s_dest_addr.sll_halen = ETH_ALEN;
    /*MAC - begin*/
    s_dest_addr.sll_addr[0] = gu8a_dest_mac[0];
    s_dest_addr.sll_addr[1] = gu8a_dest_mac[1];
    s_dest_addr.sll_addr[2] = gu8a_dest_mac[2];
    s_dest_addr.sll_addr[3] = gu8a_dest_mac[3];
    s_dest_addr.sll_addr[4] = gu8a_dest_mac[4];
    s_dest_addr.sll_addr[5] = gu8a_dest_mac[5];
    /*MAC - end*/
    s_dest_addr.sll_addr[6] = 0x00; /*not used*/
    s_dest_addr.sll_addr[7] = 0x00; /*not used*/

    printf("******Sending data using raw socket over '" NIC_NAME "'\n");

    while (1)
    {
        uint8_t data[331] = {
            0x00, 0x00, 0x26, 0x00, 0x2f, 0x40, 0x00, 0xa0, 0x20, 0x08, 0x00, 0xa0, 0x20, 0x08, 0x00, 0x00,
            0xdf, 0x32, 0xfe, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0c, 0x6c, 0x09, 0xc0, 0x00, 0xd3, 0x00,
            0x00, 0x00, 0xd3, 0x00, 0xc7, 0x01, 0xd0, 0x00, 0x3c, 0x00, 0xf8, 0x1a, 0x67, 0xb7, 0xeb, 0x0b,
            0x84, 0xf3, 0xeb, 0x73, 0x55, 0x0d, 0xf8, 0x1a, 0x67, 0xb7, 0xeb, 0x0b, 0x70, 0x71, 0x7f, 0x18,
            0xfe, 0x34, 0xb8, 0x39, 0xa8, 0xce, 0xdd, 0xff, 0x18, 0xfe, 0x34, 0x04, 0x01, 0x29, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12,
            0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0xd8, 0x87, 0xc3, 0xa5};

        s32_res = sendto(s32_sock,
                         data,
                         331,
                         0,
                         (struct sockaddr *)&s_dest_addr,
                         sizeof(s_dest_addr));

        if (-1 == s32_res)
        {
            perror("Socket send failed");
            goto LABEL_CLEAN_EXIT;
        }

        for (int i = 0; i < 331; i++)
            printf("%x ", data[i]);

        printf("\n\n");

        usleep(100);
    }

LABEL_CLEAN_EXIT:
    if (s32_sock > 0)
    {
        close(s32_sock);
    }

    printf("***** Raw Socket test- end\n");

    return EXIT_SUCCESS;
}

int32_t
get_nic_index(uint8_t *pu8_nic_card_name)
{
    int32_t s32_sock_fd = -1;
    int32_t s32_res = -1;
    struct ifreq s_ifr;

    (void)memset(&s_ifr, 0, sizeof(s_ifr));

    s32_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (-1 == s32_sock_fd)
    {
        perror("get_nic_index(): socket failed");
        goto LABEL_CLEAN_EXIT;
    }

    s_ifr.ifr_addr.sa_family = AF_INET;

    strncpy(s_ifr.ifr_name, (char *)pu8_nic_card_name, IFNAMSIZ);

    s32_res = ioctl(s32_sock_fd, SIOCGIFINDEX, &s_ifr);

    if (-1 == s32_res)
    {
        perror("get_nic_index(): ioctl failed");
    }

    close(s32_sock_fd);

LABEL_CLEAN_EXIT:
    return (s_ifr.ifr_ifru.ifru_ivalue);
}