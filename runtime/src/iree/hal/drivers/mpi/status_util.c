// Copyright 2021 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "iree/hal/drivers/mpi/status_util.h"

#include <stddef.h>

iree_status_t iree_hal_mpi_result_to_status(
    int result,
    const char* file,
    uint32_t line) {

  if (IREE_LIKELY(result == MPI_SUCCESS)) {
    return iree_ok_status();
  }

  char error_string[MPI_MAX_ERROR_STRING];
  int error_len = 0;
  if (MPI_Error_string(result, error_string, &error_len) != MPI_SUCCESS) {
    strncpy(error_string, "UNKNOWN", 8);
  }

  int error_class = 0;
  if (MPI_Error_class(result, &error_class) != MPI_SUCCESS) {
    error_class = MPI_ERR_UNKNOWN;
  }

  return iree_make_status_with_location(
    file,
    line,
    IREE_STATUS_INTERNAL,
    "MPI driver error '%d' (%d): %s", result, error_class, error_string);
}
