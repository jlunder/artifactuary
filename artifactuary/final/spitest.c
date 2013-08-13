#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <linux/spi/spidev.h>

int main(int argc, char* argv[])
{
    uint32_t speed = 2000000;
    uint8_t bits = 8;
    uint8_t mode = 0;
    uint8_t data[] = {
      0xFF, 0xFF, 0xFF,
      0x00, 0x00, 0x00,
      0xFF, 0x00, 0x00, // blue
      0x00, 0xFF, 0x00, // red
      0x00, 0x00, 0xFF, // green
      0xFF, 0xFF, 0xFF,
    };
    
    int fd = open("/dev/spidev0.0", O_RDWR);
    
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    
    write(fd, data, sizeof data);
    
    return 0;
}
