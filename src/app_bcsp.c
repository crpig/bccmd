#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>

#include "csr.h"
#include "ubcsp.h"

static uint16_t seqnum = 0x0000;

static struct ubcsp_packet send_packet;
static uint8_t send_buffer[512];

static struct ubcsp_packet receive_packet;
static uint8_t receive_buffer[512];


static int write_command(uint16_t command, uint16_t seqnum, uint16_t varid, uint8_t *value, uint16_t length)
{
	unsigned char cp[254], rp[254];
	uint16_t size;
	uint8_t delay, activity = 0x00;
	int timeout = 0, sent = 0;

	size = (length < 8) ? 9 : ((length + 1) / 2) + 5;

    memset(cp, 0, sizeof(cp));
	cp[0] = command & 0xff;
	cp[1] = command >> 8;
	cp[2] = size & 0xff;
	cp[3] = size >> 8;
	cp[4] = seqnum & 0xff;
	cp[5] = seqnum >> 8;
	cp[6] = varid & 0xff;
	cp[7] = varid >> 8;
	cp[8] = 0x00;
	cp[9] = 0x00;

	memcpy(cp + 10, value, length);

	receive_packet.length = 512;
    receive_packet.payload = receive_buffer;
	ubcsp_receive_packet(&receive_packet);

	send_packet.channel  = 2;
	send_packet.reliable = 1;
	send_packet.length   = size * 2;
    send_packet.payload  = send_buffer;
	memcpy(send_packet.payload, cp, (size * 2));

	ubcsp_send_packet(&send_packet);

	while (1) {
		delay = ubcsp_poll(&activity);

		if (activity & UBCSP_PACKET_SENT)
        {
			switch (varid) {
			case CSR_VARID_COLD_RESET:
			case CSR_VARID_WARM_RESET:
			case CSR_VARID_COLD_HALT:
			case CSR_VARID_WARM_HALT:
				return 0;
			}

			sent = 1;
			timeout = 0;
		}

		if (activity & UBCSP_PACKET_RECEIVED)
        {
			if (sent && receive_packet.channel == 2)
            {
				memcpy(rp, receive_packet.payload, receive_packet.length);
				break;
			}

			receive_packet.length = 512;
			ubcsp_receive_packet(&receive_packet);
			timeout = 0;
		}

		if (delay)
        {
			usleep(delay * 100);

			if (timeout++ > 5000)
            {
				fprintf(stderr, "Operation timed out\n");
				errno = ETIMEDOUT;
				return -1;
			}
		}
	}

    /* Status */
	if ((rp[8] + (rp[9] << 8)) != 0)
    {
		errno = ENXIO;
		return -1;
	}

	memcpy(value, rp + 10, length);

	return 0;
}

static int read_hq(uint16_t *varid, uint8_t *value, uint16_t *length)
{
	unsigned char cp[254], rp[254];
	uint8_t delay, activity = 0x00;
	int timeout = 0, received = 0;

	receive_packet.length = 512;
    receive_packet.payload = receive_buffer;
	ubcsp_receive_packet(&receive_packet);

	while (1)
    {
		delay = ubcsp_poll(&activity);

		if (activity & UBCSP_PACKET_SENT)
        {
            /* GETRESP Has been sent */
            break;
		}

		if (activity & UBCSP_PACKET_RECEIVED)
        {
            if (received)
            {
                continue;
            }

			if (receive_packet.channel == 3)
            {
                *length = receive_packet.length - 10;
				memcpy(rp, receive_packet.payload, receive_packet.length);
                received = 1;

                /* ready for send GETRESP */
                memset(cp, 0, sizeof(cp));
                cp[0] = 0x01;
                cp[1] = 0x00;
                memcpy(cp+2, rp+2, receive_packet.length-2);
                send_packet.channel  = 3;
            	send_packet.reliable = 1;
            	send_packet.length   = receive_packet.length;
                send_packet.payload  = send_buffer;
            	memcpy(send_packet.payload, cp, receive_packet.length);

            	ubcsp_send_packet(&send_packet);

                receive_packet.length = 512;
			    ubcsp_receive_packet(&receive_packet);
			    timeout = 0;
			}
            else
            {
                receive_packet.length = 512;
			    ubcsp_receive_packet(&receive_packet);
			    timeout = 0;
            }
		}

		if (delay)
        {
			usleep(delay * 100);

			if (timeout++ > 5000)
            {
				fprintf(stderr, "Operation timed out\n");
				errno = ETIMEDOUT;
				return -1;
			}
		}
	}

    /* Status */
	if ((rp[8] + (rp[9] << 8)) != 0) {
		errno = ENXIO;
		return -1;
	}

    *varid = rp[6] + (uint16)(rp[7] << 8);

	memcpy(value, rp + 10, *length);

	return 0;
}

int csr_read_hq(uint16_t *varid, uint8_t *value, uint16_t *length)
{
	return read_hq(varid, value, length);
}

int csr_write_bccmd(uint16_t varid, uint8_t *value, uint16_t length)
{
	return write_command(0x0002, seqnum++, varid, value, length);
}

