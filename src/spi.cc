#include "spi.hpp"
//#define DBG
//#define PRINT_BUFFER

void SPI::init(){
fs = open(SPI_DEVICE, O_RDWR);
    if(fs < 0) {
        printf("Could not open the SPI device...\r\n");
        exit(EXIT_FAILURE);
    }

    ret = ioctl(fs, SPI_IOC_RD_MODE, &scratch32);
    if(ret != 0) {
        printf("Could not read SPI mode...\r\n");
        close(fs);
        exit(EXIT_FAILURE);
    }
    scratch32 |= SPI_MODE_0;
    ret = ioctl(fs, SPI_IOC_WR_MODE, &scratch32);//SPI_IOC_WR_MODE32
    if(ret != 0) {
        printf("Could not write SPI mode...\r\n");
        close(fs);
        exit(EXIT_FAILURE);
    }
#ifdef DBG
	printf("before this->spi_speed %d\n",scratch32);
#endif
	ret = ioctl(fs, SPI_IOC_RD_MAX_SPEED_HZ, &scratch32);
#ifdef DBG
	printf("after this->spi_speed %d\n",scratch32);
#endif
    if(ret != 0) {
        printf("Could not read the SPI max speed...\r\n");
        close(fs);
        exit(EXIT_FAILURE);
    }

    scratch32 = 5000000;

    ret = ioctl(fs, SPI_IOC_WR_MAX_SPEED_HZ, &scratch32);
#ifdef DBG
	printf("before this->spi_speed %d\n",scratch32);
#endif
    if(ret != 0) {
        printf("Could not write the SPI max speed...\r\n");
        close(fs);
        exit(EXIT_FAILURE);
    }
#ifdef DBG
printf("this->spi_speed %d\n",scratch32);
#endif
}



void SPI::read_write(size_t size ,uint16_t cmd){
#ifdef DBG
	std::cout<<"size_t : "<< size <<"\n";
#endif
spi.len = size;
    for(looper=0; looper<4; ++looper) {
        tx_buffer[looper] = cmd;
        rx_buffer[looper] = 0x0;
    }

ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
#ifdef DBG
	printf("fs : status : %d\n" , fs);
#endif
    if(ret != 0) {
#ifdef DBG
  printf("SPI transfer returned %d...\r\n", ret);
#endif
 }
#ifdef PRINT_BUFFER
    printf("Received SPI buffer...\r\n");
    for(looper=0; looper<32; ++looper) {
        printf(" %02x",rx_buffer[looper]);
    }
#endif
}

void SPI::spi_close(){
    close(fs);
    exit(EXIT_SUCCESS);
}

void SPI::settings_spi(){
spi.tx_buf = (unsigned long)tx_buffer;
spi.rx_buf = (unsigned long)rx_buffer;
spi.bits_per_word = 0;
spi.speed_hz = spi_speed;
spi.delay_usecs = 0;
spi.len = 2;//despues se modifica
   for(looper=0; looper<4; ++looper) {
        tx_buffer[looper] = 0x00;
        rx_buffer[looper] = 0xFF;
    }
}

SPI::SPI(){
std::cout<<"constructor spi\n";
	init();
	settings_spi();
}

SPI::~SPI(){
std::cout<<"destructor spi\n";
	spi_close();
if(fs)
	close(fs);
}



void SPI::write( uint16_t cmd){

spi.len = 2;
/*    for(looper=0; looper<2; ++looper)
    {
        tx_buffer[looper] = cmd;
        rx_buffer[looper] = 0x0;
	printf("looper : %d \n",looper);
    }
*/  
    tx_buffer[1] = cmd;
//  tx_buffer[0] = (cmd >>8 )&& 0xff;
ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
    if(ret != 0) return;
  return ;
}
