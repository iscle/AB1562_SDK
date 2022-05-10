/*
 *  Minimal configuration for TLS 1.1 (RFC 4346)
 *
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
/*
 * Minimal configuration for TLS 1.1 (RFC 4346), implementing only the
 * required ciphersuite: MBEDTLS_TLS_RSA_WITH_3DES_EDE_CBC_SHA
 *
 * See README.txt for usage instructions.
 */

#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

#if 1 // mbedtls related settings
/* System support */
//#define MBEDTLS_PLATFORM_MEMORY
//#define MBEDTLS_PLATFORM_NO_STD_FUNCTIONS
//#define MBEDTLS_PLATFORM_SNPRINTF_ALT

/* mbed TLS feature support */
//#define MBEDTLS_PKCS1_V15
//#define MBEDTLS_PKCS1_V21

/* mbed TLS modules */
//#define MBEDTLS_ASN1_PARSE_C
//#define MBEDTLS_BIGNUM_C
//#define MBEDTLS_MD_C
//#define MBEDTLS_OID_C
//#define MBEDTLS_PLATFORM_C
//#define MBEDTLS_RSA_C
#define MBEDTLS_SHA256_C
#define MBEDTLS_CHACHA20_C
#endif

#include "mbedtls/check_config.h"

#if 0 // lzma related settings
#define LZMA_LZMADEC_C
#endif

#endif /* MBEDTLS_CONFIG_H */
