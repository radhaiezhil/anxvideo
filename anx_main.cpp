#include <boost/asio.hpp>
#include <iostream>
#include <sys/ioctl.h>
#include<vector>
#include <array>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <variant>
#include <unistd.h> 
#include <cstdlib>
#include <fcntl.h>


extern "C"
{
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
}

boost::asio::io_service io_service;
boost::posix_time::seconds interval(5);  // 5 second
boost::asio::deadline_timer timer(io_service, interval);

int i2c_writeread(char *i2c_dev,uint8_t slave_add, std::vector<uint8_t> writeDataReg,std::vector<uint8_t>& response,
                                    uint8_t readBytes, uint8_t writeBytes)
                                    
{
    struct i2c_msg messages[2];
    std::vector<unsigned char> writeData(writeBytes, 0);
    std::copy(writeDataReg.begin(), writeDataReg.end(), writeData.begin());
    std::vector<unsigned char> readData(readBytes, 0);
    int fd = open(i2c_dev,O_RDWR);
     // First message (write operation)
   
  
        messages[0].addr = slave_add;
        messages[0].flags = 0;
    	messages[0].len = writeBytes;
    	messages[0].buf = writeData.data();
        std::cout<<
            "ANX I2C Write"<<std::endl;

 


    	// Second message (read operation)
    	messages[1].addr = slave_add;
    	messages[1].flags = I2C_M_RD; 
    	messages[1].len = readBytes;
    	messages[1].buf = readData.data(); // Buffer to store the read data
           std::cout<<
            "ANX I2C Read"<<std::endl;


   
    struct i2c_rdwr_ioctl_data ioctl_data;
    ioctl_data.msgs = messages;
    ioctl_data.nmsgs = 2;
    // I2C read and write operation
    if (ioctl(fd, I2C_RDWR, &ioctl_data) < 0)
    {
         std::cout<<
            "ANX Error in i2c write Read"<<std::endl;
         return false;
    }
    std::copy(readData.data(), readData.data() + readBytes / sizeof(readData[0]),
              std::back_inserter(response));
    return true;
}

int InitANXDevice (void)
{
    uint8_t  	Slave;
    char	i2c_dev [16];
    int 	retval = 0;
    
    uint8_t write_count = -1;
    uint8_t read_count = -1;
    std::vector<uint8_t> write_buffer;
    std::vector<uint8_t> read_buffer;
    
    #define ANX_RESET_VALUE		0x82
/* Create the device file string */
    std::cout<<"ANX Init"<<std::endl;

    snprintf( i2c_dev, sizeof(i2c_dev),"/dev/i2c-8");
   
    
       //	system("modprobe i2c_hw");
	
    read_count = 1;
    write_count = 0;
    write_buffer [write_count++] = 0x05;
    Slave = 0x72;
    
	retval = i2c_writeread ( i2c_dev, (Slave >> 1), write_buffer, read_buffer,  write_count, read_count);
        std::cout<<"ANX Reg 0x05 value"<<read_buffer[0]<<std::endl;
	if( retval < 0 )
	{
		return -1;
        }
	if ( (read_buffer[0] & ANX_RESET_VALUE) ){    
		 //system("/usr/local/bin/i2c-test -b 8 -s 0x39 -m 1 -rc 1 -d 0x5 0x0");
    	write_count = 0;
    	write_buffer [write_count++] = 0x05;
    	write_buffer [write_count++] = 0x00;
    	Slave = 0x72;
        

		 
		 retval = i2c_writeread ( i2c_dev, (Slave >> 1), write_buffer, read_buffer,  write_count, read_count);
		 if( retval < 0 )
		 {
			return -1;
		 }
		 
		 //system("/usr/local/bin/i2c-test -b 8 -s 0x39 -m 1 -rc 1 -d 0x6 0x6");
		 write_count = 0;
		 write_buffer [write_count++] = 0x06;
		 write_buffer [write_count++] = 0x06;
		 
	 retval = i2c_writeread ( i2c_dev, (Slave >> 1), write_buffer, read_buffer,  write_count, read_count);
	 if( retval < 0 )
		 {
			 return -1;
		 }
		 
		 //system("/usr/local/bin/i2c-test -b 8 -s 0x39 -m 1 -rc 1 -d 0x6 0x4");
		 write_count = 0;
		 write_buffer [write_count++] = 0x06;
		 write_buffer [write_count++] = 0x04;
		
		 retval = i2c_writeread ( i2c_dev, (Slave >> 1), write_buffer, read_buffer,  write_count, read_count);
		 if( retval < 0 )
	 {
			 return -1;
		 }
		 
		 //	system("/usr/local/bin/i2c-test -b 8 -s 0x39 -m 1 -rc 1 -d 0x7 0x40");
		 write_count = 0;
		 write_buffer [write_count++] = 0x07;
		 write_buffer [write_count++] = 0x40;
		
		 retval = i2c_writeread ( i2c_dev, (Slave >> 1), write_buffer, read_buffer,  write_count, read_count);
		 if( retval < 0 )
		 {
		 return -1;
		 }		 
		 

		 //	system("/usr/local/bin/i2c-test -b 8 -s 0x38 -m 1 -rc 1 -d 0xa8 0x01");
		 write_count = 0;
		 write_buffer [write_count++] = 0xa8;
		 write_buffer [write_count++] = 0x01;
		 Slave = 0x70;
		
		 retval = i2c_writeread ( i2c_dev, (Slave >> 1), write_buffer, read_buffer,  write_count, read_count);
		 if( retval < 0 )
		 {
			return -1; 
		 }
		 
		 //	system("/usr/local/bin/i2c-test -b 8 -s 0x39 -m 1 -rc 1 -d 0x08 0x81");
		 write_count = 0;
		 write_buffer [write_count++] = 0x08;
		 write_buffer [write_count++] = 0x81;
		 Slave = 0x72;
		
		 retval = i2c_writeread ( i2c_dev, (Slave >> 1), write_buffer, read_buffer,  write_count, read_count);
		 if( retval < 0 )
		 {
			 return -1;
		 }	 
		 
	 return retval;
	} else {
		return retval;
	}
	//return 0;

}

