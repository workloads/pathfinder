#include "WifiClient.h"

constexpr char WifiClient::hostnamePrefix[32];
constexpr char WifiClient::instanceName[32];

char* WifiClient::generateHostname() {
    const char* logTag = __func__;

    // Static storage to ensure the string remains valid after the function returns
    static char wifiHostname[32];

    // Get the device identifier
    char* deviceId = Device::identifier(4);

    // Assemble the Wi-Fi hostname
    snprintf(wifiHostname, sizeof(wifiHostname), "%s-%s", WifiClient::hostnamePrefix, deviceId);

    Logger::debug(logTag, "Generated hostname: `%s`", wifiHostname);

    return strdup(wifiHostname);
}

bool WifiClient::disconnect() {
    const char* logTag = __func__;

    Logger::info(logTag, "Disconnecting from SSID `%s`", wifiSsid);

    // First `false` = do not turn Wi-Fi radio off
    // Second `false` = do not erase Wi-Fi AP data
    WiFi.disconnect(false, false);

    // Block flow and allow for cleanup
    delay(disconnectDelay);

    // Since WiFi.disconnect() does not provide a return value, assume success
    return true;
}

bool WifiClient::connect() {
    const char* logTag = __func__;

    Logger::debug(logTag, "Generating Wi-Fi hostname");
    char* wifiHostname = generateHostname();

    // Set Wi-Fi hostname
    Logger::debug(logTag, "Setting Wi-Fi hostname to `%s`", wifiHostname);
    WiFi.setHostname(wifiHostname);

    // Set Wi-Fi auto-reconnect option
    const char* wifiAutoReconnectStr = WifiClient::autoReconnect ? "true" : "false";
    Logger::debug(logTag, "Setting Wi-Fi auto-reconnect option to `%s`", wifiAutoReconnectStr);
    WiFi.setAutoReconnect(WifiClient::autoReconnect);

    // Set Wi-Fi mode
    WiFi.mode(WIFI_STA);

    // Initialize Wi-Fi connection
    WiFi.begin(WifiClient::wifiSsid, WifiClient::wifiPassword);
    Logger::info(logTag, "Attempting to connect to SSID `%s`", wifiSsid);

    // Try to establish a connection with a limited number of retries
    int attemptsLeft = WifiClient::reconnectAttempts;

    while (attemptsLeft > 0) {
        // Check the connection status after a delay
        delay(reconnectDelay);
        int attemptCount = reconnectAttempts - attemptsLeft + 1;

        switch (WiFi.status()) {
            case WL_NO_SHIELD:
                Logger::error(logTag, "Device unable to find Wi-Fi Radio");
                return false;

            case WL_NO_SSID_AVAIL:
                Logger::info(logTag, "Device unable to find SSID `%s` after %d attempts", wifiSsid, attemptCount);
                break;

            case WL_CONNECT_FAILED:
                Logger::warning(logTag, "Device unable to connect");
                break;

            case WL_CONNECTION_LOST:
                Logger::info(logTag, "Device lost connection");
                break;

            case WL_SCAN_COMPLETED:
                Logger::debug(logTag, "Device completed scan");
                break;

            case WL_DISCONNECTED:
                Logger::info(logTag, "Device disconnected");
                break;

            case WL_CONNECTED:
                Logger::info(logTag, "Device connected with IP `%s`", WiFi.localIP().toString().c_str());

                // Connection established, start mDNS service
                startMdnsService(wifiHostname, WifiClient::instanceName);

                return true;

            default:
                Logger::warning(logTag, "Unhandled Wi-Fi status `%d` on attempt %d", WiFi.status(), attemptCount);
                break;
        }

        // Decrement attempts after processing status
        attemptsLeft--;

        if (attemptsLeft <= 0) {
            Logger::error(logTag, "Failed to connect to SSID `%s` after %d attempts", wifiSsid, reconnectAttempts);
            disconnect();

            return false;
        }
    }

    return false;
}

bool WifiClient::connectionAlive() {
    return WiFi.status() == WL_CONNECTED;
}

void WifiClient::reconnect() {
    const char* logTag = __func__;

    Logger::info(logTag, "Disconnecting from Wi-Fi");
    disconnect();

    Logger::info(logTag, "Reconnecting to Wi-Fi SSID `%s`", wifiSsid);
    WiFi.reconnect();

    // Print to serial monitor while connection status is not "connected"
    while (WiFi.status() != WL_CONNECTED) {
        delay(statusMessageDelay);
        Serial.print(".");
    }

    // Once connected, log the new IP address
    const char* ipAddress = helperIpAddressToString(WiFi.localIP());
    Logger::info(logTag, "Connected with IP `%s`", ipAddress);
}

void WifiClient::scanNetworks() {
    const char* logTag = __func__;

    // Start Wi-Fi scan
    Logger::info(logTag, "Starting Wi-Fi scan");
    int networkCount = WiFi.scanNetworks();

    // Check if Wi-Fi scan was successful
    if (networkCount == -1) {
        Logger::error(logTag, "Failed to complete Wi-Fi scan");
    } else {
        Logger::info(logTag, "Wi-Fi scan completed, found %d networks", networkCount);

        // Log details of found networks
        for (int i = 0; i < networkCount; i++) {
            Logger::info(logTag, "%d: `%s` (RSSI: %d, Encryption: %s)", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i), (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "🔓" : "🔒");
        }
    }

    // Wait a bit before scanning again
    delay(rescanDelay);
}

void WifiClient::startMdnsService(const char* hostname, const char* instance_name) {
    const char* logTag = __func__;

    // Initialize mDNS Service
    Logger::debug(logTag, "Initializing mDNS service");
    esp_err_t err = mdns_init();

    if (err) {
        Logger::error(logTag, "Failed to initialize mDNS: %s", esp_err_to_name(err));
        return;
    }

    Logger::debug(logTag, "Setting mDNS hostname to `%s`", hostname);
    mdns_hostname_set(hostname);

    Logger::debug(logTag, "Setting mDNS instance name to `%s`", instance_name);
    mdns_instance_name_set(instance_name);
}
