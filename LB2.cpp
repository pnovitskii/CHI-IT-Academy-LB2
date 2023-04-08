#include <iostream>
#include <sstream>
#include <type_traits>

class CustomClassThatCanConvertItselfToString {
public:
    std::string buffer = "IamBufferFromSstring";
    operator std::string() const {
        return this->buffer;
    }
};

struct CustomStringBuffer
{
public:
    CustomStringBuffer() {};

    operator std::string() const {
        return this->buffer;
    }

    template <typename T>
    CustomStringBuffer& operator+=(T& value) {
        this->buffer += "CTS:" + static_cast<std::string>(value) + " ";
        return *this;
    }

    template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, bool>>>
    CustomStringBuffer& operator+=(T value) {
        this->buffer += "Ar:" + std::to_string(value) + " ";
        return *this;
    }

    template <typename T, typename std::enable_if_t<std::is_same_v<T, bool>, long> = 0>
    CustomStringBuffer& operator+=(T value) {
        this->buffer += "B:";
        this->buffer += value ? "true" : "false";
        return *this;
    }

    std::string buffer = "";
};

int main()
{
    CustomClassThatCanConvertItselfToString Sstring;
    std::string string = "string_first";

    CustomStringBuffer SBuffer;  // buffer = ""
    SBuffer += string;           // buffer = "CTS:string_first"
    SBuffer += 42;               // buffer = "CTS:string_first Ar:42"
    SBuffer += 4.2;              // buffer = "CTS:string_first Ar:42 Ar:4.2"
    SBuffer += -42424224244242;  // buffer = "CTS:string_first Ar:42 Ar:4.2 Ar:-42424224244242"
    SBuffer += Sstring;          // buffer = "CTS:string_first Ar:42 Ar:4.2 Ar:-42424224244242 CTS:IamBufferFromSstring"
    SBuffer += false;            // buffer = "CTS:string_first Ar:42 Ar:4.2 Ar:-42424224244242 CTS:IamBufferFromSstring B:false"

    std::cout << SBuffer.buffer << std::endl;

    // CTS - Convert To String template overload
    // Ar  - Arithmetic template overload
    // B   - Boolean template overload

    return 0;
}
