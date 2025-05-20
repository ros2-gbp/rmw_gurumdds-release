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

extern "C"
{
rmw_guard_condition_t *
rmw_create_guard_condition(rmw_context_t * context)
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

  rmw_guard_condition_t * guard_condition = rmw_guard_condition_allocate();
  if (guard_condition == nullptr) {
    RMW_SET_ERROR_MSG("failed to allocate guard condition handle");
    return nullptr;
  }

  dds_GuardCondition * dds_guard_condition = dds_GuardCondition_create();
  if (dds_guard_condition == nullptr) {
    RMW_SET_ERROR_MSG("failed to create guard condition");
    goto fail;
  }

  guard_condition->implementation_identifier = RMW_GURUMDDS_ID;
  guard_condition->data = dds_guard_condition;
  return guard_condition;

fail:
  if (guard_condition != nullptr) {
    rmw_guard_condition_free(guard_condition);
  }

  return nullptr;
}

rmw_ret_t
rmw_destroy_guard_condition(rmw_guard_condition_t * guard_condition)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(guard_condition, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    guard condition,
    guard_condition->implementation_identifier,
    RMW_GURUMDDS_ID,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);

  dds_GuardCondition * dds_guard_condition =
    static_cast<dds_GuardCondition *>(guard_condition->data);
  dds_GuardCondition_delete(dds_guard_condition);
  rmw_guard_condition_free(guard_condition);

  return RMW_RET_OK;
}

rmw_ret_t
rmw_trigger_guard_condition(const rmw_guard_condition_t * guard_condition)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(guard_condition, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_TYPE_IDENTIFIERS_MATCH(
    guard condition,
    guard_condition->implementation_identifier,
    RMW_GURUMDDS_ID,
    return RMW_RET_INCORRECT_RMW_IMPLEMENTATION);

  dds_GuardCondition * dds_guard_condition =
    static_cast<dds_GuardCondition *>(guard_condition->data);
  dds_ReturnCode_t ret = dds_GuardCondition_set_trigger_value(dds_guard_condition, true);
  if (ret != dds_RETCODE_OK) {
    return RMW_RET_ERROR;
  }

  return RMW_RET_OK;
}
}  // extern "C"
