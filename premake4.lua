solution "kruto.js"
	objdir "build"
	targetdir "build"
	language "C++"

  if _ACTION == "clean" then
    os.rmdir("build")
  end

	configurations { "debug", "release" }
	includedirs { "src", "src/kruto", "extern/v8/include" }
	libdirs { "extern/v8/out/native/obj.target/tools/gyp" }

  configuration { "release", "linux", "gmake" }
    buildoptions { "-O3" }

	configuration { "linux", "gmake" }
    buildoptions { "-fdiagnostics-show-option", "-Wall", "-Wextra", "-pedantic", "-ansi" }

  project "kruto.js"
    kind "WindowedApp"
    files { "src/**.hpp", "src/**.cpp"  }
    flags { "WinMain" }

    configuration { "linux", "gmake" }
      links { "GL", "SDL", "SDL_image", "SDL_mixer", "v8_base", "v8_snapshot", "pthread" }

newaction 
{
	trigger     = "stdlib",
	description = "Builds stdlib.hpp from stdlib.js",
	execute		= function ()
    src = path.join(os.getcwd(), "src/kruto/sandbox/stdlib.js")
    dst = path.join(os.getcwd(), "src/kruto/sandbox/stdlib.hpp")

    local input  = io.open(src, "r")
    local output = io.open(dst, "w")

    output:write([[// Generated by Kruto.js
// DO NOT EDIT THIS FILE DIRECTLY
// Instead, edit 'stdlib.js', and run 'premake4 stdlib'
#ifndef KRSTDLIB_HPP
#define KRSTDLIB_HPP]])

    output:write("\n\n")
    output:write("namespace kruto {\n\n")
    output:write("const char *STDLIB =")

    while true do
      local line = input:read()
      if line == nil then break end
      output:write('"' .. line:gsub('"', '\\"') .. '\\n"\n')
    end

    input:close()
    
    output:write(";\n\n")
    output:write("}\n\n")
    output:write("#endif")
    output:close()
 	end
}
