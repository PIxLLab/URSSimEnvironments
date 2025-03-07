#include <vector>

#include <ros/ros.h>

#include "urs_wearable/common.h"

#include <geometry_msgs/Pose.h>
#include <std_msgs/String.h>
#include <urs_wearable/AddArea.h>
#include <urs_wearable/AddLocation.h>
#include <urs_wearable/Feedback.h>
#include <urs_wearable/SetGoal.h>

const std::string SET_GOAL_SERVICE_NAME = "/urs_wearable/set_goal";

void feedbackCB(const urs_wearable::Feedback::ConstPtr& msg)
{
  ros_warn("status: " + std::to_string(msg->status));
}

int main(int argc, char **argv)
{
  // Initialize ROS and sets up a node
  ros::init(argc, argv, "fake_wearable");
  ros::NodeHandle nh;

  ros::Publisher battery_pub = nh.advertise<std_msgs::String>("/w_battery_value", 1);
  ros::Subscriber feedback_sub = nh.subscribe("/urs_wearable/feedback", 100, feedbackCB);
  ros::ServiceClient add_location_client = nh.serviceClient<urs_wearable::AddLocation>("/urs_wearable/add_location");
  ros::ServiceClient add_area_client = nh.serviceClient<urs_wearable::AddArea>("/urs_wearable/add_area");

  std::vector<std::uint8_t> loc_ids;
  urs_wearable::AddLocation add_location_srv;
  add_location_srv.request.pose.position.x = 10;
  add_location_srv.request.pose.position.y = 10;
  add_location_srv.request.pose.position.z = 15;
  if (add_location_client.call(add_location_srv))
  {
    loc_ids.push_back(add_location_srv.response.loc_id);
  }
  else
  {
    ros_error("Error in calling /urs_wearable/add_location");
  }
  add_location_srv.request.pose.position.x = 20;
  add_location_srv.request.pose.position.y = 20;
  if (add_location_client.call(add_location_srv))
  {
    loc_ids.push_back(add_location_srv.response.loc_id);
  }
  else
  {
    ros_error("Error in calling /urs_wearable/add_location");
  }

  std::vector<std::uint8_t> area_ids;
  urs_wearable::AddArea add_area_srv;
  add_area_srv.request.loc_id_left = loc_ids[0];
  add_area_srv.request.loc_id_right = loc_ids[1];
  if (add_area_client.call(add_area_srv))
  {
    area_ids.push_back(add_area_srv.response.area_id);
  }
  else
  {
    ros_error("Error in calling /urs_wearable/add_area");
  }

  urs_wearable::SetGoal set_goal_srv;
  urs_wearable::Predicate pred;
  pred.truth_value = true;

  pred.type = urs_wearable::Predicate::TYPE_AT;
  pred.at.d.value = 0;
  pred.at.l.value = loc_ids[0];
  set_goal_srv.request.goal.push_back(pred);

  pred.type = urs_wearable::Predicate::TYPE_AT;
  pred.at.d.value = 1;
  pred.at.l.value = loc_ids[1];
  set_goal_srv.request.goal.push_back(pred);

  pred.type = urs_wearable::Predicate::TYPE_SCANNED;
  pred.scanned.d.value = 2;
  pred.scanned.a.value = area_ids[0];
  set_goal_srv.request.goal.push_back(pred);

  if (!ros::service::call(SET_GOAL_SERVICE_NAME, set_goal_srv))
  {
    ros_error("Error in calling " + SET_GOAL_SERVICE_NAME);
  }

  int battery_level = 100;
  while (ros::ok() && battery_level >= 0)
  {
    std_msgs::String s;
    s.data = " {'longitude':0.0,'latitude':0.0,'player_id':0,'building_number':0,'drone_clue_value':0,'drone_id':0,'battery_value':"
             + std::to_string(battery_level) + "}";
    battery_pub.publish(s);
    ros::Duration(0.1).sleep();

    s.data = " {'longitude':0.0,'latitude':0.0,'player_id':0,'building_number':0,'drone_clue_value':0,'drone_id':2,'battery_value':"
             + std::to_string(battery_level) + "}";
    battery_pub.publish(s);
    ros::Duration(0.1).sleep();

    battery_level--;
    ros::spinOnce();
  }

  ros::spin();

  return EXIT_SUCCESS;
}
