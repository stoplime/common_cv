#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <string> 
#include <iostream>

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

int main(int argc, char **argv)
{
	ros::init(argc, argv, "odroid_pub_test");
	
	ros::NodeHandle n;
	
	ros::Publisher odroid_pub = n.advertise<std_msgs::String>("publisher", 1);
	
	ros::Rate looprate(10);
	
	int frequency = 0;
	int count = 0;
	//char esc_key = '';
	while(ros::ok())
	{
		std_msgs::String msg;
		string s = "Im gonna ssh into you "+patch::to_string(count);
		
		msg.data = s;
		odroid_pub.publish(msg);
		cout << "pubing" << ++count << endl;
		
		ros::spinOnce();
		looprate.sleep();
		
	}
	
}
