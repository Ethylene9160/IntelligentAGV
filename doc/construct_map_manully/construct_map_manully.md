# 手动构建一个地图

## 包依赖

ros官方核心库`ros/ros.h`

导航包下的栅格化地图类`nav_msgs/OccupancyGrid.h`

请正确引入您的这个包的位置。

进行建图时，需要：

```c++
#include"ros/ros.h"
#include"nav_msgs/OccupancyGrid.h"
```



## 初始化map话题发布者

同别的话题类似，需要初始化Publisher实例变量，发布的消息类型为`nav_msgs::OccupancyGrid`。

```c++
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<nav_msgs::OccupancyGrid>("/map", 10);// put "nav_msgs::OccupancyGrid" into the templete. Inside "/map" means the topic you name is called "/map".
```

同发布话题类似，请设置好频率。

## 更新map并发布

在nav_msgs::OccupancyGrid msg中，有三个参数我们将会用到，这三个参数是非常重要的：

`nav_msgs::OccupancyGrid_<std::allocator<void> >::_data_type data`: 一个**一维数组**。实际上，`data`变量在这里的数据类型是`std::vector<int8_t>`，长度应当被resize为地图的**行数乘以列数**。

`int32_t height`: 地图的高度。

`int32_t width`: 地图的宽度。

其中，地图中，有以下三种数据：

> 100: 表示障碍物
>
> 1：表示无阻碍
>
> -1：表示未探索

例如，这样一个数组：

```c++
std::vector<int8_t> yourmap = {
    {100,100,0,	-1	},
    {0,	0,	0,	0	}
};
```

表示的含义是：

```bash
	y
	^----|----|----|----|
	|	 |	  |	   |	|
	|XXXX|XXXX|	   |????|
    |--------------------> x
```

`XXXX`表示障碍，`????`表示未知，空白表示可通过。

此外，还有几个可能用到的需要修改的参数：

`float nav_msgs::OccupancyGrid.nav_msgs::OccupancyGrid.info.origin.position.x`: 起点横坐标偏移量。地图将会向x轴正方向平移x米。

`float info.origin.position.y`: 起点纵坐标偏移量。地图将会向y轴正方向平移x米。

`float nav_msgs::OccupancyGrid.info.origin.position.z`：起点横坐标偏移量。地图将会向x轴正方向平移x米。

* Example:

```c++
	//initialize the topic info
	msg.header.frame_id = "map";
	nav_msgs::OccupancyGrid msg;
	msg.header.stamp = ros::Time::now();

	// Point (0,0) in map, we're going to correct it with real world.
	msg.info.origin.position.x = 0.0;// x bias = 0(float in meters)
	msg.info.origin.position.y = 0.0;// y bias = 0(float in meters)
	msg.info.resolution = 1.0;// float in meters

	//the map we're going to publish is 4*2.
	// msg.info.height = 2;
	// msg.info.width = 4; 
	msg.info.height = mmap.size();
	msg.info.width = mmap[0].size();
	//we  have to convert the map (2D array) into an 1D array.
	//in fact, "nav_msgs::OccupancyGrid.data" is a type defined from "std::vector<int8_t>".
	msg.data.resize(msg.info.height * msg.info.width);
	//assign values into the "msg.data".
	arrayCopy(msg.data, mmap);
	pub.publish(msg);
	r.sleep();//sleep.
```

详细请参考`mmap_pkg/src/mMap.cpp`.

## 使用

针对于该例程，节点名被我们设置为了`map_pub_node`

使用cmake指令编译后，保证roscore为启动状态后，运行节点，即可发布`/map`话题。

```bash
catkin_make -j16
source ./devel/setup.bash
rosrun mmap_pkg map_pub_node
```

然后，启动rviz，电机`add`，加入topic下的map话题，就能看到这个map了。