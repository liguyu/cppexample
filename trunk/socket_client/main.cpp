//------------------------------------------------------------------------------
// Echo Server Client
//------------------------------------------------------------------------------
#include <string.h>
#include "ActiveSocket.h"       // Include header for active socket object definition

#define TEST_PACKET "TestPacket"
#define MAX_PACKET  1024

int main(int argc, char **argv)
{
    CActiveSocket socket;       // Instantiate active socket object (defaults to TCP).
    char          szData[MAX_PACKET];
	
    memset(szData, 0, MAX_PACKET);
	
    //--------------------------------------------------------------------------
    // Initialize our socket object
    //--------------------------------------------------------------------------
    socket.Initialize();
	
    //--------------------------------------------------------------------------
    // Create a connection to the time server so that data can be sent
    // and received.
    //--------------------------------------------------------------------------
    if (socket.Open((const uint8 *)"localhost", 6789))
    {
        //----------------------------------------------------------------------
        // Send a requtest the server requesting the current time.
        //----------------------------------------------------------------------
        if (socket.Send((const uint8 *)TEST_PACKET, strlen(TEST_PACKET)))
        {
            //------------------------------------------------------------------
            // Receive response from the server.
            //------------------------------------------------------------------
            socket.Receive(1024);
			
            strncpy(szData, (const char *)socket.GetData(), socket.GetBytesReceived());
            printf("%s\n", szData);
			
            //------------------------------------------------------------------
            // Close the connection.
            //------------------------------------------------------------------
            socket.Close();
        }
    }
	
	
    return 1;
}