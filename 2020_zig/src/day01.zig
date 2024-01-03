const std = @import("std");
const parseInt = std.fmt.parseInt;
const lreaders = @import("linereader.zig");

const ARRAYLEN = 2020 / 64 + 1;

pub fn get_data(lines: anytype) ![]const u64 {
    var allo = std.heap.page_allocator;
     
    var costs = std.ArrayList(u64).init(allo);

    while (lines.get_line()) |line| {
        try costs.append(try parseInt(u64, line, 10));
    }

    return costs.toOwnedSlice();
}

pub fn part1(costs: []const u64) !u64 {

    var inv_costs: [ARRAYLEN]u64 = [_]u64{0} ** ARRAYLEN;

    for (costs) |cost| {
        const icost = 2020 - cost;
        const bin = cost / 64;
        const slot = cost % 64;

        const mask = ( @as(u64, 1) << @truncate(slot));

        if (inv_costs[bin] & mask == mask) {
            return cost * icost;
        }
        else {
            const ibin = icost / 64;
            const islot = icost % 64;
            inv_costs[ibin] |= (@as(u64, 1) << @truncate(islot));
        }

    }
    return 0;
}

pub fn part2(costs: anytype) !u64 {

    for (costs) |first| {
        var inv_costs: [ARRAYLEN]u64 = [_]u64{0} ** ARRAYLEN;
        var limit = 2020 - first;

        for (costs) |cost| {
            if (cost >= limit) continue;

            const icost = limit - cost;
            const bin = cost / 64;
            const slot = cost % 64;

            const mask = ( @as(u64, 1) << @truncate(slot));

            if (inv_costs[bin] & mask == mask) {
                std.debug.print("{} * {} * {}\n", .{cost, icost, first});
                std.debug.print("{} + {} + {} = {}\n", .{cost, icost, first, cost+icost+first});
                return cost * icost * first;
            }
            else {
                const ibin = icost / 64;
                const islot = icost % 64;
                inv_costs[ibin] |= (@as(u64, 1) << @truncate(islot));
            }
        }
    }

    return 0;
}

test "example" {

    const input = 
        \\ 1721
        \\ 979
        \\ 366
        \\ 299
        \\ 675
        \\ 1456
        ;

    try std.testing.expect(part1(lreaders.from_string(input)) == 514579);
    try std.testing.expect(part2(lreaders.from_string(input)) == 293450526);
}
