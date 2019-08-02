#include<metal/machine.h>
#include<metal/i2c.h>

extern inline void metal_i2c_init(struct metal_i2c *i2c,struct metal_i2c_config *cfg);
extern inline int metal_i2c_write_transfer(struct metal_i2c *i2c,struct metal_i2c_config *cfg ,int len,unsigned char *tx_buf);
extern inline int metal_i2c_read_transfer(struct metal_i2c *i2c,struct metal_i2c_config *cfg,int len,unsigned char *rx_buf);
extern inline int metal_i2c_set_address_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg);
extern inline int metal_i2c_set_speed_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg);
extern inline int metal_i2c_set_target_address(struct metal_i2c *i2c,struct metal_i2c_config *cfg,unsigned int address);
//extern inline struct metal_interrupt* metal_i2c_get_interrupt_controller(struct metal_i2c *i2c);
//extern inline int metal_i2c_get_interrupt_id(struct metal_i2c *i2c);

struct metal_i2c *metal_i2c_get_device(int device_num)
{
	if(device_num >= __METAL_DT_MAX_I2CS){return NULL;}

	return (struct metal_i2c *) __metal_i2c_table[device_num];
}
