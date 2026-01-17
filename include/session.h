#ifndef DHCP_SESSION_H
#define DHCP_SESSION_H

/**
* file: session.h
*
* API for creating and handling DHCP sessions for the DHCP server. DHCP sessions are necessary for keeping track of transactions. We
* can identify a transaction specific to a client MAC address. The said client is the first to initiate a transaction by generating
* a session ID (XID) that we can find in the DHCP header. As such, we define a DHCP session by XID, leased IP address, and the lease time.
*
* As for the grace periods and deadlines, we can implement this after we have built the mechanism for keeping track of DHCP sessions.
*
* We define a session by its states (OFFERED, REQUESTED, BOUND, RENEWING). This is from the perspective of the server
*
* OFFERED: (initial) The server releases an available IP address to the client. Performs
*          a soft reservation for the IP. Here, the server awaits for a request proper to transition to the REQUESTED state.
*
* REQUESTED: The server received a DHCPREQUEST packet from the client, and is performing the necessary server ID, address, and state checks 
*            to verify the correctness.
* 
* BOUND: The server has transmitted a DHCPACK to the client, fully securing that IP to the client
*/



#endif DHCP_SESSION_H

