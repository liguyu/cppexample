//------------------------------------------------------------------------------
// ECHO SERVER CODE
//------------------------------------------------------------------------------
#include "PassiveSocket.h"       // Include header for active socket object definition

#define MAX_PACKET 4096 

int main(int argc, char **argv)
{
    CPassiveSocket socket;
    CActiveSocket *pClient = NULL;
    //--------------------------------------------------------------------------
    // Initialize our socket object 
    //--------------------------------------------------------------------------
    socket.Initialize();
	
    socket.Listen((const uint8 *)"127.0.0.1", 6789);

	printf("ServerAddr:%s, ServerPort:%d.\nWait connect Listenning...\n",socket.GetServerAddr(),socket.GetServerPort());

    while (true)
    {
        if ((pClient = socket.Accept()) != NULL)
        {
			printf("Received a connection from:%s, type:%d\n",pClient->GetClientAddr() , pClient->GetSocketType());

            //------------------------------------------------------------------
            // Receive request from the client.
            //------------------------------------------------------------------
            if (pClient->Receive(MAX_PACKET))
            {
				for (uint16 i=0;i<pClient->GetBytesReceived(); i++)
				{
					printf("%c ",*(pClient->GetData()+i));
				}
				printf("\n");
                //--------------------------------------------------------------
                // Send response to client and close connection to the client.
                //--------------------------------------------------------------
                pClient->Send((const uint8 *)pClient->GetData(), pClient->GetBytesReceived());
                pClient->Close();
            }
			
            delete pClient;
        }
    }
	
    //--------------------------------------------------------------------------
    // Receive request from the client.
    //--------------------------------------------------------------------------
    socket.Close();
	
    return 1;
}
