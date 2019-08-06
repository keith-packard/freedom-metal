#ifndef METAL__DRIVERS__SYNOPSIS_I2C_H
#define METAL__DRIVERS__SYNOPSIS_I2C_H

#include<metal/compiler.h>
#include<metal/drivers/riscv_plic0.h>
#include<metal/i2c.h>
#include <metal/clock.h>

struct __metal_driver_vtable_synopsys_i2c
{
	const struct metal_i2c_vtable i2c;
};

struct __metal_driver_synopsys_i2c;

 void __metal_driver_synopsys_i2c_init(struct metal_i2c *i2c,struct metal_i2c_config *cfg);
 int __metal_driver_synopsys_i2c_write_transfer(struct metal_i2c *i2c,struct metal_i2c_config *cfg,int len,unsigned char *tx_buf);
 int __metal_driver_synopsys_i2c_read_transfer(struct metal_i2c *i2c,struct metal_i2c_config *cfg,int len,unsigned char *rx_buf);
 int __metal_driver_synopsys_i2c_set_address_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg);
 int __metal_driver_synopsys_i2c_set_speed_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg);
 int __metal_driver_synopsys_i2c_set_target_address(struct metal_i2c *i2c,struct metal_i2c_config *cfg,unsigned int address);
// struct metal_interrupt* __metal_driver_synopsys_i2c_get_interrupt_controller(struct metal_i2c *i2c);
// int __metal_driver_synopsys_i2c_get_interrupt_id(struct metal_i2c *i2c);


__METAL_DECLARE_VTABLE(__metal_driver_vtable_synopsys_i2c) =
{

	.i2c.init				= __metal_driver_synopsys_i2c_init,
	.i2c.write_transfer 	= __metal_driver_synopsys_i2c_write_transfer,
	.i2c.read_transfer 		= __metal_driver_synopsys_i2c_read_transfer,
	.i2c.set_address_mode 	= __metal_driver_synopsys_i2c_set_address_mode,
	.i2c.set_speed_mode 	= __metal_driver_synopsys_i2c_set_speed_mode,
	.i2c.set_target_address = __metal_driver_synopsys_i2c_set_target_address,
	//.i2c.get_interrupt_controller = __metal_driver_synopsys_i2c_get_interrupt_controller,
	//.i2c.get_interrupt_id   = __metal_driver_synopsys_i2c_get_interrupt_id
};



struct __metal_driver_synopsys_i2c
{

 struct metal_i2c i2c;
 const struct __metal_driver_vtable_synopsys_i2c *vtable;
 const unsigned long control_base;
 const unsigned long control_size;
 const struct metal_clock *clock;
 const struct metal_interrupt *interrupt_parent;
 const int num_interrupts;
 const int interrupt_lines[];
 
};

#endif
