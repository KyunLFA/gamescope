project('glm',
    'cpp',
    version: '1.0.0-39',
    license: 'MIT'
)

glm_dep = declare_dependency(
    include_directories: include_directories('.'),
 	version            : meson.project_version()
)

meson.override_dependency('glm', glm_dep)
