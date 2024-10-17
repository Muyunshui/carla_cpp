// Copyright (c) 2020 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include <atomic>  /// �ṩԭ�Ӳ�����ȷ���̰߳�ȫ
#include <chrono>  /// �ṩʱ�书�ܣ�����ʱ�����
#include <random>  /// �ṩ��������ɹ���
#include <unordered_map> /// �ṩ����ӳ�����������ڿ��ٲ���
/// ����Carla�ͻ�����ص�ͷ�ļ�
#include "carla/client/Actor.h"
#include "carla/client/Vehicle.h"
#include "carla/Memory.h"/// ����Carla�ڴ������ص�ͷ�ļ�
#include "carla/rpc/ActorId.h"/// ����Carla Rpc ͨ����ص�ͷ�ļ��������˲����ߵ�Ψһ��ʶ��

#include "carla/trafficmanager/AtomicActorSet.h"/// ����Carla��ͨ�����������ͷ�ļ�
#include "carla/trafficmanager/AtomicMap.h"

namespace carla {
namespace traffic_manager {
    /// ʹ�ñ����򻯴����е�����
namespace cc = carla::client;
namespace cg = carla::geom;
using ActorPtr = carla::SharedPtr<cc::Actor>;/// �����ߵ�����ָ������
using ActorId = carla::ActorId;/// �����ߵ�Ψһ��ʶ������
using Path = std::vector<cg::Location>;/// ·�����ͣ���һϵ�е���λ�����
using Route = std::vector<uint8_t>;/// ·�����ͣ���һϵ���ֽ���ɣ���ʾ·����Ϣ
/// ������Ϣ�ṹ��
struct ChangeLaneInfo {
  bool change_lane = false;/// �Ƿ񻻵�
  bool direction = false;/// ��������
};
/// ��ͨ�������
class Parameters {

private:
  /// �����ٶ����Ʋ���ĵ�������Ŀ���ٶ�ӳ��
  AtomicMap<ActorId, float> percentage_difference_from_speed_limit;
  /// ���������ĳ���ƫ��ӳ��
  AtomicMap<ActorId, float> lane_offset;
  /// ���������ٶȵĵ�������Ŀ���ٶ�ӳ��
  AtomicMap<ActorId, float> exact_desired_speed;
  /// ȫ��Ŀ���ٶ����Ʋ���ٷֱ�
  float global_percentage_difference_from_limit = 0;
  /// ȫ�ֳ���ƫ��
  float global_lane_offset = 0;
  /// ����ײ����ڼ�Ҫ���Ե���Ա����ӳ��
  AtomicMap<ActorId, std::shared_ptr<AtomicActorSet>> ignore_collision;
  /// ��ǰ�������ľ���ӳ��
  AtomicMap<ActorId, float> distance_to_leading_vehicle;
  /// ǿ�ƻ�������ӳ��
  AtomicMap<ActorId, ChangeLaneInfo> force_lane_change;
  /// �Զ���������ӳ��
  AtomicMap<ActorId, bool> auto_lane_change;
  /// ����ͨ�źŵưٷֱ�ӳ��
  AtomicMap<ActorId, float> perc_run_traffic_light;
  /// ����ͨ��־�ٷֱ�ӳ��
  AtomicMap<ActorId, float> perc_run_traffic_sign;
  /// �������˰ٷֱ�ӳ��
  AtomicMap<ActorId, float> perc_ignore_walkers;
  /// ���Գ����ٷֱ�ӳ��
  AtomicMap<ActorId, float> perc_ignore_vehicles;
  /// ������ʻ����ٷֱ�ӳ��
  AtomicMap<ActorId, float> perc_keep_right;
  /// ����󻻵��ٷֱ�ӳ��
  AtomicMap<ActorId, float> perc_random_left;
  /// ����һ����ٷֱ�ӳ��
  AtomicMap<ActorId, float> perc_random_right;
  /// �����ƹ��Զ����±�־ӳ��
  AtomicMap<ActorId, bool> auto_update_vehicle_lights;
  /// ͬ������
  std::atomic<bool> synchronous_mode{false};
  /// ����߾�
  std::atomic<float> distance_margin{2.0};
  /// �������ģʽ����
  std::atomic<bool> hybrid_physics_mode{false};
  /// �Զ�����ģʽ����
  std::atomic<bool> respawn_dormant_vehicles{false};
  /// ��������ǳ�������С��������
  std::atomic<float> respawn_lower_bound{100.0};
  /// ��������ǳ����������������
  std::atomic<float> respawn_upper_bound{1000.0};
  /// ��������ǳ�������С������������
  float min_lower_bound;
  /// ��������ǳ�������������������
  float max_upper_bound;
  /// �������뾶
  std::atomic<float> hybrid_physics_radius {70.0};
  /// Open Street Mapģʽ����
  std::atomic<bool> osm_mode {true};
  /// �Ƿ����Զ���·���Ĳ���ӳ��
  AtomicMap<ActorId, bool> upload_path;
  /// �洢�����Զ���·���Ľṹ
  AtomicMap<ActorId, Path> custom_path;
  /// �Ƿ����Զ���·�ߵĲ���ӳ��
  AtomicMap<ActorId, bool> upload_route;
  /// �洢�����Զ���·�ߵĽṹ
  AtomicMap<ActorId, Route> custom_route;

public:
  Parameters();
  ~Parameters();

