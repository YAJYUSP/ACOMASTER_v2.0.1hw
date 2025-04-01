#include "cs43131.h" 
#include "simi2c.h"
#include "main.h"

//CODEC全局音量值，0-100
uint8_t cs43131_volume = 0;


// Vol: 0 = mute, 100 = 0 dB
void bsp_43131_set_volume(uint8_t vol) 
{
		// Set volume for channel B
		CS43131_WriteReg(0x90001, ~(vol * 255 / 100));
		// Set volume for channel A
		CS43131_WriteReg(0x90002, ~(vol * 255 / 100));
}

/* Initialization sequence based on Datasheet */
void bsp_43131_init(void) 
{
/*PLL SETTINGS: INPUT MCLK 24.576MHz*/
		// PLL reference divide select.
    CS43131_WriteReg(0x40002, 0x3);
		// PLL integer portion of divide ratio. Integer portion of PLL feedback divider
    CS43131_WriteReg(0x30005, 0x49);
		// PLL fractional portion of divide ratio LSB.
    CS43131_WriteReg(0x30002, 0x0);
		// PLL fractional portion of divide ratio middle byte
    CS43131_WriteReg(0x30003, 0x0);
		// PLL fractional portion of divide ratio MSB
    CS43131_WriteReg(0x30004, 0x80);
		// Final PLL clock output divide value.
    CS43131_WriteReg(0x30008, 0x0A);
		// 500/512 factor used in PLL frequency calculation equation
    CS43131_WriteReg(0x3001B, 0x01);
		// Enable PLL output after it has been properly configured
    CS43131_WriteReg(0x30001, 0x01);
		
/*Clear pending interrupts*/
    CS43131_ReadReg(0xf0000);

/* Configure ASP interface*/
    // Set ASP sample rate to 96kHz
		/* 0x00:32KHz, 0x01:44.1KHz, 0x02:48KHz */
		/* 0x04:96KHz, 0x06:192KHz, 0x08:384KHz */
    CS43131_WriteReg(0x1000b, 0x02); 
    // Set ASP sample bit size
		// 0x00: 32 bits, 0x05: 24bits 0x06: 16bits
    CS43131_WriteReg(0x1000c, 0x05); 
    // Set ASP numerator//////////////////////////////////////////////////////////to be confed
    CS43131_WriteReg(0x40010, 0x01);
    CS43131_WriteReg(0x40011, 0x00);
    // Set ASP denominator///////////////////////////////////////////////////////
    CS43131_WriteReg(0x40012, 0x08);
    CS43131_WriteReg(0x40013, 0x00);
    // Set ASP LRCK high time
    CS43131_WriteReg(0x40014, 0x1f);
    CS43131_WriteReg(0x40015, 0x00);
    // Set ASP LRCK period
    CS43131_WriteReg(0x40016, 0x3f);
    CS43131_WriteReg(0x40017, 0x00);
    // Set ASP to slave, configure clock polarity
    CS43131_WriteReg(0x40018, 0x00);
    // Configure ASP frame
    CS43131_WriteReg(0x40019, 0x0a);
    // Set ASP channel location
    CS43131_WriteReg(0x50000, 0x00);
    CS43131_WriteReg(0x50001, 0x00);
    // Set ASP channel size and enable
    // For 24 bit per sample
    CS43131_WriteReg(0x5000a, 0x06);
    CS43131_WriteReg(0x5000b, 0x0e);

/* Configure PCM interface*/
    // Configure PCM filter
    CS43131_WriteReg(0x90000, 0x02);
    // Set volume for channel B
    CS43131_WriteReg(0x90001, 0x00);
    // Set volume for channel A
    CS43131_WriteReg(0x90002, 0x00);
    // Configure PCM path signal control
    CS43131_WriteReg(0x90003, 0xec);
    CS43131_WriteReg(0x90004, 0x00);

/* Configure HP*/
    // Configure class H amplifier
    CS43131_WriteReg(0xb0000, 0x1e);
    // Set HP output to full scale
    CS43131_WriteReg(0x80000, 0x30);
    // Disable headphone detect////////////////////////////////////////////
    CS43131_WriteReg(0xd0000, 0x04);
		
		// Power on appropriate interface and wait
		CS43131_WriteReg(0x20000, 0x00);////////////////////////////////////////////////////////////////
		HAL_Delay(5);
		
/*Clear pending interrupts*/
    CS43131_ReadReg(0xf0000);
		CS43131_ReadReg(0xf0001);

    // Enable ASP interrupts
    CS43131_WriteReg(0xf0011, 0x07);

    // Internal MCLK is expected to be 24.576 MHz, PLLCLK Mode and wait
    CS43131_WriteReg(0x10006, 0x01);

    // Wait at least 150us
    HAL_Delay(5);

    // Enable ASP clocks
    CS43131_WriteReg(0x1000d, 0x02);

/*Power up HP*/
		// Pop-free power-up settings
    CS43131_WriteReg(0x10010, 0x99);
		CS43131_WriteReg(0x80032, 0x20);
    
		// wait for 12ms
		HAL_Delay(12);
		//  Restore default settings
		CS43131_WriteReg(0x80032, 0x00);
		CS43131_WriteReg(0x10010, 0x00);
		
		
		
		//    // Configure XTAL driver
//    CS43131_WriteReg(0x20052, 0x04);
//    // Clear pending interrupts
//    CS43131_ReadReg(0xf0000);
//    // Enable XTAL interrupts
//    CS43131_WriteReg(0xf0010, 0xe7);
//    // Start XTAL
//    CS43131_WriteReg(0x20000, 0xf6);

//    // Set ASP sample rate to 44.1kHz
//    CS43131_WriteReg(0x1000b, 0x01);
//    // Set ASP sample bit size
//    CS43131_WriteReg(0x1000c, 0x06); // 0x00: 32 bits, 0x05: 24bits 0x06: 16bits
//    // Set ASP numerator
//    CS43131_WriteReg(0x40010, 0x01);
//    CS43131_WriteReg(0x40011, 0x00);
//    // Set ASP denominator
//    CS43131_WriteReg(0x40012, 0x08);
//    CS43131_WriteReg(0x40013, 0x00);
//    // Set ASP LRCK high time
//    CS43131_WriteReg(0x40014, 0x1f);
//    CS43131_WriteReg(0x40015, 0x00);
//    // Set ASP LRCK period
//    CS43131_WriteReg(0x40016, 0x3f);
//    CS43131_WriteReg(0x40017, 0x00);
//    // Set ASP to master, configure clock polarity
//    CS43131_WriteReg(0x40018, 0x1c); //0x1c
//    // Set ASP to slave, configure clock polarity
//    //CS43131_WriteReg(0x40018, 0x0c);
//    // Configure ASP frame
//    CS43131_WriteReg(0x40019, 0x0a);
//    // Set ASP channel location
//    CS43131_WriteReg(0x50000, 0x00);
//    CS43131_WriteReg(0x50001, 0x00);
//    // Set ASP channel size and enable
//    // For 16 bit per sample
//    CS43131_WriteReg(0x5000a, 0x05);
//		CS43131_WriteReg(0x5000b, 0x0d);
//    // For 32 bit per sample
//    //CS43131_WriteReg(0x5000a, 0x07);
//    //CS43131_WriteReg(0x5000b, 0x0f);

//    // Configure PCM filter
//    CS43131_WriteReg(0x90000, 0x02);
//    // Set volume for channel B
//    CS43131_WriteReg(0x90001, 0x00);
//    // Set volume for channel A
//    CS43131_WriteReg(0x90002, 0x00);
//    // Configure PCM path signal control
//    CS43131_WriteReg(0x90003, 0xec);
//    CS43131_WriteReg(0x90004, 0x00);

//    // Configure class H amplifier
//    CS43131_WriteReg(0xb0000, 0x1e);
//    // Set HP output to full scale
//    CS43131_WriteReg(0x80000, 0x30);
//    // Disable headphone detect
//    CS43131_WriteReg(0xd0000, 0x04);

//    // Enable ASP interrupts
//    CS43131_WriteReg(0xf0011, 0x07);

//    // Wait for XTAL to be ready
//    while (!(CS43131_ReadReg(0xf0000) & 0x10))
//		{
//				HAL_Delay(1);
//    }
// 
//    // Switch MCLK source to XTAL
//    CS43131_WriteReg(0x10006, 0x04);

//    // Wait at least 150us
//    HAL_Delay(1);

//    // Enable CLKOUT
//    CS43131_WriteReg(0x20000, 0xf4);

//    // Enable ASP clocks
//    CS43131_WriteReg(0x1000d, 0x02);

//    // Power up HP
//    //CS43130_PCMPowerUp();

}




