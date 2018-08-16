
#include <stdio.h>
#include "driver/i2c.h"

#include "lcd1602_lib.h"

int lcd1602_write_cmd(uint8_t addr, uint8_t cmd)
{
    i2c_cmd_handle_t hCmd = i2c_cmd_link_create();
    i2c_master_start(hCmd);
    i2c_master_write_byte(hCmd, ( addr << 1 ) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(hCmd, cmd, ACK_CHECK_EN);
    i2c_master_stop(hCmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, hCmd, 10 / portTICK_RATE_MS);
    i2c_cmd_link_delete(hCmd);

    //if (ret != ESP_OK)
    //    printf("lcd1602_write_cmd Error [0x%x]\n", cmd);

    return ret;
}

int lcd1602_write_data(uint8_t addr, uint8_t data[], int len)
{
    i2c_cmd_handle_t hCmd = i2c_cmd_link_create();
    i2c_master_start(hCmd);
    i2c_master_write_byte(hCmd, ( addr << 1 ) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write(hCmd, data, len, ACK_CHECK_EN);
    i2c_master_stop(hCmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, hCmd, 10 / portTICK_RATE_MS);
    i2c_cmd_link_delete(hCmd);

    //if (ret != ESP_OK)
    //    printf("lcd1602_write_data Error [0x%x]\n", data[0]);

    return ret;
}

int lcd1602_write_cmdNdata(uint8_t addr, uint8_t cmd, uint8_t data)
{
    i2c_cmd_handle_t hCmd = i2c_cmd_link_create();
    i2c_master_start(hCmd);
    i2c_master_write_byte(hCmd, ( addr << 1 ) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(hCmd, cmd, ACK_CHECK_EN);
    i2c_master_write_byte(hCmd, data, ACK_CHECK_EN);
    i2c_master_stop(hCmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, hCmd, 10 / portTICK_RATE_MS);
    i2c_cmd_link_delete(hCmd);

    //if (ret != ESP_OK)
    //    printf("lcd1602_write_cmdNdata Error [0x%x]\n", cmd);

    return ret;
}

int lcd1602_SendInternal(uint8_t data, uint8_t flags)
{
	int res;
	uint8_t up = data & 0xF0;
	uint8_t lo = (data << 4) & 0xF0;

	uint8_t data_arr[4];
	data_arr[0] = up|flags|BACKLIGHT|PIN_EN;
	data_arr[1] = up|flags|BACKLIGHT;
	data_arr[2] = lo|flags|BACKLIGHT|PIN_EN;
	data_arr[3] = lo|flags|BACKLIGHT;

    res = lcd1602_write_data(LCD1602_I2C_ADDR, data_arr, 4);

    vTaskDelay(LCD_DELAY_MS / portTICK_RATE_MS);
	return res;
}

void lcd1602_SendCommand(uint8_t cmd)
{
	lcd1602_SendInternal(cmd, 0);
}

void lcd1602_SendData(uint8_t data)
{
	lcd1602_SendInternal(data, PIN_RS);
}

void lcd1602_SendString(char *str)
{
	while(*str)
	{
		lcd1602_SendData((uint8_t)(*str));
		str++;
	}
}

void lcd1602_Init(void)
{
	// 4-bit mode, 2 lines, 5x7 format
	lcd1602_SendCommand(0x30);
	// display & cursor home (keep this!)
	lcd1602_SendCommand(0x02);
	// display on, right shift, underline off, blink off
	lcd1602_SendCommand(0x0C);
	// clear display (optional here)
	lcd1602_SendCommand(0x01);
}

void i2c_scan(void)
{
    int res;
	char info[] = "Scanning I2C bus...\r\n";

	printf("%s", info);

	for(uint16_t i = 0; i < 128; i++)
	{
		res = lcd1602_write_cmd(i, 0x00);
		if(res == ESP_OK)
		{
    		char msg[64];

    		snprintf(msg, sizeof(msg), "0x%02X", i);
    		printf("%s", msg);
		}
        else
		{
			printf("%s", ".");
		}
	}

	printf("%s", "\r\n");
}

void lcd1602_i2c_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = LCD1602_GPIO_SDA;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = LCD1602_GPIO_SCL;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    i2c_driver_install(i2c_master_port, conf.mode,
                       I2C_MASTER_RX_BUF_DISABLE,
                       I2C_MASTER_TX_BUF_DISABLE, 0);
}