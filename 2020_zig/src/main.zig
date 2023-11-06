const std = @import("std");
const lreaders = @import("linereader.zig");
const day01 = @import("day01.zig");
const day02 = @import("day02.zig");


const stdout_file = std.io.getStdOut().writer();
var bw = std.io.bufferedWriter(stdout_file);
const stdout = bw.writer();

pub fn main() !void {
    var lines = try lreaders.open_file("input02.txt");
    defer lines.deinit();
    const data = try day02.get_data(&lines);

    try stdout.print("answer 1: {}\n", .{try day02.part1(data)});
    try stdout.print("answer 2: {}\n", .{try day02.part2(data)});
    try bw.flush();
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit(); // try commenting this out and see if zig detects the memory leak!
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}
