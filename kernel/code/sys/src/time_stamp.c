#include <time_stamp.h>
#include <ARMCM3.h>
#include <mktype.h>

#if USE_TIME_STAMP

#define	DWT_DEMCR_REG	(*(volatile mk_uint32 *)0XE000EDFC)
#define DWT_CTRL_REG	(*(volatile mk_uint32 *)0XE0001000)
#define DWT_CYCCNT_REG	(*(volatile mk_uint32 *)0XE0001004)
	
#define DWT_ENABLE		(1<<24)
#define DWT_CYCCNTENA	(1<<0)

static mk_uint32 BSP_CPU_ClkFrep(void){
	mk_uint32 system_clock;
	
	system_clock = SystemCoreClock;
	
	return system_clock;
}

void mk_time_stamp_init(void){
	mk_uint32 fclk_frep;
	fclk_frep = BSP_CPU_ClkFrep();
	//³õÊ¼»¯DWT
	DWT_DEMCR_REG |= (mk_uint32)DWT_ENABLE;
	DWT_CYCCNT_REG = (mk_uint32)0u;
	DWT_CTRL_REG  &= (~((mk_uint32)DWT_CYCCNTENA));
}

mk_uint32 mk_get_time_stamp(void){
	mk_uint32 time = 0;
	
	time = (mk_uint32)DWT_CYCCNT_REG;
	
	return time;
}

#endif


