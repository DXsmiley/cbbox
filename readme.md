# cbbox

A library to check whether axis-aligned bounding boxes are overlapping.

*cbbox* is designed for resolving bulk queries. You pass it many rectangles at once and it returns all pairs of rectangles that overlap.

## Installation

	pip install https://github.com/DXsmiley/cbbox/archive/master.zip --upgrade

## Usage

```python
import cbbox
cbbox.collide(rectangles)
```

### cbbox.collide(`rectangles`)

#### Arguments

The `rectangles` arguments should be a *sorted* list of rectangles. Each rectangle should be a tuple of 4 floats, denoting the `left`, `bottom`, `right` and `top` side of the rectangle in that order.

Note that `left` < `right` and `bottom` < `top`.

The rectangles should be sorted by their left coordinate, in increasing order. This is acheiveable with the following:

```python
rectangles.sort(key = lambda r: r[0])
```

In the future, the library may do this for you.

#### Return value

The function returns a list of tuples. Each tuple contains two integers - the indexes of two rectangles that overlapped.

### Example:

```python
print(cbboxcollide([
	(0, 0, 10, 10),
	(5, 5, 20, 20),
	(12, 4, 14, 32)
]))
```
