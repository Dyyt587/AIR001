#ifndef INTER_FACE_IIC_H
#define INTER_FACE_IIC_H

typedef struct
{
    char mode; // 0待机，1速度，2位置,
    float speed;
    float position;

} Control_t;
extern Control_t control;

void interface_init(void);

#endif
