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

#include "CCScriptEventDispatcher.h"
#include "script_support/CCScriptSupport.h"

NS_CC_BEGIN

int CCScriptEventDispatcher::s_nextScriptListenerHandle = 0;
CCScriptListenersForDispatcher CCScriptEventDispatcher::s_emptyListeners;
CCScriptListenersForEvent CCScriptEventDispatcher::s_emptyListenersForEvent;

CCScriptEventDispatcher::CCScriptEventDispatcher()
{
    m_scriptListeners = NULL;
}

CCScriptEventDispatcher::~CCScriptEventDispatcher()
{
    if (m_scriptListeners)
    {
        removeAllScriptEventListeners();
        delete m_scriptListeners;
    }
}

int CCScriptEventDispatcher::addScriptEventListener(int event, int listener, int tag /* = 0 */)
{
    if (!m_scriptListeners)
    {
        m_scriptListeners = new CCScriptListenersForDispatcher();
    }
    s_nextScriptListenerHandle++;
    CCScriptHandlePair p = {listener, tag};
    (*m_scriptListeners)[event][s_nextScriptListenerHandle] = p;
    return s_nextScriptListenerHandle;
}

void CCScriptEventDispatcher::removeScriptEventListener(int handle)
{
    if (!m_scriptListeners) return;
    for (CCScriptListenersForDispatcherIterator it = m_scriptListeners->begin(); it != m_scriptListeners->end(); ++it)
    {
        for (CCScriptListenersForEventIterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
        {
            if (it->first == handle)
            {
                const CCScriptHandlePair &p = it2->second;
                CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(p.listener);
                LUALOG("[LUA] Remove script event listener: %d", callback);
                it->second.erase(it2);
                return;
            }
        }
    }
}

void CCScriptEventDispatcher::removeAllScriptEventListenersForEvent(int event)
{
    if (!m_scriptListeners) return;
    CCScriptListenersForDispatcherIterator it = m_scriptListeners->find(event);
    if (it != m_scriptListeners->end())
    {
        CCScriptListenersForEvent &listeners = it->second;
        for (CCScriptListenersForEventIterator it2 = listeners.begin(); it2 != listeners.end(); ++it2)
        {
            const CCScriptHandlePair &p = it2->second;
            CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(p.listener);
            LUALOG("[LUA] Remove script event listener: %d", callback);
        }
        m_scriptListeners->erase(it);
    }
}

void CCScriptEventDispatcher::removeAllScriptEventListenersForTag(int tag)
{
    if (!m_scriptListeners) return;
    for (CCScriptListenersForDispatcherIterator it = m_scriptListeners->begin(); it != m_scriptListeners->end(); ++it)
    {
        for (CCScriptListenersForEventIterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
        {
            const CCScriptHandlePair &p = it2->second;
            if (p.tag == tag)
            {
                CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(p.listener);
                LUALOG("[LUA] Remove script event listener: %d", callback);
                it->second.erase(it2);
            }
        }
    }
}

void CCScriptEventDispatcher::removeAllScriptEventListeners()
{
    if (!m_scriptListeners) return;
    for (CCScriptListenersForDispatcherIterator it = m_scriptListeners->begin(); it != m_scriptListeners->end(); ++it)
    {
        removeAllScriptEventListenersForEvent(it->first);
    }
    m_scriptListeners->clear();
}

bool CCScriptEventDispatcher::hasScriptEventListener(int event)
{
    if (!m_scriptListeners) return false;
    return m_scriptListeners->find(event) != m_scriptListeners->end();
}

CCScriptListenersForEvent &CCScriptEventDispatcher::getScriptListenersForEvent(int event) const
{
    CCScriptListenersForDispatcherIterator it = m_scriptListeners->find(event);
    return it != m_scriptListeners->end() ? it->second : s_emptyListenersForEvent;
}

CCScriptListenersForDispatcher &CCScriptEventDispatcher::getScriptListenersForDispatcher() const
{
    return m_scriptListeners ? *m_scriptListeners : s_emptyListeners;
}

NS_CC_END
