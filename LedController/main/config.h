#define DEBUGGING true
#define BAUDRATE  115200


#define NUM_TLC59711 1
#define data   11
#define clock  13

const float MAX_BRIGHTNESS  = 65536;
const float MIN_BRIGHTNESS  = 0;
const float INCREMENT_VALUE = 655.35;
const float DECREMENT_VALUE = 655.35;

const uint8_t CHANNELS_MODE1[] = {0,1,2,3};
const uint8_t CHANNELS_MODE2[] = {4,5,6,7};
const uint8_t CHANNELS_MODE3[] = {8,9,10,11};


const uint8_t SIZE_MODE1 = sizeof(CHANNELS_MODE1) / sizeof(CHANNELS_MODE1[0]);
const uint8_t SIZE_MODE2 = sizeof(CHANNELS_MODE2) / sizeof(CHANNELS_MODE2[0]);
const uint8_t SIZE_MODE3 = sizeof(CHANNELS_MODE3) / sizeof(CHANNELS_MODE3[0]);


// increase this value to reduce fade speed
typedef struct {
  const uint16_t Mode1 = 200;
  const uint16_t Mode2 = 200;
  const uint16_t Mode3 = 200;
}update_delay;

typedef struct {
  uint32_t Mode1 = 0;
  uint32_t Mode2 = 0;
  uint32_t Mode3 = 0;
}last_update;

bool doIncreaseMode1 = false;
bool doIncreaseMode2 = false;

typedef struct {
  uint16_t Mode1 = 0;
  uint16_t Mode2 = 0;
  uint16_t Mode3 = 0;
}brightness;


typedef enum {
  NONE,
  MODE1,
  MODE2,
  MODE3,
}modes;


typedef struct {
  const uint8_t BTN_1     = 16;
  const uint8_t BTN_2     = 17;
  const uint8_t BTN_3     = 18;
  const uint16_t DEBOUNCE = 500;
}btn;
