add_rules("mode.debug", "mode.release")
set_languages("c++23")

add_requires("rapidjson", "fmt", "angelscript", "uuid_v4", "libpng")


target("AgeAPI")
    set_kind("static")
    add_files("AgeAPI/**.cpp")
    add_includedirs("AgeAPI/", {public = true})
    add_headerfiles("AgeAPI/**.hpp")
    add_packages("rapidjson", "fmt", "angelscript", "uuid_v4", "libpng")
    set_symbols("debug")

target("AgeRuntime")
    set_kind("binary")
    add_files("AgeRuntime/**.cpp")
    add_includedirs("AgeRuntime/", {public = false})
    add_headerfiles("AgeRuntime/**.hpp")
    add_deps("AgeAPI", "AgeCore")
    set_symbols("debug")

target("AgeCore")
    set_kind("shared")
    add_files("AgeCore/**.cpp")
    add_includedirs("AgeCore/", {public = false})
    add_headerfiles("AgeCore/**.hpp")
    add_deps("AgeAPI")

target("AgeDataDumper")
    set_kind("binary")
    add_files("AgeDataDumper/**.cpp")
    add_includedirs("AgeDataDumper/", {public = false})
    add_headerfiles("AgeDataDumper/**.hpp")
    add_deps("rapidjson", "fmt")
    
