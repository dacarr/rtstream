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
#ifndef CODEC_H_INCLUDED
#define CODEC_H_INCLUDED

#include <opus/opus.h>

#include <stdio.h>


struct codec {
    int sample_rate;
    int channels;
	int quality;
    int bitrate;
    int complexity;
    int frame_size;
};

void codec_opus_encoder_open(struct codec *co);
int  codec_opus_encoder_encode(opus_int16 *audio_frame, int frame_size, char* data, int max_data_bytes);
void codec_opus_encoder_close();

void codec_opus_decoder_open(struct codec *co);
void codec_opus_decoder_decode(char *data, opus_int32 len, opus_int16 *pcm, int frame_size);
void codec_opus_decoder_close();


#endif // CODEC_H_INCLUDED
