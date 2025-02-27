/********************************** (C) COPYRIGHT *******************************
 * File Name          : broadcaster.h
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2018/12/11
 * Description        :
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#ifndef BROADCASTER_H
#define BROADCASTER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */


 #define STATUS_FLAG_BATTERY_MASK           0b11000000  // 前两位表示电池状态
 #define STATUS_FLAG_MEDIUM_BATTERY         0b01000000  // 中等电量
 #define STATUS_FLAG_LOW_BATTERY            0b10000000  // 低电量
 #define STATUS_FLAG_CRITICALLY_LOW_BATTERY 0b11000000  // 严重低电量

// Simple BLE Broadcaster Task Events
#define SBP_START_DEVICE_EVT         0x0001
#define SBP_PERIODIC_EVT             0x0002
#define SBP_ADV_IN_CONNECTION_EVT    0x0004

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task Initialization for the BLE Broadcaster Application
 */
extern void Broadcaster_Init(void);

/*
 * Task Event Processor for the BLE Broadcaster Application
 */
extern uint16_t Broadcaster_ProcessEvent(uint8_t task_id, uint16_t events);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif
