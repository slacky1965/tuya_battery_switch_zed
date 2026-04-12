#ifndef SRC_INCLUDE_APP_LEVEL_H_
#define SRC_INCLUDE_APP_LEVEL_H_

#define LEVEL_STEP_SIZE         25
#define LEVEL_TRANSITION_TIME   25

void app_move_to_level(uint8_t ep, uint8_t up_down);
void app_move_level(uint8_t ep, uint8_t up_down);
void app_stop_level(uint8_t ep);
void app_step_level(uint8_t ep, uint8_t up_down);

#endif /* SRC_INCLUDE_APP_LEVEL_H_ */
