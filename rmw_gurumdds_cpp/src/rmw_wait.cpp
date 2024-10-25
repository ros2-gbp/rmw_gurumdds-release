// Copyright 2019 GurumNetworks, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "rmw/allocators.h"
#include "rmw/error_handling.h"
#include "rmw/impl/cpp/macros.hpp"
#include "rmw/rmw.h"

#include "rmw_gurumdds_cpp/dds_include.hpp"
#include "rmw_gurumdds_cpp/identifier.hpp"
#include "rmw_gurumdds_cpp/event_info_common.hpp"
#include "rmw_gurumdds_cpp/wait.hpp"

extern "C"
{
rmw_wait_set_t *
rmw_create_wait_set(rmw_context_t * context, size_t max_conditions)
{
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(context, nullptr);
  RMW_CHECK_FOR_NULL_WITH_MSG(
    context->impl,
    "expected initialized context",
    return nullptr);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    context,
    context->implementation_identifier,
    RMW_GURUMDDS_ID,
    return nullptr);

  RCUTILS_UNUSED(max_conditions);
  rmw_wait_set_t * wait_set = rmw_wait_set_allocate();

  rmw_gurumdds_cpp::WaitSetInfo * wait_set_info = nullptr;

  if (!wait_set) {
    RMW_SET_ERROR_MSG("failed to allocate wait set");
    goto fail;
  }

  wait_set->implementation_identifier = RMW_GURUMDDS_ID;
  wait_set->data = rmw_allocate(sizeof(rmw_gurumdds_cpp::WaitSetInfo));
  wait_set_info = static_cast<rmw_gurumdds_cpp::WaitSetInfo *>(wait_set->data);

  if (!wait_set_info) {
    RMW_SET_ERROR_MSG("failed to allocate wait set");
    goto fail;
  }

  wait_set_info->wait_set = dds_WaitSet_create();
  if (wait_set_info->wait_set == nullptr) {
    RMW_SET_ERROR_MSG("failed to allocate wait set");
    goto fail;
  }

  wait_set_info->active_conditions = dds_ConditionSeq_create(4);
  if (wait_set_info->active_conditions == nullptr) {
    RMW_SET_ERROR_MSG("failed to allocate active_conditions sequence");
    goto fail;
  }

  wait_set_info->attached_conditions = dds_ConditionSeq_create(4);
  if (wait_set_info->attached_conditions == nullptr) {
    RMW_SET_ERROR_MSG("failed to allocate attached_conditions sequence");
    goto fail;
  }

  return wait_set;

fail:
  if (wait_set_info != nullptr) {
    if (wait_set_info->active_conditions != nullptr) {
      dds_ConditionSeq_delete(wait_set_info->active_conditions);
    }

    if (wait_set_info->attached_conditions != nullptr) {
      dds_ConditionSeq_delete(wait_set_info->attached_conditions);
    }

    if (wait_set_info->wait_set != nullptr) {
      dds_WaitSet_delete(wait_set_info->wait_set);
    }

    wait_set_info = nullptr;
  }

  if (wait_set != nullptr) {
    if (wait_set->data != nullptr) {
      rmw_free(wait_set->data);
    }

    rmw_wait_set_free(wait_set);
  }

  return nullptr;
}

rmw_ret_t
rmw_destroy_wait_set(rmw_wait_set_t * wait_set)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(wait_set, RMW_RET_ERROR);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    wait_set,
    wait_set->implementation_identifier,
    RMW_GURUMDDS_ID,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);

  rmw_gurumdds_cpp::WaitSetInfo * wait_set_info = static_cast<rmw_gurumdds_cpp::WaitSetInfo *>(wait_set->data);

  if (wait_set_info->active_conditions != nullptr) {
    dds_ConditionSeq_delete(wait_set_info->active_conditions);
  }

  if (wait_set_info->attached_conditions != nullptr) {
    dds_ConditionSeq_delete(wait_set_info->attached_conditions);
  }

  if (wait_set_info->wait_set != nullptr) {
    dds_WaitSet_delete(wait_set_info->wait_set);
  }

  wait_set_info = nullptr;

  if (wait_set->data != nullptr) {
    rmw_free(wait_set->data);
  }

  if (wait_set != nullptr) {
    rmw_wait_set_free(wait_set);
  }

  return RMW_RET_OK;
}

rmw_ret_t
rmw_wait(
  rmw_subscriptions_t * subscriptions,
  rmw_guard_conditions_t * guard_conditions,
  rmw_services_t * services,
  rmw_clients_t * clients,
  rmw_events_t * events,
  rmw_wait_set_t * wait_set,
  const rmw_time_t * wait_timeout)
{
  return rmw_gurumdds_cpp::wait(
    RMW_GURUMDDS_ID, subscriptions, guard_conditions,
    services, clients, events, wait_set, wait_timeout);
}
}  // extern "C"
