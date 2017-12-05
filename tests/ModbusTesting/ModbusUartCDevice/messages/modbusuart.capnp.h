// Generated by Cap'n Proto compiler, DO NOT EDIT
// source: modbusuart.capnp

#ifndef CAPNP_INCLUDED_882e1c0f1af44d05_
#define CAPNP_INCLUDED_882e1c0f1af44d05_

#include <capnp/generated-header-support.h>

#if CAPNP_VERSION != 5003
#error "Version mismatch between generated code and library headers.  You must use the same version of the Cap'n Proto compiler and library."
#endif


namespace capnp {
namespace schemas {

CAPNP_DECLARE_SCHEMA(92fec1d990520ff7);
enum class ModbusCommands_92fec1d990520ff7: uint16_t {
  NO_CMD,
  READ_COIL_BITS,
  READ_INPUT_BITS,
  READ_INPUT_REGS,
  READ_HOLDING_REGS,
  WRITE_COIL_BIT,
  WRITE_HOLDING_REG,
  WRITE_COIL_BITS,
  WRITE_MULTI_HOLDING_REGS,
  WRITE_READ_HOLDING_REGS,
};
CAPNP_DECLARE_ENUM(ModbusCommands, 92fec1d990520ff7);
CAPNP_DECLARE_SCHEMA(bdf4d5da8a8adbdf);
CAPNP_DECLARE_SCHEMA(8a589e3f31c18b5b);

}  // namespace schemas
}  // namespace capnp

namespace riapsModbusUART {

typedef ::capnp::schemas::ModbusCommands_92fec1d990520ff7 ModbusCommands;

struct CommandFormat {
  CommandFormat() = delete;

  class Reader;
  class Builder;
  class Pipeline;

  struct _capnpPrivate {
    CAPNP_DECLARE_STRUCT_HEADER(bdf4d5da8a8adbdf, 2, 1)
    #if !CAPNP_LITE
    static constexpr ::capnp::_::RawBrandedSchema const* brand = &schema->defaultBrand;
    #endif  // !CAPNP_LITE
  };
};

struct ResponseFormat {
  ResponseFormat() = delete;

  class Reader;
  class Builder;
  class Pipeline;

  struct _capnpPrivate {
    CAPNP_DECLARE_STRUCT_HEADER(8a589e3f31c18b5b, 1, 1)
    #if !CAPNP_LITE
    static constexpr ::capnp::_::RawBrandedSchema const* brand = &schema->defaultBrand;
    #endif  // !CAPNP_LITE
  };
};

// =======================================================================================

class CommandFormat::Reader {
public:
  typedef CommandFormat Reads;

  Reader() = default;
  inline explicit Reader(::capnp::_::StructReader base): _reader(base) {}

  inline ::capnp::MessageSize totalSize() const {
    return _reader.totalSize().asPublic();
  }

#if !CAPNP_LITE
  inline ::kj::StringTree toString() const {
    return ::capnp::_::structString(_reader, *_capnpPrivate::brand);
  }
#endif  // !CAPNP_LITE

  inline  ::riapsModbusUART::ModbusCommands getCommandType() const;

  inline  ::int16_t getRegisterAddress() const;

  inline  ::int16_t getNumberOfRegs() const;

  inline bool hasValues() const;
  inline  ::capnp::List< ::int16_t>::Reader getValues() const;

  inline  ::int16_t getWreadRegAddress() const;

  inline  ::int16_t getWreadNumOfRegs() const;

private:
  ::capnp::_::StructReader _reader;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::List;
  friend class ::capnp::MessageBuilder;
  friend class ::capnp::Orphanage;
};

class CommandFormat::Builder {
public:
  typedef CommandFormat Builds;

  Builder() = delete;  // Deleted to discourage incorrect usage.
                       // You can explicitly initialize to nullptr instead.
  inline Builder(decltype(nullptr)) {}
  inline explicit Builder(::capnp::_::StructBuilder base): _builder(base) {}
  inline operator Reader() const { return Reader(_builder.asReader()); }
  inline Reader asReader() const { return *this; }

  inline ::capnp::MessageSize totalSize() const { return asReader().totalSize(); }
#if !CAPNP_LITE
  inline ::kj::StringTree toString() const { return asReader().toString(); }
#endif  // !CAPNP_LITE

