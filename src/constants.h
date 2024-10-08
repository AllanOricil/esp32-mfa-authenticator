#define CONFIG_FILE_PATH "/config.yml"
#define KEYS_FILE_PATH "/keys.txt"

// NOTE: NTP server variables
#define NTP_SERVER_URL "pool.ntp.org"
#define GMT_OFFSET_SEC 0
#define DAYLIGHT_OFFSET_SEC 3600

// NOTE: all totps must have a period of 30 seconds, for now
#define TOTP_PERIOD 30
// NOTE: due to mem limits there has to exist a max number of services we can generate TOTPs for
#define MAX_NUMBER_OF_SERVICES 10
// NOTE: 60 digits + 1 for the null-terminating character
#define MAX_SERVICE_NAME_LENGTH 61
// NOTE: 6 digits + 1 for the null-terminating character
#define MAX_TOTP_LENGTH 7

// MQTT
#define MQTT_RECONNECT_INTERVAL 60000
#define MQTT_MAX_PAYLOAD_SIZE 1024
#define MQTT_WRITE_NEW_SECRET_TOPIC "esp32-totp-write-new-secret"

// PIN
#define PIN_MIN_LENGTH 6
#define PIN_MAX_LENGTH 20

// SECURITY
#define MAX_NUMBER_OF_WRONG_UNLOCK_ATTEMPTS 3

// UI
#define PIN_SCREEN_NAME "pin"
#define TOTP_SCREEN_NAME "totp"

// TOUCH
#define TOUCH_DOUBLE_TOUCH_INTERVAL 300

// DISPLAY
#define LVGL_TICK_PERIOD_MS 1
#define LVGL_BUFFER_PIXELS (DISPLAY_WIDTH * DISPLAY_HEIGHT / 4)
#define LVGL_BUFFER_MALLOC_FLAGS (MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT)
#define PWM_CHANNEL_BCKL (SOC_LEDC_CHANNEL_NUM - 1)
#define PWM_FREQ_BCKL 400
#define PWM_BITS_BCKL 8
#define PWM_MAX_BCKL ((1 << PWM_BITS_BCKL) - 1)
#define SLEEP_TIMEOUT 10
