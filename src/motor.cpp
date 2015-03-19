#include <ros/ros.h>
#include <geometry_msgs/Quaternion.h>
#include <sensor_msgs/Joy.h>

 class TeleopTurtle
 {
 public:
   TeleopTurtle();
 
 private:
   void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
   
   ros::NodeHandle nh_;
 
   int linear_, angular_;
   double l_scale_, a_scale_;
   ros::Publisher vel_pub_;
   ros::Subscriber joy_sub_;
   
 };


 TeleopTurtle::TeleopTurtle():
   linear_(1),
   angular_(2)
 {
 
   nh_.param("axis_linear", linear_, linear_);
   nh_.param("axis_angular", angular_, angular_);
   nh_.param("scale_angular", a_scale_, a_scale_);
   nh_.param("scale_linear", l_scale_, l_scale_);


   vel_pub_ = nh_.advertise<geometry_msgs::Quaternion>("motordriver", 1);
 
   joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &TeleopTurtle::joyCallback, this);
 
 }

 void TeleopTurtle::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
 {

  geometry_msgs::Quaternion quat;
  float lin = l_scale_*joy->axes[linear_];
  float ang = a_scale_*joy->axes[angular_];

  if(lin>0)
  {
    quat.x=0.0 ;quat.z=0.0 ;
    quat.w = 80*lin;
    quat.y = 80*lin;

    quat.w += 40*ang;
    quat.y -= 40*ang;
  }
  if(lin<0)
  {
    quat.w=0.0 ;quat.y=0.0 ;
    quat.x = -80*lin;
    quat.z = -80*lin;

    quat.x += 40*ang;
    quat.z -= 40*ang;
  }
  if(lin==0)
  {
    quat.x=0.0 ;
    quat.z=0.0 ;
    quat.w=0.0 ;
    quat.y=0.0 ;

    if(ang<0)
    {
      quat.y = 40*ang;
      quat.x = 40*ang;
    }
    if(ang>0)
    {
      quat.w = -40*ang;
      quat.z = -40*ang;
    }
  }
  vel_pub_.publish(quat);
 }

 int main(int argc, char** argv)
 {
   ros::init(argc, argv, "motor_node");
   TeleopTurtle teleop_turtle;

   ros::spin();
 }
