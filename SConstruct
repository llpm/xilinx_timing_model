LibPaths = []
env = Environment(CPPPATH=['./libs/bil/src'],
                  CXXFLAGS=['-O0', '-mfpmath=sse', '-msse4', '-march=native',
                            '-Wall', '-g', '-std=c++0x'],
                  LIBS=['c', 'dl'],
                  LIBPATH=LibPaths,
                  LINKFLAGS=['-pthread'] + map(lambda x: "-Wl,-rpath=%s" % x, LibPaths))

bil = env.Library('bil', Glob("./libs/bil/src*/*.cpp") +
                         Glob("./libs/bil/src/*/*/*.cpp"))

Default(bil)

