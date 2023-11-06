const std = @import("std");
const pcre2 = @cImport({
    @cDefine("PCRE2_CODE_UNIT_WIDTH", "8");
    @cInclude("pcre2.h");
});

pub const RegexInitError = error {
    CompileError,
};

pub const RegexError = struct {
    code: c_int = 0,
    offset: pcre2.PCRE2_SIZE = 0,
};


pub const RegexReturn = union(enum) {
    regex: Regex,
    err_message: RegexError,
};

pub const Regex = struct {
    regex: ?*pcre2.pcre2_code_8 = null,

    const Self = @This();

    pub fn init(pattern: []const u8) RegexReturn {
        var _error: RegexError = .{};
        const regex = pcre2.pcre2_compile_8(
            &pattern[0], pattern.len,
            0,
            &_error.code, &_error.offset, null);

        if (regex == null) {
            return .{ .err_message = _error };
        }

        return .{ .regex = Self{.regex = regex}};
    }

    pub fn expect_init(pattern: []const u8) RegexInitError!Self {
        switch (Self.init(pattern)) {
            .regex => |regex| return regex,
            .err_message => |err| {
                var buf: [1024]u8 = undefined;
                _ = pcre2.pcre2_get_error_message_8(err.code, &buf, err.offset);
                std.debug.print("{s}\n{s}\n", .{buf, pattern});
                for (0..err.offset-1) |_|{
                    std.debug.print(" ", .{});
                }
                std.debug.print("^\n", .{});
                return RegexInitError.CompileError;
            }
        }
    }

    pub fn deinit(self: *Self) void {
        if (self.regex != null) {
            pcre2.pcre2_code_free_8(&self.regex);
        }
    }
};

pub const RegexMatcher = struct {
    regex: Regex = undefined,
    match_block: ?*pcre2.pcre2_match_data_8 = null,
    // we want to own the input
    input: std.ArrayList(u8) = undefined,

    const Self = @This();

    pub fn init(pattern: []const u8) RegexInitError!Self {
        const rg = try Regex.expect_init(pattern);
        const mb = pcre2.pcre2_match_data_create_from_pattern_8(rg.regex, null);
        var allo = std.heap.page_allocator;

        return .{ .regex=rg, .match_block=mb, .input = std.ArrayList(u8).init(allo) };
    }

    pub fn match(self: *Self, input: []const u8) !i32 {
        // we want to own the input so that we can return substrings as slices
        try self.input.resize(input.len);
        @memcpy(self.input.items, input);

        return @as(i32, pcre2.pcre2_match_8(
            self.regex.regex,
            &self.input.items[0],
            self.input.items.len,
            0,
            0,
            self.match_block,
            null,
        ));
    }

    pub fn match_count(self: Self) u32 {
        return @as(u32, pcre2.pcre2_get_ovector_count_8(self.match_block));
    }

    pub fn submatch_by_number(self: Self, number: usize) []const u8 {
        const ovector = pcre2.pcre2_get_ovector_pointer_8(self.match_block);
        return self.input.items[ovector[number*2]..ovector[number*2+1]];
    }

    pub fn deinit(self: *Self) void {
        self.regex.deinit();
        if (self.match_block != null) {
            pcre2.pcre2_match_block_free_8(self.match_block);
        }
    }
};

