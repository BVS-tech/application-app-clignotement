#ifndef Test_SERIAL_COMMUNICATION_H
#define Test_SERIAL_COMMUNICATION_H

#include "liBVSSerialCommunication.h"

// COM ordi      "/dev/ttyUSB0"
// COM raspberry "/dev/ttyAMA0"

#define TEST_NUMBER_BYTES 5

/**
 * \SurgirisSerialCommunication
 * Class to through serial communication using the Surgiris protcol.
 *
 * The user must redefine the function `initCommunication` to open 
 * the serial port and set its communication parameters.
 */
class Test_SerialCommunication : public SerialCommunication
{
    
    public :
        Test_SerialCommunication(std::string COMName);
        int on(int ID, int Power);
        int off(int ID);
        int change_power(int ID, int Power);
         
    protected :         
        /// Open ans initilise the serial communication.
        int initCommunication();
};

#endif
