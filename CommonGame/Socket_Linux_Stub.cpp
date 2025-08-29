// Minimal Linux stubs for Socket class to satisfy linker
#include "Socket.h"
#include <string>

using namespace std;

Socket::Socket() {}
// Don't define destructor - let it use default

bool Socket::init() { return true; }
bool Socket::initUDP(const string& host, unsigned short port) { return true; }
bool Socket::waitForData(unsigned int timeout) { return false; }
bool Socket::receiveData(char* buffer, int size, int* bytesReceived) { 
    if (bytesReceived) *bytesReceived = 0; 
    return false; 
}
bool Socket::sendData(const char* data, int size) { return true; }
bool Socket::close(Uint32 timeout, bool force) { return true; }
