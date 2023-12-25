/*
0	0	0	0
100	100	0	-1

*/
#include "ros/ros.h"
#include "nav_msgs/OccupancyGrid.h"

void arrayCopy(nav_msgs::OccupancyGrid_<std::allocator<void> >::_data_type&target, int8_t origin[][4], int height, int width) {
	int counter = 0;
	for(int i = 0; i < height; ++i){
		for (int j = 0; j < width; ++j) {
			// target[counter++] = *(origin+i*width+j);
			target[counter++] = origin[i][j];
		}
	}
}

void arrayCopy(nav_msgs::OccupancyGrid_<std::allocator<void> >::_data_type&target, std::vector<std::vector<int8_t>>&original) {
	uint32_t height = original.size();
	uint32_t width  = original[0].size();
	int counter = 0;
	for(int i = 0; i < height; ++i){
		for (int j = 0; j < width; ++j) {
			// target[counter++] = *(origin+i*width+j);
			target[counter++] = original[i][j];
		}
	}
}

int main(int argc, char* argv[]) {
	ros::init(argc, argv, "mmap_pub_node");
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<nav_msgs::OccupancyGrid>("/map", 10);
	// int8_t mmap[2][4] = {
	// 	{100, 100, 0, -1},
	// 	{0,0,0,0}
	// };
	std::vector<std::vector<int8_t>> mmap = {
		{100, 100, 0, -1},
		{0,0,0,0}
	};
	ros::Rate r(1);// 1 Hz
	//occupancy grid topic init
	nav_msgs::OccupancyGrid msg;
	while (ros::ok()) {
		msg.header.frame_id = "map";
		msg.header.stamp = ros::Time::now();

		// Point (0,0) in map, we're going to correct it with real world.
		msg.info.origin.position.x = 0;// x bias = 0(float)
		msg.info.origin.position.y = 0;// y bias = 0(float)
		msg.info.resolution = 1.0;// m.(float)
		//the map we're going to publish is 4*2.
		// msg.info.height = 2;
		// msg.info.width = 4; 
		msg.info.height = mmap.size();
		msg.info.width = mmap[0].size();

		msg.data.resize(msg.info.height * msg.info.width);
		// arrayCopy(msg.data, mmap, msg.info.height, msg.info.width);
		arrayCopy(msg.data, mmap);
		pub.publish(msg);
		r.sleep();
	}
	return 0;
}

