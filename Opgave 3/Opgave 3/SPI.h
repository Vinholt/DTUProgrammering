  
/*
    
    Kasper Nyhus Kaae
        s195087
---------------------------------
SPI module for
ARDUINO MEGA2560
---------------------------------
MISO  =>  pin 50 (PB3)
MOSI  =>  pin 51 (PB2)
SCK   =>  pin 52 (PB1)
SS    =>  pin 53 (PB0)
*/

extern void SPI_init_master();
extern void SPI_init_slave();
extern void SPI_MasterTransmit(char cData);
extern char SPI_SlaveReceive(void);