#include "Test_SerialCommunication.h"

using namespace std;

Test_SerialCommunication::Test_SerialCommunication(std::string COMName) :
    SerialCommunication(COMName)
{
    initCommunication();
}
 
/// Open and initilise the serial communication.
int Test_SerialCommunication::initCommunication()
{
    struct termios opt;
    
	cout << "INIT " << serialHandle << endl;
	
	if ((serialHandle = open (portName.c_str(), O_RDWR | O_NONBLOCK | O_NOCTTY)) < 0)
	{
	    perror ("Test_SerialCommunication::initCommunication : open");
	    return false;
	}	
	
	cout << "Test_SerialCommunication::initCommunication: "
	     << "opening succesful " 
	     << serialHandle << endl;
	
	//--set Baud Rate
	tcflush (serialHandle, TCIOFLUSH);  
	cfsetispeed(&opt, B115200);    
    cfsetospeed(&opt, B115200);  
    
    if (tcsetattr(serialHandle, TCSANOW, &opt) != 0)  
    {  
        perror ("Test_SerialCommunication::initCommunication : tcsetattr speed");
        return false;  
    }  
    tcflush (serialHandle, TCIOFLUSH);  
        
    //--configure 8bits data, no parity, 1bit stop
    opt.c_cflag&=~CSIZE;
    opt.c_cflag|=CS8;
    opt.c_cflag&=~PARENB;
    opt.c_cflag &= ~CSTOPB; 
    tcflush(serialHandle,TCIFLUSH); 
    
    if (tcsetattr(serialHandle, TCSANOW, &opt) != 0)  
    {  
        perror ("Test_SerialCommunication::initCommunication : tcsetattr conf");
        return false;
    }
    
    return true;
}
int Test_SerialCommunication::on(int ID, int pwr)
{
    string stro = "12345";
    stro[0] = ID;
    stro[1] = 102; 
    stro[2] = 17; 
    stro[3] = 2; 
    stro[4] = pwr;
    writeMessage(stro); 
    cout << "message envoyee" << endl;
}
int Test_SerialCommunication::off(int ID)
{
    string strf = "12345";
    strf[0] = ID;
    strf[1] = 102;
    strf[2] = 17;
    strf[3] = 3;
    strf[4] = 255;
    writeMessage(strf);
    cout << "message envoyee" << endl;
}
int Test_SerialCommunication::change_power(int ID, int pwr)
{
    string strc = "12345";
    strc[0] = ID;
    strc[1] = 102;
    strc[2] = 17;   
    strc[3] = 4;
    strc[4] = pwr;
    writeMessage(strc);
}