void MonitorANX ()
{
	
	char        i2c_dev [16];
	uint8_t  	Slave;
	
	uint8_t write_count = -1;
        uint8_t read_count = -1;
        std::vector<uint8_t> write_buffer;
        std::vector<uint8_t> read_buffer;

		
	#define ANX_DP_PLUG		0x01	//cable plug in detected
	#define ANX_DP_HPD_LOST		0x02	//cable remove detected
	#define ANX_DP_HOTPLUG_CHG 	0x04	//change in cable state
	#define ANX_RESET_VALUE		0x82	
	
        if(snprintf( i2c_dev, sizeof(i2c_dev),"/dev/i2c-8") >= (signed int)sizeof(i2c_dev))
        {
       		printf("Buffer Overflow in File: %s Line: %d  Function : %s\n",__FILE__, __LINE__, __func__);
        	return;
        }
	//first check if the device has been reset
	write_count = 0;
	read_count = 1;
	
	write_buffer [write_count++] = 0x05;		
	Slave = 0x72;				
	
	if( i2c_writeread ( i2c_dev, (Slave >> 1), write_buffer, read_buffer,  write_count, read_count) == -1)
	{
		return;
	
	}
	result = read_buffer[0];
	
	if ( ( result & ANX_RESET_VALUE) ){
	
		PDK_InitANXDevice();
		
	}
	
	write_count = 0;
	write_buffer [write_count++] = 0xF4;		//#define CI_STATUS_REGISTER_4 0xF4
	Slave = 0x72;					//#define ANX_SLAVE_DEVICE 0x72


    //if(g_HALI2CHandle[HAL_I2C_RW] != NULL){

    	// check Common Interrupt Status Register 4 (72, F4) 
		if( i2c_writeread ( i2c_dev, (Slave >> 1), write_buffer, read_buffer,  write_count, read_count) == -1)
		{
			return;
		}

		result = read_buffer[0];
			
		// if bit 2 - HOTPLUG_CHG == 1
		//if HOTPLUG_CHG = 1, check PLUG (0) and HPD_LOST (1). Clear HOTPLUG_CHG by writing 1
		if( (result & ANX_DP_HOTPLUG_CHG) ){
			//a change has been detected. Determine which it is
			
			if ( (result & ANX_DP_PLUG) ){
				//run link training

				write_count = 0;
				write_buffer [write_count++] = 0xA8;
				write_buffer [write_count++] = 0x01;
				Slave = 0x70;
				if( i2c_writeread ( i2c_dev, (Slave >> 1), write_buffer, read_buffer,  write_count, read_count)== -1)
				{
					return;
				}
				
				//clear the ANX_DP_HPD_LOST bit
				write_count = 0;
				write_buffer [write_count++] = 0xF4;
				write_buffer [write_count++] = ANX_DP_HPD_LOST;
				Slave = 0x72;

				if( i2c_writeread ( i2c_dev, (Slave >> 1), write_buffer, read_buffer,  write_count, read_count)== -1)
				{
					return;
				}
			}
			
			if ( (result & ANX_DP_HPD_LOST) ){
				write_count = 0;
				write_buffer [write_count++] = 0xF4;
				write_buffer [write_count++] = ANX_DP_PLUG;
				Slave = 0x72;
				if(i2c_writeread ( i2c_dev, (Slave >> 1), write_buffer, read_buffer,  write_count, read_count) == -1)
				{
					return;
				}
			}
			
			//clear ANX_DP_HOTPLUG_CHG bit
			write_count = 0;
			write_buffer [write_count++] = 0xF4;
			write_buffer [write_count++] = ANX_DP_HOTPLUG_CHG;
			Slave = 0x72;
			if(i2c_writeread ( i2c_dev, (Slave >> 1), write_buffer, read_buffer,  write_count, read_count) == -1)
			{
				return;
			}
		}
	//}
    // Reschedule the timer for 5 second in the future:
    timer.expires_at(timer.expires_at() + interval);
    // Posts the timer event
    timer.async_wait(MonitorANX);
 }
 
int main(void) {
    // Schedule the timer for the first time:
    timer.async_wait(MonitorANX);
    // Enter IO loop. The timer will fire for the first time 1 second from now:
    io_service.run();
    return 0;
}
