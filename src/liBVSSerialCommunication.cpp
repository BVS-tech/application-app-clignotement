#include "liBVSSerialCommunication.h"

using namespace std;

SerialCommunication::SerialCommunication(string COMName) : portName(COMName),
                                                           serialHandle(-1),
                                                           tLastMessage(0)
{
}

SerialCommunication::~SerialCommunication()
{   
    // je ne suis pas sur pour le 0.
    if (serialHandle >= 0)
    {
        close(serialHandle);
    }
}

/// Write a message to the port.      
int SerialCommunication::writeMessage(string message)
{
    if (serialHandle < 0)
    {
        cerr << "SerialCommunication::write : port not open" << endl;
	    return -1;
    }
    
	for (int i=0 ; i< message.size() ; i++)
	{
		if (write(serialHandle, &message[i], 1) <0)
		    perror ("SerialCommunication::write");
	}	
	tcflush (serialHandle, TCIOFLUSH);
	
	tLastMessage=clock();
	
	return message.size();
}
/// Read the port
int SerialCommunication::readMessage(string &message)
{
    if (serialHandle < 0)
    {
        cerr << "SerialCommunication::readMessage : port not open" << endl;
	    return -1;
    }
    
    char buff[50];
    int n = 0;
    
    fcntl(serialHandle, F_SETFL, FNDELAY);
    n = read(serialHandle, buff, sizeof(buff));
        
    if (n < 0)
    {
        // No message, there may have a timelapse before the message is send 
        // back
        //perror ("SerialCommunication::read");
        message = "";
    }
    else
    {
        message.assign(buff, n);
    }    
    
    return n;
}

/// Write a message to the port, wait if last message is too recent
int SerialCommunication::writeWithDelay(string message, clock_t delay)
{
    if (serialHandle < 0)
    {
        cerr << "SerialCommunication::writeWithDelay : port not open" << endl;
	    return -1;
    }   
    
    while((clock()-tLastMessage) <= ((delay*CLOCKS_PER_SEC)/1000));    
    
    return writeMessage(message); 
}
/*
int SerialCommunication::init()
{
    return initCommunication();
}
*/

