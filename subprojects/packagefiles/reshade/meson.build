project('reshade',
    'cpp',
    version: '6.1.2-dev',
    license: 'BSD-3'
)

reshade_dep = declare_dependency(
    include_directories: include_directories('.'),
 	version            : meson.project_version()
)

meson.override_dependency('reshade', reshade_dep)
