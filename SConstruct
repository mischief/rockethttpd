import os
import platform

opts = Variables('scache.conf')

opts.AddVariables(
  BoolVariable('debug', 'Enable debugging symbols', False),
  BoolVariable('fancy', 'Enable images in directory listings', False),  
)

env = Environment(options = opts, ENV = os.environ)
Help(opts.GenerateHelpText(env))
opts.Save('scache.conf', env)

if env['debug']:
  env.Append(CFLAGS = ' -g -O0 -fkeep-inline-functions -finline-functions ')
  env.Append(CPPDEFINES = 'DEBUG')

if env['fancy']:
  env.Append(CPPDEFINES = 'FANCY')


env.Append(CPPFLAGS=['-Wall','-Wextra','-std=c99','-pthread','-pipe'])

env.Append(CPPFLAGS = '-Isrc', CPPDEFINES = ['_GNU_SOURCE=1', '_REENTRANT=1'])

files = Glob('src/*.c')

env.Program('rockethttpd', files, LIBS = ['pthread', 'm'])

