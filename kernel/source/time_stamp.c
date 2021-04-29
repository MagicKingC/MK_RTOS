#include <time_stamp.h>
#include <ARMCM3.h>

#if USE_TIME_STAMP
static mk_uint32 BSP_CPU_ClkFrep(void){
	mk_uint32 system_clock;
	
	system_clock = SystemCoreClock;
	
	return system_clock;
}


void mk_time_stamp_init(void){
	mk_uint32 fclk_frep;
	fclk_frep = BSP_CPU_ClkFrep();

}
#endif


