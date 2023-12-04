--[[ Project definitions ]]
set_project("aoc_2023")
set_languages("c++latest")
set_version("0.1.0")
set_description("Modern c++ solutions to the Advent of Code 2023 challenges")
set_license("MIT")
set_xmakever("2.8.5")

--[[ Project settings ]]
set_toolchains("clang")
add_rules("mode.debug", "mode.release", "mode.releasedbg", "plugin.compile_commands.autoupdate")
set_defaultmode("releasedbg")
set_allowedmodes("release", "debug", "releasedbg")
set_warnings("all", "error")
set_optimize("fastest")
add_includedirs("include", "/usr/local/include", "/usr/include")
add_cxflags("-Wno-attributes", "-std=c++26", "-stdlib=libc++", "-fexperimental-library", "-g", { force = true })
add_ldflags("-L/usr/local/lib", "-lpthread", "-lc++", "-lc++abi", "-lfmt", "-lSDL2main", "-lSDL2", { force = true })

-- [[ Project dependencies ]]
-- add_repositories("hpc_research_laboratory  https://github.com/HpcResearchLaboratory/repo.git main")

local aoc_deps = {}
local aoc_cli_deps = { "cxxopts" }
-- local test_deps = { "gtest" }
-- local bench_deps = { "benchmark" }

add_requires(table.unpack(aoc_deps))
add_requires(table.unpack(aoc_cli_deps))
-- add_requires(table.unpack(test_deps))
-- add_requires(table.unpack(bench_deps))

-- [[ Project targets ]]
target("aoc", function()
	set_kind("static")
	add_files("src/aoc/*.cpp", "src/aoc/**/*.cpp")
	add_packages(table.unpack(aoc_deps))
end)

target("aoc_cli", function()
	set_kind("binary")
	add_files("src/aoc_cli/*.cpp")
	add_packages(table.unpack(aoc_cli_deps))
	add_deps("aoc")
end)

-- target("test", function() end)
--
-- target("bench", function() end)
