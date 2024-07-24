from distutils.core import setup, Extension
import numpy

module = Extension(
    'example', 
    sources = ['example.cpp'],
    include_dirs=[numpy.get_include()],
)

module_2 = Extension(
    'mymodules',
    sources = ['PyQueue.cpp', 'Queue.cpp'],
)

setup(
    name='ExampleModule',
    version='1.0',
    description='This is a demo package with NumPy support',
    ext_modules=[module, module_2]
)