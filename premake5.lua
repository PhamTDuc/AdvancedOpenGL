workspace "AdvancedOpenGL"
    architecture "x86"

    configurations
    {
        "Debug",
        "Release",
    }

outputdir ="%{cfg.builcfg}-%{cfg.system}-%{cfg.architecture}"

project "AdvancedOpenGL"
    location "AdvancedOpenGL"
    kind "ConsoleApp"
    flags "MultiProcessorCompile"
    language "C++"
    targetdir("bin/%{prj.name}")
    objdir("int-bin/%{prj.name}")

    files
    {
        "%{prj.name}/**.cpp",
        "%{prj.name}/**.h",
        "{prj.name}/**.c",
        "%{prj.name}/**.hpp",
        "%{prj.name}/**.inl",
    }

    libdirs {"%{prj.name}/include"}
    links 
    {
        "glfw3.lib",
        "freetype.lib",
    }
    includedirs
    {
        "%{prj.name}/include",
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "10.0.17763.0"
        -- postbuildcommands
        -- {
        --     "{COPY} %{cfg.name}/fonts %{cfg.targetdir}",
        -- }

    filter "configurations:Debug"
        buildoptions "/MDd"
        symbols "On"
        links
        {
            "msvcrtd.lib",
        }

    filter "configurations:Release"
        optimize "On"
        buildoptions "/MD"
        links
        {
            "msvcrt.lib",
        }

