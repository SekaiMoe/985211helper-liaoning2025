const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "helper",
        .target = target,
        .optimize = optimize,
    });

    exe.addCSourceFile(.{ .file = "src/helper.cpp", .flags = &.{}}); // Updated
    exe.addCSourceFile(.{ .file = "src/index.cpp", .flags = &.{}}); // Updated
    exe.addCSourceFile(.{ .file = "src/json.cpp", .flags = &.{}}); // Updated
    exe.addCSourceFile(.{ .file = "src/search.cpp", .flags = &.{}}); // Updated
    exe.addCSourceFile(.{ .file = "src/signal.cpp", .flags = &.{}}); // Updated
    exe.addCSourceFile(.{ .file = "src/usage.cpp", .flags = &.{}}); // Updated
    exe.addCSourceFile(.{ .file = "src/version.cpp", .flags = &.{}}); // Updated
    exe.addCSourceFile(.{ .file = "src/data.cpp", .flags = &.{}}); // Updated
    exe.addCSourceFile(.{ .file = "src/web/web.cpp", .flags = &.{}}); // Updated


    // Set C++ standard
    exe.addCxxFlags(&.{"-std=c++17"});

    // Add compile options
    exe.addCxxFlags(&.{
        "-Wno-unused-result",
        "-Wall",
        "-fstack-clash-protection",
        "-fstack-protector-all",
        "-pipe",
    });

    // Add include directory
    exe.addIncludePath(.{ .path = "include" });

    // Define build date and time
    const build_date = b.fmt("{d}-{d:0>2}-{d:0>2}", .{
        std.time.timestamp() / (24 * 60 * 60) / 365 + 1970,
        @mod(std.time.timestamp() / (24 * 60 * 60) / 30 + 1, 12) + 1,
        @mod(std.time.timestamp() / (24 * 60 * 60), 31) + 1,
    });
    const build_time = b.fmt("{d:0>2}:{d:0>2}:{d:0>2}", .{
        @mod(std.time.timestamp() / (60 * 60), 24),
        @mod(std.time.timestamp() / 60, 60),
        @mod(std.time.timestamp(), 60),
    });
    exe.defineCMacro("BUILD_DATE", build_date);
    exe.defineCMacro("BUILD_TIME", build_time);

    // Build options
    const static_build = b.option(bool, "static", "Enable static building") orelse false;
    const debug_mode = b.option(bool, "debug", "Enable debugging") orelse false;
    const appui_webui = b.option(bool, "webui", "Enable WEBUI (Web User Interface)") orelse false;

    if (static_build) {
        exe.linkage = .static;
    }

    if (debug_mode) {
        exe.optimize = .Debug;
        exe.addCxxFlags(&.{
            "-Wpedantic", "-pedantic",
            "-Wextra",
            "-Wredundant-decls", "-Wlogical-op",
            "-Wstrict-overflow=5", "-Winit-self",
            "-Wuninitialized", "-Wsign-conversion",
            "-Wcast-qual",
        });
    } else {
        exe.optimize = .ReleaseSafe;
        exe.strip = true;
        exe.addCxxFlags(&.{
            "-D_FILE_OFFSET_BITS=64",
            "-D_LARGEFILE_SOURCE",
            "-D_REENTRANT",
        });
    }

    if (appui_webui) {
        exe.defineCMacro("APPUI", "WEBUI");
        exe.defineCMacro("WEBUI", "");
    } else {
        exe.defineCMacro("APPUI", "TUI");
        exe.defineCMacro("TUI", "");
    }

    // Install
    b.installArtifact(exe);

    // Add test step
    const run_unit_tests = b.addTest(.{
        .root_source_file = .{ .path = "test/main.zig" },
        .target = target,
        .optimize = optimize,
    });

    const run_unit_tests_step = b.step("test", "Run unit tests");
    run_unit_tests_step.dependOn(&run_unit_tests.step);
}
