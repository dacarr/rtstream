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
#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#include <alsa/asoundlib.h>

enum audio_formats {
	FORMAT_S16_LE
};


struct audio {
	int   format;		// ex. FORMAT_FLOAT_LE
    char *device;		// name of device
    int   sample_rate;  // ex. 48000
    int   channels;		// 1=mono 2=stereo
	int   buffer;		// buffer 0.5sec 500000
};


void audio_alsa_capture_open(struct audio *au);
int  audio_alsa_capture_read(short *buffer, int size);
void audio_alsa_capture_close();

void audio_alsa_play_open(struct audio *au);
int  audio_alsa_play_write(short *buffer, int size);
void audio_alsa_play_close();

#endif // AUDIO_H_INCLUDED
