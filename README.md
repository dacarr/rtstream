RTStream
========

Realtime media streamer with Opus codec for Linux:

- Send low latency, stereo high quality audio to one or multiple receivers.

- Useful for audio links in radio stations.


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


Libs needed to compile: libopus-dev, libasound-dev

more info for opus: http://opus-codec.org/
