// Copyright 2021 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef IREE_HAL_DRIVERS_MPI_MPI_DEVICE_H_
#define IREE_HAL_DRIVERS_MPI_MPI_DEVICE_H_

#include "iree/base/api.h"
#include "iree/hal/api.h"
// #include "iree/hal/drivers/mpi/api.h"
// #include "iree/hal/drivers/mpi/dynamic_symbols.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

// Creates a device that owns and manages an MPI context.
iree_status_t iree_hal_mpi_device_create(
    // iree_hal_driver_t* driver,
    iree_string_view_t identifier,
    const iree_hal_mpi_device_params_t* params,
    // iree_hal_mpi_dynamic_symbols_t* syms,
    // iree_allocator_t host_allocator,
    iree_hal_device_t** out_device);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // IREE_HAL_DRIVERS_MPI_MPI_DEVICE_H_
