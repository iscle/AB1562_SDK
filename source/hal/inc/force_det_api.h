
#ifndef __FORCE_DET_API_H__
#define __FORCE_DET_API_H__
#ifdef  HAL_CAPTOUCH_FORCE_TOUCH_ENABLED
typedef struct _force_det_par_ force_det_par;
void force_det_ini(force_det_par *par, short pre_thr, short rel_thr, short lev_thr, short cba_thr, short hw_thr_l, short ini_delay);
int get_force_det_par_size();
int force_det(force_det_par *par, short adc_val);
#endif
#endif
