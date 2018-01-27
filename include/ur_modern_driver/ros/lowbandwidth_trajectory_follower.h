#pragma once

#include <inttypes.h>
#include <array>
#include <atomic>
#include <cstddef>
#include <cstring>
#include <string>
#include <thread>
#include <vector>
#include "ur_modern_driver/log.h"
#include "ur_modern_driver/ur/commander.h"
#include "ur_modern_driver/ur/server.h"
#include "ur_modern_driver/ros/trajectory_follower_interface.h"

class LowBandwidthTrajectoryFollower:  public TrajectoryFollowerInterface
{
private:
  std::atomic<bool> running_;
  std::array<double, 6> last_positions_;
  URCommander &commander_;
  URServer server_;

  double time_interval_, servoj_time_, servoj_time_waiting_, max_waiting_time_, \
         servoj_gain_, servoj_lookahead_time_, max_joint_difference_;
  bool debug_, more_debug_;

  std::string program_;

  template <typename T>
  size_t append(uint8_t *buffer, T &val)
  {
    size_t s = sizeof(T);
    std::memcpy(buffer, &val, s);
    return s;
  }

  bool execute(const std::array<double, 6> &positions,
               const std::array<double, 6> &velocities,
               double sample_number, double time_in_seconds);

public:
  LowBandwidthTrajectoryFollower(URCommander &commander, std::string &reverse_ip, int reverse_port, bool version_3);

  bool start();
  bool execute(std::vector<TrajectoryPoint> &trajectory, std::atomic<bool> &interrupt);
  void stop();

  virtual ~LowBandwidthTrajectoryFollower() {};
};
