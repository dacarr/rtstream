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
#include <stdio.h>
#include <string.h>

#include <getopt.h>

#include "audio.h"
#include "codec.h"
#include "rtp.h"
#include "socket.h"


void help() {
    printf("Usage: stream --receive/transmit [--option value]\n");
    printf("\n");
    printf("Send and receive RTP, codec in OPUS readed/writed on ALSA\n");
    printf("\nGeneral options:\n");
    printf(" -h, --help         This help\n");
    printf(" --receive          Receive mode\n");
    printf(" --transmit         Transmit mode\n");
    printf("\nNetwork options:\n");
    printf(" --host             Network host to transmit\n");
    printf(" --port             Network port to send or receive (default: 5004)\n");
    printf(" --ipv6             Set the IPv6 protocol (default: IPv4)\n");
    printf("\n");
    printf("\nEncoding options: (in transmit mode)\n");
    printf(" --bitrate          Encoding bitrate in kilobit/s (5-512 default 64kbps)\n");
    //printf(" --vbr              Use variable bitrate encoding (default)\n");
    //printf(" --cvbr             Use constrained variable bitrate encoding\n");
    //printf(" --hard-cbr         Use hard constant bitrate encoding\n");
    printf(" --comp n           Encoding complexity (0-10, default: 10)\n");
    printf(" --framesize n      Maximum frame size in milliseconds\n");
    printf("                      (2.5, 5, 10, 20, 40, 60, default: 20)\n");
    //printf(" --expect-loss      Percentage packet loss to expect (default: 0)\n");
    printf("\nSoundcard options:\n");
    printf(" --device n         Play or capture device (hw:0,x, plug:yourxdevice, \n");
    printf("                      plughw:1,x, default: default device)\n");
    printf(" --samplerate n     Sampling rate (Hz), (8000, 12000, 16000, 24000,48000 \n");
    printf("                      default: 48000)\n");
    printf(" --channels n       Audio channels, (1, 2, default: 2)\n");
    printf(" --buffer n         Soundcard buffer in nanoseconds (default: 500000, \n");
    printf("                      in no realtime kernels lower buffers causes xruns)\n");
}


