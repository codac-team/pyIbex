.. _basics:

First steps
###########

This sections demonstrates the basic features of pyIbex. Before getting
started, pyIbex and its dependancies must be correctly installed.


Install from pipy
============================

For windows 64 bits and Mac OS X users a version using pyzo is avaiable on pipy.
This version is base on python 3.4 and can be found at `PyZo website`_

.. _`PyZo website`: https://cmake.org/runningcmake

.. code-block:: bash

   pip install py

Building from sources
============================

Linux/MacOS
-----------

On Linux  you'll need to install the **python-dev** or **python3-dev** packages as
well as **cmake**. On Mac OS, the included python version works out of the box,
but **cmake** must still be installed.

After installing the prerequisites, run

.. code-block:: bash

   cmake .
   make -j 4

followed by

.. code-block:: bash

   make test

Windows
-------

On Windows, use the `CMake GUI`_ to create a Visual Studio project. Note that
only the 2015 release and newer versions are supported since pybind11 relies on
various C++11 language features that break older versions of Visual Studio.
After running CMake, open the created :file:`pybind11.sln` file and perform a
release build, which will will produce a file named
:file:`Release\\example.pyd`. Copy this file to the :file:`example` directory
and run :file:`example\\run_test.py` using the targeted Python version.

.. _`CMake GUI`: https://cmake.org/runningcmake

.. Note::

    When all tests fail, make sure that

    1. The Python binary and the testcases are compiled for the same processor
       type and bitness (i.e. either **i386** or **x86_64**)

    2. The Python binary used to run :file:`example\\run_test.py` matches the
       Python version specified in the CMake GUI. This is controlled via
       the ``PYTHON_EXECUTABLE`` ``PYTHON_INCLUDE_DIR``, and
       ``PYTHON_LIBRARY`` variables.

.. seealso::

    Advanced users who are already familiar with Boost.Python may want to skip
    the tutorial and look at the test cases in the :file:`example` directory,
    which exercise all features of pybind11.
