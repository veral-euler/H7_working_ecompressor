/******************************************************************************
 *                                                                            *
 *  MCUViewer Project                                                         *
 *                                                                            *
 *  (c) 2025 Piotr Wasilewski                                                 *
 *  https://mcuviewer.com                                                     *
 *                                                                            *
 *  All rights reserved.                                                      *
 *                                                                            *
 *  Permission is granted to use, modify, and distribute this file,           *
 *  in binary or source form, in private or commercial projects,              *
 *  provided that this copyright notice and disclaimer                        *
 *  are preserved without changes.                                            *
 *                                                                            *
 *  THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,           *
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO WARRANTIES OF            *
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.   *
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR          *
 *  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,     *
 *  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     *
 *  OTHER DEALINGS IN THE SOFTWARE.                                           *
 *                                                                            *
 ******************************************************************************/

#include "recorder.h"

____Recorder ____recorder =
	{
		.version = ____RECORDER_VERSION,
		.revision = ____RECORDER_REVISION,
		.timestepNs = ____RECORDER_TIMEBASE_NS,
		.maxBufferSize = ____RECORDER_BUFFERSIZE,
		.maxVariables = ____RECORDER_MAXVARS,
		.floatSupport = ____RECORDER_FLOAT_SUPPORT,

		.samplesToSkip = 0,
		.variableCount = 0,
		.maxActualBufferSize = 0,
		.samplesPackSize = 0,
		.samplesSinceStart = 0,
		.skippedSamples = 0,

		.state = ____STATE_INIT,
		.recorderbuf.head = 0,
		.recorderbuf.start = 0,
		.trigger.state = ____STATE_SETUP,
		.trigger.edge = ____EDGE_RISING,
		.trigger.type = ____UNKNOWN,
		.trigger.sourceAddress = 0,
		.trigger.preTriggerSamples = 0,
		.trigger.triggerValue = 0,
		.trigger.actValue = 0,
		.trigger.prevValue = 0,
		.trigger.triggerValueU32 = 0,
		.trigger.actValueU32 = 0,
		.trigger.prevValueU32 = 0,
#if ____RECORDER_FLOAT_SUPPORT == 1
		.trigger.triggerValueFloat = 0.0f,
		.trigger.actValueFloat = 0.0f,
		.trigger.prevValueFloat = 0.0f,
#endif
};