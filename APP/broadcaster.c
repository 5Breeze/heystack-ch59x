/********************************** (C) COPYRIGHT *******************************
 * File Name          : broadcaster.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        : 广播应用程序，初始化广播连接参数，然后处于广播态一直广播

 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "CONFIG.h"
#include "devinfoservice.h"
#include "broadcaster.h"

// 广播间隔 (units of 625us, min is 1600 = 1000ms)
#define DEFAULT_ADVERTISING_INTERVAL 1600 * 2

// 密钥转换间隔
#define SBP_PERIODIC_EVT_PERIOD 3600 * 3 * 1600 //3小时

// 电压粗调校准循环次数
#define BAT_MAX_COUNTS 2

#define MAX_KEYS 50

// Create space for MAX_KEYS public keys
static volatile const char public_key[MAX_KEYS + 1][28] = {
    [0] = "OFFLINEFINDINGPUBLICKEYHERE!",
    [MAX_KEYS] = "ENDOFKEYSENDOFKEYSENDOFKEYS!",
};

int last_filled_index = -1;
int current_index = 0;
// ADC 采样粗调偏差值
static signed short RoughCalib_Value = 0;

// 电池电量标志
static uint8_t status_flag;

// Task ID for internal task/event processing
static uint8_t Broadcaster_TaskID;

// 初始化广播地址
static uint8_t bt_addr[6] = { 0xFF, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

/** 广播载荷 */
static uint8_t advertData[] = {
    0x1e, /* 长度 (30) */
    0xff, /* 厂商特定数据 (类型 0xff) */
    0x4c, 0x00, /* 公司ID (Apple) */
    0x12, 0x19, /* 离线查找类型和长度 */
    0x00, /* 状态 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* 22位密钥 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, /* First two bits */
    0x00, /* Hint (0x00) */
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void Broadcaster_ProcessTMOSMsg(tmos_event_hdr_t* pMsg);
static void Broadcaster_StateNotificationCB(gapRole_States_t newState);
extern bStatus_t GAP_UpdateAdvertisingData(uint8_t taskID, uint8_t adType, uint16_t dataLen, uint8_t* pAdvertData);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesBroadcasterCBs_t Broadcaster_BroadcasterCBs = {
    Broadcaster_StateNotificationCB, // Profile State Change Callbacks
    NULL
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

// 从key中获取地址
__HIGH_CODE
static void set_addr_from_key(const char* key)
{
    /* copy first 6 bytes */
    bt_addr[5] = key[0] | 0b11000000;
    bt_addr[4] = key[1];
    bt_addr[3] = key[2];
    bt_addr[2] = key[3];
    bt_addr[1] = key[4];
    bt_addr[0] = key[5];
}

// 将key填充入广播
__HIGH_CODE
static void fill_adv_template_from_key(const char* key)
{
    memcpy(&advertData[7], &key[6], 22);
    advertData[29] = key[0] >> 6;
}

// 动态修改MAC地址
__HIGH_CODE
static void ble_set_mac_address(uint8_t* addr)
{
    uint8_t initial_advertising_enable = 0;
    // 关闭广播
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initial_advertising_enable);
    // 修改MAC地址
    GAP_ConfigDeviceAddr(ADDRTYPE_PRIVATE_NONRESOLVE, addr);
    initial_advertising_enable = 1;
    // 开启广播
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initial_advertising_enable);
}

// 电池电压采样
__HIGH_CODE
uint16_t sample_battery_voltage()
{
    // VINA 实际电压值 1050±15mV
    const int vref = 1050;
    ADC_InterBATSampInit();
    static uint8_t calib_count = 0;
    calib_count++;
    if (calib_count == 1) {
        RoughCalib_Value = ADC_DataCalib_Rough();
    }
    calib_count %= BAT_MAX_COUNTS;
    ADC_ChannelCfg(CH_INTE_VBAT);
    return (ADC_ExcutSingleConver() + RoughCalib_Value) * vref / 512 - 3 * vref;
}

__HIGH_CODE
void get_battery()
{
    uint16_t battery_level = sample_battery_voltage();
    // 清除电池状态相关的前两位
    status_flag &= (~STATUS_FLAG_BATTERY_MASK);
    if (battery_level > 3000) {
        // 电量大于80%，不设置任何标志
    } else if (battery_level > 2800) {
        status_flag |= STATUS_FLAG_MEDIUM_BATTERY; // 中等电量
    } else if (battery_level > 2500) {
        status_flag |= STATUS_FLAG_LOW_BATTERY; // 低电量
    } else {
        status_flag |= STATUS_FLAG_CRITICALLY_LOW_BATTERY; // 严重低电量
    }
}

__HIGH_CODE
void ble_set_advertisement_key(const char* key)
{
    set_addr_from_key(key);
    fill_adv_template_from_key(key);
    ble_set_mac_address(bt_addr);
}