  ////////////////////////////////// SETTERS /////////////////////////////////////

  /// Set a vehicle's % decrease in velocity with respect to the speed limit.
  /// If less than 0, it's a % increase.
  void SetPercentageSpeedDifference(const ActorPtr &actor, const float percentage);

  /// Method to set a lane offset displacement from the center line.
  /// Positive values imply a right offset while negative ones mean a left one.
  void SetLaneOffset(const ActorPtr &actor, const float offset);

  /// Set a vehicle's exact desired velocity.
  void SetDesiredSpeed(const ActorPtr &actor, const float value);

  /// Set a global % decrease in velocity with respect to the speed limit.
  /// If less than 0, it's a % increase.
  void SetGlobalPercentageSpeedDifference(float const percentage);

  /// Method to set a global lane offset displacement from the center line.
  /// Positive values imply a right offset while negative ones mean a left one.
  void SetGlobalLaneOffset(float const offset);

  /// Method to set collision detection rules between vehicles.
  void SetCollisionDetection(
      const ActorPtr &reference_actor,
      const ActorPtr &other_actor,
      const bool detect_collision);

  /// Method to force lane change on a vehicle.
  /// Direction flag can be set to true for left and false for right.
  void SetForceLaneChange(const ActorPtr &actor, const bool direction);

  /// Enable/disable automatic lane change on a vehicle.
  void SetAutoLaneChange(const ActorPtr &actor, const bool enable);

  /// Method to specify how much distance a vehicle should maintain to
  /// the leading vehicle.
  void SetDistanceToLeadingVehicle(const ActorPtr &actor, const float distance);

  /// Method to set % to run any traffic sign.
  void SetPercentageRunningSign(const ActorPtr &actor, const float perc);

  /// Method to set % to run any traffic light.
  void SetPercentageRunningLight(const ActorPtr &actor, const float perc);

  /// Method to set % to ignore any vehicle.
  void SetPercentageIgnoreVehicles(const ActorPtr &actor, const float perc);

  /// Method to set % to ignore any vehicle.
  void SetPercentageIgnoreWalkers(const ActorPtr &actor, const float perc);

  /// Method to set % to keep on the right lane.
  void SetKeepRightPercentage(const ActorPtr &actor, const float percentage);

  /// Method to set % to randomly do a left lane change.
  void SetRandomLeftLaneChangePercentage(const ActorPtr &actor, const float percentage);

  /// Method to set % to randomly do a right lane change.
  void SetRandomRightLaneChangePercentage(const ActorPtr &actor, const float percentage);

  /// Method to set the automatic vehicle light state update flag.
  void SetUpdateVehicleLights(const ActorPtr &actor, const bool do_update);

  /// Method to set the distance to leading vehicle for all registered vehicles.
  void SetGlobalDistanceToLeadingVehicle(const float dist);

  /// Set Synchronous mode time out.
  void SetSynchronousModeTimeOutInMiliSecond(const double time);

  /// Method to set hybrid physics mode.
  void SetHybridPhysicsMode(const bool mode_switch);

  /// Method to set synchronous mode.
  void SetSynchronousMode(const bool mode_switch = true);

  /// Method to set hybrid physics radius.
  void SetHybridPhysicsRadius(const float radius);

  /// Method to set Open Street Map mode.
  void SetOSMMode(const bool mode_switch);

  /// Method to set if we are automatically respawning vehicles.
  void SetRespawnDormantVehicles(const bool mode_switch);

