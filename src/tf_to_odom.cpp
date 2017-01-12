#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

#include <tf_to_odom/tf_to_odom.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "");

  ros::NodeHandle node("~");
/*
  ros::service::waitForService("spawn");
  ros::ServiceClient add_turtle =
    node.serviceClient<turtlesim::Spawn>("spawn");
  turtlesim::Spawn srv;
  add_turtle.call(srv);
*/

  std::string lis_parent_frame_id;
  std::string lis_child_frame_id;
  std::string pub_parent_frame_id;
  std::string pub_child_frame_id;

  node.param("lis_parent_frame_id", lis_parent_frame_id, std::string("/map"));
  node.param("lis_child_frame_id", lis_child_frame_id, std::string("/in_odom_link"));
  //odometry topic, parent and child frameid
  node.param("pub_parent_frame_id", pub_parent_frame_id, std::string("/map"));
  node.param("pub_child_frame_id", pub_child_frame_id, std::string("base_link"));

  ros::Publisher odom_pub =
    node.advertise<nav_msgs::Odometry>("odom", 100);

  tf::TransformListener listener;
  tf::TransformBroadcaster br;
  //the tf data is expected to arrive @ 100Hz
  ros::Rate rate(100.0);
  while (node.ok()){
    //tf::StampedTransform odom_transform;
    geometry_msgs::Quaternion quat;
    quat.w = 1.0;
    geometry_msgs::Point point;
    point.x = 0;
    point.y = 0;
    point.z = 0;
    geometry_msgs::PoseStamped stamped_pose_in;
    stamped_pose_in.header.frame_id = lis_child_frame_id;
    stamped_pose_in.pose.orientation = quat;
    stamped_pose_in.pose.position = point;
    geometry_msgs::PoseStamped stamped_pose_out;
    stamped_pose_out.header.frame_id = lis_parent_frame_id;
    
    try{
      listener.waitForTransform(lis_parent_frame_id, lis_child_frame_id, ros::Time(0), ros::Duration(1.0) );
      //listener.lookupTransform(lis_parent_frame_id, lis_child_frame_id,
      //                         ros::Time(0), odom_transform);
      listener.transformPose(lis_parent_frame_id,stamped_pose_in,stamped_pose_out);
    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }

    geometry_msgs::TwistWithCovariance odom_vel;
//     odom_vel.twist.angular.x = 0.0;
//     odom_vel.twist.angular.y = 0.0;
//     odom_vel.twist.angular.z = 0.0;
//     odom_vel.twist.linear.x = 0.0;
//     odom_vel.twist.linear.y = 0.0;
//     odom_vel.twist.linear.z = 0.0;
    
//     odom_vel.covariance[0] = 1.0; 
//     odom_vel.covariance[1*6 +1] = 1.0; 
//     odom_vel.covariance[2*6 +2] = 1.0; 
//     odom_vel.covariance[3*6 +3] = 1.0;
//     odom_vel.covariance[4*6 +4] = 1.0;
//     odom_vel.covariance[5*6 +5] = 1.0;
//     for(int i = 0; i<36 ; i++){
//       odom_vel.covariance[i] = 1.0;
//     }
    
    geometry_msgs::PoseWithCovariance odom_pose;
    odom_pose.pose = stamped_pose_out.pose;
//     for(int i = 0; i<36 ; i++){
//       odom_pose.covariance[i] = 1.0;
//     }
    //odom_pose.covariance = {1.0};
//     odom_pose.covariance[0] = 2.0; 
//     odom_pose.covariance[1*6 +1] = 2.0; 
//     odom_pose.covariance[2*6 +2] = 2.0; 
//     odom_pose.covariance[3*6 +3] = 2.0;
//     odom_pose.covariance[4*6 +4] = 2.0;
//     odom_pose.covariance[5*6 +5] = 2.0;
    
    nav_msgs::Odometry odom_msg;
    odom_msg.header.frame_id = pub_parent_frame_id;
    odom_msg.child_frame_id = pub_child_frame_id;
    odom_msg.header.stamp = ros::Time::now();
    odom_msg.pose = odom_pose;
    odom_msg.twist = odom_vel;

    odom_pub.publish(odom_msg);
    
//     odom_transform.frame_id_ = lis_child_frame_id;
//     odom_transform.child_frame_id_ = "odom_tf";
//     br.sendTransform(odom_transform);
    

    rate.sleep();
  }
  return 0;
};





