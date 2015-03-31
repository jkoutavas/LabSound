/*
 * Copyright (C) 2012, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef AudioSummingJunction_h
#define AudioSummingJunction_h

#include "AudioBus.h"
#include <set>
#include <vector>
#include <array>

namespace LabSound {
    class ContextGraphLock;
    class ContextRenderLock;
}


namespace WebCore {

    class AudioContext;
    class AudioNodeOutput;
    
    using namespace LabSound;

// An AudioSummingJunction represents a point where zero, one, or more AudioNodeOutputs connect.

class AudioSummingJunction {
public:
    explicit AudioSummingJunction();
    virtual ~AudioSummingJunction();

    // This copies m_outputs to m_renderingOutputs. Please see comments for these lists below.
    // This must be called when we own the context's graph lock in the audio thread at the very start or end of the render quantum.
    void updateRenderingState(ContextRenderLock& r);

    // Rendering code accesses its version of the current connections here.
    size_t numberOfRenderingConnections() const;
    std::shared_ptr<AudioNodeOutput> renderingOutput(unsigned i) { return m_renderingOutputs[i]; }
    const std::shared_ptr<AudioNodeOutput> renderingOutput(unsigned i) const { return m_renderingOutputs[i]; }
    bool isConnected() const { return numberOfRenderingConnections() > 0; }

    virtual bool canUpdateState() = 0;
    virtual void didUpdate(ContextRenderLock&) = 0;

    void addOutput(std::shared_ptr<AudioNodeOutput>);
    void removeOutput(std::shared_ptr<AudioNodeOutput>);
    void setDirty() { m_renderingStateNeedUpdating = true; }
    
    static void markSummingJunctionDirty(std::shared_ptr<AudioSummingJunction>);
    static void handleDirtyAudioSummingJunctions(ContextRenderLock& r);
    
protected:
    // m_outputs contains the AudioNodeOutputs representing current connections which are not disabled.
    // The rendering code should never use this directly, but instead uses m_renderingOutputs.
    # define SUMMING_JUNCTION_MAX_OUTPUTS 8
    std::array<std::shared_ptr<AudioNodeOutput>, SUMMING_JUNCTION_MAX_OUTPUTS> m_outputs;

    // numberOfConnections() should never be called from the audio rendering thread.
    // Instead numberOfRenderingConnections() and renderingOutput() should be used.
    size_t numberOfConnections() const;

    // m_renderingOutputs is a copy of m_outputs which will never be modified during the graph rendering on the audio thread.
    // This is the list which is used by the rendering code.
    // Whenever m_outputs is modified, the context is told so it can later update m_renderingOutputs from m_outputs at a safe time.
    // Most of the time, m_renderingOutputs is identical to m_outputs.
    std::shared_ptr<AudioNodeOutput> m_renderingOutputs[SUMMING_JUNCTION_MAX_OUTPUTS];

    // m_renderingStateNeedUpdating indicates outputs were changed
    bool m_renderingStateNeedUpdating;
};

} // namespace WebCore

#endif // AudioSummingJunction_h
