// SPDX-FileCopyrightText: (C) 2022 user4223 and (other) contributors to ticket-decoder <https://github.com/user4223/ticket-decoder>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Record.h"
#include "Field.h"

#include <string>
#include <vector>
#include <optional>
#include <map>
#include <cstdint>
#include <functional>
#include <span>

namespace interpreter::detail::common
{
   class Context
   {
      using IteratorType = std::span<std::uint8_t const>::iterator;

      std::optional<std::vector<std::uint8_t>> raw;
      std::span<std::uint8_t const> data;
      IteratorType begin;
      IteratorType position;
      IteratorType end;
      std::map<std::string, Field> output;
      std::map<std::string, Record> records;

   public:
      /* Does NOT take ownership of input data
       */
      Context(std::vector<std::uint8_t> const &input, std::string origin);
      /* Does NOT take ownership of input data
       */
      Context(std::vector<std::uint8_t> const &input, Context &&otherContext);
      /* Does NOT take ownership of input data
       */
      Context(std::span<std::uint8_t const> input);
      /* Takes ownership of data
       */
      Context(std::vector<std::uint8_t> &&input);

      Context(Context const &) = delete;
      Context &operator=(Context const &) = delete;

      Context(Context &&) = default;
      Context &operator=(Context &&) = default;

      /* Returns a copy of iterator to the current position.
         Attention! The copy gets not updated when the internal position moves on.
       */
      IteratorType getPosition() const;

      /* Returns just one byte from current position
         to current position + 1 without consumtion.
         Throws runtime_error if size exceeds remaining bytes.
       */
      std::uint8_t peekByte() const;

      /* Returns just one byte from current position + offset
         to current position + offset + 1 without consumtion.
         Throws runtime_error if size exceeds remaining bytes.
       */
      std::uint8_t peekByte(std::size_t offset) const;

      /* Returns size bytes in a vector from current position
         to current position + size without consumtion.
         Throws runtime_error if size exceeds remaining bytes.
       */
      std::span<std::uint8_t const> peekBytes(std::size_t size) const;

      /* Returns size bytes in a span from current position + offset
         to current position + offset + size without consumtion.
         Throws runtime_error if offset + size exceeds remaining bytes.
       */
      std::span<std::uint8_t const> peekBytes(std::size_t offset, std::size_t size) const;

      /* Returns and consumes just one byte from current position
         to current position + 1.
       */
      std::uint8_t consumeByte();

      /* Returns and consumes size bytes from current position
         to current position + size.
         Throws runtime_error if size exceeds remaining bytes.
      */
      std::span<std::uint8_t const> consumeBytes(std::size_t size);

      /* Returns and consumes just one byte from end of all bytes.
         Throws runtime_error if size exceeds remaining bytes.
       */
      std::uint8_t consumeByteEnd();

      /* Returns and consumes size bytes from end of all bytes.
         Throws runtime_error if size exceeds remaining bytes.
      */
      std::span<std::uint8_t const> consumeBytesEnd(std::size_t size);

      /* Returns and consumes as a maximum size bytes from current position
         to current position + 0...size.
      */
      std::span<std::uint8_t const> consumeMaximalBytes(std::size_t size);

      /* Returns and consumes all remaining bytes from current
         postion to end.
       */
      std::span<std::uint8_t const> consumeRemainingBytes();

      /* Consumes and copies all remaining bytes from current
         postion to end.
       */
      std::vector<std::uint8_t> consumeRemainingBytesCopy();

      /* Consumes all remaining bytes from current postion to end,
         appends given postfix string to the end and returns a copy.
       */
      std::vector<std::uint8_t> consumeRemainingBytesAppend(std::span<std::uint8_t const> postfix);

      /* Ignores and skips size bytes from current position
         to current position + size.
       */
      std::size_t ignoreBytes(std::size_t size);

      /* Consumes expected bytes from current position when they
         match, otherwise nothing is consumed.
         Returns true when matching bytes has been consumed, false otherwise.
       */
      bool ignoreBytesIf(std::vector<std::uint8_t> expectedValue);

      /* Ignores and skips all remaining bytes from current
         position to end.
       */
      std::size_t ignoreRemainingBytes();

      /* Returns all bytes from begin to end as base64
         encoded string. This does NOT consume bytes, it just returns the
         entire buffer as base64 encoded string.
       */
      std::string getAllBase64Encoded() const;

      bool hasInput() const;

      bool hasOutput() const;

      bool isEmpty() const;

      void ensureEmpty() const;

      void ensureRemaining(std::size_t size) const;

      constexpr std::size_t getOverallSize() const
      {
         return std::distance(begin, end);
      }

      constexpr std::size_t getRemainingSize() const
      {
         return std::distance(position, end);
      }

      constexpr std::size_t getConsumedSize() const
      {
         return std::distance(begin, position);
      }

      // Fields

      std::map<std::string, Field> const &getFields() const;

      std::optional<Field> getField(std::string key) const;

      Context &ifField(std::string key, std::function<void(std::string const &)> consumer);

      Context &setField(std::string key, Field &&field);

      Context &addField(std::string key, std::string value);

      Context &addField(std::string key, std::string value, std::string description);

      Context &addField(std::string key, std::string value, std::optional<std::string> description);

      // Json

      std::optional<std::string> getJson(int indent = -1);

      // Records

      Context &addRecord(Record &&record);

      Record const &getRecord(std::string recordKey) const;

      std::map<std::string, Record> const &getRecords() const;
   };
}
