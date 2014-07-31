LibPaths = []
env = Environment(CPPPATH=['./libs/bil/src'],
                  CXXFLAGS=['-O3', '-mfpmath=sse', '-msse4', '-march=native',
                            '-Wall', '-g', '-std=c++0x'],
                  LIBS=['c', 'dl'],
                  LIBPATH=LibPaths,
                  LINKFLAGS=['-pthread'] + map(lambda x: "-Wl,-rpath=%s" % x, LibPaths))

bil = env.Library('bil', Glob("./libs/bil/src/*/*.cpp") +
                         Glob("./libs/bil/src/*/*/*.cpp"))

llpm = env.Library('llpm', Glob("./lib/*.cpp") +
                         Glob("./lib/*/*.cpp"))
env.Append(LIBS=[bil, llpm])

for d in Glob("./drivers/*"):
    d = str(d).split("/")[-1]
    driver = env.Program(d, Glob("./drivers/%s/*.cpp" % d))
    Default(driver)


