#include <metal/drivers/synopsys_i2c.h>
#include <metal/io.h>

// no need of below #defines need to be placed in the bare_metal
#define IC_ENABLE 0x6c
#define IC_CON 0x00
#define IC_TAR 0x4
#define IC_FS_SPKLEN 0xa0 
#define IC_SS_SCL_HCNT 0X14
#define IC_SS_SCL_LCNT 0x18
#define IC_FS_SCL_LCNT 0x20
#define IC_FS_SCL_HCNT 0x1C
#define IC_HS_SCL_HCNT 0x24
#define IC_HS_SCL_LCNT 0x28
#define IC_INTR_MASK 0x30
#define IC_RX_TL 0X38
#define IC_TX_TL 0X3C
#define IC_SAR 0X8
#define IC_ACK 0X98
#define IC_DATA_CMD 0X10
#define IC_UFM_TBUF_CNT 0x1c
#define IC_INTR_STAT 0x2c
#define IC_UFM_SCL_HCNT0x
#define IC_CLR_INTR 0x40
#define IC_CLR_RX_UNDER 0X44
#define IC_CLR_RX_OVER 0X48
#define IC_CLR_TX_OVER 0X4C
#define IC_CLR_RD_REQ 0X50
#define IC_CLR_TX_ABRT 0X54
#define IC_CLR_RX_DONE 0X58
#define IC_CLR_ACTIVITY 0X5C
#define IC_CLR_STOP_DET 0X60

#define IC_RAW_INTR_STAT 0X34

/////////////////////////////////////////
#define REGISTER_DATATYPE uint32_t
#define INTERFACE_SIZE 8
#define REGISTER_SIZE 32
#define METAL_I2C_STANDARD_MODE 0X1
#define METAL_I2C_FAST_SPEED  0x2
#define METAL_I2C_HIGH_SPEED 0x3

#define I2C_REG(offset) (((unsigned long)(((struct __metal_driver_synopsys_i2c *)(i2c))->control_base) + offset))
#define I2C_REGB(offset) (__METAL_ACCESS_ONCE((__metal_io_u8 *)I2C_REG(offset)))
#define I2C_REG_RW_BIT(offset,bit,val) (write_register(i2c,offset,set_bit(read_register(i2c,offset),bit,val)))
#define I2C_REG_RW_BIT_RANGE(offset,start_bit,stop_bit,val) (write_register(i2c,offset,set_bit_range(read_register(i2c,offset),start_bit,stop_bit,val)))

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


REGISTER_DATATYPE power(REGISTER_DATATYPE base,REGISTER_DATATYPE power)
{
	REGISTER_DATATYPE val = 1;

	if(power == 0)
	{
		return 1;
	}
	else
	{
		for(int i=0; i<power; ++i)
			{
				val = val*base; 
			}

	}	return val;
}

REGISTER_DATATYPE split_lsb(REGISTER_DATATYPE value, int bit)
{
	REGISTER_DATATYPE lsb;

		lsb = (power(2,bit)-1);
		lsb = (value & lsb);
		return lsb;					
}

REGISTER_DATATYPE get_new_msb(REGISTER_DATATYPE value,int bit,REGISTER_DATATYPE val)
{
	REGISTER_DATATYPE temp;

	temp = (value >> (bit+1))<<1;

	return (temp | val);
}

REGISTER_DATATYPE set_bit(REGISTER_DATATYPE value, int bit, REGISTER_DATATYPE val)
{
	REGISTER_DATATYPE MSB , LSB;

				LSB = split_lsb(value,bit);
				MSB = get_new_msb(value,bit,val);
				return ((MSB<<bit) | (LSB));
}

REGISTER_DATATYPE set_bit_range(REGISTER_DATATYPE value,int start_bit,int end_bit,REGISTER_DATATYPE val)
{
	REGISTER_DATATYPE max_val,bit_val,last_val;

	for(int i= start_bit; i<= end_bit; ++i)
	{
		max_val = power(2,i-start_bit);
		bit_val = (max_val & val)>>(i-start_bit); 
		last_val = set_bit(value,i,bit_val);
	}
	return last_val;
}

void write_register(struct metal_i2c *i2c,REGISTER_DATATYPE offset,REGISTER_DATATYPE value)
{

	for(int i=0; i<(REGISTER_SIZE/INTERFACE_SIZE); ++i)
	{
		if(INTERFACE_SIZE == 16)
		{
			I2C_REGB(offset+(2*i)) =  value>>(INTERFACE_SIZE*i);
		}
		else
		{
			 I2C_REGB(offset+i) = value>>(INTERFACE_SIZE*i);
		}
	}
}	


REGISTER_DATATYPE read_register(struct metal_i2c *i2c,REGISTER_DATATYPE offset)
{
	
	REGISTER_DATATYPE new_val;
	REGISTER_DATATYPE prev_val = 0;


	for(int i=0; i<(REGISTER_SIZE/INTERFACE_SIZE); ++i)
	{
		if(INTERFACE_SIZE == 16)
		{
			new_val = I2C_REGB(offset+(2*i));
		}
		else 
		{
			new_val = I2C_REGB(offset+i);
		}	
			
		new_val = new_val<<(INTERFACE_SIZE * i);
		prev_val = prev_val | new_val;
	}
	
		return prev_val;
}

REGISTER_DATATYPE read_register_bit(struct metal_i2c *i2c,REGISTER_DATATYPE offset,int bit)
{
	return ((read_register(i2c,offset) & power(2,bit))>>bit);
}

