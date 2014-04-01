/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __CCEVENT_DISPATCHER_H__
#define __CCEVENT_DISPATCHER_H__

#include <map>

#include "ccMacros.h"
#include "CCObject.h"

using namespace std;

NS_CC_BEGIN

typedef struct {
    int listener;
    int tag;
} CCScriptHandlePair;

// listener handle -> listener pair
typedef map<int, CCScriptHandlePair> CCScriptListenersForEvent;
typedef CCScriptListenersForEvent::iterator CCScriptListenersForEventIterator;

// event -> ScriptListenersForEvent
typedef map<int, CCScriptListenersForEvent> CCScriptListenersForDispatcher;
typedef CCScriptListenersForDispatcher::iterator CCScriptListenersForDispatcherIterator;

class CC_DLL CCScriptEventDispatcher : public CCObject
{
public:
    CCScriptEventDispatcher();
    virtual ~CCScriptEventDispatcher();
    int addScriptEventListener(int event, int listener, int tag = 0);
    void removeScriptEventListener(int handle);
    void removeAllScriptEventListenersForEvent(int event);
    void removeAllScriptEventListenersForTag(int tag);
    void removeAllScriptEventListeners();
    
    bool hasScriptEventListener(int event);
    CCScriptListenersForEvent &getScriptListenersForEvent(int event) const;
    CCScriptListenersForDispatcher &getScriptListenersForDispatcher() const;
    
private:
    CCScriptListenersForDispatcher *m_scriptListeners;
    static int s_nextScriptListenerHandle;
    static CCScriptListenersForDispatcher s_emptyListeners;
    static CCScriptListenersForEvent s_emptyListenersForEvent;
};

NS_CC_END

#endif /* defined(__CCEVENT_DISPATCHER_H__) */
