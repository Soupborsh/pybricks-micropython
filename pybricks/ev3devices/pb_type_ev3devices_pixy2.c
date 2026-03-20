
// SPDX-License-Identifier: MIT
// Copyright (c) 2018-2023 The Pybricks Authors

#include "py/mpconfig.h"

#if PYBRICKS_PY_EV3DEVICES

#include <pbio/port_interface.h>
#include <pbio/port_dcm.h>

#include <pybricks/common.h>
#include <pybricks/ev3devices.h>
#include <pybricks/parameters.h>

#include <pybricks/util_mp/pb_kwarg_helper.h>
#include <pybricks/util_mp/pb_obj_helper.h>
#include <pybricks/util_pb/pb_error.h>

#include <pbdrv/i2c.h>

// pybricks.ev3devices.Pixy2 class object
typedef struct _ev3devices_Pixy2_obj_t {
    mp_obj_base_t base;
    pbio_port_t *port;
    pbdrv_i2c_dev_t *i2c_dev;
} ev3devices_Pixy2_obj_t;

// pybricks.ev3devices.Pixy2.get_blocks
static mp_obj_t ev3devices_Pixy2_get_blocks(mp_obj_t self_in) {
    ev3devices_Pixy2_obj_t *self = MP_OBJ_TO_PTR(self_in);

    const uint8_t request[] = { 0xAF, 0xC1, 32, 2, 1, 1 };
    uint8_t *data = NULL;
    pbio_os_state_t state = {0};
    pb_assert(pbdrv_i2c_write_then_read(&state, self->i2c_dev, 0x54, request, sizeof(request), &data, 20, false));
    if (data == NULL) {
        return mp_const_none;
    }

    // Create a bytearray of 14 bytes (excluding the 6-byte header)
    mp_obj_t raw_data = mp_obj_new_bytearray(14, &data[6]);
    return raw_data;
    // return mp_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(ev3devices_Pixy2_get_blocks_obj, ev3devices_Pixy2_get_blocks);

// pybricks.ev3devices.Pixy2.__init__
static mp_obj_t ev3devices_Pixy2_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    PB_PARSE_ARGS_CLASS(n_args, n_kw, args,
                        PB_ARG_REQUIRED(port));

    ev3devices_Pixy2_obj_t *self = mp_obj_malloc(ev3devices_Pixy2_obj_t, type);
    pbio_port_id_t port_id = pb_type_enum_get_value(port_in, &pb_enum_type_Port);
    pb_assert(pbio_port_get_port(port_id, &self->port));
    pb_assert(pbio_port_get_i2c_dev(self->port, &self->i2c_dev));

    return MP_OBJ_FROM_PTR(self);
}

// dir(pybricks.ev3devices.Pixy2)
static const mp_rom_map_elem_t ev3devices_Pixy2_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_get_blocks), MP_ROM_PTR(&ev3devices_Pixy2_get_blocks_obj) },
};
static MP_DEFINE_CONST_DICT(ev3devices_Pixy2_locals_dict, ev3devices_Pixy2_locals_dict_table);

// type(pybricks.ev3devices.Pixy2)
MP_DEFINE_CONST_OBJ_TYPE(pb_type_ev3devices_Pixy2,
                         MP_QSTR_Pixy2,
                         MP_TYPE_FLAG_NONE,
                         make_new, ev3devices_Pixy2_make_new,
                         locals_dict, &ev3devices_Pixy2_locals_dict);

#endif // PYBRICKS_PY_EV3DEVICES
