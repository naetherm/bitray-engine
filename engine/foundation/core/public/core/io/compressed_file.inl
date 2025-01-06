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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/memory/memory.h"
#include "core/tools/compression.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
template<typename TCompression>
CompressedFile<TCompression>::CompressedFile()
  : mNumberOfDecompressedBytes(0)
  , mCurrentDataPointer(nullptr) {

}

template<typename TCompression>
CompressedFile<TCompression>::CompressedFile(
  core::sizeT reservedNumberOfCompressedBytes,
  core::sizeT reservedNumberOfDecompressedBytes)
  : mNumberOfDecompressedBytes(0)
  , mCurrentDataPointer(nullptr) {
  mCompressedData.reserve(reservedNumberOfCompressedBytes);
  mDecompressedData.reserve(reservedNumberOfDecompressedBytes);
}

template<typename TCompression>
CompressedFile<TCompression>::~CompressedFile() {

}


template<typename TCompression>
const typename CompressedFile<TCompression>::ByteVector& CompressedFile<TCompression>::get_byte_vector() const {
  return mDecompressedData;
}

template<typename TCompression>
typename CompressedFile<TCompression>::ByteVector& CompressedFile<TCompression>::get_byte_vector() {
  return mDecompressedData;
}


template<typename TCompression>
void CompressedFile<TCompression>::decompress() {
  mDecompressedData.resize(mNumberOfDecompressedBytes);
  [[maybe_unused]] const int numberOfDecompressedBytes = mCompression.decompress(
    mDecompressedData.data(),
    mCompressedData.data(),
    mCompressedData.size(),
    mNumberOfDecompressedBytes
  );

  BE_ASSERT(mNumberOfDecompressedBytes == static_cast<uint32>(numberOfDecompressedBytes), "Invalid number of decompressed bytes")
  mCurrentDataPointer = mDecompressedData.data();
}

template<typename TCompression>
bool CompressedFile<TCompression>::load_compressed_data_from_file(
  uint32 formatType,
  uint32 formatVersion,
  FileImpl &file
) {
  CompressedFileFormatHeader fileFormatHeader;
  file.read(&fileFormatHeader, sizeof(CompressedFileFormatHeader));
  if (formatType == fileFormatHeader.formatType && formatVersion == fileFormatHeader.formatVersion)
  {
    // Tell the memory mapped file about the LZ4 compressed data
    set_compressed_data(
      file,
      fileFormatHeader.numberOfCompressedBytes,
      fileFormatHeader.numberOfDecompressedBytes);

    // Done
    return true;
  }

  else
  {
    // Error!
    BE_ASSERT(false, "Invalid file format header")
    return false;
  }

}

template<typename TCompression>
void CompressedFile<TCompression>::set_compressed_data(
  FileImpl &file,
  uint32 numberOfCompressedBytes,
  uint32 numberOfDecompressedBytes
) {
  // Sanity checks
  BE_ASSERT(0 != numberOfCompressedBytes, "Zero LZ4 compressed bytes are invalid")
  BE_ASSERT(0 != numberOfDecompressedBytes, "Zero LZ4 decompressed bytes are invalid")

  // Read data
  mNumberOfDecompressedBytes = numberOfDecompressedBytes;
  mDecompressedData.clear();
  mCurrentDataPointer = nullptr;
  mCompressedData.resize(numberOfCompressedBytes);
  file.read(mCompressedData.data(), numberOfCompressedBytes);
}

template<typename TCompression>
bool CompressedFile<TCompression>::write_compressed_data_to_file(
  uint32 formatType,
  uint32 formatVersion,
  FileImpl &file
) {
  // Write file
  const int destinationCapacity = mCompression.compress_bound(static_cast<int>(mDecompressedData.size()));
  core::uint8* destination = new core::uint8[static_cast<unsigned int>(destinationCapacity)];
  {
    const int numberOfWrittenBytes = mCompression.compress(
      destination,
      mDecompressedData.data(),
      mDecompressedData.size(),
      destinationCapacity
    );

    { // Write down the file format header
      CompressedFileFormatHeader fileFormatHeader;
      fileFormatHeader.formatType				   = formatType;
      fileFormatHeader.formatVersion			   = formatVersion;
      fileFormatHeader.numberOfCompressedBytes   = static_cast<uint32>(numberOfWrittenBytes);
      fileFormatHeader.numberOfDecompressedBytes = static_cast<uint32>(mDecompressedData.size());
      file.write(&fileFormatHeader, sizeof(CompressedFileFormatHeader));
    }

    // Write down the compressed data
    file.write(destination, static_cast<size_t>(numberOfWrittenBytes));
  }
  delete [] destination;

  // Done
  return true;
}


template<typename TCompression>
sizeT CompressedFile<TCompression>::get_number_of_bytes() {
  return mDecompressedData.size();
}

template<typename TCompression>
uint64 CompressedFile<TCompression>::read(void *destinationBuffer, sizeT numberOfBytes) {
  BE_ASSERT(nullptr != destinationBuffer, "Letting a file read into a null destination buffer is not allowed")
  BE_ASSERT(0 != numberOfBytes, "Letting a file read zero bytes is not allowed")
  BE_ASSERT((mCurrentDataPointer - mDecompressedData.data()) + numberOfBytes <= mDecompressedData.size(), "Invalid number of bytes")
  core::Memory::copy(destinationBuffer, mCurrentDataPointer, numberOfBytes);
  mCurrentDataPointer += numberOfBytes;
  return numberOfBytes;
}

template<typename TCompression>
void CompressedFile<TCompression>::skip(sizeT numberOfBytes) {
  BE_ASSERT(0 != numberOfBytes, "Letting a file skip zero bytes is not allowed")
  BE_ASSERT((mCurrentDataPointer - mDecompressedData.data()) + numberOfBytes <= mDecompressedData.size(), "Invalid number of bytes")
  mCurrentDataPointer += numberOfBytes;
}

template<typename TCompression>
uint64 CompressedFile<TCompression>::write(const void *sourceBuffer, sizeT numberOfBytes) {
  BE_ASSERT(nullptr != sourceBuffer, "Letting a file write from a null source buffer is not allowed")
  BE_ASSERT(0 != numberOfBytes, "Letting a file write zero bytes is not allowed")
  std::copy(
    static_cast<const core::uint8 *>(sourceBuffer),
    static_cast<const core::uint8 *>(sourceBuffer) + numberOfBytes,
    std::back_inserter(mDecompressedData));
  return numberOfBytes;
}

template<typename TCompression>
int64 CompressedFile<TCompression>::get_last_modification_time() const {
  return 0;
}

template<typename TCompression>
int64 CompressedFile<TCompression>::get_file_size() const {
  return mDecompressedData.size();
}

template<typename TCompression>
String CompressedFile<TCompression>::get_str() {
  return "";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}