const std = @import("std");

const BuildMode = enum {
    Debug,
    ReleaseFast,
};

pub fn build(b: *std.build.Builder) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const mode = switch (optimize) {
        .Debug => BuildMode.Debug,
        else => BuildMode.ReleaseFast,
    };

    const exe = b.addExecutable("helper", "src/helper.cpp");
    exe.setBuildMode(mode);
    exe.setTarget(target);
    exe.addSourceFile("src/index.cpp");
    exe.addSourceFile("src/json.cpp");
    exe.addSourceFile("src/search.cpp");
    exe.addSourceFile("src/signal.cpp");
    exe.addSourceFile("src/usage.cpp");
    exe.addSourceFile("src/version.cpp");
    exe.addSourceFile("src/data.cpp");
    exe.addSourceFile("src/web/web.cpp");

    exe.linkLibC(); // Link against libc

    exe.addCSourceFlags(&[_][]const u8{
        "-std=c++17",
        "-Wall",
        "-Wno-unused-result",
        "-fstack-clash-protection",
        "-fstack-protector-all",
        "-pipe",
    });

    exe.addLinkFlags(&[_][]const u8{
        "-ffunction-sections",
        "-fdata-sections",
        "-z", "now",
        "-z", "noexecstack",
        "-fPIE",
        "-flto",
    });

    // Optional support for ccache
    if (std.fs.cwd().fileExists("ccache")) {
        exe.addCSourceFlags(&[_][]const u8{"-DUSE_CCACHE"});
    }

    // Set preprocessor definitions based on options
    const enable_webui = b.option(bool, "enable-webui", "Enable WebUI (Web User Interface)").get(false);
    if (enable_webui) {
        exe.addDefine("APPUI", "WEBUI");
        exe.addDefine("WEBUI", null);
    } else {
        exe.addDefine("APPUI", "TUI");
        exe.addDefine("TUI", null);
    }

    // Install target to /usr/local/bin
    exe.install().setInstallPath("/usr/local/bin/helper");

    // Add optional build testing
    const enable_tests = b.option(bool, "build-test", "Whether to build tests").get(true);
    if (enable_tests and !enable_webui) {
        const test_dir = b.addTest("test");
        test_dir.setBuildMode(mode);
        test_dir.setTarget(target);
        b.installArtifact(test_dir);
    } else if (enable_tests and enable_webui) {
        std.debug.warn("Please disable WebUI to build tests.\n", .{});
    }

    b.defaultStep.install();
}

