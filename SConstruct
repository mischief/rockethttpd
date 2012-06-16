import os
import platform

opts = Variables('scache.conf')

opts.AddVariables(
  BoolVariable('debug', 'Enable debugging symbols', False),
)

env = Environment(options = opts, ENV = os.environ)
Help(opts.GenerateHelpText(env))
opts.Save('scache.conf', env)

if env['debug']:
  env.Append(CFLAGS = ' -g -O0 -fkeep-inline-functions -finline-functions ')
  env.Append(CPPDEFINES = 'DEBUG')

env.Append(CPPFLAGS=['-Wall','-Wextra','-std=c99','-pthread','-pipe'])

env.Append(CPPFLAGS = '-Isrc')

files = Glob('src/*.c')

env.Program('rockethttpd', files)

