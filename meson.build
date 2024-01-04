project(
  'anx_main',
  'cpp',
  version: '0.1',
  meson_version: '>=0.57.0',
  default_options: [
    'werror=true',
    'warning_level=3',
    'cpp_std=c++20',
  ])

# Project Arguments
cpp = meson.get_compiler('cpp')
add_project_arguments(
  cpp.get_supported_arguments([
    '-DBOOST_ERROR_CODE_HEADER_ONLY',
    '-DBOOST_SYSTEM_NO_DEPRECATED',
    '-DBOOST_ALL_NO_LIB',
    '-DBOOST_NO_RTTI',
    '-DBOOST_NO_TYPEID',
    '-DBOOST_ASIO_DISABLE_THREADS',
    '-DBOOST_COROUTINES_NO_DEPRECATION_WARNING',
    '-Wno-psabi',
    '-Wno-pedantic',
  ]),
  language : 'cpp')

fs = import('fs')

if get_option('poplar').enabled()
  add_project_arguments(
    cpp.get_supported_arguments([
        '-DPOPLAR_ENABLED',
    ]),
    language : 'cpp')
endif

if get_option('orca').enabled()
  add_project_arguments(
    cpp.get_supported_arguments([
        '-DORCA_ENABLED',
    ]),
    language : 'cpp')
endif

root_inc = include_directories('.', 'include')

# Dependencies
sdbusplus_dep = dependency('sdbusplus')
phosphor_dbus_interfaces_dep = dependency('phosphor-dbus-interfaces')
phosphor_logging_dep = dependency('phosphor-logging')
ipmid_dep = dependency('libipmid')
gpiod_dep = dependency('libgpiodcxx',
      default_options: ['bindings=cxx'],
      )

if cpp.has_header_symbol(
        'nlohmann/json.hpp',
        'nlohmann::json::string_t',
        required:false)
    nlohmann_json_dep = declare_dependency()
else
    nlohmann_json_dep = dependency('nlohmann-json')
endif

zvesoemcmds_pre = declare_dependency(
  include_directories: root_inc,
  dependencies: [
    ipmid_dep,
    nlohmann_json_dep,
    phosphor_dbus_interfaces_dep,
    phosphor_logging_dep,
    sdbusplus_dep,
    gpiod_dep,
])

sources = files('anx_main.cpp')


zvesoemcmds_lib = library(
  'zvesoemcmds',
  sources,
  implicit_include_directories: false,
  dependencies: zvesoemcmds_pre,
  version: meson.project_version(),
  override_options: ['b_lundef=false'],
  install: true,
  install_dir: get_option('libdir') / 'ipmid-providers')
