from _bbox import ffi, lib

RESULT_LIMIT = 512

def collide(boxes):
	buffer_in = ffi.new("struct box[]", len(boxes))
	for i, b in enumerate(boxes):
		left, bottom, right, top = b
		buffer_in[i].left = left
		buffer_in[i].right = right
		buffer_in[i].bottom = bottom
		buffer_in[i].top = top
	buffer_out = ffi.new("struct result[]", RESULT_LIMIT)
	result = lib.detect(buffer_in, len(boxes), buffer_out, RESULT_LIMIT)
	final = []
	for i in range(result):
		final.append((
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
