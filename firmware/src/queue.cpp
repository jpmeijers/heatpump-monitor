#include "queue.hpp"

QueueHandle_t queue;

void setupQueue() {
    // Set up queue
    queue = xQueueCreate( 10, sizeof( struct message ) );
    if(queue == NULL){
        Serial.println("Error creating the queue");
    }
}