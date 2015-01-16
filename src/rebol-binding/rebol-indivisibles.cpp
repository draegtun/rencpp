#include <stdexcept>

#include "rencpp/values.hpp"
#include "rencpp/indivisibles.hpp"
#include "rencpp/engine.hpp"


namespace ren {

///
/// UNSET
///

bool Value::isUnset() const {
    return IS_UNSET(&cell);
}

Value::Value (unset_t, Engine * engine) :
    Value (Dont::Initialize)
{
    SET_UNSET(&cell);

    // REVIEW: Should some types not need an engine field?
    if (not engine)
        engine = &Engine::runFinder();

    finishInit(engine->getHandle());
}



///
/// NONE
///

bool Value::isNone() const {
    return IS_NONE(&cell);
}

Value::Value (none_t, Engine * engine) :
    Value (Dont::Initialize)
{
    SET_NONE(&cell);

    // REVIEW: Should some types not need an engine field?
    if (not engine)
        engine = &Engine::runFinder();

    finishInit(engine->getHandle());
}



///
/// LOGIC
///

bool Value::isLogic() const {
    return IS_LOGIC(&cell);
}

bool Value::isTrue() const {
    return isLogic() && VAL_LOGIC(&cell);
}

bool Value::isFalse() const {
    return isLogic() && !VAL_LOGIC(&cell);
}

Value::Value (bool someBool, Engine * engine) :
    Value (Dont::Initialize)
{
    SET_LOGIC(&cell, someBool);

    // REVIEW: Should some types not need an engine field?
    if (not engine)
        engine = &Engine::runFinder();

    finishInit(engine->getHandle());
}

Logic::operator bool() const {
    return VAL_INT32(&cell);
}



///
/// CHARACTER
///

bool Value::isCharacter() const {
    return IS_CHAR(&cell);
}

Value::Value (char c, Engine * engine) :
    Value (Dont::Initialize)
{
    SET_CHAR(&cell, c);

    // REVIEW: Should some types not need an engine field?
    if (not engine)
        engine = &Engine::runFinder();

    finishInit(engine->getHandle());
}

Value::Value (wchar_t wc, Engine * engine) :
    Value (Dont::Initialize)
{
    SET_CHAR(&cell, wc);

    // REVIEW: Should some types not need an engine field?
    if (not engine)
        engine = &Engine::runFinder();

    finishInit(engine->getHandle());
}

Character::operator char () const {
    REBUNI uni = VAL_CHAR(&cell);
    if (uni > 127)
        throw std::runtime_error("Non-ASCII codepoint cast to char");
    return static_cast<char>(uni);
}


Character::operator wchar_t () const {
    REBUNI uni = VAL_CHAR(&cell);
    // will throw in Red for "astral plane" unicode codepoints
    return static_cast<wchar_t>(uni);
}


long Character::codepoint() const {
    REBUNI uni = VAL_CHAR(&cell);
    // will probably not throw in Red, either
    return uni;
}


#if REN_CLASSLIB_QT
Character::operator QChar () const {
    REBUNI uni = VAL_CHAR(&cell);
    return QChar(uni);
}
#endif



///
/// INTEGER
///

bool Value::isInteger() const {
    return IS_INTEGER(&cell);
}

Value::Value (int someInt, Engine * engine) :
    Value (Dont::Initialize)
{
    SET_INTEGER(&cell, someInt);

    // REVIEW: Should some types not need an engine field?
    if (not engine)
        engine = &Engine::runFinder();

    finishInit(engine->getHandle());
}

Integer::operator int() const {
    return VAL_INT32(&cell);
}



///
/// FLOAT
///

bool Value::isFloat() const {
    return IS_DECIMAL(&cell);
}

Value::Value (double someDouble, Engine * engine) :
    Value (Dont::Initialize)
{
    SET_DECIMAL(&cell, someDouble);

    // REVIEW: Should some types not need an engine field?
    if (not engine)
        engine = &Engine::runFinder();

    finishInit(engine->getHandle());
}

Float::operator double() const {
    return VAL_DECIMAL(&cell);
}



///
/// DATE
///

bool Value::isDate() const {
    return IS_DATE(&cell);
}

} // end namespace ren