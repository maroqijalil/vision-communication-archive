#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <linux/serial.h>
#include <sys/ioctl.h>

#include <vector>
#include <iostream>

#define MAXNUM_TXPARAM      (256)
#define MAXNUM_RXPARAM      (1024)

enum
{
    ID                  = 2,
    LENGTH              = 3,
    INSTRUCTION         = 4,
    ERRBIT              = 4,
    PARAMETER           = 5,
    DEFAULT_BAUDNUMBER  = 1
};

enum
{
    ID_CM			= 200,
    ID_BROADCAST	= 254
};

enum
{
    INST_PING           = 1,
    INST_READ           = 2,
    INST_WRITE          = 3,
    INST_REG_WRITE      = 4,
    INST_ACTION         = 5,
    INST_RESET          = 6,
    INST_SYNC_WRITE     = 131,
    INST_BULK_READ      = 146
};

enum
{
    P_MODEL_NUMBER_L		= 0,
    P_MODEL_NUMBER_H		= 1,
    P_VERSION				= 2,
    P_ID					= 3,
    P_BAUD_RATE				= 4,
    P_RETURN_DELAY_TIME		= 5,			
    P_RETURN_LEVEL			= 16,
    P_DXL_POWER				= 24,
    P_LED_PANNEL			= 25,
    P_LED_HEAD_L			= 26,
    P_LED_HEAD_H			= 27,
    P_LED_EYE_L				= 28,
    P_LED_EYE_H				= 29,
    P_BUTTON				= 30,			
    P_GYRO_Z_L				= 38,
    P_GYRO_Z_H				= 39,
    P_GYRO_Y_L				= 40,
    P_GYRO_Y_H				= 41,
    P_GYRO_X_L				= 42,
    P_GYRO_X_H				= 43,
    P_ACCEL_X_L				= 44,
    P_ACCEL_X_H				= 45,
    P_ACCEL_Y_L				= 46,
    P_ACCEL_Y_H				= 47,
    P_ACCEL_Z_L				= 48,
    P_ACCEL_Z_H				= 49,
    P_VOLTAGE				= 50,
    P_LEFT_MIC_L			= 51,
    P_LEFT_MIC_H			= 52,
    P_ADC2_L				= 53,
    P_ADC2_H				= 54,
    P_ADC3_L				= 55,
    P_ADC3_H				= 56,
    P_ADC4_L				= 57,
    P_ADC4_H				= 58,
    P_ADC5_L				= 59,
    P_ADC5_H				= 60,
    P_ADC6_L				= 61,
    P_ADC6_H				= 62,
    P_ADC7_L				= 63,
    P_ADC7_H				= 64,
    P_ADC8_L				= 65,
    P_ADC8_H				= 66,
    P_RIGHT_MIC_L			= 67,
    P_RIGHT_MIC_H			= 68,
    P_ADC10_L				= 69,
    P_ADC10_H				= 70,
    P_ADC11_L				= 71,
    P_ADC11_H				= 72,
    P_ADC12_L				= 73,
    P_ADC12_H				= 74,
    P_ADC13_L				= 75,
    P_ADC13_H				= 76,
    P_ADC14_L				= 77,
    P_ADC14_H				= 78,
    P_ADC15_L				= 79,
    P_ADC15_H				= 80,
    MAXNUM_ADDRESS
};

enum
{
    SUCCESS,
    TX_CORRUPT,
    TX_FAIL,
    RX_FAIL,
    RX_TIMEOUT,
    RX_CORRUPT
};

int Socket_fd;

int readPort(unsigned char* packet, int numPacket)
{
	return read(Socket_fd, packet, numPacket);
}

void write_command(int id, int address, int value)
{
    unsigned char txpacket[MAXNUM_TXPARAM + 10] = {0};
	unsigned char rxpacket[MAXNUM_RXPARAM + 10] = {0};

    txpacket[0]            = 0xFF;
    txpacket[1]            = 0xFF;
    txpacket[ID]           = (unsigned char)id;
    txpacket[LENGTH]       = 4;
    txpacket[INSTRUCTION]  = INST_WRITE;
	txpacket[PARAMETER]    = (unsigned char)address;
    txpacket[PARAMETER+1]  = (unsigned char)value;

    unsigned char checksum = 0x00;
    for(int i=2; i<txpacket[LENGTH]+3; i++)
		checksum += txpacket[i];
	int length = txpacket[LENGTH] + 4;

    txpacket[length - 1] = ~checksum;    

    while(true)
    {
        tcflush(Socket_fd, TCIFLUSH);
        write(Socket_fd, txpacket, length);
    
        int to_length = 0;

        if(txpacket[INSTRUCTION] == INST_READ)
            to_length = txpacket[PARAMETER+1] + 6;
        else
            to_length = 6;

        int get_length = 0;
        
        fprintf(stderr, "RX: ");
        
        while(1)
        {
            length = readPort(&rxpacket[get_length], to_length - get_length);

            if(rxpacket[get_length]==NULL)
                exit(0);

            if(true)
            {
                for(int n=0; n<length; n++)
                    fprintf(stderr, "%.2X ", rxpacket[get_length + n]);
            }
            get_length += length;

            if(get_length == to_length)
            {
                int i;
                for(i = 0; i < (get_length - 1); i++)
                {
                    if(rxpacket[i] == 0xFF && rxpacket[i+1] == 0xFF)
                        break;
                    else if(i == (get_length - 2) && rxpacket[get_length - 1] == 0xFF)
                        break;
                }

                if(i == 0)
                {
                    unsigned char checksum2 = 0x00;
                    for(int i=2; i<rxpacket[LENGTH]+3; i++)
                        checksum2 += rxpacket[i];
                    if(true)
                        fprintf(stderr, "CHK:%.2X\n", checksum2);
                    
                    break;
                }
                else
                {
                    for(int j = 0; j < (get_length - i); j++)
                        rxpacket[j] = rxpacket[j+i];
                    get_length -= i;
                }						
            }
        }

    }
}

int main(int argc, char **argv)
{
    Socket_fd = open("/dev/ttyUSB1", O_RDWR|O_NOCTTY|O_NONBLOCK);

    struct termios newtio;
    struct serial_struct serinfo;
    double baudrate = 1000000.0;

    memset(&newtio, 0, sizeof(newtio));
    newtio.c_cflag      = B38400|CS8|CLOCAL|CREAD;
    newtio.c_iflag      = IGNPAR;
    newtio.c_oflag      = 0;
    newtio.c_lflag      = 0;
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN]   = 0;
    tcsetattr(Socket_fd, TCSANOW, &newtio);

    ioctl(Socket_fd, TIOCGSERIAL, &serinfo);

    serinfo.flags &= ~ASYNC_SPD_MASK;
    serinfo.flags |= ASYNC_SPD_CUST;
    serinfo.flags |= ASYNC_LOW_LATENCY;
    serinfo.custom_divisor = serinfo.baud_base / baudrate;

    int res = ioctl(Socket_fd, TIOCSSERIAL, &serinfo);
    
    // example: turning LED Pannel on
    write_command(ID_CM, P_LED_PANNEL, atoi(argv[1]));
  
    return 0;    
}