  /// Method to set boundaries for respawning vehicles.
  void SetBoundariesRespawnDormantVehicles(const float lower_bound, const float upper_bound);

  /// Method to set limits for boundaries when respawning vehicles.
  void SetMaxBoundaries(const float lower, const float upper);

  /// Method to set our own imported path.
  void SetCustomPath(const ActorPtr &actor, const Path path, const bool empty_buffer);

  /// Method to remove a list of points.
  void RemoveUploadPath(const ActorId &actor_id, const bool remove_path);

  /// Method to update an already set list of points.
  void UpdateUploadPath(const ActorId &actor_id, const Path path);

  /// Method to set our own imported route.
  void SetImportedRoute(const ActorPtr &actor, const Route route, const bool empty_buffer);

  /// Method to remove a route.
  void RemoveImportedRoute(const ActorId &actor_id, const bool remove_path);

  /// Method to update an already set route.
  void UpdateImportedRoute(const ActorId &actor_id, const Route route);

  ///////////////////////////////// GETTERS /////////////////////////////////////

  /// Method to retrieve hybrid physics radius.
  float GetHybridPhysicsRadius() const;

  /// Method to query target velocity for a vehicle.
  float GetVehicleTargetVelocity(const ActorId &actor_id, const float speed_limit) const;

  /// Method to query lane offset for a vehicle.
  float GetLaneOffset(const ActorId &actor_id) const;

  /// Method to query collision avoidance rule between a pair of vehicles.
  bool GetCollisionDetection(const ActorId &reference_actor_id, const ActorId &other_actor_id) const;

  /// Method to query lane change command for a vehicle.
  ChangeLaneInfo GetForceLaneChange(const ActorId &actor_id);

  /// Method to query percentage probability of keep right rule for a vehicle.
  float GetKeepRightPercentage(const ActorId &actor_id);

  /// Method to query percentage probability of a random right lane change for a vehicle.
  float GetRandomLeftLaneChangePercentage(const ActorId &actor_id);

  /// Method to query percentage probability of a random left lane change for a vehicle.
  float GetRandomRightLaneChangePercentage(const ActorId &actor_id);

  /// Method to query auto lane change rule for a vehicle.
  bool GetAutoLaneChange(const ActorId &actor_id) const;

  /// Method to query distance to leading vehicle for a given vehicle.
  float GetDistanceToLeadingVehicle(const ActorId &actor_id) const;

  /// Method to get % to run any traffic light.
  float GetPercentageRunningSign(const ActorId &actor_id) const;

  /// Method to get % to run any traffic light.
  float GetPercentageRunningLight(const ActorId &actor_id) const;

  /// Method to get % to ignore any vehicle.
  float GetPercentageIgnoreVehicles(const ActorId &actor_id) const;

  /// Method to get % to ignore any walker.
  float GetPercentageIgnoreWalkers(const ActorId &actor_id) const;

  /// Method to get if the vehicle lights should be updates automatically
  bool GetUpdateVehicleLights(const ActorId &actor_id) const;

  /// Method to get synchronous mode.
  bool GetSynchronousMode() const;

  /// Get synchronous mode time out
  double GetSynchronousModeTimeOutInMiliSecond() const;

  /// Method to retrieve hybrid physics mode.
  bool GetHybridPhysicsMode() const;

  /// Method to retrieve if we are automatically respawning vehicles.
  bool GetRespawnDormantVehicles() const;

  /// Method to retrieve minimum distance from hero vehicle when respawning vehicles.
  float GetLowerBoundaryRespawnDormantVehicles() const;

  /// Method to retrieve maximum distance from hero vehicle when respawning vehicles.
  float GetUpperBoundaryRespawnDormantVehicles() const;

  /// Method to get Open Street Map mode.
  bool GetOSMMode() const;

  /// Method to get if we are uploading a path.
  bool GetUploadPath(const ActorId &actor_id) const;

  /// Method to get a custom path.
  Path GetCustomPath(const ActorId &actor_id) const;

  /// Method to get if we are uploading a route.
  bool GetUploadRoute(const ActorId &actor_id) const;

  /// Method to get a custom route.
  Route GetImportedRoute(const ActorId &actor_id) const;

  /// Synchronous mode time out variable.
  std::chrono::duration<double, std::milli> synchronous_time_out;
};

} // namespace traffic_manager
} // namespace carla
