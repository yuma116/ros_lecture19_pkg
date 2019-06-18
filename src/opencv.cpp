#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/image_raw", 1,
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_topic_2", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    // Draw an example circle on the video stream
    //if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
    //  cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));

    for(int j = 0; j < cv_ptr->image.rows; j++){
        for(int i = 0; i < cv_ptr->image.cols; i++){
            double pix_ave = (cv_ptr->image.at<cv::Vec3b>(j,i)[0]
                           +  cv_ptr->image.at<cv::Vec3b>(j,i)[1]
                           +  cv_ptr->image.at<cv::Vec3b>(j,i)[2]) / 3;
            if(pix_ave < 100){
              cv_ptr->image.at<cv::Vec3b>(j,i)[0] = 0; //B
              cv_ptr->image.at<cv::Vec3b>(j,i)[1] = 0; //G
              cv_ptr->image.at<cv::Vec3b>(j,i)[2] = 0; //R
            } else {
              cv_ptr->image.at<cv::Vec3b>(j,i)[0] = 255; //B
              cv_ptr->image.at<cv::Vec3b>(j,i)[1] = 255; //G
              cv_ptr->image.at<cv::Vec3b>(j,i)[2] = 255; //R
            }
            
        }
    }


    // Update GUI Window
    cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    cv::waitKey(3);

    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
