from distutils.core import setup, Extension

module1 = Extension('sht610',
                    include_dirs = ['/usr/local/include'],
                    libraries = ['pthread', 'usb'],
                    library_dirs = ['/usr/local/lib'],
                    sources = ['sht610module.c'])

setup (name = 'pysht610',
       version = '0.0',
       author = 'Hans-Christoph Steiner',
       author_email = 'hans@eds.org',
       description = 'Control the Sankyo SHT610 card dispenser',
       ext_modules = [module1])
