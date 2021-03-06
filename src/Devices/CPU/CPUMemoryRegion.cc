/*
    Copyright (c) 2016 General Processor Tech.
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/
/**
 * @author tomi.aijo@parmance.com for General Processor Tech.
 */

#include "CPUMemoryRegion.hh"

namespace phsa {

CPUMemoryRegion::CPUMemoryRegion(hsa_region_segment_t Segment)
    : RegionSegment(Segment) {}

void *CPUMemoryRegion::allocate(std::size_t Size, std::size_t Align) {
  void *Ptr = nullptr;
  std::lock_guard<std::mutex> Guard(AllocationsLock);
  if (Align < sizeof(void *))
    Ptr = malloc(Size);
  else if (posix_memalign(&Ptr, Align, Size) != 0)
    return nullptr;
  Allocations.insert(Ptr);
  return Ptr;
}

bool CPUMemoryRegion::free(void *Ptr) {
  std::lock_guard<std::mutex> Guard(AllocationsLock);
  if (Allocations.count(Ptr) > 0) {
    std::free(Ptr);
    Allocations.erase(Ptr);
    return true;
  } else {
    return false;
  }
}


}
