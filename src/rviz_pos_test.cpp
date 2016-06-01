#include "ros/ros.h"
#include "std_msgs/String.h"
#include "keyboard/Key.h"
#include <sstream>
#include <string> 
#include <iostream>
#include <vector>
#include "nav_msgs/Odometry.h"

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
			pos.y-=speed;
			break;
		case 100:
			pos.y+=speed;
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

int main(int argc, char **argv)
{
	ros::init(argc, argv, "odroid_send_pos");
	
	ros::NodeHandle n;
	
	ros::Publisher odroid_pub = n.advertise<nav_msgs::Odometry>("pose", 1);
	ros::Subscriber keyboard_input = n.subscribe("/keyboard/keydown", 1, keyboard_callback);
	
	ros::Rate looprate(10);
	
	//int count = 0;
	while(ros::ok())
	{
		std_msgs::String msg;
		string s = "Pos: "+patch::to_string(pos.x)+", "+patch::to_string(pos.y)+", "+patch::to_string(pos.z);
		
		nav_msgs::Odometry pose;
		pose.pose.pose.position = pos;
		odroid_pub.publish(pose);
		cout << s << endl;
		
		ros::spinOnce();
		looprate.sleep();
		
	}
	
}
