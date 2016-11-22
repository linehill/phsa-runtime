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
 * @author tomi.aijo@parmance.com and pekka.jaaskelainen@parmance.com
 *         for General Processor Tech.
 */

#include "Queue.hh"
#include "Agent.hh"

namespace phsa {

std::unordered_map<const hsa_queue_t *, Queue *> Queue::Registry;
boost::shared_mutex Queue::RegistryLock;

std::atomic<uint64_t> Queue::QueueCount{0};

void Queue::garbageCollect() {
  for (auto &kv : Registry) {
    delete kv.second;
  }

  boost::lock_guard<boost::shared_mutex> L(RegistryLock);
  Registry.clear();
}

void Queue::setInactivated() {
  Inactivated = true;
  // Soft queues does not have owner
  if (Owner != nullptr)
    Owner->terminateQueue(this);
}

Queue *Queue::FindQueue(const hsa_queue_t *HSAQueue) {
  return (Queue *)HSAQueue->id;
}

}