int main(int argc, char** argv){

    int option_index = 0;
    struct option long_options[] = {
        {"transmit", 0, NULL, 0},
        {"receive", 0, NULL, 0},
        {"test", 0, NULL, 0},
        {"host", 1, NULL, 0},
        {"port", 1, NULL, 0},
        {"ipv6", 0, NULL, 0},
        {"device", 1, NULL, 0},
		{"buffer", 1, NULL, 0},
        {"bitrate", 1, NULL, 0},
        /*{"hard-cbr", 0,NULL, 0},
        {"vbr", 0,NULL, 0},
        {"cvbr", 0,NULL, 0},
        {"comp", 0, NULL, 0},*/
        {"complexity", 0, NULL, 0},
        {"framesize", 0, NULL, 0},
        //{"expect-loss", 0, NULL, 0},
        {"help", 0, NULL, 0},
        {0, 0, 0, 0}
    };


    /*Settings*/
    int   mode = 5;
    char *host = NULL;
    int   port = 5004;
    int   ipv = 4;
    char *device = "default";
	int   buffer = 500000;
    int   bitrate = 64000;
    int   sample_rate = 48000;
    int   frame_size = 960; //at 48 kHz the permitted values are 120, 240, 480, 960, 1920, and 2880.
    int   channels = 2;
//    int                with_hard_cbr = 0;
//    int                with_cvbr = 0;
    int           expect_loss = 0;
    int           complexity = 10;

    /*Process command-line options*/
    while(1){
        int c;
        c = getopt_long(argc, argv, "hV",
                      long_options, &option_index);
        if(c==-1)
           break;

        switch(c){
        case 0:
            if(strcmp(long_options[option_index].name,"transmit") == 0){
                mode = 1;
            }
            else if(strcmp(long_options[option_index].name,"receive") == 0){
                mode = 0;
            }
            else if(strcmp(long_options[option_index].name,"test") == 0){
                mode = 99;
            }
            else if(strcmp(long_options[option_index].name,"host") == 0){
                host = optarg;
            }
            else if(strcmp(long_options[option_index].name,"port") == 0){
                port = atoi(optarg);
            }
            else if(strcmp(long_options[option_index].name,"ipv6") == 0){
                ipv = 6;
            }
            else if(strcmp(long_options[option_index].name,"device") == 0){
                device = optarg;
            }
            else if(strcmp(long_options[option_index].name,"buffer") == 0){
                buffer = atoi(optarg);
            }
            else if(strcmp(long_options[option_index].name,"bitrate") == 0){
                bitrate = atoi(optarg)*1000;
            }
            else if(strcmp(long_options[option_index].name,"complexity") == 0){
                complexity = atoi(optarg);
            }
            else if(strcmp(long_options[option_index].name,"framesize") == 0){
                frame_size = atoi(optarg);
            }
//            else if(strcmp(long_options[option_index].name,"hard-cbr")==0){
//              with_hard_cbr=1;
//              with_cvbr=0;
//            }
//            else if(strcmp(long_options[option_index].name,"cvbr")==0){
//              with_cvbr=1;
//              with_hard_cbr=0;
//            }
//            else if(strcmp(long_options[option_index].name,"vbr")==0){
//              with_cvbr=0;
//              with_hard_cbr=0;
//            }
            else if(strcmp(long_options[option_index].name,"help")==0){
              help();
              exit(0);
            }
            else if(strcmp(long_options[option_index].name,"expect-loss")==0){
              expect_loss=atoi(optarg);
              if(expect_loss>100||expect_loss<0){
                fprintf(stderr,"Invalid expect-loss: %s\n",optarg);
                fprintf(stderr,"Expected loss is a percent and must be 0-100.\n");
                exit(1);
              }
            }
            else if(strcmp(long_options[option_index].name,"comp")==0 ||
                     strcmp(long_options[option_index].name,"complexity")==0){
              complexity=atoi(optarg);
              if(complexity > 10 || complexity < 0){
                fprintf(stderr,"Invalid complexity: %s\n",optarg);
                fprintf(stderr,"Complexity must be 0-10.\n");
                exit(1);
              }
            }
            else if(strcmp(long_options[option_index].name,"framesize")==0){
                if(strcmp(optarg,"2.5")==0)frame_size=120;
                else if(strcmp(optarg,"5")==0)frame_size=240;
                else if(strcmp(optarg,"10")==0)frame_size=480;
                else if(strcmp(optarg,"20")==0)frame_size=960;
                else if(strcmp(optarg,"40")==0)frame_size=1920;
                else if(strcmp(optarg,"60")==0)frame_size=2880;
                else{
                    fprintf(stderr,"Invalid framesize: %s\n",optarg);
                    fprintf(stderr,"Framesize must be 2.5, 5, 10, 20, 40, or 60.\n");
                    exit(1);
                }
            }
            break;
        case 'h':
            help();
            exit(0);
            break;
        case '?':
            help();
            exit(1);
            break;
        }
    }

    if (argc == 1) {
        help();
        exit(1);
    }


    if (mode == 0) {
		//RECEIVE MODE
        char*  data;
        short* pcm;
        int    len;
        int    mtu = 1400; //tamaño maximo del paquete recibido

		printf("Receive in port: %d\n", port);

        //alloc memory
        data = (char*) malloc(mtu);
        pcm = (short*) malloc(sizeof(short)*channels*frame_size);

		struct audio au;
		au.device = device;
		au.sample_rate = sample_rate;
		au.channels = channels;
		au.buffer = buffer;
		audio_alsa_play_open(&au);

		struct codec co;
		co.sample_rate = au.sample_rate;
		co.channels =  au.channels;
		co.bitrate = bitrate;
		co.complexity = complexity;
		co.frame_size = frame_size;
		codec_opus_decoder_open(&co);

		
		if(ipv == 4) {
			struct socket so;
			so.mode = SOCK_UDP_RECEIVE;
			so.port = port;
		    socket4_init(&so);

		    while(1) {
				len = socket4_recv(data, mtu);
		        len = rtp_depayload(data, len);
		        codec_opus_decoder_decode(data, len, pcm, co.frame_size);
		        audio_alsa_play_write(pcm, frame_size);
		    }
		}
		
		else { 
			// IPV6
			struct socket so;
			so.mode = SOCK_UDP_RECEIVE;
			so.port = port;
		    socket_init(&so);

		    while(1) {
				len = socket_recv(data, mtu);
		        len = rtp_depayload(data, len);
		        codec_opus_decoder_decode(data, len, pcm, co.frame_size);
		        audio_alsa_play_write(pcm, frame_size);
		    }
		}
		
		codec_opus_encoder_close();
        audio_alsa_capture_close();
    }

    else if (mode == 1) {
		//TRANSMIT MODE
        char *packet;
        short *pcm;
        int plen;
        int max_data_bytes = 1400;

        printf("Sending to: %s port: %d\n", host, port);
		
        pcm = (short*) malloc(frame_size * channels * sizeof(short));
        packet = (char*) malloc(sizeof(char) * max_data_bytes);
		
		struct audio au;
		au.device = device;
		au.sample_rate = sample_rate;
		au.channels = channels;
		au.buffer = buffer;
		audio_alsa_capture_open(&au);
		
		struct codec co;
		co.sample_rate = sample_rate;
		co.channels =  channels;
		co.bitrate = bitrate;
		co.complexity = complexity;
		co.frame_size = frame_size;
        codec_opus_encoder_open(&co);

		struct rtp rt;
		rt.clock_rate = sample_rate;
        rtp_payload_init(&rt);
		

		if(ipv == 4) {
			struct socket so;
    		so.mode = SOCK_UDP_TRANSMIT;
    		socket4_init(&so);
			
			socket4_sendto_list_add(host, port);
			
		    while(1) {
		        audio_alsa_capture_read(pcm, frame_size);
		        plen = codec_opus_encoder_encode(pcm, frame_size, packet, max_data_bytes);
		        plen = rtp_payload(packet, plen);
		        socket4_sendto_list(packet, plen);
		    }
		}

		else {
			// IPV6
			struct socket so;
    		so.mode = SOCK_UDP_TRANSMIT;
    		socket_init(&so);
			
			socket_sendto_list_add(host, port);
			
		    while(1) {
		        audio_alsa_capture_read(pcm, frame_size);
		        plen = codec_opus_encoder_encode(pcm, frame_size, packet, max_data_bytes);
		        plen = rtp_payload(packet, plen);
		        socket_sendto_list(packet, plen);
		    }
		}
		
        codec_opus_encoder_close();
        audio_alsa_capture_close();
    }

	return 0;
}
