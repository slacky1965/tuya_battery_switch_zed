#ifndef SRC_INCLUDE_APP_SCENE_H_
#define SRC_INCLUDE_APP_SCENE_H_

typedef struct __attribute__((packed)) {
    uint8_t  sceneId;
    uint16_t groupId;
} app_scene_t;

void app_scene_send(uint8_t ep);

#endif /* SRC_INCLUDE_APP_SCENE_H_ */
