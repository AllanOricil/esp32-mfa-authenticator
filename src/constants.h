// NOTE: network to sync to NTP. Uncomment it to setup the wifi connection

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

// MQTT Broker
#define MQTT_SERVER "192.168.31.198"
#define MQTT_PORT 1883
#define MQTT_USERNAME "MQTT_USERNAME"
#define MQTT_PASSWORD "MQTT_PASSWORD"
#define MQTT_RECONNECT_INTERVAL 5000
#define MQTT_MAX_PAYLOAD_SIZE 1024

// MQTT Topics
// NOTE: this topic is used to create new secrets in the sd card
#define MQTT_WRITE_NEW_SECRET_TOPIC "esp32-totp-write-new-secret"

// PIN
#define PIN_MIN_LENGTH 10
#define PIN_MAX_LENGTH 20
#define PIN_HASH "ccab26629773e35c4f32748e829b382e42950f04a851b72ee6aa52c0bc1141f0"
#define PIN_SALT "=kxCImuICYdLOTPC-Xer!ksJSB5*+xIK"

// UI
#define PIN_SCREEN_NAME "pin"
#define TOTP_SCREEN_NAME "totp"