REGISTER_DATATYPE read_register_bit_range(struct metal_i2c *i2c,REGISTER_DATATYPE offset,int start_bit,int stop_bit)
{
	return ((read_register(i2c,offset) & ((power(2,(stop_bit-start_bit+1))-1)<<start_bit))>>start_bit);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int __metal_driver_synopsys_i2c_set_speed_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{	

	I2C_REG_RW_BIT(IC_ENABLE,0,0);

		if(cfg->speed_mode == METAL_I2C_STANDARD_MODE)
		{
			I2C_REG_RW_BIT_RANGE(IC_CON,1,2,cfg->speed_mode);
		}
		else if(cfg->speed_mode == METAL_I2C_FAST_SPEED)
		{
			I2C_REG_RW_BIT_RANGE(IC_CON,1,2,cfg->speed_mode);
		}
		else if(cfg->speed_mode == METAL_I2C_HIGH_SPEED)
		{
			I2C_REG_RW_BIT_RANGE(IC_CON,1,2,cfg->speed_mode);
		}

	I2C_REG_RW_BIT(IC_ENABLE,0,1);	
			
}		

int __metal_driver_synopsys_i2c_set_address_mode(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{

	I2C_REG_RW_BIT(IC_ENABLE,0,0);

		if(cfg->addressing == METAL_I2C_7_BIT)
		{
			I2C_REG_RW_BIT(IC_CON,3,cfg->addressing);
			I2C_REG_RW_BIT(IC_CON,4,cfg->addressing);
		}
		else if(cfg->addressing == METAL_I2C_10_BIT)
		{
			I2C_REG_RW_BIT(IC_CON,3,cfg->addressing);
			I2C_REG_RW_BIT(IC_CON,4,cfg->addressing);
		}

	I2C_REG_RW_BIT(IC_ENABLE,0,1);	
		
}		


int __metal_driver_synopsys_i2c_set_target_address(struct metal_i2c *i2c,struct metal_i2c_config *cfg,unsigned int address)
{
	write_register((struct metal_i2c *)(i2c),IC_TAR,set_bit(set_bit((REGISTER_DATATYPE)(address & 0x3ff),10,cfg->transmission_mode),11,cfg->enable_special_bit));
}

void __metal_driver_synopsys_i2c_init(struct metal_i2c *i2c,struct metal_i2c_config *cfg)
{
		I2C_REG_RW_BIT(IC_ENABLE,0,0);
		if(cfg->operation_mode == METAL_I2C_MASTER)
			{
				I2C_REG_RW_BIT(IC_CON,0,1);
				I2C_REG_RW_BIT(IC_CON,6,1);
				write_register((struct metal_i2c *)(i2c),IC_TAR,cfg->target_address);
			}	
		else if(cfg->operation_mode == METAL_I2C_SLAVE)
			{
				I2C_REG_RW_BIT(IC_CON,0,0);
				I2C_REG_RW_BIT(IC_CON,6,0);
				write_register((struct metal_i2c *)(i2c),IC_SAR,cfg->slave_address);
			}		

		__metal_driver_synopsys_i2c_set_address_mode((struct metal_i2c *)(i2c),cfg);	

		I2C_REG_RW_BIT(IC_CON,5,1);

		__metal_driver_synopsys_i2c_set_target_address((struct metal_i2c *)(i2c),cfg,cfg->target_address);		


		write_register((struct metal_i2c *)(i2c),IC_TX_TL,cfg->transmitter_threshold);
		write_register((struct metal_i2c *)(i2c),IC_RX_TL,cfg->receiver_threshold);
		
	
		I2C_REG_RW_BIT(IC_ENABLE,0,1);
		

}

int __metal_driver_synopsys_i2c_write_transfer(struct metal_i2c *i2c,struct metal_i2c_config *cfg,int len,unsigned char *tx_buf)
{
	for (int i = 0;i<len;i++)
		{
			if((!(read_register_bit((struct metal_i2c *)(i2c),IC_RAW_INTR_STAT,3))) && (len-i))
				{
					if(tx_buf)
						{
							REGISTER_DATATYPE value = set_bit(tx_buf[i],8,0);
							value = set_bit(value,9,cfg->issue_stop);
							value = set_bit(value,10,cfg->issue_restart);
							write_register((struct metal_i2c *)(i2c),IC_DATA_CMD,value);
						}
					else 
						{
							return -1;
						}	
				}
		}
}			

int __metal_driver_synopsys_i2c_read_transfer(struct metal_i2c *i2c,struct metal_i2c_config *cfg,int len,unsigned char *rx_buf)
{
	if(!(read_register_bit(i2c,IC_CON,0)))		{
				for(int i=0;i<len;i++)
				{
					rx_buf[i] = (unsigned char)read_register(i2c,IC_DATA_CMD);
				}
				return 1;
		}
   
}

__METAL_DEFINE_VTABLE(__metal_driver_vtable_synopsys_i2c) = {
	.i2c.init				= __metal_driver_synopsys_i2c_init,
	.i2c.write_transfer 	= __metal_driver_synopsys_i2c_write_transfer,
	.i2c.read_transfer 		= __metal_driver_synopsys_i2c_read_transfer,
	.i2c.set_address_mode 	= __metal_driver_synopsys_i2c_set_address_mode,
	.i2c.set_speed_mode 	= __metal_driver_synopsys_i2c_set_speed_mode,
	.i2c.set_target_address = __metal_driver_synopsys_i2c_set_target_address,
	//.i2c.get_interrupt_controller = __metal_driver_synopsys_i2c_get_interrupt_controller,
	//.i2c.get_interrupt_id   = __metal_driver_synopsys_i2c_get_interrupt_id
};



