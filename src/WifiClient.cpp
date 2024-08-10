#include "WifiClient.h"

#include "Device.h"
#include "Helpers.h"
#include "Log.h"

////////////////////////////////////////////////////////////////////////////////
// CONFIGURATION                                                              //
////////////////////////////////////////////////////////////////////////////////

// Toggle to enable auto-reconnect to Wi-Fi
constexpr bool wifiAutoReconnect = true;

// How many milliseconds to delay after disconnect
constexpr int wifiDisconnectDelay = 1000;

// Wi-Fi hostname prefix
// Must not be longer than 27 characters, as 5 characters
// are reserved for the device identifier and a separator)
constexpr char wifiHostnamePrefix[] = "pathfinder";

// Wi-Fi hostname
char wifiHostname[32];

// Wi-Fi / mDNS instance name
constexpr char wifiInstanceName[] = "Pathfinder";

// Wi-Fi password
char wifiPassword[64] = "workloads";

// Wi-Fi Protected Access Mode
constexpr char wifiProtectedAccessMode[] = "WPA2";

// How many times to attempt to connect to Wi-Fi
constexpr int wifiReconnectAttempts = 20;

// How many milliseconds to delay before attempting reconnection
constexpr int wifiReconnectDelay = 1000;

// How many milliseconds to delay the next Wi-Fi scan
constexpr int wifiRescanDelay = 10000;

// TODO(ksatirli): load from config file
// Wi-Fi SSID (network name)
char wifiSsid[32] = "workloads";

// Toggle for hidden Wi-Fi SSID
constexpr bool wifiSsidHidden = false;

// How many milliseconds to delay between status messages
constexpr int wifiStatusMessageDelay = 500;
////////////////////////////////////////////////////////////////////////////////

char *wifiGenerateHostname() {
    const char *logTag = __func__;

    // Static storage to ensure the string remains valid after the function returns
    static char wifiHostname[32];

    // Get the device identifier
    char *deviceId = deviceIdentifier(4);

    // Assemble the Wi-Fi hostname
    snprintf(wifiHostname, sizeof(wifiHostname), "%s-%s", wifiHostnamePrefix, deviceId);

    logDebug(logTag, "Generated hostname: `%s`", wifiHostname);

    return strdup(wifiHostname);
}

bool wifiDisconnect() {
    const char *logTag = __func__;

    logInfo(logTag, "Disconnecting from SSID `%s`", wifiSsid);

    // First `false` = do not turn Wi-Fi radio off
    // Second `false` = do not erase Wi-Fi AP data
    // See https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html#disconnect
    WiFi.disconnect(false, false);

    // Block flow and allow for cleanup
    delay(wifiDisconnectDelay);

    // Since WiFi.disconnect() does not provide a return value, assume success
    return true;
}

