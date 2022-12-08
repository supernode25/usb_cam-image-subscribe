#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <ctime> 
#include <iostream>

using namespace std;

static const std::string OPENCV_WINDOW = "receiving";
int imgCount = 0;

class ImageRawSub
{
    ros::NodeHandle node;
    image_transport::ImageTransport it;  
    image_transport::Subscriber image_sub;
    image_transport::Subscriber video_sub;

public:
    ImageRawSub()
        : it(node)
        {
            image_sub = it.subscribe("usb_cam/image_raw",10,&ImageRawSub::ImageCallback,this);
            video_sub = it.subscribe("usb_cam/image_raw",10,&ImageRawSub::VideoCallback,this);

            cv::namedWindow(OPENCV_WINDOW);
        }

    ~ImageRawSub()
    {
        cv::destroyWindow(OPENCV_WINDOW);
    }

    void ImageCallback(const sensor_msgs::Image::ConstPtr& img)
    {
      time_t timer;  
      struct tm* t;
      timer = time(NULL);
      t = localtime(&timer);
      string type = ".bmp";
      string textimg = "/home/cona/opencv_test_image/" + to_string(t->tm_year + 1900) + "-" +
      to_string(t->tm_mon + 1) + "-" + to_string(t->tm_mday) + "-" + to_string(t->tm_hour) + ":" +
      to_string(t->tm_min) + ":" + to_string(t->tm_sec) + type;
      
      cv_bridge::CvImagePtr cv_ptr;  
      try {cv_ptr = cv_bridge::toCvCopy(img, sensor_msgs::image_encodings::BGR8);}
      catch (cv_bridge::Exception &e){ ROS_ERROR(" converting Error"); return;}  

      char c;
      cin.get(c);
      if(c == '\n')
        {              
            ++imgCount;
            cv::imwrite(textimg, cv_ptr->image);
            cout <<  t->tm_year + 1900 << "-" << t->tm_mon + 1 << "-" << t->tm_mday << " " 
            << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << " " << imgCount 
            << "번째" << " usb_cam의 img파일을 받았습니다 " << endl;
        }
      else if(c == 'x')   {  exit(1);  }  
    }
 

    void VideoCallback(const sensor_msgs::Image::ConstPtr& video)
    {
      cv_bridge::CvImagePtr cv_ptr;
      try{cv_ptr = cv_bridge::toCvCopy(video, sensor_msgs::image_encodings::BGR8); }
      catch (cv_bridge::Exception &e){ ROS_ERROR(" converting Error"); return; }
      cv::imshow("video_subscribe", cv_ptr->image); 
    }
};


int main(int argc, char **argv)
{
    cv::startWindowThread();
    ros::init(argc, argv, "img_sub_node");
    ImageRawSub sub;
    ros::AsyncSpinner spinner(0); 
    spinner.start(); 
    ros::waitForShutdown();
 
  
    return 0; 
}


















