/* Copyright Statement:
 *
 * (C) 2017  Airoha Technology Corp. All rights reserved.
 *
 * This software/firmware and related documentation ("Airoha Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to Airoha Technology Corp. ("Airoha") and/or its licensors.
 * Without the prior written permission of Airoha and/or its licensors,
 * any reproduction, modification, use or disclosure of Airoha Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) Airoha Software
 * if you have agreed to and been bound by the applicable license agreement with
 * Airoha ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of Airoha Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT AIROHA SOFTWARE RECEIVED FROM AIROHA AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. AIROHA EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES AIROHA PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH AIROHA SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN AIROHA SOFTWARE. AIROHA SHALL ALSO NOT BE RESPONSIBLE FOR ANY AIROHA
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND AIROHA'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO AIROHA SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT AIROHA'S OPTION, TO REVISE OR REPLACE AIROHA SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * AIROHA FOR SUCH AIROHA SOFTWARE AT ISSUE.
 */

#include "mux.h"
#include "mux_port_device.h"
#include "race_cmd.h"
#include "syslog_debug.h"

virtual_read_write_point_t g_mux_airapp_r_w_point;
bool online_log_start = false;

#ifdef MTK_CPU_NUMBER_0

void airapp_send_data(uint8_t port_index,mux_ctrl_para_t *para)
{
    uint8_t  channel_id;
    uint32_t channelNum;
    virtual_read_write_point_t *p=&g_mux_airapp_r_w_point;

    PORT_MUX_UNUSED(port_index);
    PORT_MUX_UNUSED(para);
    //uint32_t p_tx_hw_rptr, p_tx_hw_wptr;
    //p_tx_hw_rptr = mux_common_device_get_hw_rptr(p, false);
    //p_tx_hw_wptr = mux_common_device_get_hw_wptr(p, false);
    channelNum = RACE_GetChannelIdByType(&channel_id, 1, RACE_CHANNEL_TYPE_AIRAPP);
    if(channelNum)
    {
        U8* pAirApp = RACE_ClaimPacket(RACE_TYPE_NOTIFICATION, 0xffff, p->tx_buff_available_len, channel_id);
        if(pAirApp)
        {
            if(p->tx_buff_write_point > p->tx_buff_read_point)
            {
                memcpy(pAirApp - 6, (void *)p->tx_buff_read_point, p->tx_buff_available_len);
            }
            else
            {
                memcpy(pAirApp - 6, (void *)p->tx_buff_read_point, p->tx_buff_end - p->tx_buff_read_point);
                memcpy(pAirApp - 6 + p->tx_buff_end - p->tx_buff_read_point, (void *)p->tx_buff_start, p->tx_buff_write_point - p->tx_buff_start);
            }
            RACE_FlushPacket(pAirApp, p->tx_buff_available_len);
        }
    }
    mux_common_device_set_tx_hw_rptr_internal_use(&g_mux_airapp_r_w_point, p->tx_buff_available_len);

}

mux_status_t port_mux_airapp_normal_init(uint8_t port_index, mux_port_config_t *p_setting,mux_irq_handler_t irq_handler)
{
    PORT_MUX_UNUSED(port_index);
    PORT_MUX_UNUSED(irq_handler);

    virtual_read_write_point_t *p = &g_mux_airapp_r_w_point;
    mux_common_device_r_w_point_init(p,p_setting);

    return MUX_STATUS_OK;
}

mux_status_t port_mux_airapp_deinit(uint8_t port_index)
{
    PORT_MUX_UNUSED(port_index);
    return MUX_STATUS_OK;
}

void port_mux_airapp_exception_init(uint8_t port_index)
{
    PORT_MUX_UNUSED(port_index);
}

void port_mux_airapp_exception_send(uint8_t port_index, uint8_t *buffer, uint32_t size)
{
    PORT_MUX_UNUSED(port_index);
    PORT_MUX_UNUSED(buffer);
    PORT_MUX_UNUSED(size);
}

bool port_mux_airapp_get_buf_is_full(uint8_t port_index, bool is_rx)
{
    PORT_MUX_UNUSED(port_index);
    PORT_MUX_UNUSED(is_rx);

    return 0;
}

#endif


uint32_t port_mux_airapp_get_hw_rptr(uint8_t port_index, bool is_rx)
{
    PORT_MUX_UNUSED(port_index);
    PORT_MUX_UNUSED(is_rx);
    return mux_common_device_get_hw_rptr(&g_mux_airapp_r_w_point,is_rx);
}

uint32_t port_mux_airapp_get_hw_wptr(uint8_t port_index, bool is_rx)
{
    PORT_MUX_UNUSED(port_index);
    PORT_MUX_UNUSED(is_rx);
    return mux_common_device_get_hw_wptr(&g_mux_airapp_r_w_point,is_rx);
}

void port_mux_airapp_set_hw_rptr(uint8_t port_index, uint32_t move_bytes)
{
    PORT_MUX_UNUSED(port_index);
    PORT_MUX_UNUSED(move_bytes);
    mux_common_device_set_rx_hw_rptr(&g_mux_airapp_r_w_point, move_bytes);
}

void port_mux_airapp_set_hw_wptr(uint8_t port_index, uint32_t move_bytes)
{
    virtual_read_write_point_t *p=&g_mux_airapp_r_w_point;
    PORT_MUX_UNUSED(port_index);

    mux_common_device_set_tx_hw_wptr(p,move_bytes);

    if(online_log_start != true){
        mux_common_device_set_tx_hw_rptr_internal_use(p, move_bytes);
        return;
    }
}

mux_status_t port_mux_airapp_control(uint8_t port_index, mux_ctrl_cmd_t command, mux_ctrl_para_t *para)
{
    virtual_read_write_point_t *p=&g_mux_airapp_r_w_point;
    PORT_MUX_UNUSED(para);

    if(port_index > (MUX_AIRAPP_END - MUX_AIRAPP_BEGIN)) {
        return MUX_STATUS_ERROR;
    }

    switch(command) {
        case MUX_CMD_CONNECT: {
            online_log_start = true;
        }break;

        case MUX_CMD_DISCONNECT:{
            online_log_start = false;
        }break;

        case MUX_CMD_CLEAN: {
        }break;

        case MUX_CMD_GET_VIRTUAL_TX_AVAIL_LEN: {
            para->mux_get_tx_avail.ret_size = p->tx_buff_available_len;
        }break;

        case MUX_CMD_GET_VIRTUAL_RX_AVAIL_LEN: {
            para->mux_get_rx_avail.ret_size = p->rx_buff_available_len;
        }break;

        case MUX_CMD_TX_BUFFER:{
            airapp_send_data(port_index, para);
        }break;

        default:break;
    }

    return MUX_STATUS_OK;
}

port_mux_device_ops_t g_port_mux_airapp_ops = {
#ifdef MTK_CPU_NUMBER_0
    port_mux_airapp_normal_init,
    port_mux_airapp_deinit,
    NULL,
    NULL,
    port_mux_airapp_get_buf_is_full,
#endif
    port_mux_airapp_get_hw_rptr,
    port_mux_airapp_set_hw_rptr,
    port_mux_airapp_get_hw_wptr,
    port_mux_airapp_set_hw_wptr,
    port_mux_airapp_control,
};

