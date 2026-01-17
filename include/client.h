#ifndef DHCP_CLIENT_H
#define DHCP_CLIENT_H

/**
* file: client.h
*
* Contains clientside structures and methods. The clientside is capable of creating a DHCP session with a server (if found). 
* The clientside must keep track of its states which determine its stage in the life cycle of DHCP handling. Since DHCP requires the knowledge of MAC and
* IP addresses, the clientside must contain the necessary API handling for determining these kinds of information in supplication to the packet handling API 
* through packet.h.
*/

#include <iostream>
#include "packet.h"



#endif DHCP_PACKET_H

