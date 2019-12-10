#include "i2c-bq24295.h"
void _i2c_send_8(I2C_HandleTypeDef *i2c_port, uint8_t address, uint8_t reg, uint8_t data);
uint8_t _i2c_recv_8(I2C_HandleTypeDef *i2c_port, uint8_t address, uint8_t reg);


bool bq24295_init(I2C_HandleTypeDef *i2c_port) {
    uint8_t chip_id = _i2c_recv_8(i2c_port, BQ24295_DEVICE_ADDRESS, BQ24295_ID_REGISTER);

    return chip_id == 0b11000000;
}

uint8_t bq24295_get_status(I2C_HandleTypeDef *i2c_port) {
    return _i2c_recv_8(i2c_port, BQ24295_DEVICE_ADDRESS, BQ24295_SYS_STATUS_REGISTER);
}

void bq24295_enable_shipping_mode(I2C_HandleTypeDef *i2c_port){
    uint8_t op_timer = _i2c_recv_8(i2c_port, BQ24295_DEVICE_ADDRESS, BQ24295_TIMER_REGISTER);
    op_timer &= 0b11001111; // Set WATCHDOG to 0b00
    _i2c_send_8(i2c_port, BQ24295_DEVICE_ADDRESS, BQ24295_TIMER_REGISTER, op_timer);

    uint8_t op_control = _i2c_recv_8(i2c_port, BQ24295_DEVICE_ADDRESS, BQ24295_OP_CONTROL_REGISTER);
    op_control |= 0b00100000; // Set BATTFET_Disable to 1
    _i2c_send_8(i2c_port, BQ24295_DEVICE_ADDRESS, BQ24295_OP_CONTROL_REGISTER, op_control);
}

uint8_t _i2c_recv_8(I2C_HandleTypeDef *i2c_port,  uint8_t address, uint8_t reg ){
    uint8_t result;
    HAL_I2C_Master_Transmit(i2c_port, address, &reg, 1, HAL_TIMEOUT); //set register pointer   
    HAL_Delay(1); 
    HAL_I2C_Master_Receive(i2c_port, address, &result, 1, HAL_TIMEOUT); //read twoo bytes from register 
    return result;
}

void _i2c_send_8(I2C_HandleTypeDef *i2c_port, uint8_t address, uint8_t reg, uint8_t data){
    uint8_t data_buffer[2];
    data_buffer[0] = reg;
    data_buffer[1] = data;
    HAL_I2C_Master_Transmit(i2c_port, address, &data_buffer[0], 2, HAL_TIMEOUT);
}