bool wifiConnect() {
    const char *logTag = __func__;

    logDebug(logTag, "Generating Wi-Fi hostname");
    char *wifiHostname = wifiGenerateHostname();

    // Set Wi-Fi hostname
    logDebug(logTag, "Setting Wi-Fi hostname to `%s`", wifiHostname);
    WiFi.setHostname(wifiHostname);

    // Set Wi-Fi auto-reconnect option
    const char *wifiAutoReconnectStr = wifiAutoReconnect ? "true" : "false";
    logDebug(logTag, "Setting Wi-Fi auto-reconnect option to `%s`", wifiAutoReconnectStr);
    WiFi.setAutoReconnect(wifiAutoReconnect);

    // Set Wi-Fi mode
    // TODO(ksatirli): move to variables?
    WiFi.mode(WIFI_STA);

    // Initialize Wi-Fi connection
    WiFi.begin(wifiSsid, wifiPassword);
    logInfo(logTag, "Attempting to connect to SSID `%s`", wifiSsid);

    // Try to establish a connection with a limited number of retries
    int attemptsLeft = wifiReconnectAttempts;

    while (attemptsLeft > 0) {
        // Check the connection status after a delay
        delay(wifiReconnectDelay);
        int attemptCount = wifiReconnectAttempts - attemptsLeft + 1;

        switch (WiFi.status()) {
            case WL_NO_SHIELD:
                logError(logTag, "Device unable to find Radio");
                return false;

            case WL_NO_SSID_AVAIL:
                logInfo(logTag, "Device unable to find SSID `%s` after %d attempts", wifiSsid, attemptCount);
                break;

            case WL_CONNECT_FAILED:
                logWarning(logTag, "Device unable to connect");
                break;

            case WL_CONNECTION_LOST:
                logInfo(logTag, "Device lost connection");
                break;

            case WL_SCAN_COMPLETED:
                logDebug(logTag, "Device completed scan");
                break;

            case WL_DISCONNECTED:
                logInfo(logTag, "Device disconnected");
                break;

            case WL_CONNECTED:
                logInfo(logTag, "Device connected with IP `%s`", WiFi.localIP().toString().c_str());

                // Connection established, start mDNS service
                wifiStartMdnsService(wifiHostname, wifiInstanceName);

                return true;

            default:
                logWarning(logTag, "Unhandled Wi-Fi status `%d` on attempt %d", WiFi.status(), attemptCount);
                break;
        }

        // Decrement attempts after processing status
        attemptsLeft--;

        if (attemptsLeft <= 0) {
            logError(logTag, "Failed to connect to SSID `%s` after %d attempts", wifiSsid, wifiReconnectAttempts);
            wifiDisconnect();

            return false;
        }
    }

    return false;
}

bool wifiConnectionAlive() {
    return WiFi.status() == WL_CONNECTED;
}

void wifiReconnect() {
    const char *logTag = __func__;

    logInfo(logTag, "Disconnecting from Wi-Fi");
    wifiDisconnect();

    logInfo(logTag, "Reconnecting to Wi-Fi SSID `%s`", wifiSsid);
    WiFi.reconnect();

    // Print to serial monitor while connection status is not "connected"
    while (WiFi.status() != WL_CONNECTED) {
        delay(wifiStatusMessageDelay);
        Serial.print(".");
    }

    // Once connected, log the new IP address
    const char *ipAddress = helperIpAddressToString(WiFi.localIP());
    logInfo(logTag, "Connected with IP `%s`", ipAddress);
    // As ipAddress is returned from helperIpAddressToString, no need for manual memory management here
}

void wifiScanNetworks() {
    const char *logTag = __func__;

    // Start Wi-Fi scan
    // See https://www.arduino.cc/reference/en/libraries/wifi/wifi.scannetworks/
    logInfo(logTag, "Starting Wi-Fi scan");
    int networkCount = WiFi.scanNetworks();

    // Check if Wi-Fi scan was successful
    if (networkCount == -1) {
        logError(logTag, "Failed to complete Wi-Fi scan");
    } else {
        logInfo(logTag, "Wi-Fi scan completed, found %d networks", networkCount);

        // Log details of found networks
        for (int i = 0; i < networkCount; i++) {
            // clang-format off
            logInfo(logTag,
                "%d: `%s` (RSSI: %d, Encryption: %s)",
                i + 1,
                WiFi.SSID(i).c_str(),
                WiFi.RSSI(i),
                (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "🔓" : "🔒");
            // clang-format on
        }
    }

    // Wait a bit before scanning again
    delay(wifiRescanDelay);
}

void wifiStartMdnsService(const char *hostname, const char *instance_name) {
    const char *logTag = __func__;

    // Initialize mDNS service
    logDebug(logTag, "Initializing mDNS service");
    esp_err_t err = mdns_init();

    if (err) {
        // TODO(ksatirli): add `err` to log message
        logError(logTag, "Failed to initialize mDNS: %s", esp_err_to_name(err));

        return;
    }

    logDebug(logTag, "Setting mDNS hostname to `%s`", hostname);
    mdns_hostname_set(hostname);

    logDebug(logTag, "Setting mDNS instance name to `%s`", instance_name);
    mdns_instance_name_set(instance_name);
}
