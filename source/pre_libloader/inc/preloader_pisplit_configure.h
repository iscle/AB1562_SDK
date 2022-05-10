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

#ifndef __PRELOADER_PILSI_CONFIGURE_H__
#define __PRELOADER_PILSI_CONFIGURE_H__

#include "xt_library_loader.h"


#ifdef __cplusplus
extern "C" {
#endif

extern xtlib_packaged_library dsp0_pisplit_library_build_demo;// user muts be extern your lib to here!!!


#if defined (DSP0_PISPLIT_DEMO_LIBRARY)
// user muts be add your lib to here!!!
#define PIC_LIB_LIST {\
    &dsp0_pisplit_library_build_demo,\
}
#else
#define PIC_LIB_LIST  {\
}
#endif



#ifdef __cplusplus
}
#endif

#endif /* __PRELOADER_PILSI_CONFIGURE_H__ */
