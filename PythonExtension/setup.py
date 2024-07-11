from distutils.core import setup, Extension

module = Extension('example', sources = ['example.cpp'])

setup(
    name='ExampleModule',
    version='1.0',
    description='This is a demo package',
    ext_modules=[module]
)