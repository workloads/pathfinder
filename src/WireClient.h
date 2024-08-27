#ifndef WIRECLIENT_H_
#define WIRECLIENT_H_

#include <Wire.h>

/**
 * @brief WireClient class for managing I2C communication initialization.
 */
class WireClient {
 private:
    // Configuration
    constexpr static int S_SCL = 33;
    constexpr static int S_SDA = 32;

 public:
    /**
     * @brief Initializes the Wire (I2C) communication.
     *
     * @return True if initialization was successful, false otherwise.
     */
    static bool init();
};

#endif  // WIRECLIENT_H_
