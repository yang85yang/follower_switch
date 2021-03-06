#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int32.h>
#include <math.h>



class roscar_follower
{
public:
    roscar_follower();
    int32_t g_flag;
private:
    ros::NodeHandle n;
    ros::Subscriber turtlebot_follower_cmd_sub;
    ros::Publisher roscar_follower_cmd_pub;
    ros::Subscriber follower_flag_sub;
    geometry_msgs::Twist roscar_cmd;
    void followerCallBack(const geometry_msgs::Twist::ConstPtr &follower_cmd);
    void followerFlagCallBack(const std_msgs::Int32::ConstPtr &followerflag);

};

roscar_follower::roscar_follower()
{
    roscar_follower_cmd_pub=n.advertise<geometry_msgs::Twist>("/cmd_vel",100);
    turtlebot_follower_cmd_sub=n.subscribe<geometry_msgs::Twist>("/follower_velocity_smoother/raw_cmd_vel",100,&roscar_follower::followerCallBack,this);
    follower_flag_sub = n.subscribe<std_msgs::Int32>("follower_flag",1,&roscar_follower::followerFlagCallBack,this);
}

void roscar_follower::followerCallBack(const geometry_msgs::Twist::ConstPtr &follower_cmd)
{
    roscar_cmd.linear.x = follower_cmd->linear.x;
    roscar_cmd.angular.z = follower_cmd->angular.z;

#if 1
                if(g_flag !=101)
                {
                    //If g_flag is not 101,publsih zero velocity
                    geometry_msgs::Twist cmd_vel_zero;
                    cmd_vel_zero.linear.x = 0.0;
                    cmd_vel_zero.angular.z = 0.0;
                    roscar_follower_cmd_pub.publish(cmd_vel_zero);
                }
                else
                {
                    //If g_flag is 101,publsih velocity
                    roscar_follower_cmd_pub.publish(roscar_cmd);
                }
#endif
}

void roscar_follower::followerFlagCallBack(const std_msgs::Int32::ConstPtr &followerflag)
{
    //Receive flag to switch pub velocity
        //int32_t flag=INFINITY;
        g_flag=followerflag->data;
}

int main(int argc,char **argv)
{
   ros::init(argc,argv,"roscar_follower");
   roscar_follower follower;
   ROS_INFO("roscar follower start...");
   ros::spin();

   return 0;
}
