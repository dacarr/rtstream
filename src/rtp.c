/*
 * Copyright (C) 2013  Daniel Carrera <dcarrera@miten.es>
 *
 * This file is part of rtstream.
 * 
 * rtstream is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rtstream is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rtstream.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "rtp.h"

uint16_t rtp_seqnum;
uint32_t t;//timestamp
uint32_t SSRC;

int payload_type;
int clock_rate;



void rtp_payload_init(struct rtp *rt) {

	rtp_seqnum = rand();
    t = rand();
    SSRC = rand();

	payload_type = 0x60;
	clock_rate = rt->clock_rate;

}


int rtp_payload(char *data, int len) {

    memcpy(data+12, data, len);

	t += clock_rate;

	// version | padding | extension |  CSRC  |
	//   01    |    0    |     0     |  0000  |
	// marker  |         payload type         |
	//   0     |           1100010            |
	data[0] = 0x80 & 0xFF;
	//payload type
	data[1] = payload_type & 0xFF;
	//data[1] = 0x00 & 0xFF; //test remove
	//sequence number 16bit
	data[2] = (rtp_seqnum >> 8) & 0xFF;
	data[3] = rtp_seqnum & 0xFF;
	//timestamp 32bit
	data[4] = (t >> 24) & 0xFF;
	data[5] = (t >> 16) & 0xFF;
	data[6] = (t >> 8) & 0xFF;
	data[7] = t & 0xFF;
	//SSRC
	data[8] = (SSRC >> 24) & 0xFF;
	data[9] = (SSRC >> 16) & 0xFF;
	data[10] = (SSRC >> 8) & 0xFF;
	data[11] = SSRC & 0xFF;


	rtp_seqnum++;

	return (len+12);
}


int rtp_depayload(char *data, int len){

    //rtp
	//if sequence is not +1 then later loop data == null

	//Payload.
    //remove the rtp data.
    memcpy(data, data+12, (len-12));
	
    //return new len of data
    return (len-12);
}
