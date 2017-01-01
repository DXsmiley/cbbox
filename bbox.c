#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef DEBUG
#include "bbox.h"
#endif

// Constants

#define BUCKET_SIZE 256
#define NUM_BUCKETS 64

// Utility functions

int clamp(int val, int small, int large) {
	return val < small ? small : val > large ? large : val;
}

// Implementation of a resizeable array

struct array {
	int capacity;
	int size;
	int content_size;
	char * start; // assuming char is 1 byte
};

struct array * array_create(int content_size) {
	struct array * a = calloc(1, sizeof (struct array));
	a -> content_size = content_size;
	a -> capacity = 4;
	a -> size = 0;
	a -> start = calloc(1, content_size * 4);
	return a;
}

void * array_get(struct array * a, int index) {
	return (a -> start) + (a -> content_size) * index;
}

void array_push(struct array * a, void * value) {
	if (a -> size == a -> capacity) {
		a -> capacity *= 2;
		char * new = calloc(a -> content_size, a -> capacity);
		memcpy(new, a -> start, a -> size * a -> content_size);
		free(a -> start);
		a -> start = new;
	}
	memcpy(array_get(a, a -> size), value, a -> content_size);
	a -> size += 1;
}

void array_free(struct array * a) {
	free(a -> start);
	free(a);
}

// Overlap for a single box

int overlap(struct box a, struct box b) {
	return a.left < b.right && a.right > b.left && a.bottom < b.top && a.top > b.bottom;
}

// Implementation of the spacial hash.

struct array * buckets[NUM_BUCKETS];

void setup() {
	static int done = 0;
	if (!done) {
		// Create the bucket objects.
		for (int i = 0; i < NUM_BUCKETS; ++i) buckets[i] = array_create(sizeof(int));
		done = 1;
	}
}

double y_offset = (NUM_BUCKETS * BUCKET_SIZE) / 2;

int detect(struct box * boxes, int box_num, struct result * result, int result_limit) {
	int result_num = 0;
	if (box_num > 0) {
		setup();
		int min_used = NUM_BUCKETS - 1;
		int max_used = 0;
		for (int i = 0; i < box_num; ++i) {
			// Find which buckets the object sits in.
			int bot = clamp((int)(boxes[i].bottom + y_offset) / BUCKET_SIZE, 0, NUM_BUCKETS - 1);
			int top = clamp((int)(boxes[i].top + y_offset) / BUCKET_SIZE, 0, NUM_BUCKETS - 1);
			min_used = min(min_used, bot);
			max_used = max(max_used, top);
			for (int k = bot; k <= top; ++k) {
				int * contents = (int *)(buckets[k] -> start);
				int new_size = 0;
				// Remove things from the bucket if they are no longer relavent.
				for (int c = 0; c < buckets[k] -> size; ++c) {
					if (boxes[contents[c]].right >= boxes[i].left) {
						contents[new_size++] = contents[c];
					}
				}
				buckets[k] -> size = new_size;
				// Look at the contents of the bucket and see what it overlaps with.
				for (int c = 0; c < new_size; ++c) {
					if (overlap(boxes[i], boxes[contents[c]])) {
						if (result_num < result_limit) {
							result[result_num].first = contents[c];
							result[result_num].second = i;
						}
						result_num++;
					}
				}
			}
			for (int k = bot; k <= top; ++k) {
				array_push(buckets[k], &i);
			}
		}
		// Empty the buckets. No need to completely clear the contents.
		for (int i = min_used; i <= max_used; ++i) buckets[i] -> size = 0;
	}
	// Return the number of collisions
	return result_num;
}
