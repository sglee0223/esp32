
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

#define LCD1602_I2C_ADDR (0x27)//(0x27 << 1)

#define LCD1602_GPIO_SDA	21
#define LCD1602_GPIO_SCL	22

#define PIN_RS    (1 << 0)
#define PIN_EN    (1 << 2)
#define BACKLIGHT (1 << 3)

#define LCD_DELAY_MS 5

int lcd1602_SendInternal(uint8_t data, uint8_t flags);
void lcd1602_SendCommand(uint8_t cmd);
void lcd1602_SendData(uint8_t data);
void lcd1602_SendString(char *str);
void lcd1602_Init(void);
void i2c_scan(void);
void lcd1602_i2c_init(void);