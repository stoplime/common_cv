#include "ros/ros.h"
#include "std_msgs/String.h"
#include "keyboard/Key.h"
#include <sstream>
#include <string> 
#include <iostream>
#include <vector>
#include "nav_msgs/Odometry.h"
#include <visualization_msgs/Marker.h>

using namespace std;

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

//vector<float> pos(3,0);
geometry_msgs::Point pos;
const float speed = 1;

void keyboard_callback(const keyboard::Key& msg)
{
	int value = msg.code;
	switch (value)
	{
		case 119:
			pos.x+=speed;
			break;
		case 115:
			pos.x-=speed;
			break;
		case 97:
			pos.y+=speed;
			break;
		case 100:
			pos.y-=speed;
			break;
		case 32:
			pos.z+=speed;
			break;
		case 304:
			pos.z-=speed;
			break;
		//default:
			
	}
}

visualization_msgs::Marker create_marker()
{
	visualization_msgs::Marker marker;
	
	// Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "/my_frame";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "basic_shapes";
    marker.id = 0;

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = visualization_msgs::Marker::CUBE;

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
    marker.action = visualization_msgs::Marker::ADD;

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 1.0;
    marker.scale.y = 1.0;
    marker.scale.z = 1.0;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 1.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    //marker.lifetime = ros::Duration();
    return marker;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "odroid_send_pos");
	
	ros::NodeHandle n;
	
	ros::Publisher odroid_pub = n.advertise<visualization_msgs::Marker>("pose", 1);
	ros::Subscriber keyboard_input = n.subscribe("/keyboard/keydown", 1, keyboard_callback);
	
	ros::Rate looprate(10);
	
	//int count = 0;
	while(ros::ok())
	{
		std_msgs::String msg;
		string s = "Pos: "+patch::to_string(pos.x)+", "+patch::to_string(pos.y)+", "+patch::to_string(pos.z);
		
		visualization_msgs::Marker marker;
		marker = create_marker();
		marker.pose.position = pos;
		odroid_pub.publish(marker);
		cout << s << endl;
		
		ros::spinOnce();
		looprate.sleep();
		
	}
	
}
