﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AoC2024
{
    internal class Day05 : AoCSolution
    {
        public override string Day => "05";

        public override string Title => "Print Queue";

        public void Load(IEnumerable<string> input, Logger logger)
        {
            using var lines = input.GetEnumerator();

            var rules = new List<(int, int)>();

            while (lines.MoveNext())
            {
                if (lines.Current.Length == 0) break;

                var ints = lines.Current.Split('|');

                rules.Add((int.Parse(ints[0]), int.Parse(ints[1])));
            }

            _rules = rules.ToArray();

            var page_updates = new List<int[]>();
            while (lines.MoveNext())
            {
                page_updates.Add(lines.Current.Split(',').Select(i => int.Parse(i)).ToArray());
            }
            _page_updates = page_updates.ToArray();
        }

        public override string part1(IEnumerable<string> input, Logger logger)
        {
            Load(input, logger);

            bool is_valid(int[] page_updates)
            {
                var occured = new HashSet<int>();
                foreach (var i in page_updates)
                {
                    foreach (var rule in _rules.Where((b, _) => b.Item1 == i))
                    {
                        if (occured.Contains(rule.Item2)) return false;
                    }
                    occured.Add(i);
                }

                //logger.Info("valid: " + string.Join(", ", page_updates.Select(p => p.ToString())));
                return true;
            }

            return _page_updates.Where(page_update => is_valid(page_update)).Select(page_update => page_update[page_update.Length / 2]).Sum().ToString();
        }

        public override string part2(IEnumerable<string> input, Logger logger)
        {
            Load(input, logger);

            bool is_valid(int[] page_updates)
            {
                var occured = new HashSet<int>();
                foreach (var i in page_updates)
                {
                    foreach (var rule in _rules.Where((b, _) => b.Item1 == i))
                    {
                        if (occured.Contains(rule.Item2)) return false;
                    }
                    occured.Add(i);
                }

                //logger.Info("valid: " + string.Join(", ", page_updates.Select(p => p.ToString())));
                return true;
            }

            int[] make_valid(int[] input)
            {
                logger.Debug($"invalid: {string.Join(',', input.Select(x => x.ToString()).ToArray())}");
                var page_updates = input.ToArray();

                void apply_rule((int, int) rule, ref int[] pages)
                {
                    int first = -1;
                    int second = -1;

                    for (var i = 0; i < pages.Length; i++)
                    {
                        if (pages[i] == rule.Item1 && second != -1)
                        {
                            logger.Debug($">> rule: {rule.Item1} | {rule.Item2}");
                            first = i;
                            for (var idx = first; idx > second; idx--)
                            {
                                pages[idx] = pages[idx - 1];
                            }
                            pages[second] = rule.Item1;

                            logger.Debug($"> {string.Join(',', pages.Select(x => x.ToString()).ToArray())}");
                            break;
                        }
                        if (pages[i] == rule.Item2)
                        {
                            second = i;
                        }
                    }

                }
                foreach (var rule in _rules)
                {
                    apply_rule(rule, ref page_updates);
                }
                foreach (var rule in _rules.Reverse())
                {
                    apply_rule(rule, ref page_updates);
                }
                logger.Debug($"validated: {string.Join(',', page_updates.Select(x => x.ToString()).ToArray())}");
                logger.Debug("----");
                return page_updates;
            }

            return _page_updates.Where(page_update => !is_valid(page_update))
                .Select(page_update => make_valid(page_update))
                .Select(page_update => page_update[page_update.Length / 2]).Sum().ToString();
        }

        public override TestCase[] tests()
        {
            return [
                new TestCase(
                    "47|53\n97|13\n97|61\n97|47\n75|29\n61|13\n75|53\n29|13\n97|29\n53|29\n61|53\n97|53\n61|29\n47|13\n75|47\n97|75\n47|61\n75|61\n47|29\n75|13\n53|13\n\n75,47,61,53,29\n97,61,53,29,13\n75,29,13\n75,97,47,61,53\n61,13,29\n97,13,75,29,47",
                    "143",
                    "123")
                ];
        }

        private (int, int)[] _rules = [];
        private int[][] _page_updates = [];
    }
}
