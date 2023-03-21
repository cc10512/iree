// Copyright 2022 The IREE Authors
//
// Licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include "iree/hal/drivers/mpi/registration/driver_module.h"

#include <inttypes.h>
#include <stddef.h>

#include "iree/base/api.h"
#include "iree/hal/drivers/mpi/mpi_driver.h"
#include "iree/hal/local/loaders/registration/init.h"

static iree_status_t iree_hal_mpi_driver_factory_enumerate(
    void* self,
    iree_host_size_t* out_driver_info_count,
    const iree_hal_driver_info_t** out_driver_infos) {
  static const iree_hal_driver_info_t driver_infos[1] = {
      {
          .driver_name = IREE_SVL("mpi"),
          .full_name = IREE_SVL("Distrbuted execution using MPI"),
      },
  };
  *out_driver_info_count = IREE_ARRAYSIZE(driver_infos);
  *out_driver_infos = driver_infos;
  return iree_ok_status();
}

static iree_status_t iree_hal_mpi_driver_factory_try_create(
    void* self,
    iree_string_view_t driver_name,
    iree_allocator_t host_allocator,
    iree_hal_driver_t** out_driver) {
  if (!iree_string_view_equal(driver_name, IREE_SV("mpi"))) {
    return iree_make_status(IREE_STATUS_UNAVAILABLE,
                            "no driver '%.*s' is provided by this factory",
                            (int)driver_name.size, driver_name.data);
  }

  iree_hal_mpi_device_params_t default_params;
  iree_hal_mpi_device_params_initialize(&default_params);

  // Create executors for each topology specified by flags.
  // Stack allocated storage today but we can query for the total count and
  // grow if needed in the future (16 NUMA nodes is enough for anyone, right?).
  iree_task_executor_t* executor_storage[16] = {NULL};
  iree_task_executor_t** executors = executor_storage;
  iree_host_size_t executor_count = 0;
  IREE_RETURN_IF_ERROR(iree_task_executors_create_from_flags(
      host_allocator, IREE_ARRAYSIZE(executor_storage), executors,
      &executor_count));

  // Create a loader for elf images
  iree_hal_executable_loader_t* loaders = NULL;
  iree_status_t status = iree_hal_create_executable_loader_by_name(
    IREE_SV("embedded-elf"),
    iree_hal_executable_import_provider_default(),
    host_allocator,
    loader);


  // Create a task driver that will use the given executors for scheduling work
  // and loaders for loading executables.
  if (iree_status_is_ok(status)) {
    status = iree_hal_mpi_driver_create(
        driver_name, &default_params, executor_count, executors, loader_count,
        loaders, device_allocator, host_allocator, out_driver);
  }

  return status;
}

IREE_API_EXPORT iree_status_t iree_hal_mpi_driver_module_register(
    iree_hal_driver_registry_t* registry) {
  static const iree_hal_driver_factory_t factory = {
      .self = NULL,
      .enumerate = iree_hal_mpi_driver_factory_enumerate,
      .try_create = iree_hal_mpi_driver_factory_try_create,
  };
  return iree_hal_driver_registry_register_factory(registry, &factory);
}
