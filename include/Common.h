/* Copyright (c) 2011-2012 Stanford University
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * \file
 * Common utilities and definitions.
 */

#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <memory>
#include <string>
#include <vector>

#ifndef COMMON_H
#define COMMON_H

namespace DLog {

/**
 * Construct a new object and wrap it in a unique_ptr.
 *
 * For example:
 *      unique<int>(4)
 * is the same as:
 *      std::unique_ptr<int>(new int(4))
 *
 * \tparam T
 *      The type to construct.
 * \param args
 *      Arguments to T's constructor.
 * \return
 *      A new T(args) that is wrapped in a unique_ptr<T>.
 */
template<typename T, typename... Args>
std::unique_ptr<T>
unique(Args&&... args) {
    return std::unique_ptr<T>(new T(static_cast<Args&&>(args)...));
}

/**
 * Cast a bigger int down to a smaller one.
 * Asserts that no precision is lost at runtime.
 */
// This was taken from the RAMCloud project.
template<typename Small, typename Large>
Small
downCast(const Large& large)
{
    Small small = static_cast<Small>(large);
    // The following comparison (rather than "large==small") allows
    // this method to convert between signed and unsigned values.
    assert(large - small == 0);
    return small;
}

/**
 * Sort an R-value in place.
 * \param container
 *      An R-value to sort.
 * \return
 *      The sorted input.
 */
template<typename Container>
Container
sorted(Container&& container)
{
    std::sort(container.begin(), container.end());
    return container;
}

/**
 * Return a copy of the keys of a map.
 */
template<typename Map>
std::vector<typename Map::key_type>
getKeys(const Map& map)
{
    std::vector<typename Map::key_type> keys;
    for (auto it = map.begin(); it != map.end(); ++it)
        keys.push_back(it->first);
    return keys;
}

/**
 * Return a copy of the values of a map.
 */
template<typename Map>
std::vector<typename Map::mapped_type>
getValues(const Map& map)
{
    std::vector<typename Map::mapped_type> values;
    for (auto it = map.begin(); it != map.end(); ++it)
        values.push_back(it->second);
    return values;
}

/**
 * Return a copy of the key-value pairs of a map.
 */
template<typename Map>
std::vector<std::pair<typename Map::key_type,
                      typename Map::mapped_type>>
getItems(const Map& map)
{
    std::vector<std::pair<typename Map::key_type,
                          typename Map::mapped_type>> items;
    for (auto it = map.begin(); it != map.end(); ++it)
        items.push_back(*it);
    return items;
}

/**
 * Return true if all elements of 'haystack' are equal to 'needle'.
 */
template<typename Container, typename Item>
bool
hasOnly(const Container& haystack, const Item& needle) {
    for (auto it = haystack.begin(); it != haystack.end(); ++it) {
        if (*it != needle)
            return false;
    }
    return true;
}

/**
 * A safe version of sprintf.
 */
std::string format(const char* format, ...)
    __attribute__((format(printf, 1, 2)));

/**
 * Determine whether a null-terminated string is printable.
 * \param str
 *      A null-terminated string.
 * \return
 *      True if all the bytes of str before its null terminator are nice to
 *      display in a single line of text.
 */
bool
isPrintable(const char* str);

/**
 * Determine whether some data is a printable, null-terminated string.
 * \param data
 *      The first byte.
 * \param length
 *      The number of bytes of 'data'.
 * \return
 *      True if the last byte of data is a null terminator and all the bytes of
 *      data before that are nice to display in a single line of text.
 */
bool
isPrintable(const void* data, size_t length);

/**
 * For strings, replace all occurrences of 'needle' in 'haystack' with
 * 'replacement'.
 *
 * If this isn't what you're looking for, the standard algorithm std::replace
 * might help you.
 */
void
replaceAll(std::string& haystack,
           const std::string& needle,
           const std::string& replacement);

/// Like sizeof but returns a uint32_t.
#define sizeof32(x) DLog::downCast<uint32_t>(sizeof(x))

} // namespace DLog

#endif /* COMMON_H */
