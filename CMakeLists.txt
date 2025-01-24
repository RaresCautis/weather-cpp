cmake_minimum_required(VERSION 3.22)

project(weather-cpp
  LANGUAGES CXX
  VERSION 1.0.0
)

# Set C++ standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)
# --- Fetch FTXUI --------------------------------------------------------------

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/arthursonzogni/ftxui.git
  GIT_TAG        v5.0.0  # Specify the version or branch
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(ftxui)
# ------------------------------------------------------------------------------

# --- Fetch json --------------------------------------------------------------

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG        v3.11.3  # Specify the version or branch
  GIT_PROGRESS   TRUE
  GIT_SHALLOW    TRUE
  EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(json)
# ------------------------------------------------------------------------------

# Add your executable
add_executable(weather-cpp src/main.cpp)
target_sources(weather-cpp PRIVATE src/components/menu.cpp)
target_sources(weather-cpp PRIVATE src/components/helpers.cpp)
target_sources(weather-cpp PRIVATE src/components/serializers.cpp)
target_sources(weather-cpp PRIVATE src/components/window.cpp)
target_sources(weather-cpp PRIVATE src/components/config.cpp)
target_sources(weather-cpp PRIVATE src/components/colorManager.cpp)
target_include_directories(weather-cpp PRIVATE src src/components)

# Link libraries
target_link_libraries(weather-cpp
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE nlohmann_json::nlohmann_json
)
