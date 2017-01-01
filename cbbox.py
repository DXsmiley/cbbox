from _bbox import ffi, lib


buffer_in_size = 1
buffer_in = ffi.new("struct box[]", buffer_in_size)
buffer_out_size = 512
buffer_out = ffi.new("struct result[]", buffer_out_size)


def _next_power_of_2(x):
	return 1 << (x-1).bit_length()


def collide(boxes):
	global buffer_in_size
	global buffer_in
	global buffer_out_size
	global buffer_out
	num_boxes = len(boxes)
	# Increase the size of the input buffer if required
	if num_boxes > buffer_in_size:
		buffer_in = ffi.new("struct box[]", len(boxes))
		buffer_in_size = num_boxes
	# Copy the boxes into the input buffer
	for i, b in enumerate(boxes):
		left, bottom, right, top = b
		buffer_in[i].left = left
		buffer_in[i].right = right
		buffer_in[i].bottom = bottom
		buffer_in[i].top = top
	# Call the C function
	result = lib.detect(buffer_in, num_boxes, buffer_out, buffer_out_size)
	# If there were more collisions than could be handled, incease the output buffer and try again
	if result > buffer_out_size:
		buffer_out_size = _next_power_of_2(result)
		buffer_out = ffi.new("struct result[]", buffer_out_size)
		result = lib.detect(buffer_in, num_boxes, buffer_out, buffer_out_size)
	# Unpack the output buffer, removing duplicate collisions
	final = set()
	for i in range(result):
		final.add((
			buffer_out[i].first,
			buffer_out[i].second
		))
	return final


if __name__ == '__main__':
	print(collide([
		(0, 0, 10, 10),
		(5, 5, 20, 20),
		(12, 4, 14, 32)
	]))
