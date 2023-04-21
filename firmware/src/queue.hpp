#include <Arduino.h>

#ifndef MY_QUEUE_H
#define MY_QUEUE_H

struct message {
  unsigned long time;
  char topic[256];
  char raw[256];
};

extern QueueHandle_t queue;

void setupQueue();

#endif