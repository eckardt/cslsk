# cslsk

This project is an incomplete command-line client for the Soulseek peer-to-peer file-sharing network I created between 2004 and 2006.

My motivation for this was to have fun and learn UNIX network programming in C.

Back at the time I used a [Toshiba Libretto 100CT](https://en.wikipedia.org/wiki/Toshiba_Libretto) which didn't have a lot of RAM and CPU so I tried to use command line tools as much as possible since they were lighter on the tiny computer's limited resources.

A challenging part of this project was reverse-engineering the Soulseek networking protocol.
The official client was a closed-source program for Microsoft Windows and didn't come with a protocol specification describing the data structures sent between the Soulseek client and server. At the time there was only one other open-source implementation written in python. The author of this client created an incomplete protocol specification which I based my work on.

In addition to that I used Ethereal (today called Wireshark) to record the data streams sent and received by the official Windows client.
I analyzed the recorded network traffic to figure out the order of messages sent by the client and to debug my own implementation.

The code is functional but neither fast nor beautiful.
The client is incomplete and not really usable but at some point I got to download some unreleased tracks by Aphex Twin.
