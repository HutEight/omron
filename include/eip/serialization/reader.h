/**
Software License Agreement (proprietary)

\file      reader.h
\authors   Kareem Shehata <kshehata@clearpathrobotics.com>
\copyright Copyright (c) 2015, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, is not permitted without the
express permission of Clearpath Robotics.
*/

#ifndef EIP_READER_H
#define EIP_READER_H

#include <stdexcept>
#include <boost/asio.hpp>
#include <boost/endian/conversion.hpp>

#include "eip/eip_types.h"

using boost::asio::mutable_buffer;

namespace eip {
namespace serialization {

/**
 * Generic interface for reading binary data
 */
class Reader
{
public:

  /**
   * Read a value
   * @return value read from input of type T
   * @throw std::length_error if buffer is too small to deserialize value
   */
  template <typename T> void read(T& v)
  {
    readBytes(&v, sizeof(v));
  }


  /**
   * Read a set of bytes from the input into the given buffer
   * @param buf buffer to use for reading
   * @param n number of bytes to read
   * @return pointer into the buffer
   * @throw std::length_error if the buffer is too small to contain that many bytes
   */
  virtual void readBytes(void* buf, size_t n) = 0;

  /**
   * Read input into the given buffer
   * @param buf Buffer into which to copy input
   */
  virtual void readBuffer(mutable_buffer buf) = 0;

  /**
   * Get the number of bytes read from the buffer
   * @return number of bytes read from the buffer
   */
  virtual size_t getByteCount() = 0;

  /**
   * Skip forward in input by n bytes
   * @param n number of bytes to skip forward
   * @throw std::length_error if end of output buffer reached
   */
  virtual void skip(size_t n) = 0;
};

/**
 * Specialized template to handle big endian fields in the socket address.
 */
template <> inline void Reader::read(struct sockaddr_in& v)
{
  readBytes(&v, sizeof(v));
  boost::endian::big_to_native_inplace(v.sin_family);
  //boost::endian::big_to_native_inplace(v.sin_port);
  //boost::endian::big_to_native_inplace(v.sin_addr.s_addr);
}

} // namespace serialization
} // namespace eip
#endif  // EIP_READER_H
