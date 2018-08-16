
#define I2C_MASTER_NUM             I2C_NUM_1        /*!< I2C port number for master dev */
#define I2C_MASTER_TX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE  0                /*!< I2C master do not need buffer */
#define I2C_MASTER_FREQ_HZ         100000            /*!< I2C master clock frequency */

#define WRITE_BIT                          I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT                           I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN                       0x1              /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS                      0x0              /*!< I2C master will not check ack from slave */
#define ACK_VAL                            0x0              /*!< I2C ack value */
#define NACK_VAL                           0x1              /*!< I2C nack value */

/* Display resolution */
#define SSD1306_LCD_WIDTH	128
#define SSD1306_LCD_HEIGHT	64

/* I2C settings */
#define SSD1306_I2C_ADDR	0x3C

/* GPIO settings */
#define SSD1306_GPIO_RST	16
#define SSD1306_GPIO_SDA	4
#define SSD1306_GPIO_SCL	15

#define SSD1306_CMD_ADDR_MODE		0x20
#define SSD1306_CMD_SET_COLUMN_ADDR	0x21
#define SSD1306_CMD_SET_PAGE_ADDR	0x22
#define SSD1306_CMD_SET_START_LINE	0x40
#define SSD1306_CMD_SET_CONTRAST	0x81
#define SSD1306_CMD_SET_CHARGEPUMP	0x8D
#define SSD1306_CMD_SET_SEGMENT_REMAP	0xA1
#define SSD1306_CMD_DISPLAY_ALL_ON_RES	0xA4
#define SSD1306_CMD_NORMAL		0xA6
#define SSD1306_CMD_SET_MUX		0xA8
#define SSD1306_CMD_DISPLAY_OFF		0xAE
#define SSD1306_CMD_DISPLAY_ON		0xAF
#define SSD1306_CMD_SET_COM_SCAN_INC	0xC0
#define SSD1306_CMD_SET_COM_SCAN_DEC	0xC8
#define SSD1306_CMD_SET_OFFSET		0xD3
#define SSD1306_CMD_SET_CLK_DIV		0xD5
#define SSD1306_CMD_SET_PRECHARGE	0xD9
#define SSD1306_CMD_SET_COM_PINS	0xDA
#define SSD1306_CMD_SET_VCOM_DESELECT	0xDB
#define SSD1306_CMD_PAGE_START_ADDR	0xB0
#define SSD1306_CMD_COLUMN_LOW_ADDR	0x00
#define SSD1306_CMD_COLUMN_HIGH_ADDR	0x10
#define SSD1306_ADDR_MODE_HORIZ	0
#define SSD1306_ADDR_MODE_VERT	1
#define SSD1306_ADDR_MODE_PAGE	2

int ssd1306_set_addr_mode(uint8_t);

int ssd1306_set_cursor(uint8_t row, uint8_t column);

int ssd1306_clear(void);

int ssd1306_putc(uint8_t c);

int ssd1306_puts(char string[]);

int ssd1306_init(void);

void ssd1306_i2c_init(void);

void ssd1306_reset(void);

void ssd1306_i2c_scan(void);
