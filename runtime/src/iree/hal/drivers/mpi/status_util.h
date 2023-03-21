// Copyright 2021 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef IREE_HAL_DRIVERS_MPI_STATUS_UTIL_H_
#define IREE_HAL_DRIVERS_MPI_STATUS_UTIL_H_

#include <stdint.h>
#include <mpi.h>

#include "iree/base/api.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

// Converts an MPI_Status to an iree_status_t.
//
// Usage:
//   iree_status_t status = MPI_RESULT_TO_STATUS(MPI_function(...));
#define MPI_RESULT_TO_STATUS(expr, ...) \
    iree_hal_mpi_result_to_status((expr), __FILE__, __LINE__)

// IREE_RETURN_IF_ERROR but implicitly converts the MPI_Status return value to
// a Status.
//
// Usage:
//   MPI_RETURN_IF_ERROR(MPI_Function(...), "message");
#define MPI_RETURN_IF_ERROR(expr, ...)                           \
  IREE_RETURN_IF_ERROR(iree_hal_cuda_result_to_status((expr),    \
                                                      __FILE__,  \
                                                      __LINE__), \
                       __VA_ARGS__)

// IREE_IGNORE_ERROR but implicitly converts the MPI_Status return value to a
// Status.
//
// Usage:
//   MPI_IGNORE_ERROR(MPI_Function(...));
#define MPI_IGNORE_ERROR(expr)                                   \
  IREE_IGNORE_ERROR(iree_hal_cuda_result_to_status((expr),       \
                                                   __FILE__,     \
                                                   __LINE__))

// Converts an MPI_Status to a Status object.
iree_status_t iree_hal_mpi_result_to_status(
    int result,
    const char* file,
    uint32_t line);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // IREE_HAL_DRIVERS_MPI_STATUS_UTIL_H_
