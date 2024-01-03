const std = @import("std");
const regex = @import("regex.zig");

const Password = struct {
    pos1: u8,
    pos2: u8,
    letter: u8,
    passw: []const u8,
};

pub fn get_data(lines: anytype) ![]const Password {
    var allo = std.heap.page_allocator;

    var passwords = std.ArrayList(Password).init(allo);

    var matcher = try regex.RegexMatcher.init("(\\d+)-(\\d+)\\s+(\\w):\\s+(\\w+)");

    while (lines.get_line()) |line| {
        _ = try matcher.match(line);
        const mpass = matcher.submatch_by_number(4);
        const pass = try allo.alloc(u8, mpass.len);
        @memcpy(pass, mpass);
        try passwords.append(.{
            .pos1 = try std.fmt.parseInt(u8, matcher.submatch_by_number(1), 10),
            .pos2 = try std.fmt.parseInt(u8, matcher.submatch_by_number(2), 10),
            .letter = matcher.submatch_by_number(3)[0],
            .passw = pass
        }
        );
    }

    return passwords.toOwnedSlice();
}

pub fn part1(data: []const Password) !i32 {
    var valid: i32 = 0;
    for (data) |password| {
        var count: i32 = 0;
        for (password.passw) |c| {
            if ( c == password.letter) {
                count += 1;
            }
        }
        if ((count >= password.pos1) and (count <= password.pos2)) {
            valid += 1;
        }
    }

    return valid;
}

pub fn part2(data: []const Password) !i32 {
    var valid: i32 = 0;
    for (data) |password| {
        const passw = password.passw;
        const letter = password.letter;
        const p1 = password.pos1 - 1;
        const p2 = password.pos2 - 1;
        if ((passw[p1] == letter) != (passw[p2] == letter))
            valid += 1;
    }

    return valid;
}
