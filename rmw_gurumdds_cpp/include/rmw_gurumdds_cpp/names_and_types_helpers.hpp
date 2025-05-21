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

#ifndef RMW_GURUMDDS__NAMES_AND_TYPES_HELPERS_HPP_
#define RMW_GURUMDDS__NAMES_AND_TYPES_HELPERS_HPP_

#include <map>
#include <set>
#include <string>

#include "rcutils/allocator.h"

#include "rmw/convert_rcutils_ret_to_rmw_ret.h"
#include "rmw/error_handling.h"
#include "rmw/names_and_types.h"
#include "rmw/rmw.h"

namespace rmw_gurumdds_cpp
{
rmw_ret_t
copy_services_to_names_and_types(
  const std::map<std::string, std::set<std::string>> & services,
  rcutils_allocator_t * allocator,
  rmw_names_and_types_t * service_names_and_types);

rmw_ret_t
copy_topics_names_and_types(
  const std::map<std::string, std::set<std::string>> & topics,
  rcutils_allocator_t * allocator,
  bool no_demangle,
  rmw_names_and_types_t * topic_names_and_types);

// Topic name creater
std::string
create_topic_name(
  const char * prefix,
  const char * topic_name,
  const char * suffix,
  bool avoid_ros_namespace_conventions);

std::string
create_topic_name(
  const char * prefix,
  const char * topic_name,
  const char * suffix,
  const rmw_qos_profile_t * qos_policies);
} // namespace rmw_gurumdds_cpp

#endif // RMW_GURUMDDS__NAMES_AND_TYPES_HELPERS_HPP_
