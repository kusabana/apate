<div align="center">
  <h3><a href="https://github.com/kusabana">
    ~kusabana/</a>apate – α‧πά‧τη ( deceit, deception )
  </h3>
c++20 header-only x86 hooking library
</div>

#
### installation
* cmake
```cmake
# Subdir
add_subdirectory("path/to/apate")

# FetchContent
FetchContent_Declare(
  apate
  GIT_REPOSITORY https://github.com/kusabana/apate.git
  GIT_TAG 1.0.0
)

FetchContent_MakeAvailable(apate)

# ...

target_link_libraries(library PUBLIC apate)
```
* cmkr.build
```toml
[fetch-content]
apate = { git = "https://github.com/kusabana/apate", tag = "1.0.0" }

[target.library]
link-libraries = ["apate"]
```