  inline  ::riapsModbusUART::ModbusCommands getCommandType();
  inline void setCommandType( ::riapsModbusUART::ModbusCommands value);

  inline  ::int16_t getRegisterAddress();
  inline void setRegisterAddress( ::int16_t value);

  inline  ::int16_t getNumberOfRegs();
  inline void setNumberOfRegs( ::int16_t value);

  inline bool hasValues();
  inline  ::capnp::List< ::int16_t>::Builder getValues();
  inline void setValues( ::capnp::List< ::int16_t>::Reader value);
  inline void setValues(::kj::ArrayPtr<const  ::int16_t> value);
  inline  ::capnp::List< ::int16_t>::Builder initValues(unsigned int size);
  inline void adoptValues(::capnp::Orphan< ::capnp::List< ::int16_t>>&& value);
  inline ::capnp::Orphan< ::capnp::List< ::int16_t>> disownValues();

  inline  ::int16_t getWreadRegAddress();
  inline void setWreadRegAddress( ::int16_t value);

  inline  ::int16_t getWreadNumOfRegs();
  inline void setWreadNumOfRegs( ::int16_t value);

private:
  ::capnp::_::StructBuilder _builder;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  friend class ::capnp::Orphanage;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
};

#if !CAPNP_LITE
class CommandFormat::Pipeline {
public:
  typedef CommandFormat Pipelines;

  inline Pipeline(decltype(nullptr)): _typeless(nullptr) {}
  inline explicit Pipeline(::capnp::AnyPointer::Pipeline&& typeless)
      : _typeless(kj::mv(typeless)) {}

private:
  ::capnp::AnyPointer::Pipeline _typeless;
  friend class ::capnp::PipelineHook;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
};
#endif  // !CAPNP_LITE

class ResponseFormat::Reader {
public:
  typedef ResponseFormat Reads;

  Reader() = default;
  inline explicit Reader(::capnp::_::StructReader base): _reader(base) {}

  inline ::capnp::MessageSize totalSize() const {
    return _reader.totalSize().asPublic();
  }

#if !CAPNP_LITE
  inline ::kj::StringTree toString() const {
    return ::capnp::_::structString(_reader, *_capnpPrivate::brand);
  }
#endif  // !CAPNP_LITE

  inline  ::riapsModbusUART::ModbusCommands getCommandType() const;

  inline  ::int16_t getRegisterAddress() const;

  inline  ::int16_t getNumberOfRegs() const;

  inline bool hasValues() const;
  inline  ::capnp::List< ::int16_t>::Reader getValues() const;

private:
  ::capnp::_::StructReader _reader;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::List;
  friend class ::capnp::MessageBuilder;
  friend class ::capnp::Orphanage;
};

class ResponseFormat::Builder {
public:
  typedef ResponseFormat Builds;

  Builder() = delete;  // Deleted to discourage incorrect usage.
                       // You can explicitly initialize to nullptr instead.
  inline Builder(decltype(nullptr)) {}
  inline explicit Builder(::capnp::_::StructBuilder base): _builder(base) {}
  inline operator Reader() const { return Reader(_builder.asReader()); }
  inline Reader asReader() const { return *this; }

  inline ::capnp::MessageSize totalSize() const { return asReader().totalSize(); }
#if !CAPNP_LITE
  inline ::kj::StringTree toString() const { return asReader().toString(); }
#endif  // !CAPNP_LITE

  inline  ::riapsModbusUART::ModbusCommands getCommandType();
  inline void setCommandType( ::riapsModbusUART::ModbusCommands value);

  inline  ::int16_t getRegisterAddress();
  inline void setRegisterAddress( ::int16_t value);

  inline  ::int16_t getNumberOfRegs();
  inline void setNumberOfRegs( ::int16_t value);

  inline bool hasValues();
  inline  ::capnp::List< ::int16_t>::Builder getValues();
  inline void setValues( ::capnp::List< ::int16_t>::Reader value);
  inline void setValues(::kj::ArrayPtr<const  ::int16_t> value);
  inline  ::capnp::List< ::int16_t>::Builder initValues(unsigned int size);
  inline void adoptValues(::capnp::Orphan< ::capnp::List< ::int16_t>>&& value);
  inline ::capnp::Orphan< ::capnp::List< ::int16_t>> disownValues();

private:
  ::capnp::_::StructBuilder _builder;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
  friend class ::capnp::Orphanage;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::_::PointerHelpers;
};

#if !CAPNP_LITE
class ResponseFormat::Pipeline {
public:
  typedef ResponseFormat Pipelines;

