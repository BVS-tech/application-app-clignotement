#ifndef LIBVS_SERIAL_COMMUNICATION_H
#define LIBVS_SERIAL_COMMUNICATION_H

#include <iostream>
#include <string>

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>  
#include <time.h>

/**
 * \SerialCommunication
 * Basic class allowing to communicate through serial communication
 *
 * The user must redefine the function `initCommunication` to open 
 * the serial port and set its communication parameters.
 */
class SerialCommunication
{

    public :
        /// Open a terminal and initialise the communication
        /**
         * \param COMName [in] name of the port to open
         */
        SerialCommunication(std::string COMName);

        /// Close the Communication
        virtual ~SerialCommunication();

        /// Write a message through the port.
        /**
         * Does not work if the port is not open.
         *
         * \param message [in] message to write
         *
         * \return if the write was succesful or not : -1 = not good
         */         
        int writeMessage(std::string message);
        /// Read the port
        /**
         * Does not work if the port is not open.
         *
         * \param message [out] message read
         *
         * \return if the read was succesful or not : -1 = not good
         */
         int readMessage(std::string& message);
        
        /// Write a message to the port, wait if last message is too recent.
        /**
         * Does not work if the port is not open.
         * Block the programme while waiting.
         *
         * \param message [in] message to write
         * \param delay [in] minimal time to wait between two message (in ms)
         *
         * \return if the write was succesful or not
         */         
        int writeWithDelay(std::string message, clock_t delay = 15);
        
    protected :
        std::string portName; ///< Name of the port
        int serialHandle; ///< Handle for the port once open
        
        clock_t tLastMessage; ///< Time of the last message send
        
        /*
        int init();
        /// Open ans initilise the serial communication.
        virtual int initCommunication() = 0; 
        */
};

#endif
