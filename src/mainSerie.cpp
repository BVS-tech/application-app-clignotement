#include "Lucibel_SerialCommunication.h"
#include <cstdlib>
#include <iostream>
#include <sstream>

Lucibel_SerialCommunication serial("/dev/ttyAMA0");
clock_t tMessage;

using namespace std;
#define ID_MASTER 102
#define DELAY_UART 100
long id_slave = 15;

int main()
{
    char j;
    while(1)
    {
        cout << "appuis sur o, a, s, u ou f";
        cin >> j;

        if (j == 'o')
	    {
	        serial.on(15, 150);
	    }    
        if (j == 'f')
	    {
	        serial.off(15);
	    }
	    if (j == 'a')
	    {
	        string strf = "12345";
                strf[0] = id_slave;
                strf[1] = ID_MASTER;
                strf[2] = 26;
                strf[3] = 255;
                strf[4] = 255;
                serial.writeWithDelay(strf, DELAY_UART);
	    }    
        if (j == 's')
	    {
	        string strf = "12345";
                strf[0] = id_slave;
                strf[1] = ID_MASTER;
                strf[2] = 75;
                strf[3] = 255;
                strf[4] = 255;
                serial.writeWithDelay(strf, DELAY_UART);
	    }
	    if (j == 'u')
	    {
	        string strf = "12345";
                strf[0] = id_slave;
                strf[1] = ID_MASTER;
                strf[2] = 12;
                strf[3] = 255;
                strf[4] = 255;
                serial.writeWithDelay(strf, DELAY_UART);
	    }
    }
}