  inline Pipeline(decltype(nullptr)): _typeless(nullptr) {}
  inline explicit Pipeline(::capnp::AnyPointer::Pipeline&& typeless)
      : _typeless(kj::mv(typeless)) {}

private:
  ::capnp::AnyPointer::Pipeline _typeless;
  friend class ::capnp::PipelineHook;
  template <typename, ::capnp::Kind>
  friend struct ::capnp::ToDynamic_;
};
#endif  // !CAPNP_LITE

// =======================================================================================

inline  ::riapsModbusUART::ModbusCommands CommandFormat::Reader::getCommandType() const {
  return _reader.getDataField< ::riapsModbusUART::ModbusCommands>(
      0 * ::capnp::ELEMENTS);
}

inline  ::riapsModbusUART::ModbusCommands CommandFormat::Builder::getCommandType() {
  return _builder.getDataField< ::riapsModbusUART::ModbusCommands>(
      0 * ::capnp::ELEMENTS);
}
inline void CommandFormat::Builder::setCommandType( ::riapsModbusUART::ModbusCommands value) {
  _builder.setDataField< ::riapsModbusUART::ModbusCommands>(
      0 * ::capnp::ELEMENTS, value);
}

inline  ::int16_t CommandFormat::Reader::getRegisterAddress() const {
  return _reader.getDataField< ::int16_t>(
      1 * ::capnp::ELEMENTS);
}

inline  ::int16_t CommandFormat::Builder::getRegisterAddress() {
  return _builder.getDataField< ::int16_t>(
      1 * ::capnp::ELEMENTS);
}
inline void CommandFormat::Builder::setRegisterAddress( ::int16_t value) {
  _builder.setDataField< ::int16_t>(
      1 * ::capnp::ELEMENTS, value);
}

inline  ::int16_t CommandFormat::Reader::getNumberOfRegs() const {
  return _reader.getDataField< ::int16_t>(
      2 * ::capnp::ELEMENTS);
}

inline  ::int16_t CommandFormat::Builder::getNumberOfRegs() {
  return _builder.getDataField< ::int16_t>(
      2 * ::capnp::ELEMENTS);
}
inline void CommandFormat::Builder::setNumberOfRegs( ::int16_t value) {
  _builder.setDataField< ::int16_t>(
      2 * ::capnp::ELEMENTS, value);
}

inline bool CommandFormat::Reader::hasValues() const {
  return !_reader.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline bool CommandFormat::Builder::hasValues() {
  return !_builder.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline  ::capnp::List< ::int16_t>::Reader CommandFormat::Reader::getValues() const {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::get(
      _reader.getPointerField(0 * ::capnp::POINTERS));
}
inline  ::capnp::List< ::int16_t>::Builder CommandFormat::Builder::getValues() {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::get(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}
inline void CommandFormat::Builder::setValues( ::capnp::List< ::int16_t>::Reader value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::set(
      _builder.getPointerField(0 * ::capnp::POINTERS), value);
}
inline void CommandFormat::Builder::setValues(::kj::ArrayPtr<const  ::int16_t> value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::set(
      _builder.getPointerField(0 * ::capnp::POINTERS), value);
}
inline  ::capnp::List< ::int16_t>::Builder CommandFormat::Builder::initValues(unsigned int size) {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::init(
      _builder.getPointerField(0 * ::capnp::POINTERS), size);
}
inline void CommandFormat::Builder::adoptValues(
    ::capnp::Orphan< ::capnp::List< ::int16_t>>&& value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::adopt(
      _builder.getPointerField(0 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnp::List< ::int16_t>> CommandFormat::Builder::disownValues() {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::disown(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}

inline  ::int16_t CommandFormat::Reader::getWreadRegAddress() const {
  return _reader.getDataField< ::int16_t>(
      3 * ::capnp::ELEMENTS);
}

inline  ::int16_t CommandFormat::Builder::getWreadRegAddress() {
  return _builder.getDataField< ::int16_t>(
      3 * ::capnp::ELEMENTS);
}
inline void CommandFormat::Builder::setWreadRegAddress( ::int16_t value) {
  _builder.setDataField< ::int16_t>(
      3 * ::capnp::ELEMENTS, value);
}

inline  ::int16_t CommandFormat::Reader::getWreadNumOfRegs() const {
  return _reader.getDataField< ::int16_t>(
      4 * ::capnp::ELEMENTS);
}

inline  ::int16_t CommandFormat::Builder::getWreadNumOfRegs() {
  return _builder.getDataField< ::int16_t>(
      4 * ::capnp::ELEMENTS);
}
inline void CommandFormat::Builder::setWreadNumOfRegs( ::int16_t value) {
  _builder.setDataField< ::int16_t>(
      4 * ::capnp::ELEMENTS, value);
}

inline  ::riapsModbusUART::ModbusCommands ResponseFormat::Reader::getCommandType() const {
  return _reader.getDataField< ::riapsModbusUART::ModbusCommands>(
      0 * ::capnp::ELEMENTS);
}

inline  ::riapsModbusUART::ModbusCommands ResponseFormat::Builder::getCommandType() {
  return _builder.getDataField< ::riapsModbusUART::ModbusCommands>(
      0 * ::capnp::ELEMENTS);
}
inline void ResponseFormat::Builder::setCommandType( ::riapsModbusUART::ModbusCommands value) {
  _builder.setDataField< ::riapsModbusUART::ModbusCommands>(
      0 * ::capnp::ELEMENTS, value);
}

inline  ::int16_t ResponseFormat::Reader::getRegisterAddress() const {
  return _reader.getDataField< ::int16_t>(
      1 * ::capnp::ELEMENTS);
}

inline  ::int16_t ResponseFormat::Builder::getRegisterAddress() {
  return _builder.getDataField< ::int16_t>(
      1 * ::capnp::ELEMENTS);
}
inline void ResponseFormat::Builder::setRegisterAddress( ::int16_t value) {
  _builder.setDataField< ::int16_t>(
      1 * ::capnp::ELEMENTS, value);
}

inline  ::int16_t ResponseFormat::Reader::getNumberOfRegs() const {
  return _reader.getDataField< ::int16_t>(
      2 * ::capnp::ELEMENTS);
}

inline  ::int16_t ResponseFormat::Builder::getNumberOfRegs() {
  return _builder.getDataField< ::int16_t>(
      2 * ::capnp::ELEMENTS);
}
inline void ResponseFormat::Builder::setNumberOfRegs( ::int16_t value) {
  _builder.setDataField< ::int16_t>(
      2 * ::capnp::ELEMENTS, value);
}

inline bool ResponseFormat::Reader::hasValues() const {
  return !_reader.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline bool ResponseFormat::Builder::hasValues() {
  return !_builder.getPointerField(0 * ::capnp::POINTERS).isNull();
}
inline  ::capnp::List< ::int16_t>::Reader ResponseFormat::Reader::getValues() const {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::get(
      _reader.getPointerField(0 * ::capnp::POINTERS));
}
inline  ::capnp::List< ::int16_t>::Builder ResponseFormat::Builder::getValues() {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::get(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}
inline void ResponseFormat::Builder::setValues( ::capnp::List< ::int16_t>::Reader value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::set(
      _builder.getPointerField(0 * ::capnp::POINTERS), value);
}
inline void ResponseFormat::Builder::setValues(::kj::ArrayPtr<const  ::int16_t> value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::set(
      _builder.getPointerField(0 * ::capnp::POINTERS), value);
}
inline  ::capnp::List< ::int16_t>::Builder ResponseFormat::Builder::initValues(unsigned int size) {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::init(
      _builder.getPointerField(0 * ::capnp::POINTERS), size);
}
inline void ResponseFormat::Builder::adoptValues(
    ::capnp::Orphan< ::capnp::List< ::int16_t>>&& value) {
  ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::adopt(
      _builder.getPointerField(0 * ::capnp::POINTERS), kj::mv(value));
}
inline ::capnp::Orphan< ::capnp::List< ::int16_t>> ResponseFormat::Builder::disownValues() {
  return ::capnp::_::PointerHelpers< ::capnp::List< ::int16_t>>::disown(
      _builder.getPointerField(0 * ::capnp::POINTERS));
}

}  // namespace

#endif  // CAPNP_INCLUDED_882e1c0f1af44d05_
