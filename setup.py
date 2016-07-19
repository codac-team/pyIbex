from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import setuptools
import subprocess
# import pkgconfig # helper class to get ibex flags and libs
__version__ = '1.5.0a'


class get_pybind_include(object):
    """Helper class to determine the pybind11 include path
    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked. """

    def __init__(self, user=False):
        self.user = user

    def __str__(self):
        import pybind11
        return pybind11.get_include(self.user)


def get_ibexflags():
    result = subprocess.check_output(["pkg-config", "--cflags", "--libs", "ibex"])
    result = result.decode('utf-8').rstrip()
    # print(result)
    ret = {'library_dirs': [], 'include_dirs':[], 'libraries': [], 'cpp_flag':[]}
    for s in result.split(' '):
      if '-I' in s:
        ret['include_dirs'].append(s)
      elif '-L' in  s:
        ret['library_dirs'].append(s)
      elif '-l' in s[:2]:
        ret['libraries'].append(s)
      else:
        ret['cpp_flag'].append(s)

    return ret

# # copied from https://github.com/pierriko/atlaas/blob/master/python/setup.py
# def pkg_config(*packages, **kw):
#     flag_map = {'-I': 'include_dirs', '-L': 'library_dirs', '-l': 'libraries'}
#     command = ['pkg-config', '--libs', '--cflags']+list(packages)
#     for token in subprocess.check_output(command).decode().split():
#         kw.setdefault(flag_map.get(token[:2]), []).append(token[2:])
#     kw.pop(None, None) # in case of unwanted result, eg. -Wl,-rpath,
#     if 'library_dirs' in kw:
#       kw['runtime_library_dirs'] = kw['library_dirs'] # configure rpath
#     print(kw)
#     return kw


ext_modules = [
    Extension(
        'pyibex.pyibex',
        [
          'src/core/pyIbex.cpp',
          'src/core/pyIbex_Interval.cpp',
          'src/core/pyIbex_IntervalVector.cpp',
          'src/core/pyIbex_Function.cpp',
          'src/core/pyIbex_Ctc.cpp',
          'src/core/pyIbex_Separators.cpp',
          'src/core/pyIbex_Bsc.cpp',
          'src/core/pyIbex_IntervalMatrix.cpp',
        ],
        include_dirs=[
            'src',
            'pybind11/include'
        ],
        language='c++'
    )
]


# As of Python 3.6, CCompiler has a `has_flag` method.
# cf http://bugs.python.org/issue26689
def has_flag(compiler, flagname):
    """Return a boolean indicating whether a flag name is supported on
    the specified compiler.
    """
    import tempfile
    with tempfile.NamedTemporaryFile('w', suffix='.cpp') as f:
        f.write('int main (int argc, char **argv) { return 0; }')
        try:
            compiler.compile([f.name], extra_postargs=[flagname])
        except setuptools.distutils.errors.CompileError:
            return False
    return True


def cpp_flag(compiler):
    """Return the -std=c++[11/14] compiler flag.
    The c++14 is prefered over c++11 (when it is available).
    """
    if has_flag(compiler, '-std=c++14'):
        return '-std=c++14'
    elif has_flag(compiler, '-std=c++11'):
        return '-std=c++11'
    else:
        raise RuntimeError('Unsupported compiler -- at least C++11 support '
                           'is needed!')

class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""
    c_opts = {
        'msvc': ['/EHsc'],
        'unix': [],
    }

    if sys.platform == 'darwin':
        c_opts['unix'] += ['-stdlib=libc++', '-mmacosx-version-min=10.7']

    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        if ct == 'unix':
            opts.append('-DVERSION_INFO="%s"' % self.distribution.get_version())
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, '-fvisibility=hidden'):
                opts.append('-fvisibility=hidden')
        elif ct == 'msvc':
            opts.append('/DVERSION_INFO=\\"%s\\"' % self.distribution.get_version())

        ibex_config = get_ibexflags()
        for d in ibex_config['include_dirs']:
          self.compiler.add_include_dir(d.replace('-I',""))
        for d in ibex_config['library_dirs']:
          self.compiler.add_library_dir(d.replace("-L",""))
        for l in ibex_config['libraries']:
          self.compiler.add_library(l.replace('-l',""))
        for f in ibex_config['cpp_flag']:
          opts.append(f)

        for ext in self.extensions:
            ext.extra_compile_args = opts
        build_ext.build_extensions(self)


setup(
    name='pyibex',
    version=__version__,
    author='Benoit Desrochers',
    author_email='ben.ensta@gmail.com',
    url='https://github.com/benEnsta/pyIbex',
    description='PyIbex a python interface of ibexlib ',
    long_description='',
    ext_modules=ext_modules,
    # install_requires=['pybind11>=1.7'],
    cmdclass={'build_ext': BuildExt},
    packages=['pyibex', 'pyibex.examples', 'pyibex.polar', "pyibex.geometry"],
    license="LGPL v3",


    # zip_safe=False,
)
