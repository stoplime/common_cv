#include "ros/ros.h"
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include "opencv/cv.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sensor_msgs/image_encodings.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

namespace enc = sensor_msgs::image_encodings;

image_transport::Publisher image_pub;
static const char WINDOW[] = "Image Unprocessed";
static const char WINDOW2[] = "Image Processed";
Scalar minRange = Scalar(200,200,200);
Scalar maxRange = Scalar(255,255,255);
Point test_point = Point(0,0);
Point prev_point = test_point;
int rangeSlider = 25;
Mat current_frame;

static void update_process()
{
	if (test_point != prev_point)
	{
		prev_point = test_point;
		
		Scalar temp;
		Scalar radius = Scalar(rangeSlider,rangeSlider,rangeSlider);
		
		temp = Scalar(current_frame.at<Vec3b>(test_point));
		minRange = temp-radius;
		maxRange = temp+radius;
	}
}

static void on_trackbar(int, void*)
{
	update_process();
}

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	cout << "running" << endl;
	cv_bridge::CvImagePtr cv_ptr;
    try
    {
		cv_ptr = cv_bridge::toCvCopy(msg, enc::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    
    imshow(WINDOW, cv_ptr->image);
    
    current_frame = cv_ptr->image;
    
    // set click
    update_process();
	
    //processing
    inRange(current_frame, minRange, maxRange, current_frame);
    
    imshow(WINDOW2, current_frame);
    
    //Convert to publish
    cv_bridge::CvImage out_msg;
	out_msg.header   = msg->header; // Same timestamp and tf frame as input image
	out_msg.encoding = enc::MONO8; // sensor_msgs::image_encodings
	out_msg.image    = current_frame; // Your cv::Mat
    
    
	image_pub.publish(out_msg.toImageMsg());
	//cout << "Published" << endl;
	
	//reset current_frame
	current_frame = cv_ptr->image;
	
	waitKey(10);
}

static void onMouse( int event, int x, int y, int, void* )
{
	if( event != EVENT_LBUTTONDOWN )
        return;
	
	test_point = Point(x,y);
}

int main (int argc, char **argv)
{
	//Mat img;
	
	ros::init(argc, argv, "processed_image");
	
	ros::NodeHandle n;
	namedWindow(WINDOW);
	namedWindow(WINDOW2);
	
	createTrackbar("Range", WINDOW, &rangeSlider, 255, on_trackbar);
	on_trackbar(rangeSlider, 0);
	
	setMouseCallback(WINDOW, onMouse, 0);
	
	image_transport::ImageTransport it(n);
	cout << "! Has image transport" << endl;
	image_transport::Subscriber image_sub = it.subscribe("/usb_cam/image_raw", 1, imageCallback);
	cout << "! Created Subsriber" << endl;
	image_pub = it.advertise("color_filtered", 1);
	
	cout << "is running" << endl;
	
	ros::spin();
	return 0;
}
