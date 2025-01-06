////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 RacoonStudios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/core.h"
#include "core/container/vector.h"
#include "core/io/file_impl.h"
#include "core/io/types.h"
#include "core/tools/compression_lz4.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
 * @class
 * CompressedFile
 *
 * @brief
 * A class for handling compressed files, providing methods for compression and decompression
 * of file data using a specified compression algorithm.
 *
 * The `CompressedFile` class supports reading, writing, and decompressing files while managing
 * both compressed and decompressed states. It provides functionality to handle files with a
 * specific format and compression type.
 *
 * @tparam TCompression
 * The compression algorithm to be used for compressing and decompressing file data.
 */
template<typename TCompression>
class CompressedFile : public FileImpl {

  /**
   * @brief
   * Structure for holding the compressed file format header information.
   *
   * This structure stores metadata such as the format type, format version, and the number of
   * compressed and decompressed bytes.
   */
  struct CompressedFileFormatHeader {
    core::int32 formatType;           /**< Type of the compression format */
    core::int32 formatVersion;        /**< Version of the compression format */
    core::int32 numberOfCompressedBytes; /**< Number of bytes in the compressed state */
    core::int32 numberOfDecompressedBytes; /**< Number of bytes in the decompressed state */
  };

public:
  typedef Vector<uint8> ByteVector; /**< Type definition for the byte vector representing the data */

public:
  /**
   * @brief
   * Default constructor.
   *
   * Initializes an empty compressed file with no reserved data.
   */
  CompressedFile();

  /**
   * @brief
   * Constructor with reserved space.
   *
   * Initializes the compressed file with reserved space for the specified number of compressed
   * and decompressed bytes.
   *
   * @param[in] reservedNumberOfCompressedBytes
   * Number of bytes to reserve for the compressed data.
   *
   * @param[in] reservedNumberOfDecompressedBytes
   * Number of bytes to reserve for the decompressed data.
   */
  CompressedFile(
    sizeT reservedNumberOfCompressedBytes,
    sizeT reservedNumberOfDecompressedBytes);

  /**
   * @brief
   * Destructor.
   *
   * Cleans up resources used by the compressed file.
   */
  ~CompressedFile() override;


  /**
   * @brief
   * Retrieves the byte vector containing the compressed data.
   *
   * @return
   * A constant reference to the byte vector representing the compressed data.
   */
  const ByteVector& get_byte_vector() const;

  /**
   * @brief
   * Retrieves the byte vector containing the compressed data.
   *
   * @return
   * A reference to the byte vector representing the compressed data.
   */
  ByteVector& get_byte_vector();


  /**
   * @brief
   * Decompresses the compressed data into the decompressed byte vector.
   *
   * This method decompresses the data stored in the compressed state and loads it into the
   * decompressed data vector.
   */
  void decompress();

  /**
   * @brief
   * Loads compressed data from a file and updates the file's format information.
   *
   * @param[in] formatType
   * The format type of the compressed data.
   *
   * @param[in] formatVersion
   * The version of the compressed data format.
   *
   * @param[in] file
   * The file from which to load the compressed data.
   *
   * @return
   * "true" if data was successfully loaded, "false" otherwise.
   */
  bool load_compressed_data_from_file(
    uint32 formatType,
    uint32 formatVersion,
    FileImpl &file);

  /**
   * @brief
   * Sets the compressed data from the file.
   *
   * @param[in] file
   * The file from which to load the compressed data.
   *
   * @param[in] numberOfCompressedBytes
   * The number of compressed bytes in the file.
   *
   * @param[in] numberOfDecompressedBytes
   * The number of decompressed bytes the data will have after decompression.
   */
  void set_compressed_data(
    FileImpl &file,
    uint32 numberOfCompressedBytes,
    uint32 numberOfDecompressedBytes);

  /**
   * @brief
   * Writes the compressed data to a file with the specified format information.
   *
   * @param[in] formatType
   * The format type of the compressed data.
   *
   * @param[in] formatVersion
   * The version of the compressed data format.
   *
   * @param[in] file
   * The file to which the compressed data will be written.
   *
   * @return
   * "true" if data was successfully written, "false" otherwise.
   */
  bool write_compressed_data_to_file(
    uint32 formatType,
    uint32 formatVersion,
    FileImpl &file);

public:

  /**
   * @brief
   * Returns the number of bytes currently used in the decompressed state.
   *
   * @return
   * Number of bytes currently used in the decompressed state.
   *
   * This function returns the size of the data after decompression, which may differ from the
   * size of the compressed data.
   */
  sizeT get_number_of_bytes() override;

  /**
   * @brief
   * Reads a requested number of bytes from the file.
   *
   * @param[out] destinationBuffer
   * Destination buffer to write the data into. This buffer must be large enough to hold the
   * specified number of bytes.
   *
   * @param[in] numberOfBytes
   * The number of bytes to read from the file.
   *
   * @return
   * The number of bytes actually read from the file.
   */
  virtual uint64 read(void *destinationBuffer, uint64 numberOfBytes) override;

  /**
   * @brief
   * Skips a requested number of bytes in the file.
   *
   * @param[in] numberOfBytes
   * The number of bytes to skip.
   *
   * This function advances the read position in the file by the specified number of bytes.
   */
  virtual void skip(uint64 numberOfBytes) override;

  //[-------------------------------------------------------]
  //[ Write                                                 ]
  //[-------------------------------------------------------]

  /**
   * @brief
   * Writes a requested number of bytes into the file.
   *
   * @param[in] sourceBuffer
   * Source buffer from which to read the data. This buffer must be at least as large as
   * the specified number of bytes.
   *
   * @param[in] numberOfBytes
   * The number of bytes to write to the file.
   *
   * @return
   * The number of bytes actually written to the file.
   */
  virtual uint64 write(const void *sourceBuffer, uint64 numberOfBytes) override;

  /**
   * @brief
   * Gets the last modification time of the file.
   *
   * @return
   * The last modification time of the file.
   */
  [[nodiscard]] virtual core::int64 get_last_modification_time() const override;

  /**
   * @brief
   * Gets the file size.
   *
   * @return
   * The size of the file in bytes. Returns -1 if the size cannot be determined.
   */
  [[nodiscard]] virtual core::int64 get_file_size() const override;

  /**
   * @brief
   * Returns a string representation of the file.
   *
   * @return
   * A string representation of the file.
   */
  virtual String get_str() override;

protected:
  /** Instance of the compression algorithm to use */
  TCompression mCompression;
  /** The compressed data */
  ByteVector mCompressedData;
  /** The decompressed data */
  ByteVector mDecompressedData;
  /** Number of bytes in the decompressed state */
  uint32 mNumberOfDecompressedBytes;
  /** Pointer to the current read/write position in the data */
  uint8* mCurrentDataPointer;
};


typedef CompressedFile<CompressionLZ4> CompressedFileLZ4;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "core/io/compressed_file.inl"