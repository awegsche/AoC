const std = @import("std");

const MAX_LINE_LENGTH = 1024;
const MAX_FILE_SIZE = 10 * 1024;


pub fn open_file(path: []const u8) !LineReader(FileReader) {
    return lineReader(try FileReader.init(path));
}

pub fn from_string(string: []const u8) !LineReader(StringReader) {
    return lineReader(try StringReader.init(string));
}

/// constructs a new line reader from the given underlying reader (either a StringReader or a FileReader)
///
/// Pulls out lines from an underlying reader (StringReader for in-momory strings,
/// FileReader for files from disk).
/// The lines are provided one after another, temporarily written into a buffer.
/// Note: getting a line overwrites the contents of the last line. If you want the lines in a vector
/// you must copy them out manually
pub fn lineReader(inner: anytype) LineReader(@TypeOf(inner)) {
    return .{ .reader = inner };
}

/// LineReader class.
pub fn LineReader(comptime T: type) type {
    return struct {
        reader: T,
        line_buffer: [1024]u8 = undefined,

        const Self = @This();

        pub fn init(inner: T) Self {
            return .{ .reader = inner, };
        }

        pub fn get_line(self: *Self) ?[]const u8 {
            var out_stream = std.io.fixedBufferStream(&self.line_buffer);
            self.reader.get_stream().streamUntilDelimiter(out_stream.writer(), '\n', null) catch |err| {
                if (err == error.EndOfStream) {
                    return null;
                }
            };
            return self.line_buffer[0..out_stream.pos];
        }

        pub fn deinit(self: *Self) void {
            self.reader.deinit();
        }
    };
}

pub const FileReader = struct {
    const Self = @This();
    const BReader = std.io.BufferedReader(4096, std.fs.File.Reader);

    file: std.fs.File,
    buf_reader: BReader,

    pub fn init(path: []const u8) !Self {
        var file = try std.fs.cwd().openFile(path, .{});
        var breader = std.io.bufferedReader(file.reader());
        return .{
            .file = file,
            .buf_reader = breader,
        };
    }

    pub fn get_stream(self: *Self) BReader.Reader {
        return self.buf_reader.reader();
    }

    pub fn deinit(self: *Self) void {
        self.file.close();
    }
};

pub const StringReader = struct {
    const Self = @This();
    const Buffer = std.io.FixedBufferStream([]u8);

    buffer: [] u8,
    stream: Buffer,

    pub fn init(text: []const u8) !Self {
        const buffer = try std.heap.page_allocator.alloc(u8, text.len);
        @memcpy(buffer, text);
        const stream = std.io.fixedBufferStream(buffer);
         
        return .{
            .buffer = buffer,
            .stream = stream
        };
    }

    pub fn get_stream(self: *Self) Buffer.Reader {
        return self.stream.reader();
    }

    pub fn deinit(self: *Self) void {
        _ = self;
    }
};

