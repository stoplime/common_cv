#include "ros/ros.h"
#include "std_msgs/String.h"
#include "keyboard/Key.h"
#include <sstream>
#include <string> 
#include <iostream>
#include <vector>

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

vector<float> pos(3,0);
const float speed = 1;

void keyboard_callback(const keyboard::Key& msg)
{
	int value = msg.code;
	switch (value)
	{
		case 119:
			pos[0]+=speed;
			break;
		case 115:
			pos[0]-=speed;
			break;
		case 97:
			pos[1]-=speed;
			break;
		case 100:
			pos[1]+=speed;
			break;
		case 32:
			pos[2]+=speed;
			break;
		case 304:
			pos[2]-=speed;
			break;
		//default:
			
	}
	
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "odroid_pub");
	
	ros::NodeHandle n;
	
	ros::Publisher odroid_pub = n.advertise<std_msgs::String>("publisher", 1);
	ros::Subscriber keyboard_input = n.subscribe("/keyboard/keydown", 1, keyboard_callback);
	
	ros::Rate looprate(10);
	
	//int count = 0;
	while(ros::ok())
	{
		std_msgs::String msg;
		string s = "Pos: "+patch::to_string(pos[0])+", "+patch::to_string(pos[1])+", "+patch::to_string(pos[2]);
		
		msg.data = s;
		odroid_pub.publish(msg);
		cout << s << endl;
		
		ros::spinOnce();
		looprate.sleep();
		
	}
	
}
