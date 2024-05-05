// NOTE: used when reading data from the SD card
#define TF_CS 5

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

#define KEYS_FILE_PATH "/keys.txt"

// MQTT
#define MQTT_RECONNECT_INTERVAL 5000
#define MQTT_MAX_PAYLOAD_SIZE 1024
#define MQTT_WRITE_NEW_SECRET_TOPIC "esp32-totp-write-new-secret"

// PIN
#define PIN_MIN_LENGTH 6
#define PIN_MAX_LENGTH 20

// UI
#define PIN_SCREEN_NAME "pin"
#define TOTP_SCREEN_NAME "totp"
