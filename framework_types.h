typedef void (*mqtt_callback_t)(String payload); //callback function pointer

typedef struct {
  const char* topic;
  mqtt_callback_t callback;
} mqtt_topic_handler_t;