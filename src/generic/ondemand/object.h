#include "simdjson/error.h"

namespace {
namespace SIMDJSON_IMPLEMENTATION {
namespace ondemand {

/**
 * A forward-only JSON object field iterator.
 */
class object {
public:
  simdjson_really_inline object() noexcept;
  simdjson_really_inline ~object() noexcept;
  simdjson_really_inline object(object &&other) noexcept;
  simdjson_really_inline object &operator=(object &&other) noexcept;
  object(const object &) = delete;
  object &operator=(const object &) = delete;

  class iterator {
  public:
    simdjson_really_inline iterator(object &o) noexcept;
    simdjson_really_inline iterator(const object::iterator &o) noexcept;
    simdjson_really_inline iterator &operator=(const object::iterator &o) noexcept;

    //
    // Iterator interface
    //

    // Reads key and value, yielding them to the user.
    simdjson_really_inline simdjson_result<field> operator*() noexcept; // MUST ONLY BE CALLED ONCE PER ITERATION.
    // Assumes it's being compared with the end. true if depth < doc->iter.depth.
    simdjson_really_inline bool operator==(const object::iterator &) noexcept;
    // Assumes it's being compared with the end. true if depth >= doc->iter.depth.
    simdjson_really_inline bool operator!=(const object::iterator &) noexcept;
    // Checks for ']' and ','
    simdjson_really_inline object::iterator &operator++() noexcept;
  private:
    object *o{};
    simdjson_really_inline iterator() noexcept;
    friend struct simdjson_result<object::iterator>;
  };

  simdjson_really_inline object::iterator begin() noexcept;
  simdjson_really_inline object::iterator end() noexcept;
  simdjson_really_inline simdjson_result<value> operator[](const std::string_view key) noexcept;

protected:
  /**
   * Begin object iteration.
   *
   * @param iter The iterator to use in the object.
   */
  static simdjson_really_inline simdjson_result<object> start(json_iterator_ref &&iter) noexcept;
  static simdjson_really_inline object started(json_iterator_ref &&iter) noexcept;

  /**
   * Object creation for non-empty objects.
   */
  simdjson_really_inline object(json_iterator_ref &&iter) noexcept;

  simdjson_really_inline error_code yield_error() noexcept;

  simdjson_really_inline error_code check_has_next() noexcept;

  /**
   * Iterator we're going to move through.
   */
  json_iterator_ref iter{};
  /**
   * Whether we are at the start.
   * 
   * PERF NOTE: this should be elided into inline control flow: it is only used for the first []
   * or * call, and SSA optimizers commonly do first-iteration loop optimization.
   */
  bool at_start{};
  /**
   * Error, if there is one. Errors are only yielded once.
   *
   * PERF NOTE: we *hope* this will be elided into control flow, as it is only used (a) in the first
   * iteration of the loop, or (b) for the final iteration after a missing comma is found in ++. If
   * this is not elided, we should make sure it's at least not using up a register. Failing that,
   * we should store it in document so there's only one of them.
   */
  error_code error{};

  friend class value;
  friend class document;
  friend struct simdjson_result<object>;
};

} // namespace ondemand
} // namespace SIMDJSON_IMPLEMENTATION
} // namespace {

namespace simdjson {

template<>
struct simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object> : public internal::simdjson_result_base<SIMDJSON_IMPLEMENTATION::ondemand::object> {
public:
  simdjson_really_inline simdjson_result(SIMDJSON_IMPLEMENTATION::ondemand::object &&value) noexcept; ///< @private
  simdjson_really_inline simdjson_result(error_code error) noexcept; ///< @private

  simdjson_really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object::iterator> begin() noexcept;
  simdjson_really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object::iterator> end() noexcept;
  simdjson_really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::value> operator[](std::string_view key) noexcept;
};

template<>
struct simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object::iterator> : public internal::simdjson_result_base<SIMDJSON_IMPLEMENTATION::ondemand::object::iterator> {
public:
  simdjson_really_inline simdjson_result() noexcept;
  simdjson_really_inline simdjson_result(SIMDJSON_IMPLEMENTATION::ondemand::object::iterator &&value) noexcept; ///< @private
  simdjson_really_inline simdjson_result(error_code error) noexcept; ///< @private
  simdjson_really_inline simdjson_result(const simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object::iterator> &a) noexcept;
  simdjson_really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object::iterator> &operator=(const simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object::iterator> &a) noexcept;

  //
  // Iterator interface
  //

  // Reads key and value, yielding them to the user.
  simdjson_really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::field> operator*() noexcept; // MUST ONLY BE CALLED ONCE PER ITERATION.
  // Assumes it's being compared with the end. true if depth < doc->iter.depth.
  simdjson_really_inline bool operator==(const simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object::iterator> &) noexcept;
  // Assumes it's being compared with the end. true if depth >= doc->iter.depth.
  simdjson_really_inline bool operator!=(const simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object::iterator> &) noexcept;
  // Checks for ']' and ','
  simdjson_really_inline simdjson_result<SIMDJSON_IMPLEMENTATION::ondemand::object::iterator> &operator++() noexcept;
};

} // namespace simdjson