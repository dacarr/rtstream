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
#include "codec.h"

int encoded_bytes;


/*
 * OPUS audio codec
 */
OpusEncoder *enc;
OpusDecoder *dec;

/* encoder */
void codec_opus_encoder_open(struct codec *co) {

	int err;
	enc = opus_encoder_create(co->sample_rate, co->channels, OPUS_APPLICATION_RESTRICTED_LOWDELAY, &err);
	if (err != OPUS_OK) {
		fprintf(stderr, "Cannot create encoder: %s\n", opus_strerror(err));
		return;
	}

    opus_encoder_ctl(enc, OPUS_SET_BANDWIDTH(OPUS_BANDWIDTH_FULLBAND));
	opus_encoder_ctl(enc, OPUS_SET_BITRATE(co->bitrate));
	opus_encoder_ctl(enc, OPUS_SET_COMPLEXITY(co->complexity));
	opus_encoder_ctl(enc, OPUS_SET_SIGNAL(OPUS_AUTO));
	opus_encoder_ctl(enc, OPUS_SET_VBR(0));
}

int codec_opus_encoder_encode(opus_int16 *audio_frame, int frame_size, char* data, int max_data_bytes){

	encoded_bytes = opus_encode(enc, audio_frame, frame_size, (unsigned char*) data, max_data_bytes);
	if(encoded_bytes < 0){
        printf("Error opus encode %d\n", encoded_bytes);
	}
	return encoded_bytes;
}

void codec_opus_encoder_close() {
	opus_encoder_destroy(enc);
}


/* decoder */
void codec_opus_decoder_open(struct codec *co){

    int error;
    dec = opus_decoder_create(co->sample_rate, co->channels, &error);

    if (error<0)
        printf("error %d\n", error);
}

void codec_opus_decoder_decode(char *data, opus_int32 len, opus_int16 *pcm, int frame_size){

    //fec forward error correction
    int	decode_fec = 0;

    if(opus_decode(dec, (unsigned char *) data, len, pcm, frame_size, decode_fec) < 0){
        printf("OPUS Decoding error!\n");
	}

}

void codec_opus_decoder_close() {
	opus_decoder_destroy(dec);
}
