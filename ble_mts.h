#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"

#define BLE_MTS_OBSERVER_PRIO                   2

#define BLE_MTS_DEF(_name)                      \
static ble_mts_t _name;                         \
NRF_SDH_BLE_OBSERVER(_name ## _obs,             \
                     BLE_MTS_OBSERVER_PRIO,     \
                     ble_mts_on_ble_evt, &_name)


// UUID: 441619d0-c9b2-11e8-a8d5-f2801f1b9fd1
#define MTS_UUID_BASE {0xBC, 0x8A, 0xBF, 0x45, 0xCA, 0x05, 0x50, 0xBA, \
                                 0x40, 0x42, 0xB0, 0x00, 0xC9, 0xAD, 0x64, 0xF3}

#define MTS_UUID_SERVICE        0x1400
#define MTS_UUID_MOTOR_CHAR     0x1401


// Forward declaration of the ble_mts_t type
typedef struct ble_mts_s ble_mts_t;

typedef void (*ble_mts_motor_write_handler_t) (uint16_t conn_handle, ble_mts_t * p_mts, uint8_t motor_state);

/** @brief Motor Service init structure. This structure containes all options and data needed for
 *        initialization of the service. */
typedef struct {
    ble_mts_motor_write_handler_t motor_write_handler; /**< Event handler to be called when the Motor Characteristic is written. */
} ble_mts_init_t;

/**@brief Motor Service structure. This contains varius status information for the service. */
struct ble_mts_s {
    uint16_t                       service_handle;       /**< Handle of Motor Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t      motor_char_handles;   /**< Handles related to the Motor Characteristic. */
    uint8_t                       uuid_type;            /**< UUID type for the Motor Service. */
    ble_mts_motor_write_handler_t motor_write_handler;  /**< Event handler to be called when the Motor Characteristic is written. */
};

/**@brief Function for initializing the Motor Service.
 *
 *@param[out] p_mts       Motor Service structure. This structure must be supplied by
 *                        the application. It is initialized by this function and will later
 *                        be used to indentify this particular service instance.
 *
 *@param[out] p_mts_init  Information needed to initialize the service.
 *
 *@retval NRF_SUCCESS If the service was initialized succesfully. Otherwise, an error code is returned.
 */
uint32_t ble_mts_init(ble_mts_t * p_mts, const ble_mts_init_t * p_mts_init);

/**@brief Function for handling the application's BLE stack events.
 *
 * @details This function handles all events from the BLE stack that are of interest to the Motor Service.
 *
 * @param[in] p_ble_evt  Event received from the BLE stack.
 * @param[in] p_context  Motor Service structure.
 */
void ble_mts_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);