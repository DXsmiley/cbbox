from distutils.core import setup, Extension

setup(
	name = 'cbbox',
	version = '0.0.1',
	description = 'C extention to check bounding box overlaps',
	author = 'DXsmiley',
	setup_requires=["cffi>=1.0.0"],
	cffi_modules=["build.py:ffibuilder"],
	install_requires=["cffi>=1.0.0"],
	py_modules = [
		'cbbox'
	]
)
