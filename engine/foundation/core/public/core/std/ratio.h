////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2024 RacoonStudios
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
#include <ratio>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
// [ratio.ratio](http://eel.is/c++draft/ratio#ratio), class template ratio
using std::ratio;

// [ratio.arithmetic](http://eel.is/c++draft/ratio#arithmetic), ratio arithmetic
using std::ratio_add;
using std::ratio_subtract;
using std::ratio_multiply;
using std::ratio_divide;

// [ratio.comparison](http://eel.is/c++draft/ratio#comparison), ratio comparison
using std::ratio_equal;
using std::ratio_equal_v;
using std::ratio_not_equal;
using std::ratio_not_equal_v;
using std::ratio_less;
using std::ratio_less_v;
using std::ratio_less_equal;
using std::ratio_less_equal_v;
using std::ratio_greater;
using std::ratio_greater_v;
using std::ratio_greater_equal;
using std::ratio_greater_equal_v;

// [ratio.si](http://eel.is/c++draft/ratio#si), convenience SI type aliases
using std::atto;
using std::femto;
using std::pico;
using std::nano;
using std::micro;
using std::milli;
using std::centi;
using std::deci;
using std::deca;
using std::hecto;
using std::kilo;
using std::mega;
using std::giga;
using std::tera;
using std::peta;
using std::exa;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}