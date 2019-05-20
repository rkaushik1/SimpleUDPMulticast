# SimpleUDPMulticast

A many to one system for communication using UDP. Does not serialize messages in any way, but does append an increasing sequence number.

clang++ -std=c++11 main.cpp Multiplexer.cpp -o run
