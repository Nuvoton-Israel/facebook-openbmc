project('libevb-npcm845-gpio',
    'c',
    version: '0.1',
    license: 'GPL2',
    default_options: [
        'warning_level=3',
        'werror=true',
    ],
    meson_version: '>=0.40'
)

headers = [
    'evb-npcm845_gpio.h',
]

sources = [
    'evb-npcm845_gpio.c',
]

deps = [
    dependency('libevb-npcm845-common')
]

install_headers(
    headers,
    subdir: 'facebook'
)

gpio_lib = shared_library('evb-npcm845-gpio',
    sources,
    dependencies: deps,
    version: meson.project_version(),
    install: true
)

pkg = import('pkgconfig')
pkg.generate(
    libraries: [ gpio_lib ],
    name: meson.project_name(),
    version: meson.project_version(),
    description: 'EVB-NPCM845 gpio Library'
)