__HIGH_CODE
void set_and_advertise_next_key()
{
    current_index = (current_index + 1) % (last_filled_index + 1);

    if (current_index < 0 || current_index > last_filled_index) {
        current_index = 0;
    }

    // 更新广播包中的电量数据
    get_battery();
    advertData[6] = status_flag;

    // Set key to be advertised
    ble_set_advertisement_key((const char *)public_key[current_index]);
}

/*********************************************************************
 * @fn      Broadcaster_Init
 *
 * @brief   Initialization function for the Broadcaster App
 *          Task. This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ... ).
 *
 * @param   task_id - the ID assigned by TMOS.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void Broadcaster_Init()
{
    Broadcaster_TaskID = TMOS_ProcessEventRegister(Broadcaster_ProcessEvent);

    // Setup the GAP Broadcaster Role Profile
    {
        // Device starts advertising upon initialization，默认使用所有信道
        uint8_t initial_advertising_enable = TRUE;
        uint8_t initial_adv_event_type = GAP_ADTYPE_ADV_NONCONN_IND; // 不可连接的非定向广播
        uint8_t initial_adv_filter_type = GAP_FILTER_POLICY_ALL; // 允许来自任何设备的扫描和连接请求

        // Set the GAP Role Parameters
        GAPRole_SetParameter(GAPROLE_ADV_FILTER_POLICY, sizeof(uint8_t), &initial_adv_filter_type);
        GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initial_advertising_enable);
        GAPRole_SetParameter(GAPROLE_ADV_EVENT_TYPE, sizeof(uint8_t), &initial_adv_event_type);
        GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(advertData), advertData);
    }

    // Set advertising interval
    {
        // 广播时间
        uint16_t advInt = DEFAULT_ADVERTISING_INTERVAL;
        GAP_SetParamValue(TGAP_DISC_ADV_INT_MIN, advInt);
        GAP_SetParamValue(TGAP_DISC_ADV_INT_MAX, advInt);
    }

    // 开机后初始化蓝牙广播
    tmos_set_event(Broadcaster_TaskID, SBP_START_DEVICE_EVT);

    // 定时滚动密钥读取电压更新广播
    tmos_start_task(Broadcaster_TaskID, SBP_PERIODIC_EVT, 2000); // 开机后2s第一次执行初始化广播数据任务
}

/*********************************************************************
 * @fn      Broadcaster_ProcessEvent
 *
 * @brief   Broadcaster Application Task event processor. This
 *          function is called to process all events for the task. Events
 *          include timers, messages and any other user defined events.
 *
 * @param   task_id  - The TMOS assigned task ID.
 * @param   events - events to process.  This is a bit map and can
 *                   contain more than one event.
 *
 * @return  events not processed
 */
uint16_t Broadcaster_ProcessEvent(uint8_t task_id, uint16_t events)
{
    if (events & SYS_EVENT_MSG) {
        uint8_t* pMsg;

        if ((pMsg = tmos_msg_receive(Broadcaster_TaskID)) != NULL) {
            Broadcaster_ProcessTMOSMsg((tmos_event_hdr_t*)pMsg);

            // Release the TMOS message
            tmos_msg_deallocate(pMsg);
        }

        // return unprocessed events
        return (events ^ SYS_EVENT_MSG);
    }

    if (events & SBP_START_DEVICE_EVT) {
        // Start the Device
        GAPRole_BroadcasterStartDevice(&Broadcaster_BroadcasterCBs);
        // Find the last filled index
        for (int i = MAX_KEYS - 2; i >= 0; i--) {
          if (strlen((const char *)public_key[i]) > 0) {
                last_filled_index = i;
                break;
            }
        }
        set_and_advertise_next_key();
        return (events ^ SBP_START_DEVICE_EVT);
    }

    if (events & SBP_PERIODIC_EVT) {
        tmos_start_task(Broadcaster_TaskID, SBP_PERIODIC_EVT, SBP_PERIODIC_EVT_PERIOD);
        set_and_advertise_next_key();
        GAP_UpdateAdvertisingData(0, TRUE, sizeof(advertData), advertData);
        return (events ^ SBP_PERIODIC_EVT);
    }

    return 0;
}

/*********************************************************************
 * @fn      Broadcaster_ProcessTMOSMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void Broadcaster_ProcessTMOSMsg(tmos_event_hdr_t* pMsg)
{
    switch (pMsg->event) {
    default:
        break;
    }
}

/*********************************************************************
 * @fn      Broadcaster_StateNotificationCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void Broadcaster_StateNotificationCB(gapRole_States_t newState)
{
    switch (newState) {
    case GAPROLE_STARTED:
        PRINT("Initialized..\n");
        break;

    case GAPROLE_ADVERTISING:
        PRINT("Advertising..\n");
        break;

    case GAPROLE_WAITING:
        PRINT("Waiting for advertising..\n");
        break;

    case GAPROLE_ERROR:
        PRINT("Error..\n");
        break;

    default:
        break;
    }
}
