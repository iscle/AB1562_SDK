/*
 * Copyright (c) 2012-2013 by Tensilica Inc. ALL RIGHTS RESERVED.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include "elf.h"
#ifdef __XTENSA__
#include <xtensa/hal.h>  /* xthal_memcpy */
#endif
#include <string.h>

#ifdef PRELOADER_ENABLE

#include"preloader_pisplit.h"
#include "preloader_pisplit_log.h"
#include"preloader_pisplit_configure.h"
#include "multi_pool_heap.h"
#include "xt_library_loader.h"

#include "hal.h"
#include "assert.h"
#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t _preloader_pic_iram_start[];    //this is DSP0 IRAM free start, defined in memory layout link script
extern uint32_t _preloader_pic_iram_end[];      //this is DSP0 IRAM free end, defined in memory layout link script

extern uint32_t _preloader_pic_dram_start[];    //this is DSP0 DRAM free start, defined in memory layout link script
extern uint32_t _preloader_pic_dram_end[];      //this is DSP0 DRAM free end, defined in memory layout link script

/* just a sample
extern uint32_t _preloader_pic_sysram_start[];
extern uint32_t _preloader_pic_sysram_end[];

extern uint32_t _preloader_pic_psram_start[];
extern uint32_t _preloader_pic_psram_end[];
*/

/*This structure for static PIC memory pools.
If you want to assign some static memory as PIC memory pools for PIC library.
User can register your pools information to here!!!
If user have no any change, please clone this structure to your project.
User must be pay attention to the parameter of preloader_pisplit_memory_type_t, this means the type of memory pool.
When you load your PIC load, should choose which memory type.
*/
preloader_pisplit_pool_info_t preloader_pisplit_customer_static_pools[] =
{
  /*{//SYSRAM   just a sample
        (void *)_preloader_pic_sysram_start,
        (uint32_t)_preloader_pic_sysram_end,
        PRELOADER_I_LOW_PERFORMANCE|PRELOADER_D_LOW_PERFORMANCE
    },
    {//PSRAM
        (void *)_preloader_pic_psram_start,
        (uint32_t)_preloader_pic_psram_end,
        PRELOADER_I_LOW_PERFORMANCE|PRELOADER_D_LOW_PERFORMANCE
    },*/
    {//IRAM
        (void *)_preloader_pic_iram_start,
        (uint32_t)_preloader_pic_iram_end,
        PRELOADER_I_HIGH_PERFORMANCE
    }, 
    {//DRAM
        (void *)_preloader_pic_dram_start,
        (uint32_t)_preloader_pic_dram_end,
        PRELOADER_D_HIGH_PERFORMANCE
    },
};


/*This function implement on by project level, for register the static PIC memory pools to Preloader_pisplit driver.
User must need this step, after this step, user can do PIC library load or use preloader_pisplit_malloc_memory()  to 
allocate extra memory from PIC memory heap.
Please clone this function to your project!!!
And make sure this API called on your init flow after preloader_pisplit_init()

example code on init flow:
    preloader_pisplit_init();
    pisplit_configure_static_pool();
*/
void pisplit_configure_static_pool()
{
    uint32_t count;
    count = sizeof(preloader_pisplit_customer_static_pools)/sizeof(preloader_pisplit_pool_info_t);
    preloader_pisplit_add_pools(preloader_pisplit_customer_static_pools,count);
    
}

#ifdef __cplusplus
}
#endif

#endif

