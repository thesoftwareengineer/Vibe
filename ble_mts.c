#include "sdk_common.h"
#include "ble_mts.h"
#include "ble_srv_common.h"

/**@brief Function for handling the Write event.
 *
 *
 * @param[in] p_mts        Motor Service structure.
 * @param[in] p_ble_evt    Event received from BLE stack.
 */
static void on_write(ble_mts_t * p_mts, ble_evt_t const * p_ble_evt) {
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if((p_evt_write->handle == p_mts->motor_char_handles.value_handle)
      && (p_evt_write->len == 1)
      && (p_mts->motor_write_handler != NULL)) {
       
       p_mts->motor_write_handler(p_ble_evt->evt.gap_evt.conn_handle, p_mts, p_evt_write->data[0]);
    }
}

void ble_mts_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context) {
    ble_mts_t * p_mts = (ble_mts_t *)p_context;

    switch(p_ble_evt->header.evt_id) {
        case BLE_GATTS_EVT_WRITE:
            on_write(p_mts, p_ble_evt);
            break;
        default:
            // No implementation needed.
            break;
    }
}

uint32_t ble_mts_init(ble_mts_t * p_mts, const ble_mts_init_t * p_mts_init) {
    uint32_t              err_code;
    ble_uuid_t            ble_uuid;
    ble_add_char_params_t add_char_params;

    // Initialize service structure.
    p_mts->motor_write_handler = p_mts_init->motor_write_handler;

    // Add service.
    ble_uuid128_t base_uuid = {MTS_UUID_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_mts->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_mts->uuid_type;
    ble_uuid.uuid = MTS_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_mts->service_handle);
    VERIFY_SUCCESS(err_code);

    // Add Motor characteristic
    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid              = MTS_UUID_MOTOR_CHAR;
    add_char_params.uuid_type         = p_mts->uuid_type;
    add_char_params.init_len          = sizeof(uint8_t);
    add_char_params.max_len           = sizeof(uint8_t);
    add_char_params.char_props.read   = 1;
    add_char_params.char_props.write  = 1;

    add_char_params.read_access  = SEC_OPEN;
    add_char_params.write_access = SEC_OPEN;

    return characteristic_add(p_mts->service_handle, &add_char_params, &p_mts->motor_char_handles);
}