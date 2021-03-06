#ifndef RENCPP_ERROR_HPP
#define RENCPP_ERROR_HPP

//
// error.hpp
// This file is part of RenCpp
// Copyright (C) 2015 HostileFork.com
//
// Licensed under the Boost License, Version 1.0 (the "License")
//
//      http://www.boost.org/LICENSE_1_0.txt
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied.  See the License for the specific language governing
// permissions and limitations under the License.
//
// See http://rencpp.hostilefork.com for more information on this project
//

#include <exception>

#include "value.hpp"


namespace ren {


///
/// ERROR VALUE
///

//
// Ren has its own competing "exception"-like type called ERROR!.  And
// if you throw a C++ exception, there is no way for the Ren runtime to
// catch it.  And in fact, "throw" and "catch" are distinct from Rebol's
// notion of "trying" and "raising" an error:
//
//     http://stackoverflow.com/questions/24412153/
//
// One way to get the runtime to "raise" an error is to apply it:
//
//     ren::Error myerror {"Invalid hedgehog found"};
//     myerror.apply();
//     throw "Unreachable Code"; // make compiler happy?
//
// That should within the guts of apply end up throwing a ren::evaluation_error
// with the error object inside of it.  Those are derived from std::exception
// for proper C++ error handling.
//
// However, if you know yourself to be writing code that is inside of
// a ren::Function, a shorthand is provided in the form of:
//
//     throw ren::Error {"Invalid hedgehog found"};
//
// Because C++ throws cannot be caught by Ren runtime's CATCH, the meaning
// chosen for throwing an error object is effectively to "raise" an error, as
// if you had written:
//
//     ren::runtime("do make error! {Invalid Hedgehog found}");
//
// Yet you should not throw other value types; they will be handled as
// exceptions if you do.  And when using this convenience, remember that
// throwing an exception intended to be caught directly by C++ that isn't
// derived from std::exception is a poor practice:
//
//    http://stackoverflow.com/questions/1669514/
//
// So if you are writing code that may-or-may-not be inside of a ren::Function,
// consider throwing a ren::evaluation_error instead of the error directly.
// That is "universal", and can be processed both by ren::Function as well as
// in the typical C++ execution stack.
//

class Error : public Value {
protected:
    friend class Value;
    Error (Dont) : Value (Dont::Initialize) {}
    inline bool isValid() const { return isError(); }

public:
    Error (const char * msg, Engine * engine = nullptr);
};


#ifdef REN_RUNTIME
class evaluation_error : public std::exception {
private:
    Error errorValue;
    std::string whatString;

public:
    evaluation_error (Error const & error) :
        errorValue (error),
        whatString (to_string(errorValue))
    {
    }

    char const * what() const noexcept override {
        return whatString.c_str();
    }

    Error error() const noexcept {
        return errorValue;
    }
};
#endif


//
// The question of when the binding in C++ code should throw the "same
// error as the runtime" for certain circumstances is one to think about.
// for instance, what if you write:
//
//     Value x;
//     if (x) { std::cout << "This needs to throw an exception." }
//
// If the runtime were running, it would say "Error: x has no Value".  But
// generating the exact same error requires some figuring.  Also, it is
// inappropriate to raise an "evaluation error".
//
// So for now, the binding doesn't attempt to mimic the runtime or use
// the same error class.  However, if a has_no_value were to bubble up
// through the runtime layer in an extension, it could be converted there.
//

class has_no_value : public std::exception {
public:
    has_no_value ()
    {
    }

    char const * what() const noexcept override {
        return "ren::has_no_value";
    }
};




///
/// CANCELLATION EXCEPTION
///

//
// Cancellation of evaluations (such as in the console with ^C) has no
// user-facing error object in the ren runtime, because it is "meta" and
// means "stop evaluating".  There is no way to "catch" it.
//
// However, when a multithreaded C++ host has an evaluator on one thread
// and requests a cancellation from another, then this exception will be
// thrown to the evaluation thread when (and if) the cancellation request
// is processed.  If running as an interpreted loop, it should (modulo
// bugs in the interpreter) always be possible to interrupt this way
// in a timely manner.
//
// What should the interface for cancellations of evaluations be?  How might
// timeouts or quotas of operations be managed?
//
// https://github.com/hostilefork/rencpp/issues/19

class evaluation_cancelled : public std::exception {
public:
    evaluation_cancelled ()
    {
    }

    char const * what() const noexcept override {
        return "ren::evaluation_cancelled";
    }
};



///
/// EXIT COMMAND
///

//
// This command corresponds to the desire expressed by C's exit(int), however
// it can be "caught" in Ren runtime by the TRY command when executing nested
// code.  If your C++ code wraps up a call to the evaluator and you catch
// this exception, it means the code running either called EXIT or QUIT
//

class exit_command : public std::exception {
private:
    int codeValue;
    std::string whatString;

public:
    exit_command (int code) :
        codeValue (code),
        whatString ("ren::exit_command("
                    + std::to_string(code)
                    + ")")
    {
    }

    char const * what() const noexcept override {
        return whatString.c_str();
    }

    int code() const noexcept {
        return codeValue;
    }
};


}

#endif
