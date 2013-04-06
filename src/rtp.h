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
#ifndef RTP_H
#define RTP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

struct rtp {
	int payload_type;
	int clock_rate;
};

void rtp_payload_init(struct rtp *rt);
int  rtp_payload(char *data, int len);
int  rtp_depayload(char *data, int len);


#endif /* RTP_H */
