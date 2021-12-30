/**
 * @file   ln_ty_ate.h
 * @author LightningSemi WLAN Team
 * Copyright (C) 2021-2021 LightningSemi Technology Co., Ltd. All rights reserved.
 * 
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-16     MurphyZhao   the first version
 */

#ifndef __LN_TY_ATE_H__
#define __LN_TY_ATE_H__

#include <stdbool.h>

/**
 * @brief ate_mode_check_pfn
 *     Check if device can enter ATE mode.
 * @return true:  device can enter ATE mode
 *         false: device can't enter ATE mode
*/
typedef bool (* ate_mode_check_pfn)(void);

/**
 * @brief ln_ty_ate_mode_check
 *        Check if the device can enter ATE mode.
 *        Returns "false" by default. User needs to
 *        reimplement this callback function.
 * @return true:  device can enter ATE mode
 *         false: device can't enter ATE mode
*/
bool ln_ty_ate_mode_check(void);

/**
 * @brief ln_ty_ate_main
 *        Try to enter ATE mode
 * @return  0: Successfully entered ATE mode
 *         -1: Failed to enter ATE mode
*/
int ln_ty_ate_main(ate_mode_check_pfn cb);

#endif /* __LN_TY_ATE_H__ */
