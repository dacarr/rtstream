rtstream
========

Realtime media streamer with Opus codec on linux:

Send live audio from alsa on RTP in IPv4 or IPv6 to one or multiple receivers.

Useful for link on radio stations (now is tested as permanent link in a radio station).

Low latency.


Use:

for IPv4:

rtstream --transmit --host 192.168.1.10

rtstream --receive


for IPv6 (very useful, no nat):

rtstream --transmit --ipv6 --host 1a:2b:3c::1

rtstream --receive  --ipv6

Options:

--device plug:myalsadevice

--port 5005

--bitrate 96

for more options type rtstream


Libs needed:

libopus-dev

libasound-dev

more info for opus: http://opus-codec.org/

(next step is video and RTSP server)
