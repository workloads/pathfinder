#ifndef WIFICLIENT_H_
#define WIFICLIENT_H_

#include <ESPmDNS.h>
#include <WiFi.h>

#include "Device.h"
#include "Helpers.h"
#include <Logger.h>

/**
 * @brief WifiClient class for managing Wi-Fi connections and related services.
 */
class WifiClient {
 private:
    // Configuration
    constexpr static bool autoReconnect      = true;
    constexpr static int  disconnectDelay    = 1000;
    constexpr static char hostnamePrefix[32] = "pathfinder";
    constexpr static int  reconnectAttempts  = 20;
    constexpr static int  reconnectDelay     = 1000;
    constexpr static int  rescanDelay        = 10000;
    constexpr static bool ssidHidden         = false;
    constexpr static int  statusMessageDelay = 500;

    // Wi-Fi credentials and details
    char wifiHostname[32];
    char wifiSsid[32]                = "workloads";
    char wifiPassword[64]            = "workloads";
    char wifiProtectedAccessMode[32] = "WPA2";

 public:
    constexpr static char instanceName[32] = "Pathfinder";  // No WifiClient:: prefix

    /**
     * @brief Generate Wi-Fi hostname.
     *
     * @return nullptr if the hostname was generated successfully
     */
    char* generateHostname();

    /**
     * @brief Disconnect from a Wi-Fi network.
     *
     * @return True if the disconnection was successful, false otherwise.
     */
    bool disconnect();

    /**
     * @brief Establish connection with a Wi-Fi network.
     *
     * @return True if the connection was successful, false otherwise.
     * @note This function has the device acting as a client (= station).
     */
    bool connect();

    /**
     * @brief Check if the Wi-Fi connection is alive.
     *
     * @return True if the connection is alive, false otherwise.
     */
    bool connectionAlive();

    /**
     * @brief Reestablish connection with a Wi-Fi network.
     *
     * @note This function has the device acting as a client (= station).
     */
    void reconnect();

    /**
     * @brief Scan for Wi-Fi networks.
     */
    void scanNetworks();

    /**
     * @brief Start mDNS service.
     *
     * @param hostname The hostname to use.
     * @param instance_name The instance name to use.
     */
    void startMdnsService(const char* hostname, const char* instance_name);
};

#endif  // WIFICLIENT_H_
