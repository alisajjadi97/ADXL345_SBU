#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>

#define ADXL345_ADDRESS 0x53
#define ADXL345_REG_POWER_CTL 0x2D
#define ADXL345_REG_DATA_FORMAT 0x31
#define ADXL345_REG_DATAX0 0x32
unsigned char data_format;

void i2c_adxl_init();void adxl_setrange(int);int read_word_2c(int);
int fd;//wiringPi I2C Setup Fuction Output

int main(int argc, char **argv)
{
	wiringPiSetupPhys();
	i2c_adxl_init();	
	adxl_setrange(2);
	while(1){
		printf("ACC_X:%d \n",read_word_2c(ADXL345_REG_DATAX0));
		delay(50);
	}	
	return 0;
}

void i2c_adxl_init()
{
	fd = wiringPiI2CSetup (ADXL345_ADDRESS);
	wiringPiI2CWriteReg8 (fd,ADXL345_REG_POWER_CTL,0x08);//disable sleep mode 
	printf("set Register Power Control  =%X\n",wiringPiI2CReadReg8 (fd,ADXL345_REG_POWER_CTL));
}

void adxl_setrange(int g)
{
	data_format = wiringPiI2CReadReg8 (fd,ADXL345_REG_DATA_FORMAT);
	printf("The Range was =%X\n",wiringPiI2CReadReg8 (fd,ADXL345_REG_DATA_FORMAT));	
	
	switch (g) {
		case 2 : //2G
			data_format |= 0X00; 
		break;
		case 4 : //4G
			data_format |= 0X01;
		break;
		case 8 ://8G
			data_format |= 0X02;
		break;
		case 16 : //16G
			data_format |= 0X03;
		break;		
	}
	data_format |= 0x08;//make sure that the FULL-RES bit is enabled for range scaling	
	wiringPiI2CWriteReg8 (fd,ADXL345_REG_DATA_FORMAT,data_format);//Set Range	
	printf("set Range to =%X\n",wiringPiI2CReadReg8 (fd,ADXL345_REG_DATA_FORMAT) );
}
int read_word_2c(int addr)
{
  int val;
  val = wiringPiI2CReadReg8(fd, addr+1);
  val = val << 8;
  val += wiringPiI2CReadReg8(fd, addr);
  if (val >= 0x8000)
    val = -(65536 - val);

  return val;
}
