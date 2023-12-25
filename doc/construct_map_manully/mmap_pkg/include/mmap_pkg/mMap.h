/*
0	0	0	0
100	100	0	-1

*/
#include<ros/ros.h>
#include<anv_msgs/occupancyGrid.h>

void arrayCopy(int* target, int origin[][], int height, int width) {
	int counter = 0;
	for(int i = 0; i < height; ++i){
		for (int j = 0; j < width; ++j) {
			target[counter++] = origin[i][j];
		}
	}
}

