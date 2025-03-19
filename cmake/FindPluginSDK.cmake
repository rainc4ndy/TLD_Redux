include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

# Check for environment variable PLUGIN_SDK_DIR
find_path(PluginSDK_ROOT_DIR
  NAMES "shared/PluginBase.h"
  HINTS
    "$ENV{PLUGIN_SDK_DIR}"  # Use ENV to read the environment variable
    "${PluginSDK_ROOT}"      # Keep PluginSDK_ROOT as an additional hint
  NO_DEFAULT_PATH
)

# Shared include directory
find_path(PluginSDK_shared_INCLUDE_DIR
  NAMES "PluginBase.h"
  HINTS "${PluginSDK_ROOT_DIR}/shared"
  NO_DEFAULT_PATH
)
if(PluginSDK_shared_INCLUDE_DIR)
  set(PluginSDK_shared_INCLUDE_DIRS "${PluginSDK_shared_INCLUDE_DIR}" "${PluginSDK_shared_INCLUDE_DIR}/game")
endif()

# GTA III configuration
set(PluginSDK_gta3_REL_LIB_NAMES "plugin_iii")
set(PluginSDK_gta3_DBG_LIB_NAMES "plugin_iii_d")
set(PluginSDK_gta3_INC_NAMES "plugin_III.h")
set(PluginSDK_gta3_INCDIR_NAME "plugin_III")
set(PluginSDK_gta3_GAME_INCDIR_NAME "game_III")
set(PluginSDK_gta3_DEFINES
  PLUGIN_SGV_10EN
  GTA3
  GTAGAME_NAME="3"
  GTAGAME_ABBR="3"
  GTAGAME_ABBRLOW="3"
  GTAGAME_PROTAGONISTNAME="Claude"
  GTAGAME_CITYNAME="Liberty City"
)

# GTA: VC configuration
set(PluginSDK_gtavc_REL_LIB_NAMES "plugin_vc")
set(PluginSDK_gtavc_DBG_LIB_NAMES "plugin_vc_d")
set(PluginSDK_gtavc_INC_NAMES "plugin_vc.h")
set(PluginSDK_gtavc_INCDIR_NAME "plugin_vc")
set(PluginSDK_gtavc_GAME_INCDIR_NAME "game_vc")
set(PluginSDK_gtavc_DEFINES
  PLUGIN_SGV_10EN
  GTAVC
  GTAGAME_NAME="Vice City"
  GTAGAME_ABBR="VC"
  GTAGAME_ABBRLOW="vc"
  GTAGAME_PROTAGONISTNAME="Tommy"
  GTAGAME_CITYNAME="Vice City"
)

# GTA: SA configuration
set(PluginSDK_gtasa_REL_LIB_NAMES "plugin" "plugin_sa")
set(PluginSDK_gtasa_DBG_LIB_NAMES "plugin_d" "plugin_sa_d")
set(PluginSDK_gtasa_INC_NAMES "plugin_sa.h")
set(PluginSDK_gtasa_INCDIR_NAME "plugin_sa")
set(PluginSDK_gtasa_GAME_INCDIR_NAME "game_sa")
set(PluginSDK_gtasa_DEFINES
  PLUGIN_SGV_10US
  GTASA
  GTAGAME_NAME="San Andreas"
  GTAGAME_ABBR="SA"
  GTAGAME_ABBRLOW="sa"
  GTAGAME_PROTAGONISTNAME="CJ"
  GTAGAME_CITYNAME="San Andreas"
)

# Process components
foreach(comp_name ${PluginSDK_FIND_COMPONENTS})
  set(component PluginSDK_${comp_name})
  if(${component}_INC_NAMES)
    find_path(${component}_INCLUDE_DIR
      NAMES ${${component}_INC_NAMES}
      HINTS "${PluginSDK_ROOT_DIR}/${${component}_INCDIR_NAME}"
      NO_DEFAULT_PATH
    )
    if(${component}_INCLUDE_DIR)
      set(${component}_INCLUDE_DIRS "${${component}_INCLUDE_DIR}" "${${component}_INCLUDE_DIR}/${${component}_GAME_INCDIR_NAME}")
    endif()

    find_library(${component}_LIBRARY_RELEASE
      NAMES ${${component}_REL_LIB_NAMES}
      HINTS "${PluginSDK_ROOT_DIR}/output"
      PATH_SUFFIXES "lib"
      NO_DEFAULT_PATH
    )
    find_library(${component}_LIBRARY_DEBUG
      NAMES ${${component}_DBG_LIB_NAMES}
      HINTS "${PluginSDK_ROOT_DIR}/output"
      PATH_SUFFIXES "lib"
      NO_DEFAULT_PATH
    )

    select_library_configurations("${component}")
    mark_as_advanced(${component}_INCLUDE_DIR)

    set(target PluginSDK::${comp_name})
    if(${component}_FOUND AND NOT TARGET ${target})
      add_library(${target} STATIC IMPORTED)
      target_include_directories(${target}
        INTERFACE
          "${${component}_INCLUDE_DIRS}"
          "${PluginSDK_shared_INCLUDE_DIRS}"
      )
      target_compile_definitions(${target}
        INTERFACE
          "_CRT_SECURE_NO_WARNINGS"
          "_CRT_NON_CONFORMING_SWPRINTFS"
          ${${component}_DEFINES}
      )
      set_target_properties(${target}
        PROPERTIES
          IMPORTED_LOCATION "${${component}_LIBRARY_RELEASE}"
          IMPORTED_LOCATION_DEBUG "${${component}_LIBRARY_DEBUG}"
      )
    endif()
  else()
    message(FATAL_ERROR "Wrong component name: ${component}")
  endif()
endforeach()

find_package_handle_standard_args("PluginSDK" REQUIRED_VARS PluginSDK_ROOT_DIR HANDLE_COMPONENTS)
mark_as_advanced(PluginSDK_ROOT_DIR PluginSDK_shared_INCLUDE_DIR)
