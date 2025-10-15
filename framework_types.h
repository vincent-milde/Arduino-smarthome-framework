#define MAX_TOPICS 10

typedef void (*mqtt_callback_t)(String payload);
typedef struct {
  const char* topic;
  mqtt_callback_t callback;
} mqtt_topic_handler_t;