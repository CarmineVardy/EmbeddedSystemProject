/*
 * Project: Wearable System for Health Monitoring
 * Authors: Carmine Vardaro, Alessia Parente, Antonio D'Arienzo
 *
 * Licensed under the GNU General Public License v3.0 (GPL v3).
 * For more details, see: https://www.gnu.org/licenses/gpl-3.0.html
 *
 * Description:
 * This file defines the function prototypes for handling alarm checks related to health metrics like temperature and ECG.
 * Alarms are triggered based on the monitored values and the user's activity state.
 */

#ifndef INC_ALARMS_H_
#define INC_ALARMS_H_

#include "activity_tracking.h"  // Include the activity tracking module for user state information

/*
 * Checks the temperature and triggers an alarm if necessary.
 * It evaluates the current temperature against the threshold and considers the user's state.
 */
void checkTemperature_Alarm(float temp, UserState state);

/*
 * Checks the ECG data (heart rate and heart rate variability) and triggers an alarm if necessary.
 * The function compares the heart rate and HRV values with predefined limits based on the user's state.
 */
void checkECG_Alarm(uint8_t HR, float HRV, UserState state);

#endif /* INC_ALARMS_H_ */
