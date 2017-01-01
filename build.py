import cffi

ffibuilder = cffi.FFI()

HEAD = open('bbox.h').read()
BODY = open('bbox.c').read()

ffibuilder.cdef(HEAD)
ffibuilder.set_source("_bbox", HEAD + '\n\n' + BODY)

ffibuilder.compile(verbose = True)
