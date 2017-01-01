struct box {
	double left;
	double right;
	double bottom;
	double top;
};

struct result {
	int first;
	int second;
};

int detect(struct box * boxes, int box_num, struct result * result, int result_limit);
