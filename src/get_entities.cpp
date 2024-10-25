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

#include "rmw_gurumdds_cpp/get_entities.hpp"
#include "rmw_gurumdds_cpp/identifier.hpp"
#include "rmw_gurumdds_cpp/rmw_context_impl.hpp"
#include "rmw_gurumdds_cpp/event_info_common.hpp"
#include "rmw_gurumdds_cpp/event_info_service.hpp"

namespace rmw_gurumdds_cpp
{
dds_DomainParticipant *
get_participant(rmw_node_t * node)
{
  if (node == nullptr) {
    return nullptr;
  }

  if (node->implementation_identifier != RMW_GURUMDDS_ID) {
    return nullptr;
  }

  return node->context->impl->participant;
}

dds_Publisher *
get_publisher(rmw_publisher_t * publisher)
{
  if (publisher == nullptr) {
    return nullptr;
  }

  if (publisher->implementation_identifier != RMW_GURUMDDS_ID) {
    return nullptr;
  }

  PublisherInfo * impl = static_cast<PublisherInfo *>(publisher->data);
  return dds_DataWriter_get_publisher(impl->topic_writer);
}

dds_DataWriter *
get_data_writer(rmw_publisher_t * publisher)
{
  if (publisher == nullptr) {
    return nullptr;
  }

  if (publisher->implementation_identifier != RMW_GURUMDDS_ID) {
    return nullptr;
  }

  PublisherInfo * impl = static_cast<PublisherInfo *>(publisher->data);
  return impl->topic_writer;
}

dds_Subscriber *
get_subscriber(rmw_subscription_t * subscription)
{
  if (subscription == nullptr) {
    return nullptr;
  }

  if (subscription->implementation_identifier != RMW_GURUMDDS_ID) {
    return nullptr;
  }

  SubscriberInfo * impl = static_cast<SubscriberInfo *>(subscription->data);
  return dds_DataReader_get_subscriber(impl->topic_reader);
}

dds_DataReader *
get_data_reader(rmw_subscription_t * subscription)
{
  if (subscription == nullptr) {
    return nullptr;
  }

  if (subscription->implementation_identifier != RMW_GURUMDDS_ID) {
    return nullptr;
  }

  SubscriberInfo * impl = static_cast<SubscriberInfo *>(subscription->data);
  return impl->topic_reader;
}

dds_DataWriter *
get_request_data_writer(rmw_client_t * client)
{
  if (client == nullptr) {
    return nullptr;
  }

  if (client->implementation_identifier != RMW_GURUMDDS_ID) {
    return nullptr;
  }

  ClientInfo * impl = static_cast<ClientInfo *>(client->data);
  return impl->request_writer;
}

dds_DataReader *
get_response_data_reader(rmw_client_t * client)
{
  if (client == nullptr) {
    return nullptr;
  }

  if (client->implementation_identifier != RMW_GURUMDDS_ID) {
    return nullptr;
  }

  ClientInfo * impl = static_cast<ClientInfo *>(client->data);
  return impl->response_reader;
}

dds_DataReader *
get_request_data_reader(rmw_service_t * service)
{
  if (service == nullptr) {
    return nullptr;
  }

  if (service->implementation_identifier != RMW_GURUMDDS_ID) {
    return nullptr;
  }

  ServiceInfo * impl = static_cast<ServiceInfo *>(service->data);
  return impl->request_reader;
}

dds_DataWriter *
get_response_data_writer(rmw_service_t * service)
{
  if (service == nullptr) {
    return nullptr;
  }

  if (service->implementation_identifier != RMW_GURUMDDS_ID) {
    return nullptr;
  }

  ServiceInfo * impl = static_cast<ServiceInfo *>(service->data);
  return impl->response_writer;
}
} // namespace rmw_gurumdds_cpp
