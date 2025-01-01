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
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <X11/X.h>
#include <X11/keysym.h>
#include <cmath>
#include <xcb/randr.h>
#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>
#include <xcb/xcb_aux.h>
#include <xcb/xcb_cursor.h>
#include <xcb/xcb_event.h>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_image.h>
#include <xcb/xcb_xrm.h>
#include <xcb/xcb_keysyms.h>
#include <xkbcommon/xkbcommon-x11.h>
#include <xkbcommon/xkbcommon.h>



enum AtomTypes {
  WM_DELETE_WINDOW = 0,
  WM_CHANGE_STATE,
  _MOTIF_WM_HINTS,
  CLIPBOARD,
  CLIP_PROP,
  XDND_PROP,
  TARGETS,
  UTF8,
  TEXT,
  STRING,
  PLAIN_UTF8,
  PLAIN,
  URI_LIST,
  XDND_TYPE_LIST,
  XDND_AWARE,
  XDND_SELECTION,
  XDND_ENTER,
  XDND_POSITION,
  XDND_ACTION_COPY,
  XDND_ACTION_MOVE,
  XDND_ACTION_LINK,
  XDND_ACTION_ASK,
  XDND_ACTION_PRIVATE,
  XDND_STATUS,
  XDND_LEAVE,
  XDND_DROP,
  XDND_FINISHED,

  ATOMS_COUNT
};

static const char *SXcbAtomNames[27] = {
  "WM_DELETE_WINDOW",
  "WM_CHANGE_STATE",
  "_MOTIF_WM_HINTS",
  "CLIPBOARD",
  "CLIP_PROP",
  "XDND_PROP",
  "TARGETS",
  "UTF8_STRING",
  "TEXT",
  "STRING",
  "text/plain;charset=utf-8",
  "text/plain",
  "text/uri-list",
  "XdndTypeList",
  "XdndAware",
  "XdndSelection",
  "XdndEnter",
  "XdndPosition",
  "XdndActionCopy",
  "XdndActionMove",
  "XdndActionLink",
  "XdndActionAsk",
  "XdndActionPrivate",
  "XdndStatus",
  "XdndLeave",
  "XdndDrop",
  "XdndFinished"
};