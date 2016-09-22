#!/usr/bin/env python

from setuptools import setup


setup(name='mitutuyo_interface',
      version='0.1',
      description='A python module to interface with a mitutoyo interface',
      author='Catherine Holloway',
      author_email='milankie@gmail.com',
      url='https://gitlab.com/milankie/teensy-mitutoyo-interface',
      zip_safe=False,
      install_requirements=['instruments'],
      py_modules=['mitutoyo_interface']
      )