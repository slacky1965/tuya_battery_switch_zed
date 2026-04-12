#ifndef SRC_INCLUDE_APP_BUTTON_H_
#define SRC_INCLUDE_APP_BUTTON_H_

#ifndef DEBOUNCE_BUTTON
#define DEBOUNCE_BUTTON     16      /* number of polls for debounce                 */
#endif

typedef enum {
    ACTION_HOLD = 0,
    ACTION_SINGLE,
    ACTION_DOUBLE,
    ACTION_TRIPLE,
    ACTION_QUADRUPLE,
    ACTION_CLEAR = 250,
    ACTION_RELEASE = 255,
    ACTION_EMPTY = 300,
} switch_action_t;

void button_handler();
bool button_idle();
void button_init();

#endif /* SRC_INCLUDE_APP_BUTTON_H_ */
