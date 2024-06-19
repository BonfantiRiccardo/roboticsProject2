#include "ros/ros.h"
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Define a type alias for the Goal struct
typedef struct goal_s{
    double x;
    double y;
    double theta;
} goal_t;

// Function to read goals from a CSV file
std::vector<goal_t> readCSV(const std::string& filename){

	std::vector<goal_t> goals;
	std::ifstream file(filename);
	std::string line;


	 // Read each line from the CSV file
	while (std::getline(file, line)) {
    	std::stringstream ss(line);
        std::string x_str, y_str, theta_str;
        
         // Split the line by commas and convert to double
        if (std::getline(ss, x_str, ',') && std::getline(ss, y_str, ',') && std::getline(ss, theta_str, ',')) {
            goal_t goal;
            goal.x = std::stod(x_str);
            goal.y = std::stod(y_str);
            goal.theta = std::stod(theta_str);
            goals.push_back(goal);
        }
    }

    return goals;

}

// Function to send a goal to the move_base action server
void sendGoal(const goal_t& goal, actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>& ac) {

    // Create a MoveBaseGoal message
    move_base_msgs::MoveBaseGoal move_base_goal;
    move_base_goal.target_pose.header.frame_id = "map";

    move_base_goal.target_pose.pose.position.x = goal.x;
    move_base_goal.target_pose.pose.position.y = goal.y;
    move_base_goal.target_pose.pose.position.z = 0;

    // Calculate quaternion from theta
    double theta = goal.theta;
    move_base_goal.target_pose.pose.orientation.x = 0.0;
    move_base_goal.target_pose.pose.orientation.y = 0.0;
    move_base_goal.target_pose.pose.orientation.z = sin(theta / 2.0);
    move_base_goal.target_pose.pose.orientation.w = cos(theta / 2.0);

    // Send the goal to the action server
    ac.sendGoal(move_base_goal);

    ROS_INFO("Sent new goal: %lf, %lf, %lf", goal.x, goal.y, goal.theta);}

int main(int argc, char *argv[]){

    ros::init(argc, argv, "goal_publisher");  // Initialize the ROS node
    ros::NodeHandle nh;  // Create a NodeHandle

    // Check if the path to the CSV file is provided
    if (argc != 2) {
        ROS_ERROR("Usage: goal_publisher <path_to_csv>");
        return 1;
    }

    std::string filename = argv[1];  // Get the CSV file path from the command line argument
    std::vector<goal_t> goals = readCSV(filename);  // Read goals from the CSV file


    // create the action client
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);

    ROS_INFO("Waiting for the move_base action server to start");
    ac.waitForServer(); // Wait for the action server to start

    ROS_INFO("Action server started, sending goal.");

    // Iterate through each goal and send it to the action server
    for (const auto& goal : goals) {

        sendGoal(goal, ac);  // Send the current goal

        // Wait for the result with a timeout of 60 seconds
        bool finished = ac.waitForResult(ros::Duration(0.0));

        actionlib::SimpleClientGoalState state = ac.getState();
        if (state == actionlib::SimpleClientGoalState::SUCCEEDED) {
            ROS_INFO("Goal succeeded, sending next goal");
        } else {
            ROS_WARN("Goal failed with state: %s", state.toString().c_str());
        }
    }








    




    
}