static uint8_t CS_I2C_WritePayload(uint8_t addr, uint8_t *payload, uint8_t len) 
{
		uint8_t i;
    IIC_Start();
    IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令
    if(IIC_Wait_Ack())	//等待应答
    {
        IIC_Stop();
        return 1;
    }
    for(i=0; i<len; i++)
    {
        IIC_Send_Byte(payload[i]);	//发送数据
        if(IIC_Wait_Ack())		//等待ACK
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_Stop();
		return 0;
}

static uint8_t CS_I2C_ReadPayload(uint8_t addr, uint8_t *tx_payload, uint8_t tx_len, uint8_t *rx_payload, uint8_t rx_len) 
{
		uint8_t i;
    IIC_Start();
    IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令
    if(IIC_Wait_Ack())	//等待应答
    {
        IIC_Stop();
        return 1;
    }
    for(i=0; i<tx_len; i++)
    {
        IIC_Send_Byte(tx_payload[i]);	//发送数据
        if(IIC_Wait_Ack())		//等待ACK
        {
            IIC_Stop();
            return 1;
        }
    }
    //IIC_Stop();
    IIC_Send_Byte((addr<<1)|1);//发送器件地址+读命令
    IIC_Wait_Ack();		//等待应答
    while(rx_len)
    {
        if(rx_len==1)*rx_payload=IIC_Read_Byte(0);//读数据,发送nACK
        else *rx_payload=IIC_Read_Byte(1);		//读数据,发送ACK
        rx_len--;
        rx_payload++;
    }
    IIC_Stop();	//产生一个停止条件
		return 0;
}

static uint8_t CS43131_ReadReg(uint32_t reg) 
{
    uint8_t tx_payload[4];
    uint8_t rx_val;

    // Register address
    tx_payload[0] = (reg >> 16) & 0xff;
    tx_payload[1] = (reg >> 8) & 0xff;
    tx_payload[2] = (reg) & 0xff;
    // Control byte
    tx_payload[3] = 0x00;

    CS_I2C_ReadPayload(CS43131_ADDR, tx_payload, 4, &rx_val, 1);
    return rx_val;
}

static void CS43131_WriteReg(uint32_t reg, uint8_t val) 
{
    uint8_t tx_payload[5];

    // Register address
    tx_payload[0] = (reg >> 16) & 0xff;
    tx_payload[1] = (reg >> 8) & 0xff;
    tx_payload[2] = (reg) & 0xff;
    // Control byte
    tx_payload[3] = 0x00;
    // Register value
    tx_payload[4] = val;

    CS_I2C_WritePayload(CS43131_ADDR, tx_payload, 5);
}
