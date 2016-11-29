#ifndef __STATICJSONBUFFERMODIFY_HPP__
#define __STATICJSONBUFFERMODIFY_HPP__

namespace ArduinoJson {

template <size_t CAPACITY>
class StaticJsonBufferModify : public JsonBuffer {
 public:
  explicit StaticJsonBufferModify() : _size(0) {}

  size_t capacity() const {
    return CAPACITY;
  }
  size_t size() const {
    return _size;
  }
  
  void reset() {
    _size = 0;
  }

  virtual void* alloc(size_t bytes) {
    if (_size + bytes > CAPACITY) return NULL;
    void* p = &_buffer[_size];
    _size += round_size_up(bytes);
    return p;
  }

 private:
  uint8_t _buffer[CAPACITY];
  size_t _size;
};
}


#endif