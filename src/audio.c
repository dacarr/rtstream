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
#include "audio.h"


/* Global ALSA PCM */
snd_pcm_t *pcmc;
snd_pcm_t *pcmp;


int audio_format_to_alsa_format(int format) {

	int aformat;
	if(format == FORMAT_S16_LE) {
		aformat = SND_PCM_FORMAT_S16_LE;
	}
	else {
		aformat = SND_PCM_FORMAT_S16_LE;
	}

	return aformat;
}


/* Simple functions to handle soundcard.
 * now is only for ALSA */
void audio_alsa_capture_open(struct audio *au) {
	
	int err;
	err = snd_pcm_open(&pcmc, au->device, SND_PCM_STREAM_CAPTURE, 0);
	if (err < 0) {
		printf("ALSA: Playback open error: %s\n", snd_strerror(err));
		return;
	}
	
	int format;
	format = audio_format_to_alsa_format(au->format);
	
	if(!au->buffer){
		au->buffer = 500000;// buf 0.5sec 500000
	}

	err = snd_pcm_set_params(pcmc, format, SND_PCM_ACCESS_RW_INTERLEAVED,
	                         au->channels, au->sample_rate, 1, au->buffer);
	if (err < 0) {
		printf("ALSA: Playback open error: %s\n", snd_strerror(err));
		return;
	}
}

/* Function to read audio from soundcard */
int audio_alsa_capture_read(short *buffer, int size) {
	
	snd_pcm_sframes_t frames;
	frames = snd_pcm_readi(pcmc, buffer, size);

	if (frames == -EBADFD) {
		fprintf (stderr, "ALSA: PCM is not in the right state\n");
	}
	else if (frames == -EPIPE) {
		fprintf (stderr, "ALSA: an underrun occurred\n");
		frames = snd_pcm_prepare(pcmc);
	}
	else if (frames == -ESTRPIPE) {
		fprintf (stderr, "ALSA: a suspend event occurred (stream is suspended and waiting for an application recovery)\n");
	}

	return frames;
}

void audio_alsa_capture_close() {
	// wait to empty pcm buffer
	snd_pcm_drain (pcmc);
	snd_pcm_close (pcmc);
}

/* Simple functions to handle soundcard.
 * now is only for ALSA */
void audio_alsa_play_open(struct audio *au) {
	int err;
	err = snd_pcm_open(&pcmp, au->device, SND_PCM_STREAM_PLAYBACK, 0);
	if (err < 0) {
		printf("ALSA: Playback open error: %s\n", snd_strerror(err));
		return;
	}

	int format;
	format = audio_format_to_alsa_format(au->format);
	

	err = snd_pcm_set_params(pcmp, format, SND_PCM_ACCESS_RW_INTERLEAVED,
	                         au->channels, au->sample_rate, 1, au->buffer);
	if (err < 0) {
		printf("ALSA: Playback open error: %s\n", snd_strerror(err));
		return;
	}
}

/* Function to write audio on soundcard */
int audio_alsa_play_write(short *buffer, int size) {
	snd_pcm_sframes_t frames;
	frames = snd_pcm_writei(pcmp, buffer, size);

	if (frames == -EBADFD) {
		fprintf (stderr, "ALSA: PCM is not in the right state\n");
	}
	else if (frames == -EPIPE) {
		fprintf (stderr, "ALSA: an underrun occurred\n");
		frames = snd_pcm_prepare(pcmp);
	}
	else if (frames == -ESTRPIPE) {
		fprintf (stderr, "ALSA: a suspend event occurred (stream is suspended and waiting for an application recovery)\n");
	}

	return frames;
}


void audio_alsa_play_close() {
	// wait to empty pcm buffer
	snd_pcm_drain (pcmp);
	snd_pcm_close (pcmp);
}
