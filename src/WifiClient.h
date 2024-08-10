#ifndef WIFICLIENT_H_
#define WIFICLIENT_H_

// Built-in and Third-Party libraries
#include <ESPmDNS.h>
#include <WiFi.h>

/**
 * @brief Generate Wi-Fi hostname.
 *
 * @return nil if the hostname was generated successfully
 */
char *wifiGenerateHostname();

/**
 * @brief Disconnect from a Wi-Fi network.
 *
 * @return True if the disconnection was successful, false otherwise.
 */
bool wifiDisconnect();

/**
 * @brief Establish connection with a Wi-Fi network.
 *
 * @return True if the connection was successful, false otherwise.
 * @note This function has the device acting as a client (= station).
 */
bool wifiConnect();

/**
 * @brief Check if the Wi-Fi connection is alive.
 *
 * @return True if the connection is alive, false otherwise.
 */
bool wifiConnectionAlive();

/**
 * @brief Reestablish connection with a Wi-Fi network.
 *
 * @note This function has the device acting as a client (= station).
 */
void wifiReconnect();

/**
 * @brief Scan for Wi-Fi networks.
 */
void wifiScanNetworks();

/**
 * @brief Start mDNS service.
 *
 * @param hostname The hostname to use.
 * @param instance_name The instance name to use.
 */
void wifiStartMdnsService(const char *hostname, const char *instance_name);

#endif  // WIFICLIENT_H_
