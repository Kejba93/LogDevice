/**
 * Copyright (c) 2017-present, Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "AdminServerSettings.h"

#include "logdevice/common/commandline_util_chrono.h"
#include "logdevice/common/settings/Validators.h"

using namespace facebook::logdevice::setting_validators;

namespace facebook { namespace logdevice {

void AdminServerSettings::defineSettings(SettingEasyInit& init) {
  using namespace SettingFlag;

  // clang-format off
  init
    ("admin-port", &admin_port, "6440", validate_port,
     "TCP port on which the server listens to for admin commands, supports "
     "commands over SSL",
     SERVER | REQUIRES_RESTART,
     SettingsCategory::AdminAPI)

    ("admin-unix-socket", &admin_unix_socket, "", validate_unix_socket,
     "Path to the unix domain socket the server will use to listen for admin "
     "thrift interface",
     SERVER | REQUIRES_RESTART,
     SettingsCategory::AdminAPI)

    ("safety-check-max-logs-in-flight", &safety_max_logs_in_flight, "1000",
     [](int x) -> void {
       if (x <= 0) {
         throw boost::program_options::error(
           "safety-check-max-logs-in-flight must be a positive integer"
         );
       }
     },
     "The number of concurrent logs that we runs checks against during execution"
     " of the CheckImpact operation either internally during a maintenance or "
     "through the Admin API's checkImpact() call",
     SERVER,
     SettingsCategory::AdminAPI)

    ("safety-check-timeout", &safety_check_timeout, "10min",
     [](std::chrono::milliseconds val) -> void {
       if (val.count() <= 0) {
         throw boost::program_options::error(
           "safety-check-timeout must be positive"
         );
       }
     },
     "The total time the safety check should take to run. This is the time that "
     "the CheckImpact operation need to take to scan all logs along with all "
     "the historical metadata to ensure than a maintenance is safe",
     SERVER,
     SettingsCategory::AdminAPI)
    ;
  // clang-format on
};

}} // namespace facebook::logdevice
